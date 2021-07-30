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
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ECSWorld
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ECSWorld);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ECSWorld);

    //////////////////////////////////////////
    ECSWorld::ECSWorld()
        : m_entitiesIdCounter(0)
    {
    }

    //////////////////////////////////////////
    ECSWorld::~ECSWorld()
    {
        m_systems.clear();
        m_samples.clear();

        m_entitiesMap.clear();
        m_addingEntities.clear();
        m_removingEntities.clear();
        m_componentsChangedEntities.clear();
        m_activeChangedEntities.clear();
        m_activeEntities.clear();
    }

    //////////////////////////////////////////
    ECSWorldPtr ECSWorld::Create(EntityId _entitiesIdCounter)
    {
        ECSWorldPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ECSWorld, object, init(_entitiesIdCounter));
        return object;
    }

    //////////////////////////////////////////
    bool ECSWorld::init(EntityId _entitiesIdCounter)
    {
        m_entitiesIdCounter = _entitiesIdCounter;

        return true;
    }

    //////////////////////////////////////////
    void ECSWorld::update(F32 _dt)
    {
        processStartedEntities();
        processAddingEntities();
        processChangedEntities();
        processRemovingEntities();

        for (Size j = 0, jn = m_systems.size(); j < jn; ++j)
            m_systems[j]->update(_dt);

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
    EntityPtr ECSWorld::createEntity()
    {
        EntityPtr entity = Entity::Create();
        addEntity(entity);
        return entity;
    }

    //////////////////////////////////////////
    bool ECSWorld::addEntity(EntityPtr const& _entity)
    {
        MAZE_DEBUG_BP_IF(!_entity->getRemoving() && _entity->getECSWorld());
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

            _entity->setECSWorld(this);
            return true;
        }

        _entity->setAdding(true);
        m_addingEntities.push_back(_entity.get());

        _entity->setECSWorld(this);

        if (_entity->getActiveInHierarchy())
        {
            m_activeEntities.emplace_back(_entity.get());
        }

        _entity->setActiveInHierarchyPrevFrame(_entity->getActiveInHierarchy());

        m_startedEntities.current().emplace_back(_entity.get());

        return true;
    }

    //////////////////////////////////////////
    bool ECSWorld::removeEntity(EntityPtr const& _entity)
    {
        if (_entity->getRemoving())
        {
            return false;
        }

        if (_entity->getAdding())
        {
            _entity->setAdding(false);
                        
            {
                Deque<Entity*>::iterator it = std::find(m_addingEntities.begin(), m_addingEntities.end(), _entity.get());
                if (it != m_addingEntities.end())
                {
                    m_addingEntities.erase(it);
                }
            }

            if (_entity->getComponentsChanged())
            {
                Deque<Entity*>::iterator it = std::find(m_componentsChangedEntities.begin(), m_componentsChangedEntities.end(), _entity.get());
                if (it != m_componentsChangedEntities.end())
                {
                    m_componentsChangedEntities.erase(it);
                }

                _entity->setComponentsChanged(false);
            }

            if (_entity->getActiveChanged())
            {
                Deque<Entity*>::iterator it = std::find(m_activeChangedEntities.begin(), m_activeChangedEntities.end(), _entity.get());
                if (it != m_activeChangedEntities.end())
                {
                    m_activeChangedEntities.erase(it);
                }

                _entity->setActiveChanged(false);
            }

            if (_entity->getActiveInHierarchy())
            {
                auto it = m_activeEntities.find(_entity.get());
                if (it != m_activeEntities.end())
                {
                    m_activeEntities.eraseUnordered(it);
                }
            }

            if (!_entity->getTransitionFlag(Entity::TransitionFlags::Started))
            {
                {
                    auto it = m_startedEntities.current().find(_entity.get());
                    if (it != m_startedEntities.current().end())
                        m_startedEntities.current().erase(it);
                }

                {
                    auto it = m_startedEntities.other().find(_entity.get());
                    if (it != m_startedEntities.other().end())
                        m_startedEntities.other().erase(it);
                }
            }

            _entity->setECSWorld(nullptr);

            m_entitiesMap.erase(_entity->getId());

            return true;
        }
       
        _entity->setRemoving(true);
        m_removingEntities.push_back(_entity.get());

        processEntityComponentsChanged(_entity.get());

        return true;
    }

    //////////////////////////////////////////
    EntityPtr const& ECSWorld::getEntityById(EntityId _id) const
    {
        static EntityPtr nullPointer;
        
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
    Size ECSWorld::getEntitiesCount()
    {
        return m_entitiesMap.size();
    }

    //////////////////////////////////////////
    void ECSWorld::processStartedEntities()
    {
        auto& startedEntities = m_startedEntities.switchContainer();

        for (Entity* entity : startedEntities)
            entity->tryStart();

        startedEntities.clear();
    }

    //////////////////////////////////////////
    void ECSWorld::processAddingEntities()
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
    void ECSWorld::processRemovingEntities()
    {
        while (!m_removingEntities.empty())
        {
            Entity* entity = m_removingEntities.front();
            m_removingEntities.pop_front();
            
            entity->setECSScene(nullptr);
            entity->setECSWorld(nullptr);
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
                Deque<Entity*>::iterator it = std::find(m_activeChangedEntities.begin(), m_activeChangedEntities.end(), entity);
                if (it != m_activeChangedEntities.end())
                {
                    m_activeChangedEntities.erase(it);
                }

                entity->setActiveChanged(false);
            }

            if (!eventEntityRemoved.empty())
            {
                EntityPtr entityShared = entity->getSharedPtr();
                eventEntityRemoved(entityShared);            
            }

            if (entity->getActiveInHierarchy())
            {
                auto it = m_activeEntities.find(entity);
                if (it != m_activeEntities.end())
                {
                    m_activeEntities.eraseUnordered(it);
                }
                else
                {
                    MAZE_DEBUG_ERROR("activeEntities array is not synchronized");
                }
            }

            for (Size i = 0, in = m_samples.size(); i < in; ++i)
                m_samples[i]->processEntity(entity);

            m_entitiesMap.erase(entity->getId());

            processChangedEntities();
        }
    }

    //////////////////////////////////////////
    void ECSWorld::processChangedEntities()
    {
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
            entity->setActiveChanged(false);
            m_activeChangedEntities.pop_front();

            if (entity->getActiveInHierarchy() == entity->getActiveInHierarchyPrevFrame())
                continue;

            entity->setActiveInHierarchyPrevFrame(entity->getActiveInHierarchy());


            if (entity->getActiveInHierarchy())
            {
                MAZE_DEBUG_ERROR_IF(m_activeEntities.find(entity) != m_activeEntities.end(), "activeEntities array is not synchronized");
                m_activeEntities.emplace_back(entity);
            }
            else
            {
                auto it = m_activeEntities.find(entity);
                if (it != m_activeEntities.end())
                {
                    m_activeEntities.eraseUnordered(it);
                }
                else
                {
                    MAZE_DEBUG_ERROR("activeEntities array is not synchronized");
                }
            }

            for (Size i = 0, in = m_samples.size(); i < in; ++i)
                m_samples[i]->processEntity(entity);
        }
    }

    //////////////////////////////////////////
    void ECSWorld::processEntityComponentsChanged(Entity* _entity)
    {
        if (_entity->getComponentsChanged())
            return;

        _entity->setComponentsChanged(true);
        m_componentsChangedEntities.push_back(_entity);

        eventEntityChanged(_entity);
    }

    //////////////////////////////////////////
    void ECSWorld::processEntityActiveChanged(Entity* _entity)
    {
        _entity->setActiveChanged(true);
        m_activeChangedEntities.push_back(_entity);
    }

    //////////////////////////////////////////
    void ECSWorld::addSystem(ComponentSystemPtr const& _system)
    {
#if (MAZE_DEBUG)
        for (Size i = 0, in = m_systems.size(); i < in; ++i)
        {
            if (m_systems[i]->getClassUID() == _system->getClassUID())
            {
                MAZE_ERROR_RETURN("System is already exist!");
            }
        }
#endif

        m_systems.push_back(_system);
        std::sort(
            m_systems.begin(),
            m_systems.end(),
            [](ComponentSystemPtr const& system0, ComponentSystemPtr const& system1) -> bool
            {
                return system0->getOrder() < system1->getOrder();
            });

        _system->setWorld(getSharedPtr());
        _system->processSystemAdded();

        eventComponentSystemAdded(_system);
    }

    //////////////////////////////////////////
    void ECSWorld::removeSystem(ComponentSystemPtr const& _system)
    {
        for (Size i = 0, in = m_systems.size(); i < in; ++i)
        {
            if (m_systems[i] == _system)
            {
                ComponentSystemPtr system = _system;
                m_systems.erase(m_systems.begin() + i);
                system->processSystemRemoved();
                system->setWorld(getSharedPtr());

                return;
            }
        }
    }

    //////////////////////////////////////////
    EntitiesSamplePtr ECSWorld::requestCommonSample(EntityAspect const& _aspect)
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


} // namespace Maze
//////////////////////////////////////////
