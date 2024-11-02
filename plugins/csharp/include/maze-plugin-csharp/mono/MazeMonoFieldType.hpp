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
#if (!defined(_MazeMonoFieldType_hpp_))
#define _MazeMonoFieldType_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_25_API(MAZE_PLUGIN_CSHARP_API, MonoFieldType,
        TypeString,
        TypeBool,
        TypeS8,
        TypeS16,
        TypeS32,
        TypeS64,
        TypeU8,
        TypeU16,
        TypeU32,
        TypeU64,
        TypeF32,
        TypeF64,
        TypeVec2S,
        TypeVec3S,
        TypeVec4S,
        TypeVec2U,
        TypeVec3U,
        TypeVec4U,
        TypeVec2F,
        TypeVec3F,
        TypeVec4F,
        TypeMat3F,
        TypeMat4F,
        TypeTMat,
        TypeRect2F);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoFieldType_hpp_
//////////////////////////////////////////
