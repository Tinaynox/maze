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
#include "maze-core/managers/ios/MazeSystemManagerIOS.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/ios/MazeThreadHelperIOS.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeThread)
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>

#import "maze-core/system/ios/MazeApplicationIOS.hpp"
#import "maze-core/system/ios/MazeApplicationDelegateIOS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerIOS
    //
    //////////////////////////////////////////
    SystemManagerIOS::SystemManagerIOS()
    {
        m_deviceCategory = DeviceCategory::Phone;
    }
    
    //////////////////////////////////////////
    SystemManagerIOS::~SystemManagerIOS()
    {
    }
    
    //////////////////////////////////////////
    bool SystemManagerIOS::initApplication()
    {        
        ThreadHelper::EnsureThreadHasAutoreleasePool();
            
        ApplicationIOS* applicationIOS = static_cast<ApplicationIOS*>([ApplicationIOS sharedApplication]);
        ApplicationDelegateIOS* applicationDelegateIOS = [[ApplicationDelegateIOS alloc] init];
        [applicationIOS setDelegate: applicationDelegateIOS];
        
        
        S32 result = UIApplicationMain((S32)m_commandLineArguments.size(), (S8**)&m_commandLineArguments[0], @"ApplicationIOS", @"ApplicationDelegateIOS");
        Debug::Log("UIApplicationMain ended with code %d\n", result);
    
        return true;
    }
    
    //////////////////////////////////////////
    bool SystemManagerIOS::init(Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        if (!SystemManagerUnix::init(_commandLineArguments))
            return false;
        
        return true;
    }

    ////////////////////////////////////
    void SystemManagerIOS::processApplicationWillTerminate()
    {
    }
    
} // namespace Maze
//////////////////////////////////////////
