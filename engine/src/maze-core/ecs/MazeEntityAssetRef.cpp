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
#include "maze-core/ecs/MazeEntityAssetRef.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/assets/MazeAssetUnitEntityPrefab.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EntityAssetRef
    //
    //////////////////////////////////////////
    String EntityAssetRef::toString() const
    {
        if (!m_entity)
            return String();

        HashedCString name = EntityPrefabManager::GetInstancePtr()->getEntityPrefabName(m_entity.get());
        return !name.empty() ? String(name.str) : EcsHelper::GetName(m_entity.get());
    }

    //////////////////////////////////////////
    void EntityAssetRef::setString(CString _data, Size _count)
    {
        EntityPtr const& m_entity = EntityPrefabManager::GetInstancePtr()->getOrLoadEntityPrefab(_data);
        setEntity(m_entity);
    }

    //////////////////////////////////////////
    bool EntityAssetRef::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
        {
            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
            switch (paramType)
            {
                // by AUID
                case DataBlockParamType::ParamU32:
                {
                    AssetUnitId auid = _dataBlock.getU32(paramIndex);

                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(auid);
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitEntityPrefab>::UID())
                    {
                        setEntity(assetUnit->castRaw<AssetUnitEntityPrefab>()->loadPrefab());
                        return true;
                    }

                    break;
                }
                // by name
                case DataBlockParamType::ParamString:
                {
                    String const& name = _dataBlock.getString(paramIndex);
                    EntityPtr const& m_entity = EntityPrefabManager::GetInstancePtr()->getOrLoadEntityPrefab(name);
                    setEntity(m_entity);
                    return true;
                }
                default:
                {
                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                    break;
                }
            }
        }

        setEntity(EntityPtr());
        return true;
    }

    //////////////////////////////////////////
    void EntityAssetRef::toDataBlock(DataBlock& _dataBlock) const
    {
        if (!m_entity)
        {
            _dataBlock.clearData();
            return;
        }

        // Save as AUID
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(
                HashedCString(EcsHelper::GetName(m_entity.get())));
            if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitEntityPrefab>::UID())
            {
                EntityPtr const& assetUnitEntity = assetUnit->castRaw<AssetUnitEntityPrefab>()->getPrefab();
                if (assetUnitEntity == m_entity)
                {
                    ValueToDataBlock(assetUnit->getAssetUnitId(), _dataBlock);
                    return;
                }
            }
        }

        // Save as string
        ValueToDataBlock(EcsHelper::GetName(m_entity.get()), _dataBlock);
    }
    
} // namespace Maze
//////////////////////////////////////////
