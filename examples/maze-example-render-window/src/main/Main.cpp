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
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
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
namespace Maze
{
    //////////////////////////////////////////
    extern Maze::RenderSystemOpenGL3PluginPtr s_plugin;

} // namespace Maze
//////////////////////////////////////////


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
    Maze::SystemManagerPtr        systemManager;
    Maze::InputManagerPtr        inputManager;
    Maze::WindowManagerPtr        windowManager;
    Maze::DynLibManagerPtr        dynLibManager;
    Maze::PluginManagerPtr        pluginManager;
    Maze::GraphicsManagerPtr    graphicsManager;
    Maze::SceneManagerPtr        sceneManager;
    Maze::AssetManagerPtr        assetManager;
    Maze::EntityManagerPtr        entityManager;

    Maze::ColorU32 renderWindow00ClearColor;
    Maze::ColorU32 renderWindow01ClearColor;
    Maze::ColorU32 renderWindow10ClearColor;
    Maze::ColorU32 renderWindow11ClearColor;

    Maze::RenderWindowPtr renderWindow0;
    Maze::RenderWindowPtr renderWindow1;

    SceneExamplePtr scene;
};
std::unique_ptr<MainContainer> g_main;
Maze::RenderTargetPtr g_defaultRenderTarget;


Maze::F32 g_colorTimer = 0.0f;
Maze::F32 g_colorTime = 1.0f;
bool g_colorDirection = true;

Maze::F32 g_contextDestroyTimer = 0.0f;
Maze::F32 g_contextDestroyTime = 3.0f;



//////////////////////////////////////////
void OnUpdate(Maze::F32 _dt);


//////////////////////////////////////////
void OnInit()
{
    Maze::Profiler::SetProfiling(true);

    g_main->renderWindow00ClearColor = Maze::ColorU32::c_cyan;
    g_main->renderWindow01ClearColor = Maze::ColorU32::c_magenta;
    g_main->renderWindow10ClearColor = Maze::ColorU32::c_red;
    g_main->renderWindow11ClearColor = Maze::ColorU32::c_yellow;

    Maze::LogService::GetInstancePtr()->setLogFile((Maze::FileHelper::GetDefaultLogDirectory() + "/log.log").c_str());

    Maze::UpdateManager::GetInstancePtr()->eventUpdate.subscribe(OnUpdate);

    Maze::InputManager::Initialize(g_main->inputManager);
    Maze::WindowManager::Initialize(g_main->windowManager);
    Maze::DynLibManager::Initialize(g_main->dynLibManager);
    Maze::PluginManager::Initialize(g_main->pluginManager);
    Maze::EntityManager::Initialize(g_main->entityManager);
    Maze::GraphicsManager::Initialize(g_main->graphicsManager);
    Maze::SceneManager::Initialize(g_main->sceneManager);
    Maze::AssetManager::Initialize(g_main->assetManager);
    

    Maze::String workingDir = Maze::FileHelper::GetWorkingDirectory();
    Maze::Debug::log << "Binary: " << Maze::FileHelper::GetBinaryFullPath() << Maze::endl;
    Maze::Debug::log << "Working Directory: " << workingDir.c_str() << Maze::endl;


#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
    Maze::String applicationId = Maze::FileHelper::GetPackageName();
    MAZE_LOG("Package Name: %s\n", applicationId.c_str());
#endif

    MAZE_LOAD_PLATFORM_PLUGIN(RenderSystemOpenGL3);
    MAZE_LOAD_PLATFORM_PLUGIN(LoaderPNG);


    Maze::Debug::log << "Available Render Systems: " << Maze::endl;
    for (auto const& renderSystemData : g_main->graphicsManager->getRenderSystems())
    {
        Maze::Debug::log << "\t" << renderSystemData.first;

        if (renderSystemData.second == g_main->graphicsManager->getDefaultRenderSystem())
            Maze::Debug::log << " [Default]";

        Maze::Debug::log << Maze::endl;
    }


    Maze::Debug::log << Maze::PlatformHelper::ConstructPlatformInfo();
    Maze::Debug::log << Maze::PlatformHelper::ConstructEngineInfo();
    Maze::Debug::log << Maze::SystemHelper::ConstructSystemInfo();
    Maze::Debug::log << g_main->windowManager->constructDisplaysInfo();    
    

    Maze::RenderWindowParams params;
    params.windowParams = Maze::WindowParams::Create();
    params.windowParams->title = "RenderWindow0";
    params.windowParams->clientSize = Maze::Vec2DU(640, 480);
    params.windowParams->flags |= Maze::WindowStyleFlags::MinimizeButton;
    params.windowParams->flags |= Maze::WindowStyleFlags::MaximizeButton;
    params.windowParams->flags |= Maze::WindowStyleFlags::Resizable;
    g_main->renderWindow0 = Maze::RenderWindow::Create(params);
    if (g_main->renderWindow0)
    {
        g_main->renderWindow0->setVSync(false);

        Maze::Debug::log << "RenderWindow0 created!" << Maze::endl;
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

    params.windowParams = Maze::WindowParams::Create();
    params.windowParams->title = "RenderWindow1";
    g_main->renderWindow1 = Maze::RenderWindow::Create(params);

    if (g_main->renderWindow1)
    {
        g_main->renderWindow1->setVSync(false);

        Maze::log << "RenderWindow1 created!" << Maze::endl;
        g_main->renderWindow1->setClearColor(g_main->renderWindow01ClearColor);
    }
    else
    {
        MAZE_ERROR("RenderWindow1 creation error!");
    }

#endif

    Maze::Debug::log << g_main->windowManager->constructWindowsInfo();


    Maze::AssetManager::GetInstancePtr()->addAssetsDirectoryPath(Maze::AssetManager::GetInstancePtr()->getDefaultAssetsDirectory());

    Maze::RenderSystemPtr const& renderSystem = g_main->graphicsManager->getDefaultRenderSystem();
    Maze::ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();

    shaderSystem->findAssetShadersAndAddToCache();

    g_main->scene = g_main->sceneManager->loadScene<SceneExample>();
}

//////////////////////////////////////////
void OnShutdown();

//////////////////////////////////////////
void OnFrame()
{
    if (Maze::UpdateManager::GetInstancePtr())
        Maze::UpdateManager::GetInstancePtr()->processUpdate();

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
            Maze::Debug::log << "There is no render windows left - Shutdown..." << Maze::endl;
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

    Maze::Profiler::FinishSample();
}

//////////////////////////////////////////
void OnShutdown()
{
    Maze::Debug::Log("Shutdown!");
    
    g_defaultRenderTarget.reset();
    g_main.reset();
}

//////////////////////////////////////////
Maze::S32 main(Maze::S32 argc, Maze::S8 const* argv[])
{
    for (Maze::S32 i = 0; i < argc; ++i)
    {
        Maze::Debug::log << i << " -> " << argv[i] << Maze::endl;
    }

    Maze::Vector<Maze::S8 const*> commandLineArguments;
    for (Maze::S32 i = 0; i < argc; ++i)
        commandLineArguments.emplace_back(argv[i]);

    g_main = std::unique_ptr<MainContainer>(new MainContainer());

    Maze::SystemManager::Initialize(g_main->systemManager, commandLineArguments);

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
