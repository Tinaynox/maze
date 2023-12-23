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
#if (!defined(_MazeDataBlockHelper_hpp_))
#define _MazeDataBlockHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace DataBlockHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void SerializeMetaInstanceToDataBlock(
            MetaClass const* _metaClass,
            ConstMetaInstance _metaInstance,
            DataBlock& _dataBlock);

        //////////////////////////////////////////
        MAZE_CORE_API void DeserializeMetaInstanceFromDataBlock(
            MetaClass const* _metaClass,
            MetaInstance _metaInstance,
            DataBlock const& _dataBlock);

    } // namespace DataBlockHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDataBlockHelper_hpp_
//////////////////////////////////////////
