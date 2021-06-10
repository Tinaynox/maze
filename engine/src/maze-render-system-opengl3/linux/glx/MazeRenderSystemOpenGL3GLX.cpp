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
#if (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)


//////////////////////////////////////////
#include "maze-render-system-opengl3/linux/glx/MazeRenderSystemOpenGL3GLX.hpp"
#include "maze-render-system-opengl3/linux/glx/MazeContextOpenGL3GLX.hpp"
#include "maze-render-system-opengl3/linux/glx/MazeExtensionsOpenGL3GLX.hpp"
#include "maze-core/system/linux/MazeWindowLinux.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderSystemOpenGL3GLXGLX
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderSystemOpenGL3GLX, RenderSystemOpenGL3);
    
    //////////////////////////////////////////
    RenderSystemOpenGL3GLX::RenderSystemOpenGL3GLX()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3GLX::~RenderSystemOpenGL3GLX()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3GLXPtr RenderSystemOpenGL3GLX::Create(RenderSystemOpenGLConfig const& _config)
    {
        RenderSystemOpenGL3GLXPtr renderSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemOpenGL3GLX, renderSystem, init(_config));
        return renderSystem;
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL3GLX::init(RenderSystemOpenGLConfig const& _config)
    {
        if (!RenderSystemOpenGL3::init(_config))
            return false;

        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemOpenGL3GLX::getName()
    {
        static String s_name = "Render System GL3+ (GLX)";
        return s_name;
    }


    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL3GLX::createContextImpl(
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        return ContextOpenGL3GLX::Create(cast<RenderSystemOpenGL>(), _window, _config);
    }

    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL3GLX::createContextImpl(
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        return ContextOpenGL3GLX::Create(cast<RenderSystemOpenGL>(), _width, _height, _config);
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3GLX::assignGLFunctions(ContextOpenGLPtr const& _context)
    {
        RenderSystemOpenGL3::assignGLFunctions(_context);

        AssignFunctionsOpenGL3GLX(_context);
    }

    //////////////////////////////////////////
    RenderWindowPtr RenderSystemOpenGL3GLX::createRenderWindow(RenderWindowParams const& _params)
    {
        RenderWindowParams params = _params;
        params.EnsureWindowParams()->castRaw<WindowParamsLinux>()->fetchVisualInfoCallback = &ContextOpenGL3GLX::SelectBestVisualForWindow;

        return RenderSystemOpenGL3::createRenderWindow(params);
    }


} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
//////////////////////////////////////////
