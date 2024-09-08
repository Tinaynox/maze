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
#if (!defined(_MazeRandom_hpp_))
#define _MazeRandom_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Random
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern std::random_device g_randomDevice;
        MAZE_CORE_API extern std::mt19937 g_mt19937;


        //////////////////////////////////////////
        // UnitRandom - [0;1]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue UnitRandom()
        {
            return TValue(rand()) / TValue(RAND_MAX);
        }

        //////////////////////////////////////////
        // UnitRandomRadius - [0;1]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline TValue UnitRandomRadius()
        {
            TValue r = UnitRandom<TValue>() + UnitRandom<TValue>();
            if (r > (TValue)1)
                r = (TValue)2 - r;
            return r;
        }

        //////////////////////////////////////////
        // SymmetricRandom - [-1;1]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue SymmetricRandom()
        {
            return TValue(2.0) * TValue(rand() / RAND_MAX) - TValue(1.0);
        }

        //////////////////////////////////////////
        // Random - [0;value]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue Random(TValue _value)
        {
            return static_cast<TValue>(_value * UnitRandom());
        }

        //////////////////////////////////////////
        // RangeRandom - [low;high]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline MAZE_CONSTEXPR TValue RangeRandom(TValue const& _low, TValue const& _high)
        {
            return static_cast<TValue>((_high - _low) * UnitRandom() + _low);
        }

        //////////////////////////////////////////
        // RandomMT - [0;value]
        //
        //////////////////////////////////////////
        template <class TValue = F32>
        inline TValue RandomMT(TValue _value)
        {
            std::uniform_int_distribution<TValue> dist(static_cast<TValue>(0), _value);
            return dist(g_mt19937);
        }

        //////////////////////////////////////////
        // RangeRandomMT - [from;to)
        //
        //////////////////////////////////////////
        inline S32 RangeRandomMT(S32 _from, S32 _to)
        {
            std::uniform_int_distribution<S32> dist(_from, _to - 1);
            return dist(g_mt19937);
        }

        //////////////////////////////////////////
        template <>
        inline S32 RangeRandom(S32 const& _low, S32 const& _high)
        {
            return RangeRandomMT(_low, _high);
        }


        //////////////////////////////////////////
        inline U32 RangeRandomMT(U32 _from, U32 _to)
        {
            std::uniform_int_distribution<U32> dist(_from, _to - 1);
            return dist(g_mt19937);
        }

        //////////////////////////////////////////
        template <>
        inline U32 RangeRandom(U32 const& _low, U32 const& _high)
        {
            return RangeRandomMT(_low, _high);
        }


        //////////////////////////////////////////
        inline U32 CustomRand(U32 _seed)
        {
            return ((_seed * 1103515245 + 12345) % 4294967295);
        }
        
    
    
    } // namespace Random
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRandom_hpp_
//////////////////////////////////////////
