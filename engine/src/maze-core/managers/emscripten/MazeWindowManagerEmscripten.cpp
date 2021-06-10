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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/emscripten/MazeWindowManagerEmscripten.hpp"
#include "maze-core/system/emscripten/MazeWindowEmscripten.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowManagerEmscripten
    //
    //////////////////////////////////////////
    WindowManagerEmscripten::WindowManagerEmscripten()
    {
    }

    //////////////////////////////////////////
    WindowManagerEmscripten::~WindowManagerEmscripten()
    {
    }

    //////////////////////////////////////////
    bool WindowManagerEmscripten::init()
    {
        if (!WindowManagerUnix::init())
            return false;

        return true;
    }
    
    //////////////////////////////////////////
    void WindowManagerEmscripten::updateDisplaysList()
    {
        
    }
    
    //////////////////////////////////////////
    bool WindowManagerEmscripten::processWindowCanBeCreated(WindowParamsPtr const& _params)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_windows.empty(), false, "You cannot create more than one window on Emscripten");

        return WindowManager::processWindowCanBeCreated(_params);
    }
    
    //////////////////////////////////////////
    bool WindowManagerEmscripten::processWindowCreated(WindowPtr const& _window)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_windows.empty(), false, "You cannot create more than one window on Emscripten");

        m_window = _window->cast<WindowEmscripten>();
        
        return WindowManager::processWindowCreated(_window);
    }

} // namespace Maze
//////////////////////////////////////////
