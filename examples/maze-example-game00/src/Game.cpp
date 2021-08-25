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
#include "Game.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Plugin.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
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
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/ecs/systems/MazeBounds2DSystem.hpp"
#include "maze-core/ecs/systems/MazeSizePolicy2DSystem.hpp"
#include "maze-core/ecs/systems/MazeTransformUtilsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderPreparationSystem.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "maze-ui/ecs/systems/MazeUITweenTransitionSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-debugger/managers/MazeDebuggerManager.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"
#include "maze-physics2d/ecs/systems/MazePhysicsControlSystem2D.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "settings/GameGraphicsSettings.hpp"
#include "settings/GameDebugSettings.hpp"
#include "scenes/SceneSplash.hpp"
#include "scenes/SceneDebug.hpp"
#include "scenes/SceneTest00.hpp"
#include "managers/PlayerManager.hpp"
#include "managers/GameManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Game
    //
    //////////////////////////////////////////
    Game* Game::s_instance = nullptr;

    //////////////////////////////////////////
    Game::Game()
        : m_mainRenderWindowState(GameMainRenderWindowState::None)
        , m_mainRenderWindowViewport(0.0f, 0.0f, 1.0f, 1.0f)
        , m_debugEditorProgress(0.0f)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    Game::~Game()
    {
        m_playerManager.reset();
        m_gameManager.reset();
        m_sceneManager.reset();

        m_mainRenderWindow.reset();

        m_settingsManager->getSettings<DebuggerSettings>()->eventActiveChanged.unsubscribe(this);

        s_instance = nullptr;

        
    }

    //////////////////////////////////////////
    GamePtr Game::Create(EngineConfig const& _config)
    {
        GamePtr engine;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Game, engine, init(_config));
        return engine;
    }

    //////////////////////////////////////////
    bool Game::init(EngineConfig const& _config)
    {
        if (!Engine::init(_config))
            return false;
        
        eventFrame.subscribe(this, &Game::notifyFrame);

        return true;
    }    

    //////////////////////////////////////////
    void Game::update(F32 _dt)
    {
        if (m_debuggerManager)
        {
            F32 const debugEditorProgressSpeed = 5.0f;

            if (m_debuggerManager->getDebugEditorActive())
            {
                if (m_debugEditorProgress < 1.0f)
                {
                    m_debugEditorProgress += debugEditorProgressSpeed * _dt;
                    if (m_debugEditorProgress > 1.0f)
                        m_debugEditorProgress = 1.0f;

                    updateDebugEditorViewport();
                }
            }
            else
            {
                if (m_debugEditorProgress > 0.0f)
                {
                    m_debugEditorProgress -= debugEditorProgressSpeed * _dt;
                    if (m_debugEditorProgress < 0.0f)
                        m_debugEditorProgress = 0.0f;

                    updateDebugEditorViewport();
                }
            }
        }

        Engine::update(_dt);

        if (m_mainRenderWindowState == GameMainRenderWindowState::ReadyToCreate)
        {
            if (createMainRenderWindowAndGoToSplashNow())
            {
                m_mainRenderWindowState = GameMainRenderWindowState::Created;
            }
            else
            {
                MAZE_ERROR("createMainRenderWindowAndGoToSplashNow FAILED!");
                m_mainRenderWindowState = GameMainRenderWindowState::Error;
                shutdown();
                return;
            }
        }
        else
        if (m_mainRenderWindowState == GameMainRenderWindowState::Created)
        {
            bool mainRenderWindowActive = (m_mainRenderWindow && m_mainRenderWindow->getWindow() && m_mainRenderWindow->getWindow()->isOpened());
            if (!mainRenderWindowActive)
            {
                shutdown();
            }
        }
    }

    //////////////////////////////////////////
    void Game::updateDebugEditorViewport()
    {
        F32 p = Math::SmoothStep(0.0f, 1.0f, m_debugEditorProgress);
        Rect2DF viewport(
            Math::Lerp(0.0f, 0.25f, p),
            Math::Lerp(0.0f, 0.33f, p),
            Math::Lerp(1.0f, 0.5f, p),
            Math::Lerp(1.0f, 0.67f, p));

        setMainRenderWindowViewport(viewport);
    }

    //////////////////////////////////////////
    void Game::notifyFrame()
    {
        
    }

    //////////////////////////////////////////
    bool Game::initMainManagers()
    {
        LogService::GetInstancePtr()->setLogFile((FileHelper::GetDefaultLogDirectory() + "/log.log").c_str());

        if (!Engine::initMainManagers())
            return false;

        m_inputManager->eventKeyboard.subscribe(this, &Game::notifyKeyboard);

        m_settingsManager->registerSettings<GameGraphicsSettings>();
        m_settingsManager->registerSettings<GameDebugSettings>();
        m_settingsManager->loadSettings();
        m_settingsManager->getSettings<DebuggerSettings>()->eventActiveChanged.subscribe(this, &Game::notifyDebuggerActiveChanged);

        if (!Game::initGameManagers())
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool Game::initGameManagers()
    {
        if (!loadPlugins())
            return false;

        GameManager::Initialize(m_gameManager);
        if (!m_gameManager)
            return false;

        PlayerManager::Initialize(m_playerManager);
        if (!m_playerManager)
            return false;

        m_physics2DManager->getWorld()->setGravity(Vec2DF::c_zero);

        processReadyToCreateWindowAndGoToSplash();

        return true;
    }

    //////////////////////////////////////////
    bool Game::loadPlugins()
    {
#if (MAZE_STATIC)

        Debug::log << "Plugins Static installation..." << endl;
        InstallRenderSystemOpenGL3Plugin();
        Debug::log << "Plugins Static installation finished." << endl;

#else

#    if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#        if (MAZE_ARCH == MAZE_ARCH_X86)
        m_pluginManager->loadPlugin("maze-render-system-opengl3-x86-d");
#        else
        m_pluginManager->loadPlugin("maze-render-system-opengl3-x64-d");
#        endif
#    elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        //g_pluginManager->loadPlugin(FileHelper::GetLibDirectory() + "/" + "libmaze-render-system-gl3-d.so");
        m_pluginManager->loadPlugin("libmaze-render-system-opengl3-d");
#    elif (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
        m_pluginManager->loadPlugin("libmaze-render-system-opengl3-d");
#    else
        m_pluginManager->loadPlugin("maze-render-system-opengl3-d");
#    endif
#endif

        Debug::log << "Available Render Systems: " << endl;
        for (auto const& renderSystemData : m_graphicsManager->getRenderSystems())
        {
            Debug::log << "\t" << renderSystemData.first;

            if (renderSystemData.second == m_graphicsManager->getDefaultRenderSystem())
                Debug::log << " [Default]";

            Debug::log << endl;
        }


        Debug::log << PlatformHelper::ConstructPlatformInfo();
        Debug::log << PlatformHelper::ConstructEngineInfo();
        Debug::log << SystemHelper::ConstructSystemInfo();
        Debug::log << m_windowManager->constructDisplaysInfo();

        return true;
    }

    //////////////////////////////////////////
    void Game::processReadyToCreateWindowAndGoToSplash()
    {
        if (m_mainRenderWindowState == GameMainRenderWindowState::None)
            m_mainRenderWindowState = GameMainRenderWindowState::ReadyToCreate;
    }

    //////////////////////////////////////////
    bool Game::createMainRenderWindow()
    {
        RenderWindowParams params;
        params.windowParams = WindowParams::Create();
        params.windowParams->clientSize = Vec2DU(1280, 800);
        params.windowParams->title = "Space Pressure 3D";
        params.windowParams->flags |= WindowStyleFlags::MinimizeButton;
        params.windowParams->flags |= WindowStyleFlags::MaximizeButton;
        params.windowParams->flags |= WindowStyleFlags::Resizable;
        m_mainRenderWindow = RenderWindow::Create(params);
        if (!m_mainRenderWindow)
        {
            m_mainRenderWindowState = GameMainRenderWindowState::Error;
            return false;
        }

        GameGraphicsSettingsPtr const& graphicsSettings = m_settingsManager->getSettings<GameGraphicsSettings>();

#if MAZE_RELEASE
        if (graphicsSettings->getFullscreen())
            m_mainRenderWindow->getWindow()->maximizeFullscreen();
#else
        graphicsSettings->setFullscreen(false);
#endif

#if (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN) || \
    (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        graphicsSettings->setVSync(1)
#endif

        m_mainRenderWindow->setVSync(graphicsSettings->getVSync());
        
        Debug::log << m_windowManager->constructWindowsInfo();
    
        m_mainRenderWindowState = GameMainRenderWindowState::Created;
        eventMainRenderWindowCreated();

        return true;
    }

    //////////////////////////////////////////
    bool Game::createMainRenderWindowAndGoToSplashNow()
    {
        if (!createMainRenderWindow())
            return false;

        AssetManager::GetInstancePtr()->addAssetsDirectory(AssetManager::GetInstancePtr()->getDefaultAssetsDirectory(), true);
        RenderSystemPtr const& renderSystem = m_graphicsManager->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();

        shaderSystem->findAssetShadersAndAddToCache();

        m_graphicsManager->getGizmosManager()->createGizmosElements();
        m_uiManager->createUIElements();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();

        createDefaultECSWorldSystems(world, m_mainRenderWindow, m_graphicsManager->getDefaultRenderSystem());

        m_sceneManager->loadScene<SceneSplash>();
        

        return true;
    }

    //////////////////////////////////////////
    void Game::openDebugEditor()
    {
        m_debuggerManager->openDebugEditor(m_mainRenderWindow);
    }

    //////////////////////////////////////////
    void Game::closeDebugEditor()
    {
        m_debuggerManager->closeDebugEditor();
    }

    //////////////////////////////////////////
    void Game::updateDebugEditor()
    {
        if (SettingsManager::GetInstancePtr()->getSettings<DebuggerSettings>()->getActive())
        {
            openDebugEditor();
        }
        else
        {
            closeDebugEditor();
        }
    }

    //////////////////////////////////////////
    void Game::setMainRenderWindowViewport(Rect2DF const& _mainRenderWindowViewport)
    {
        if (m_mainRenderWindowViewport == _mainRenderWindowViewport)
            return;

        m_mainRenderWindowViewport = _mainRenderWindowViewport;

        eventMainRenderWindowViewportChanged(m_mainRenderWindowViewport);
    }

    //////////////////////////////////////////
    void Game::notifyDebuggerActiveChanged(bool _active)
    {
        updateDebugEditor();
    }

    //////////////////////////////////////////
    void Game::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
                    case KeyCode::F1:
                    {
                        SettingsManager::GetInstancePtr()->getSettings<DebuggerSettings>()->switchActive();
                        SettingsManager::GetInstancePtr()->saveSettings();
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    bool Game::isMainWindowReadyToRender()
    {
        if (!getRunning())
            return false;

        if (!getMainRenderWindow())
            return false;

        if (!getMainRenderWindow()->getWindow()->isOpened())
            return false;

        if (getMainRenderWindow()->getWindow()->getMinimized())
            return false;

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
