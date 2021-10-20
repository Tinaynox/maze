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
#include "maze-render-system-opengl3/linux/glx/MazeExtensionsOpenGL3GLX.hpp"
#include "maze-render-system-opengl3/linux/glx/MazeContextOpenGL3GLX.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ExtensionsOpenGL3GLX
    //
    //////////////////////////////////////////
    ExtensionsOpenGL3GLX::ExtensionsOpenGL3GLX()
    {
    }

    //////////////////////////////////////////
    ExtensionsOpenGL3GLX::~ExtensionsOpenGL3GLX()
    {
    }

    //////////////////////////////////////////
    ExtensionsOpenGL3GLXPtr ExtensionsOpenGL3GLX::Create(ContextOpenGL* _context)
    {
        ExtensionsOpenGL3GLXPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ExtensionsOpenGL3GLX, object, init(_context));
        return object;
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL3GLX::init(ContextOpenGL* _context)
    {
        if (!ExtensionsOpenGL3::init(_context))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL3GLX::loadGLExtensionsImpl()
    {
        if (!ExtensionsOpenGL3::loadGLExtensionsImpl())
            return false;

        return true;
    }


} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
//////////////////////////////////////////
