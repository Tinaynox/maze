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
#if (!defined(_MazeContextOpenGL3WGL_hpp_))
#define _MazeContextOpenGL3WGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeContextOpenGL3.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeFunctionsOpenGL3WGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenGL3WGL);
    MAZE_USING_SHARED_PTR(ExtensionsOpenGL3WGL);


    //////////////////////////////////////////
    // Class ContextOpenGL3WGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API ContextOpenGL3WGL 
        : public ContextOpenGL3
    {
    public:

        //////////////////////////////////////////
        static ContextOpenGL3WGLPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        static ContextOpenGL3WGLPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width, U32 _height,
            ContextOpenGLConfig* _config = nullptr);


        //////////////////////////////////////////
        virtual ~ContextOpenGL3WGL();


        //////////////////////////////////////////
        virtual void flushConfig() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void destroyGLContext() MAZE_OVERRIDE;

        
        //////////////////////////////////////////
        inline HDC getDeviceContext() const { return m_deviceContext; }

        //////////////////////////////////////////
        inline HGLRC getOpenGLContext() const { return m_openGLContext; }

        //////////////////////////////////////////
        inline HWND getWindowHandle() const { return m_windowHandle; }

        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync);

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
        
    
    protected:

        //////////////////////////////////////////
        ContextOpenGL3WGL();

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
        void createSurface(U32 _width, U32 _height, U32 _bitsPerPixel);

        //////////////////////////////////////////
        void createSurface(HWND _handle, U32 _bitsPerPixel);

        //////////////////////////////////////////
        void createWGLContext();

        //////////////////////////////////////////
        void createExtensions();

        //////////////////////////////////////////
        S32 findBestPixelFormat(
            HDC _deviceContext,
            U32 _bitsPerPixel,
            const ContextOpenGLConfig& _config,
            bool _pbuffer = false);

        //////////////////////////////////////////
        void selectBestDevicePixelFormat(U32 _bitsPerPixel);

        //////////////////////////////////////////
        void setDevicePixelFormat(S32 _format);

        //////////////////////////////////////////
        void updateSettingsFromPixelFormat();

        
        //////////////////////////////////////////
        virtual bool createGLContextWindow() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool createGLContextPixelBuffer() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool makeCurrent() MAZE_OVERRIDE;

        //////////////////////////////////////////
        ExtensionsOpenGL3WGL* getAnyExtensions();

    protected:

        // Handle to a pbuffer if one was created
        HPBUFFERARB m_pbuffer;

        // Device context associated to the context
        HDC m_deviceContext;

        // OpenGL context
        HGLRC m_openGLContext;

        // Window to which the context is attached
        HWND m_windowHandle; 

        // Do we own the target window?
        bool m_ownsWindow;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenGL3WGL_hpp_
//////////////////////////////////////////
