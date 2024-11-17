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
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Entity
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(Entity,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(EntityId, id, c_invalidEntityId, getId, setId),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, flags, U8(Entity::Flags::ActiveSelf), getFlags, setFlags));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Entity);

    //////////////////////////////////////////
    Entity::Entity()
    {
    }

    //////////////////////////////////////////
    Entity::~Entity()
    {
        setEcsScene(nullptr);

        while (!m_components.empty())
        {
            m_components.begin()->second->setEntity(nullptr);
            m_components.erase(m_components.begin());
        }
    }

    //////////////////////////////////////////
    EntityPtr Entity::Create()
    {
        EntityPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Entity, object, init());
        return object;
    }

    //////////////////////////////////////////
    EntityPtr Entity::Create(
        Entity* _entity,
        EntityCopyData _copyData)
    {
        EntityPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Entity, object, init(_entity, _copyData));
        return object;
    }

    //////////////////////////////////////////
    EntityPtr const& Entity::GetEntity(EntityId _eid)
    {
        return EcsWorld::GetDefaultWorldRaw()->getEntity(_eid);
    }

    //////////////////////////////////////////
    bool Entity::init()
    {
        return true;
    }

    //////////////////////////////////////////
    bool Entity::init(
        Entity* _entity,
        EntityCopyData _copyData)
    {
        m_transitionFlags |= static_cast<U8>(TransitionFlags::AwakeForbidden);

        _copyData.pushStackDepth();
        _copyData.getEntities()[_entity] = this;

        if (!_copyData.getWorld())
            _copyData.setWorld(_entity->m_world);

        if (!_copyData.getScene())
            _copyData.setScene(_entity->m_scene);

        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(!_copyData.getWorld(), false, "You cannot copy entities out of World!");

        setEcsScene(_copyData.getScene());

        setFlags(_entity->m_flags);

        _copyData.getWorld()->addEntity(getSharedPtr());

        for (auto const& componentData : _entity->m_components)
        {
            auto componentIt = m_components.find(componentData.second->getComponentId());
            if (componentIt != m_components.end())
            {
                componentIt->second->init(
                    componentData.second.get(),
                    _copyData);
            }
            else
            {
                ComponentPtr component = componentData.second->createCopy(_copyData);
                addComponent(component);
            }

        }

        _copyData.popStackDepth();

        // Root entity serialized
        if (_copyData.getStackDepth() == 0)
        {
            for (EntityCopyData::ComponentPropertyData const& data : _copyData.getComponentProperties())
            {
                ClassUID metaPropertyClassUID = data.metaProperty->getValueClassUID();
                if (metaPropertyClassUID == ClassInfo<ComponentPtr>::UID())
                {
                    ComponentPtr component;
                    data.metaProperty->getValue(data.objMetaInstance, component);

                    if (component)
                    {
                        auto const& it = _copyData.getComponents().find(component.get());
                        if (it != _copyData.getComponents().end())
                        {
                            ComponentPtr copiedComponent = it->second->getSharedPtr();
                            data.metaProperty->setValue(data.metaInstance, &copiedComponent);
                        }
                        else
                        {
                            data.metaProperty->copy(data.metaInstance, data.objMetaInstance);
                        }
                    }
                    else
                    {
                        data.metaProperty->copy(data.metaInstance, data.objMetaInstance);
                    }
                }
                else
                if (metaPropertyClassUID == ClassInfo<Vector<ComponentPtr>>::UID())
                {
                    Vector<ComponentPtr> components;
                    data.metaProperty->getValue(data.objMetaInstance, components);

                    Vector<ComponentPtr> copiedComponents;
                    copiedComponents.resize(components.size());

                    for (Size i = 0, in = components.size(); i < in; ++i)
                    {
                        if (components[i])
                        {
                            auto const& it = _copyData.getComponents().find(components[i].get());
                            if (it != _copyData.getComponents().end())
                            {
                                ComponentPtr copiedComponent = it->second->getSharedPtr();
                                copiedComponents[i] = copiedComponent;
                            }
                            else
                            {
                                copiedComponents[i] = components[i];
                            }
                        }
                        else
                        {
                            copiedComponents[i] = components[i];
                        }
                    }

                    data.metaProperty->setValue(data.metaInstance, &copiedComponents);
                }
                else
                {
                    MAZE_NOT_IMPLEMENTED
                }
            }

            for (EntityCopyData::EntityPropertyData const& data : _copyData.getEntityProperties())
            {
                ClassUID metaPropertyClassUID = data.metaProperty->getValueClassUID();
                if (metaPropertyClassUID == ClassInfo<EntityPtr>::UID())
                {
                    EntityPtr entity;
                    data.metaProperty->getValue(data.objMetaInstance, entity);

                    auto const& it = _copyData.getEntities().find(entity.get());
                    if (it != _copyData.getEntities().end())
                    {
                        EntityPtr copiedEntity = it->second->getSharedPtr();
                        data.metaProperty->setValue(data.metaInstance, &copiedEntity);
                    }
                    else
                    {
                        data.metaProperty->copy(data.metaInstance, data.objMetaInstance);
                    }
                }
                else
                if (metaPropertyClassUID == ClassInfo<Vector<EntityPtr>>::UID())
                {
                    Vector<EntityPtr> entities;
                    data.metaProperty->getValue(data.objMetaInstance, entities);

                    Vector<EntityPtr> copiedEntities;
                    copiedEntities.resize(entities.size());

                    for (Size i = 0, in = entities.size(); i < in; ++i)
                    {
                        auto const& it = _copyData.getEntities().find(entities[i].get());
                        if (it != _copyData.getEntities().end())
                        {
                            EntityPtr copiedEntity = it->second->getSharedPtr();
                            copiedEntities[i] = copiedEntity;
                        }
                        else
                        {
                            copiedEntities[i] = entities[i];
                        }
                    }

                    data.metaProperty->setValue(data.metaInstance, &copiedEntities);
                }
                else
                {
                    MAZE_NOT_IMPLEMENTED
                }
            }
        }


        m_transitionFlags &= ~static_cast<U8>(TransitionFlags::AwakeForbidden);
        tryAwake();

        return true;
    }

    //////////////////////////////////////////
    ComponentPtr const& Entity::addComponent(ComponentPtr const& _component)
    {
        static ComponentPtr const nullPointer;
        MAZE_DEBUG_BP_RETURN_VALUE_IF(!_component, nullPointer);
        

        auto componentIt = m_components.find(_component->getComponentId());
        if (componentIt != m_components.end())
        {
            MAZE_ERROR("Component %s (%d) already exists!", componentIt->second->getClassName().str, (S32)_component->getComponentId());
            return componentIt->second;
        }
        
        _component->removeFromEntity();
        m_components.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_component->getComponentId()),
            std::forward_as_tuple(_component));
        _component->setEntity(this);

        m_flags |= (U8)Flags::ComponentsMaskDirty;

        if (m_world)
        {
            m_world->processEntityComponentsChanged(getId());
        }
                

        if (m_transitionFlags & static_cast<U8>(TransitionFlags::Awakened))
        {
            _component->processEntityAwakened();
        }

        if (getEcsScene())
            _component->processSceneSet();

        return _component;
    }

    //////////////////////////////////////////
    bool Entity::removeComponent(ComponentId _componentUID)
    {
        ComponentsContainer::iterator it = m_components.find(_componentUID);
        if (it == m_components.end())
            return false;
        
        it->second->setEntity(nullptr);
        
        m_components.erase(it);
        m_flags |= (U8)Flags::ComponentsMaskDirty;
            
        if (m_world)
        {
            m_world->processEntityComponentsChanged(getId());
        }

        return true;
    }

    //////////////////////////////////////////
    void Entity::tryAwake()
    {
        if (m_transitionFlags & static_cast<U8>(TransitionFlags::AwakeForbidden))
            return;

        if (m_transitionFlags & static_cast<U8>(TransitionFlags::Awakened))
            return;

        m_transitionFlags |= static_cast<U8>(TransitionFlags::Awakened);


        // #TODO: Remove
        for (auto const& componentData : m_components)
            componentData.second->processEntityAwakened();
    }

    //////////////////////////////////////////
    void Entity::setEcsWorld(EcsWorld* _world)
    {
        if (m_world == _world)
            return;

        m_world = _world;

        if (_world)
        {
            tryAwake();
        }
    }

    //////////////////////////////////////////
    void Entity::setEcsScene(EcsScene* _scene)
    {
        if (m_scene == _scene)
            return;

        if (m_scene)
        {
            for (auto const& componentData : m_components)
                componentData.second->processSceneReset();

            m_scene->processEntityRemoved(this);
        }

        m_scene = _scene;

        if (m_scene)
        {
            m_scene->processEntityAdded(this);

            for (auto const& componentData : m_components)
                componentData.second->processSceneSet();
            
        }
    }

    //////////////////////////////////////////
    void Entity::removeFromEcsWorld()
    {
        if (!m_world)
            return;
            
        m_world->removeEntity(getSharedPtr());
            
    }

    //////////////////////////////////////////
    void Entity::setFlags(U8 _flags)
    {
        if (m_flags == _flags)
            return;

        U8 prevFlags = m_flags;
        m_flags = _flags;

        if (m_world)
        {
            // Active changed
            if ((m_flags ^ prevFlags) & (U8(Flags::ActiveSelf) | U8(Flags::DisabledByHierarchy)))
            {

                bool prevActiveInHierarchy = 
                    (prevFlags & static_cast<U8>(Flags::ActiveSelf)) &&
                    !(prevFlags & static_cast<U8>(Flags::DisabledByHierarchy));

                bool currentActiveInHierarchy = getActiveInHierarchy();

                if (prevActiveInHierarchy != currentActiveInHierarchy)
                {
                    m_world->processEntityActiveChanged(getId());

                    if (currentActiveInHierarchy)
                    {
                        for (auto const& componentData : m_components)
                            componentData.second->processEntityEnabled();
                    }
                    else
                    {
                        for (auto const& componentData : m_components)
                            componentData.second->processEntityDisabled();
                    }
                }
            }
        }
    }

    //////////////////////////////////////////
    void Entity::processEvent(Event* _event)
    {
        if (!getEcsWorld())
            return;

        for (auto const& componentData : m_components)
        {
            componentData.second->processEvent(_event);
            
            if (_event->getCaptured())
                break;
        }
    }
    
    
} // namespace Maze
//////////////////////////////////////////
