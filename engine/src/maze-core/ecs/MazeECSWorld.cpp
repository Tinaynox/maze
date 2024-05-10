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
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // #define MAZE_ECS_EXTENSIVE_CHECKS (1)


    //////////////////////////////////////////
    SimpleComponentSystemEventHandlerPtr const& FindSystem(
        Vector<SimpleComponentSystemEventHandlerPtr> const& _eventHandlers,
        HashedString const& _name)
    {
        static SimpleComponentSystemEventHandlerPtr nullPointer;

        auto it = std::find_if(
            _eventHandlers.begin(),
            _eventHandlers.end(),
            [&_name](SimpleComponentSystemEventHandlerPtr const& _system) {
                return _system->getName() == _name;
            });
        if (it == _eventHandlers.end())
            return nullPointer;

        return *it;
    }


    //////////////////////////////////////////
    void CalculateAfterAndBeforeIndices(
        Vector<SimpleComponentSystemEventHandlerPtr> const& _eventHandlers,
        SimpleComponentSystemEventHandlerPtr const& _system,
        S32& afterIndex,
        S32& beforeIndex)
    {
        S32 arrSize = (S32)_eventHandlers.size();
        afterIndex = -1;
        beforeIndex = arrSize;

        for (S32 i = 0; i < arrSize; ++i)
        {
            SimpleComponentSystemEventHandlerPtr const& system = _eventHandlers[i];
            if (system == _system)
                continue;

            if (system->getOrder().after.count(_system->getName()) || _system->getOrder().before.count(system->getName()))
            {
                beforeIndex = i;
                break;
            }
        }

        for (S32 i = arrSize - 1; i > -1; --i)
        {
            SimpleComponentSystemEventHandlerPtr const& system = _eventHandlers[i];
            if (system == _system)
                continue;

            if (system->getOrder().before.count(_system->getName()) || _system->getOrder().after.count(system->getName()))
            {
                afterIndex = i;
                break;
            }
        }
    }


    //////////////////////////////////////////
    bool AddSystemEventHandler(
        Vector<SimpleComponentSystemEventHandlerPtr>& _eventHandlers,
        SimpleComponentSystemEventHandlerPtr const& _system,
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
                Vector<SimpleComponentSystemEventHandlerPtr> shouldBeBeforeNewSystem;
                Vector<SimpleComponentSystemEventHandlerPtr> shouldBeAfterNewSystem;

                for (SimpleComponentSystemEventHandlerPtr const& s : _eventHandlers)
                {
                    if (s->getOrder().before.count(_system->getName()) || _system->getOrder().after.count(s->getName()))
                        shouldBeBeforeNewSystem.push_back(s);
                    else
                    if (s->getOrder().after.count(_system->getName()) || _system->getOrder().before.count(s->getName()))
                        shouldBeAfterNewSystem.push_back(s);
                }

                for (SimpleComponentSystemEventHandlerPtr s : shouldBeAfterNewSystem)
                {
                    auto it = std::find(_eventHandlers.begin(), _eventHandlers.end(), s);
                    MAZE_ASSERT(it != _eventHandlers.end());
                    _eventHandlers.erase(it);

                    S32 sAfterIndex = -1;
                    S32 sBeforeIndex = arrSize;
                    CalculateAfterAndBeforeIndices(_eventHandlers, s, sAfterIndex, sBeforeIndex);
                    
                    _eventHandlers.insert(_eventHandlers.begin() + sBeforeIndex, s);
                }

                for (SimpleComponentSystemEventHandlerPtr s : shouldBeBeforeNewSystem)
                {
                    auto it = std::find(_eventHandlers.begin(), _eventHandlers.end(), s);
                    MAZE_ASSERT(it != _eventHandlers.end());
                    _eventHandlers.erase(it);

                    S32 sAfterIndex = -1;
                    S32 sBeforeIndex = arrSize;
                    CalculateAfterAndBeforeIndices(_eventHandlers, s, sAfterIndex, sBeforeIndex);
                    
                    _eventHandlers.insert(_eventHandlers.begin() + (sAfterIndex + 1), s);
                }

                AddSystemEventHandler(_eventHandlers, _system, false);

                return true;
            }
            else
            {
                Debug::LogError("Systems list:");
                for (SimpleComponentSystemEventHandlerPtr const& s : _eventHandlers)
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
    MAZE_IMPLEMENT_METACLASS(EcsWorld);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EcsWorld);

    //////////////////////////////////////////
    EcsWorld::EcsWorld()
        : m_entitiesIdCounter(0)
    {
    }

    //////////////////////////////////////////
    EcsWorld::~EcsWorld()
    {
        SimpleComponentSystemHolder::Detach(this);

        m_samples.clear();

        m_entitiesMap.clear();
        m_addingEntities.clear();
        m_removingEntities.clear();
        m_componentsChangedEntities.clear();        
        m_activeChangedEntities.clear();
        m_activeEntities.clear();

        eventOnDestroy(this);
    }

    //////////////////////////////////////////
    EcsWorldPtr EcsWorld::Create(
        HashedString const& _name,
        EntityId _entitiesIdCounter,
        bool _attachSystems)
    {
        EcsWorldPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EcsWorld, object, init(_name, _entitiesIdCounter, _attachSystems));
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
        EntityId _entitiesIdCounter,
        bool _attachSystems)
    {
        m_name = _name;
        m_entitiesIdCounter = _entitiesIdCounter;

        if (_attachSystems)
        {
            SimpleComponentSystemHolder::Attach(this);
        }

        return true;
    }

    //////////////////////////////////////////
    void EcsWorld::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("EcsWorld::update");

        processStartedEntities();
        processAddingEntities();
        processChangedEntities();
        processRemovingEntities();

        {
            PreUpdateEvent updateEvent(_dt);
            sendEventImmediate<PreUpdateEvent>(updateEvent);
        }
        {
            UpdateEvent updateEvent(_dt);
            sendEventImmediate<UpdateEvent>(updateEvent);
        }
        {
            PostUpdateEvent updateEvent(_dt);
            sendEventImmediate<PostUpdateEvent>(updateEvent);
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
    }

    //////////////////////////////////////////
    EntityPtr EcsWorld::createEntity()
    {
        EntityPtr entity = Entity::Create();
        addEntity(entity);
        return entity;
    }

    //////////////////////////////////////////
    bool EcsWorld::addEntity(EntityPtr const& _entity)
    {
        MAZE_DEBUG_BP_IF(!_entity->getRemoving() && _entity->getEcsWorld());
        MAZE_DEBUG_BP_IF(_entity->getAdding());

        // Generate id
        if (_entity->getId() == 0)
        {
            _entity->setId(++m_entitiesIdCounter);
        }

        MAZE_DEBUG_BP_IF(getEntityById(_entity->getId()) != nullptr);

        m_entitiesMap[_entity->getId()] = _entity;

        if (_entity->getRemoving())
        {
            _entity->setRemoving(false);

            {
                Deque<Entity*>::iterator it = std::find(m_removingEntities.begin(), m_removingEntities.end(), _entity.get());
                if (it != m_removingEntities.end())
                {
                    m_removingEntities.erase(it);
                }
            }

            _entity->setEcsWorld(this);
            return true;
        }

        _entity->setAdding(true);

        m_addingEntities.push_back(_entity.get());

        _entity->setEcsWorld(this);


        if (_entity->getActiveInHierarchy())
        {
            m_activeEntities.emplace_back(_entity.get());
        }

        _entity->setActiveInHierarchyPrevFrame(_entity->getActiveInHierarchy());

        m_startedEntities.current().emplace_back(_entity.get());

        return true;
    }

    //////////////////////////////////////////
    bool EcsWorld::removeEntity(EntityPtr const& _entity)
    {
        Entity* entityRaw = _entity.get();

        if (_entity->getRemoving())
        {
            return false;
        }

        if (_entity->getAdding())
        {
            _entity->setAdding(false);
            
            {
                Deque<Entity*>::iterator it = std::find(m_addingEntities.begin(), m_addingEntities.end(), entityRaw);
                if (it != m_addingEntities.end())
                {
                    m_addingEntities.erase(it);
                }
            }

            if (_entity->getComponentsChanged())
            {
                Deque<Entity*>::iterator it = std::find(m_componentsChangedEntities.begin(), m_componentsChangedEntities.end(), entityRaw);
                if (it != m_componentsChangedEntities.end())
                {
                    m_componentsChangedEntities.erase(it);
                }

                _entity->setComponentsChanged(false);
            }

            if (_entity->getActiveChanged())
            {
                Deque<Entity*>::iterator startIt = m_activeChangedEntities.begin();
                Deque<Entity*>::iterator it = std::find(startIt, m_activeChangedEntities.end(), entityRaw);
                while (it != m_activeChangedEntities.end())
                {
                    startIt = m_activeChangedEntities.erase(it);
                    it = std::find(startIt, m_activeChangedEntities.end(), entityRaw);
                }

                _entity->setActiveChanged(false);
            }

            // if (_entity->getActiveInHierarchy())
            {
                auto it = m_activeEntities.find(entityRaw);
                if (it != m_activeEntities.end())
                {
                    m_activeEntities.eraseUnordered(it);
                }
            }

            if (!_entity->getTransitionFlag(Entity::TransitionFlags::Started))
            {
                {
                    auto it = m_startedEntities.current().find(entityRaw);
                    if (it != m_startedEntities.current().end())
                        m_startedEntities.current().erase(it);
                }

                {
                    auto it = m_startedEntities.other().find(entityRaw);
                    if (it != m_startedEntities.other().end())
                        m_startedEntities.other().erase(it);
                }
            }

            _entity->setEcsScene(nullptr);
            _entity->setEcsWorld(nullptr);

            m_entitiesMap.erase(_entity->getId());
#if (MAZE_ECS_EXTENSIVE_CHECKS)
            _validateDontHave(entityRaw);
#endif

            return true;
        }
       
        _entity->setRemoving(true);
        m_removingEntities.push_back(entityRaw);

        processEntityComponentsChanged(entityRaw);

        return true;
    }

    //////////////////////////////////////////
    EntityPtr const& EcsWorld::getEntityById(EntityId _id) const
    {
        static EntityPtr const nullPointer;
        
        if (_id == 0)
            return nullPointer;
        
        {
            UnorderedMap<EntityId, EntityPtr>::const_iterator it = m_entitiesMap.find(_id);
            if (it != m_entitiesMap.end())
            {
                if (it->second->getRemoving())
                    return nullPointer;

                return it->second;
            }
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    Size EcsWorld::getEntitiesCount()
    {
        return m_entitiesMap.size();
    }

    //////////////////////////////////////////
    void EcsWorld::processStartedEntities()
    {
        auto& startedEntities = m_startedEntities.switchContainer();

        for (Entity* entity : startedEntities)
            entity->tryStart();

        startedEntities.clear();
    }

    //////////////////////////////////////////
    void EcsWorld::processAddingEntities()
    {
        while (!m_addingEntities.empty())
        {
            Entity* entity = m_addingEntities.front();
            m_addingEntities.pop_front();

            entity->setAdding(false);
            
            processEntityComponentsChanged(entity);

            if (!eventEntityAdded.empty())
            {
                EntityPtr entityShared = entity->getSharedPtr();
                eventEntityAdded(entityShared);
            }
        }
    }

    //////////////////////////////////////////
    void EcsWorld::processRemovingEntities()
    {
        while (!m_removingEntities.empty())
        {
            Entity* entity = m_removingEntities.front();
            m_removingEntities.pop_front();
            
            entity->setEcsScene(nullptr);
            entity->setEcsWorld(nullptr);
            entity->setRemoving(false);

            if (entity->getComponentsChanged())
            {
                Deque<Entity*>::iterator it = std::find(m_componentsChangedEntities.begin(), m_componentsChangedEntities.end(), entity);
                if (it != m_componentsChangedEntities.end())
                {
                    m_componentsChangedEntities.erase(it);
                }

                entity->setComponentsChanged(false);
            }

            if (entity->getActiveChanged())
            {
                Deque<Entity*>::iterator startIt = m_activeChangedEntities.begin();
                Deque<Entity*>::iterator it = std::find(startIt, m_activeChangedEntities.end(), entity);
                while (it != m_activeChangedEntities.end())
                {
                    startIt = m_activeChangedEntities.erase(it);
                    it = std::find(startIt, m_activeChangedEntities.end(), entity);
                }

                entity->setActiveChanged(false);
            }

            if (!eventEntityRemoved.empty())
            {
                EntityPtr entityShared = entity->getSharedPtr();
                eventEntityRemoved(entityShared);            
            }

            // if (entity->getActiveInHierarchy())
            {
                auto it = m_activeEntities.find(entity);
                if (it != m_activeEntities.end())
                {
                    m_activeEntities.eraseUnordered(it);
                }
            }

            for (Size i = 0, in = m_samples.size(); i < in; ++i)
                m_samples[i]->processEntity(entity);

            m_entitiesMap.erase(entity->getId());
#if (MAZE_ECS_EXTENSIVE_CHECKS)
            _validateDontHave(entity);
#endif

            processChangedEntities();
        }
    }

    //////////////////////////////////////////
    void EcsWorld::processChangedEntities()
    {
        MAZE_PROFILE_EVENT("EcsWorld::processChangedEntities");

        while (!m_componentsChangedEntities.empty())
        {
            Entity* entity = m_componentsChangedEntities.front();
            entity->setComponentsChanged(false);
            m_componentsChangedEntities.pop_front();

            for (Size i = 0, in = m_samples.size(); i < in; ++i) 
                m_samples[i]->processEntity(entity);
        }

        while (!m_activeChangedEntities.empty())
        {
            Entity* entity = m_activeChangedEntities.front();

            MAZE_DEBUG_ERROR_IF(entity->getEcsWorld() != this, "Entity from different world");

            entity->setActiveChanged(false);
            m_activeChangedEntities.pop_front();

            if (entity->getActiveInHierarchy() == entity->getActiveInHierarchyPrevFrame())
                continue;

            entity->setActiveInHierarchyPrevFrame(entity->getActiveInHierarchy());


            if (entity->getActiveInHierarchy())
            {
                // MAZE_DEBUG_ERROR_IF(m_activeEntities.find(entity) != m_activeEntities.end(), "activeEntities array is not synchronized");
                m_activeEntities.emplace_back(entity);
            }
            else
            {
                auto it = m_activeEntities.find(entity);
                if (it != m_activeEntities.end())
                {
                    m_activeEntities.eraseUnordered(it);
                }
            }

            for (Size i = 0, in = m_samples.size(); i < in; ++i)
                m_samples[i]->processEntity(entity);
        }

    }

    //////////////////////////////////////////
    void EcsWorld::processEntityComponentsChanged(Entity* _entity)
    {
        MAZE_DEBUG_ERROR_IF(_entity->getEcsWorld() != this, "Entity from different world");

        if (_entity->getComponentsChanged())
            return;

        _entity->setComponentsChanged(true);
        m_componentsChangedEntities.push_back(_entity);

        eventEntityChanged(_entity);
    }

    //////////////////////////////////////////
    void EcsWorld::processEntityActiveChanged(Entity* _entity)
    {
        _entity->setActiveChanged(true);
        
        m_activeChangedEntities.push_back(_entity);
    }

    //////////////////////////////////////////
    void EcsWorld::addSystemEventHandler(SimpleComponentSystemEventHandlerPtr const& _system)
    {
        if (!_system)
            return;

        ClassUID eventUID = _system->getEventUID();
        Vector<SimpleComponentSystemEventHandlerPtr>& eventHandlers = m_eventHandlers[eventUID];
        
        AddSystemEventHandler(eventHandlers, _system, true);
    }

    //////////////////////////////////////////
    void EcsWorld::removeSystemEventHandler(SimpleComponentSystemEventHandlerPtr const& _system)
    {
        if (!_system)
            return;

        ClassUID eventUID = _system->getEventUID();
        Vector<SimpleComponentSystemEventHandlerPtr>& eventHandlers = m_eventHandlers[eventUID];
        for (Size i = 0, in = eventHandlers.size(); i < in; ++i)
        {
            if (eventHandlers[i] == _system)
            {
                eventHandlers.erase(eventHandlers.begin() + i);
                return;
            }
        }
    }

    //////////////////////////////////////////
    EntitiesSamplePtr EcsWorld::requestCommonSample(EntityAspect const& _aspect)
    {
        for (Size i = 0, in = m_samples.size(); i < in; ++i)
        {
            if (m_samples[i]->getType() != EntitiesSampleType::Common)
                continue;

            if (m_samples[i]->getAspect() == _aspect)
                return m_samples[i]->cast<EntitiesSample>();
        }

        EntitiesSamplePtr sample = EntitiesSample::Create(getSharedPtr(), _aspect);
        m_samples.push_back(sample);

        for (Size i = 0, in = m_activeEntities.size(); i < in; ++i)
            sample->processEntity(m_activeEntities[i]);

        for (Size i = 0, in = m_addingEntities.size(); i < in; ++i)
            sample->processEntity(m_addingEntities[i]);

        return sample;
    }

    //////////////////////////////////////////
    void EcsWorld::_validateDontHave(Entity* _ptr)
    {
        {
            Deque<Entity*>::iterator it = std::find(m_addingEntities.begin(), m_addingEntities.end(), _ptr);
            if (it != m_addingEntities.end())
            {
                MAZE_ERROR("m_addingEntities");
            }
        }

        {
            Deque<Entity*>::iterator it = std::find(m_removingEntities.begin(), m_removingEntities.end(), _ptr);
            if (it != m_removingEntities.end())
            {
                MAZE_ERROR("m_removingEntities");
            }
        }

        {
            Deque<Entity*>::iterator it = std::find(m_componentsChangedEntities.begin(), m_componentsChangedEntities.end(), _ptr);
            if (it != m_componentsChangedEntities.end())
            {
                MAZE_ERROR("m_componentsChangedEntities");
            }
        }

        {
            Deque<Entity*>::iterator it = std::find(m_activeChangedEntities.begin(), m_activeChangedEntities.end(), _ptr);
            if (it != m_activeChangedEntities.end())
            {
                MAZE_ERROR("m_activeChangedEntities");
            }
        }

        {
            FastVector<Entity*>::iterator it = std::find(m_activeEntities.begin(), m_activeEntities.end(), _ptr);
            if (it != m_activeEntities.end())
            {
                MAZE_ERROR("m_activeEntities");
            }
        }

        {
            FastVector<Entity*>::iterator it = std::find(m_startedEntities.current().begin(), m_startedEntities.current().end(), _ptr);
            if (it != m_startedEntities.current().end())
            {
                MAZE_ERROR("m_activeEntities");
            }
        }
        {
            FastVector<Entity*>::iterator it = std::find(m_startedEntities.other().begin(), m_startedEntities.other().end(), _ptr);
            if (it != m_startedEntities.other().end())
            {
                MAZE_ERROR("m_activeEntities");
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
