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
#include "MazeProfilerHeader.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-plugin-profiler/MazeProfilerService.hpp"
#include "maze-plugin-profiler/settings/MazeProfilerSettings.hpp"
#include "maze-plugin-profiler/scene/MazeSceneProfiler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ProfilerService
    //
    //////////////////////////////////////////
    template <>
    SharedPtr<ProfilerService> Singleton<ProfilerService>::s_instance{};
    template <>
    Mutex Singleton<ProfilerService>::s_mutex{};
    

    //////////////////////////////////////////
    template class Singleton<ProfilerService>;
    
    //////////////////////////////////////////
    ProfilerService::ProfilerService()
    {
        
    }

    //////////////////////////////////////////
    ProfilerService::~ProfilerService()
    {
        m_renderSystem = nullptr;
        m_renderWindow = nullptr;
    }

    //////////////////////////////////////////
    void ProfilerService::initialize()
    {
        MAZE_ERROR_RETURN_IF(!SettingsManager::GetInstancePtr(), "SettingsManager is null!");
        MAZE_ERROR_RETURN_IF(!SceneManager::GetInstancePtr(), "SceneManager is null!");
        MAZE_ERROR_RETURN_IF(!GraphicsManager::GetInstancePtr(), "GraphicsManager is null!");
        MAZE_ERROR_RETURN_IF(!InputManager::GetInstancePtr(), "InputManager is null!");

        SettingsManager::GetInstancePtr()->registerSettings<ProfilerSettings>();
        ProfilerSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerSettings>();
        profilerSettings->getActiveChangedEvent().subscribe(this, &ProfilerService::notifyProfilerActiveChanged);

        if (GraphicsManager::GetInstancePtr())
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.subscribe(this, &ProfilerService::notifyDefaultRenderSystemChanged);

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &ProfilerService::notifyKeyboard);

        updateActive();
    }

    //////////////////////////////////////////
    void ProfilerService::shutdown()
    {
        unloadScene();

        if (SettingsManager::GetInstancePtr())
        {
            ProfilerSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerSettings>();
            profilerSettings->getActiveChangedEvent().unsubscribe(this);

            SettingsManager::GetInstancePtr()->clearSettings<ProfilerSettings>();
        }

        setRenderSystem(nullptr);
        setRenderWindow(nullptr);
        
        if (GraphicsManager::GetInstancePtr())
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.unsubscribe(this);

        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
    }

    //////////////////////////////////////////
    void ProfilerService::setRenderSystem(RenderSystem* _renderSystem)
    {
        if (m_renderSystem == _renderSystem)
            return;

        setRenderWindow(nullptr);

        if (m_renderSystem)
            m_renderSystem->eventRenderWindowsChanged.unsubscribe(this);

        m_renderSystem = _renderSystem;

        if (m_renderSystem)
            m_renderSystem->eventRenderWindowsChanged.subscribe(this, &ProfilerService::notifyRenderSystemRenderWindowsChanged);
    }

    //////////////////////////////////////////
    void ProfilerService::setRenderWindow(RenderWindow* _renderWindow)
    {
        if (m_renderWindow == _renderWindow)
            return;

        if (m_renderWindow)
        {
            if (m_renderWindow->getWindow())
                m_renderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);

            unloadScene();
        }

        m_renderWindow = _renderWindow;

        if (m_renderWindow)
        {
            if (m_renderWindow->getWindow())
                m_renderWindow->getWindow()->eventWindowWillClose.subscribe(this, &ProfilerService::notifyRenderWindowWillClose);
        }

        updateActive();
    }

    //////////////////////////////////////////
    void ProfilerService::notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem)
    {
        setRenderSystem(_renderSystem.get());
    }

    //////////////////////////////////////////
    void ProfilerService::notifyRenderSystemRenderWindowsChanged()
    {
        if (m_renderWindow)
            return;

        Set<RenderWindow*> const& renderWindows = m_renderSystem->getRenderWindows();
        if (renderWindows.empty())
            return;

        setRenderWindow(*renderWindows.begin());
    }

    //////////////////////////////////////////
    void ProfilerService::notifyRenderWindowWillClose(Window* _window)
    {
        setRenderWindow(nullptr);
    }

    //////////////////////////////////////////
    void ProfilerService::notifyProfilerActiveChanged(bool const& _active)
    {
        updateActive();
    }

    //////////////////////////////////////////
    void ProfilerService::updateActive()
    {
        if (!m_renderWindow)
            return;

        ProfilerSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerSettings>();
        bool active = profilerSettings->getActive();

        if (active)
            loadScene();
        else
            unloadScene();
    }

    //////////////////////////////////////////
    void ProfilerService::loadScene()
    {
        SceneProfilerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneProfiler>();
        if (!scene)
            scene = SceneManager::GetInstancePtr()->loadScene<SceneProfiler>(true, m_renderWindow->cast<RenderTarget>());
    }

    //////////////////////////////////////////
    void ProfilerService::unloadScene()
    {
        if (!SceneManager::GetInstancePtr())
            return;

        SceneProfilerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneProfiler>();
        if (scene)
            SceneManager::GetInstancePtr()->destroyScene(scene);
    }

    //////////////////////////////////////////
    void ProfilerService::notifyKeyboard(InputEventKeyboardData const& _keyboardData)
    {
        switch (_keyboardData.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                if (_keyboardData.isAltDown() &&
                    _keyboardData.keyCode == KeyCode::P)
                {
                    if (SettingsManager::GetInstancePtr())
                    {
                        ProfilerSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerSettings>();
                        profilerSettings->switchActive();
                    }
                }

                break;
            }
            default:
                break;
        }
    }

} // namespace Maze
//////////////////////////////////////////
