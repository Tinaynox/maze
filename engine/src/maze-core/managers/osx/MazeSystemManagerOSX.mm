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
#include "maze-core/managers/osx/MazeSystemManagerOSX.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/osx/MazeThreadHelperOSX.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeThread)
#import <IOKit/IOKitLib.h>
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>

#import "maze-core/system/osx/MazeApplicationOSX.hpp"
#import "maze-core/system/osx/MazeApplicationDelegateOSX.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerOSX
    //
    //////////////////////////////////////////
    SystemManagerOSX::SystemManagerOSX()
        : m_applicationOSX(nullptr)
        , m_applicationDelegateOSX(nullptr) 
    {
        m_deviceCategory = DeviceCategory::Desktop;
    }
    
    //////////////////////////////////////////
    SystemManagerOSX::~SystemManagerOSX()
    {
        shutdownApplication();
    }
    
    //////////////////////////////////////////
    bool SystemManagerOSX::initApplication()
    {
        ThreadHelper::EnsureThreadHasAutoreleasePool();
        
        m_applicationOSX = [ApplicationOSX sharedApplication];
        m_applicationDelegateOSX = [[ApplicationDelegateOSX alloc] init];
        
        [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps: YES];
        
        [m_applicationDelegateOSX setSystemManager: this];
        [m_applicationOSX setDelegate: m_applicationDelegateOSX];
        
        S32 result = NSApplicationMain(m_commandLineArguments.size(), &m_commandLineArguments[0]);
        Debug::Log("NSApplicationMain ended with code %d\n", result);
        
        return true;
    }

    //////////////////////////////////////////
    void SystemManagerOSX::shutdownApplication()
    {
        if (!m_applicationDelegateOSX)
            return;
        
        [m_applicationDelegateOSX setSystemManager: nil];
        [m_applicationDelegateOSX shutdown];
        m_applicationDelegateOSX = nullptr;
    }
    
    //////////////////////////////////////////
    bool SystemManagerOSX::init(Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        if (!SystemManagerUnix::init(_commandLineArguments))
            return false;
        
        return true;
    }
    
    
    
} // namespace Maze
//////////////////////////////////////////
