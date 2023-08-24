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
#include "maze-core/managers/win/MazeWindowManagerWin.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/system/win/MazeWindowWin.hpp"
#include "maze-core/helpers/win/MazeWindowHelperWin.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    BOOL CALLBACK DisplayMonitorsProc(HMONITOR _hMonitor, HDC _hdcMonitor, LPRECT _lprcMonitor, LPARAM _dwData)
    {
        MONITORINFOEX iMonitor;
        iMonitor.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(_hMonitor, &iMonitor);

        if (iMonitor.dwFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
        {
            return true;
        }
        else
        {
            U32 displayFlags = 0;
            String displayName;
            
            Vector<WindowVideoMode> fullscreenModes;

            if (iMonitor.szDevice)
            {
                displayName = iMonitor.szDevice;
                fullscreenModes = WindowHelper::GetDisplayFullscreenModes(displayName);
            }
            else
            {
                displayName = "Unnamed";
            }

            if (iMonitor.dwFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
                displayFlags |= Display::Flags::Primary;


            DisplayPtr display = Display::Create(displayName, 0, displayFlags, fullscreenModes);

            display->setUserData(_hMonitor);

            reinterpret_cast<Vector<DisplayPtr>*>(_dwData)->push_back(display);
            return true;
        };
    }


    //////////////////////////////////////////
    // Class WindowManagerWin
    //
    //////////////////////////////////////////
    WindowManagerWin::WindowManagerWin()
        : m_processWindowsMessages(true)
    {
    }

    //////////////////////////////////////////
    WindowManagerWin::~WindowManagerWin()
    {
    }

    //////////////////////////////////////////
    bool WindowManagerWin::init()
    {
        if (!WindowManager::init())
            return false;

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        updateDisplaysList();

        return true;
    }

    //////////////////////////////////////////
    void WindowManagerWin::updateDisplaysList()
    {
        m_displays.clear();

        EnumDisplayMonitors(NULL, NULL, &DisplayMonitorsProc, reinterpret_cast<LPARAM>(&m_displays));
    }

    //////////////////////////////////////////
    void WindowManagerWin::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("WindowManagerWin::update");

        if (m_processWindowsMessages)
        {
            MSG msg;

            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    break;

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
