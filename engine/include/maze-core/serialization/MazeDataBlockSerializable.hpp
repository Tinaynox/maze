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
#if (!defined(_MazeDataBlockSerializable_hpp_))
#define _MazeDataBlockSerializable_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class IDataBlockSerializable
    //
    //////////////////////////////////////////
    class MAZE_CORE_API IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_ABSTRACT;
    };

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
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeDataBlockSerializable_hpp_
