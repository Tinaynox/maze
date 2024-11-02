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
#if (!defined(_MazeMonoBinds_hpp_))
#define _MazeMonoBinds_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #define MAZE_MONO_BIND_FUNC(DNamespace, DName) mono_add_internal_call(DNamespace ## ".InternalCalls::" #DName, DName);

    //////////////////////////////////////////
    #define MAZE_MONO_BIND_FUNC_WITH_NAME(DNamespace, DFunc, DName) mono_add_internal_call(DNamespace ## ".InternalCalls::" #DName, DFunc);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoBinds_hpp_
//////////////////////////////////////////
