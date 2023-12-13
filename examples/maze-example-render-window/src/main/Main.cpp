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
#include "SceneExample.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/system/MazeThread.hpp"
#include "maze-core/system/std/MazeThread_std.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/system/MazePath.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeDynLibManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-plugin-loader-png/MazeLoaderPNGPlugin.hpp"
#include <memory>

#include MAZE_INCLUDE_OS_FILE(maze-core/helpers, MazeFileHelper)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#    include "maze-render-system-opengl3/win/wgl/MazeContextOpenGL3WGL.hpp"
#endif


#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
#   include <unistd.h>
#endif

#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Plugin.hpp"


//////////////////////////////////////////
using namespace Maze;


//////////////////////////////////////////
extern RenderSystemOpenGL3PluginPtr s_plugin;


//////////////////////////////////////////
bool g_applicationActive = true;


//////////////////////////////////////////
struct MainContainer
{
public:
    
    //////////////////////////////////////////
    ~MainContainer()
    {
        scene.reset();
        
        renderWindow0.reset();
        renderWindow1.reset();
        
        entityManager.reset();
        assetManager.reset();
        sceneManager.reset();
        graphicsManager.reset();
        pluginManager.reset();
        dynLibManager.reset();
        windowManager.reset();
        inputManager.reset();
        systemManager.reset();
    }
    
public:
    SystemManagerPtr        systemManager;
    InputManagerPtr        inputManager;
    WindowManagerPtr        windowManager;
    DynLibManagerPtr        dynLibManager;
    PluginManagerPtr        pluginManager;
    GraphicsManagerPtr    graphicsManager;
    SceneManagerPtr        sceneManager;
    AssetManagerPtr        assetManager;
    EntityManagerPtr        entityManager;

    ColorU32 renderWindow00ClearColor;
    ColorU32 renderWindow01ClearColor;
    ColorU32 renderWindow10ClearColor;
    ColorU32 renderWindow11ClearColor;

    RenderWindowPtr renderWindow0;
    RenderWindowPtr renderWindow1;

    SceneExamplePtr scene;
};
std::unique_ptr<MainContainer> g_main;
RenderTargetPtr g_defaultRenderTarget;


F32 g_colorTimer = 0.0f;
F32 g_colorTime = 1.0f;
bool g_colorDirection = true;

F32 g_contextDestroyTimer = 0.0f;
F32 g_contextDestroyTime = 3.0f;



//////////////////////////////////////////
void OnUpdate(F32 _dt);


//////////////////////////////////////////
void OnInit()
{
    Profiler::SetProfiling(true);

    g_main->renderWindow00ClearColor = ColorU32::c_cyan;
    g_main->renderWindow01ClearColor = ColorU32::c_magenta;
    g_main->renderWindow10ClearColor = ColorU32::c_red;
    g_main->renderWindow11ClearColor = ColorU32::c_yellow;

    LogService::GetInstancePtr()->setLogFile((FileHelper::GetDefaultLogDirectory() + "/log.log").c_str());

    UpdateManager::GetInstancePtr()->eventUpdate.subscribe(OnUpdate);

    InputManager::Initialize(g_main->inputManager);
    WindowManager::Initialize(g_main->windowManager);
    DynLibManager::Initialize(g_main->dynLibManager);
    PluginManager::Initialize(g_main->pluginManager);
    EntityManager::Initialize(g_main->entityManager);
    GraphicsManager::Initialize(g_main->graphicsManager);
    SceneManager::Initialize(g_main->sceneManager);
    AssetManager::Initialize(g_main->assetManager);
    

    String workingDir = FileHelper::GetWorkingDirectory();
    Debug::log << "Binary: " << FileHelper::GetBinaryFullPath() << endl;
    Debug::log << "Working Directory: " << workingDir.c_str() << endl;


#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
    String applicationId = FileHelper::GetPackageName();
    MAZE_LOG("Package Name: %s\n", applicationId.c_str());
#endif

    MAZE_LOAD_PLATFORM_PLUGIN(RenderSystemOpenGL3);
    MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG);


    Debug::log << "Available Render Systems: " << endl;
    for (auto const& renderSystemData : g_main->graphicsManager->getRenderSystems())
    {
        Debug::log << "\t" << renderSystemData.first;

        if (renderSystemData.second == g_main->graphicsManager->getDefaultRenderSystem())
            Debug::log << " [Default]";

        Debug::log << endl;
    }


    LogService::GetInstancePtr()->splitAndLog(PlatformHelper::ConstructApplicationInfo());
    LogService::GetInstancePtr()->splitAndLog(PlatformHelper::ConstructEngineInfo());
    LogService::GetInstancePtr()->splitAndLog(SystemHelper::ConstructSystemInfo());
    LogService::GetInstancePtr()->splitAndLog(g_main->windowManager->constructDisplaysInfo());    
    

    RenderWindowParams params;
    params.windowParams = WindowParams::Create();
    params.windowParams->title = "RenderWindow0";
    params.windowParams->clientSize = Vec2U32(640, 480);
    params.windowParams->flags |= WindowStyleFlags::MinimizeButton;
    params.windowParams->flags |= WindowStyleFlags::MaximizeButton;
    params.windowParams->flags |= WindowStyleFlags::Resizable;
    g_main->renderWindow0 = RenderWindow::Create(params);
    if (g_main->renderWindow0)
    {
        g_main->renderWindow0->setVSync(false);

        Debug::log << "RenderWindow0 created!" << endl;
        g_main->renderWindow0->setClearColor(g_main->renderWindow00ClearColor);

        g_defaultRenderTarget = g_main->renderWindow0;
    }
    else
    {
        MAZE_ERROR("RenderWindow0 creation error!");
    }

#if (  (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)        \
    || (MAZE_PLATFORM == MAZE_PLATFORM_OSX)            \
    || (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)) && 0

    params.windowParams = WindowParams::Create();
    params.windowParams->title = "RenderWindow1";
    g_main->renderWindow1 = RenderWindow::Create(params);

    if (g_main->renderWindow1)
    {
        g_main->renderWindow1->setVSync(false);

        log << "RenderWindow1 created!" << endl;
        g_main->renderWindow1->setClearColor(g_main->renderWindow01ClearColor);
    }
    else
    {
        MAZE_ERROR("RenderWindow1 creation error!");
    }

#endif

    Debug::log << g_main->windowManager->constructWindowsInfo();


    AssetManager::GetInstancePtr()->addAssetsDirectoryPath(AssetManager::GetInstancePtr()->getDefaultAssetsDirectory());

    RenderSystemPtr const& renderSystem = g_main->graphicsManager->getDefaultRenderSystem();
    ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();

    shaderSystem->findAssetShadersAndAddToLibrary();

    g_main->scene = g_main->sceneManager->loadScene<SceneExample>();
}

//////////////////////////////////////////
void OnShutdown();

//////////////////////////////////////////
void OnFrame()
{
    if (UpdateManager::GetInstancePtr())
        UpdateManager::GetInstancePtr()->processUpdate();

    bool renderWindow0Active = (g_main->renderWindow0 && g_main->renderWindow0->getWindow() && g_main->renderWindow0->getWindow()->isOpened());
    bool renderWindow1Active = (g_main->renderWindow1 && g_main->renderWindow1->getWindow() && g_main->renderWindow1->getWindow()->isOpened());


    if (!renderWindow0Active && !renderWindow1Active)
    {
        if (g_applicationActive)
        {
#if (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
            OnShutdown();
            g_applicationActive = false;
#elif (MAZE_PLATFORM != MAZE_PLATFORM_ANDROID)
            Debug::log << "There is no render windows left - Shutdown..." << endl;
            g_applicationActive = false;
#endif
        }

        return;
    }
}

//////////////////////////////////////////
void OnUpdate(float _dt)
{
    g_colorTimer += _dt;
    while (g_colorTimer >= g_colorTime)
    {
        g_colorTimer -= g_colorTime;
        g_colorDirection = !g_colorDirection;
    }

    float colorScalar = g_colorTimer / g_colorTime;

    g_contextDestroyTimer += _dt;

    Profiler::FinishSample();
}

//////////////////////////////////////////
void OnShutdown()
{
    Debug::Log("Shutdown!");
    
    g_defaultRenderTarget.reset();
    g_main.reset();
}

//////////////////////////////////////////
S32 main(S32 argc, S8 const* argv[])
{
    for (S32 i = 0; i < argc; ++i)
    {
        Debug::log << i << " -> " << argv[i] << endl;
    }

    Vector<S8 const*> commandLineArguments;
    for (S32 i = 0; i < argc; ++i)
        commandLineArguments.emplace_back(argv[i]);

    g_main = std::unique_ptr<MainContainer>(new MainContainer());

    SystemManager::Initialize(g_main->systemManager, commandLineArguments);

    g_main->systemManager->eventApplicationInit.subscribe(&OnInit);
    g_main->systemManager->eventApplicationFrame.subscribe(&OnFrame);

    if (!g_main->systemManager->initApplication())
        return 1;

    while (g_applicationActive)
    {
        g_main->systemManager->processApplicationFrame();
    }

    OnShutdown();


    return 0;
}
