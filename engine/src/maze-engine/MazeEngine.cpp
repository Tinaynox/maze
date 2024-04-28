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
#include "MazeEngineHeader.hpp"
#include "MazeEngine.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/system/MazeThread.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeDynLibManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "maze-ui/ecs/systems/MazeUISystem2D.hpp"
#include "maze-particles/ecs/systems/MazeParticlesDrawerSystem.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Engine
    //
    //////////////////////////////////////////
    Engine* Engine::s_instance = nullptr;

    //////////////////////////////////////////
    Engine::Engine()
        : m_running(true)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    Engine::~Engine()
    {
        m_particlesManager.reset();
        m_editorToolsManager.reset();
        m_soundManager.reset();
        m_uiManager.reset();
        m_physics2DManager.reset();
        m_gamepadManager.reset();
        m_entityManager.reset();
        m_graphicsManager.reset();
        m_assetManager.reset();
        m_sceneManager.reset();

        if (m_pluginManager)
            m_pluginManager->unloadAllPlugins();
        m_pluginManager.reset();

        m_dynLibManager.reset();
        m_windowManager.reset();
        m_inputManager.reset();
        m_eventManager.reset();
        m_settingsManager.reset();

        m_taskManager.reset();
        m_systemManager.reset();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    EnginePtr Engine::Create(EngineConfig const& _config)
    {
        EnginePtr engine;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Engine, engine, init(_config));
        return engine;
    }

    //////////////////////////////////////////
    bool Engine::init(EngineConfig const& _config)
    {
        Debug::log << "Engine::init started..." << endl;

        m_config = _config;

        if (!PlatformHelper::TestSystem())
        {
            return false;
        }

        if (m_config.initDelegate)
            eventInit.subscribe(m_config.initDelegate);

        if (m_config.frameDelegate)
            eventFrame.subscribe(m_config.frameDelegate);

        if (m_config.shutdownDelegate)
            eventShutdown.subscribe(m_config.shutdownDelegate);

        Debug::log << "SystemManager::Initialize started..." << endl;
        SystemManager::Initialize(m_systemManager, m_config.commandLineArguments);
        if (!m_systemManager)
            return false;

        m_systemManager->eventApplicationInit.subscribe(this, &Engine::notifyApplicationInit);
        m_systemManager->eventApplicationFrame.subscribe(this, &Engine::notifyApplicationFrame);

        Debug::log << "SystemManager initApplication started..." << endl;
        if (!m_systemManager->initApplication())
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool Engine::frame()
    {
        MAZE_PROFILE_FRAME("Engine Frame");

        UpdateManager* updateManager = UpdateManager::GetInstancePtr();

        U32 currentFrameTimeMS = updateManager->getMilliseconds();

        updateManager->processUpdate();    

        eventFrame();

        {
            MAZE_PROFILE_EVENT("Idle");
            U32 timeAfterUpdateMS = updateManager->getMilliseconds();
            while (timeAfterUpdateMS - currentFrameTimeMS < m_config.minFrameDeltaTimeMS)
            {
                timeAfterUpdateMS = updateManager->getMilliseconds();
            }
        }

        if (!m_running)
        {
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    void Engine::run()
    {
        Timer t;
        bool play = true;
        do
        {
            {
                MAZE_PROFILER_SCOPED_LOCK(FRAME);
                play = frame();
            }
#if (PROFILER_ENABLED)
            Maze::Profiler::FinishSample();
#endif
        }
        while (play);
    }


    //////////////////////////////////////////
    void Engine::shutdown()
    {
        if (!m_running)
            return;
        
        MAZE_LOG("Shutdown started!");
        m_running = false;
        
        if (m_systemManager)
            m_systemManager->shutdownApplication();
    }

    //////////////////////////////////////////
    void Engine::update(F32 _dt)
    {
        if (m_physics2DManager)
            m_physics2DManager->update(_dt);
    }

    //////////////////////////////////////////
    void Engine::notifyApplicationInit()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);
        if (!initMainManagers())
        {
            shutdown();
            return;
        }

        eventInit();
    }

    //////////////////////////////////////////
    void Engine::notifyApplicationFrame()
    {
        {
            MAZE_PROFILER_SCOPED_LOCK(FRAME);
            frame();
        }
#if (PROFILER_ENABLED)
        Maze::Profiler::FinishSample();
#endif
    }

    //////////////////////////////////////////
    bool Engine::initMainManagers()
    {
        TaskManager::Initialize(m_taskManager);
        if (!m_taskManager)
            return false;

        SettingsManager::Initialize(m_settingsManager, m_config.projectName);
        if (!m_settingsManager)
            return false;

        EventManager::Initialize(m_eventManager);
        if (!m_eventManager)
            return false;

        InputManager::Initialize(m_inputManager);
        if (!m_inputManager)
            return false;

        WindowManager::Initialize(m_windowManager);
        if (!m_windowManager)
            return false;

        DynLibManager::Initialize(m_dynLibManager);
        if (!m_dynLibManager)
            return false;

        PluginManager::Initialize(m_pluginManager);
        if (!m_pluginManager)
            return false;

        SceneManager::Initialize(m_sceneManager);
        if (!m_sceneManager)
            return false;

        AssetManager::Initialize(m_assetManager, m_config.params.getDataBlock(MAZE_HS("assetConfig"), DataBlock::c_empty));
        if (!m_assetManager)
            return false;

        EntityManager::Initialize(m_entityManager);
        if (!m_entityManager)
            return false;

        GraphicsManager::Initialize(m_graphicsManager);
        if (!m_graphicsManager)
            return false;

        GamepadManager::Initialize(m_gamepadManager);
        if (!m_gamepadManager)
            return false;

        Physics2DManager::Initialize(m_physics2DManager, m_physicsWorld2DConfig);
        if (!m_physics2DManager)
            return false;

        UIManager::Initialize(m_uiManager);
        if (!m_uiManager)
            return false;

        SoundManager::Initialize(m_soundManager);
        if (!m_soundManager)
            return false;

#if !(MAZE_PRODUCTION)
        EditorToolsManager::Initialize(m_editorToolsManager);
        if (!m_editorToolsManager)
            return false;
#endif

        ParticlesManager::Initialize(m_particlesManager);
        if (!m_particlesManager)
            return false;
        

        return true;
    }

    //////////////////////////////////////////
    void Engine::createPrimaryECSWorldSystems(
        ECSWorldPtr const& _world,
        RenderWindowPtr const& _renderWindow,
        RenderSystemPtr const& _renderSystem)
    {
        _world->addSystem(RenderControlSystem::Create(_renderSystem));
        _world->addSystem(UISystem2D::Create());
        _world->addSystem(InputSystem2D::Create());
    }

    //////////////////////////////////////////
    void Engine::createSecondaryECSWorldSystems(
        ECSWorldPtr const& _world,
        RenderWindowPtr const& _renderWindow,
        RenderSystemPtr const& _renderSystem)
    {
        _world->addSystem(ParticlesDrawerSystem::Create(_renderSystem));
    }

} // namespace Maze
//////////////////////////////////////////
