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
#if (!defined(_MazeHashCRC_hpp_))
#define _MazeHashCRC_hpp_


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
        MAZE_CORE_API U32 CalculateCRC32(Char const* _text, Size _length, U32 _crc = 0);

        //////////////////////////////////////////
        inline MAZE_CORE_API U32 CalculateCRC32(U8 const* _data, Size _length, U32 _crc = 0)
        {
            return CalculateCRC32((Char const*)_data, _length, _crc);
        }

        //////////////////////////////////////////
        MAZE_CORE_API U32 CalculateCRC32(std::ifstream& _file, U32 _crc = 0);
    
    } // namespace Hash
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHashCRC_hpp_
//////////////////////////////////////////
