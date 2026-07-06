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
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazePrefabInstance.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EcsHelper
    {

        //////////////////////////////////////////
        MAZE_CORE_API CString GetName(Entity* _entity)
        {
            Name* name = _entity->getComponentRaw<Name>();
            if (name)
                return name->getName().c_str();

            StaticName* staticName = _entity->getComponentRaw<StaticName>();
            if (staticName)
                return staticName->getName();

            return "";
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SetName(Entity* _entity, String const& _name)
        {
            _entity->removeComponent<StaticName>();
            _entity->ensureComponent<Name>()->setName(_name);
        }

        //////////////////////////////////////////
        MAZE_CORE_API EntityId GetEntityParentId(Entity const* _entity)
        {
            EntityId parentId = c_invalidEntityId;

            Transform2D* transform2D = _entity->getComponentRaw<Transform2D>();
            if (transform2D && transform2D->getParent())
                parentId = transform2D->getParent()->getEntityId();
            else
            {
                Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
                if (transform3D && transform3D->getParent())
                    parentId = transform3D->getParent()->getEntityId();
            }

            return parentId;
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureEntityIdBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:EntityId", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeEntityIdToDataBlock(DataBlock& _data, CString _name, EntityId _id)
        {
            EnsureEntityIdBlock(_data, _name)->setS32(MAZE_HCS("value"), (S32)_id);
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetEntityIdBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:EntityId", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API EntityId DeserializeEntityIdFromDataBlock(DataBlock const& _data, CString _name)
        {
            DataBlock const* dataBlock = GetEntityIdBlock(_data, _name);
            if (!dataBlock)
                return c_invalidEntityId;

            return EntityId(dataBlock->getS32(MAZE_HCS("value"), (S32)c_invalidEntityId));
        }


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureEntityIdArrayBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<EntityId>", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetEntityIdArrayBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<EntityId>", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureComponentBlock(DataBlock& _parentBlock, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Component", _name);
            return _parentBlock.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeComponentToDataBlock(DataBlock& _cmpBlock, Component* _component)
        {
            if (!_component || !_component->getEntity())
                return;

            SerializeEntityIdToDataBlock(_cmpBlock, "eid", _component->getEntityId());
            _cmpBlock.setString(MAZE_HCS("class"), _component->getComponentClassName());
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeComponentToDataBlock(DataBlock& _parentBlock, CString _name, Component* _component)
        {
            if (!_component || !_component->getEntity())
                return;

            DataBlock* cmpBlock = EnsureComponentBlock(_parentBlock, _name);
            SerializeComponentToDataBlock(*cmpBlock, _component);
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetComponentBlock(DataBlock const& _parentBlock, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Component", _name);

            return _parentBlock.getDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API Component* DeserializeComponentFromDataBlock(EcsWorld* _ecsWorld, DataBlock const& _cmpBlock)
        {
            EntityId eid = DeserializeEntityIdFromDataBlock(_cmpBlock, "eid");
            EntityPtr const& entity = _ecsWorld->getEntity(eid);
            if (!entity)
                return nullptr;

            CString className = _cmpBlock.getCString(MAZE_HCS("class"));
            if (!className)
                return nullptr;

            ComponentId componentId = GetComponentIdByName(className);
            return entity->getComponentById(componentId).get();
        }

        //////////////////////////////////////////
        MAZE_CORE_API Component* DeserializeComponentFromDataBlock(EcsWorld* _ecsWorld, DataBlock const& _parentBlock, CString _name)
        {
            DataBlock const* cmpBlock = GetComponentBlock(_parentBlock, _name);
            if (!cmpBlock)
                return nullptr;

            return DeserializeComponentFromDataBlock(_ecsWorld, *cmpBlock);
        }


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureComponentArrayBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<Component>", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetComponentArrayBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<Component>", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationId(Entity* _entity, EcsSerializationId _sid)
        {
            if (Transform2D* transform2D = _entity->getComponentRaw<Transform2D>())
                return FindEntityWithSerializationId(transform2D, _sid);
            else
            if (Transform3D* transform3D = _entity->getComponentRaw<Transform3D>())
                return FindEntityWithSerializationId(transform3D, _sid);

            return nullptr;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationId(Transform2D* _transform2D, EcsSerializationId _sid)
        {
            if (_transform2D->getEntityRaw()->getSerializationId() == _sid)
                return _transform2D->getEntityRaw();

            for (Transform2D* child : _transform2D->getChildren())
                if (Entity* entity = FindEntityWithSerializationId(child, _sid))
                    return entity;

            return nullptr;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationId(Transform3D* _transform3D, EcsSerializationId _sid)
        {
            if (_transform3D->getEntityRaw()->getSerializationId() == _sid)
                return _transform3D->getEntityRaw();

            for (Transform3D* child : _transform3D->getChildren())
                if (Entity* entity = FindEntityWithSerializationId(child, _sid))
                    return entity;

            return nullptr;
        }

        //////////////////////////////////////////
        static Entity* FindEntityWithSerializationIdInPrefabSpaceHelper(Transform2D* _transform2D, EcsSerializationId _sid)
        {
            Entity* entity = _transform2D->getEntityRaw();

            // Match before the boundary check - nested instance roots carry sids of the current space
            if (entity->getSerializationId() == _sid)
                return entity;

            // Do not descend into nested prefab spaces
            if (entity->getComponentRaw<PrefabInstance>())
                return nullptr;

            for (Transform2D* child : _transform2D->getChildren())
                if (Entity* result = FindEntityWithSerializationIdInPrefabSpaceHelper(child, _sid))
                    return result;

            return nullptr;
        }

        //////////////////////////////////////////
        static Entity* FindEntityWithSerializationIdInPrefabSpaceHelper(Transform3D* _transform3D, EcsSerializationId _sid)
        {
            Entity* entity = _transform3D->getEntityRaw();

            // Match before the boundary check - nested instance roots carry sids of the current space
            if (entity->getSerializationId() == _sid)
                return entity;

            // Do not descend into nested prefab spaces
            if (entity->getComponentRaw<PrefabInstance>())
                return nullptr;

            for (Transform3D* child : _transform3D->getChildren())
                if (Entity* result = FindEntityWithSerializationIdInPrefabSpaceHelper(child, _sid))
                    return result;

            return nullptr;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationIdInPrefabSpace(Entity* _entity, EcsSerializationId _sid)
        {
            // The boundary check is not applied to the search root itself
            if (_entity->getSerializationId() == _sid)
                return _entity;

            if (Transform2D* transform2D = _entity->getComponentRaw<Transform2D>())
            {
                for (Transform2D* child : transform2D->getChildren())
                    if (Entity* result = FindEntityWithSerializationIdInPrefabSpaceHelper(child, _sid))
                        return result;
            }
            else
            if (Transform3D* transform3D = _entity->getComponentRaw<Transform3D>())
            {
                for (Transform3D* child : transform3D->getChildren())
                    if (Entity* result = FindEntityWithSerializationIdInPrefabSpaceHelper(child, _sid))
                        return result;
            }

            return nullptr;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Entity* GetEnclosingPrefabInstanceRootEntity(Entity* _entity)
        {
            PrefabInstance* prefabInstance = nullptr;

            if (Transform2D* transform2D = _entity->getComponentRaw<Transform2D>())
            {
                if (Transform2DPtr const& parent = transform2D->getParent())
                    prefabInstance = parent->getFirstTrunkComponent<PrefabInstance>();
            }
            else
            if (Transform3D* transform3D = _entity->getComponentRaw<Transform3D>())
            {
                if (Transform3DPtr const& parent = transform3D->getParent())
                    prefabInstance = parent->getFirstTrunkComponent<PrefabInstance>();
            }

            return prefabInstance ? prefabInstance->getEntityRaw() : nullptr;
        }

        //////////////////////////////////////////
        MAZE_CORE_API S32 BuildPrefabSidChain(
            Entity* _entity,
            Map<EntityId, EcsSerializationId> const& _entityIndices,
            EcsSerializationId (&_outChain)[c_maxPrefabRefDepth])
        {
            // The entity itself is a prefab instance root serialized in this document
            // (used for refs to components owned by instance roots)
            auto selfIt = _entityIndices.find(_entity->getId());
            if (selfIt != _entityIndices.end())
            {
                _outChain[0] = selfIt->second;
                return 1;
            }

            EcsSerializationId reversedChain[c_maxPrefabRefDepth];
            S32 count = 0;

            Entity* cur = _entity;
            while (true)
            {
                Entity* root = GetEnclosingPrefabInstanceRootEntity(cur);
                if (!root)
                    return 0;

                if (count == c_maxPrefabRefDepth)
                    return 0;

                auto it = _entityIndices.find(root->getId());
                if (it != _entityIndices.end())
                {
                    // Reached a top-level prefab instance of this document
                    reversedChain[count++] = it->second;
                    break;
                }

                // Nested instance root - its sid belongs to the enclosing prefab's space
                if (root->getSerializationId() == c_invalidSerializationId)
                    return 0;

                reversedChain[count++] = root->getSerializationId();
                cur = root;
            }

            for (S32 i = 0; i < count; ++i)
                _outChain[i] = reversedChain[count - 1 - i];

            return count;
        }

        //////////////////////////////////////////
        MAZE_CORE_API void WritePrefabRefParams(
            DataBlock* _block,
            EcsSerializationId const* _chain,
            S32 _chainCount,
            EcsSerializationId _entitySid)
        {
            for (S32 i = 0; i < _chainCount; ++i)
                _block->addS32(MAZE_HCS("prefabSid"), _chain[i]);
            _block->setS32(MAZE_HCS("prefabEntitySid"), _entitySid);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void RemovePrefabRefParams(DataBlock* _block)
        {
            while (_block->removeParam(MAZE_HCS("prefabSid")));
            _block->removeParam(MAZE_HCS("prefabEntitySid"));
        }

        //////////////////////////////////////////
        MAZE_CORE_API Entity* ResolvePrefabSidChainTarget(
            DataBlock const& _block,
            Map<EcsSerializationId, EntityPtr> const& _outEntities)
        {
            DataBlock::SharedStringId prefabSidNameId = _block.getSharedStringId(MAZE_HCS("prefabSid"));

            Entity* cur = nullptr;
            for (DataBlock::ParamIndex i = 0, count = (DataBlock::ParamIndex)_block.getParamsCount(); i < count; ++i)
            {
                DataBlock::Param const& param = _block.getParam(i);
                if ((DataBlock::SharedStringId)param.nameId != prefabSidNameId ||
                    (DataBlockParamType)param.type != DataBlockParamType::ParamS32)
                    continue;

                EcsSerializationId sid = _block.getS32(i);
                if (!cur)
                {
                    auto it = _outEntities.find(sid);
                    MAZE_WARNING_RETURN_VALUE_IF(
                        it == _outEntities.end() || !it->second,
                        nullptr,
                        "Undefined prefab with sid=%d", sid);
                    cur = it->second.get();
                }
                else
                {
                    cur = FindEntityWithSerializationIdInPrefabSpace(cur, sid);
                    MAZE_WARNING_RETURN_VALUE_IF(!cur, nullptr, "Undefined nested prefab with sid=%d", sid);
                }
            }

            if (!cur)
                return nullptr;

            EcsSerializationId prefabEntitySid = _block.getS32(MAZE_HCS("prefabEntitySid"), c_invalidSerializationId);
            if (prefabEntitySid == c_invalidSerializationId)
                return nullptr;

            Entity* target = FindEntityWithSerializationIdInPrefabSpace(cur, prefabEntitySid);
            MAZE_WARNING_RETURN_VALUE_IF(!target, nullptr, "Undefined prefab entity with sid=%d", prefabEntitySid);
            return target;
        }

        //////////////////////////////////////////
        MAZE_CORE_API ComponentPtr const& ResolvePrefabComponentRef(
            DataBlock const& _block,
            Map<EcsSerializationId, EntityPtr> const& _outEntities)
        {
            static ComponentPtr const nullPointer;

            Entity* targetEntity = ResolvePrefabSidChainTarget(_block, _outEntities);
            if (!targetEntity)
                return nullPointer;

            return targetEntity->getComponentBySerializationId(
                _block.getS32(MAZE_HCS("prefabComponentSid"), c_invalidSerializationId));
        }

    } // namespace EcsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
