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
#include "maze-core/system/osx/MazeApplicationDelegateOSX.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemManager)


//////////////////////////////////////////
@implementation ApplicationDelegateOSX

@synthesize m_updateTimer;


//////////////////////////////////////////
- (void)applicationDidFinishLaunching:(NSNotification*)_notification
{
    m_updateTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(0.016f)
                                               target:self
                                               selector:@selector(updateTimerCallback:)
                                               userInfo:nil
                                               repeats:YES];
    
    
    m_systemManager->processApplicationInit();
}


//////////////////////////////////////////
- (void)setSystemManager:(Maze::SystemManagerOSX*)_systemManager
{
    m_systemManager = _systemManager;
}


//////////////////////////////////////////
- (void)updateTimerCallback:(id)_sender
{
    if (m_systemManager)
        m_systemManager->processApplicationFrame();
}


//////////////////////////////////////////
- (void)shutdown
{
    exit(0);
}


//////////////////////////////////////////
- (void)applicationWillTerminate:(NSNotification*)_notification
{
    m_updateTimer = nil;
}


//////////////////////////////////////////
- (void)dealloc
{
    if (m_updateTimer)
    {
        [m_updateTimer invalidate];
        m_updateTimer = nil;
    }
    
    [super dealloc];
}

@end

