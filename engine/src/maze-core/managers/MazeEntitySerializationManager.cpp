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
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazePrefabInstance.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-core/serialization/MazeDataBlockTextSerialization.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static const S32 c_enititySerializationVersion = 1;


    //////////////////////////////////////////
    // Class EntitySerializationManager
    //
    //////////////////////////////////////////
    EntitySerializationManager* EntitySerializationManager::s_instance = nullptr;

    //////////////////////////////////////////
    EntitySerializationManager::EntitySerializationManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EntitySerializationManager::~EntitySerializationManager()
    {
        s_instance = nullptr;

        
    }

    //////////////////////////////////////////
    void EntitySerializationManager::Initialize(EntitySerializationManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EntitySerializationManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void PrepareEntitiesToSerialize(
        Vector<EntitySerializationData>& _entityComponents,
        Vector<PrefabSerializationData>& _prefabs,
        Map<void*, EcsSerializationId>& _outPointerIndices,
        Map<EntityId, EcsSerializationId>& _outEntityIndices,
        Map<AssetUnitId, EntityPtr>& _outIdentityPrefabs,
        EcsWorldPtr& _outIdentityPrefabsWorld)
    {
        // Sort entities
        {
            Vector<Entity*> unassignedEntities;
            Vector<Component*> unassginedComponents;
            Vector<PrefabInstance*> unassginedPrefabInstances;

            Set<EcsSerializationId> usedIds;

            EcsSerializationId maxSerializationId = 0;
            for (EntitySerializationData const& entityData : _entityComponents)
            {
                if (entityData.entity->getSerializationId() != c_invalidSerializationId)
                {
                    if (usedIds.count(entityData.entity->getSerializationId()) == 0)
                        usedIds.insert(entityData.entity->getSerializationId());
                    else
                        entityData.entity->setSerializationId(c_invalidSerializationId);
                }

                if (entityData.entity->getSerializationId() == c_invalidSerializationId)
                    unassignedEntities.push_back(entityData.entity.get());
                else
                {
                    if (entityData.entity->getSerializationId() > maxSerializationId)
                        maxSerializationId = Math::Max(maxSerializationId, entityData.entity->getSerializationId());
                    
                    _outPointerIndices[entityData.entity.get()] = entityData.entity->getSerializationId();
                    _outEntityIndices[entityData.entity->getId()] = entityData.entity->getSerializationId();
                }

                for (ComponentPtr const& component : entityData.components)
                {
                    if (component->getSerializationId() != c_invalidSerializationId)
                    {
                        if (usedIds.count(component->getSerializationId()) == 0)
                            usedIds.insert(component->getSerializationId());
                        else
                            component->setSerializationId(c_invalidSerializationId);
                    }

                    if (component->getSerializationId() == c_invalidSerializationId)
                        unassginedComponents.push_back(component.get());
                    else
                    {
                        if (component->getSerializationId() > maxSerializationId)
                            maxSerializationId = Math::Max(maxSerializationId, component->getSerializationId());
                        _outPointerIndices[component.get()] = component->getSerializationId();
                    }
                }
            }

            for (PrefabSerializationData const& prefabData : _prefabs)
            {
                if (prefabData.prefabInstance->getEntityRaw()->getSerializationId() != c_invalidSerializationId)
                {
                    if (usedIds.count(prefabData.prefabInstance->getEntityRaw()->getSerializationId()) == 0)
                        usedIds.insert(prefabData.prefabInstance->getEntityRaw()->getSerializationId());
                    else
                        prefabData.prefabInstance->getEntityRaw()->setSerializationId(c_invalidSerializationId);
                }

                if (prefabData.prefabInstance->getEntityRaw()->getSerializationId() == c_invalidSerializationId)
                    unassginedPrefabInstances.push_back(prefabData.prefabInstance);
                else
                {
                    Entity* prefabEntity = prefabData.prefabInstance->getEntityRaw();
                    if (prefabEntity->getSerializationId() > maxSerializationId)
                        maxSerializationId = Math::Max(maxSerializationId, prefabEntity->getSerializationId());

                    _outPointerIndices[prefabEntity] = prefabEntity->getSerializationId();
                    _outEntityIndices[prefabEntity->getId()] = prefabEntity->getSerializationId();
                }
            }

            // Assign serialization ids for unassigned
            for (Entity* entity : unassignedEntities)
            {
                entity->setSerializationId(++maxSerializationId);

                _outPointerIndices[entity] = entity->getSerializationId();
                _outEntityIndices[entity->getId()] = entity->getSerializationId();
            }


            for (Component* comp : unassginedComponents)
            {
                comp->setSerializationId(++maxSerializationId);
                _outPointerIndices[comp] = comp->getSerializationId();
            }

            for (PrefabInstance* prefabInstance : unassginedPrefabInstances)
            {
                Entity* entity = prefabInstance->getEntityRaw();
                entity->setSerializationId(++maxSerializationId);

                _outPointerIndices[entity] = entity->getSerializationId();
                _outEntityIndices[entity->getId()] = entity->getSerializationId();
            }
            
            // Sort entities
            std::sort(_entityComponents.begin(), _entityComponents.end(),
                [](EntitySerializationData const& _first, EntitySerializationData const& _second)
                {
                    return _first.entity->getSerializationId() < _second.entity->getSerializationId();
                });

            // Sort prefabs
            std::sort(_prefabs.begin(), _prefabs.end(),
                [](PrefabSerializationData const& _first, PrefabSerializationData const& _second)
                {
                    return _first.prefabInstance->getEntityRaw()->getSerializationId() < _second.prefabInstance->getEntityRaw()->getSerializationId();
                });
        }


        if (!_prefabs.empty())
            _outIdentityPrefabsWorld = EcsWorld::Create(
                MAZE_HS("PrefabSerialization"),
                false);

        for (auto& prefabInstanceData : _prefabs)
        {
            PrefabInstance* prefabInstance = prefabInstanceData.prefabInstance;
            /*
            S32 entityIndex = ++indexCounter;
            _outPointerIndices[prefabInstance->getEntityRaw()] = entityIndex;
            _outEntityIndices[prefabInstance->getEntityId()] = entityIndex;
            */

            auto it = _outIdentityPrefabs.find(prefabInstance->getAssetUnitId());
            if (it == _outIdentityPrefabs.end())
            {
                // #TODO: Optimize
                HashedString const& prefabName = AssetUnitManager::GetInstancePtr()->getAssetUnitName(
                    prefabInstance->getAssetUnitId());

                EntityPtr identityPrefab = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                    prefabName.getString(),
                    _outIdentityPrefabsWorld.get());

                MAZE_ERROR_IF(!identityPrefab, "Prefab '%s' (auid=%u) is not found!",
                    prefabName.getString().c_str(),
                    prefabInstance->getAssetUnitId());

                _outIdentityPrefabs.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(prefabInstance->getAssetUnitId()),
                    std::forward_as_tuple(identityPrefab));
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::saveComponentToDataBlockDefault(
        EntitiesToDataBlockContext& _context,
        ComponentPtr const& _component,
        DataBlock& _componentBlock) const
    {
        auto propertyValueIndexIt = _context.pointerIndices.find(_component.get());

        MetaClass const* metaClass = _component->getMetaClass();
        MetaInstance metaInstance = _component->getMetaInstance();

        for (MetaClass* metaClass : metaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);

                HashedCString propertyName = metaProperty->getName();

                ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                if (metaPropertyUID != 0)
                {
                    MetaClass const* metaPropertyMetaClass = metaProperty->getMetaClass();
                    if (metaPropertyMetaClass)
                    {
                        if (metaPropertyMetaClass->isInheritedFrom<Component>())
                        {
                            void* propertyValuePointer = metaProperty->getSharedPtrPointer(metaInstance);
                            if (propertyValuePointer)
                            {
                                EcsSerializationId propertyValueIndex = _context.pointerIndices[propertyValuePointer];
                                EcsHelper::EnsureComponentBlock(_componentBlock, propertyName)->setS32(MAZE_HCS("value"), propertyValueIndex);
                            }
                            continue;
                        }
                        else
                        if (metaPropertyMetaClass == Entity::GetMetaClass())
                        {
                            void* propertyValuePointer = metaProperty->getSharedPtrPointer(metaInstance);
                            if (propertyValuePointer)
                            {
                                EcsSerializationId propertyValueIndex = _context.pointerIndices[propertyValuePointer];
                                EcsHelper::EnsureEntityIdBlock(_componentBlock, propertyName)->setS32(MAZE_HCS("value"), propertyValueIndex);
                            }
                            continue;
                        }
                    }
                    else
                    if (metaPropertyUID == ClassInfo<EntityId>::UID())
                    {
                        EntityId eid;
                        metaProperty->getValue(metaInstance, eid);
                        EcsSerializationId propertyValueIndex = _context.entityIndices[eid];
                        EcsHelper::EnsureEntityIdBlock(_componentBlock, propertyName)->setS32(MAZE_HCS("value"), propertyValueIndex);
                        continue;
                    }
                    else
                    if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                    {
                        Vector<ComponentPtr> comps;
                        metaProperty->getValue<Vector<ComponentPtr>>(metaInstance, comps);

                        if (!comps.empty())
                        {
                            Vector<EcsSerializationId> compIndices;
                            compIndices.resize(comps.size());
                            for (S32 i = 0, in = S32(comps.size()); i < in; ++i)
                                compIndices[i] = comps[i]->getSerializationId();

                            Char newPropertyName[128];
                            StringHelper::FormatString(newPropertyName, sizeof(newPropertyName), "%s:Array<EntityId>", propertyName);
                            AddDataToDataBlock(_componentBlock, HashedCString(newPropertyName), compIndices);
                        }

                        continue;
                    }
                    else
                    if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                    {
                        Vector<EntityPtr> ents;
                        metaProperty->getValue<Vector<EntityPtr>>(metaInstance, ents);

                        if (!ents.empty())
                        {
                            Vector<EcsSerializationId> entIndices;
                            entIndices.resize(ents.size());
                            for (S32 i = 0, in = S32(ents.size()); i < in; ++i)
                                entIndices[i] = ents[i]->getSerializationId();

                            Char newPropertyName[128];
                            StringHelper::FormatString(newPropertyName, sizeof(newPropertyName), "%s:Array<Component>", propertyName);
                            AddDataToDataBlock(_componentBlock, HashedCString(newPropertyName), entIndices);
                        }

                        continue;
                    }
                }

                DataBlockHelper::SerializeMetaPropertyToDataBlock(metaInstance, metaProperty, _componentBlock);
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::saveComponentModificationsToDataBlockDefault(
        EntitiesToDataBlockContext& _context,
        ComponentPtr const& _component,
        ComponentPtr const& _identityComponent,
        DataBlock& _prefabBlock) const
    {
        MetaClass const* metaClass = _component->getMetaClass();
        MetaInstance metaInstance = _component->getMetaInstance();
        MetaInstance identityComponentMetaInstance = _identityComponent->getMetaInstance();

        for (MetaClass* metaClass : metaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);

                HashedCString propertyName = metaProperty->getName();

                if (!metaProperty->hasOperatorEquals())
                    continue;

                if (!metaProperty->isEqual(metaInstance, identityComponentMetaInstance))
                {
                    MetaClass const* metaPropertyMetaClass = metaProperty->getMetaClass();
                    if (metaPropertyMetaClass && (metaPropertyMetaClass->isInheritedFrom<Component>() || metaPropertyMetaClass->isInheritedFrom<Entity>()))
                    {
                        void* propertyValuePointer = metaProperty->getSharedPtrPointer(metaInstance);
                        if (propertyValuePointer)
                        {
                            auto propertyValueIndexIt = _context.pointerIndices.find(propertyValuePointer);
                            if (propertyValueIndexIt != _context.pointerIndices.end())
                            {
                                EcsSerializationId propertyValueIndex = propertyValueIndexIt->second;

                                DataBlock* modificationBlock = _prefabBlock.addNewDataBlock(MAZE_HCS("modification"));
                                modificationBlock->setCString(MAZE_HCS("component"), metaClass->getName());
                                if (strcmp(_component->getClassQualifiedName().str, _component->getComponentClassName()) != 0)
                                    modificationBlock->setCString(MAZE_HCS("_ct"), _component->getComponentClassName());

                                modificationBlock->setCString(MAZE_HCS("property"), propertyName);
                                modificationBlock->setS32(MAZE_HCS("value"), propertyValueIndex);
                            }
                        }

                        continue;
                    }
                    else
                    {
                        DataBlock* modificationBlock = _prefabBlock.addNewDataBlock(MAZE_HCS("modification"));
                        modificationBlock->setCString(MAZE_HCS("component"), metaClass->getName());
                        if (strcmp(_component->getClassQualifiedName().str, _component->getComponentClassName()) != 0)
                            modificationBlock->setCString(MAZE_HCS("_ct"), _component->getComponentClassName());

                        modificationBlock->setCString(MAZE_HCS("property"), propertyName);
                        DataBlockHelper::SerializeMetaPropertyToDataBlock(metaInstance, metaProperty, MAZE_HCS("value"), *modificationBlock);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::saveEntitiesToDataBlock(
        EntitiesToDataBlockContext& _context,
        DataBlock& _dataBlock) const
    {
        auto saveComponentFunc =
            [&](EntitiesToDataBlockContext& _context, ComponentPtr const& _component, DataBlock& _parentBlock)
            {
                DataBlock* componentBlock = _parentBlock.addNewDataBlock(MAZE_HCS("component"));
                componentBlock->setCString(MAZE_HCS("_t"), static_cast<CString>(_component->getMetaClass()->getName()));
                if (strcmp(_component->getClassQualifiedName().str, _component->getComponentClassName()) != 0)
                    componentBlock->setCString(MAZE_HCS("_ct"), static_cast<CString>(_component->getComponentClassName()));

                ClassUID componentUID = _component->getClassUID();
                auto it = m_componentCustomSerializationByClassUID.find(componentUID);
                if (it == m_componentCustomSerializationByClassUID.end())
                    saveComponentToDataBlockDefault(_context, _component, *componentBlock);
                else
                    it->second.toDataBlockFunc(_context, _component, *componentBlock);
            };

        // Entities
        for (auto const& entityComponentsData : _context.entityComponents)
        {
            EntityPtr const& entity = entityComponentsData.entity;
            Vector<ComponentPtr> const& components = entityComponentsData.components;

            DataBlock* entityBlock = _dataBlock.addNewDataBlock(MAZE_HCS("entity"));
            entityBlock->setS32(MAZE_HCS("_sid"), entityComponentsData.entity->getSerializationId());

            if (!entity->getActiveSelf())
                entityBlock->setBool(MAZE_HCS("active"), entity->getActiveSelf());

            for (ComponentPtr const& component : components)
                saveComponentFunc(_context, component, *entityBlock);
        }

        // Prefabs
        for (auto& prefabInstanceData : _context.prefabs)
        {
            PrefabInstance* prefabInstance = prefabInstanceData.prefabInstance;

            EntityPtr const& prefabEntity = prefabInstance->getEntity();
            EntityPtr const& identityPrefabEntity = _context.identityPrefabs[prefabInstance->getAssetUnitId()];

            MAZE_ERROR_CONTINUE_IF(
                !identityPrefabEntity,
                "Failed to find identity prefab for entity!\n"
                "prefab auid=%u\n"
                "entity name=%s",
                prefabInstance->getAssetUnitId(),
                EcsHelper::GetName(prefabEntity.get()));

            DataBlock* entityBlock = _dataBlock.addNewDataBlock(MAZE_HCS("prefabInstance"));
            entityBlock->setS32(MAZE_HCS("_sid"), prefabEntity->getSerializationId());
            entityBlock->setU32(MAZE_HCS("source"), (U32)prefabInstance->getAssetUnitId());

            if (!prefabEntity->getActiveSelf())
                entityBlock->setBool(MAZE_HCS("active"), prefabEntity->getActiveSelf());

            for (auto const& componentData : prefabEntity->getComponents())
            {
                if (componentData.first == ClassInfo<PrefabInstance>::UID())
                    continue;

                if (m_componentsToIgnore.count(componentData.first) > 0)
                    continue;

                if (componentData.second->getFlag(Component::Flags::SerializationDisabled))
                    continue;

                ComponentPtr const& identityComponent = identityPrefabEntity->getComponentById(componentData.first);
                if (identityComponent)
                {
                    ClassUID componentUID = componentData.second->getClassUID();
                    auto it = m_componentCustomSerializationByClassUID.find(componentUID);
                    if (it == m_componentCustomSerializationByClassUID.end())
                        saveComponentModificationsToDataBlockDefault(_context, componentData.second, identityComponent, *entityBlock);
                    else
                        it->second.modificationsToDataBlockFunc(_context, componentData.second, identityComponent, *entityBlock);
                }
                else
                    saveComponentFunc(_context, componentData.second, *entityBlock);
            }
        }

        replaceDataBlockEcsIds(_dataBlock, _context.entityIndices, _context.entityComponents, _context.pointerIndices, _context.ecsWorld);
    }

    //////////////////////////////////////////
    void EntitySerializationManager::replaceDataBlockEcsIds(
        DataBlock& _dataBlock,
        Map<EntityId, EcsSerializationId> const& _entityIndices,
        Vector<EntitySerializationData> const& _entityComponents,
        Map<void*, EcsSerializationId>& _pointerIndices,
        EcsWorld* _ecsWorld) const
    {
        for (DataBlock* subBlock : _dataBlock)
        {
            if (StringHelper::IsEndsWith(subBlock->getName().str, ":EntityId"))
            {
                EntityId eid = EntityId(subBlock->getS32(MAZE_HCS("value"), (S32)c_invalidEntityId));
                if (eid != c_invalidEntityId)
                {
                    auto it = _entityIndices.find(eid);
                    if (it != _entityIndices.end())
                    {
                        subBlock->setS32(MAZE_HCS("value"), it->second);
                    }
                    else
                    {
                        Entity* entity = _ecsWorld->getEntity(eid).get();
                        MAZE_ERROR_CONTINUE_IF(!entity, "Entity is not found: %d", (S32)eid);
                        EcsSerializationId entityIndex = _pointerIndices[entity];

                        if (entityIndex == c_invalidSerializationId)
                        {
                            PrefabInstance* prefabInstance = EcsHelper::GetFirstTrunkComponent<PrefabInstance>(entity);
                            if (prefabInstance)
                            {
                                EcsSerializationId prefabEntityIndex = prefabInstance->getEntityRaw()->getSerializationId();
                                MAZE_ERROR_CONTINUE_IF(prefabEntityIndex == c_invalidSerializationId, "Potential prefab instance is invalid");
                                subBlock->removeParam(MAZE_HCS("value"));
                                subBlock->setS32(MAZE_HCS("prefabSid"), prefabEntityIndex);
                                subBlock->setS32(MAZE_HCS("prefabEntitySid"), entity->getSerializationId());
                            }
                            else
                            {
                                MAZE_ERROR("Entity is not found: %d", (S32)eid);
                            }
                        }
                        else
                        {
                            subBlock->setS32(MAZE_HCS("value"), entityIndex);
                        }
                    }
                }
            }
            else
            {
                replaceDataBlockEcsIds(*subBlock, _entityIndices, _entityComponents, _pointerIndices, _ecsWorld);
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::restoreDataBlockEcsIds(
        DataBlock& _dataBlock,
        Map<EcsSerializationId, EntityPtr>& _outEntities,
        Map<EcsSerializationId, ComponentPtr>& _outComponents) const
    {
        for (DataBlock* subBlock : _dataBlock)
        {
            if (StringHelper::IsEndsWith(subBlock->getName().str, ":EntityId"))
            {
                DataBlock::ParamIndex paramIndex = subBlock->findParamIndex(MAZE_HCS("value"));
                if (paramIndex >= 0)
                {
                    EcsSerializationId entityIndex = subBlock->getS32(paramIndex);
                    if (entityIndex != c_invalidSerializationId)
                    {
                        EntityPtr const& entity = _outEntities[entityIndex];
                        if (entity)
                            subBlock->setS32(MAZE_HCS("value"), (S32)entity->getId());
                        else
                            subBlock->setS32(MAZE_HCS("value"), (S32)c_invalidEntityId);
                    }
                    else
                        subBlock->setS32(MAZE_HCS("value"), (S32)c_invalidEntityId);
                }
                else
                {
                    EcsSerializationId prefabSid = subBlock->getS32(MAZE_HCS("prefabSid"), c_invalidSerializationId);
                    if (prefabSid != c_invalidSerializationId)
                    {
                        auto it = _outEntities.find(prefabSid);
                        MAZE_WARNING_CONTINUE_IF(it == _outEntities.end(), "Undefined prefab with sid=%d", prefabSid);

                        EcsSerializationId prefabEntitySid = subBlock->getS32(MAZE_HCS("prefabEntitySid"), c_invalidSerializationId);
                        if (prefabEntitySid != c_invalidSerializationId)
                        {
                            Entity* prefabEntity = it->second.get();
                            Entity* targetEntity = EcsHelper::FindEntityWithSerializationId(prefabEntity, prefabEntitySid);
                            MAZE_WARNING_CONTINUE_IF(!targetEntity, "Undefined prefab with prefabSid=%d and prefabEntitySid=%d", prefabSid, prefabEntitySid);
                            subBlock->removeParam(MAZE_HCS("prefabSid"));
                            subBlock->removeParam(MAZE_HCS("prefabEntitySid"));
                            subBlock->setS32(MAZE_HCS("value"), (S32)targetEntity->getId());
                        }
                    }
                }
            }
            else
            if (StringHelper::IsEndsWith(subBlock->getName().str, ":Array<EntityId>"))
            {
                for (DataBlock::ParamIndex i = 0; i < (DataBlock::ParamIndex)subBlock->getParamsCount(); ++i)
                {
                    DataBlockParamType paramType = subBlock->getParamType(i);
                    if (paramType == DataBlockParamType::ParamS32)
                    {
                        EcsSerializationId entityIndex = subBlock->getS32(i);
                        if (entityIndex != c_invalidSerializationId)
                        {
                            EntityPtr const& entity = _outEntities[entityIndex];
                            if (entity)
                                subBlock->setS32(i, (S32)entity->getId());
                            else
                                subBlock->setS32(i, (S32)c_invalidEntityId);
                        }
                        else
                            subBlock->setS32(i, (S32)c_invalidEntityId);
                    }
                }
            }
            else
            {
                restoreDataBlockEcsIds(*subBlock, _outEntities, _outComponents);
            }
        }
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::saveEntitiesToDataBlock(Set<Entity*> const& _entities, DataBlock& _dataBlock) const
    {
        if (_entities.empty())
            return false;

        EntitiesToDataBlockContext context;
        context.ecsWorld = (*_entities.begin())->getEcsWorld();
        collectEntitiesComponentsMap(_entities, context.entityComponents, context.prefabs);

        EcsWorldPtr identityPrefabsWorld;
        PrepareEntitiesToSerialize(
            context.entityComponents, context.prefabs, context.pointerIndices, context.entityIndices, context.identityPrefabs, identityPrefabsWorld);

        saveEntitiesToDataBlock(context, _dataBlock);

        _dataBlock.setS32(MAZE_HCS("_version"), c_enititySerializationVersion);


        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::savePrefabToDataBlock(EntityPtr const& _entity, DataBlock& _dataBlock) const
    {
        if (!_entity)
            return false;

        EntitiesToDataBlockContext context;
        context.ecsWorld = _entity->getEcsWorld();
        collectEntityComponentsMap(_entity, context.entityComponents, context.prefabs);

        EcsWorldPtr identityPrefabsWorld;
        PrepareEntitiesToSerialize(
            context.entityComponents, context.prefabs, context.pointerIndices, context.entityIndices, context.identityPrefabs, identityPrefabsWorld);

        saveEntitiesToDataBlock(context, _dataBlock);

        _dataBlock.setS32(MAZE_HCS("_rootIndex"), context.pointerIndices[_entity.get()]);
        _dataBlock.setS32(MAZE_HCS("_version"), c_enititySerializationVersion);

        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::saveSceneToDataBlock(EcsScenePtr const& _scene, DataBlock& _dataBlock) const
    {
        _scene->serializeSceneCommonInfo(_dataBlock);

        DataBlock* entitiesBlock = _dataBlock.addNewDataBlock(MAZE_HCS("entities"));

        if (!saveEntitiesToDataBlock(
            _scene->getEntities(),
            *entitiesBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::savePrefabToDataBlockFile(EntityPtr const& _entity, Path const& _fileFullPath) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::savePrefabToDataBlockFile");

        if (!_entity)
            return false;

        if (_fileFullPath.empty())
            return false;

        Path directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
        FileHelper::CreateDirectoryRecursive(directoryFullPath);

        DataBlock dataBlock;
        if (!savePrefabToDataBlock(_entity, dataBlock))
            return false;

        dataBlock.saveTextFile(_fileFullPath);

        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::saveSceneToDataBlockFile(EcsScenePtr const& _scene, Path const& _fileFullPath) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::savePrefabToDataBlockFile");

        if (!_scene)
            return false;

        if (_fileFullPath.empty())
            return false;

        Path directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
        FileHelper::CreateDirectoryRecursive(directoryFullPath);

        DataBlock dataBlock;
        if (!saveSceneToDataBlock(_scene, dataBlock))
            return false;


        dataBlock.saveTextFile(_fileFullPath);

        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::loadSceneFromDataBlock(EcsScenePtr const& _scene, DataBlock& _dataBlock) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadSceneFromDataBlock");

        if (!_scene)
            return false;

        _scene->deserializeSceneCommonInfo(_dataBlock);

        DataBlock* entitiesBlock = _dataBlock.getDataBlock(MAZE_HCS("entities"));
        if (entitiesBlock)
        {
            EntitiesFromDataBlockContext context;
            context.world = _scene->getWorld();
            context.scene = _scene.get();
            loadEntities(*entitiesBlock, context);
        }

        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::loadSceneFromDataBlockFile(EcsScenePtr const& _scene, Path const& _fileFullPath) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadSceneFromDataBlockFile");

        if (!_scene)
            return false;

        if (_fileFullPath.empty())
            return false;

        AssetFilePtr const& file = AssetManager::GetInstancePtr()->getAssetFileByFullPath(_fileFullPath);
        if (!file)
            return false;

        DataBlock dataBlock = file->readAsDataBlock();
        if (!loadSceneFromDataBlock(_scene, dataBlock))
            return false;

        return true;
    }


    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        AssetFilePtr const& _assetFile,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        DataBlock dataBlock = _assetFile->readAsDataBlock();

        MAZE_WARNING_IF(dataBlock.getS32(MAZE_HCS("_version")) == 0, "Invalid prefab with no version!");

        return loadPrefab(dataBlock, _world, _scene);
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        Path const& _assetFileName,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_assetFileName);
        if (!assetFile)
            return EntityPtr();

        return loadPrefab(assetFile, _world, _scene);
    }

    //////////////////////////////////////////
    void EntitySerializationManager::loadComponentFromDataBlockDefault(
        DataBlock const& _componentBlock,
        ComponentPtr const& _component,
        EntitiesFromDataBlockContext& _context) const
    {
        MetaClass const* componentMetaClass = _component->getMetaClass();
        MetaInstance componentMetaInstance = _component->getMetaInstance();

        for (Maze::MetaClass* metaClass : componentMetaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                Maze::MetaProperty* metaProperty = metaClass->getProperty(i);

                HashedCString propertyName = metaProperty->getName();

                {
                    ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                    if (metaPropertyUID != 0)
                    {
                        if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                        {
                            DataBlock const* cmpBlock = EcsHelper::GetComponentBlock(_componentBlock, propertyName.str);
                            if (cmpBlock)
                            {
                                EcsSerializationId valueIndex = cmpBlock->getS32(MAZE_HCS("value"));

                                MAZE_ERROR_IF(
                                    valueIndex > 0 && !_context.outComponents[valueIndex],
                                    "Component %d is not found! propertyName=%s",
                                    valueIndex,
                                    propertyName.str);

                                metaProperty->setValue(componentMetaInstance, &_context.outComponents[valueIndex]);
                            }
                            else
                            // #TODO: OBSOLETE, remove later
                            if (_componentBlock.isParamExists(propertyName))
                            {
                                MAZE_ERROR("Obsolete component param: %s!", propertyName.str);
                                S32 valueIndex = _componentBlock.getS32(propertyName);
                                metaProperty->setValue(componentMetaInstance, &_context.outComponents[valueIndex]);
                            }

                            continue;
                        }
                        else
                        if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                        {
                            DataBlock const* entityIdBlock = EcsHelper::GetEntityIdBlock(_componentBlock, propertyName.str);
                            if (entityIdBlock)
                            {
                                EntityId entityId(entityIdBlock->getS32(MAZE_HCS("value")));
                                metaProperty->setValue(componentMetaInstance, &_context.entitiesPerEntityId[entityId]);

                                MAZE_ERROR_IF(
                                    entityId != c_invalidEntityId && !_context.entitiesPerEntityId[entityId],
                                    "Entity %d is not found! propertyName=%s",
                                    (S32)entityId,
                                    propertyName.str);
                            }
                            else
                            // #TODO: OBSOLETE, remove later
                            if (_componentBlock.isParamExists(propertyName))
                            {
                                MAZE_ERROR("Obsolete eid param: %s!", propertyName.str);
                                S32 valueIndex = _componentBlock.getS32(propertyName);
                                metaProperty->setValue(componentMetaInstance, &_context.outEntities[valueIndex]);
                            }
                            continue;
                        }
                        else
                        if (metaPropertyUID == ClassInfo<EntityId>::UID())
                        {
                            DataBlock const* entityIdBlock = EcsHelper::GetEntityIdBlock(_componentBlock, propertyName.str);
                            if (entityIdBlock)
                            {
                                EntityId entityId(entityIdBlock->getS32(MAZE_HCS("value")));
                                metaProperty->setValue(componentMetaInstance, &entityId);

                                MAZE_ERROR_IF(
                                    entityId != c_invalidEntityId && !_context.entitiesPerEntityId[entityId],
                                    "Entity %d is not found! propertyName=%s",
                                    (S32)entityId,
                                    propertyName.str);
                            }
                            else
                            {
                                MAZE_ERROR("Wrong syntax");
                            }
                        }
                        else
                        if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                        {
                            DataBlock const* cmpArrayBlock = EcsHelper::GetComponentArrayBlock(_componentBlock, propertyName.str);
                            if (cmpArrayBlock)
                            {
                                Vector<EcsSerializationId> componentsIndices;
                                ValueFromDataBlock(componentsIndices, *cmpArrayBlock);

                                Vector<ComponentPtr> componentsValue;
                                componentsValue.resize(componentsIndices.size());
                                for (Size i = 0, in = componentsIndices.size(); i < in; ++i)
                                {
                                    MAZE_ERROR_IF(
                                        componentsIndices[i] > 0 && !_context.outComponents[componentsIndices[i]],
                                        "Component %d is not found! propertyName=%s",
                                        componentsIndices[i],
                                        propertyName.str);

                                    componentsValue[i] = _context.outComponents[componentsIndices[i]];
                                }
                                metaProperty->setValue(componentMetaInstance, &componentsValue);
                            }
                            else
                            {
                                DataBlock const* propertyBlock = _componentBlock.getDataBlock(propertyName);
                                if (propertyBlock)
                                {
                                    MAZE_ERROR("Obsolete component param: %s!", propertyName.str);

                                    Vector<EcsSerializationId> componentsIndices;
                                    ValueFromDataBlock(componentsIndices, *propertyBlock);

                                    Vector<ComponentPtr> componentsValue;
                                    componentsValue.resize(componentsIndices.size());
                                    for (Size i = 0, in = componentsIndices.size(); i < in; ++i)
                                        componentsValue[i] = _context.outComponents[componentsIndices[i]];
                                    metaProperty->setValue(componentMetaInstance, &componentsValue);
                                }
                                else
                                {
                                    MAZE_ERROR("Invalid property '%s'!", propertyName.str);
                                }
                            }

                            continue;
                        }
                        else
                        if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                        {
                            DataBlock const* entityIdArrayBlock = EcsHelper::GetEntityIdArrayBlock(_componentBlock, propertyName.str);
                            if (entityIdArrayBlock)
                            {
                                Vector<EcsSerializationId> entitiesIds;
                                ValueFromDataBlock(entitiesIds, *entityIdArrayBlock);

                                Vector<EntityPtr> entitiesValue;
                                entitiesValue.resize(entitiesIds.size());
                                for (Size i = 0, in = entitiesIds.size(); i < in; ++i)
                                {
                                    EntityId entityId(entitiesIds[i]);
                                    entitiesValue[i] = _context.entitiesPerEntityId[entityId];
                                    MAZE_ERROR_IF(
                                        entityId != c_invalidEntityId && !entitiesValue[i],
                                        "Entity %d is not found! propertyName=%s",
                                        entitiesIds[i],
                                        propertyName.str);
                                }
                                metaProperty->setValue(componentMetaInstance, &entitiesValue);
                            }
                            // #TODO: OBSOLETE, remove later
                            else
                            {
                                DataBlock const* propertyBlock = _componentBlock.getDataBlock(propertyName);
                                if (propertyBlock)
                                {
                                    MAZE_ERROR("Obsolete eid array block: %s!", propertyName.str);

                                    Vector<EcsSerializationId> entitiesIndices;
                                    ValueFromDataBlock(entitiesIndices, *propertyBlock);

                                    Vector<EntityPtr> entitiesValue;
                                    entitiesValue.resize(entitiesIndices.size());
                                    for (Size i = 0, in = entitiesIndices.size(); i < in; ++i)
                                        entitiesValue[i] = _context.outEntities[entitiesIndices[i]];
                                    metaProperty->setValue(componentMetaInstance, &entitiesValue);
                                }
                                else
                                {
                                    MAZE_ERROR("Invalid property '%s'", propertyName.str);
                                }
                            }
                            continue;
                        }
                    }

                    DataBlockHelper::DeserializeMetaPropertyFromDataBlock(componentMetaInstance, metaProperty, _componentBlock);
                }


            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::loadComponentModificationFromDataBlockDefault(
        DataBlock const& _modificationBlock,
        ComponentPtr const& _component,
        CString _propertyName,
        EntitiesFromDataBlockContext& _context) const
    {
        MetaProperty* metaProperty = _component->getMetaClass()->getProperty(_propertyName);
        if (metaProperty)
        {
            ClassUID metaPropertyUID = metaProperty->getValueClassUID();
            if (metaPropertyUID != 0)
            {
                if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                {
                    EcsSerializationId valueIndex = _modificationBlock.getS32(MAZE_HCS("value"));
                    metaProperty->setValue(_component->getMetaInstance(), &_context.outComponents[valueIndex]);
                    return;
                }
                else
                if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                {
                    EcsSerializationId valueIndex = _modificationBlock.getS32(MAZE_HCS("value"));
                    metaProperty->setValue(_component->getMetaInstance(), &_context.outEntities[valueIndex]);
                    return;
                }
                else
                if (metaPropertyUID == ClassInfo<EntityId>::UID())
                {
                    EcsSerializationId valueIndex = _modificationBlock.getS32(MAZE_HCS("value"));
                    EntityPtr const& entity = _context.outEntities[valueIndex];
                    EntityId entityId = entity ? entity->getId() : c_invalidEntityId;
                    metaProperty->setValue(_component->getMetaInstance(), &entityId);
                    return;
                }
                else
                if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                {
                    DataBlock const* propertyBlock = _modificationBlock.getDataBlock(MAZE_HCS("value"));
                    if (propertyBlock)
                    {
                        Vector<EcsSerializationId> componentsIndices;
                        ValueFromDataBlock(componentsIndices, *propertyBlock);

                        Vector<ComponentPtr> componentsValue;
                        componentsValue.resize(componentsIndices.size());
                        for (Size i = 0, in = componentsIndices.size(); i < in; ++i)
                            componentsValue[i] = _context.outComponents[componentsIndices[i]];
                        metaProperty->setValue(_component->getMetaInstance(), &componentsValue);
                    }
                    else
                    {
                        MAZE_ERROR("Invalid property 'value'!");
                    }
                    return;
                }
                else
                if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                {
                    DataBlock const* propertyBlock = _modificationBlock.getDataBlock(MAZE_HCS("value"));
                    if (propertyBlock)
                    {
                        Vector<EcsSerializationId> entitiesIndices;
                        ValueFromDataBlock(entitiesIndices, *propertyBlock);

                        Vector<EntityPtr> entitiesValue;
                        entitiesValue.resize(entitiesIndices.size());
                        for (Size i = 0, in = entitiesIndices.size(); i < in; ++i)
                            entitiesValue[i] = _context.outEntities[entitiesIndices[i]];
                        metaProperty->setValue(_component->getMetaInstance(), &entitiesValue);
                    }
                    else
                    {
                        MAZE_ERROR("Invalid property 'value'");
                    }
                    return;
                }
            }

            DataBlockHelper::DeserializeMetaPropertyFromDataBlock(_component->getMetaInstance(), metaProperty, MAZE_HCS("value"), _modificationBlock);
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::loadEntities(
        DataBlock& _dataBlock,
        EntitiesFromDataBlockContext& _context) const
    {
        _context.scene = _context.scene ? _context.scene
            : (_context.world ? nullptr : SceneManager::GetInstancePtr()->getMainScene().get());

        _context.world = _context.world ? _context.world
            : (_context.scene ? _context.scene->getWorld() : nullptr);

        ComponentFactoryPtr const& factory = EntityManager::GetInstancePtr()->getComponentFactory();

        EcsSerializationId autoIndexCounter = 0;
        EcsSerializationId maxSerializationId = 0;

        // Prepare
        {
            for (DataBlock const* subBlock : _dataBlock)
            {
                if (subBlock->isComment())
                    continue;

                if (subBlock->getName() == MAZE_HCS("entity"))
                {
                    EcsSerializationId entityIndex = subBlock->getS32(MAZE_HCS("_sid"));
                    if (entityIndex == c_invalidSerializationId)
                        entityIndex = --autoIndexCounter;
                    else
                        maxSerializationId = Math::Max(maxSerializationId, entityIndex);

                    EntityPtr entity = Entity::Create();
                    entity->setSerializationId(entityIndex);
                    entity->setAwakeForbidden(true);
                    _context.world->addEntity(entity);

                    _context.outEntities[entityIndex] = entity;

                    for (DataBlock const* componentBlock : *subBlock)
                    {
                        if (componentBlock->isComment())
                            continue;

                        if (componentBlock->getName() != MAZE_HCS("component"))
                            continue;

                        S32 componentIndex = componentBlock->getS32(MAZE_HCS("_sid"));
                        if (componentIndex == c_invalidSerializationId)
                            componentIndex = --autoIndexCounter;
                        else
                            maxSerializationId = Math::Max(maxSerializationId, entityIndex);

                        CString componentClassName = componentBlock->getCString(MAZE_HCS("_t"));
                        HashedCString componentDynamicClassName = componentBlock->getHashedCString(MAZE_HCS("_ct"));
                        ComponentId componentId = !componentDynamicClassName.empty() ? GetComponentIdByName(componentDynamicClassName)
                                                                                     : factory->getComponentId(componentClassName);
                        ComponentPtr component = entity->getComponentById(componentId);
                        if (!component)
                        {
                            component = !componentDynamicClassName.empty() ? factory->createDynamicComponent(componentClassName, componentDynamicClassName)
                                                                           : factory->createComponent(componentId);

                            if (!component)
                            {
                                MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                continue;
                            }
                            entity->addComponent(component);
                        }

                        if (component)
                            component->setSerializationId(componentIndex);

                        _context.outComponents[componentIndex] = component;
                    }
                }
                else
                if (subBlock->getName() == MAZE_HCS("prefabInstance"))
                {
                    EcsSerializationId entityIndex = subBlock->getS32(MAZE_HCS("_sid"));
                    if (entityIndex == c_invalidSerializationId)
                        entityIndex = --autoIndexCounter;
                    else
                        maxSerializationId = Math::Max(maxSerializationId, entityIndex);

                    EntityPtr prefab;
                    auto sourceParamIndex = subBlock->findParamIndex(MAZE_HCS("source"));
                    auto sourceParamType = subBlock->getParamType(sourceParamIndex);
                    
                    if (sourceParamType == DataBlockParamType::ParamU32)
                    {
                        AssetUnitId prefabAssetUnitId = subBlock->getU32(sourceParamIndex);
                        prefab = EntityPrefabManager::GetInstancePtr()->getOrLoadEntityPrefab(prefabAssetUnitId);
                    }
                    else
                    {
                        CString prefabName = subBlock->getCString(MAZE_HCS("source"));
                        if (prefabName)
                            prefab = EntityPrefabManager::GetInstancePtr()->getOrLoadEntityPrefab(prefabName);
                        
                    }
                    

                    if (prefab)
                    {                       
                        EntityPtr entity = EntityPrefabManager::GetInstancePtr()->instantiatePrefab(prefab, _context.world, _context.scene);
                        if (entity == nullptr)
                        {
                            MAZE_ERROR("Entity is nullptr!");
                            continue;
                        }

                        entity->setSerializationId(entityIndex);
                        _context.outEntities[entityIndex] = entity;

                        for (DataBlock const* prefabChildBlock : *subBlock)
                        {
                            if (prefabChildBlock->isComment())
                                continue;

                            if (prefabChildBlock->getName() == MAZE_HCS("modification"))
                            {
                                CString componentClassName = prefabChildBlock->getCString(MAZE_HCS("component"));
                                CString componentPropertyName = prefabChildBlock->getCString(MAZE_HCS("property"));

                                if (componentClassName && componentPropertyName)
                                {
                                    // #TODO: New entities and components
                                }
                            }
                            else
                            if (prefabChildBlock->getName() == MAZE_HCS("component"))
                            {
                                EcsSerializationId componentIndex = prefabChildBlock->getS32(MAZE_HCS("_sid"));
                                if (componentIndex == c_invalidSerializationId)
                                    componentIndex = --autoIndexCounter;
                                else
                                    maxSerializationId = Math::Max(maxSerializationId, entityIndex);

                                CString componentClassName = prefabChildBlock->getCString(MAZE_HCS("_t"));
                                HashedCString componentDynamicClassName = prefabChildBlock->getHashedCString(MAZE_HCS("_ct"));
                                ComponentId componentId = !componentDynamicClassName.empty() ? GetComponentIdByName(componentDynamicClassName)
                                                                                             : factory->getComponentId(componentClassName);
                                ComponentPtr component = entity->getComponentById(componentId);
                                if (!component)
                                {
                                    component = !componentDynamicClassName.empty() ? factory->createDynamicComponent(componentClassName, componentDynamicClassName)
                                                                                   : factory->createComponent(componentId);

                                    if (!component)
                                    {
                                        MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                        continue;
                                    }

                                    entity->addComponent(component);
                                }

                                if (component)
                                    component->setSerializationId(componentIndex);

                                _context.outComponents[componentIndex] = component;
                            }
                        }
                    }
                    else
                    {
                        MAZE_ERROR("PrefabInstance without source!");
                    }
                }

            }
        }

        autoIndexCounter = 0;


        // Fix auto indices
        for (Map<EcsSerializationId, EntityPtr>::iterator it = _context.outEntities.begin(), end = _context.outEntities.end(); it != end;)
        {
            if (it->first < 0)
            {
                it->second->setSerializationId(it->second->getSerializationId() - it->first);
                it = _context.outEntities.erase(it);
                _context.outEntities.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(it->second->getSerializationId()),
                    std::forward_as_tuple(it->second));
                end = _context.outEntities.end();
            }
            else
                ++it;
        }

        for (Map<EcsSerializationId, ComponentPtr>::iterator it = _context.outComponents.begin(), end = _context.outComponents.end(); it != end;)
        {
            if (it->first < 0)
            {
                it->second->setSerializationId(it->second->getSerializationId() - it->first);
                it = _context.outComponents.erase(it);
                _context.outComponents.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(it->second->getSerializationId()),
                    std::forward_as_tuple(it->second));
                end = _context.outComponents.end();
            }
            else
                ++it;
        }

        for (auto const& entityData : _context.outEntities)
        {
            _context.entitiesPerEntityId.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(entityData.second->getId()),
                std::forward_as_tuple(entityData.second));
        }

        restoreDataBlockEcsIds(_dataBlock, _context.outEntities, _context.outComponents);

        // Load
        {
            for (DataBlock const* subBlock : _dataBlock)
            {
                if (subBlock->isComment())
                    continue;

                if (subBlock->getName() == MAZE_HCS("entity") || subBlock->getName() == MAZE_HCS("prefabInstance"))
                {
                    EcsSerializationId entityIndex = subBlock->getS32(MAZE_HCS("_sid"));
                    EntityPtr const& entity = _context.outEntities[entityIndex];
                    if (entity == nullptr)
                    {
                        MAZE_ERROR("Entity is nullptr!");
                        continue;
                    }

                    entity->setActiveSelf(subBlock->getBool(MAZE_HCS("active"), true));

                    for (DataBlock const* componentBlock : *subBlock)
                    {
                        if (componentBlock->isComment())
                            continue;

                        if (componentBlock->getName() == MAZE_HCS("modification"))
                        {
                            CString componentClassName = componentBlock->getCString(MAZE_HCS("component"));
                            CString componentPropertyName = componentBlock->getCString(MAZE_HCS("property"));

                            if (componentClassName && componentPropertyName)
                            {
                                HashedCString componentDynamicClassName = componentBlock->getHashedCString(MAZE_HCS("_ct"));
                                
                                ComponentId componentId = !componentDynamicClassName.empty() ? GetComponentIdByName(componentDynamicClassName)
                                    : factory->getComponentId(componentClassName);

                                ComponentPtr const& component = entity->getComponentById(componentId);
                                MAZE_WARNING_CONTINUE_IF(!component, "There is no %s component in the entity (entityIndex=%d, property=%s)",
                                    componentClassName, entityIndex, componentPropertyName);

                                ClassUID componentClassUID = component->getClassUID();
                                auto it = m_componentCustomSerializationByClassUID.find(componentClassUID);
                                if (it == m_componentCustomSerializationByClassUID.end())
                                    loadComponentModificationFromDataBlockDefault(*componentBlock, component, componentPropertyName, _context);
                                else
                                    it->second.modificationFromDataBlockFunc(*componentBlock, component, componentPropertyName, _context);
                            }
                        }
                        if (componentBlock->getName() == MAZE_HCS("component"))
                        {
                            EcsSerializationId componentIndex = componentBlock->getS32(MAZE_HCS("_sid"));

                            ComponentPtr const& component = _context.outComponents[componentIndex];
                            if (!component)
                                continue;

                            ClassUID componentClassUID = component->getClassUID();
                            auto it = m_componentCustomSerializationByClassUID.find(componentClassUID);
                            if (it == m_componentCustomSerializationByClassUID.end())
                                loadComponentFromDataBlockDefault(*componentBlock, component, _context);
                            else
                                it->second.fromDataBlockFunc(*componentBlock, component, _context);
                        }
                    }
                }
            }
        }

        // Add to ECS scene
        for (auto entityData : _context.outEntities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setEcsScene(_context.scene);
        }

        // Awake
        for (auto entityData : _context.outEntities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setAwakeForbidden(false);
            entityData.second->tryAwake();
        }
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        DataBlock& _dataBlock,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadPrefab");

        static EntityPtr const nullPointer;

        
        EcsSerializationId rootIndex = _dataBlock.getS32(MAZE_HCS("_rootIndex"), c_invalidSerializationId);
        if (rootIndex == c_invalidSerializationId)
            return nullptr;


        EntitiesFromDataBlockContext context;
        context.world = _world;
        context.scene = _scene;
        loadEntities(_dataBlock, context);

        return context.outEntities[rootIndex];
    }

    //////////////////////////////////////////
    void EntitySerializationManager::collectAllChildrenEntity(
        Entity* _entity,
        Vector<EntityPtr>& _entities,
        Vector<PrefabSerializationData>& _prefabInstances) const
    {
        if (PrefabInstance* prefabInstance = _entity->getComponentRaw<PrefabInstance>())
        {
            PrefabSerializationData data;
            data.prefabInstance = prefabInstance;
            _prefabInstances.emplace_back(data);
            return;
        }
        else
        {
            _entities.emplace_back(_entity->getSharedPtr());
        }

        {
            Transform2D* transform = _entity->getComponentRaw<Transform2D>();
            if (transform)
            {
                for (Transform2D* childTransform : transform->getChildren())
                    collectAllChildrenEntity(childTransform->getEntity().get(), _entities, _prefabInstances);
            }
        }

        {
            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (transform)
            {
                for (Transform3D* childTransform : transform->getChildren())
                    collectAllChildrenEntity(childTransform->getEntity().get(), _entities, _prefabInstances);
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::collectEntitiesComponentsMap(
        Set<Entity*> const& _entities,
        Vector<EntitySerializationData>& _entityComponents,
        Vector<PrefabSerializationData>& _prefabs
    ) const
    {
        Vector<EntityPtr> entities;

        for (auto const& entity : _entities)
        {

            // Skip all child entities
            if (EcsHelper::GetEntityParentId(entity) != c_invalidEntityId)
                continue;

            collectAllChildrenEntity(entity, entities, _prefabs);
        }

        for (EntityPtr const& entity : entities)
        {
            Vector<ComponentPtr> components;
            for (auto const& componentData : entity->getComponents())
            {
                if (m_componentsToIgnore.count(componentData.first) > 0)
                    continue;

                if (componentData.second->getFlag(Component::Flags::SerializationDisabled))
                    continue;

                components.push_back(componentData.second);
            }

            EntitySerializationData data{ entity, components };
            _entityComponents.emplace_back(data);
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::collectEntityComponentsMap(
        EntityPtr const& _entity,
        Vector<EntitySerializationData>& _entityComponents,
        Vector<PrefabSerializationData>& _prefabs) const
    {
        Vector<EntityPtr> entities;

        collectAllChildrenEntity(_entity.get(), entities, _prefabs);

        for (EntityPtr const& entity : entities)
        {
            Vector<ComponentPtr> components;
            for (auto const& componentData : entity->getComponents())
            {
                if (m_componentsToIgnore.count(componentData.first) > 0)
                    continue;

                if (componentData.second->getFlag(Component::Flags::SerializationDisabled))
                    continue;

                components.push_back(componentData.second);
            }

            EntitySerializationData data{ entity, components };
            _entityComponents.emplace_back(data);
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::registerComponentSerializationByClassUID(
        ClassUID _classUID,
        ComponentSerializationFunctions _funcs)
    {
        m_componentCustomSerializationByClassUID.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_classUID),
            std::forward_as_tuple(_funcs));
    }

} // namespace Maze
//////////////////////////////////////////
