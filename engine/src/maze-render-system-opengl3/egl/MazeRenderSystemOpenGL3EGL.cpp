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
#include "MazeRenderSystemOpenGL3Header.hpp"


//////////////////////////////////////////
#if (  (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)       \
    || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)    \
    || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))


//////////////////////////////////////////
#include "maze-render-system-opengl3/egl/MazeRenderSystemOpenGL3EGL.hpp"
#include "maze-render-system-opengl3/egl/MazeContextOpenGL3EGL.hpp"
#include "maze-render-system-opengl3/egl/MazeExtensionsOpenGL3EGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderSystemOpenGL3EGLEGL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderSystemOpenGL3EGL, RenderSystemOpenGL3);
    
    //////////////////////////////////////////
    RenderSystemOpenGL3EGL::RenderSystemOpenGL3EGL()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3EGL::~RenderSystemOpenGL3EGL()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3EGLPtr RenderSystemOpenGL3EGL::Create(RenderSystemOpenGLConfig const& _config)
    {
        RenderSystemOpenGL3EGLPtr renderSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemOpenGL3EGL, renderSystem, init(_config));
        return renderSystem;
    }
    

    //////////////////////////////////////////
    bool RenderSystemOpenGL3EGL::init(RenderSystemOpenGLConfig const& _config)
    {
        if (!RenderSystemOpenGL3::init(_config))
            return false;

        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemOpenGL3EGL::getName()
    {
        static String s_name = "Render System GL3+ (EGL)";
        return s_name;
    }


    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL3EGL::createContextImpl(
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        return ContextOpenGL3EGL::Create(cast<RenderSystemOpenGL>(), _window, _config);
    }

    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL3EGL::createContextImpl(
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        return ContextOpenGL3EGL::Create(cast<RenderSystemOpenGL>(), _width, _height, _config);
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3EGL::assignGLFunctions(ContextOpenGLPtr const& _context)
    {
        RenderSystemOpenGL3::assignGLFunctions(_context);

        AssignFunctionsOpenGL3EGL(_context);
    }


} // namespace Maze
//////////////////////////////////////////


#endif  // if ( (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)     \
        //   || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)  \
        //   || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))
//////////////////////////////////////////
