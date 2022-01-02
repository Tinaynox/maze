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
#include "maze-core/system/osx/MazeWindowOSX.hpp"
#include MAZE_INCLUDE_OS_FILE(managers, MazeWindowManager)
#include MAZE_INCLUDE_OS_FILE(managers, MazeSystemManager)
#include MAZE_INCLUDE_OS_FILE(system, MazeApplicationDelegate)
#include MAZE_INCLUDE_OS_FILE(system, MazeWindowDelegate)
#include MAZE_INCLUDE_OS_FILE(system, MazeView)
#include MAZE_INCLUDE_OS_FILE(system, MazeThread)
#include "maze-core/system/MazeDisplay.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowParamsOSX
    //
    //////////////////////////////////////////
    WindowParamsOSX::WindowParamsOSX()
    {
    }

    ////////////////////////////////////
    bool WindowParamsOSX::init(WindowParamsPtr const& _windowParams)
    {
        if (!WindowParams::init(_windowParams))
            return false;


        return true;
    }
    
    
    //////////////////////////////////////////
    // Class WindowOSX
    //
    //////////////////////////////////////////
    WindowOSX::WindowOSX()
        : m_windowDelegateOSX(nullptr)
        , m_viewOSX(nullptr)
    {
    }
    
    
    //////////////////////////////////////////
    WindowOSX::~WindowOSX()
    {
        close();
    }
    
    //////////////////////////////////////////
    bool WindowOSX::init(WindowParamsPtr const& _params)
    {
        if (!Window::init(_params))
            return false;
        
        if (!createWindowOSX())
            return false;
        
        return true;
    }
    
    //////////////////////////////////////////
    bool WindowOSX::isOpened()
    {
        return (m_windowDelegateOSX != nullptr);
    }
    
    //////////////////////////////////////////
    void WindowOSX::setClientSize(Vec2DU const& _size)
    {
        m_params->clientSize = _size;
        updateWindowStyle();
    }
    
    //////////////////////////////////////////
    Vec2DU WindowOSX::getClientSize()
    {
        auto size = [[m_windowDelegateOSX contentView] frame].size;
        auto backingSize = [[m_windowDelegateOSX contentView] convertSizeToBacking:size];
        return Vec2DU(
            backingSize.width,
            backingSize.height);
    }
    
    //////////////////////////////////////////
    Vec2DU WindowOSX::getFullSize()
    {
        auto size = [m_windowDelegateOSX frame].size;
        auto backingSize = [[m_windowDelegateOSX contentView] convertSizeToBacking:size];
        return Vec2DU(
            backingSize.width,
            backingSize.height);
    }
    
    //////////////////////////////////////////
    void WindowOSX::setPosition(Vec2DS const& _position)
    {
        if (!isOpened())
            return;
        
        NSRect frame = NSMakeRect(
            _position.x,
            _position.y,
            [m_windowDelegateOSX frame].size.width,
            [m_windowDelegateOSX frame].size.height);
        [m_windowDelegateOSX setFrame:frame display:YES];
    }
    
    //////////////////////////////////////////
    Vec2DS WindowOSX::getPosition()
    {
        return Vec2DS(
            [m_windowDelegateOSX frame].origin.x,
            [m_windowDelegateOSX frame].origin.y);
    }
    
    //////////////////////////////////////////
    void WindowOSX::close()
    {
        if (!m_windowDelegateOSX)
            return;
        
        [m_windowDelegateOSX close];
        m_windowDelegateOSX = nil;
        m_viewOSX = nil;
    }
    
    //////////////////////////////////////////
    bool WindowOSX::getFocused()
    {
        return ([NSApplication sharedApplication].keyWindow == m_windowDelegateOSX);
    }
    
    //////////////////////////////////////////
    void WindowOSX::setFocused(bool _value)
    {
        if (_value)
        {
            [m_windowDelegateOSX makeKeyWindow];
        }
        else
        {
        }
    }
    
    //////////////////////////////////////////
    DisplayPtr const& WindowOSX::getRelatedDisplay()
    {
        static DisplayPtr nullPointer;
        
        if (!isOpened())
            return nullPointer;
        
        NSScreen* screen = [m_windowDelegateOSX screen];
        NSDictionary* screenDictionary = [screen deviceDescription];
        
        NSNumber* screenID = [screenDictionary objectForKey:@"NSScreenNumber"];
        CGDirectDisplayID aID = [screenID unsignedIntValue]; 
        for (DisplayPtr const& display : WindowManager::GetInstancePtr()->getDisplays())
        {
            CGDirectDisplayID displayId = (CGDirectDisplayID)(Size)display->getUserData(); 
            if (displayId == aID)
            {
                return display;
            }
        }
        
        return nullPointer;
    }
    
    //////////////////////////////////////////
    bool WindowOSX::updateTitle()
    {
        if (!isOpened())
            return true;
        
        if (!m_windowDelegateOSX)
            return false;
        
        [m_windowDelegateOSX setTitle: [NSString stringWithUTF8String: m_params->title.c_str()]];
        
        return true;
    }
    
    //////////////////////////////////////////
    bool WindowOSX::updateWindowMode()
    {        
        updateWindowStyle();
        
        SystemManagerOSX* systemManager = static_cast<SystemManagerOSX*>(SystemManager::GetInstancePtr());
        [m_windowDelegateOSX toggleFullScreen:systemManager->getApplicationDelegateOSX()];
        
        return false;
    }
    
    //////////////////////////////////////////
    bool WindowOSX::updateMinimized()
    {
        return false;
    }
    
    //////////////////////////////////////////
    void WindowOSX::processWindowDelegateOSXDidResize()
    {
        processWindowSizeChanged();
    }
    
    //////////////////////////////////////////
    void WindowOSX::processWindowDelegateOSXDidMove()
    {
        processWindowPositionChanged();
    }
    
    //////////////////////////////////////////
    void WindowOSX::processWindowDelegateOSXWillClose()
    {
        processWindowWillClose();
        
        m_windowDelegateOSX = nil;
        m_viewOSX = nil;
        
        processWindowClosed();
    }
    
    //////////////////////////////////////////
    void WindowOSX::processWindowDelegateOSXDidBecomeKey()
    {
        processWindowFocusChanged();
    }
    
    //////////////////////////////////////////
    void WindowOSX::processWindowDelegateOSXDidResignKey()
    {
        processWindowFocusChanged();
    }
    
    
    //////////////////////////////////////////
    bool WindowOSX::createWindowOSX()
    {
        close();
        
        NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
        mainDisplayRect.origin.x = (mainDisplayRect.origin.x + (mainDisplayRect.size.width/2)) - m_params->clientSize.x;
        mainDisplayRect.origin.y = (mainDisplayRect.origin.y + (mainDisplayRect.size.height/2)) - m_params->clientSize.y;
        mainDisplayRect.size.width = m_params->clientSize.x;
        mainDisplayRect.size.height = m_params->clientSize.y;
        
        
        NSUInteger styleMask = 0;
        if (!getFullscreen() && m_params->flags & WindowStyleFlags::Titlebar)
        {
            styleMask |= NSWindowStyleMaskTitled;
            styleMask |= NSWindowStyleMaskMiniaturizable;
            
            if (m_params->flags & WindowStyleFlags::CloseButton)
            {
                styleMask |= NSWindowStyleMaskClosable;
            }
        }
        
        if (!getFullscreen() && m_params->flags & WindowStyleFlags::Resizable)
        {
            styleMask |= NSWindowStyleMaskResizable;
        }
        
        
        m_windowDelegateOSX = [[WindowDelegateOSX alloc] initWithContentRect: mainDisplayRect
            styleMask: styleMask
            backing:NSBackingStoreBuffered
            defer:NO];
        
        MAZE_ERROR_RETURN_VALUE_IF(!m_windowDelegateOSX, false, "WindowDelegateOSX is not created!");
        
        
        [m_windowDelegateOSX setWindow: this];
        
        [m_windowDelegateOSX setHidesOnDeactivate: NO];
        [m_windowDelegateOSX setAutodisplay:YES];
        [m_windowDelegateOSX initialise];
        [m_windowDelegateOSX setTitle: [NSString stringWithUTF8String: m_params->title.c_str()]];
        [m_windowDelegateOSX setBackgroundColor:[NSColor grayColor]];
        [m_windowDelegateOSX setAcceptsMouseMovedEvents:YES];
        [m_windowDelegateOSX setIgnoresMouseEvents:NO];
        [m_windowDelegateOSX makeKeyAndOrderFront:nil];
        
        NSRect viewRect = NSMakeRect(
            0.0f,
            0.0f,
            [m_windowDelegateOSX frame].size.width,
            [m_windowDelegateOSX frame].size.height);
        
        m_viewOSX = [[ViewOSX alloc] initWithFrame:viewRect];
        MAZE_ERROR_RETURN_VALUE_IF(!m_viewOSX, false, "ViewOSX is not created!");
        [m_viewOSX setWindow:this];
        [m_windowDelegateOSX setContentView: m_viewOSX];
        [m_windowDelegateOSX makeFirstResponder: m_viewOSX];
        
        SystemManagerOSX* systemManager = static_cast<SystemManagerOSX*>(SystemManager::GetInstancePtr());
        if (getFullscreen())
        {
            [m_windowDelegateOSX toggleFullScreen:systemManager->getApplicationDelegateOSX()];
        }
        
        updateWindowStyle();
        
        return true;
    }
    
    //////////////////////////////////////////
    void WindowOSX::updateWindowStyle()
    {
        WindowManager* windowManager = WindowManager::GetInstancePtr();
        MAZE_ERROR_RETURN_IF(!windowManager, "WindowManager is not exists!");
        
        WindowVideoMode desktopMode = windowManager->getPrimaryDisplayVideoMode();
        
        Vec2DU currentClientSize = getClientSize();
        
        F32 scaleFactor = desktopMode.pixelScale;
        
        
        if (getFullscreen())
        {
            NSUInteger styleMask = [m_windowDelegateOSX styleMask];
            styleMask |= NSWindowStyleMaskBorderless;
            styleMask |= NSWindowStyleMaskFullScreen;
            styleMask |= NSWindowStyleMaskClosable;
            styleMask |= NSWindowStyleMaskTitled;
            styleMask |= NSWindowStyleMaskMiniaturizable;
            
            styleMask &= ~NSWindowStyleMaskResizable;
            
            [m_windowDelegateOSX setStyleMask: styleMask];
            [m_windowDelegateOSX setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];
            [m_windowDelegateOSX setOpaque: YES];
            
            Vec2DF size(
                desktopMode.width / scaleFactor,
                desktopMode.height / scaleFactor);
            
            CGFloat x = (desktopMode.width - size.x) / 2.0;
            CGFloat y = (desktopMode.height - size.y) / 2.0;
            NSRect oglRect = NSMakeRect(x, y, size.x, size.y);
            
            [m_windowDelegateOSX setFrame:oglRect display:YES];
            [m_windowDelegateOSX setLevel:NSMainMenuWindowLevel + 1];
            [[NSApplication sharedApplication] setPresentationOptions: NSApplicationPresentationAutoHideMenuBar];
        }
        else
        {
            F32 paramsVideoModeWidth = m_params->clientSize.x / scaleFactor;
            F32 paramsVideoModeHeight = m_params->clientSize.y / scaleFactor;
            
            NSUInteger styleMask = [m_windowDelegateOSX styleMask];
            styleMask &= ~NSWindowStyleMaskBorderless;
            styleMask &= ~NSWindowStyleMaskFullScreen;
            [m_windowDelegateOSX setCollectionBehavior: NSWindowCollectionBehaviorFullScreenAuxiliary];
            
            if (m_params->flags & WindowStyleFlags::Titlebar)
            {
                styleMask |= NSWindowStyleMaskTitled;
                styleMask |= NSWindowStyleMaskMiniaturizable;
                
                if (m_params->flags & WindowStyleFlags::CloseButton)
                {
                    styleMask |= NSWindowStyleMaskClosable;
                }
            }
            
            if (m_params->flags & WindowStyleFlags::Resizable)
            {
                styleMask |= NSWindowStyleMaskResizable;
            }
            else
            {
                styleMask &= ~NSWindowStyleMaskFullScreen;
            }
                
            
            if (m_params->flags & WindowStyleFlags::Background)
            {
                [m_windowDelegateOSX setOpaque: YES];
                [m_windowDelegateOSX setHasShadow: YES];
            }
            else
            {
                [m_windowDelegateOSX setOpaque: NO];
                [m_windowDelegateOSX setHasShadow: NO];
            }
            
            
            [m_windowDelegateOSX setStyleMask:styleMask];
            
            F32 titleBarHeight = NSHeight([m_windowDelegateOSX frame]) - NSHeight([[m_windowDelegateOSX contentView] frame]);
            F32 height = paramsVideoModeHeight + titleBarHeight;
            NSRect screenFrame = [[NSScreen mainScreen] visibleFrame];
            CGFloat maxVisibleHeight = screenFrame.size.height;
            if (height > maxVisibleHeight)
                height = maxVisibleHeight;
            
            CGFloat x = (desktopMode.width - paramsVideoModeWidth) / 2.0;
            CGFloat y = (desktopMode.height - height) / 2.0;
            NSRect frame = NSMakeRect(
                x,
                y,
                paramsVideoModeWidth,
                height);
            [m_windowDelegateOSX setFrame:frame display:YES];
            [m_windowDelegateOSX setStyleMask: styleMask];
            [m_windowDelegateOSX setLevel:NSNormalWindowLevel];
            [[NSApplication sharedApplication] setPresentationOptions: NSApplicationPresentationDefault];
        }
        
        if (currentClientSize != getClientSize())
        {
            processWindowSizeChanged();
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
