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
// subject to the folloIOSg restrictions:
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
#include "maze-core/helpers/ios/MazeWindowHelperIOS.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <UIKit/UIKit.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace WindowHelper
    {
        
        //////////////////////////////////////////
        MAZE_CORE_API Vector<WindowVideoMode> GetDisplayFullscreenModes(U64 _displayHandle)
        {
            Vector<WindowVideoMode> modes;
            
            MAZE_TODO;
            
            return modes;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API Vector<WindowVideoMode> GetDisplayFullscreenModes(Display const& _display)
        {
            return GetDisplayFullscreenModes(_display.getHandle());
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API WindowVideoMode GetDisplayCurrentMode(U64 _displayHandle)
        {
            MAZE_TODO;
            
            return WindowVideoMode();
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API WindowVideoMode GetDisplayCurrentMode(Display const& _display)
        {
            return GetDisplayCurrentMode(_display.getHandle());
        }

    } // namespace WindowHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
