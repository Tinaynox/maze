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
#include "maze-core/math/MazeMat4.hpp"
#include "maze-graphics/MazeRenderCommands.hpp"
#include "maze-graphics/MazeRenderCommandsBuffer.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamModelMatrix.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamColor.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamUV.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"


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
        static inline U8 ConstructUVMask(Vec4F const* _uvs[MAZE_UV_CHANNELS_MAX])
        {
            if (_uvs == nullptr)
                return 0x0;

            return
                ((_uvs[0] != nullptr) ? 1 << 0 : 0) |
                ((_uvs[1] != nullptr) ? 1 << 1 : 0) |
                ((_uvs[2] != nullptr) ? 1 << 2 : 0) |
                ((_uvs[3] != nullptr) ? 1 << 3 : 0) |
                ((_uvs[4] != nullptr) ? 1 << 4 : 0) |
                ((_uvs[5] != nullptr) ? 1 << 5 : 0) |
                ((_uvs[6] != nullptr) ? 1 << 6 : 0) |
                ((_uvs[7] != nullptr) ? 1 << 7 : 0);
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObject* _vao,
            Mat4F const& _modelMatrix = Mat4F::c_identity,
            Vec4F const* _color = nullptr,
            Vec4F const* _uvs[MAZE_UV_CHANNELS_MAX] = nullptr)
        {
            bool useColorStream = (_color != nullptr);
            U8 uvMask = ConstructUVMask(_uvs);

            if (   m_lastDrawVAOInstancedCommand 
                && m_lastDrawVAOInstancedCommand->vao == _vao
                && m_lastDrawVAOInstancedCommand->count < (S32)m_maxInstancesPerDrawCall
                && useColorStream == (m_instanceStreamColor->getOffset() > 0)
                && m_lastDrawVAOInstancedCommand->uvMask == uvMask)
            {
                ++m_lastDrawVAOInstancedCommand->count;
            }
            else
            {
                m_lastDrawVAOInstancedCommand = m_renderCommandsBuffer.createCommand<RenderCommandDrawVAOInstanced>(_vao);
                m_lastDrawVAOInstancedCommand->useColorStream = useColorStream;
                m_lastDrawVAOInstancedCommand->uvMask = uvMask;
            }

            pushInstanceModelMatrix(_modelMatrix);

            if (useColorStream)
                pushInstanceColor(*_color);

            for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                if (uvMask & (1 << i))
                    pushInstanceUV(i, *_uvs[i]);
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObject* _vao,
            S32 _count,
            Mat4F const* _modelMatricies,
            Vec4F const* _colors = nullptr,
            Vec4F const* _uvs[MAZE_UV_CHANNELS_MAX] = nullptr)
        {
            bool useColorStream = (_colors != nullptr);
            U8 uvMask = ConstructUVMask(_uvs);

            S32 maxInstancesPerDrawCall = m_maxInstancesPerDrawCall;

            if (   m_lastDrawVAOInstancedCommand
                && m_lastDrawVAOInstancedCommand->vao == _vao
                && m_lastDrawVAOInstancedCommand->count + _count <= maxInstancesPerDrawCall
                && useColorStream == (m_instanceStreamColor->getOffset() > 0)
                && m_lastDrawVAOInstancedCommand->uvMask == uvMask)
            {
                m_lastDrawVAOInstancedCommand->count += _count;

                pushInstanceModelMatricies(_modelMatricies, _count);

                if (useColorStream)
                    pushInstanceColors(_colors, _count);

                for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                    if (uvMask & (1 << i))
                        pushInstanceUV(i, _uvs[i], _count);
            }
            else
            {
                while (_count > maxInstancesPerDrawCall)
                {
                    m_lastDrawVAOInstancedCommand = m_renderCommandsBuffer.createCommand<RenderCommandDrawVAOInstanced>(_vao, maxInstancesPerDrawCall);
                    m_lastDrawVAOInstancedCommand->useColorStream = useColorStream;
                    m_lastDrawVAOInstancedCommand->uvMask = uvMask;

                    pushInstanceModelMatricies(_modelMatricies, maxInstancesPerDrawCall);
                    _modelMatricies += maxInstancesPerDrawCall;

                    if (useColorStream)
                    {
                        pushInstanceColors(_colors, maxInstancesPerDrawCall);
                        _colors += maxInstancesPerDrawCall;
                    }

                    for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                        if (uvMask & (1 << i))
                        {
                            pushInstanceUV(i, _uvs[i], maxInstancesPerDrawCall);
                            _uvs[i] += maxInstancesPerDrawCall;
                        }

                    _count -= maxInstancesPerDrawCall;
                }
                
                
                m_lastDrawVAOInstancedCommand = m_renderCommandsBuffer.createCommand<RenderCommandDrawVAOInstanced>(_vao, _count);
                m_lastDrawVAOInstancedCommand->useColorStream = useColorStream;
                m_lastDrawVAOInstancedCommand->uvMask = uvMask;

                pushInstanceModelMatricies(_modelMatricies, _count);

                if (useColorStream)
                    pushInstanceColors(_colors, _count);

                for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                    if (uvMask & (1 << i))
                        pushInstanceUV(i, _uvs[i], _count);
            }

            
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObjectPtr const& _vao,
            Mat4F const& _modelMatrix = Mat4F::c_identity,
            Vec4F const* _color = nullptr)
        {
            addDrawVAOInstancedCommand(_vao.get(), _modelMatrix, _color);
        }

        //////////////////////////////////////////
        inline void addDrawVAOInstancedCommand(
            VertexArrayObjectPtr const& _vao,
            S32 _count,
            Mat4F const* _modelMatricies,
            Vec4F const* _colors = nullptr,
            Vec4F const* _uvs[MAZE_UV_CHANNELS_MAX] = nullptr)
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
            Vec4F const& _plane)
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
            Mat4F const& _modelMatrix);

        //////////////////////////////////////////
        void pushInstanceModelMatricies(
            Mat4F const* _modelMatricies,
            S32 _count);


        //////////////////////////////////////////
        void pushInstanceColor(
            Vec4F const& _color);

        //////////////////////////////////////////
        void pushInstanceColors(
            Vec4F const* _colors,
            S32 _count);


        //////////////////////////////////////////
        void pushInstanceUV(
            S32 _index,
            Vec4F const& _uv);

        //////////////////////////////////////////
        void pushInstanceUV(
            S32 _index,
            Vec4F const* _uvs,
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
        InstanceStreamUVPtr m_instanceStreamUVs[MAZE_UV_CHANNELS_MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderQueue_hpp_
//////////////////////////////////////////
