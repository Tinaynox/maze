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
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/system/MazeThread.hpp"
#include "maze-core/system/std/MazeThread_std.hpp"
#include "maze-core/system/MazeTimer.hpp"
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
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    extern String GetExampleName();

    //////////////////////////////////////////
    Maze::ExampleRenderSystemType g_renderSystemType = Maze::ExampleRenderSystemType::OpenGL;

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
Maze::S32 main(Maze::S32 argc, Maze::S8 const* argv[])
{
    for (Maze::S32 i = 0; i < argc; ++i)
    {
        Maze::Debug::log << i << " -> " << argv[i] << Maze::endl;
    }

#if (MAZE_DEBUG_MEMORY)
    Maze::MemoryTrackerService::StartMemoryTracking();
#endif

    Maze::Vector<Maze::S8 const*> commandLineArguments;
    for (Maze::S32 i = 0; i < argc; ++i)
        commandLineArguments.emplace_back(argv[i]);

    Maze::EngineConfig engineConfig;
    engineConfig.commandLineArguments = commandLineArguments;
    engineConfig.projectName = Maze::GetExampleName();

    // Optional engine config (e.g. 'renderSystem:String = "DX11"')
    Maze::Path engineConfigPath = Maze::FileHelper::GetWorkingDirectory() + "/engine.mzdata";
    if (Maze::FileHelper::IsFileExists(engineConfigPath))
    {
        engineConfig.params.loadTextFile(engineConfigPath);

        Maze::String const& renderSystemName = engineConfig.params.getString(MAZE_HCS("renderSystem"));

#if MAZE_RENDER_SYSTEM_DX11_ENABLED
        if (renderSystemName == "DX11")
            Maze::g_renderSystemType = Maze::ExampleRenderSystemType::DirectX11;
#endif
#if MAZE_RENDER_SYSTEM_VULKAN_ENABLED
		Maze::ThreadHelper::SleepCurrentThread(1000); // Give some time for Vulkan to load nvoglv64.dll to avoid conflicts with OpenGL
        if (renderSystemName == "Vulkan")
        {
            Maze::g_renderSystemType = Maze::ExampleRenderSystemType::Vulkan;
        }
#endif

    }

    bool restart = false;
    do
    {
        if (restart)
        {
#if MAZE_RENDER_SYSTEM_VULKAN_ENABLED
            Maze::ThreadHelper::SleepCurrentThread(1000); // Give some time for Vulkan to load nvoglv64.dll to avoid conflicts with OpenGL
#endif
        }

        Maze::ExamplePtr example = Maze::Example::Create(engineConfig);
        if (!example)
            break;

        example->run();
        restart = example->getRestart();
    }
    while (restart);

#if (MAZE_DEBUG_MEMORY)
    Maze::MemoryTrackerService::StopMemoryTracking();
#endif

    return 0;
}
