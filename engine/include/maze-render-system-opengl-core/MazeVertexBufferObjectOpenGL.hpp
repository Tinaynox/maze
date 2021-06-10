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
#if (!defined(_MazeVertexBufferOpenGL_hpp_))
#define _MazeVertexBufferOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class VertexBufferObjectMappingControllerOpenGL;

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexBufferObjectOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Struct VertexBufferObjectBlockOpenGL
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_OPENGL_CORE_API VertexBufferObjectBlockOpenGL
    {
        //////////////////////////////////////////
        // Struct VertexBufferObjectBlockOpenGL
        //
        //////////////////////////////////////////
        struct PaddingInfo
        {
            //////////////////////////////////////////
            PaddingInfo()
                : offsetAfterPadding(0)
                , padding(0)
            {}

            //////////////////////////////////////////
            PaddingInfo(
                Size _offsetAfterPadding,
                Size _padding)
                : offsetAfterPadding(_offsetAfterPadding)
                , padding(_padding)
            {}

            //////////////////////////////////////////
            inline bool operator()(
                PaddingInfo const& _left,
                Size _right) const
            {
                return _left.offsetAfterPadding < _right;
            }

            //////////////////////////////////////////
            inline bool operator()(
                Size _left,
                PaddingInfo const& _right) const
            {
                return _left < _right.offsetAfterPadding;
            }

            //////////////////////////////////////////
            inline bool operator()(
                PaddingInfo const& _left,
                PaddingInfo const& _right) const
            {
                return _left.offsetAfterPadding < _right.offsetAfterPadding;
            }

            Size offsetAfterPadding;
            Size padding;
        };

        //////////////////////////////////////////
        VertexBufferObjectBlockOpenGL(
            Size _offset,
            Size _size)
            : offset(_offset)
            , size(_size)
        {
        }

        Size offset;
        Size size;
    };


    //////////////////////////////////////////
    // Class VertexBufferObjectOpenGLScopeBind
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API VertexBufferObjectOpenGLScopeBind
    {
    public:

        //////////////////////////////////////////    
        VertexBufferObjectOpenGLScopeBind(
            RenderSystemOpenGL* _rs,
            MZGLuint _newVBO);

        //////////////////////////////////////////    
        VertexBufferObjectOpenGLScopeBind(VertexBufferObjectOpenGL* _newVBO);

        //////////////////////////////////////////
        VertexBufferObjectOpenGLScopeBind(VertexBufferObjectOpenGL const& _newVBO);

        //////////////////////////////////////////
        inline VertexBufferObjectOpenGLScopeBind(VertexBufferObjectOpenGLPtr const& _newVBO)
            : VertexBufferObjectOpenGLScopeBind(_newVBO.get())
        {}

        //////////////////////////////////////////
        ~VertexBufferObjectOpenGLScopeBind();

    private:
        ContextOpenGL* m_context;

        MZGLuint m_prevVBO;
    };


    //////////////////////////////////////////
    // Class VertexBufferObjectOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API VertexBufferObjectOpenGL
        : public VertexBufferObject
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~VertexBufferObjectOpenGL();

        //////////////////////////////////////////
        static VertexBufferObjectOpenGLPtr Create(
            RenderSystem* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping,
            ContextOpenGL* _contextOpenGL);

        
        //////////////////////////////////////////
        inline ContextOpenGL* getContextOpenGL() const { return m_context; }

        //////////////////////////////////////////
        inline RenderSystemOpenGL* getRenderSystemOpenGLRaw() const { return m_renderSystem->castRaw<RenderSystemOpenGL>(); }


        //////////////////////////////////////////
        inline MZGLuint getGLVBO() const { return m_glVBO; }

        //////////////////////////////////////////
        Size getSizeBytes() const { return m_sizeBytes; }

        
        //////////////////////////////////////////
        inline VertexBufferObjectMappingControllerOpenGL* getMappingController() const { return m_mappingController; }

        //////////////////////////////////////////
        virtual void resize(Size _bytes) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(
            void const* _data,
            Size _bytes) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool findSuitableBlock(
            Size _sizeBytes,
            Size _alignment,
            Size& _outBestBlockIndex,
            bool& _outMatchingStride) const;


        //////////////////////////////////////////
        VertexBufferObjectBlockOpenGL& getFreeBlock(Size _index) { return m_freeBlocks.at(_index); }

        //////////////////////////////////////////
        void addFreeBlock(
            Size _bufferOffset,
            Size _sizeBytes);

        //////////////////////////////////////////
        void eraseFreeBlock(Size _index)
        {
            m_freeBlocks.erase(m_freeBlocks.begin() + _index);
        };

        //////////////////////////////////////////
        void addPaddingInfo(
            Size _offsetAfterPadding,
            Size _padding);

        //////////////////////////////////////////
        bool erasePaddingInfo(
            Size _offsetAfterPadding,
            VertexBufferObjectBlockOpenGL::PaddingInfo& _outPaddingInfo);

    protected:

        //////////////////////////////////////////
        VertexBufferObjectOpenGL();

        //////////////////////////////////////////
        using VertexBufferObject::init;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping,
            ContextOpenGL* _contextOpenGL);


        //////////////////////////////////////////
        void generateGLObjects();

        //////////////////////////////////////////
        void deleteGLObjects();
    

        //////////////////////////////////////////
        void notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void setContextOpenGL(ContextOpenGL* _contextOpenGL);

    protected:
        ContextOpenGL* m_context;

        MZGLuint m_glVBO;
        Size m_sizeBytes;

        Vector<VertexBufferObjectBlockOpenGL> m_freeBlocks;
        Vector<VertexBufferObjectBlockOpenGL::PaddingInfo> m_usedPadding;

        VertexBufferObjectMappingControllerOpenGL* m_mappingController;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexBufferOpenGL_hpp_
//////////////////////////////////////////
