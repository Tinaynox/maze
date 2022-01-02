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
#if (!defined(_MazeContextOpenGL3NS_hpp_))
#define _MazeContextOpenGL3NS_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeContextOpenGL3.hpp"
#include "maze-render-system-opengl3/osx/ns/MazeFunctionsOpenGL3NS.hpp"


//////////////////////////////////////////
#ifdef __OBJC__
#   import <AppKit/AppKit.h>
#endif


//////////////////////////////////////////
#ifdef __OBJC__

@class NSOpenGLContext;
using NSOpenGLContextRef = NSOpenGLContext*;

@class NSOpenGLView;
using NSOpenGLViewRef = NSOpenGLView*;

@class NSWindow;
using NSWindowRef = NSWindow*;

#else

using NSOpenGLContextRef = void*;
using NSOpenGLViewRef = void*;
using NSWindowRef = void*;

#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenGL3NS);


    //////////////////////////////////////////
    // Class ContextOpenGL3NS
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API ContextOpenGL3NS 
        : public ContextOpenGL3
    {
    public:

        //////////////////////////////////////////
        static ContextOpenGL3NSPtr Create( 
            RenderSystemOpenGLPtr const& _renderSystem,
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr );

        //////////////////////////////////////////
        static ContextOpenGL3NSPtr Create( 
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width, U32 _height,
            ContextOpenGLConfig* _config = nullptr );


        //////////////////////////////////////////
        virtual ~ContextOpenGL3NS();


        //////////////////////////////////////////
        virtual void flushConfig() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void destroyGLContext() MAZE_OVERRIDE;

        
        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;
        
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

        
    protected:

        //////////////////////////////////////////
        ContextOpenGL3NS();

        //////////////////////////////////////////
        virtual bool init( 
            RenderSystemOpenGLPtr const& _renderSystem,
            const WindowPtr& window,
            ContextOpenGLConfig* config = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 width = 1, U32 height = 1,
            ContextOpenGLConfig* config = nullptr) MAZE_OVERRIDE;

        
        //////////////////////////////////////////
        virtual bool createGLContextWindow() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool createGLContextPixelBuffer() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool makeCurrent() MAZE_OVERRIDE;
        
        
        //////////////////////////////////////////
        void createNSContext(U32 _bpp, ContextOpenGLConfig const& _config);
        
        //////////////////////////////////////////
        void createExtensions();

    protected:

        NSOpenGLContextRef    m_context;
        NSOpenGLViewRef       m_view;
        NSWindowRef           m_window;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenGL3NS_hpp_
//////////////////////////////////////////
