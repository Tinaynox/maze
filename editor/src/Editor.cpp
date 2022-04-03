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
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "maze-ui/ecs/systems/MazeUITweenTransitionSystem.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-debugger/managers/MazeDebuggerManager.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"
#include "maze-physics2d/ecs/systems/MazePhysicsControlSystem2D.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "managers/EditorManager.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "scenes/SceneMain.hpp"
#include "scenes/SceneDebug.hpp"
#include "scenes/SceneWorkspace.hpp"


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
        m_sceneManager.reset();

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

        m_inputManager->eventKeyboard.subscribe(this, &Editor::notifyKeyboard);

        m_settingsManager->registerSettings<EditorSettings>();
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

        m_physics2DManager->getWorld()->setGravity(Vec2DF::c_zero);

        processReadyToCreateWindowAndGoToSplash();

        return true;
    }

    //////////////////////////////////////////
    bool Editor::loadPlugins()
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
        params.windowParams->clientSize = Vec2DU(1280, 800);
        params.windowParams->title = "Editor";
        params.windowParams->flags |= WindowStyleFlags::MinimizeButton;
        params.windowParams->flags |= WindowStyleFlags::MaximizeButton;
        params.windowParams->flags |= WindowStyleFlags::Resizable;
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

        AssetManager::GetInstancePtr()->addAssetsDirectory(AssetManager::GetInstancePtr()->getDefaultAssetsDirectory(), true);
        

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();
        createPrimaryECSWorldSystems(world, m_mainRenderWindow, m_graphicsManager->getDefaultRenderSystem());

        m_sceneManager->loadScene<SceneMain>();
        m_sceneManager->loadScene<SceneDebug>();
        
        m_editorManager->start();

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
    void Editor::loadCoreEditorAssets()
    {
        RenderSystem::GetCurrentInstancePtr()->createBuiltinAssets();
        GraphicsManager::GetInstancePtr()->getGizmosManager()->createGizmosElements();
        UIManager::GetInstancePtr()->createUIElements();
        ParticlesManager::GetInstancePtr()->createBuiltinAssets();

        eventCoreEditorResourcesLoaded();
    }

    //////////////////////////////////////////
    void Editor::notifyCoreEditorResourcesLoaded()
    {
        RenderSystemPtr const& renderSystem = m_graphicsManager->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();

        /*
        shaderSystem->findAssetShadersAndAddToCache();
        renderSystem->getTextureManager()->loadAllAssetTextures();
        renderSystem->getMaterialManager()->loadAllAssetMaterials();
        renderSystem->getRenderMeshManager()->loadAllAssetRenderMeshes();
        */

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();
        createSecondaryECSWorldSystems(world, m_mainRenderWindow, m_graphicsManager->getDefaultRenderSystem());
    }

} // namespace Maze
//////////////////////////////////////////
