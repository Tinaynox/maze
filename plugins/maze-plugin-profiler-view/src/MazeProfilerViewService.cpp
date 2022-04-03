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
#include "MazeProfilerViewHeader.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewService.hpp"
#include "maze-plugin-profiler-view/settings/MazeProfilerViewSettings.hpp"
#include "maze-plugin-profiler-view/scene/MazeSceneProfilerView.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ProfilerViewService
    //
    //////////////////////////////////////////
    ProfilerViewService::ProfilerViewService()
    {
        
    }

    //////////////////////////////////////////
    ProfilerViewService::~ProfilerViewService()
    {
        
    }

    //////////////////////////////////////////
    void ProfilerViewService::initialize()
    {
        MAZE_ERROR_RETURN_IF(!SettingsManager::GetInstancePtr(), "SettingsManager is null!");
        MAZE_ERROR_RETURN_IF(!SceneManager::GetInstancePtr(), "SceneManager is null!");
        MAZE_ERROR_RETURN_IF(!GraphicsManager::GetInstancePtr(), "GraphicsManager is null!");
        MAZE_ERROR_RETURN_IF(!InputManager::GetInstancePtr(), "InputManager is null!");

        SettingsManager::GetInstancePtr()->registerSettings<ProfilerViewSettings>();
        ProfilerViewSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>();
        profilerSettings->getActiveChangedEvent().subscribe(this, &ProfilerViewService::notifyProfilerViewActiveChanged);

        if (GraphicsManager::GetInstancePtr())
        {
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.subscribe(this, &ProfilerViewService::notifyDefaultRenderSystemWillBeChanged);
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.subscribe(this, &ProfilerViewService::notifyDefaultRenderSystemChanged);

            if (GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw())
                setRenderSystem(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw());
        }

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &ProfilerViewService::notifyKeyboard);

        updateActive();
    }

    //////////////////////////////////////////
    void ProfilerViewService::shutdown()
    {
        unloadScene();

        if (SettingsManager::GetInstancePtr())
        {
            ProfilerViewSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>();
            profilerSettings->getActiveChangedEvent().unsubscribe(this);

            SettingsManager::GetInstancePtr()->clearSettings<ProfilerViewSettings>();
        }

        setRenderSystem(nullptr);
        setRenderWindow(nullptr);
        
        if (GraphicsManager::GetInstancePtr())
        {
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.unsubscribe(this);
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.unsubscribe(this);
        }

        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
    }

    //////////////////////////////////////////
    void ProfilerViewService::setRenderSystem(RenderSystem* _renderSystem)
    {
        if (m_renderSystem == _renderSystem)
            return;

        setRenderWindow(nullptr);

        if (m_renderSystem)
            m_renderSystem->eventRenderWindowsChanged.unsubscribe(this);

        m_renderSystem = _renderSystem;

        if (m_renderSystem)
        {
            m_renderSystem->eventRenderWindowsChanged.subscribe(this, &ProfilerViewService::notifyRenderSystemRenderWindowsChanged);
            notifyRenderSystemRenderWindowsChanged();
        }
    }

    //////////////////////////////////////////
    void ProfilerViewService::setRenderWindow(RenderWindow* _renderWindow)
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
                m_renderWindow->getWindow()->eventWindowWillClose.subscribe(this, &ProfilerViewService::notifyRenderWindowWillClose);
        }

        updateActive();
    }

    //////////////////////////////////////////
    void ProfilerViewService::notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem)
    {
        setRenderSystem(_renderSystem.get());
    }

    //////////////////////////////////////////
    void ProfilerViewService::notifyDefaultRenderSystemWillBeChanged(RenderSystemPtr const& _renderSystem)
    {
        setRenderSystem(nullptr);
    }

    //////////////////////////////////////////
    void ProfilerViewService::notifyRenderSystemRenderWindowsChanged()
    {
        if (m_renderWindow)
            return;

        Set<RenderWindow*> const& renderWindows = m_renderSystem->getRenderWindows();
        if (renderWindows.empty())
            return;

        setRenderWindow(*renderWindows.begin());
    }

    //////////////////////////////////////////
    void ProfilerViewService::notifyRenderWindowWillClose(Window* _window)
    {
        setRenderWindow(nullptr);
    }

    //////////////////////////////////////////
    void ProfilerViewService::notifyProfilerViewActiveChanged(bool const& _active)
    {
        updateActive();
    }

    //////////////////////////////////////////
    void ProfilerViewService::updateActive()
    {
        if (!m_renderWindow)
            return;

        ProfilerViewSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>();
        bool active = profilerSettings->getActive();

        Profiler::SetProfiling(active);

        if (active)
            loadScene();
        else
            unloadScene();
    }

    //////////////////////////////////////////
    void ProfilerViewService::loadScene()
    {
        SceneProfilerViewPtr scene = SceneManager::GetInstancePtr()->getScene<SceneProfilerView>();
        if (!scene)
            scene = SceneManager::GetInstancePtr()->loadScene<SceneProfilerView>(true, m_renderWindow->cast<RenderTarget>());
    }

    //////////////////////////////////////////
    void ProfilerViewService::unloadScene()
    {
        if (!SceneManager::GetInstancePtr())
            return;

        SceneProfilerViewPtr scene = SceneManager::GetInstancePtr()->getScene<SceneProfilerView>();
        if (scene)
            SceneManager::GetInstancePtr()->destroyScene(scene);
    }

    //////////////////////////////////////////
    void ProfilerViewService::notifyKeyboard(InputEventKeyboardData const& _keyboardData)
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
                        ProfilerViewSettings* profilerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ProfilerViewSettings>();
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
