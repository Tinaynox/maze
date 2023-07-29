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
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
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
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/helpers, MazeFileHelper)
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemManager)


//////////////////////////////////////////
Maze::SystemManagerPtr g_systemManager;
Maze::InputManagerPtr g_inputManager;
Maze::AssetManagerPtr g_assetManager;


//////////////////////////////////////////
void OnInit()
{
    Maze::Debug::log << "OnInit..." << Maze::endl;

    Maze::LogService::GetInstancePtr()->setLogFile((Maze::FileHelper::GetDefaultLogDirectory() + "/log.log").c_str());

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)

    Maze::String applicationId = Maze::FileHelper::GetPackageName();
    MAZE_LOG("Package Name: %s\n", applicationId.c_str());

    Maze::String internalDataDirectory = Maze::FileHelper::GetInternalDataDirectory();
    MAZE_LOG("Internal Data Directory: %s\n", internalDataDirectory.c_str());

    Maze::String externalDataDirectory = Maze::FileHelper::GetExternalDataDirectory();
    MAZE_LOG("External Data Directory: %s\n", externalDataDirectory.c_str());

#endif

    Maze::Debug::log << "InputManager Initialization..." << Maze::endl;
    Maze::InputManager::Initialize(g_inputManager);

    Maze::Debug::log << "AssetManager Initialization..." << Maze::endl;
    Maze::AssetManager::Initialize(g_assetManager);

    Maze::Debug::log << Maze::PlatformHelper::ConstructApplicationInfo();
    Maze::Debug::log << Maze::PlatformHelper::ConstructEngineInfo();
    Maze::Debug::log << Maze::SystemHelper::ConstructSystemInfo();
    
    Maze::Debug::log << "Adding resources directory: " << g_assetManager->getDefaultAssetsDirectory() << "..." << Maze::endl;
    g_assetManager->addAssetsDirectoryPath(g_assetManager->getDefaultAssetsDirectory());
    Maze::Debug::log << "Asset directory added." << Maze::endl;

    Maze::Debug::log << g_assetManager->constructAssetsInfo();

    Maze::AssetFilePtr testFile = g_assetManager->getAssetFileByFileName(
        MAZE_HASHED_CSTRING("Test.txt"));
    Maze::String testFileText;
    if (testFile)
    {
        testFileText = testFile->readAsString();
    }

    Maze::Debug::log << "Test file text=" << testFileText << Maze::endl;
}

//////////////////////////////////////////
void OnFrame()
{
    Maze::UpdateManager::GetInstancePtr()->processUpdate();
}

//////////////////////////////////////////
void OnShutdown()
{

}


//////////////////////////////////////////
Maze::S32 main( Maze::S32 argc, Maze::S8 const* argv[] )
{
    for (Maze::S32 i = 0; i < argc; ++i)
    {
        Maze::Debug::log << i << " -> " << argv[i] << Maze::endl;
    }

    Maze::Vector< Maze::S8 const* > commandLineArguments;
    for( Maze::S32 i = 0; i < argc; ++i )
        commandLineArguments.emplace_back( argv[i] );


    Maze::Debug::log << "SystemManager Initialization..." << Maze::endl;
    Maze::SystemManager::Initialize( g_systemManager, commandLineArguments );
    
    g_systemManager->eventApplicationInit.subscribe( &OnInit );
    g_systemManager->eventApplicationFrame.subscribe( &OnFrame );

    Maze::Debug::log << "Application Initialization..." << Maze::endl;
    g_systemManager->initApplication();

    while( true )
    {
        g_systemManager->processApplicationFrame();
    }

    
    std::cin.get();

    return 0;
}
