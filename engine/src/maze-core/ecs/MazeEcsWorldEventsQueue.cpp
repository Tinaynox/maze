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
#include "maze-core/ecs/MazeEcsWorldEventsQueue.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EcsWorldEventsQueue
    //
    //////////////////////////////////////////
    EcsWorldEventsQueue::EcsWorldEventsQueue()
    {
    }

    //////////////////////////////////////////
    EcsWorldEventsQueue::~EcsWorldEventsQueue()
    {
    }

    //////////////////////////////////////////
    EcsWorldEventsQueuePtr EcsWorldEventsQueue::Create(EcsWorld* _world)
    {
        EcsWorldEventsQueuePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EcsWorldEventsQueue, object, init(_world));
        return object;
    }
    
    //////////////////////////////////////////
    bool EcsWorldEventsQueue::init(EcsWorld* _world)
    {
        m_world = _world;

        return true;
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::processEvents()
    {
        m_processingEvents = true;

        for (EcsWorldEventType eventType : m_eventTypes)
        {
            switch (eventType)
            {
                case EcsWorldEventType::AddingEntity:
                {
                    invokeAddingEntityEvent();
                    break;
                }
                case EcsWorldEventType::RemovingEntity:
                {
                    invokeRemovingEntityEvent();
                    break;
                }
                case EcsWorldEventType::AddingToSampleEntity:
                {
                    invokeAddingToSampleEntityEvent();
                    break;
                }
                case EcsWorldEventType::RemovingFromSampleEntity:
                {
                    invokeRemovingFromSampleEntityEvent();
                    break;
                }
                case EcsWorldEventType::ComponentsChanged:
                {
                    invokeComponentsChangedEvent();
                    break;
                }
                case EcsWorldEventType::ActiveChanged:
                {
                    invokeActiveChangedEvent();
                    break;
                }
                case EcsWorldEventType::UsualEvent:
                {
                    invokeUsualEventEvent();
                    break;
                }
                default:
                    MAZE_ERROR("Unsupported EcsWorldEventType - %d!", eventType);
                    break;
            }
        }

        m_eventTypes.clear();

        MAZE_DEBUG_ASSERT(m_addingEntities.empty());
        MAZE_DEBUG_ASSERT(m_removingEntities.empty());
        MAZE_DEBUG_ASSERT(m_componentsChangedEntities.empty());
        MAZE_DEBUG_ASSERT(m_activeChangedEntities.empty());
        MAZE_DEBUG_ASSERT(m_usualEvents.empty());

        m_processingEvents = false;
    }

    //////////////////////////////////////////
    bool EcsWorldEventsQueue::addEntity(EntityPtr const& _entity)
    {
        MAZE_DEBUG_ASSERT(!m_processingEvents);

        MAZE_DEBUG_BP_IF(!_entity->getRemoving() && _entity->getEcsWorld());
        MAZE_DEBUG_BP_IF(_entity->getAdding());

        EntityId entityId = m_world->generateNewEntityId();
        _entity->setId(entityId);

        _entity->setAdding(true);
        _entity->setEcsWorld(m_world);

        m_addingEntities.push_back(_entity);
        m_eventTypes.push_back(EcsWorldEventType::AddingEntity);

        processEntityComponentsChanged(entityId);

        return true;
    }

    //////////////////////////////////////////
    bool EcsWorldEventsQueue::removeEntity(EntityPtr const& _entity)
    {
        MAZE_DEBUG_ASSERT(!m_processingEvents);

        if (_entity->getRemoving())
            return false;

        _entity->setRemoving(true);
        
        m_removingEntities.push(_entity->getId());
        m_eventTypes.push_back(EcsWorldEventType::RemovingEntity);

        return true;
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::processEntityAddedToSample(
        ComponentSystemEventHandlerPtr const& _handler,
        EntityId _id)
    {
        // Rework from sync to async later?
        EntityAddedToSampleEvent event;
        _handler->processEvent(_id, &event);
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::processEntityRemovedFromSample(
        ComponentSystemEventHandlerPtr const& _handler,
        EntityId _id)
    {
        // Rework from sync to async later?
        EntityRemovedFromSampleEvent event;
        _handler->processEvent(_id, &event, EcsEventParams(false, false));
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::processEntityComponentsChanged(EntityId _id)
    {
        MAZE_DEBUG_ASSERT(!m_processingEvents);

        EntityPtr const& entity = m_world->getEntity(_id);
        if (!entity)
            return;

        MAZE_DEBUG_ERROR_IF(entity->getEcsWorld() != m_world, "Entity from different world");

        if (entity->getComponentsChanged())
            return;

        entity->setComponentsChanged(true);

        m_componentsChangedEntities.push(_id);
        m_eventTypes.push_back(EcsWorldEventType::ComponentsChanged);
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::processEntityActiveChanged(EntityId _id)
    {
        MAZE_DEBUG_ASSERT(!m_processingEvents);

        EntityPtr const& entity = m_world->getEntity(_id);
        if (!entity)
            return;

        entity->setActiveChanged(true);
        m_activeChangedEntities.push(_id);
        m_eventTypes.push_back(EcsWorldEventType::ActiveChanged);

        m_world->sendEventImmediate<EntityActiveChangedEvent>(_id, entity->getActiveInHierarchy());
    }

    //////////////////////////////////////////
    EntityPtr const& EcsWorldEventsQueue::getAddingEntity(EntityId _id)
    {
        static EntityPtr nullPointer;

        for (EntityPtr const& entity : m_addingEntities)
            if (entity->getId() == _id)
                return entity;

        return nullPointer;
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeAddingEntityEvent()
    {
        EntityPtr entity = m_addingEntities.front();
        m_addingEntities.pop_front();

        m_world->addEntityNow(entity);
        
        entity->setAdding(false);
        entity->setActiveInHierarchyPrevFrame(entity->getActiveInHierarchy());


        if (!m_world->eventEntityAdded.empty())
            m_world->eventEntityAdded(entity);

        m_world->processEntityForSamples(entity.get());

        m_world->sendEventImmediate<EntityAddedEvent>(entity->getId());
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeRemovingEntityEvent()
    {
        EntityId entityId = m_removingEntities.front();
        m_removingEntities.pop();

        EntityPtr const& entity = m_world->getEntity(entityId);
        if (!entity)
            return;
        
        if (!entity->getRemoving())
            return;

        m_world->sendEventImmediate<EntityRemovedEvent>(entityId, EcsEventParams(false));

        entity->setEcsWorld(nullptr);
        entity->setRemoving(false);

        if (!m_world->eventEntityRemoved.empty())
            m_world->eventEntityRemoved(entity);

        m_world->processEntityForSamples(entity.get());

        m_world->removeEntityNow(entity->getId());
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeAddingToSampleEntityEvent()
    {
        MAZE_NOT_IMPLEMENTED;
        /*
        Pair<ComponentSystemEventHandlerPtr, EntityId> value = m_addingToSampleEntities.front();
        m_addingToSampleEntities.pop();

        EntityAddedToSampleEvent event;
        value.first->processEvent(value.second, &event);
        */
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeRemovingFromSampleEntityEvent()
    {
        MAZE_NOT_IMPLEMENTED;
        /*
        Pair<ComponentSystemEventHandlerPtr, EntityId> value = m_removingFromSampleEntities.front();
        m_removingFromSampleEntities.pop();

        EntityRemovedFromSampleEvent event;
        value.first->processEvent(value.second, &event, EcsEventParams(false, false));
        */
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeComponentsChangedEvent()
    {
        EntityId entityId = m_componentsChangedEntities.front();
        m_componentsChangedEntities.pop();

        EntityPtr const& entity = m_world->getEntity(entityId);
        if (!entity)
            return;
        
        entity->setComponentsChanged(false);

        if (!m_world->eventEntityChanged.empty())
            m_world->eventEntityChanged(entity);

        m_world->processEntityForSamples(entity.get());        
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeActiveChangedEvent()
    {
        EntityId entityId = m_activeChangedEntities.front();
        m_activeChangedEntities.pop();

        EntityPtr const& entity = m_world->getEntity(entityId);
        if (!entity)
            return;
        
        MAZE_DEBUG_ERROR_IF(entity->getEcsWorld() != m_world, "Entity from different world");

        entity->setActiveChanged(false);

        bool value = entity->getActiveInHierarchy();
        if (value == entity->getActiveInHierarchyPrevFrame())
            return;

        entity->setActiveInHierarchyPrevFrame(entity->getActiveInHierarchy());

        if (!value)
            m_world->sendEventImmediate<EntityActiveChangedEvent>(entity->getId(), value);

        m_world->processEntityForSamples(entity.get());

        if (value)
            m_world->sendEventImmediate<EntityActiveChangedEvent>(entity->getId(), value);

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEvent<EcsEntityActiveChangedEvent>(
                m_world, entityId, value);
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::invokeUsualEventEvent()
    {
        EventPtr evt = m_usualEvents.front();
        m_usualEvents.pop();

        MAZE_NOT_IMPLEMENTED
    }

    //////////////////////////////////////////
    void EcsWorldEventsQueue::clear()
    {
        MAZE_DEBUG_ASSERT(!m_processingEvents);

        m_eventTypes.clear();
        m_addingEntities.clear();
        while (!m_removingEntities.empty()) { m_removingEntities.pop(); }
        while (!m_componentsChangedEntities.empty()) { m_componentsChangedEntities.pop(); }
        while (!m_activeChangedEntities.empty()) { m_activeChangedEntities.pop(); }
        while (!m_usualEvents.empty()) { m_usualEvents.pop(); }
    }


} // namespace Maze
//////////////////////////////////////////
