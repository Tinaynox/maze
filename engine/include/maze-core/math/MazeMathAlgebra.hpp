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
#if (!defined(_MazeMathAlgebra_hpp_))
#define _MazeMathAlgebra_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {
        //////////////////////////////////////////
        inline U32 GetNextPowerOfTwo(U32 _x)
        {
            U32 powerOfTwo = 1;

            while (powerOfTwo < _x && powerOfTwo < 2147483648)
                powerOfTwo = powerOfTwo << 1;

            return powerOfTwo;
        }

        //////////////////////////////////////////
        inline U32 GetAvailablePowersOfTwo(U32 _x)
        {
            U32 powerOfTwo = 1;
            U32 availablePowers = 0;

            while (powerOfTwo < _x && powerOfTwo < 2147483648)
            {
                powerOfTwo = powerOfTwo << 1;
                availablePowers++;
            }
            return availablePowers;
        }

        //////////////////////////////////////////
        inline bool IsPowerOfTwo(U32 _x)
        {
            return !(_x == 0) && !(_x & (_x - 1));
        }


        //////////////////////////////////////////
        // x^2 + px + q = 0
        MAZE_CORE_API S32 SolveNormalQuadratic(F32 _p, F32 _q, F32 (&_out)[2]);

        //////////////////////////////////////////
        // c[0] + c[1]*x + c[2]*x^2 = 0
        MAZE_CORE_API S32 SolveQuadratic(F32 _c0, F32 _c1, F32 _c2, F32(&_out)[2]);

        //////////////////////////////////////////
        // x^3 + Ax^2 + Bx + C = 0 = 0
        MAZE_CORE_API S32 SolveNormalCubic(F32 _a, F32 _b, F32 _c, F32(&_out)[3]);

        //////////////////////////////////////////
        // c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 = 0
        MAZE_CORE_API S32 SolveCubic(F32 _c0, F32 _c1, F32 _c2, F32 _c3, F32(&_out)[3]);

        //////////////////////////////////////////
        // x^4 + Ax^3 + Bx^2 + Cx + D = 0
        MAZE_CORE_API S32 SolveNormalQuartic(F32 _a, F32 _b, F32 _c, F32 _d, F32(&_out)[4]);

        //////////////////////////////////////////
        // c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 + c[4]*x^4 = 0
        MAZE_CORE_API S32 SolveQuartic(F32 _c0, F32 _c1, F32 _c2, F32 _c3, F32 _c4, F32(&_out)[4]);


    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMathAlgebra_hpp_
//////////////////////////////////////////
