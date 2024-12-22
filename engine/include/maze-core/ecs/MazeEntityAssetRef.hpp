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
#if (!defined(_MazeEntityAssetRef_hpp_))
#define _MazeEntityAssetRef_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EntityAssetRef
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityAssetRef
        : public IStringSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        EntityAssetRef(EntityPtr const& _value = nullptr)
            : m_entity(_value)
        {}

        //////////////////////////////////////////
        void setEntity(EntityPtr const& _value) { m_entity = _value; }

        //////////////////////////////////////////
        inline EntityPtr const& getEntity() const { return m_entity; }

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline bool operator==(EntityAssetRef const& _value) const { return m_entity == _value.getEntity(); }

        //////////////////////////////////////////
        inline bool operator!=(EntityAssetRef const& _value) const { return m_entity != _value.getEntity(); }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    private:
        EntityPtr m_entity;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntityAssetRef_hpp_
//////////////////////////////////////////
