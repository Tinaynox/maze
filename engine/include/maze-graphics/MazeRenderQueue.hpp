//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#pragma once
#if (!defined(_MazeRenderQueue_hpp_))
#define _MazeRenderQueue_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-graphics/MazeRenderCommands.hpp"
#include "maze-graphics/MazeRenderCommandsBuffer.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamModelMatrix.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamColor.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamUV.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderQueue);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(GPUTextureBuffer);


    //////////////////////////////////////////
    enum class MAZE_GRAPHICS_API RenderQueueIndex
    {
        Opaque = 2000,
        Transparent = 3000
    };


    //////////////////////////////////////////
    struct RenderCommandDrawVAOInstanced;
       

    //////////////////////////////////////////
    // Struct RenderQueueGPUBufferInfo
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderQueueGPUBufferInfo
    {
        ////////////////////////////////////
        RenderQueueGPUBufferInfo(
            GPUTextureBufferPtr _buffer = GPUTextureBufferPtr(),
            void* _mappedPointer = nullptr);

        GPUTextureBufferPtr buffer;
        void* mappedPointer;
        Size mappedElementStart;
        Size mappedElementEnd;
    };


    //////////////////////////////////////////
    // Class RenderQueue
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderQueue
        : public SharedObject<RenderQueue>
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderQueue();

        //////////////////////////////////////////
        virtual void clear();

        //////////////////////////////////////////
        inline bool empty() const { return m_renderCommandsBuffer.empty(); }

        //////////////////////////////////////////
        void addSelectRenderPassCommand(RenderPass* _materialPass);

        //////////////////////////////////////////
        inline void addSelectRenderPassCommand(RenderPassPtr const& _materialPass)
        {
            addSelectRenderPassCommand(_materialPass.get());
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObject* _vao,
            Mat4DF const& _modelMatrix = Mat4DF::c_identity,
            Vec4DF const* _color = nullptr,
            Vec4DF const* _uv = nullptr)
        {
            bool useColorStream = (_color != nullptr);
            bool useUVStream = (_uv != nullptr);

            if (   m_lastDrawVAOInstancedCommand 
                && m_lastDrawVAOInstancedCommand->vao == _vao
                && m_lastDrawVAOInstancedCommand->count < (S32)m_maxInstancesPerDrawCall
                && useColorStream == (m_instanceStreamColor->getOffset() > 0)
                && useUVStream == (m_instanceStreamUV->getOffset() > 0))
            {
                ++m_lastDrawVAOInstancedCommand->count;
            }
            else
            {
                m_lastDrawVAOInstancedCommand = m_renderCommandsBuffer.createCommand<RenderCommandDrawVAOInstanced>(_vao);
                m_lastDrawVAOInstancedCommand->useColorStream = useColorStream;
                m_lastDrawVAOInstancedCommand->useUVStream = useUVStream;
            }

            pushInstanceModelMatrix(_modelMatrix);

            if (useColorStream)
                pushInstanceColor(*_color);

            if (useUVStream)
                pushInstanceUV(*_uv);
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObject* _vao,
            S32 _count,
            Mat4DF const* _modelMatricies,
            Vec4DF const* _colors = nullptr,
            Vec4DF const* _uvs = nullptr)
        {
            bool useColorStream = (_colors != nullptr);
            bool useUVStream = (_uvs != nullptr);

            S32 maxInstancesPerDrawCall = m_maxInstancesPerDrawCall;

            if (   m_lastDrawVAOInstancedCommand
                && m_lastDrawVAOInstancedCommand->vao == _vao
                && m_lastDrawVAOInstancedCommand->count + _count <= maxInstancesPerDrawCall
                && useColorStream == (m_instanceStreamColor->getOffset() > 0)
                && useUVStream == (m_instanceStreamUV->getOffset() > 0))
            {
                m_lastDrawVAOInstancedCommand->count += _count;

                pushInstanceModelMatricies(_modelMatricies, _count);

                if (useColorStream)
                    pushInstanceColors(_colors, _count);

                if (useUVStream)
                    pushInstanceUVs(_uvs, _count);
            }
            else
            {
                while (_count > maxInstancesPerDrawCall)
                {
                    m_lastDrawVAOInstancedCommand = m_renderCommandsBuffer.createCommand<RenderCommandDrawVAOInstanced>(_vao, maxInstancesPerDrawCall);
                    m_lastDrawVAOInstancedCommand->useColorStream = useColorStream;
                    m_lastDrawVAOInstancedCommand->useUVStream = useUVStream;

                    pushInstanceModelMatricies(_modelMatricies, maxInstancesPerDrawCall);
                    _modelMatricies += maxInstancesPerDrawCall;

                    if (useColorStream)
                    {
                        pushInstanceColors(_colors, maxInstancesPerDrawCall);
                        _colors += maxInstancesPerDrawCall;
                    }

                    if (useUVStream)
                    {
                        pushInstanceUVs(_uvs, maxInstancesPerDrawCall);
                        _uvs += maxInstancesPerDrawCall;
                    }

                    _count -= maxInstancesPerDrawCall;
                }
                
                
                m_lastDrawVAOInstancedCommand = m_renderCommandsBuffer.createCommand<RenderCommandDrawVAOInstanced>(_vao, _count);
                m_lastDrawVAOInstancedCommand->useColorStream = useColorStream;
                m_lastDrawVAOInstancedCommand->useUVStream = useUVStream;

                pushInstanceModelMatricies(_modelMatricies, _count);

                if (useColorStream)
                    pushInstanceColors(_colors, _count);

                if (useUVStream)
                    pushInstanceUVs(_uvs, _count);
            }

            
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObjectPtr const& _vao,
            Mat4DF const& _modelMatrix = Mat4DF::c_identity,
            Vec4DF const* _color = nullptr)
        {
            addDrawVAOInstancedCommand(_vao.get(), _modelMatrix, _color);
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObjectPtr const& _vao,
            S32 _count,
            Mat4DF const* _modelMatricies,
            Vec4DF const* _colors = nullptr,
            Vec4DF const* _uvs = nullptr)
        {
            addDrawVAOInstancedCommand(_vao.get(), _count, _modelMatricies, _colors, _uvs);
        }

        //////////////////////////////////////////
        inline void addClearCurrentRenderTargetCommand(
            bool _colorBuffer = true,
            bool _depthBuffer = true)
        {
            m_lastDrawVAOInstancedCommand = nullptr;
            m_renderCommandsBuffer.createCommand<RenderCommandClearCurrentRenderTarget>(_colorBuffer, _depthBuffer);
        }

        //////////////////////////////////////////
        inline void addPushScissorRectCommand(
            Rect2DF const& _scissorRect)
        {
            m_lastDrawVAOInstancedCommand = nullptr;
            MAZE_DEBUG_ERROR_IF(_scissorRect.size.x < 0.0f || _scissorRect.size.y < 0.0f, "Scissor size is cannot be negative!");
            m_renderCommandsBuffer.createCommand<RenderCommandPushScissorRect>(_scissorRect);
        }

        //////////////////////////////////////////
        inline void addPopScissorRectCommand()
        {
            m_lastDrawVAOInstancedCommand = nullptr;
            m_renderCommandsBuffer.createCommand<RenderCommandPopScissorRect>();
        }

        //////////////////////////////////////////
        inline void addEnableClipPlaneCommand(
            S32 _index,
            Vec4DF const& _plane)
        {
            m_lastDrawVAOInstancedCommand = nullptr;
            m_renderCommandsBuffer.createCommand<RenderCommandEnableClipPlane>(_index, _plane);
        }

        //////////////////////////////////////////
        inline void addDisableClipPlaneCommand(S32 _index)
        {
            m_lastDrawVAOInstancedCommand = nullptr;
            m_renderCommandsBuffer.createCommand<RenderCommandDisableClipPlane>(_index);
        }


        //////////////////////////////////////////
        virtual void draw() MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline void clearDrawCalls() { m_drawCalls = 0; }

        //////////////////////////////////////////
        S32 getDrawCalls() const { return m_drawCalls; }


        //////////////////////////////////////////
        inline void setDrawCallsLimit(S32 _drawCallsLimit) { m_drawCallsLimit = _drawCallsLimit; }

        //////////////////////////////////////////
        inline S32 getDrawCallsLimit() const { return m_drawCallsLimit; }

    protected:

        //////////////////////////////////////////
        RenderQueue();

        //////////////////////////////////////////
        virtual bool init(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void pushInstanceModelMatrix(
            Mat4DF const& _modelMatrix);

        //////////////////////////////////////////
        void pushInstanceModelMatricies(
            Mat4DF const* _modelMatricies,
            S32 _count);


        //////////////////////////////////////////
        void pushInstanceColor(
            Vec4DF const& _color);

        //////////////////////////////////////////
        void pushInstanceColors(
            Vec4DF const* _colors,
            S32 _count);


        //////////////////////////////////////////
        void pushInstanceUV(
            Vec4DF const& _uv);

        //////////////////////////////////////////
        void pushInstanceUVs(
            Vec4DF const* _uvs,
            S32 _count);

    protected:
        RenderTarget* m_renderTarget;
        RenderCommandsBuffer m_renderCommandsBuffer;

        RenderPass* m_currentRenderPass;

        RenderCommandDrawVAOInstanced* m_lastDrawVAOInstancedCommand = nullptr;

        U32 m_maxInstancesPerDrawCall;
        U32 m_maxInstancesPerDraw;

        S32 m_drawCalls;
        S32 m_drawCallsLimit;

        InstanceStreamModelMatrixPtr m_instanceStreamModelMatrix;
        InstanceStreamColorPtr m_instanceStreamColor;
        InstanceStreamUVPtr m_instanceStreamUV;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderQueue_hpp_
//////////////////////////////////////////
