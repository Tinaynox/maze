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
#pragma once
#if (!defined(_MazeEntitiesSample_hpp_))
#define _MazeEntitiesSample_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/events/MazeEcsEvents.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/MazeEntityAspect.hpp"
#include "maze-core/utils/MazeIndexTupleBuilder.hpp"
#include <functional>
#include <tuple>
#include <utility>
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(IEntitiesSample);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(EcsWorld);


    //////////////////////////////////////////
    // Enum class EntitiesSampleType
    //
    //////////////////////////////////////////
    enum class EntitiesSampleType
    {
        Common,
        GenericInclusive,
    };


    //////////////////////////////////////////
    // Class IEntitiesSample
    //
    //////////////////////////////////////////
    class MAZE_CORE_API IEntitiesSample
        : public SharedObject<IEntitiesSample>
    {
    public:

        //////////////////////////////////////////
        virtual ~IEntitiesSample();

        //////////////////////////////////////////
        virtual EntitiesSampleType getType() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void clear() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processEntity(Entity* _entity) MAZE_ABSTRACT;


        //////////////////////////////////////////
        EntityAspect const& getAspect() const { return m_aspect; }


        //////////////////////////////////////////
        virtual void process(void (*)()) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processEvent(Event* _event, EcsEventParams _params, void (*)()) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processEvent(EntityId _entityId, Event* _event, EcsEventParams _params, void (*)()) MAZE_ABSTRACT;

    public:

        //////////////////////////////////////////
        MultiDelegate<Entity*> eventEntityAdded;
        MultiDelegate<Entity*> eventEntityWillBeRemoved;
        MultiDelegate<Entity*> eventEntityRemoved;

    protected:

        //////////////////////////////////////////
        IEntitiesSample();

        //////////////////////////////////////////
        virtual bool init(
            EcsWorldPtr const& _ecs,
            EntityAspect const& _aspect);

    protected:
        EcsWorldWPtr m_world;
        EcsWorld* m_worldRaw;
        EntityAspect m_aspect;
    };


    //////////////////////////////////////////
    // Class EntitiesSample
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntitiesSample
        : public IEntitiesSample
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EntitiesSample)

    public:

        //////////////////////////////////////////
        virtual ~EntitiesSample();

        //////////////////////////////////////////
        static EntitiesSamplePtr Create(
            EcsWorldPtr const& _ecs,
            EntityAspect const& _aspect)
        {
            EntitiesSamplePtr object;
            MAZE_CREATE_AND_INIT_SHARED_PTR(EntitiesSample, object, init(_ecs, _aspect));
            return object;
        }

        //////////////////////////////////////////
        virtual EntitiesSampleType getType() MAZE_OVERRIDE { return EntitiesSampleType::Common; }

        //////////////////////////////////////////
        Vector<Entity*> const& getEntities() const { return m_entities; }

        //////////////////////////////////////////
        Vector<EntityId> getEntityIds() const;

        //////////////////////////////////////////
        virtual void clear() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntity(Entity* _entity) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void process(void (*_func)()) MAZE_OVERRIDE
        { }


        //////////////////////////////////////////
        virtual void processEvent(Event* _event, EcsEventParams _params, void (*_func)()) MAZE_OVERRIDE
        { }

        //////////////////////////////////////////
        virtual void processEvent(EntityId _entityId, Event* _event, EcsEventParams _params, void (*_func)()) MAZE_OVERRIDE
        { }


    protected:

        //////////////////////////////////////////
        EntitiesSample();

        //////////////////////////////////////////
        virtual bool init(
            EcsWorldPtr const& _ecs,
            EntityAspect const& _aspect) MAZE_OVERRIDE;

    protected:
        Vector<Entity*> m_entities;
    };


    //////////////////////////////////////////
    // Class GenericInclusiveEntitiesSample
    //
    //////////////////////////////////////////
    template<typename ...TComponents>
    class GenericInclusiveEntitiesSample
        : public IEntitiesSample
    {
    protected:

        //////////////////////////////////////////
        using Indices = IndexTupleBuilder<sizeof ...(TComponents)>;

        //////////////////////////////////////////
        using ProcessFunc = std::function<void(Entity*, TComponents* ..._components)>;

        //////////////////////////////////////////
        using ProcessFuncRaw = void(*)(Entity*, TComponents* ..._components);


        //////////////////////////////////////////
        using ProcessEventFunc = std::function<void(Event&, Entity*, TComponents* ..._components)>;

        //////////////////////////////////////////
        using ProcessEventFuncRaw = void(*)(Event&, Entity*, TComponents* ..._components);

        //////////////////////////////////////////
        struct EntityData
        {
            Entity* entity = nullptr;
            std::tuple<TComponents*...> components;
        };

    public:

        //////////////////////////////////////////
        virtual ~GenericInclusiveEntitiesSample()
        {

        }

        //////////////////////////////////////////
        static inline SharedPtr<GenericInclusiveEntitiesSample> Create(
            EcsWorldPtr const& _ecs)
        {
            SharedPtr<GenericInclusiveEntitiesSample> object;
            EntityAspect aspect = EntityAspect::HaveAllOfComponents<TComponents...>();

            object = MAZE_CREATE_SHARED_PTR(GenericInclusiveEntitiesSample);
            if (!object->init(_ecs, aspect))
                object.reset();
            return object;
        }

        //////////////////////////////////////////
        virtual EntitiesSampleType getType() MAZE_OVERRIDE { return EntitiesSampleType::GenericInclusive; }

        //////////////////////////////////////////
        FastVector<EntityData> const& getEntitiesData() const { return m_entitiesData; }

        //////////////////////////////////////////
        Vector<EntityId> getEntityIds() const
        {
            Vector<EntityId> result;
            for (EntityData entityData : m_entitiesData)
                result.push_back(entityData.entity->getId());

            return result;
        }

        //////////////////////////////////////////
        virtual void clear() MAZE_OVERRIDE
        {
            for (EntityData const& entityData : m_entitiesData)
            {
                eventEntityWillBeRemoved(entityData.entity);
                invokeEntityRemoved(entityData, typename Indices::Indexes());
            }

            m_entitiesData.clear();
            m_entityIndices.clear();
        }

        //////////////////////////////////////////
        virtual void processEntity(Entity* _entity) MAZE_OVERRIDE
        {
            EntityId eid = _entity->getId();
            typename UnorderedMap<EntityId, Size>::iterator it = m_entityIndices.find(_entity->getId());

            bool intersects;
            if (!_entity->getActiveInHierarchy() || !_entity->getEcsWorld())
                intersects = false;
            else
                intersects = m_aspect.hasIntersection(_entity);
            
            if (it == m_entityIndices.end() && intersects)
            {
                EntityData entityData;
                entityData.entity = _entity;
                extractComponents(_entity, entityData.components);

                Size index = m_entitiesData.size();
                m_entitiesData.push_back(entityData);
                m_entityIndices.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(eid),
                    std::forward_as_tuple(index));
                invokeEntityAdded(entityData, typename Indices::Indexes());
            }
            else
            if (it != m_entityIndices.end() && !intersects)
            {
                eventEntityWillBeRemoved(_entity);

                Size index = it->second;
                EntityData entityData = m_entitiesData[index];
                
                m_entityIndices.erase(it);
                m_entitiesData.eraseUnordered(m_entitiesData.begin() + index);
                if (index < m_entitiesData.size())
                    m_entityIndices[m_entitiesData[index].entity->getId()] = index;

                invokeEntityRemoved(entityData, typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        void process(ProcessFunc _func)
        {
            for (EntityData entityData : m_entitiesData)
            {
                callProcess(
                    _func,
                    entityData.entity,
                    entityData.components,
                    typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        virtual void process(void (*_func)()) MAZE_OVERRIDE
        {
            ProcessFuncRaw rawFunc = (ProcessFuncRaw)_func;
            process((ProcessFunc)(rawFunc));
        }


        //////////////////////////////////////////
        inline void processEvent(Event* _event, EcsEventParams _params, ProcessEventFunc _func)
        {
            for (EntityData entityData : m_entitiesData)
            {
                if (entityData.entity->getRemoving() && _params.ignoreRemovingEntity)
                    continue;

                if (!entityData.entity->getEcsWorld() && _params.ignoreNullWorldEntity)
                    continue;

                callProcessEvent(
                    _event,
                    _func,
                    entityData.entity,
                    entityData.components,
                    typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        virtual void processEvent(Event* _event, EcsEventParams _params, void (*_func)()) MAZE_OVERRIDE
        {
            ProcessEventFuncRaw rawFunc = (ProcessEventFuncRaw)_func;
            processEvent(_event, _params, (ProcessEventFunc)(rawFunc));
        }

        //////////////////////////////////////////
        inline void processEvent(EntityId _entityId, Event* _event, EcsEventParams _params, ProcessEventFunc _func)
        {
            UnorderedMap<EntityId, Size>::iterator it = m_entityIndices.find(_entityId);

            if (it != m_entityIndices.end())
            {
                EntityData& entityData = m_entitiesData[it->second];

                if (entityData.entity->getRemoving() && _params.ignoreRemovingEntity)
                    return;

                if (!entityData.entity->getEcsWorld() && _params.ignoreNullWorldEntity)
                    return;

                callProcessEvent(
                    _event,
                    _func,
                    entityData.entity,
                    entityData.components,
                    typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        virtual void processEvent(EntityId _entityId, Event* _event, EcsEventParams _params, void (*_func)()) MAZE_OVERRIDE
        {
            ProcessEventFuncRaw rawFunc = (ProcessEventFuncRaw)_func;
            processEvent(_entityId, _event, _params,(ProcessEventFunc)(rawFunc));
        }

    protected:

        template<std::size_t TIndex = 0>
        inline
        typename std::enable_if<TIndex == sizeof...(TComponents), void>::type
        extractComponents(Entity* entity, std::tuple<TComponents*...>& _components)
        {}

        template<std::size_t TIndex = 0>
        inline
        typename std::enable_if<TIndex < sizeof...(TComponents), void>::type
        extractComponents(Entity* entity, std::tuple<TComponents*...>& _components)
        {
            using TComponentType = typename std::tuple_element<TIndex, std::tuple<TComponents...> >::type;
            std::get<TIndex>(_components) = entity->getComponentRaw<TComponentType>();

            this->extractComponents<TIndex + 1>(entity, _components);
        }

        template<S32 ...Idxs> 
        inline void callProcess(
            ProcessFunc _func,
            Entity* _entity,
            std::tuple<TComponents*...>& _components,
            IndexesTuple<Idxs...> const&)
        {
            _func(_entity, std::get<Idxs>(_components)...);
        }

        template<S32 ...Idxs>
        inline void callProcessEvent(
            Event* _event,
            ProcessEventFunc _func,
            Entity* _entity,
            std::tuple<TComponents*...>& _components,
            IndexesTuple<Idxs...> const&)
        {
            _func(*_event, _entity, std::get<Idxs>(_components)...);
        }


    public:
        MultiDelegate<Entity*, TComponents* ...> eventEntityWithComponentsAdded;
        MultiDelegate<Entity*, TComponents* ...> eventEntityWithComponentsRemoved;

    protected:

        //////////////////////////////////////////
        GenericInclusiveEntitiesSample()
        {

        }

        //////////////////////////////////////////
        virtual bool init(
            EcsWorldPtr const& _ecs,
            EntityAspect const& _aspect) MAZE_OVERRIDE
        {
            if (!IEntitiesSample::init(_ecs, _aspect))
                return false;

            return true;
        }

        //////////////////////////////////////////
        template<S32 ...Idxs> 
        inline void invokeEntityAdded(
            EntityData const& _entityData,
            IndexesTuple<Idxs...> const&)
        {
            eventEntityAdded(_entityData.entity);
            eventEntityWithComponentsAdded(_entityData.entity, std::get<Idxs>(_entityData.components)...);
        }

        //////////////////////////////////////////
        template<S32 ...Idxs> 
        inline void invokeEntityRemoved(
            EntityData const& _entityData,
            IndexesTuple<Idxs...> const&)
        {
            eventEntityRemoved(_entityData.entity);
            eventEntityWithComponentsRemoved(_entityData.entity, std::get<Idxs>(_entityData.components)...);
        }

    protected:
        FastVector<EntityData> m_entitiesData;
        UnorderedMap<EntityId, Size> m_entityIndices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntitiesSample_hpp_
//////////////////////////////////////////
