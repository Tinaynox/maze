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
#if (!defined(_MazeRenderWindowOpenGL_hpp_))
#define _MazeRenderWindowOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderWindowOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class RenderWindowOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderWindowOpenGL
        : public RenderWindow
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderWindowOpenGL, RenderWindow);

    public:

        //////////////////////////////////////////
        virtual ~RenderWindowOpenGL();

        //////////////////////////////////////////
        static RenderWindowOpenGLPtr Create(RenderWindowParams const& _params);

        //////////////////////////////////////////
        ContextOpenGLPtr const& getContext() const { return m_context; }


        //////////////////////////////////////////
        virtual void swapBuffers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool beginDraw() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool processRenderTargetWillSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetSet() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void processRenderTargetWillReset() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void* getRenderContext() MAZE_OVERRIDE { return m_context.get(); }

        //////////////////////////////////////////
        ContextOpenGLPtr const& getContextOpenGL() const { return m_context; }

        //////////////////////////////////////////
        void ensureContext(S32 _antialiasingLevel = 0);

        //////////////////////////////////////////
        void destroyContext();


        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderWindowOpenGL();

        //////////////////////////////////////////
        virtual bool init(RenderWindowParams const& _params) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void notifyWindowWillClose(Window* _window) MAZE_OVERRIDE;
    
    protected:
        ContextOpenGLPtr m_context;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWindowOpenGL_hpp_
//////////////////////////////////////////
