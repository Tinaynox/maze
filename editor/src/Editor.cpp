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
#include "Editor.hpp"
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
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/math/MazePlane.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorConfigManager.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "settings/MazeEditorSceneSettings.hpp"
#include "scenes/SceneSplash.hpp"

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
#   include "../sys-res/win/WinResources.hpp"
#endif

#if MAZE_SOUND_SYSTEM_OPENAL_ENABLED
#   include "maze-sound-system-openal/MazeSoundSystemOpenALPlugin.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Editor
    //
    //////////////////////////////////////////
    Editor* Editor::s_instance = nullptr;

    //////////////////////////////////////////
    Editor::Editor()
        : m_mainRenderWindowState(EditorMainRenderWindowState::None)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    Editor::~Editor()
    {
        m_editorToolsManager.reset();

        m_sceneManager.reset();

        m_editorManager.reset();

        m_mainRenderWindow.reset();

        s_instance = nullptr;        
    }

    //////////////////////////////////////////
    EditorPtr Editor::Create(EngineConfig const& _config)
    {
        EditorPtr engine;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Editor, engine, init(_config));
        return engine;
    }

    //////////////////////////////////////////
    bool Editor::init(EngineConfig const& _config)
    {
        eventFrame.subscribe(this, &Editor::notifyFrame);
        eventCoreEditorResourcesLoaded.subscribe(this, &Editor::notifyCoreEditorResourcesLoaded);
        
        return Engine::init(_config);
    }    

    //////////////////////////////////////////
    void Editor::update(F32 _dt)
    {
        Engine::update(_dt);

        if (m_sceneManager)
            m_sceneManager->getScene<SceneSplash>();

        if (m_mainRenderWindowState == EditorMainRenderWindowState::ReadyToCreate)
        {
            if (createMainRenderWindowAndGoToFirstSceneNow())
            {
                m_mainRenderWindowState = EditorMainRenderWindowState::Created;
            }
            else
            {
                MAZE_ERROR("createMainRenderWindowAndGoToSplashNow FAILED!");
                m_mainRenderWindowState = EditorMainRenderWindowState::Error;
                shutdown();
                return;
            }
        }
        else
        if (m_mainRenderWindowState == EditorMainRenderWindowState::Created)
        {
            bool mainRenderWindowActive = (m_mainRenderWindow && m_mainRenderWindow->getWindow() && m_mainRenderWindow->getWindow()->isOpened());
            if (!mainRenderWindowActive)
            {
                shutdown();
            }
        }
    }

    //////////////////////////////////////////
    void Editor::notifyFrame()
    {
        
    }

    //////////////////////////////////////////
    bool Editor::initMainManagers()
    {
        LogService::GetInstancePtr()->setLogFile((FileHelper::GetDefaultLogDirectory() + "/log.log").c_str());

        if (!Engine::initMainManagers())
            return false;

        InspectorManager::GetInstancePtr()->setSaveAssetButtonEnabled(false);

        m_inputManager->eventKeyboard.subscribe(this, &Editor::notifyKeyboard);

        m_settingsManager->registerSettings<EditorSettings>();
        m_settingsManager->registerSettings<EditorSceneSettings>();
        m_settingsManager->loadSettings();
        

        if (!Editor::initEditorManagers())
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool Editor::initEditorManagers()
    {
        if (!loadPlugins())
            return false;

        EditorManager::Initialize(m_editorManager);
        if (!m_editorManager)
            return false;

        EditorConfigManager::Initialize(m_editorConfigManager);
        if (!m_editorConfigManager)
            return false;

        m_physics2DManager->getWorld()->setGravity(Vec2F32::c_zero);

        processReadyToCreateWindowAndGoToSplash();

        return true;
    }

    //////////////////////////////////////////
    bool Editor::loadPlugins()
    {
#if MAZE_RENDER_SYSTEM_OPENGL_ENABLED
        {
            RenderSystemOpenGLConfig config;
            config.multiContextPolicy = OpenGLMultiContextPolicy::Unified;
            MAZE_LOAD_PLATFORM_PLUGIN(RenderSystemOpenGL3, config);
        }
#endif

#if MAZE_SOUND_SYSTEM_OPENAL_ENABLED
        {
            SoundSystemOpenALConfig config;
            MAZE_LOAD_PLATFORM_PLUGIN(SoundSystemOpenAL, config);
        }
#endif

        MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG);

        Debug::log << "Available Render Systems: " << endl;
        for (auto const& renderSystemData : m_graphicsManager->getRenderSystems())
        {
            Debug::log << "\t" << renderSystemData.first;

            if (renderSystemData.second == m_graphicsManager->getDefaultRenderSystem())
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
    void Editor::processReadyToCreateWindowAndGoToSplash()
    {
        if (m_mainRenderWindowState == EditorMainRenderWindowState::None)
            m_mainRenderWindowState = EditorMainRenderWindowState::ReadyToCreate;
    }

    //////////////////////////////////////////
    bool Editor::createMainRenderWindow()
    {
        RenderWindowParams params;
        params.windowParams = WindowParams::Create();
        params.windowParams->clientSize = Vec2U32(1280, 800);
        params.windowParams->title = m_config.projectName;
        params.windowParams->flags |= WindowStyleFlags::MinimizeButton;
        params.windowParams->flags |= WindowStyleFlags::MaximizeButton;
        params.windowParams->flags |= WindowStyleFlags::Resizable;

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
        params.windowParams->iconBig = IDI_ICON00;
        params.windowParams->iconSmall = IDI_ICON01;
#endif

        m_mainRenderWindow = RenderWindow::Create(params);
        if (!m_mainRenderWindow)
        {
            m_mainRenderWindowState = EditorMainRenderWindowState::Error;
            return false;
        }

        // m_mainRenderWindow->getWindow()->maximizeFullscreen();

#if (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN) || \
    (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        m_mainRenderWindow->setVSync(1);
#else
        m_mainRenderWindow->setVSync(0);
#endif
        
        Debug::log << m_windowManager->constructWindowsInfo();
    
        m_mainRenderWindowState = EditorMainRenderWindowState::Created;
        eventMainRenderWindowCreated();

        return true;
    }

    //////////////////////////////////////////
    bool Editor::createMainRenderWindowAndGoToFirstSceneNow()
    {
        if (!createMainRenderWindow())
            return false;

        AssetManager::GetInstancePtr()->addAssetsDirectoryPath(AssetManager::GetInstancePtr()->getDefaultAssetsDirectory());
        

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        EcsWorldPtr const& world = entityManager->getDefaultWorld();
        createPrimaryEcsWorldSystems(world, m_mainRenderWindow, m_graphicsManager->getDefaultRenderSystem());

        SceneEngineInitConfig sceneEngineInitConfig;
        sceneEngineInitConfig.renderTarget = m_mainRenderWindow;
        sceneEngineInitConfig.renderSystem = m_graphicsManager->getDefaultRenderSystem();
        createEngineScene(sceneEngineInitConfig);

        m_sceneManager->loadScene<SceneSplash>();

        return true;
    }

    //////////////////////////////////////////
    void Editor::notifyKeyboard(InputEventKeyboardData const& _data)
    {
        switch (_data.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_data.keyCode)
                {
                    case KeyCode::F1:
                    {
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
    void Editor::notifyCoreEditorResourcesLoaded()
    {
    }

} // namespace Maze
//////////////////////////////////////////
