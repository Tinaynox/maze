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
#if (!defined(_MazeContextOpenGL3EGL_hpp_))
#define _MazeContextOpenGL3EGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeContextOpenGL3.hpp"
#include "maze-render-system-opengl3/egl/MazeFunctionsOpenGL3EGL.hpp"
#include <EGL/egl.h>


//////////////////////////////////////////
#if (MAZE_DEBUG_GL)
#    define MAZE_EGL_CALL(x) do\
                             {\
                                 x;\
                                 Maze::EGLCheckOpenGLError(__FILE__, __LINE__, #x);\
                             }\
                             while (false)
#else
#    define MAZE_EGL_CALL(x) x
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL3_API bool EGLCheckOpenGLError(CString _file, U32 _line, CString _expression);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL3_API EGLNativeWindowType FetchNativeWindowHandle(WindowPtr const& _window);

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenGL3EGL);


    //////////////////////////////////////////
    // Class ContextOpenGL3EGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API ContextOpenGL3EGL 
        : public ContextOpenGL3
    {
    public:

        //////////////////////////////////////////
        static ContextOpenGL3EGLPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        static ContextOpenGL3EGLPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width, U32 _height,
            ContextOpenGLConfig* _config = nullptr);


        //////////////////////////////////////////
        virtual ~ContextOpenGL3EGL();


        //////////////////////////////////////////
        virtual void flushConfig() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void destroyGLContext() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void ensureIfCurrent();

        //////////////////////////////////////////
        virtual bool _validateIsCurrentGLContext() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool _validateIsCurrentGLContextAndSurface() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isSameGLContext(ContextOpenGL* _context) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isSameGLSurface(ContextOpenGL* _context) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GlFunctionPointer getFunction(CString _name) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void swapBuffers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        inline EGLDisplay getEGLDisplay() const { return m_eglDisplay; }

        //////////////////////////////////////////
        inline EGLContext getEGLContext() const { return m_eglContext; }

        //////////////////////////////////////////
        inline EGLSurface getEGLSurface() const { return m_eglSurface; }

        //////////////////////////////////////////
        inline EGLConfig getEGLConfig() const { return m_eglConfig; }

    protected:

        //////////////////////////////////////////
        ContextOpenGL3EGL();

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
        virtual bool makeCurrent() MAZE_OVERRIDE;


        //////////////////////////////////////////
        EGLConfig fetchBestEGLConfig(
            EGLDisplay _display,
            U32 _bitsPerPixel,
            ContextOpenGLConfig& _config);

        //////////////////////////////////////////
        void createEGLContext();

        //////////////////////////////////////////
        void createExtensions();

        //////////////////////////////////////////
        void createSurface(EGLNativeWindowType _window);

        //////////////////////////////////////////
        void destroySurface();

    protected:
        EGLDisplay m_eglDisplay;
        EGLContext m_eglContext;
        EGLSurface m_eglSurface;
        EGLConfig m_eglConfig;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenGL3EGL_hpp_
//////////////////////////////////////////
