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
#pragma once
#if (!defined(_MazeCSharpEvents_hpp_))
#define _MazeCSharpEvents_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, CSharpAssemblyLoadedEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, CSharpCoreAssemblyLoadedEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, CSharpAppAssemblyLoadedEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, CSharpEditorAssemblyLoadedEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, MonoPreShutdownEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, MonoInitializationEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, MonoShutdownEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, MonoPreReloadEvent);
    MAZE_DECLARE_SIMPLE_GENERIC_EVENT_API(MAZE_PLUGIN_CSHARP_API, MonoReloadEvent);
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCSharpEvents_hpp_
//////////////////////////////////////////
