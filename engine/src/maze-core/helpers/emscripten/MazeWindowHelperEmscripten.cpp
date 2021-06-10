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
#include "maze-core/helpers/emscripten/MazeWindowHelperEmscripten.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace WindowHelper
    {
        //////////////////////////////////////////
        Vector<WindowVideoMode> GetDisplayFullscreenModes(String const& _name)
        {
            Vector<WindowVideoMode> modes;

            EmscriptenFullscreenChangeEvent fullscreenStatus;
            emscripten_get_fullscreen_status(&fullscreenStatus);
            
            WindowVideoMode mode;
            mode.width = fullscreenStatus.screenWidth;
            mode.height = fullscreenStatus.screenHeight;
            mode.bpp = 0;

            return modes;
        }

        //////////////////////////////////////////
        Vector<WindowVideoMode> GetDisplayFullscreenModes(Display const& _display)
        {
            Vector<WindowVideoMode> fullscreenModes;
            
            EmscriptenFullscreenChangeEvent fullscreenStatus;
            emscripten_get_fullscreen_status(&fullscreenStatus);
            
            WindowVideoMode mode;
            mode.width = fullscreenStatus.screenWidth;
            mode.height = fullscreenStatus.screenHeight;
            mode.bpp = 0;
            
            fullscreenModes.push_back(mode);
            
            
            return fullscreenModes;
        }

        //////////////////////////////////////////
        WindowVideoMode GetDisplayCurrentMode(Display const& _display)
        {
            EmscriptenFullscreenChangeEvent fullscreenStatus;
            emscripten_get_fullscreen_status(&fullscreenStatus);
            
            WindowVideoMode mode;
            mode.width = fullscreenStatus.screenWidth;
            mode.height = fullscreenStatus.screenHeight;
            mode.bpp = 0;
            
            return mode;
        }

    } // namespace WindowHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
