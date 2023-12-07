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
#if (!defined(_MazeDataBlockSerializationUtils_hpp_))
#define _MazeDataBlockSerializationUtils_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #define MAZE_DATA_BLOCK_COMMENT_PREFIX "@\xC"
    #define MAZE_DATA_BLOCK_COMMENT_PREFIX_LEN 2

    #define MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP "\x1"            // Comment "// ..."
    #define MAZE_DATA_BLOCK_COMMENT_SUFFIX_C   "\x2"            // Comment "/* ... */"
    #define MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP "\x11"   // Comment "Statement // ..."
    #define MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C   "\x12"   // Comment "Statement /* ... */"

    #define MAZE_DATA_BLOCK_COMMENT_CPP MAZE_DATA_BLOCK_COMMENT_PREFIX MAZE_DATA_BLOCK_COMMENT_SUFFIX_CPP
    #define MAZE_DATA_BLOCK_COMMENT_C MAZE_DATA_BLOCK_COMMENT_PREFIX MAZE_DATA_BLOCK_COMMENT_SUFFIX_C
    #define MAZE_DATA_BLOCK_COMMENT_ENDLINE_CPP MAZE_DATA_BLOCK_COMMENT_PREFIX MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_CPP
    #define MAZE_DATA_BLOCK_COMMENT_ENDLINE_C MAZE_DATA_BLOCK_COMMENT_PREFIX MAZE_DATA_BLOCK_COMMENT_ENDLINE_SUFFIX_C


    //////////////////////////////////////////
    static inline bool IsDataBlockIdentifierChar(Char _char)
    {
        return (
            (_char >= 'a' && _char <= 'z') ||
            (_char >= 'A' && _char <= 'Z') ||
            (_char >= '0' && _char <= '9') ||
            _char == '_' ||
            _char == '-' ||
            _char == '.' ||
            _char == '~');
    }

    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDataBlockSerializationUtils_hpp_
//////////////////////////////////////////
