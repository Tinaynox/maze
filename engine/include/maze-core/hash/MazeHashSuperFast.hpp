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
#if (!defined(_MazeHashSuperFast_hpp_))
#define _MazeHashSuperFast_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_CORE_API U32 SuperFastHashIncremental(Char const* _data, S32 _len, U32 _lastHash);

    //////////////////////////////////////////
    MAZE_CORE_API inline U32 CalculateSuperFastHash(Char const* _text, S32 _length)
    {
        static S32 const c_incrementalReadBlock = 65536;

        S32 bytesRemaining = _length;
        U32 lastHash = _length;
        S32 offset = 0;

        while (bytesRemaining >= c_incrementalReadBlock)
        {
            lastHash = SuperFastHashIncremental(_text + offset, c_incrementalReadBlock, lastHash);
            bytesRemaining -= c_incrementalReadBlock;
            offset += c_incrementalReadBlock;
        }

        if (bytesRemaining > 0)
        {
            lastHash = SuperFastHashIncremental(_text + offset, bytesRemaining, lastHash);
        }

        return lastHash;
    }

    //////////////////////////////////////////
    MAZE_CORE_API inline U32 CalculateSuperFastHash(Char const* _text)
    {
        return CalculateSuperFastHash(_text, (S32)strlen(_text));
    }

    //////////////////////////////////////////
    MAZE_CORE_API inline U32 CalculateSuperFastHash(StdString const& _text)
    {
        return CalculateSuperFastHash(_text.c_str(), (S32)_text.size());
    }

    //////////////////////////////////////////
    MAZE_CORE_API U32 CalculateSuperFastHash(std::ifstream& _file);


} // namespace Maze
////////////////////////////////////


#endif // _MazeHashSuperFast_hpp_
////////////////////////////////////
