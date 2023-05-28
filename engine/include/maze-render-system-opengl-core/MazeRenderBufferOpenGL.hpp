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
#if (!defined(_MazeRenderBufferOpenGL_hpp_))
#define _MazeRenderBufferOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderBufferOpenGL);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class RenderBufferOpenGLScopeBind
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderBufferOpenGLScopeBind
    {
    public:

        //////////////////////////////////////////
        RenderBufferOpenGLScopeBind(RenderBuffer* _newRenderBuffer);

        //////////////////////////////////////////
        ~RenderBufferOpenGLScopeBind();

    private:
        ContextOpenGL* m_context;
        RenderBuffer* m_prevRenderBuffer;
    };


    //////////////////////////////////////////
    // Class RenderBufferOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderBufferOpenGL
        : public RenderBuffer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderBufferOpenGL, RenderBuffer);

    public:

        //////////////////////////////////////////
        virtual ~RenderBufferOpenGL();

        //////////////////////////////////////////
        static RenderBufferPtr Create(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        static RenderBufferPtr Create(
            RenderBufferOpenGLPtr const& _renderBuffer,
            RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>());

        //////////////////////////////////////////
        virtual RenderBufferPtr createCopy() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemOpenGL* getRenderSystemOpenGLRaw();
        

        //////////////////////////////////////////
        virtual bool setSize(Vec2DU const& _size) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool processRenderTargetWillSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetSet() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void processRenderTargetWillReset() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void* getRenderContext() MAZE_OVERRIDE { return m_context; }

        //////////////////////////////////////////
        virtual bool beginDraw() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void endDraw() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void blit(RenderBufferPtr const& _srcBuffer) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void setColorTexture(U32 _index, TexturePtr const& _texture) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setDepthTexture(TexturePtr const& _texture) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setStencilTexture(TexturePtr const& _texture) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline MZGLuint getFramebufferId() const { return m_frameBufferId; }

    protected:

        //////////////////////////////////////////
        RenderBufferOpenGL();

        //////////////////////////////////////////
        using RenderBuffer::init;
        
        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        virtual bool init(RenderBufferOpenGLPtr const& _renderBuffer);
    

        //////////////////////////////////////////
        ContextOpenGL* getContextOpenGL() const { return m_context; }


        //////////////////////////////////////////
        void setContextOpenGL(ContextOpenGL* _contextOpenGL);


        //////////////////////////////////////////
        void notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        void generateGLObjects();

        //////////////////////////////////////////
        void deleteGLObjects();

        //////////////////////////////////////////
        bool check();

        //////////////////////////////////////////
        void reloadTexture(TexturePtr const& _texture);

        //////////////////////////////////////////
        void resizeTexture(TexturePtr const& _texture, Vec2DU const& _size);

    protected:
        ContextOpenGL* m_context;

        MZGLuint m_frameBufferId;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderBufferOpenGL_hpp_
//////////////////////////////////////////
