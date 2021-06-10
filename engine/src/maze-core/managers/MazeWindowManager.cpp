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
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeWindowManager)


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class WindowManager
    //
    //////////////////////////////////////////
    WindowManager* WindowManager::s_instance = nullptr;

    //////////////////////////////////////////
    WindowManager::WindowManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    WindowManager::~WindowManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void WindowManager::Initialize(WindowManagerPtr& _windowManager)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(WindowManager, _windowManager, init());
    }

    //////////////////////////////////////////
    bool WindowManager::init()
    {

        return true;
    }

    //////////////////////////////////////////
    DisplayPtr const& WindowManager::getPrimaryDisplay() const
    {
        static DisplayPtr nullPointer = nullptr;

        if (m_displays.empty())
            return nullPointer;

        for (DisplayPtr const& display : m_displays)
        {
            if (display->isPrimary())
                return display;
        }

        return m_displays.front();
    }

    //////////////////////////////////////////
    WindowVideoMode WindowManager::getPrimaryDisplayVideoMode() const
    {
        DisplayPtr const& display = getPrimaryDisplay();
        if (!display)
            return WindowVideoMode();

        return WindowHelper::GetDisplayCurrentMode(display);
    }

    //////////////////////////////////////////
    String WindowManager::constructDisplaysInfo()
    {
        String info;

        info += "[Displays Info]\n";


        for (DisplayPtr const& display : m_displays)
        {
            info += "\t" + display->getName();

            if (display->isPrimary())
                info += " [Primary]";

            info += "\n";

            info += "\t\tResolutions:\n";

            Vector<WindowVideoMode> fullscreenModes = display->getFullscreenModes();
            WindowVideoMode primaryDisplayMode = WindowHelper::GetDisplayCurrentMode(*display.get());
            F32 primaryDisplayModeAspectRatio = (F32)primaryDisplayMode.width / (F32)primaryDisplayMode.height;

            for (WindowVideoMode const& fullscreenMode : fullscreenModes)
            {
                F32 fullscreenModeAspectRatio = (F32)fullscreenMode.width / (F32)fullscreenMode.height;
                bool isRecommended = false;

                if (fabs(fullscreenModeAspectRatio - primaryDisplayModeAspectRatio) <= 0.01f)
                    isRecommended = true;

                info += "\t\t" + fullscreenMode.toString();

                if (isRecommended)
                {
                    info += " [+]";
                }

                info += "\n";
            }
        }


        return info;
    }

    //////////////////////////////////////////
    String WindowManager::constructWindowsInfo()
    {
        String info;

        info += "[Windows Info]\n";

        if (m_windows.empty())
        {
            info += "\tWindows List is empty.\n";
        }
        else
        {
            S32 index = 0;

            for (WindowWPtr const& weakWindow : m_windows)
            {
                WindowPtr window = weakWindow.lock();
                if (!window)
                    continue;

                info += "\t#" + StringHelper::ToString(index) + "\n";

                info += "\t\tTitle: " + window->getTitle() + "\n";

                if (window->isOpened())
                    info += "\t\tStatus: Opened\n";
                else
                    info += "\t\tStatus: Closed\n";

                if (window->getFocused())
                    info += "\t\tFocused: True\n";
                else
                    info += "\t\tFocused: False\n";

                info += "\t\tPosition: " + window->getPosition().toString() + "\n";
                info += "\t\tClient Size: " + window->getClientSize().toString() + "\n";
                info += "\t\tFull Size: " + window->getFullSize().toString() + "\n";

                switch (window->getWindowMode())
                {
                    case WindowMode::Windowed: info += "\t\tWindow Mode: Windowed\n"; break;
                    case WindowMode::WindowedMaximized: info += "\t\tWindow Mode: Maximized\n"; break;
                    case WindowMode::Fullscreen: info += "\t\tWindow Mode: Fullscreen\n"; break;
                    default: break;
                }

                if (window->getRelatedDisplay())
                    info += "\t\tRelated Display: " + window->getRelatedDisplay()->getName() + "\n";
                else
                    info += "\t\tRelated Display: None\n";

                ++index;
            }
        }

        return info;
    }

    //////////////////////////////////////////
    bool WindowManager::processWindowCanBeCreated(WindowParamsPtr const& _params)
    {
        return true;
    }

    //////////////////////////////////////////
    bool WindowManager::processWindowCreated(WindowPtr const& _window)
    {
        if (!_window)
            return false;

        m_windows.push_back(_window);

        return true;
    }

    //////////////////////////////////////////
    void WindowManager::processWindowDestroyed(Window const* _window)
    {
        for (WindowsList::iterator it = m_windows.begin(),
                                   end = m_windows.end();
                                   it != end;
                                   ++it)
        {
            if (_window == (*it).lock().get())
            {
                m_windows.erase(it);
                break;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
