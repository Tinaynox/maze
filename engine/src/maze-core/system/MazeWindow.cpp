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
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct WindowParams
    //
    //////////////////////////////////////////
    WindowParamsPtr WindowParams::Create(
        Vec2DU _clientSize,
        U32 _bpp,
        CString _windowTitle,
        WindowMode _windowMode,
        bool _minimized,
        S32 _iconBig,
        S32 _iconSmall,
        WindowParams::Flags _flags)
    {
        WindowParamsPtr object = MAZE_CREATE_SHARED_PTR(MAZE_PLATFORM_OBJECT(WindowParams));

        if (!object->init(
                _clientSize,
                _bpp,
                _windowTitle,
                _windowMode,
                _minimized,
                _iconBig,
                _iconSmall,
                _flags))
            return WindowParamsPtr();
            
        return object;
    }

    //////////////////////////////////////////
    WindowParamsPtr WindowParams::Create(WindowParamsPtr const& _windowParams)
    {
        WindowParamsPtr object;
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(
            WindowParams,
            object,
            init(_windowParams));
        return object;
    }

    //////////////////////////////////////////
    bool WindowParams::init(
        Vec2DU _clientSize,
        U32 _bpp,
        CString _windowTitle,
        WindowMode _windowMode,
        bool _minimized,
        S32 _iconBig,
        S32 _iconSmall,
        WindowParams::Flags _flags)
    {
        clientSize = _clientSize;
        bpp = _bpp;
        windowMode = _windowMode;
        minimized = _minimized;
        title = _windowTitle;
        iconBig = _iconBig;
        iconSmall = _iconSmall;
        flags = _flags;

        return true;
    }

    //////////////////////////////////////////
    bool WindowParams::init(WindowParamsPtr const& _windowParams)
    {
        clientSize = _windowParams->clientSize;
        bpp = _windowParams->bpp;
        windowMode = _windowParams->windowMode;
        minimized = _windowParams->minimized;
        title = _windowParams->title;
        iconBig = _windowParams->iconBig;
        iconSmall = _windowParams->iconSmall;
        flags = _windowParams->flags;

        return true;
    }


    //////////////////////////////////////////
    // Class Window
    //
    //////////////////////////////////////////
    Window::Window()
    {
    }

    //////////////////////////////////////////
    Window::~Window()
    {
        WindowManager* windowManager = WindowManager::GetInstancePtr();
        if (windowManager)
        {
            windowManager->processWindowDestroyed(this);
        }

        eventDestroyed(this);
    }

    //////////////////////////////////////////
    WindowPtr Window::Create(WindowParamsPtr const& _params)
    {
        WindowManager* windowManager = WindowManager::GetInstancePtr();
        MAZE_ERROR_RETURN_VALUE_IF(!windowManager, WindowPtr(), "WindowManager is not exists!");

        if (!windowManager->processWindowCanBeCreated(_params))
            return WindowPtr();

        Maze::SharedPtr<Window> window = MAZE_CREATE_SHARED_PTR(MAZE_PLATFORM_OBJECT(Window));
        if (!window->init(_params))
            return Maze::SharedPtr<Window>();

        if (!windowManager->processWindowCreated(window))
            return WindowPtr();

        return window;
    }

    //////////////////////////////////////////
    bool Window::init(WindowParamsPtr const& _params)
    {
        if (_params)
            m_params = WindowParams::Create(_params);
        else
            m_params = WindowParams::Create();

        return true;
    }

    //////////////////////////////////////////
    bool Window::setTitle(String const& _title)
    {
        if (m_params->title == _title)
            return true;

        m_params->title = _title;

        return updateTitle();
    }

    //////////////////////////////////////////
    bool Window::setFullscreen(bool _fullscreen)
    {
        bool isFullscreenNow = (m_params->windowMode == WindowMode::Fullscreen);
        if (_fullscreen == isFullscreenNow)
            return false;

        m_params->windowMode = _fullscreen ? WindowMode::Fullscreen
                                           : WindowMode::Windowed;
        bool result = updateWindowMode();
        eventWindowModeChanged(this);
        return result;
    }

    //////////////////////////////////////////
    bool Window::setMinimized(bool _minimized)
    {
        if (m_params->minimized == _minimized)
            return false;

        m_params->minimized = _minimized;

        return updateMinimized();
    }

    //////////////////////////////////////////
    bool Window::setFlags(WindowParams::Flags _flags)
    {
        if (m_params->flags == _flags)
            return false;

        m_params->flags = _flags;
        
        updateWindowMode();
        return true;
    }

    //////////////////////////////////////////
    void Window::maximizeFullscreen()
    {
        WindowManager* windowManager = WindowManager::GetInstancePtr();

        DisplayPtr const& primaryDisplay = windowManager->getPrimaryDisplay();


        WindowVideoMode windowVideoMode = WindowHelper::GetDisplayCurrentMode(primaryDisplay);
        setClientSize(Vec2DU(windowVideoMode.width, windowVideoMode.height));

        setFullscreen(true);
    }

    //////////////////////////////////////////
    void Window::processWindowCreated()
    {
        eventWindowCreated(this);
    }

    //////////////////////////////////////////
    void Window::processWindowPositionChanged()
    {
        eventWindowPositionChanged(this);
    }

    //////////////////////////////////////////
    void Window::processWindowSizeChanged()
    {
        eventWindowSizeChanged(this);
    }

    //////////////////////////////////////////
    void Window::processWindowWillClose()
    {
        eventWindowWillClose(this);
    }

    //////////////////////////////////////////
    void Window::processWindowClosed()
    {
        eventWindowClosed(this);
    }

    //////////////////////////////////////////
    void Window::processWindowFocusChanged()
    {
        eventWindowFocusChanged(this);
    }

} // namespace Maze
//////////////////////////////////////////
