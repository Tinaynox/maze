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
#include "maze-core/helpers/win/MazeWindowHelperWin.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <ShlObj.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace WindowHelper
    {
        
        //////////////////////////////////////////
        MAZE_CORE_API Vector<WindowVideoMode> GetDisplayFullscreenModes(String const& _name)
        {
            Vector<WindowVideoMode> modes;

            DEVMODE win32Mode;
            win32Mode.dmSize = sizeof(win32Mode);
            for (S32 count = 0; EnumDisplaySettings(_name.c_str(), count, &win32Mode); ++count)
            {
                WindowVideoMode mode(win32Mode.dmPelsWidth, win32Mode.dmPelsHeight, win32Mode.dmBitsPerPel);

                if (std::find(modes.begin(), modes.end(), mode) == modes.end())
                    modes.push_back(mode);
            }

            std::sort(modes.begin(), modes.end());

            return modes;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Vector<WindowVideoMode> GetDisplayFullscreenModes(Display const& _display)
        {
            return GetDisplayFullscreenModes(_display.getName());
        }

        //////////////////////////////////////////
        MAZE_CORE_API WindowVideoMode GetDisplayCurrentMode(Display const& _display)
        {
            DEVMODE win32Mode;
            win32Mode.dmSize = sizeof(win32Mode);
            EnumDisplaySettings(_display.getName().c_str(), ENUM_CURRENT_SETTINGS, &win32Mode);

            return WindowVideoMode(win32Mode.dmPelsWidth, win32Mode.dmPelsHeight, win32Mode.dmBitsPerPel);
        }

    } // namespace WindowHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
