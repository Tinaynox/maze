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
        Map<EntityPtr, Vector<ComponentPtr>> const& _entityComponents,
        Vector<PrefabInstance*> const& _prefabs,
        Map<void*, S32>& _outPointerIndices,
        Map<String, EntityPtr>& _outIdentityPrefabs,
        EcsWorldPtr& _outIdentityPrefabsWorld)
    {
        S32 indexCounter = 0;

        // Index all entities and components
        for (auto const& entityComponentsData : _entityComponents)
        {
            EntityPtr const& entity = entityComponentsData.first;
            _outPointerIndices[entity.get()] = ++indexCounter;

            Vector<ComponentPtr> const& components = entityComponentsData.second;
            for (ComponentPtr const& component : components)
                _outPointerIndices[component.get()] = ++indexCounter;
        }

        if (!_prefabs.empty())
            _outIdentityPrefabsWorld = EcsWorld::Create(
                MAZE_HS("PrefabSerialization"),
                false);

        for (PrefabInstance* prefabInstance : _prefabs)
        {
            _outPointerIndices[prefabInstance->getEntityRaw()] = ++indexCounter;

            auto it = _outIdentityPrefabs.find(prefabInstance->getPrefabName());
            if (it == _outIdentityPrefabs.end())
            {
                EntityPtr identityPrefab = EntitySerializationManager::GetInstancePtr()->loadPrefab(
                    prefabInstance->getPrefabName(),
                    _outIdentityPrefabsWorld.get());

                _outIdentityPrefabs.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(prefabInstance->getPrefabName()),
                    std::forward_as_tuple(identityPrefab));
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::saveEntitiesToDataBlock(
        Map<EntityPtr, Vector<ComponentPtr>> const& _entityComponents,
        Vector<PrefabInstance*> const& _prefabs,
        Map<void*, S32>& _pointerIndices,
        Map<String, EntityPtr>& _identityPrefabs,
        DataBlock& _dataBlock) const
    {
        auto saveComponentFunc =
            [&](ComponentPtr const& _component, DataBlock& _parentBlock)
            {
                DataBlock* componentBlock = _parentBlock.addNewDataBlock(MAZE_HCS("component"));

                auto propertyValueIndexIt = _pointerIndices.find(_component.get());
                if (propertyValueIndexIt != _pointerIndices.end())
                    componentBlock->setS32(MAZE_HCS("_i"), propertyValueIndexIt->second);
                componentBlock->setCString(MAZE_HCS("_t"), static_cast<CString>(_component->getMetaClass()->getName()));

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
                            if (metaPropertyMetaClass && (metaPropertyMetaClass->isInheritedFrom<Component>() || metaPropertyMetaClass->isInheritedFrom<Entity>()))
                            {
                                void* propertyValuePointer = metaProperty->getSharedPtrPointer(metaInstance);
                                if (propertyValuePointer)
                                {
                                    S32 propertyValueIndex = _pointerIndices[propertyValuePointer];
                                    componentBlock->setS32(propertyName, propertyValueIndex);
                                }

                                continue;
                            }
                            else
                            if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                            {
                                Vector<ComponentPtr> comps;
                                metaProperty->getValue<Vector<ComponentPtr>>(metaInstance, comps);

                                if (!comps.empty())
                                {
                                    Vector<S32> compIndices;
                                    compIndices.resize(comps.size());
                                    for (S32 i = 0, in = S32(comps.size()); i < in; ++i)
                                        compIndices[i] = _pointerIndices[comps[i].get()];
                                    AddDataToDataBlock(*componentBlock, propertyName, compIndices);
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
                                    Vector<S32> entIndices;
                                    entIndices.resize(ents.size());
                                    for (S32 i = 0, in = S32(ents.size()); i < in; ++i)
                                        entIndices[i] = _pointerIndices[ents[i].get()];
                                    AddDataToDataBlock(*componentBlock, propertyName, entIndices);
                                }

                                continue;
                            }
                        }

                        DataBlockHelper::SerializeMetaPropertyToDataBlock(metaInstance, metaProperty, *componentBlock);

                    }
                }
            };

        // Entities
        for (auto const& entityComponentsData : _entityComponents)
        {
            EntityPtr const& entity = entityComponentsData.first;
            Vector<ComponentPtr> const& components = entityComponentsData.second;

            DataBlock* entityBlock = _dataBlock.addNewDataBlock(MAZE_HCS("entity"));
            entityBlock->setS32(MAZE_HCS("_i"), _pointerIndices[entity.get()]);

            if (!entity->getActiveSelf())
                entityBlock->setBool(MAZE_HCS("active"), entity->getActiveSelf());

            for (ComponentPtr const& component : components)
            {
                saveComponentFunc(component, *entityBlock);
            }
        }

        // Prefabs
        for (PrefabInstance* prefabInstance : _prefabs)
        {
            EntityPtr const& prefabEntity = prefabInstance->getEntity();
            EntityPtr const& identityPrefabEntity = _identityPrefabs[prefabInstance->getPrefabName()];

            DataBlock* entityBlock = _dataBlock.addNewDataBlock(MAZE_HCS("prefabInstance"));
            entityBlock->setS32(MAZE_HCS("_i"), _pointerIndices[prefabEntity.get()]);
            entityBlock->setString(MAZE_HCS("source"), prefabInstance->getPrefabName());

            if (!prefabEntity->getActiveSelf())
                entityBlock->setBool(MAZE_HCS("active"), prefabEntity->getActiveSelf());

            for (auto const& componentData : prefabEntity->getComponents())
            {
                if (componentData.first == ClassInfo<PrefabInstance>::UID())
                    continue;

                ComponentPtr const& identityComponent = identityPrefabEntity->getComponentById(componentData.first);
                if (identityComponent)
                {
                    MetaClass const* metaClass = componentData.second->getMetaClass();
                    MetaInstance metaInstance = componentData.second->getMetaInstance();
                    MetaInstance identityComponentMetaInstance = identityComponent->getMetaInstance();

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
                                        auto propertyValueIndexIt = _pointerIndices.find(propertyValuePointer);
                                        if (propertyValueIndexIt != _pointerIndices.end())
                                        {
                                            S32 propertyValueIndex = propertyValueIndexIt->second;

                                            DataBlock* modificationBlock = entityBlock->addNewDataBlock(MAZE_HCS("modification"));
                                            modificationBlock->setCString(MAZE_HCS("component"), metaClass->getName());
                                            modificationBlock->setCString(MAZE_HCS("property"), propertyName);
                                            modificationBlock->setS32(MAZE_HCS("value"), propertyValueIndex);
                                        }
                                    }

                                    continue;
                                }
                                else
                                {
                                    DataBlock* modificationBlock = entityBlock->addNewDataBlock(MAZE_HCS("modification"));
                                    modificationBlock->setCString(MAZE_HCS("component"), metaClass->getName());
                                    modificationBlock->setCString(MAZE_HCS("property"), propertyName);
                                    DataBlockHelper::SerializeMetaPropertyToDataBlock(metaInstance, metaProperty, MAZE_HCS("value"), *modificationBlock);
                                }
                            }
                        }
                    }
                }
                else
                {
                    saveComponentFunc(componentData.second, *entityBlock);
                }
            }
        }
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::saveEntitiesToDataBlock(Set<Entity*> const& _entities, DataBlock& _dataBlock) const
    {
        Map<EntityPtr, Vector<ComponentPtr>> entityComponents;
        Vector<PrefabInstance*> prefabs;
        collectEntitiesComponentsMap(_entities, entityComponents, prefabs);


        Map<void*, S32> pointerIndices;
        Map<String, EntityPtr> identityPrefabs;
        EcsWorldPtr identityPrefabsWorld;
        PrepareEntitiesToSerialize(entityComponents, prefabs, pointerIndices, identityPrefabs, identityPrefabsWorld);


        saveEntitiesToDataBlock(entityComponents, prefabs, pointerIndices, identityPrefabs, _dataBlock);


        _dataBlock.setS32(MAZE_HCS("_version"), c_enititySerializationVersion);


        return true;
    }

    //////////////////////////////////////////
    bool EntitySerializationManager::savePrefabToDataBlock(EntityPtr const& _entity, DataBlock& _dataBlock) const
    {
        Map<EntityPtr, Vector<ComponentPtr>> entityComponents;
        Vector<PrefabInstance*> prefabs;
        collectEntityComponentsMap(_entity, entityComponents, prefabs);


        Map<void*, S32> pointerIndices;
        Map<String, EntityPtr> identityPrefabs;
        EcsWorldPtr identityPrefabsWorld;
        PrepareEntitiesToSerialize(entityComponents, prefabs, pointerIndices, identityPrefabs, identityPrefabsWorld);


        saveEntitiesToDataBlock(entityComponents, prefabs, pointerIndices, identityPrefabs, _dataBlock);


        _dataBlock.setS32(MAZE_HCS("_rootIndex"), pointerIndices[_entity.get()]);
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
    bool EntitySerializationManager::loadSceneFromDataBlock(EcsScenePtr const& _scene, DataBlock const& _dataBlock) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadSceneFromDataBlock");

        if (!_scene)
            return false;

        _scene->deserializeSceneCommonInfo(_dataBlock);

        DataBlock const* entitiesBlock = _dataBlock.getDataBlock(MAZE_HCS("entities"));
        if (entitiesBlock)
        {
            Map<S32, EntityPtr> entities;
            Map<S32, ComponentPtr> components;
            loadEntities(*entitiesBlock, _scene->getWorld(), _scene.get(), entities, components);
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

        if (dataBlock.getS32(MAZE_HCS("_version")) > 0)
            return loadPrefab(dataBlock, _world, _scene);
        else
            return loadPrefabOBSOLETE(dataBlock, _world, _scene);
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
    EntityPtr EntitySerializationManager::loadPrefabOBSOLETE(
        DataBlock const& _dataBlock,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadPrefabOBSOLETE");

        static EntityPtr const nullPointer;

        if (_dataBlock.getName() == MAZE_HCS("Prefab"))
            return nullPointer;

        CString rootIndexAttribute = _dataBlock.getCString(MAZE_HCS("_rootIndex"));
        if (rootIndexAttribute == nullptr)
            return nullPointer;

        EcsScene* scene = _scene ? _scene
            : (_world ? nullptr : SceneManager::GetInstancePtr()->getMainScene().get());

        EcsWorld* world = _world ? _world
            : (scene ? scene->getWorld() : nullptr);

        Map<S32, EntityPtr> entities;
        Map<S32, ComponentPtr> components;

        S32 rootIndex = StringHelper::StringToS32(rootIndexAttribute);
        S32 autoEntityIndexCounter = 0;
        S32 autoComponentIndexCounter = 0;

        // Prepare
        {
            for (DataBlock const* subBlock : _dataBlock)
            {
                if (subBlock->isComment())
                    continue;

                if (subBlock->getName() == MAZE_HCS("entity"))
                {
                    S32 entityIndex = StringHelper::StringToS32(subBlock->getCString(MAZE_HCS("_i")));
                    if (entityIndex == 0)
                        entityIndex = --autoEntityIndexCounter;

                    EntityPtr entity = Entity::Create();
                    entity->setAwakeForbidden(true);
                    world->addEntity(entity);

                    entities[entityIndex] = entity;

                    for (DataBlock const* componentBlock : *subBlock)
                    {
                        if (componentBlock->isComment())
                            continue;

                        if (componentBlock->getName() != MAZE_HCS("component"))
                            continue;

                        S32 componentIndex = StringHelper::StringToS32(componentBlock->getCString(MAZE_HCS("_i")));
                        if (componentIndex == 0)
                            componentIndex = --autoComponentIndexCounter;

                        CString componentClassName = componentBlock->getCString(MAZE_HCS("_t"));
                        ComponentId componentId = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentId(componentClassName);
                        ComponentPtr component = entity->getComponentById(componentId);
                        if (!component)
                        {
                            component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentId);

                            if (!component)
                            {
                                MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                continue;
                            }

                            entity->addComponent(component);
                        }

                        components[componentIndex] = component;
                    }
                }
                else
                if (subBlock->getName() == MAZE_HCS("prefabInstance"))
                {
                    S32 entityIndex = StringHelper::StringToS32(subBlock->getCString(MAZE_HCS("_i")));
                    if (entityIndex == 0)
                        entityIndex = --autoEntityIndexCounter;

                    CString prefabName = subBlock->getCString(MAZE_HCS("source"));

                    if (prefabName)
                    {
                        EntityPtr entity = loadPrefab(prefabName, world, scene);
                        if (entity == nullptr)
                        {
                            MAZE_ERROR("Entity is nullptr!");
                            continue;
                        }
                        entity->createComponent<PrefabInstance>(prefabName);

                        entities[entityIndex] = entity;

                        for (DataBlock const* prefabChildBlock : *subBlock)
                        {
                            if (prefabChildBlock->isComment())
                                continue;

                            if (prefabChildBlock->getName() == MAZE_HCS("modification"))
                            {
                                CString componentClassName = prefabChildBlock->getCString(MAZE_HCS("component"));
                                CString componentPropertyName = prefabChildBlock->getCString(MAZE_HCS("property"));
                                CString componentPropertyValue = prefabChildBlock->getCString(MAZE_HCS("value"));

                                if (componentClassName && componentPropertyName && componentPropertyValue)
                                {
                                    ComponentId componentId = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentId(componentClassName);
                                    ComponentPtr const& component = entity->getComponentById(componentId);
                                    MetaProperty* metaProperty = component->getMetaClass()->getProperty(componentPropertyName);
                                    if (metaProperty)
                                    {
                                        ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                                        if (metaPropertyUID != 0)
                                        {
                                            if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                                            {
                                                S32 valueIndex = StringHelper::StringToS32(componentPropertyValue);
                                                metaProperty->setValue(component->getMetaInstance(), &components[valueIndex]);

                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                            {
                                                S32 valueIndex = StringHelper::StringToS32(componentPropertyValue);
                                                metaProperty->setValue(component->getMetaInstance(), &entities[valueIndex]);

                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                                            {
                                                MAZE_NOT_IMPLEMENTED;
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                                            {
                                                MAZE_NOT_IMPLEMENTED;
                                                continue;
                                            }
                                        }

                                        metaProperty->setString(component->getMetaInstance(), componentPropertyValue);
                                    }
                                }
                            }
                            else
                            if (prefabChildBlock->getName() == MAZE_HCS("component"))
                            {
                                S32 componentIndex = StringHelper::StringToS32(prefabChildBlock->getCString(MAZE_HCS("_i")));
                                if (componentIndex == 0)
                                    componentIndex = --autoComponentIndexCounter;

                                CString componentClassName = prefabChildBlock->getCString(MAZE_HCS("_t"));
                                ComponentId componentId = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentId(componentClassName);
                                ComponentPtr component = entity->getComponentById(componentId);
                                if (!component)
                                {
                                    component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentId);

                                    if (!component)
                                    {
                                        MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                        continue;
                                    }

                                    entity->addComponent(component);
                                }

                                components[componentIndex] = component;
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

        autoComponentIndexCounter = 0;

        // Load
        {
            for (DataBlock const* subBlock : _dataBlock)
            {
                if (subBlock->isComment())
                    continue;

                if (subBlock->getName() == MAZE_HCS("entity") || subBlock->getName() == MAZE_HCS("prefabInstance"))
                {
                    S32 entityIndex = StringHelper::StringToS32(subBlock->getCString(MAZE_HCS("_i")));
                    EntityPtr const& entity = entities[entityIndex];
                    if (entity == nullptr)
                    {
                        MAZE_ERROR("Entity is nullptr!");
                        continue;
                    }

                    CString entityActiveAttribute = subBlock->getCString(MAZE_HCS("active"));
                    if (entityActiveAttribute)
                        entity->setActiveSelf(StringHelper::StringToBool(entityActiveAttribute));

                    for (DataBlock const* componentBlock : *subBlock)
                    {
                        if (componentBlock->isComment())
                            continue;

                        if (componentBlock->getName() == MAZE_HCS("component"))
                        {
                            S32 componentIndex = StringHelper::StringToS32(componentBlock->getCString(MAZE_HCS("_i")));
                            if (componentIndex == 0)
                                componentIndex = --autoComponentIndexCounter;

                            ComponentPtr const& component = components[componentIndex];
                            if (!component)
                            {
                                continue;
                            }

                            MetaClass const* componentMetaClass = component->getMetaClass();
                            MetaInstance componentMetaInstance = component->getMetaInstance();

                            for (Maze::MetaClass* metaClass : componentMetaClass->getAllSuperMetaClasses())
                            {
                                for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
                                {
                                    Maze::MetaProperty* metaProperty = metaClass->getProperty(i);

                                    Maze::CString propertyName = metaProperty->getName();

                                    CString attributeValue = componentBlock->getCString(HashedCString(propertyName));
                                    if (attributeValue)
                                    {
                                        MAZE_ERROR_CONTINUE_IF(StringHelper::IsStartsWith(attributeValue, "ptr:"), "Pointer value in '%s' property!", propertyName);

                                        ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                                        if (metaPropertyUID != 0)
                                        {
                                            if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                                            {
                                                S32 valueIndex = StringHelper::StringToS32(attributeValue);
                                                metaProperty->setValue(componentMetaInstance, &components[valueIndex]);
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                            {
                                                S32 valueIndex = StringHelper::StringToS32(attributeValue);
                                                metaProperty->setValue(componentMetaInstance, &entities[valueIndex]);
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                                            {
                                                Vector<String> componentsStr;
                                                ValueFromString(componentsStr, attributeValue, strlen(attributeValue));

                                                Vector<ComponentPtr> componentsValue;
                                                componentsValue.resize(componentsStr.size());
                                                for (Size i = 0, in = componentsStr.size(); i < in; ++i)
                                                {
                                                    String const& str = componentsStr[i];
                                                    S32 valueIndex = StringHelper::StringToS32(str);
                                                    componentsValue[i] = components[valueIndex];
                                                }
                                                metaProperty->setValue(componentMetaInstance, &componentsValue);
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                                            {
                                                Vector<String> entitiesStr;
                                                ValueFromString(entitiesStr, attributeValue, strlen(attributeValue));

                                                Vector<EntityPtr> entitiesValue;
                                                entitiesValue.resize(entitiesStr.size());
                                                for (Size i = 0, in = entitiesStr.size(); i < in; ++i)
                                                {
                                                    String const& str = entitiesStr[i];
                                                    S32 valueIndex = StringHelper::StringToS32(str);
                                                    entitiesValue[i] = entities[valueIndex];
                                                }
                                                metaProperty->setValue(componentMetaInstance, &entitiesValue);
                                                continue;
                                            }
                                        }

                                        metaProperty->setString(componentMetaInstance, attributeValue);
                                    }


                                }
                            }
                        }
                    }
                }
            }
        }

        // Add to ECS scene
        for (auto entityData : entities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setEcsScene(scene);
        }

        // Awake
        for (auto entityData : entities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setAwakeForbidden(false);
            entityData.second->tryAwake();
        }

        return entities[rootIndex];
    }

    //////////////////////////////////////////
    void EntitySerializationManager::loadEntities(
        DataBlock const& _dataBlock,
        EcsWorld* _world,
        EcsScene* _scene,
        Map<S32, EntityPtr>& _outEntities,
        Map<S32, ComponentPtr>& _outComponents) const
    {
        EcsScene* scene = _scene ? _scene
            : (_world ? nullptr : SceneManager::GetInstancePtr()->getMainScene().get());

        EcsWorld* world = _world ? _world
            : (scene ? scene->getWorld() : nullptr);


        S32 autoEntityIndexCounter = 0;
        S32 autoComponentIndexCounter = 0;

        // Prepare
        {
            for (DataBlock const* subBlock : _dataBlock)
            {
                if (subBlock->isComment())
                    continue;

                if (subBlock->getName() == MAZE_HCS("entity"))
                {
                    S32 entityIndex = subBlock->getS32(MAZE_HCS("_i"));
                    if (entityIndex == 0)
                        entityIndex = --autoEntityIndexCounter;

                    EntityPtr entity = Entity::Create();
                    entity->setAwakeForbidden(true);
                    world->addEntity(entity);

                    _outEntities[entityIndex] = entity;

                    for (DataBlock const* componentBlock : *subBlock)
                    {
                        if (componentBlock->isComment())
                            continue;

                        if (componentBlock->getName() != MAZE_HCS("component"))
                            continue;

                        S32 componentIndex = componentBlock->getS32(MAZE_HCS("_i"));
                        if (componentIndex == 0)
                            componentIndex = --autoComponentIndexCounter;

                        CString componentClassName = componentBlock->getCString(MAZE_HCS("_t"));
                        ComponentId componentId = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentId(componentClassName);
                        ComponentPtr component = entity->getComponentById(componentId);
                        if (!component)
                        {
                            component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentId);

                            if (!component)
                            {
                                MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                continue;
                            }
                            entity->addComponent(component);
                        }

                        _outComponents[componentIndex] = component;
                    }
                }
                else
                if (subBlock->getName() == MAZE_HCS("prefabInstance"))
                {
                    S32 entityIndex = subBlock->getS32(MAZE_HCS("_i"));
                    if (entityIndex == 0)
                        entityIndex = --autoEntityIndexCounter;

                    CString prefabName = subBlock->getCString(MAZE_HCS("source"));

                    if (prefabName)
                    {
                        EntityPtr entity = loadPrefab(prefabName, world, scene);
                        if (entity == nullptr)
                        {
                            MAZE_ERROR("Entity is nullptr!");
                            continue;
                        }
                        entity->createComponent<PrefabInstance>(prefabName);

                        _outEntities[entityIndex] = entity;

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
                                S32 componentIndex = prefabChildBlock->getS32(MAZE_HCS("_i"));
                                if (componentIndex == 0)
                                    componentIndex = --autoComponentIndexCounter;

                                CString componentClassName = prefabChildBlock->getCString(MAZE_HCS("_t"));
                                ComponentId componentId = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentId(componentClassName);
                                ComponentPtr component = entity->getComponentById(componentId);
                                if (!component)
                                {
                                    component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentId);

                                    if (!component)
                                    {
                                        MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                        continue;
                                    }

                                    entity->addComponent(component);
                                }

                                _outComponents[componentIndex] = component;
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

        autoComponentIndexCounter = 0;

        // Load
        {
            for (DataBlock const* subBlock : _dataBlock)
            {
                if (subBlock->isComment())
                    continue;

                if (subBlock->getName() == MAZE_HCS("entity") || subBlock->getName() == MAZE_HCS("prefabInstance"))
                {
                    S32 entityIndex = subBlock->getS32(MAZE_HCS("_i"));
                    EntityPtr const& entity = _outEntities[entityIndex];
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
                                ComponentId componentId = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentId(componentClassName);
                                ComponentPtr const& component = entity->getComponentById(componentId);
                                MetaProperty* metaProperty = component->getMetaClass()->getProperty(componentPropertyName);
                                if (metaProperty)
                                {
                                    ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                                    if (metaPropertyUID != 0)
                                    {
                                        if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                                        {
                                            S32 valueIndex = componentBlock->getS32(MAZE_HCS("value"));
                                            metaProperty->setValue(component->getMetaInstance(), &_outComponents[valueIndex]);

                                            continue;
                                        }
                                        else
                                        if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                        {
                                            S32 valueIndex = componentBlock->getS32(MAZE_HCS("value"));
                                            metaProperty->setValue(component->getMetaInstance(), &_outEntities[valueIndex]);

                                            continue;
                                        }
                                        else
                                        if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                                        {
                                            DataBlock const* propertyBlock = componentBlock->getDataBlock(MAZE_HCS("value"));
                                            if (propertyBlock)
                                            {
                                                Vector<S32> componentsIndices;
                                                ValueFromDataBlock(componentsIndices, *propertyBlock);

                                                Vector<ComponentPtr> componentsValue;
                                                componentsValue.resize(componentsIndices.size());
                                                for (Size i = 0, in = componentsIndices.size(); i < in; ++i)
                                                    componentsValue[i] = _outComponents[componentsIndices[i]];
                                                metaProperty->setValue(component->getMetaInstance(), &componentsValue);
                                            }
                                            else
                                            {
                                                MAZE_ERROR("Invalid property 'value'!");
                                            }
                                            continue;
                                        }
                                        else
                                        if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                                        {
                                            DataBlock const* propertyBlock = componentBlock->getDataBlock(MAZE_HCS("value"));
                                            if (propertyBlock)
                                            {
                                                Vector<S32> entitiesIndices;
                                                ValueFromDataBlock(entitiesIndices, *propertyBlock);

                                                Vector<EntityPtr> entitiesValue;
                                                entitiesValue.resize(entitiesIndices.size());
                                                for (Size i = 0, in = entitiesIndices.size(); i < in; ++i)
                                                    entitiesValue[i] = _outEntities[entitiesIndices[i]];
                                                metaProperty->setValue(component->getMetaInstance(), &entitiesValue);
                                            }
                                            else
                                            {
                                                MAZE_ERROR("Invalid property 'value'");
                                            }
                                            continue;
                                        }
                                    }

                                    DataBlockHelper::DeserializeMetaPropertyFromDataBlock(component->getMetaInstance(), metaProperty, MAZE_HCS("value"), *componentBlock);
                                }
                            }
                        }
                        if (componentBlock->getName() == MAZE_HCS("component"))
                        {
                            S32 componentIndex = componentBlock->getS32(MAZE_HCS("_i"));
                            if (componentIndex == 0)
                                componentIndex = --autoComponentIndexCounter;

                            ComponentPtr const& component = _outComponents[componentIndex];
                            if (!component)
                            {
                                continue;
                            }

                            MetaClass const* componentMetaClass = component->getMetaClass();
                            MetaInstance componentMetaInstance = component->getMetaInstance();

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
                                                S32 valueIndex = componentBlock->getS32(propertyName);
                                                metaProperty->setValue(componentMetaInstance, &_outComponents[valueIndex]);
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                            {
                                                S32 valueIndex = componentBlock->getS32(propertyName);
                                                metaProperty->setValue(componentMetaInstance, &_outEntities[valueIndex]);
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<Vector<ComponentPtr>>::UID())
                                            {
                                                DataBlock const* propertyBlock = componentBlock->getDataBlock(propertyName);
                                                if (propertyBlock)
                                                {
                                                    Vector<S32> componentsIndices;
                                                    ValueFromDataBlock(componentsIndices, *propertyBlock);

                                                    Vector<ComponentPtr> componentsValue;
                                                    componentsValue.resize(componentsIndices.size());
                                                    for (Size i = 0, in = componentsIndices.size(); i < in; ++i)
                                                        componentsValue[i] = _outComponents[componentsIndices[i]];
                                                    metaProperty->setValue(componentMetaInstance, &componentsValue);
                                                }
                                                else
                                                {
                                                    MAZE_ERROR("Invalid property '%s'!", propertyName.str);
                                                }
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<Vector<EntityPtr>>::UID())
                                            {
                                                DataBlock const* propertyBlock = componentBlock->getDataBlock(propertyName);
                                                if (propertyBlock)
                                                {
                                                    Vector<S32> entitiesIndices;
                                                    ValueFromDataBlock(entitiesIndices, *propertyBlock);

                                                    Vector<EntityPtr> entitiesValue;
                                                    entitiesValue.resize(entitiesIndices.size());
                                                    for (Size i = 0, in = entitiesIndices.size(); i < in; ++i)
                                                        entitiesValue[i] = _outEntities[entitiesIndices[i]];
                                                    metaProperty->setValue(componentMetaInstance, &entitiesValue);
                                                }
                                                else
                                                {
                                                    MAZE_ERROR("Invalid property '%s'", propertyName.str);
                                                }
                                                continue;
                                            }
                                        }

                                        DataBlockHelper::DeserializeMetaPropertyFromDataBlock(componentMetaInstance, metaProperty, *componentBlock);
                                    }


                                }
                            }
                        }
                    }
                }
            }
        }

        // Add to ECS scene
        for (auto entityData : _outEntities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setEcsScene(scene);
        }

        // Awake
        for (auto entityData : _outEntities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setAwakeForbidden(false);
            entityData.second->tryAwake();
        }
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        DataBlock const& _dataBlock,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadPrefab");

        static EntityPtr const nullPointer;

        
        S32 rootIndex = _dataBlock.getS32(MAZE_HCS("_rootIndex"), -1);
        if (rootIndex == -1)
            return nullptr;


        Map<S32, EntityPtr> entities;
        Map<S32, ComponentPtr> components;
        loadEntities(_dataBlock, _world, _scene, entities, components);

        return entities[rootIndex];
    }

    //////////////////////////////////////////
    void EntitySerializationManager::collectAllChildrenEntity(
        Entity* _entity,
        Vector<EntityPtr>& _entities,
        Vector<PrefabInstance*>& _prefabInstances) const
    {
        if (PrefabInstance* prefabInstance = _entity->getComponentRaw<PrefabInstance>())
        {
            _prefabInstances.emplace_back(prefabInstance);
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
        Map<EntityPtr, Vector<ComponentPtr>>& _entityComponents,
        Vector<PrefabInstance*>& _prefabs) const
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
                components.push_back(componentData.second);

            _entityComponents[entity] = components;
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::collectEntityComponentsMap(
        EntityPtr const& _entity,
        Map<EntityPtr, Vector<ComponentPtr>>& _entityComponents,
        Vector<PrefabInstance*>& _prefabs) const
    {
        Vector<EntityPtr> entities;

        collectAllChildrenEntity(_entity.get(), entities, _prefabs);

        for (EntityPtr const& entity : entities)
        {
            Vector<ComponentPtr> components;
            for (auto const& componentData : entity->getComponents())
                components.push_back(componentData.second);

            _entityComponents[entity] = components;
        }
    }

} // namespace Maze
//////////////////////////////////////////
