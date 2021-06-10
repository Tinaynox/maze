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
#if (!defined(_MazeContextOpenGL3GLX_hpp_))
#define _MazeContextOpenGL3GLX_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeContextOpenGL3.hpp"
#include "maze-render-system-opengl3/linux/glx/MazeFunctionsOpenGL3GLX.hpp"
#include <glad/glx.h>
#include <X11/Xlib.h>


//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)

#    if (defined(None))
#        undef None
#    endif

#endif // (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
//////////////////////////////////////////


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenGL3GLX);
    MAZE_USING_SHARED_PTR(WindowLinux);
    

    //////////////////////////////////////////
    // Class ContextOpenGL3GLX
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API ContextOpenGL3GLX
        : public ContextOpenGL3
    {
    public:

        //////////////////////////////////////////
        static ContextOpenGL3GLXPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        static ContextOpenGL3GLXPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width, U32 _height,
            ContextOpenGLConfig* _config = nullptr);


        //////////////////////////////////////////
        virtual ~ContextOpenGL3GLX();


        //////////////////////////////////////////
        virtual void flushConfig() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void destroyGLContext() MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync);

        //////////////////////////////////////////
        void ensureIfCurrent();

        //////////////////////////////////////////
        virtual bool _validateIsCurrent() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GlFunctionPointer getFunction(CString _name) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void swapBuffers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;


        //////////////////////////////////////////
        ::Display* getDisplay() const { return m_xDisplay; }

        //////////////////////////////////////////
        ::Window getWindow() const { return m_xWindow; }

        //////////////////////////////////////////
        GLXContext getGLXContext() const { return m_glxContext; }


        //////////////////////////////////////////
        static GlFunctionPointer GetFunction(CString _name);

        //////////////////////////////////////////
        static XVisualInfo SelectBestVisual(
            ::Display* _xDisplay,
            U32 _bitsPerPixel,
            ContextOpenGLConfig const& _config);

        //////////////////////////////////////////
        static XVisualInfo SelectBestVisualForWindow(
            WindowLinux* _windowLinux);

    protected:

        //////////////////////////////////////////
        ContextOpenGL3GLX();

        //////////////////////////////////////////
        virtual bool init(
            RenderSystemOpenGLPtr const& _renderSystem,
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width = 1, U32 _height = 1,
            ContextOpenGLConfig* _config = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool createGLContextWindow() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool createGLContextPixelBuffer() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool createGLContextWindow() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool createGLContextPixelBuffer() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool makeCurrent() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void flushConfigFromWindow();

        //////////////////////////////////////////
        void flushConfigFromVisualInfo(XVisualInfo* _visualInfo);

        //////////////////////////////////////////
        void createSurface(::Window _window);

        //////////////////////////////////////////
        void createSurface(
            U32 _width,
            U32 _height,
            U32 _bitsPerPixel);

        //////////////////////////////////////////
        void createGLXContext();
        
        //////////////////////////////////////////
        void createExtensions();


    protected:
        ::Display*        m_xDisplay;
        ::Window          m_xWindow;
        GLXContext        m_glxContext;
        GLXPbuffer        m_glxPbuffer;
        bool              m_ownsWindow;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenGL3GLX_hpp_
//////////////////////////////////////////
