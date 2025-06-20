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
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/MazeCustomComponentSystemHolder.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeEcsWorldEventsQueue.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    ComponentSystemEventHandlerPtr const& FindSystem(
        Vector<ComponentSystemEventHandlerPtr> const& _eventHandlers,
        HashedString const& _name)
    {
        static ComponentSystemEventHandlerPtr nullPointer;

        auto it = std::find_if(
            _eventHandlers.begin(),
            _eventHandlers.end(),
            [&_name](ComponentSystemEventHandlerPtr const& _system) {
                return _system->getName() == _name;
            });
        if (it == _eventHandlers.end())
            return nullPointer;

        return *it;
    }


    //////////////////////////////////////////
    void CalculateAfterAndBeforeIndices(
        Vector<ComponentSystemEventHandlerPtr> const& _eventHandlers,
        ComponentSystemEventHandlerPtr const& _system,
        S32& _afterIndex,
        S32& _beforeIndex)
    {
        S32 arrSize = (S32)_eventHandlers.size();
        _afterIndex = -1;
        _beforeIndex = arrSize;

        for (S32 i = 0; i < arrSize; ++i)
        {
            ComponentSystemEventHandlerPtr const& system = _eventHandlers[i];
            if (system == _system)
                continue;

            if (system->getOrder().after.count(_system->getName()) || _system->getOrder().before.count(system->getName()))
            {
                _beforeIndex = i;
                break;
            }
        }

        for (S32 i = arrSize - 1; i > -1; --i)
        {
            ComponentSystemEventHandlerPtr const& system = _eventHandlers[i];
            if (system == _system)
                continue;

            if (system->getOrder().before.count(_system->getName()) || _system->getOrder().after.count(system->getName()))
            {
                _afterIndex = i;
                break;
            }
        }
    }


    //////////////////////////////////////////
    MAZE_CORE_API void SendEvent(EcsWorld* _ecsWorld, EntityId _id, EventPtr const& _event)
    {
        _ecsWorld->sendEvent(_id, _event);
    }

    //////////////////////////////////////////
    MAZE_CORE_API void SendEventImmediate(EcsWorld* _ecsWorld, EntityId _id, Event* _event)
    {
        _ecsWorld->sendEventImmediate(_id, _event);
    }


    //////////////////////////////////////////
    bool AddSystemEventHandler(
        Vector<ComponentSystemEventHandlerPtr>& _eventHandlers,
        ComponentSystemEventHandlerPtr const& _system,
        bool _rearrangeAvailable = true)
    {
        S32 arrSize = (S32)_eventHandlers.size();
        S32 afterIndex = -1;
        S32 beforeIndex = arrSize;
        CalculateAfterAndBeforeIndices(_eventHandlers, _system, afterIndex, beforeIndex);

        if (afterIndex < beforeIndex || afterIndex == -1)
        {
            _eventHandlers.insert(_eventHandlers.begin() + beforeIndex, _system);
            return true;
        }
        else
        if (beforeIndex == arrSize)
        {
            _eventHandlers.insert(_eventHandlers.begin() + (afterIndex + 1), _system);
            return true;
        }
        else
        {
            if (_rearrangeAvailable)
            {
                Vector<ComponentSystemEventHandlerPtr> shouldBeBeforeNewSystem;
                Vector<ComponentSystemEventHandlerPtr> shouldBeAfterNewSystem;

                for (ComponentSystemEventHandlerPtr const& s : _eventHandlers)
                {
                    if (s->getOrder().before.count(_system->getName()) || _system->getOrder().after.count(s->getName()))
                        shouldBeBeforeNewSystem.push_back(s);
                    else
                    if (s->getOrder().after.count(_system->getName()) || _system->getOrder().before.count(s->getName()))
                        shouldBeAfterNewSystem.push_back(s);
                }

                for (ComponentSystemEventHandlerPtr s : shouldBeAfterNewSystem)
                {
                    auto it = std::find(_eventHandlers.begin(), _eventHandlers.end(), s);
                    MAZE_DEBUG_ASSERT(it != _eventHandlers.end());
                    _eventHandlers.erase(it);

                    S32 sAfterIndex = -1;
                    S32 sBeforeIndex = arrSize;
                    CalculateAfterAndBeforeIndices(_eventHandlers, s, sAfterIndex, sBeforeIndex);
                    
                    _eventHandlers.insert(_eventHandlers.begin() + sBeforeIndex, s);
                }

                for (ComponentSystemEventHandlerPtr s : shouldBeBeforeNewSystem)
                {
                    auto it = std::find(_eventHandlers.begin(), _eventHandlers.end(), s);
                    MAZE_DEBUG_ASSERT(it != _eventHandlers.end());
                    _eventHandlers.erase(it);

                    S32 sAfterIndex = -1;
                    S32 sBeforeIndex = arrSize;
                    CalculateAfterAndBeforeIndices(_eventHandlers, s, sAfterIndex, sBeforeIndex);
                    
                    _eventHandlers.insert(_eventHandlers.begin() + (sAfterIndex + 1), s);
                }

                return AddSystemEventHandler(_eventHandlers, _system, false);
            }
            else
            {
                Debug::LogError("Systems list:");
                for (ComponentSystemEventHandlerPtr const& s : _eventHandlers)
                {
                    Debug::LogError("%s", s->getName().c_str());
                }

                MAZE_ERROR(
                    "Failed to place '%s' (after='%s' before='%s')!",
                    _system->getName().c_str(),
                    (afterIndex >= 0 && afterIndex < arrSize) ? _eventHandlers[afterIndex]->getName().c_str() : "NONE",
                    (beforeIndex >= 0 && beforeIndex < arrSize) ? _eventHandlers[beforeIndex]->getName().c_str() : "NONE");
                return false;
            }
        }
    }


    //////////////////////////////////////////
    // Class EcsWorld
    //
    //////////////////////////////////////////
    FastVector<EcsWorld*> EcsWorld::s_worlds = FastVector<EcsWorld*>();

    //////////////////////////////////////////
    Stack<EcsWorldId> EcsWorld::s_freeEcsWorldIndices = Stack<EcsWorldId>();
    
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EcsWorld);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EcsWorld);

    //////////////////////////////////////////
    EcsWorld::EcsWorld()
    {
        m_id = GenerateNewEcsWorldId(this);
    }

    //////////////////////////////////////////
    EcsWorld::~EcsWorld()
    {
        if (InputManager::GetInstancePtr())
        {
            InputManager::GetInstancePtr()->eventMouse.unsubscribe(this);
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
            InputManager::GetInstancePtr()->eventTouch.unsubscribe(this);
            InputManager::GetInstancePtr()->eventVirtualCursor.unsubscribe(this);
        }

        m_state = EcsWorldState::PreparingToDestroy;
        do
        {
            m_eventHolders.other()->processEvents();
            m_eventHolders.switchContainer();
        }
        while (m_eventHolders.other()->getAddingEntitiesCount() > 0 || m_eventHolders.current()->getAddingEntitiesCount() > 0);
        
        broadcastEventImmediate<EcsWorldWillBeDestroyedEvent>(EcsEventParams(false, false));

        ComponentSystemHolder::Detach(this);
        CustomComponentSystemHolder::Detach(this);

        m_state = EcsWorldState::Destroying;

        Size entitiesLeft = 0u;
        do
        {
            entitiesLeft = 0u;

            for (auto it = m_entities.begin(), end = m_entities.end(); it != end; ++it)
            {
                if (it->entity)
                {
                    ++entitiesLeft;
                    removeEntity(it->entity);
                }
            }

            m_eventHolders.other()->prepareForDestroy();
            m_eventHolders.switchContainer();
            m_eventHolders.other()->prepareForDestroy();
        }
        while (entitiesLeft > 0);

        m_samples.clear();
        
        m_eventHolders.other()->clear();
        m_eventHolders.current()->clear();

        m_entities.clear();

        m_eventHolders.other().reset();
        m_eventHolders.current().reset();

        m_state = EcsWorldState::Destroyed;
        eventOnDestroy(this);

        ReleaseEcsWorldId(m_id);
    }

    //////////////////////////////////////////
    EcsWorldPtr EcsWorld::Create(
        HashedString const& _name,
        bool _attachSystems,
        Set<HashedString> const& _tags)
    {
        EcsWorldPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EcsWorld, object, init(_name, _attachSystems, _tags));
        return object;
    }

    //////////////////////////////////////////
    EcsWorld* EcsWorld::GetDefaultWorldRaw()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorldRaw();
    }

    //////////////////////////////////////////
    bool EcsWorld::init(
        HashedString const& _name,
        bool _attachSystems,
        Set<HashedString> const& _tags)
    {
        m_name = _name;
        m_tags = _tags;

        m_eventHolders.current() = EcsWorldEventsQueue::Create(this);
        m_eventHolders.other() = EcsWorldEventsQueue::Create(this);

        if (_attachSystems)
        {
            ComponentSystemHolder::Attach(this);
            CustomComponentSystemHolder::Attach(this);
        }

        if (InputManager::GetInstancePtr())
        {
            InputManager::GetInstancePtr()->eventMouse.subscribe(this, &EcsWorld::notifyMouse);
            InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &EcsWorld::notifyKeyboard);
            InputManager::GetInstancePtr()->eventTouch.subscribe(this, &EcsWorld::notifyTouch);
            InputManager::GetInstancePtr()->eventVirtualCursor.subscribe(this, &EcsWorld::notifyVirtualCursor);
        }

        m_state = EcsWorldState::Active;

        return true;
    }

    //////////////////////////////////////////
    EntityPtr const& EcsWorld::getEntity(EntityId _id) const
    {
        static EntityPtr const nullValue;

        S32 index = convertEntityIdToIndex(_id);
        if (index >= 0 && index < (S32)m_entities.size())
        {
            EntityData const& entityData = m_entities[index];
            if (entityData.id == _id)
                return entityData.entity;
        }
        else
        {
            EntityPtr const& entity0 = m_eventHolders.current()->getAddingEntity(_id);
            if (entity0)
                return entity0;
            
            EntityPtr const& entity1 = m_eventHolders.other()->getAddingEntity(_id);
            if (entity1)
                return entity1;
        }

        return nullValue;
    }

    //////////////////////////////////////////
    void EcsWorld::reserveEntityIndices(Size _count)
    {
        MAZE_PROFILE_EVENT("EcsWorld::reserveEntityIndices");

        S32 startIndex = (S32)m_entities.size();
        
        m_entities.resize(m_entities.size() + _count);
        
        for (S32 i = 0; i < (S32)_count; ++i)
        {
            S32 index = startIndex + i;
            
            m_entities[index].id = EntityId(
                convertIndexToEntityIdIndex(index), 0);
            m_freeEntityIndices.push(index);
        }
    }

    //////////////////////////////////////////
    void EcsWorld::update(F32 _dt)
    {
        MAZE_DEBUG_ASSERT(!m_updatingNow);

        m_updatingNow = true;

        MAZE_PROFILE_EVENT("EcsWorld::update");

        
        if (m_eventHolders.other()->getEventsCount() == 0)
            m_eventHolders.switchContainer();

        m_eventHolders.other()->processEvents();

        {
            MAZE_PROFILE_EVENT("EcsWorld - PreUpdateEvent");

            PreUpdateEvent updateEvent(_dt);
            broadcastEventImmediate<PreUpdateEvent>(updateEvent);
        }
        {
            MAZE_PROFILE_EVENT("EcsWorld - UpdateEvent");

            UpdateEvent updateEvent(_dt);
            broadcastEventImmediate<UpdateEvent>(updateEvent);
        }
        {
            MAZE_PROFILE_EVENT("EcsWorld - PostUpdateEvent");

            PostUpdateEvent updateEvent(_dt);
            broadcastEventImmediate<PostUpdateEvent>(updateEvent);
        }
        
        for (Vector<IEntitiesSamplePtr>::const_iterator it = m_samples.begin(),
                                                        end = m_samples.end();
                                                        it != end;)
        {
            if ((*it).use_count() == 1)
            {
                it = m_samples.erase(it);
                end = m_samples.end();
            }
            else
                ++it;
        }

        ++m_frameNumber;
        m_updatingNow = false;
    }

    //////////////////////////////////////////
    EntityPtr EcsWorld::createEntity()
    {
        if (m_state != EcsWorldState::Active)
            return nullptr;

        EntityPtr entity = Entity::Create();
        addEntity(entity);
        return entity;
    }

    //////////////////////////////////////////
    bool EcsWorld::addEntity(EntityPtr const& _entity)
    {
        if (m_state != EcsWorldState::Active)
            return false;

        return m_eventHolders.current()->addEntity(_entity);
    }

    //////////////////////////////////////////
    bool EcsWorld::removeEntity(EntityPtr const& _entity)
    {
        return m_eventHolders.current()->removeEntity(_entity);
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityAddedToSample(
        ComponentSystemEventHandlerPtr const& _handler,
        EntityId _id)
    {
        m_eventHolders.current()->processEntityAddedToSample(_handler, _id);
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityRemovedFromSample(
        ComponentSystemEventHandlerPtr const& _handler,
        EntityId _id)
    {
        m_eventHolders.current()->processEntityRemovedFromSample(_handler, _id);
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityComponentsChanged(EntityId _id)
    {
        m_eventHolders.current()->processEntityComponentsChanged(_id);
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityActiveChanged(EntityId _id)
    {
        m_eventHolders.current()->processEntityActiveChanged(_id);
    }

    //////////////////////////////////////////
    Size EcsWorld::calculateEntitiesCount()
    {
        Size count = 0;
        for (EntityData const& entityData : m_entities)
            if (entityData.entity)
                ++count;
        return count;
    }

    //////////////////////////////////////////
    bool EcsWorld::addSystemEventHandler(ComponentSystemEventHandlerPtr const& _system)
    {
        if (!_system)
            return false;

        if (!std::includes(
            m_tags.begin(),
            m_tags.end(),
            _system->getTags().begin(),
            _system->getTags().end()))
            return false;

        ClassUID eventUID = _system->getEventUID();
        Vector<ComponentSystemEventHandlerPtr>& eventHandlers = m_eventHandlers[eventUID];
        
        if (!AddSystemEventHandler(eventHandlers, _system, true))
            return false;

        if (eventUID == ClassInfo<EntityAddedToSampleEvent>::UID())
        {
            m_entityAddedToSampleEventHandlers.push_back(
                ComponentSystemEntityAddedToSampleEventHandler::Create(this, _system));
        }
        else
        if (eventUID == ClassInfo<EntityRemovedFromSampleEvent>::UID())
        {
            m_entityRemovedFromSampleEventHandlers.push_back(
                ComponentSystemEntityRemovedFromSampleEventHandler::Create(this, _system));
        }

        return true;
    }

    //////////////////////////////////////////
    void EcsWorld::removeSystemEventHandler(ComponentSystemEventHandlerPtr const& _system)
    {
        if (!_system)
            return;

        ClassUID eventUID = _system->getEventUID();
        Vector<ComponentSystemEventHandlerPtr>& eventHandlers = m_eventHandlers[eventUID];
        for (Size i = 0, in = eventHandlers.size(); i < in; ++i)
        {
            if (eventHandlers[i] == _system)
            {
                eventHandlers.erase(eventHandlers.begin() + i);
                break;
            }
        }

        if (eventUID == ClassInfo<EntityAddedToSampleEvent>::UID())
        {
            m_entityAddedToSampleEventHandlers.erase(
                std::find_if(
                    m_entityAddedToSampleEventHandlers.begin(),
                    m_entityAddedToSampleEventHandlers.end(),
                    [systemRaw = _system.get()](auto const& _val)
                    {
                        return systemRaw == _val->getEventHandler().get();
                    }));
        }
        else
        if (eventUID == ClassInfo<EntityRemovedFromSampleEvent>::UID())
        {
            m_entityRemovedFromSampleEventHandlers.erase(
                std::find_if(
                    m_entityRemovedFromSampleEventHandlers.begin(),
                    m_entityRemovedFromSampleEventHandlers.end(),
                    [systemRaw = _system.get()](auto const& _val)
                    {
                        return systemRaw == _val->getEventHandler().get();
                    }));
        }
    }

    //////////////////////////////////////////
    EntitiesSamplePtr EcsWorld::requestCommonSample(
        EntityAspect const& _aspect,
        U8 _flags)
    {
        for (Size i = 0, in = m_samples.size(); i < in; ++i)
        {
            if (m_samples[i]->getType() != EntitiesSampleType::Common)
                continue;

            if (m_samples[i]->getFlags() != _flags)
                continue;

            if (m_samples[i]->getAspect() == _aspect)
                return m_samples[i]->cast<EntitiesSample>();
        }

        EntitiesSamplePtr sample = EntitiesSample::Create(getSharedPtr(), _aspect, _flags);
        m_samples.push_back(sample);

        for (Size i = 0, in = m_entities.size(); i < in; ++i)
            if (m_entities[i].entity)
                sample->processEntity(m_entities[i].entity.get());
        
        return sample;
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityForSamples(Entity* _entity)
    {
        for (Size i = 0, in = m_samples.size(); i < in; ++i)
            m_samples[i]->processEntity(_entity);
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityAddedForSamples(Entity* _entity)
    {
        Vector<ComponentSystemEventHandlerPtr> const& eventHandlers = m_eventHandlers[ClassInfo<EntityAddedToSampleEvent>::UID()];
        for (ComponentSystemEventHandlerPtr const& _eventHandler : eventHandlers)
            if (_eventHandler->getSample())
                _eventHandler->getSample()->processEntity(_entity);
            
        processEntityForSamples(_entity);
    }

    //////////////////////////////////////////
    EntityId EcsWorld::generateNewEntityId(EntityPtr const& _entity)
    {
        if (!m_freeEntityIndices.empty())
        {
            S32 index = m_freeEntityIndices.top();
            m_freeEntityIndices.pop();
            MAZE_DEBUG_ASSERT(index < (S32)m_entities.size() && !m_entities[index].entity);
            return m_entities[index].id;
        }

        return EntityId(
            convertIndexToEntityIdIndex((S32)m_entities.size() + (m_newEntityIdsCount++)), 0);
    }

    //////////////////////////////////////////
    S32 EcsWorld::convertEntityIdToIndex(EntityId _id) const
    {
        return _id.getIndex() - 1;
    }

    //////////////////////////////////////////
    S32 EcsWorld::convertIndexToEntityIdIndex(S32 _index) const
    {
        return _index + 1;
    }

    //////////////////////////////////////////
    void EcsWorld::addEntityNow(EntityPtr const& _entity)
    {
        EntityId entityId = _entity->getId();
        S32 index = convertEntityIdToIndex(entityId);
        if (index >= 0 && index < (S32)m_entities.size())
        {
            EcsWorld::EntityData& entityData = m_entities[index];
            MAZE_DEBUG_ASSERT(entityData.id == entityId);
            MAZE_DEBUG_ASSERT(entityData.entity == _entity);
        }
        else
        {
            m_entities.emplace_back(
                EcsWorld::EntityData(
                    entityId,
                    _entity));
            m_newEntityIdsCount--;
        }
    }

    //////////////////////////////////////////
    void EcsWorld::removeEntityNow(EntityId _id)
    {
        S32 index = convertEntityIdToIndex(_id);
        MAZE_DEBUG_ASSERT(index >= 0 && index < (S32)m_entities.size());
        EntityData const& entityData = m_entities[index];
        MAZE_DEBUG_ASSERT(entityData.entity);
        MAZE_DEBUG_ASSERT(entityData.id == _id);
        entityData.entity->setEcsScene(nullptr);
        m_freeEntityIndices.push(index);
        m_entities[index].entity.reset();
    }

    //////////////////////////////////////////
    void EcsWorld::broadcastEvent(EventPtr const& _event)
    {
        if (m_state != EcsWorldState::Active && m_state != EcsWorldState::PreparingToDestroy)
            return;

        m_eventHolders.current()->addBroadcastEvent(_event);
    }

    //////////////////////////////////////////
    void EcsWorld::sendEvent(EntityId _entityId, EventPtr const& _event)
    {
        if (m_state != EcsWorldState::Active && m_state != EcsWorldState::PreparingToDestroy)
            return;

        m_eventHolders.current()->addUnicastEvent(_entityId, _event);
    }

    //////////////////////////////////////////
    void EcsWorld::notifyMouse(InputEventMouseData const& _data)
    {
        switch (_data.type)
        {
            case InputEventMouseType::ButtonUp:
            {
                broadcastEventImmediate<InputCursorReleaseEvent>(
                    0,
                    _data.buttonId,
                    Vec2S(_data.x, _data.y),
                    _data.window);
                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void EcsWorld::notifyKeyboard(InputEventKeyboardData const& _data)
    {

    }

    //////////////////////////////////////////
    void EcsWorld::notifyTouch(InputEventTouchData const& _data)
    {
        switch (_data.type)
        {
            case InputEventTouchType::Release:
            {
                broadcastEventImmediate<InputCursorReleaseEvent>(
                    _data.index,
                    0,
                    Vec2S(_data.x, _data.y),
                    _data.window);
                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void EcsWorld::notifyVirtualCursor(InputEventVirtualCursorData const& _data)
    {
        switch (_data.type)
        {
            case InputEventVirtualCursorType::Release:
            {
                broadcastEventImmediate<InputCursorReleaseEvent>(
                    0,
                    0,
                    Vec2S(_data.x, _data.y),
                    _data.window);

                break;
            }
            default:
                break;
        }
    }


    //////////////////////////////////////////
    EcsWorld* EcsWorld::GetEcsWorld(EcsWorldId _id)
    {
        if ((Size)_id.getIndex() >= s_worlds.size())
            return nullptr;

        Size index = (Size)_id.getIndex();
        return s_worlds[index];
    }


    //////////////////////////////////////////
    EcsWorldId EcsWorld::GenerateNewEcsWorldId(EcsWorld* _world)
    {
        if (!s_freeEcsWorldIndices.empty())
        {
            EcsWorldId id = s_freeEcsWorldIndices.top();
            s_freeEcsWorldIndices.pop();
            s_worlds[(Size)id.getIndex()] = _world;
            return id;
        }

        EcsWorldId id((S8)s_worlds.size(), 0);
        s_worlds.push_back(_world);
        return id;
    }

    //////////////////////////////////////////
    void EcsWorld::ReleaseEcsWorldId(EcsWorldId _id)
    {
        s_worlds[(Size)_id.getIndex()] = nullptr;
        _id.incrementGeneration();
        s_freeEcsWorldIndices.push(_id);
    }

} // namespace Maze
//////////////////////////////////////////
