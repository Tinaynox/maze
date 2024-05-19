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
    static const S32 c_prefabSerializationVersion = 1;


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

        Map<EntityPtr, Vector<ComponentPtr>> entityComponents;
        Vector<PrefabInstance*> prefabs;
        collectEntityComponentsMap(_entity, entityComponents, prefabs);
        
        S32 indexCounter = 0;
        Map<void*, S32> pointerIndices;

        for (auto const& entityComponentsData : entityComponents)
        {
            EntityPtr const& entity = entityComponentsData.first;
            pointerIndices[entity.get()] = ++indexCounter;

            Vector<ComponentPtr> const& components = entityComponentsData.second;
            for (ComponentPtr const& component : components)
                pointerIndices[component.get()] = ++indexCounter;
        }

        Map<String, EntityPtr> identityPrefabs;
        EcsWorldPtr identityPrefabsWorld;
        if (!prefabs.empty())
            identityPrefabsWorld = EcsWorld::Create(
                MAZE_HS("PrefabSerialization"),
                false);

        for (PrefabInstance* prefabInstance : prefabs)
        {
            pointerIndices[prefabInstance->getEntityRaw()] = ++indexCounter;

            auto it = identityPrefabs.find(prefabInstance->getPrefabName());
            if (it == identityPrefabs.end())
            {
                EntityPtr identityPrefab = loadPrefab(prefabInstance->getPrefabName(), identityPrefabsWorld.get());
                identityPrefabs.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(prefabInstance->getPrefabName()),
                    std::forward_as_tuple(identityPrefab));
            }
        }

        dataBlock.setS32(MAZE_HCS("_rootIndex"), pointerIndices[_entity.get()]);
        dataBlock.setS32(MAZE_HCS("_version"), c_prefabSerializationVersion);

        auto saveComponentFunc =
            [&](ComponentPtr const& _component, DataBlock& _parentBlock)
            {
                DataBlock* componentBlock = _parentBlock.addNewDataBlock(MAZE_HCS("component"));

                auto propertyValueIndexIt = pointerIndices.find(_component.get());
                if (propertyValueIndexIt != pointerIndices.end())
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
                                    S32 propertyValueIndex = pointerIndices[propertyValuePointer];
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
                                        compIndices[i] = pointerIndices[comps[i].get()];
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
                                        entIndices[i] = pointerIndices[ents[i].get()];
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
        for (auto const& entityComponentsData : entityComponents)
        {
            EntityPtr const& entity = entityComponentsData.first;
            Vector<ComponentPtr> const& components = entityComponentsData.second;

            DataBlock* entityBlock = dataBlock.addNewDataBlock(MAZE_HCS("entity"));
            entityBlock->setS32(MAZE_HCS("_i"), pointerIndices[entity.get()]);

            if (!entity->getActiveSelf())
                entityBlock->setBool(MAZE_HCS("active"), entity->getActiveSelf());

            for (ComponentPtr const& component : components)
            {
                saveComponentFunc(component, *entityBlock);
            }
        }

        // Prefabs
        for (PrefabInstance* prefabInstance : prefabs)
        {
            EntityPtr const& prefabEntity = prefabInstance->getEntity();
            EntityPtr const& identityPrefabEntity = identityPrefabs[prefabInstance->getPrefabName()];

            DataBlock* entityBlock = dataBlock.addNewDataBlock(MAZE_HCS("prefabInstance"));
            entityBlock->setS32(MAZE_HCS("_i"), pointerIndices[prefabEntity.get()]);
            entityBlock->setString(MAZE_HCS("source"), prefabInstance->getPrefabName());

            if (!prefabEntity->getActiveSelf())
                entityBlock->setBool(MAZE_HCS("active"), prefabEntity->getActiveSelf());

            for (auto const& componentData : prefabEntity->getComponents())
            {
                if (componentData.first == ClassInfo<PrefabInstance>::UID())
                    continue;

                ComponentPtr const& identityComponent = identityPrefabEntity->getComponentByUID(componentData.first);
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
                                        auto propertyValueIndexIt = pointerIndices.find(propertyValuePointer);
                                        if (propertyValueIndexIt != pointerIndices.end())
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

        dataBlock.saveTextFile(_fileFullPath);

        return true;
    }


    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        AssetFilePtr const& _assetFile,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        DataBlock dataBlock;
        ByteBufferPtr byteBuffer = _assetFile->readAsByteBuffer();
        dataBlock.loadFromByteBuffer(*byteBuffer.get());

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
                        ClassUID componentUID = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentUID(componentClassName);
                        ComponentPtr component = entity->getComponentByUID(componentUID);
                        if (!component)
                        {
                            component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentUID);

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
                                    ClassUID componentUID = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentUID(componentClassName);
                                    ComponentPtr const& component = entity->getComponentByUID(componentUID);
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
                                ClassUID componentUID = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentUID(componentClassName);
                                ComponentPtr component = entity->getComponentByUID(componentUID);
                                if (!component)
                                {
                                    component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentUID);

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
    EntityPtr EntitySerializationManager::loadPrefab(
        DataBlock const& _dataBlock,
        EcsWorld* _world,
        EcsScene* _scene) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadPrefab");

        static EntityPtr const nullPointer;

        EcsScene* scene = _scene ? _scene
            : (_world ? nullptr : SceneManager::GetInstancePtr()->getMainScene().get());

        EcsWorld* world = _world ? _world
            : (scene ? scene->getWorld() : nullptr);

        Map<S32, EntityPtr> entities;
        Map<S32, ComponentPtr> components;

        S32 rootIndex = _dataBlock.getS32(MAZE_HCS("_rootIndex"), -1);
        if (rootIndex == -1)
            return nullptr;

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

                    entities[entityIndex] = entity;

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
                        ClassUID componentUID = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentUID(componentClassName);
                        ComponentPtr component = entity->getComponentByUID(componentUID);
                        if (!component)
                        {
                            component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentUID);

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

                        entities[entityIndex] = entity;

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
                                ClassUID componentUID = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentUID(componentClassName);
                                ComponentPtr component = entity->getComponentByUID(componentUID);
                                if (!component)
                                {
                                    component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentUID);

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
                    S32 entityIndex = subBlock->getS32(MAZE_HCS("_i"));
                    EntityPtr const& entity = entities[entityIndex];
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
                                ClassUID componentUID = EntityManager::GetInstancePtr()->getComponentFactory()->getComponentUID(componentClassName);
                                ComponentPtr const& component = entity->getComponentByUID(componentUID);
                                MetaProperty* metaProperty = component->getMetaClass()->getProperty(componentPropertyName);
                                if (metaProperty)
                                {
                                    ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                                    if (metaPropertyUID != 0)
                                    {
                                        if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                                        {
                                            S32 valueIndex = componentBlock->getS32(MAZE_HCS("value"));
                                            metaProperty->setValue(component->getMetaInstance(), &components[valueIndex]);

                                            continue;
                                        }
                                        else
                                        if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                        {
                                            S32 valueIndex = componentBlock->getS32(MAZE_HCS("value"));
                                            metaProperty->setValue(component->getMetaInstance(), &entities[valueIndex]);

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
                                                    componentsValue[i] = components[componentsIndices[i]];
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
                                                    entitiesValue[i] = entities[entitiesIndices[i]];
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

                                    HashedCString propertyName = metaProperty->getName();

                                    {
                                        ClassUID metaPropertyUID = metaProperty->getValueClassUID();
                                        if (metaPropertyUID != 0)
                                        {
                                            if (metaPropertyUID == ClassInfo<ComponentPtr>::UID())
                                            {
                                                S32 valueIndex = componentBlock->getS32(propertyName);
                                                metaProperty->setValue(componentMetaInstance, &components[valueIndex]);
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                            {
                                                S32 valueIndex = componentBlock->getS32(propertyName);
                                                metaProperty->setValue(componentMetaInstance, &entities[valueIndex]);
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
                                                        componentsValue[i] = components[componentsIndices[i]];
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
                                                        entitiesValue[i] = entities[entitiesIndices[i]];
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
    void EntitySerializationManager::collectAllChildrenEntity(
        EntityPtr const& _entity,
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
            _entities.emplace_back(_entity);
        }

        {
            Transform2D* transform = _entity->getComponentRaw<Transform2D>();
            if (transform)
            {
                for (Transform2D* childTransform : transform->getChildren())
                    collectAllChildrenEntity(childTransform->getEntity(), _entities, _prefabInstances);
            }
        }

        {
            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (transform)
            {
                for (Transform3D* childTransform : transform->getChildren())
                    collectAllChildrenEntity(childTransform->getEntity(), _entities, _prefabInstances);
            }
        }
    }

    //////////////////////////////////////////
    void EntitySerializationManager::collectEntityComponentsMap(
        EntityPtr const& _entity,
        Map<EntityPtr, Vector<ComponentPtr>>& _entities,
        Vector<PrefabInstance*>& _prefabs) const
    {
        Vector<EntityPtr> entities;

        collectAllChildrenEntity(_entity, entities, _prefabs);

        for (EntityPtr const& entity : entities)
        {
            Vector<ComponentPtr> components;
            for (auto const& componentData : entity->getComponents())
                components.push_back(componentData.second);

            _entities[entity] = components;
        }
    }

} // namespace Maze
//////////////////////////////////////////
