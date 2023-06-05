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
#if (!defined(_DamageData_hpp_))
#define _DamageData_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DamageFlags
    //
    //////////////////////////////////////////
    enum class DamageFlags
    {
        Critical        = MAZE_BIT(0),
        Blocked         = MAZE_BIT(1),
        Absorbed        = MAZE_BIT(2),
        Increased       = MAZE_BIT(3),
        Dodged          = MAZE_BIT(4),
    };


    //////////////////////////////////////////
    // Class DamageType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_7(DamageType,
        Physical,
        Fire,
        Cold,
        Energy,
        Toxic,
        Radioactive,
        Divine);


    //////////////////////////////////////////
    // Class DamageData
    //
    //////////////////////////////////////////
    struct DamageData
    {
        //////////////////////////////////////////
        DamageData(
            F32 _damageValue = 0.0f,
            F32 _criticalScale = 1.0f,
            DamageType const& _type = DamageType::Physical)
            : damageValue(_damageValue)
            , criticalScale(_criticalScale)
            , type(_type)
            , seed(Random::RangeRandom(S32_MIN, S32_MAX))
        {
        }

        F32 damageValue;
        F32 criticalScale;
        DamageType type;
        S32 seed;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _DamageData_hpp_
//////////////////////////////////////////
