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
#include "Example.hpp"
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
#include "maze-core/managers/MazeSystemCursorManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-ui/scenes/SceneDragAndDropDefault.hpp"
#include "ExampleCommonSettings.hpp"

#if MAZE_RENDER_SYSTEM_OPENGL_ENABLED
#   include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"
#endif

#if MAZE_SOUND_SYSTEM_OPENAL_ENABLED
#   include "maze-sound-system-openal/MazeSoundSystemOpenALPlugin.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    extern String GetExampleName();

    //////////////////////////////////////////
    extern void LoadFirstExampleScene(SceneManager* _sceneManager);

    //////////////////////////////////////////
    extern bool IsLoadAllAssets();

    //////////////////////////////////////////
    extern bool LoadPlugins();


    //////////////////////////////////////////
    // Class Example
    //
    //////////////////////////////////////////
    Example* Example::s_instance = nullptr;

    //////////////////////////////////////////
    Example::Example()
        : m_mainRenderWindowState(ExampleMainRenderWindowState::None)
        , m_mainRenderWindowViewport(0.0f, 0.0f, 1.0f, 1.0f)
        , m_debugEditorProgress(0.0f)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    Example::~Example()
    {
        m_uiManager.reset();

        m_sceneManager.reset();

        m_mainRenderWindow.reset();

        eventFrame.unsubscribe(this);
        eventCoreGameResourcesLoaded.unsubscribe(this);

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    ExamplePtr Example::Create(EngineConfig const& _config)
    {
        ExamplePtr engine;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Example, engine, init(_config));
        return engine;
    }

    //////////////////////////////////////////
    bool Example::init(EngineConfig const& _config)
    {
        eventFrame.subscribe(this, &Example::notifyFrame);
        eventCoreGameResourcesLoaded.subscribe(this, &Example::notifyCoreGameResourcesLoaded);

        return Engine::init(_config);
    }

    //////////////////////////////////////////
    void Example::update(F32 _dt)
    {
        if (m_editorToolsManager)
        {
            F32 const debugEditorProgressSpeed = 5.0f;

            if (m_editorToolsManager->getDebugEditorActive())
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

        if (m_mainRenderWindowState == ExampleMainRenderWindowState::ReadyToCreate)
        {
            if (createMainRenderWindowAndGoToSplashNow())
            {
                m_mainRenderWindowState = ExampleMainRenderWindowState::Created;
            }
            else
            {
                MAZE_ERROR("createMainRenderWindowAndGoToSplashNow FAILED!");
                m_mainRenderWindowState = ExampleMainRenderWindowState::Error;
                shutdown();
                return;
            }
        }
        else
        if (m_mainRenderWindowState == ExampleMainRenderWindowState::Created)
        {
            bool mainRenderWindowActive = (m_mainRenderWindow && m_mainRenderWindow->getWindow() && m_mainRenderWindow->getWindow()->isOpened());
            if (!mainRenderWindowActive)
            {
                shutdown();
            }
        }
    }

    //////////////////////////////////////////
    void Example::updateDebugEditorViewport()
    {
        F32 p = Math::SmoothStep(0.0f, 1.0f, m_debugEditorProgress);
        Rect2F viewport(
            Math::Lerp(0.0f, 0.25f, p),
            Math::Lerp(0.0f, 0.33f, p),
            Math::Lerp(1.0f, 0.5f, p),
            Math::Lerp(1.0f, 0.67f - 0.025f, p));

        setMainRenderWindowViewport(viewport);
    }

    //////////////////////////////////////////
    void Example::notifyFrame()
    {
        if (m_graphicsManager && m_graphicsManager->getDefaultRenderSystemRaw())
            m_graphicsManager->getDefaultRenderSystemRaw()->clearDrawCalls();
    }

    //////////////////////////////////////////
    bool Example::initMainManagers()
    {
        Debug::Log("Example Init - 1 [%ums]", getTime());

        LogService::GetInstancePtr()->setLogFile((FileHelper::GetDefaultLogDirectory() + "/log.log").c_str());

        if (!Engine::initMainManagers())
            return false;

        Debug::Log("Example Init - 2 [%ums]", getTime());

        m_inputManager->eventKeyboard.subscribe(this, &Example::notifyKeyboard);

        m_settingsManager->loadSettings();
        m_settingsManager->getSettings<EditorToolsSettings>()->getActiveChangedEvent().subscribe(this, &Example::notifyDebuggerActiveChanged);

        Debug::Log("Example Init - 3 [%ums]", getTime());

        if (!initExampleManagers())
            return false;

        Debug::Log("Example Init - 4 [%ums]", getTime());

        return true;
    }

    //////////////////////////////////////////
    bool Example::initExampleManagers()
    {
        if (!loadPlugins())
            return false;

        SettingsManager::GetInstancePtr()->registerSettings<ExampleCommonSettings>();

        processReadyToCreateWindowAndGoToSplash();

        return true;
    }

    //////////////////////////////////////////
    bool Example::loadPlugins()
    {
        if (!LoadPlugins())
            return false;

#if MAZE_RENDER_SYSTEM_OPENGL_ENABLED
        {
            RenderSystemOpenGLConfig config;
            config.multiContextPolicy = OpenGLMultiContextPolicy::Unified;
#   if MAZE_PLATFORM == MAZE_PLATFORM_OSX
            config.useDummyContext = false;
#   else
            config.useDummyContext = true;
#   endif
            MAZE_LOAD_PLATFORM_PLUGIN(RenderSystemOpenGL3, config);
        }
#endif

#if MAZE_SOUND_SYSTEM_OPENAL_ENABLED
        {
            SoundSystemOpenALConfig config;
            MAZE_LOAD_PLATFORM_PLUGIN(SoundSystemOpenAL, config);
        }
#endif

        Debug::log << "Available Render Systems: " << endl;
        for (auto const& renderSystemData : m_graphicsManager->getRenderSystems())
        {
            Debug::log << "\t" << renderSystemData.first;

            if (renderSystemData.second == m_graphicsManager->getDefaultRenderSystem())
                Debug::log << " [Default]";

            Debug::log << endl;
        }

        Debug::log << "Available Sound Systems: " << endl;
        for (auto const& soundSystemData : m_soundManager->getSoundSystems())
        {
            Debug::log << "\t" << soundSystemData.first;

            if (soundSystemData.second == m_soundManager->getDefaultSoundSystem())
                Debug::log << " [Default]";

            Debug::log << endl;
        }

        LogService::GetInstancePtr()->splitAndLog(PlatformHelper::ConstructApplicationInfo());
        LogService::GetInstancePtr()->splitAndLog(PlatformHelper::ConstructEngineInfo());
        LogService::GetInstancePtr()->splitAndLog(SystemHelper::ConstructSystemInfo());
        LogService::GetInstancePtr()->splitAndLog(m_windowManager->constructDisplaysInfo());

        return true;
    }

    //////////////////////////////////////////
    void Example::processReadyToCreateWindowAndGoToSplash()
    {
        if (m_mainRenderWindowState == ExampleMainRenderWindowState::None)
            m_mainRenderWindowState = ExampleMainRenderWindowState::ReadyToCreate;
    }

    //////////////////////////////////////////
    bool Example::createMainRenderWindow()
    {
        RenderWindowParams params;
        params.windowParams = WindowParams::Create();
        params.windowParams->clientSize = Vec2U32(1280, 800);
        params.windowParams->title = GetExampleName();
        params.windowParams->flags |= WindowStyleFlags::MinimizeButton;
        params.windowParams->flags |= WindowStyleFlags::MaximizeButton;
        params.windowParams->flags |= WindowStyleFlags::Resizable;
        params.antialiasingLevel = 16;
        m_mainRenderWindow = RenderWindow::Create(params);
        if (!m_mainRenderWindow)
        {
            m_mainRenderWindowState = ExampleMainRenderWindowState::Error;
            return false;
        }

        m_mainRenderWindow->setName("MainWindow");

        // m_mainRenderWindow->getWindow()->maximizeFullscreen();

#if ((MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN) || \
    (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))
        m_mainRenderWindow->setVSync(1);
#else
        m_mainRenderWindow->setVSync(0);
#endif

        LogService::GetInstancePtr()->splitAndLog(m_windowManager->constructWindowsInfo());

        m_mainRenderWindowState = ExampleMainRenderWindowState::Created;
        eventMainRenderWindowCreated();

        return true;
    }

    //////////////////////////////////////////
    bool Example::createMainRenderWindowAndGoToSplashNow()
    {
        if (!createMainRenderWindow())
            return false;

        Debug::Log("Example Init - 5 [%ums]", getTime());

        AssetManager::GetInstancePtr()->addAssetsDirectoryPath(AssetManager::GetInstancePtr()->getDefaultAssetsDirectory());

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        EcsWorldPtr const& world = entityManager->getDefaultWorld();
        createPrimaryEcsWorldSystems(world, m_mainRenderWindow, m_graphicsManager->getDefaultRenderSystem());

        Debug::Log("Example Init - 6 [%ums]", getTime());

        SceneEngineInitConfig sceneEngineInitConfig;
        sceneEngineInitConfig.renderTarget = m_mainRenderWindow;
        sceneEngineInitConfig.renderSystem = m_graphicsManager->getDefaultRenderSystem();
        createEngineScene(sceneEngineInitConfig);
        
        Debug::Log("Example Init - 7 [%ums]", getTime());

        // UI utils scenes
        m_sceneManager->loadScene<SceneDragAndDropDefault>(true, m_mainRenderWindow);
        
        LoadFirstExampleScene(m_sceneManager.get());

        return true;
    }

    //////////////////////////////////////////
    void Example::openDebugEditor()
    {
        m_editorToolsManager->openDebugEditor(m_mainRenderWindow);
    }

    //////////////////////////////////////////
    void Example::closeDebugEditor()
    {
        m_editorToolsManager->closeDebugEditor();
    }

    //////////////////////////////////////////
    void Example::updateDebugEditor()
    {
        if (SettingsManager::GetInstancePtr()->getSettings<EditorToolsSettings>()->getActive())
        {
            openDebugEditor();
        }
        else
        {
            closeDebugEditor();
        }
    }

    //////////////////////////////////////////
    void Example::setMainRenderWindowViewport(Rect2F const& _mainRenderWindowViewport)
    {
        if (m_mainRenderWindowViewport == _mainRenderWindowViewport)
            return;

        m_mainRenderWindowViewport = _mainRenderWindowViewport;

        eventMainRenderWindowViewportChanged(m_mainRenderWindowViewport);
    }

    //////////////////////////////////////////
    void Example::notifyDebuggerActiveChanged(bool const& _active)
    {
        updateDebugEditor();
    }

    //////////////////////////////////////////
    void Example::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
#if MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN
                    case KeyCode::F8:
#else
                    case KeyCode::F1:
#endif
                    {
                        SettingsManager::GetInstancePtr()->getSettings<EditorToolsSettings>()->switchActive();
                        SettingsManager::GetInstancePtr()->saveSettings();
                        break;
                    }
                    case KeyCode::S:
                    {
                        if (_data.isAltDown())
                        {
                            Debug::Log("Reload shaders");
                            RenderSystem::GetCurrentInstancePtr()->getShaderSystem()->reloadShaders();
                        }

                        break;
                    }
                    case KeyCode::Number0:
                    {
                        if (_data.isControlDown() && _data.isShiftDown())
                            LoadFirstExampleScene(m_sceneManager.get());
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
    bool Example::isMainWindowReadyToRender()
    {
        if (!getRunning())
            return false;

        if (!getMainRenderWindow())
            return false;

        if (!getMainRenderWindow()->isReadyToRender())
            return false;

        return true;
    }

    //////////////////////////////////////////
    void Example::notifyCoreGameResourcesLoaded()
    {
        RenderSystemPtr const& renderSystem = m_graphicsManager->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();

        if (IsLoadAllAssets())
        {
            shaderSystem->findAssetShadersAndAddToLibrary();
            renderSystem->getTextureManager()->loadAllAssetTextures();
            renderSystem->getMaterialManager()->loadAllAssetMaterials();
        }

        updateDebugEditor();
        m_debugEditorProgress = m_editorToolsManager->getDebugEditorActive() ? 1.0f : 0.0f;
        updateDebugEditorViewport();
    }

    //////////////////////////////////////////
    void Example::loadCoreGameAssets()
    {
        SystemCursorManager::GetInstancePtr()->createBuiltinSystemCursors();
        GraphicsManager::GetInstancePtr()->createBuiltinAssets();
        GizmosManager::GetInstancePtr()->createGizmosElements();
        UIManager::GetInstancePtr()->createUIElements();
        ParticlesManager::GetInstancePtr()->createBuiltinAssets();

        eventCoreGameResourcesLoaded();
    }

    //////////////////////////////////////////
    bool Example::isWindowFocused() const
    {
        if (!m_mainRenderWindow)
            return false;

        if (!m_mainRenderWindow->getWindow())
            return false;

        return m_mainRenderWindow->getWindow()->getFocused();
    }

} // namespace Maze
//////////////////////////////////////////
