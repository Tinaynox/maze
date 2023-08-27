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
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"



//////////////////////////////////////////
namespace Maze
{
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
    bool EntitySerializationManager::savePrefabToXMLFile(EntityPtr const& _entity, Path const& _fileFullPath) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::savePrefabToXMLFile");

        if (!_entity)
            return false;

        if (_fileFullPath.empty())
            return false;

        Path directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
        FileHelper::CreateDirectoryRecursive(directoryFullPath);

        tinyxml2::XMLDocument doc;

        tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
        doc.LinkEndChild(decl);

        tinyxml2::XMLNode* root = savePrefabToXMLElement(_entity, doc);
        if (!root)
            return false;

        doc.InsertEndChild(root);

        tinyxml2::XMLError loadError = XMLHelper::SaveXMLFile(_fileFullPath, doc);
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            MAZE_ERROR("Saving file '%s' error - %d!", _fileFullPath.toUTF8().c_str(), loadError);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    tinyxml2::XMLElement* EntitySerializationManager::savePrefabToXMLElement(EntityPtr const& _entity, tinyxml2::XMLDocument& _doc) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::savePrefabToXMLElement");

        Map<EntityPtr, Vector<ComponentPtr>> entityComponents = collectEntityComponentsMap(_entity);

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

        tinyxml2::XMLElement* prefabElement = _doc.NewElement("Prefab");
        prefabElement->SetAttribute("_rootIndex", pointerIndices[_entity.get()]);

        for (auto const& entityComponentsData : entityComponents)
        {
            EntityPtr const& entity = entityComponentsData.first;
            Vector<ComponentPtr> const& components = entityComponentsData.second;

            tinyxml2::XMLElement* entityElement = _doc.NewElement("Entity");
            entityElement->SetAttribute("_i", pointerIndices[entity.get()]);

            if (!entity->getActiveSelf())
                entityElement->SetAttribute("active", entity->getActiveSelf());

            for (ComponentPtr const& component : components)
            {
                tinyxml2::XMLElement* componentElement = _doc.NewElement("Component");
                componentElement->SetAttribute("_i", pointerIndices[component.get()]);
                componentElement->SetAttribute("_t", component->getMetaClass()->getName());

                MetaClass const* metaClass = component->getMetaClass();
                MetaInstance metaInstance = component->getMetaInstance();

                for (MetaClass* metaClass : metaClass->getAllSuperMetaClasses())
                {
                    for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
                    {
                        MetaProperty* metaProperty = metaClass->getProperty(i);

                        CString propertyName = metaProperty->getName();

                        MetaClass const* metaPropertyMetaClass = metaProperty->getMetaClass();
                        if (metaPropertyMetaClass)
                        {
                            if (metaPropertyMetaClass->isInheritedFrom<Component>() || metaPropertyMetaClass->isInheritedFrom<Entity>())
                            {
                                void* propertyValuePointer = metaProperty->getSharedPtrPointer(metaInstance);
                                if (propertyValuePointer)
                                {
                                    S32 propertyValueIndex = pointerIndices[propertyValuePointer];
                                    componentElement->SetAttribute(propertyName, StringHelper::ToString(propertyValueIndex).c_str());
                                }

                                continue;
                            }
                        }
                        
                        String properyStringValue = metaProperty->toString(metaInstance);

                        MAZE_ERROR_IF(StringHelper::IsStartsWith(properyStringValue.c_str(), "ptr:"), "Trying to save property '%s' as pointer!", propertyName);

                        componentElement->SetAttribute(propertyName, properyStringValue.c_str());
                        
                    }
                }

                entityElement->InsertEndChild(componentElement);
            }


            prefabElement->InsertEndChild(entityElement);
        }


        return prefabElement;
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefabFromXMLFile(
        Path const& _fileFullPath,
        ECSWorld* _world,
        ECSScene* _scene) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadPrefabFromXMLFile");

        static EntityPtr nullPointer;

        if (_fileFullPath.empty())
            return nullPointer;

        Debug::Log("Loading file %s'...", _fileFullPath.toUTF8().c_str());

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError loadError = XMLHelper::LoadXMLFile(_fileFullPath, doc);
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            if (tinyxml2::XML_ERROR_FILE_NOT_FOUND == loadError)
                return nullPointer;

            MAZE_ERROR("File '%s' loading error - XMLError: %d!", _fileFullPath.toUTF8().c_str(), (S32)loadError);
            return nullPointer;
        }

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node!", _fileFullPath.toUTF8().c_str());
            return nullPointer;
        }

        return loadPrefabFromXMLElement(rootNode->NextSibling()->ToElement(), _world, _scene);
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        tinyxml2::XMLDocument& _doc,
        ECSWorld* _world,
        ECSScene* _scene) const
    {
        tinyxml2::XMLNode* rootNode = _doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File loading error - empty root node!");
            return nullptr;
        }

        return loadPrefabFromXMLElement(rootNode->NextSibling()->ToElement(), _world, _scene);
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        AssetFilePtr const& _assetFile,
        ECSWorld* _world,
        ECSScene* _scene) const
    {
        tinyxml2::XMLDocument doc;
        if (!_assetFile->readToXMLDocument(doc))
            return nullptr;

        return loadPrefab(doc, _world, _scene);
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefab(
        Path const& _assetFileName,
        ECSWorld* _world,
        ECSScene* _scene) const
    {
        tinyxml2::XMLDocument doc;
        if (!AssetManager::GetInstancePtr()->openXMLDocumentAssetFile(doc, _assetFileName, true))
            return nullptr;

        return loadPrefab(doc, _world, _scene);
    }

    //////////////////////////////////////////
    EntityPtr EntitySerializationManager::loadPrefabFromXMLElement(
        tinyxml2::XMLElement* _element,
        ECSWorld* _world,
        ECSScene* _scene) const
    {
        MAZE_PROFILE_EVENT("EntitySerializationManager::loadPrefabFromXMLElement");

        static EntityPtr nullPointer;

        if (!_element)
            return nullPointer;

        if (strcmp(_element->Name(), "Prefab") != 0)
            return nullPointer;

        CString rootIndexAttribute = _element->Attribute("_rootIndex");
        if (rootIndexAttribute == nullptr)
            return nullPointer;

        ECSScene* scene = _scene    ? _scene 
                                    : (_world ? nullptr : SceneManager::GetInstancePtr()->getMainScene().get());

        ECSWorld* world = _world    ? _world 
                                    : (scene ? scene->getWorld() : nullptr);

        Map<S32, EntityPtr> entities;
        Map<S32, ComponentPtr> components;

        S32 rootIndex = StringHelper::StringToS32(rootIndexAttribute);
        S32 autoEntityIndexCounter = 0;
        S32 autoComponentIndexCounter = 0;

        // Prepare
        {
            tinyxml2::XMLNode* entityNode = _element->FirstChild();
            while (entityNode)
            {
                tinyxml2::XMLElement* entityElement = entityNode->ToElement();

                if (!entityElement)
                {
                    entityNode = entityNode->NextSibling();
                    continue;
                }

                if (strcmp(entityElement->Name(), "Entity") == 0)
                {
                    S32 entityIndex = entityElement->IntAttribute("_i");
                    if (entityIndex == 0)
                        entityIndex = --autoEntityIndexCounter;

                    EntityPtr entity = Entity::Create();
                    entity->setAwakeForbidden(true);
                    world->addEntity(entity);

                    entities[entityIndex] = entity;

                    tinyxml2::XMLNode* componentNode = entityNode->FirstChild();
                    while (componentNode)
                    {
                        tinyxml2::XMLElement* componentElement = componentNode->ToElement();

                        if (!componentElement)
                        {
                            componentNode = componentNode->NextSibling();
                            continue;
                        }

                        if (strcmp(componentElement->Name(), "Component") != 0)
                        {
                            componentNode = componentNode->NextSibling();
                            continue;
                        }

                        S32 componentIndex = componentElement->IntAttribute("_i");
                        if (componentIndex == 0)
                            componentIndex = --autoComponentIndexCounter;

                        CString componentClassName = componentElement->Attribute("_t");
                        ComponentPtr component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentClassName);

                        if (!component)
                        {
                            MAZE_ERROR("Component %s cannot be created!", componentClassName);
                            componentNode = componentNode->NextSibling();
                            continue;
                        }

                        components[componentIndex] = component;

                        entity->addComponent(component);

                        componentNode = componentNode->NextSibling();
                    }
                }
                else if (strcmp(entityElement->Name(), "PrefabInstance") == 0)
                {
                    S32 entityIndex = entityElement->IntAttribute("_i");
                    if (entityIndex == 0)
                        entityIndex = --autoEntityIndexCounter;

                    CString prefabName = entityElement->Attribute("source");

                    if (prefabName)
                    {
                        EntityPtr entity = loadPrefab(prefabName, world, scene);
                        if (entity == nullptr)
                        {
                            MAZE_ERROR("Entity is nullptr!");
                            entityNode = entityNode->NextSibling();
                            continue;
                        }
                        entities[entityIndex] = entity;

                        tinyxml2::XMLNode* prefabChildNode = entityNode->FirstChild();
                        while (prefabChildNode)
                        {
                            tinyxml2::XMLElement* prefabChildElement = prefabChildNode->ToElement();

                            if (!prefabChildElement)
                            {
                                prefabChildNode = prefabChildNode->NextSibling();
                                continue;
                            }

                            if (strcmp(prefabChildElement->Name(), "Modification") == 0)
                            {
                                CString componentClassName = prefabChildElement->Attribute("component");
                                CString componentPropertyName = prefabChildElement->Attribute("property");
                                CString componentPropertyValue = prefabChildElement->Attribute("value");

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

                                                prefabChildNode = prefabChildNode->NextSibling();
                                                continue;
                                            }
                                            else
                                            if (metaPropertyUID == ClassInfo<EntityPtr>::UID())
                                            {
                                                S32 valueIndex = StringHelper::StringToS32(componentPropertyValue);
                                                metaProperty->setValue(component->getMetaInstance(), &entities[valueIndex]);

                                                prefabChildNode = prefabChildNode->NextSibling();
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
                            else if (strcmp(prefabChildElement->Name(), "Component") == 0)
                            {
                                S32 componentIndex = prefabChildElement->IntAttribute("_i");
                                if (componentIndex == 0)
                                    componentIndex = --autoComponentIndexCounter;

                                CString componentClassName = prefabChildElement->Attribute("_t");
                                ComponentPtr component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponent(componentClassName);

                                if (component)
                                {
                                    components[componentIndex] = component;
                                    entity->addComponent(component);
                                }
                                else
                                {
                                    MAZE_ERROR("Component %s cannot be created!", componentClassName);
                                }
                            }

                            prefabChildNode = prefabChildNode->NextSibling();
                        }
                    }
                    else
                    {
                        MAZE_ERROR("PrefabInstance without source!");
                    }
                }

                entityNode = entityNode->NextSibling();
            }
        }

        autoComponentIndexCounter = 0;

        // Load
        {
            tinyxml2::XMLNode* entityNode = _element->FirstChild();
            while (entityNode)
            {
                tinyxml2::XMLElement* entityElement = entityNode->ToElement();

                if (!entityElement)
                {
                    entityNode = entityNode->NextSibling();
                    continue;
                }

                if (strcmp(entityElement->Name(), "Entity") != 0 ||
                    strcmp(entityElement->Name(), "PrefabInstance") != 0)
                {
                    S32 entityIndex = entityElement->IntAttribute("_i");
                    EntityPtr const& entity = entities[entityIndex];
                    if (entity == nullptr)
                    {
                        MAZE_ERROR("Entity is nullptr!");
                        entityNode = entityNode->NextSibling();
                        continue;
                    }

                    CString entityActiveAttribute = entityElement->Attribute("active");
                    if (entityActiveAttribute)
                        entity->setActiveSelf(StringHelper::StringToBool(entityActiveAttribute));

                    tinyxml2::XMLNode* componentNode = entityNode->FirstChild();
                    while (componentNode)
                    {
                        tinyxml2::XMLElement* componentElement = componentNode->ToElement();

                        if (!componentElement)
                        {
                            componentNode = componentNode->NextSibling();
                            continue;
                        }

                        if (strcmp(componentElement->Name(), "Component") == 0)
                        {
                            S32 componentIndex = componentElement->IntAttribute("_i");
                            if (componentIndex == 0)
                                componentIndex = --autoComponentIndexCounter;

                            ComponentPtr const& component = components[componentIndex];
                            if (!component)
                            {
                                componentNode = componentNode->NextSibling();
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

                                    CString attributeValue = componentElement->Attribute(propertyName);
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

                        componentNode = componentNode->NextSibling();
                    }
                }

                entityNode = entityNode->NextSibling();
            }
        }

        // Add to ECS scene
        for (auto entityData : entities)
        {
            if (!entityData.second)
                continue;

            entityData.second->setECSScene(scene);
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
    void EntitySerializationManager::collectAllChildrenEntity(EntityPtr const& _entity, Vector<EntityPtr>& _result) const
    {
        _result.emplace_back(_entity);

        {
            Transform2D* transform = _entity->getComponentRaw<Transform2D>();
            if (transform)
            {
                for (Transform2D* childTransform : transform->getChildren())
                    collectAllChildrenEntity(childTransform->getEntity(), _result);
            }
        }

        {
            Transform3D* transform = _entity->getComponentRaw<Transform3D>();
            if (transform)
            {
                for (Transform3D* childTransform : transform->getChildren())
                    collectAllChildrenEntity(childTransform->getEntity(), _result);
            }
        }
    }

    //////////////////////////////////////////
    Vector<EntityPtr> EntitySerializationManager::collectAllChildrenEntity(EntityPtr const& _entity) const
    {
        Vector<EntityPtr> result;
        collectAllChildrenEntity(_entity, result);
        return result;
    }

    //////////////////////////////////////////
    Map<EntityPtr, Vector<ComponentPtr>> EntitySerializationManager::collectEntityComponentsMap(EntityPtr const& _entity) const
    {
        Map<EntityPtr, Vector<ComponentPtr>> result;

        Vector<EntityPtr> entities = collectAllChildrenEntity(_entity);
        for (EntityPtr const& entity : entities)
        {
            Vector<ComponentPtr> components;
            for (auto const& componentData : entity->getComponents())
                components.push_back(componentData.second);

            result[entity] = components;
        }

        return result;
    }

} // namespace Maze
//////////////////////////////////////////
