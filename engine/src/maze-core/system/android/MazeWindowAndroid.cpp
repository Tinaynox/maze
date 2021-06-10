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
#include "maze-core/system/android/MazeWindowAndroid.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/managers/android/MazeSystemManagerAndroid.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WindowParamsAndroid
    //
    //////////////////////////////////////////
    WindowParamsAndroid::WindowParamsAndroid()
    {
    }

    ////////////////////////////////////
    bool WindowParamsAndroid::init(WindowParamsPtr const& _windowParams)
    {
        if (!WindowParams::init(_windowParams))
            return false;

        WindowParamsAndroid* windowParamsAndroid = _windowParams->castRaw<WindowParamsAndroid>();


        return true;
    }
    
    
    //////////////////////////////////////////
    // Class WindowAndroid
    //
    //////////////////////////////////////////
    WindowAndroid::WindowAndroid()
    {
    }

    //////////////////////////////////////////
    WindowAndroid::~WindowAndroid()
    {
        SystemManagerAndroid* systemManager = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr());
        if (systemManager)
        {
            systemManager->eventAndroidNativeWindowWillClose.unsubscribe(this);
            systemManager->eventAndroidAppFocusChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    bool WindowAndroid::init(WindowParamsPtr const& _params)
    {
        if (!Window::init(_params))
            return false;
        
        SystemManagerAndroid* systemManager = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr());
        systemManager->eventAndroidNativeWindowWillClose.subscribe(
                [this]()
                {
                    processWindowWillClose();
                    processWindowClosed();
                });

        systemManager->eventAndroidAppFocusChanged.subscribe(
                [this]()
                {
                    processWindowFocusChanged();
                });

        systemManager->eventAndroidNativeWindowInited.subscribe(
                [this]()
                {
                    processWindowCreated();
                });


        return true;

    }

    //////////////////////////////////////////
    bool WindowAndroid::isOpened()
    {
        // Android Window is always 'opened'
        return true;
    }
    
    ////////////////////////////////////
    void WindowAndroid::setClientSize(Vec2DU const& _size)
    {
    }

    //////////////////////////////////////////
    Vec2DU WindowAndroid::getClientSize()
    {
        SystemManagerAndroid* systemManager = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr());
        ANativeWindow* nativeWindow = systemManager->getNativeWindow();

        if (nativeWindow == nullptr)
            return Vec2DU::c_zero;

        Vec2DU result;

        result.x = (U32)ANativeWindow_getWidth(nativeWindow);
        result.y = (U32)ANativeWindow_getHeight(nativeWindow);

        return result;
    }

    //////////////////////////////////////////
    Vec2DU WindowAndroid::getFullSize()
    {
        SystemManagerAndroid* systemManager = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr());
        ANativeWindow* nativeWindow = systemManager->getNativeWindow();

        if (nativeWindow == nullptr)
            return Vec2DU::c_zero;

        Vec2DU result;

        result.x = (U32)ANativeWindow_getWidth(nativeWindow);
        result.y = (U32)ANativeWindow_getHeight(nativeWindow);

        return result;
    }

    //////////////////////////////////////////
    void WindowAndroid::setPosition(Vec2DS const& _position)
    {
    }

    //////////////////////////////////////////
    Vec2DS WindowAndroid::getPosition()
    {
        return Vec2DU::c_zero;
    }

    //////////////////////////////////////////
    void WindowAndroid::close()
    {
        
    }

    //////////////////////////////////////////
    bool WindowAndroid::getFocused()
    {
        SystemManagerAndroid* systemManager = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr());
        return systemManager->getAndroidAppFocused();
    }


    //////////////////////////////////////////
    void WindowAndroid::setFocused(bool _value)
    {

    }

    //////////////////////////////////////////
    void WindowAndroid::maximizeFullscreen()
    {
    }

    //////////////////////////////////////////
    DisplayPtr const& WindowAndroid::getRelatedDisplay()
    {
        static DisplayPtr nullPointer;

        WindowManager* windowManager = WindowManager::GetInstancePtr();
        if (!windowManager || windowManager->getDisplays().empty())
            return nullPointer;

        return windowManager->getDisplays().front();
    }
    
    //////////////////////////////////////////
    bool WindowAndroid::updateTitle()
    {
        return false;
    }
    
    //////////////////////////////////////////
    bool WindowAndroid::updateWindowMode()
    {
        return false;
    }

    //////////////////////////////////////////
    bool WindowAndroid::updateMinimized()
    {
        return false;
    }
    
} // namespace Maze
//////////////////////////////////////////
