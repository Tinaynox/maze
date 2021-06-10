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
#include "MazeCoreHeader.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {

        //////////////////////////////////////////
        U8 g_bytesMultiplicationTable[256][256];
        F32 g_sinByDegreeTable[360];
        F32 g_cosByDegreeTable[360];
        MAZE_CORE_API std::random_device g_randomDevice;
        MAZE_CORE_API std::mt19937 g_mt19937 = std::mt19937(g_randomDevice());

        //////////////////////////////////////////
        MAZE_CORE_API bool SetupMath()
        {
            // Fill bytes multiplication table
            for (S32 i = 0; i < 256; ++i)
                for (S32 j = 0; j < 256; ++j)
                    g_bytesMultiplicationTable[i][j] = static_cast<U8>(i * j / 255);

            // Fill sin and cos tables
            for (S32 i = 0; i < 360; ++i)
            {
                F32 angle = c_pi * (F32)i / 180.0f;
                g_sinByDegreeTable[i] = sinf(angle);
                g_cosByDegreeTable[i] = cosf(angle);
            }

            return true;
        }

    } // namespace Math
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
