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
#include "maze-core/helpers/MazeFileHelper.hpp"
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
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-engine/ecs/scenes/MazeSceneEngine.hpp"
#include "maze-engine/ecs/components/MazePlayerCanvas.hpp"
#include "settings/MazePlayerSettings.hpp"


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
        m_mainRenderWindow.reset();
        m_engineRenderTarget.reset();

        m_particlesManager.reset();
        m_editorToolsManager.reset();
        m_soundManager.reset();
        m_uiManager.reset();
        m_physics2DManager.reset();
        m_gamepadManager.reset();
        m_entityManager.reset();
        m_assetManager.reset();
        m_graphicsManager.reset();
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

        ++m_frame;

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
        TaskManager::Initialize(
            m_taskManager,
            m_config.params.getDataBlock(MAZE_HCS("taskConfig"), DataBlock::c_empty));
        if (!m_taskManager)
            return false;

        SettingsManager::Initialize(
            m_settingsManager,
            m_config.projectName,
            m_config.params.getDataBlock(MAZE_HCS("settingsConfig"), DataBlock::c_empty));
        if (!m_settingsManager)
            return false;
        m_settingsManager->registerSettings<PlayerSettings>();

        EventManager::Initialize(
            m_eventManager,
            m_config.params.getDataBlock(MAZE_HCS("eventConfig"), DataBlock::c_empty));
        if (!m_eventManager)
            return false;

        InputManager::Initialize(
            m_inputManager,
            m_config.params.getDataBlock(MAZE_HCS("inputConfig"), DataBlock::c_empty));
        if (!m_inputManager)
            return false;

        WindowManager::Initialize(
            m_windowManager,
            m_config.params.getDataBlock(MAZE_HCS("windowConfig"), DataBlock::c_empty));
        if (!m_windowManager)
            return false;

        DynLibManager::Initialize(
            m_dynLibManager,
            m_config.params.getDataBlock(MAZE_HCS("dynLibConfig"), DataBlock::c_empty));
        if (!m_dynLibManager)
            return false;

        PluginManager::Initialize(
            m_pluginManager,
            m_config.params.getDataBlock(MAZE_HCS("pluginConfig"), DataBlock::c_empty));
        if (!m_pluginManager)
            return false;

        SceneManager::Initialize(
            m_sceneManager,
            m_config.params.getDataBlock(MAZE_HCS("sceneConfig"), DataBlock::c_empty));
        if (!m_sceneManager)
            return false;

        AssetManager::Initialize(
            m_assetManager,
            m_config.params.getDataBlock(MAZE_HCS("assetConfig"), DataBlock::c_empty));
        if (!m_assetManager)
            return false;

        EntityManager::Initialize(
            m_entityManager,
            m_config.params.getDataBlock(MAZE_HCS("entityConfig"), DataBlock::c_empty));
        if (!m_entityManager)
            return false;

        GraphicsManager::Initialize(
            m_graphicsManager,
            m_config.params.getDataBlock(MAZE_HCS("graphicsConfig"), DataBlock::c_empty));
        if (!m_graphicsManager)
            return false;

        GamepadManager::Initialize(
            m_gamepadManager,
            m_config.params.getDataBlock(MAZE_HCS("gamepadConfig"), DataBlock::c_empty));
        if (!m_gamepadManager)
            return false;

        Physics2DManager::Initialize(
            m_physics2DManager,
            m_config.params.getDataBlock(MAZE_HCS("physics2DConfig"), DataBlock::c_empty));
        if (!m_physics2DManager)
            return false;

        UIManager::Initialize(
            m_uiManager,
            m_config.params.getDataBlock(MAZE_HCS("uiConfig"), DataBlock::c_empty));
        if (!m_uiManager)
            return false;

        SoundManager::Initialize(
            m_soundManager,
            m_config.params.getDataBlock(MAZE_HCS("soundConfig"), DataBlock::c_empty));
        if (!m_soundManager)
            return false;

#if !(MAZE_PRODUCTION)
        EditorToolsManager::Initialize(
            m_editorToolsManager,
            m_config.params.getDataBlock(MAZE_HCS("editorToolsConfig"), DataBlock::c_empty));
        if (!m_editorToolsManager)
            return false;
#endif

        ParticlesManager::Initialize(
            m_particlesManager,
            m_config.params.getDataBlock(MAZE_HCS("particlesConfig"), DataBlock::c_empty));
        if (!m_particlesManager)
            return false;
        

        // Register components
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<PlayerCanvas>("2D");


        return true;
    }

    //////////////////////////////////////////
    void Engine::createPrimaryEcsWorldSystems(
        EcsWorldPtr const& _world,
        RenderWindowPtr const& _renderWindow,
        RenderSystemPtr const& _renderSystem)
    {
        
    }

    //////////////////////////////////////////
    SceneEnginePtr Engine::createEngineScene(SceneEngineInitConfig const& _config)
    {
        return m_sceneManager->loadScene<SceneEngine>(true, _config);
    }

    //////////////////////////////////////////
    EcsAssetScenePtr Engine::loadAssetScene(
        Path const& _sceneName,
        RenderTargetPtr const& _renderTarget,
        bool _additive,
        EcsWorldId _ecsWorldId)
    {
        AssetFilePtr const& file = AssetManager::GetInstancePtr()->getAssetFile(_sceneName);
        if (!file)
        {
            Debug::LogError("Scene asset file is not found: '%s'!", _sceneName.toUTF8().c_str());
            return nullptr;
        }

        DataBlock dataBlock = file->readAsDataBlock();

        EcsWorld* ecsWorld = nullptr;
        if (_ecsWorldId == c_invalidWorldId)
            ecsWorld = m_entityManager->getDefaultWorldRaw();
        else
            ecsWorld = EcsWorld::GetEcsWorld(_ecsWorldId);

        EcsAssetScenePtr scene = EcsAssetScene::Create(
            file,
            _renderTarget,
            ecsWorld);

        scene->setName(
            HashedString(
                FileHelper::GetFileNameWithoutExtension(
                    file->getFileName()).toUTF8()));

        if (!EntitySerializationManager::GetInstancePtr()->loadSceneFromDataBlock(
            scene,
            dataBlock))
        {
            MAZE_ERROR("Failed to load asset scene: '%s'!", _sceneName.toUTF8().c_str());
            return nullptr;
        }

        SceneManager::GetInstancePtr()->loadScene(scene, _additive);

        return scene;
    }

    //////////////////////////////////////////
    EcsAssetScenePtr Engine::loadAssetScene(
        Path const& _sceneName,
        bool _additive,
        EcsWorldId _ecsWorldId)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_engineRenderTarget, nullptr, "Engine Render Target is not set!");

        return loadAssetScene(_sceneName, m_engineRenderTarget, _additive, _ecsWorldId);
    }

    //////////////////////////////////////////
    void Engine::setEngineRenderTarget(RenderTargetPtr const& _engineRenderTarget)
    {
        if (m_engineRenderTarget == _engineRenderTarget)
            return;

        m_engineRenderTarget = _engineRenderTarget;
    }

    //////////////////////////////////////////
    void Engine::setMainRenderWindow(RenderWindowPtr const& _mainRenderWindow)
    {
        if (m_mainRenderWindow == _mainRenderWindow)
            return;

        m_mainRenderWindow = _mainRenderWindow;
    }
    

} // namespace Maze
//////////////////////////////////////////
