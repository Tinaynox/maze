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
#if (!defined(_MazeMonoBindsCore_hpp_))
#define _MazeMonoBindsCore_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBinds.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #define MAZE_CORE_MONO_BIND_FUNC(DName) MAZE_MONO_BIND_FUNC("Maze.Core", DName)

    //////////////////////////////////////////
    #define MAZE_CORE_MONO_BIND_FUNC_WITH_NAME(DFunc, DName) MAZE_MONO_BIND_FUNC_WITH_NAME("Maze.Core", DFunc, DName)

    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsCore();

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoBindsCore_hpp_
//////////////////////////////////////////
