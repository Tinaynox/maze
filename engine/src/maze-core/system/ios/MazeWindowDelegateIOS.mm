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
#import "maze-core/system/ios/MazeWindowDelegateIOS.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeWindowManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
// Class WindowDelegateIOS
//
//////////////////////////////////////////
@implementation WindowDelegateIOS

//////////////////////////////////////////
- (void)initialise
{
    
}

//////////////////////////////////////////
- (void)setWindow:(Maze::WindowIOS*)_window
{
    m_window = _window;
}

//////////////////////////////////////////
- (void)windowWillClose:(NSNotification*)_notification
{
    m_window->processWindowDelegateIOSWillClose();
}

//////////////////////////////////////////
- (void)windowDidBecomeKey:(NSNotification*)_notification
{
    m_window->processWindowDelegateIOSDidBecomeKey();
}

//////////////////////////////////////////
- (void)windowDidResignKey:(NSNotification*)_notification
{
    m_window->processWindowDelegateIOSDidResignKey();
}


@end
