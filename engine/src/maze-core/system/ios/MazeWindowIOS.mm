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
#include "maze-core/system/ios/MazeWindowIOS.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include MAZE_INCLUDE_OS_FILE(managers, MazeWindowManager)
#include MAZE_INCLUDE_OS_FILE(managers, MazeSystemManager)
#include MAZE_INCLUDE_OS_FILE(system, MazeApplicationDelegate)
#include MAZE_INCLUDE_OS_FILE(system, MazeWindowDelegate)
#include MAZE_INCLUDE_OS_FILE(system, MazeView)
#include MAZE_INCLUDE_OS_FILE(system, MazeThread)
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/system/ios/MazeApplicationIOS.hpp"
#include "maze-core/system/ios/MazeViewIOS.hpp"
#include "maze-core/system/ios/MazeViewControllerIOS.hpp"
#include "maze-core/system/ios/MazeWindowDelegateIOS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowParamsIOS
    //
    //////////////////////////////////////////
    WindowParamsIOS::WindowParamsIOS()
    {
    }

    ////////////////////////////////////
    bool WindowParamsIOS::init(WindowParamsPtr const& _windowParams)
    {
        if (!WindowParams::init(_windowParams))
            return false;

        WindowParamsIOS* windowParamsIOS = _windowParams->castRaw<WindowParamsIOS>();


        return true;
    }
    
    
    //////////////////////////////////////////
    // Class WindowIOS
    //
    //////////////////////////////////////////
    WindowIOS::WindowIOS()
        : m_windowDelegateIOS(nullptr)
        , m_viewIOS(nullptr)
        , m_viewControllerIOS(nullptr)
    {
    }
    
    
    //////////////////////////////////////////
    WindowIOS::~WindowIOS()
    {
        close();
    }
    
    //////////////////////////////////////////
    bool WindowIOS::init(WindowParamsPtr const& _params)
    {
        if (!Window::init(_params))
            return false;
        
        if (!createWindowIOS())
            return false;
        
        return true;
    }
    
    //////////////////////////////////////////
    bool WindowIOS::isOpened()
    {
        return (m_windowDelegateIOS != nullptr);
    }
    
    //////////////////////////////////////////
    void WindowIOS::setClientSize(Vec2U32 const& _size)
    {
        m_params->clientSize = _size;
        updateIOSWindow();
    }
    
    //////////////////////////////////////////
    Vec2U32 WindowIOS::getClientSize()
    {
        F32 width = [m_windowDelegateIOS frame].size.width;
        F32 height = [m_windowDelegateIOS frame].size.height;
        F32 contentScaleFactor = m_viewIOS.contentScaleFactor;
        
        return Vec2U32(
            width * contentScaleFactor,
            height * contentScaleFactor);
    }
    
    //////////////////////////////////////////
    Vec2U32 WindowIOS::getFullSize()
    {
        return Vec2U32(
            [m_windowDelegateIOS frame].size.width,
            [m_windowDelegateIOS frame].size.height);
    }
    
    //////////////////////////////////////////
    void WindowIOS::setPosition(Vec2S32 const& _position )
    {
        
    }
    
    //////////////////////////////////////////
    Vec2S32 WindowIOS::getPosition()
    {
        return Vec2S32(
            [m_windowDelegateIOS frame].origin.x,
            [m_windowDelegateIOS frame].origin.y);
    }
    
    //////////////////////////////////////////
    void WindowIOS::close()
    {
        if (!m_windowDelegateIOS)
            return;
        
        [ApplicationDelegateIOS GetInstance].eventApplicationDidEnterBackground.unsubscribe(this);
        [ApplicationDelegateIOS GetInstance].eventApplicationWillEnterForeground.unsubscribe(this);
        
        m_windowDelegateIOS = nil;
        m_viewIOS = nil;    
    }
    
    //////////////////////////////////////////
    bool WindowIOS::getFocused()
    {
        return true;
    }
    
    //////////////////////////////////////////
    void WindowIOS::setFocused(bool _value)
    {
        if (_value)
        {
            [m_windowDelegateIOS makeKeyWindow];
        }
        else
        {
        }
    }
    
    //////////////////////////////////////////
    DisplayPtr const& WindowIOS::getRelatedDisplay()
    {
        static DisplayPtr nullPointer;
        
        if (!isOpened())
            return nullPointer;
        
        return nullPointer;
    }
    
    //////////////////////////////////////////
    bool WindowIOS::updateTitle()
    {
        return true;
    }
    
    //////////////////////////////////////////
    bool WindowIOS::updateWindowMode()
    {
        updateIOSWindow();
                
        return false;
    }
    
    //////////////////////////////////////////
    bool WindowIOS::updateMinimized()
    {
        return false;
    }
    
    //////////////////////////////////////////
    void WindowIOS::processOrientationChanged()
    {
        
    }
    
    
    
    //////////////////////////////////////////
    void WindowIOS::processWindowDelegateIOSWillClose()
    {
        m_windowDelegateIOS = nil;
        m_viewIOS = nil;
    }
    
    //////////////////////////////////////////
    void WindowIOS::processWindowDelegateIOSDidBecomeKey()
    {
        processWindowFocusChanged();
    }
    
    //////////////////////////////////////////
    void WindowIOS::processWindowDelegateIOSDidResignKey()
    {
        processWindowFocusChanged();
    }
    
    
    //////////////////////////////////////////
    bool WindowIOS::createWindowIOS()
    {
        close();
        
        setTitle(m_params->title);
            
        CGRect mainDisplayRect = [[UIScreen mainScreen] bounds];
        m_windowDelegateIOS = [[WindowDelegateIOS alloc] initWithFrame: mainDisplayRect];
        
        MAZE_ERROR_RETURN_VALUE_IF(!m_windowDelegateIOS, false, "Delegate is null!");
        
        [m_windowDelegateIOS setWindow: this];
        
        [m_windowDelegateIOS setOpaque: NO]; // IT'S IMPORTANT!
        [m_windowDelegateIOS initialise];

        
        CGRect viewRect = CGRectMake(
            0.0f,
            0.0f,
            [m_windowDelegateIOS frame].size.width,
            [m_windowDelegateIOS frame].size.height);
        
        
        F32 contentScaleFactor = [ApplicationDelegateIOS GetInstance].m_scaleFactor;
        m_viewIOS = [[ViewIOS alloc] initWithFrame:viewRect andScaleFactor: contentScaleFactor];
        
        
        MAZE_ERROR_RETURN_VALUE_IF(!m_viewIOS, false, "ViewIOS is null");
        [m_viewIOS setWindow: this];
        [m_viewIOS setOpaque: [m_windowDelegateIOS isOpaque]];
        [m_viewIOS setClearsContextBeforeDrawing:NO];
        
        
        m_viewControllerIOS = [[ViewControllerIOS alloc] init];
        
        [m_viewControllerIOS setWindow: this];
        [m_viewControllerIOS setView: m_viewIOS];
        
        
        [m_windowDelegateIOS setRootViewController: m_viewControllerIOS];
        [m_windowDelegateIOS setBackgroundColor: [UIColor blackColor]];
        [m_windowDelegateIOS makeKeyAndVisible];
        
        [m_viewIOS setContentScaleFactor: contentScaleFactor];
        [m_windowDelegateIOS setContentScaleFactor: contentScaleFactor];
        
    
        updateIOSWindow();
        
        
        [ApplicationDelegateIOS GetInstance].eventApplicationDidEnterBackground.subscribe(this, &WindowIOS::notifyApplicationDidEnterBackground);
        [ApplicationDelegateIOS GetInstance].eventApplicationWillEnterForeground.subscribe(this, &WindowIOS::notifyApplicationWillEnterForeground);
        
        
        processWindowCreated();
        
        return true;
    }

    //////////////////////////////////////////
    void WindowIOS::notifyApplicationDidEnterBackground()
    {
        m_params->minimized = true;
    }

    //////////////////////////////////////////
    void WindowIOS::notifyApplicationWillEnterForeground()
    {
        m_params->minimized = false;
    }
    
    //////////////////////////////////////////
    void WindowIOS::updateIOSWindow()
    {
        F32 width = [m_windowDelegateIOS frame].size.width;
        F32 height = [m_windowDelegateIOS frame].size.height;
        F32 contentScaleFactor = m_viewIOS.contentScaleFactor; 
        
        // #TODO:
        //m_size.x = width * contentScaleFactor;
        //m_size.y = height * contentScaleFactor;
        
        processWindowSizeChanged();
    }
    
} // namespace Maze
//////////////////////////////////////////
