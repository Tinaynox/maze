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
#include "maze-core/managers/android/MazeWindowManagerAndroid.hpp"
#include "maze-core/managers/android/MazeSystemManagerAndroid.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/helpers/android/MazeWindowHelperAndroid.hpp"
#include "maze-core/system/android/MazeWindowAndroid.hpp"
#include <android/hardware_buffer.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowManagerAndroid
    //
    //////////////////////////////////////////
    WindowManagerAndroid::WindowManagerAndroid()
    {
    }

    //////////////////////////////////////////
    WindowManagerAndroid::~WindowManagerAndroid()
    {
    }

    //////////////////////////////////////////
    bool WindowManagerAndroid::init()
    {
        if (!WindowManagerUnix::init())
            return false;

        updateDisplaysList();

        ANativeWindow* nativeWindow = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr())->getNativeWindow();
        MAZE_ERROR_RETURN_VALUE_IF(!nativeWindow, false, "ANativeWindow is not exists!");


        m_window = std::static_pointer_cast<WindowAndroid>(Window::Create());
        MAZE_ERROR_RETURN_VALUE_IF(!m_window, false, "Window cannot be created!");


        return true;
    }
    
    //////////////////////////////////////////
    void WindowManagerAndroid::updateDisplaysList()
    {
        m_displays.clear();

        SystemManager* systemManager = SystemManager::GetInstancePtr();
        ANativeWindow* window = static_cast<SystemManagerAndroid*>(systemManager)->getNativeWindow();
        MAZE_ERROR_RETURN_IF(!window, "Window is not created yet!");
        
        String displayName = "Main";
        
        U32 displayFlags = 0;
        displayFlags |= Display::Flags::Primary;
        
        Vector<WindowVideoMode> fullscreenModes;
        
        WindowVideoMode mode;
        mode.width = ANativeWindow_getWidth(window);
        mode.height = ANativeWindow_getHeight(window);

        AHardwareBuffer_Format format = static_cast<AHardwareBuffer_Format>(ANativeWindow_getFormat(window));

        switch (format)
        {
            case AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM: mode.bpp = 32; break;
            case AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM: mode.bpp = 32; break;
            case AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM: mode.bpp = 24; break;
            case AHARDWAREBUFFER_FORMAT_R5G6B5_UNORM: mode.bpp = 16; break;
            case AHARDWAREBUFFER_FORMAT_R16G16B16A16_FLOAT: mode.bpp = 64; break;
            case AHARDWAREBUFFER_FORMAT_R10G10B10A2_UNORM: mode.bpp = 32; break;
            case AHARDWAREBUFFER_FORMAT_BLOB: mode.bpp = 0; break;
            case AHARDWAREBUFFER_FORMAT_D16_UNORM: mode.bpp = 16; break;
            case AHARDWAREBUFFER_FORMAT_D24_UNORM: mode.bpp = 24; break;
            case AHARDWAREBUFFER_FORMAT_D24_UNORM_S8_UINT: mode.bpp = 32; break;
            case AHARDWAREBUFFER_FORMAT_D32_FLOAT: mode.bpp = 32; break;
            case AHARDWAREBUFFER_FORMAT_D32_FLOAT_S8_UINT: mode.bpp = 40; break;
            case AHARDWAREBUFFER_FORMAT_S8_UINT: mode.bpp = 8; break;
            default: mode.bpp = 0; break;
        }
        
        
        fullscreenModes.push_back(mode);
        
        DisplayPtr display = Display::Create(displayName, 0, displayFlags, fullscreenModes); 
        
        m_displays.push_back(display);
    }

    //////////////////////////////////////////
    bool WindowManagerAndroid::processWindowCanBeCreated(WindowParamsPtr const& _params)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_windows.empty(), false, "You cannot create more than one window on Android");

        return WindowManager::processWindowCanBeCreated(_params);
    }
    
    //////////////////////////////////////////
    bool WindowManagerAndroid::processWindowCreated(WindowPtr const& _window)
    {
        return WindowManager::processWindowCreated(_window);
    }


} // namespace Maze
//////////////////////////////////////////
