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
#if (!defined(_MazeHashMurmur_hpp_))
#define _MazeHashMurmur_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include <fstream>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Hash
    {
        //////////////////////////////////////////
        inline U32 CalculateMurmur2(S8* _data, U32 _length)
        {
            U32 const m = 0x5bd1e995;
            U32 const seed = 0;
            S32 const r = 24;

            U32 h = seed ^ _length;

            U8 const* data = (U8 const*)_data;
            U8 k = 0;

            while (_length >= 4)
            {
                k = data[0];
                k |= data[1] << 8;
                k |= data[2] << 16;
                k |= data[3] << 24;

                k *= m;
                k ^= k >> r;
                k *= m;

                h *= m;
                h ^= k;

                data += 4;
                _length -= 4;
            }

            switch (_length)
            {
                case 3:
                    h ^= data[2] << 16;
                case 2:
                    h ^= data[1] << 8;
                case 1:
                    h ^= data[0];
                    h *= m;
            };

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h;
        }

        //////////////////////////////////////////
        #define MAZE_MURMUR_MMIX(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

        //////////////////////////////////////////
        inline MAZE_CORE_API U32 CalculateMurmur2A(void const* _data, Size _length, U32 _seed)
        {
            U32 const m = 0x5bd1e995;
            S32 const r = 24;
            U32 l = _length;

            U8 const* data = (U8 const*)_data;

            U32 h = _seed;
            U32 k;

            while (_length >= 4)
            {
                k = *(U32*)data;

                MAZE_MURMUR_MMIX(h, k);

                data += 4;
                _length -= 4;
            }

            U32 t = 0;

            switch (_length)
            {
                case 3: t ^= data[2] << 16;
                case 2: t ^= data[1] << 8;
                case 1: t ^= data[0];
            };

            MAZE_MURMUR_MMIX(h, t);
            MAZE_MURMUR_MMIX(h, l);

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h;
        }

        //////////////////////////////////////////
        #undef MAZE_MURMUR_MMIX

    } // namespace Hash
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHashCRC_hpp_
//////////////////////////////////////////
