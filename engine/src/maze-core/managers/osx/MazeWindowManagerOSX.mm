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
#include "maze-core/managers/osx/MazeWindowManagerOSX.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/helpers/osx/MazeWindowHelperOSX.hpp"
#import <AppKit/AppKit.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowManagerOSX
    //
    //////////////////////////////////////////
    WindowManagerOSX::WindowManagerOSX()
    {
    }
    
    //////////////////////////////////////////
    WindowManagerOSX::~WindowManagerOSX()
    {
    }
    
    //////////////////////////////////////////
    bool WindowManagerOSX::init()
    {
        if (!WindowManagerUnix::init())
            return false;
        
        updateDisplaysList();
        
        return true;
    }
    
    //////////////////////////////////////////
    void WindowManagerOSX::updateDisplaysList()
    {
        m_displays.clear();
        
        
        Size screensCount = [[NSScreen screens] count];
        for (Size i = 0; i < screensCount; ++i)
        {
            NSScreen* screen = [NSScreen screens][i];
            NSDictionary* screenDictionary = [screen deviceDescription];
            
            NSNumber* screenID = [screenDictionary objectForKey:@"NSScreenNumber"];
            CGDirectDisplayID aID = [screenID unsignedIntValue];            
            
            String displayName = "Display #" + StringHelper::ToString(aID);
            U32 displayFlags = 0;
            if (CGMainDisplayID() == aID)
                displayFlags |= Display::Flags::Primary;
            
            U64 displayHandle = aID;
            
            Vector<WindowVideoMode> fullscreenModes = WindowHelper::GetDisplayFullscreenModes(displayHandle);
            
            DisplayPtr display = Display::Create(displayName, displayHandle, displayFlags, fullscreenModes);
            display->setUserData(reinterpret_cast<void*>(aID));
            
            m_displays.push_back(display);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
