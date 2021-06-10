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
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)


//////////////////////////////////////////
#include "maze-render-system-opengl3/win/wgl/MazeRenderSystemOpenGL3WGL.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeContextOpenGL3WGL.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeExtensionsOpenGL3WGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderSystemOpenGL3WGLWGL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderSystemOpenGL3WGL, RenderSystemOpenGL3);

    //////////////////////////////////////////
    RenderSystemOpenGL3WGL::RenderSystemOpenGL3WGL()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3WGL::~RenderSystemOpenGL3WGL()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3WGLPtr RenderSystemOpenGL3WGL::Create(RenderSystemOpenGLConfig const& _config)
    {
        RenderSystemOpenGL3WGLPtr renderSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemOpenGL3WGL, renderSystem, init(_config));
        return renderSystem;
    }
    

    //////////////////////////////////////////
    bool RenderSystemOpenGL3WGL::init(RenderSystemOpenGLConfig const& _config)
    {
        if (!RenderSystemOpenGL3::init(_config))
            return false;

        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemOpenGL3WGL::getName()
    {
        static String s_name = "Render System GL3+ (WGL)";
        return s_name;
    }


    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL3WGL::createContextImpl(
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        return ContextOpenGL3WGL::Create(cast<RenderSystemOpenGL>(), _window, _config);
    }

    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL3WGL::createContextImpl(
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        return ContextOpenGL3WGL::Create(cast<RenderSystemOpenGL>(), _width, _height, _config);
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3WGL::assignGLFunctions(ContextOpenGLPtr const& _context)
    {
        RenderSystemOpenGL3::assignGLFunctions(_context);

        AssignFunctionsOpenGL3WGL(_context);
    }


} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
//////////////////////////////////////////
