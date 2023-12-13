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
#if (!defined(_MazeDataBlockBinarySerialization_hpp_))
#define _MazeDataBlockBinarySerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    extern U32 const c_mzDataBlockBinaryHeaderMagic;


    //////////////////////////////////////////
    namespace DataBlockBinarySerialization
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool SaveBinary(DataBlock const& _dataBlock, ByteBuffer& _buffer, U32 _flags = 0);

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadBinary(DataBlock& _dataBlock, ByteBuffer const& _buffer);

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveBinaryFile(DataBlock const& _dataBlock, Path const& _path, U32 _flags = 0);

        //////////////////////////////////////////
        MAZE_CORE_API bool LoadBinaryFile(DataBlock& _dataBlock, Path const& _path);

    } // namespace DataBlockBinarySerialization
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDataBlockBinarySerialization_hpp_
//////////////////////////////////////////
