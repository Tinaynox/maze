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
#if (!defined(_MazeEcsWorld_hpp_))
#define _MazeEcsWorld_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEntityAspect.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/utils/MazeSwitchableContainer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(ComponentEntityLinker);
    MAZE_USING_SHARED_PTR(IEntitiesSample);


    //////////////////////////////////////////
    // Class EcsWorld
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsWorld
        : public SharedObject<EcsWorld>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EcsWorld);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EcsWorld);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        struct EntityData
        {
            //////////////////////////////////////////
            EntityData() {}

            //////////////////////////////////////////
            EntityData(
                EntityId _id,
                EntityPtr const& _entity)
                : id(_id)
                , entity(_entity)
            {}

            //////////////////////////////////////////
            EntityData(
                EntityId _id,
                EntityPtr&& _entity)
                : id(_id)
                , entity(std::move(_entity))
            {}

            EntityId id;
            EntityPtr entity;
        };

    public:

        //////////////////////////////////////////
        virtual ~EcsWorld();

        //////////////////////////////////////////
        static EcsWorldPtr Create(
            HashedString const& _name,
            bool _attachSystems = true);

        //////////////////////////////////////////
        static EcsWorld* GetDefaultWorldRaw();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        EntityPtr const& getEntity(EntityId _id) const;

        //////////////////////////////////////////
        inline bool isEntityExists(EntityId _id) const { return getEntity(_id) != nullptr; }

        //////////////////////////////////////////
        EntityPtr createEntity();

        //////////////////////////////////////////
        bool addEntity(EntityPtr const& _entity);

        //////////////////////////////////////////
        bool removeEntity(EntityPtr const& _entity);


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }



        //////////////////////////////////////////
        Size calculateEntitiesCount();


        //////////////////////////////////////////
        void processEntityComponentsChanged(EntityId _id);


        //////////////////////////////////////////
        void addSystemEventHandler(SimpleComponentSystemEventHandlerPtr const& _system);

        //////////////////////////////////////////
        void removeSystemEventHandler(SimpleComponentSystemEventHandlerPtr const& _system);


        //////////////////////////////////////////
        EntitiesSamplePtr requestCommonSample(EntityAspect const& _aspect);

        //////////////////////////////////////////
        template<typename ...TComponents>
        inline SharedPtr<GenericInclusiveEntitiesSample<TComponents...>> requestInclusiveSample()
        {
            for (Size i = 0, in = m_samples.size(); i < in; ++i)
            {
                if (m_samples[i]->getType() != EntitiesSampleType::GenericInclusive)
                    continue;

                if (m_samples[i]->getAspect().isAspect<TComponents...>(EntityAspectType::HaveAllOfComponents))
                    return m_samples[i]->cast<GenericInclusiveEntitiesSample<TComponents...>>();
            }

            SharedPtr<GenericInclusiveEntitiesSample<TComponents...>> sample = GenericInclusiveEntitiesSample<TComponents...>::Create(getSharedPtr());
            m_samples.push_back(sample);

            for (Size i = 0, in = m_entities.size(); i < in; ++i)
                if (m_entities[i].entity)
                    sample->processEntity(m_entities[i].entity.get());

            for (Size i = 0, in = m_addingEntities.size(); i < in; ++i)
                sample->processEntity(m_addingEntities[i].get());

            return sample;
        }

        //////////////////////////////////////////
        template<typename TEventType, typename ...TComponents>
        inline SimpleComponentSystemEventHandlerPtr addSystemEventHandler(
            HashedCString _name,
            void (*_func)(TEventType&, Entity*, TComponents* ...),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
        {
            SimpleComponentSystemEventHandlerPtr system = SimpleComponentSystemEventHandler::Create(
                _name,
                ClassInfo<typename std::remove_const<TEventType>::type>::UID(),
                requestInclusiveSample<TComponents...>(),
                (SimpleComponentSystemEventHandler::Func)_func,
                _order);
            addSystemEventHandler(system);
            return system;
        }

        //////////////////////////////////////////
        template <typename TEvent>
        inline void broadcastEventImmediate(TEvent* _event)
        {
            ClassUID eventUID = _event->getClassUID();
            Vector<SimpleComponentSystemEventHandlerPtr> const& eventHandlers = m_eventHandlers[eventUID];

            for (SimpleComponentSystemEventHandlerPtr const& _eventHandler : eventHandlers)
                _eventHandler->processEvent(_event);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void broadcastEventImmediate(TArgs... _args)
        {
            TEvent evt(_args...);
            MAZE_DEBUG_ERROR_BP_IF(
                evt.getClassUID() != ClassInfo<TEvent>::UID(),
                "Event %s has wrong metadata!",
                ClassInfo<TEvent>::Name());
            broadcastEventImmediate(&evt);
        }

        //////////////////////////////////////////
        template <typename TEventType>
        inline Vector<SimpleComponentSystemEventHandlerPtr> const& getSystems()
        {
            static Vector<SimpleComponentSystemEventHandlerPtr> nullValue;

            ClassUID eventUID = ClassInfo<typename std::remove_const<TEventType>::type>::UID();
            auto it = m_eventHandlers.find(eventUID);
            if (it == m_eventHandlers.end())
                return nullValue;
            return it->second;
        }

        //////////////////////////////////////////
        template <typename TEvent>
        inline void sendEventImmediate(EntityId _entityId, TEvent* _event)
        {
            ClassUID eventUID = _event->getClassUID();
            Vector<SimpleComponentSystemEventHandlerPtr> const& eventHandlers = m_eventHandlers[eventUID];

            for (SimpleComponentSystemEventHandlerPtr const& _eventHandler : eventHandlers)
                _eventHandler->processEvent(_entityId, _event);
        }
        
        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void sendEventImmediate(EntityId _entityId, TArgs... _args)
        {
            TEvent evt(_args...);
            sendEventImmediate(_entityId, &evt);
        }

    public:

        //////////////////////////////////////////
        MultiDelegate<EcsWorld*> eventOnDestroy;
        MultiDelegate<EntityPtr const&> eventEntityAdded;
        MultiDelegate<EntityPtr const&> eventEntityChanged;
        MultiDelegate<EntityPtr const&> eventEntityRemoved;


    protected:

        //////////////////////////////////////////
        EcsWorld();

        //////////////////////////////////////////
        bool init(
            HashedString const& _name,
            bool _attachSystems);

        //////////////////////////////////////////
        void processStartedEntities();

        //////////////////////////////////////////
        void processAddingEntities();

        //////////////////////////////////////////
        void processRemovingEntities();

        //////////////////////////////////////////
        void processChangedEntities();

        
        //////////////////////////////////////////
        void processEntityActiveChanged(EntityId _id);

        
        //////////////////////////////////////////
        EntityId generateNewEntityId();

        //////////////////////////////////////////
        S32 convertEntityIdToIndex(EntityId _id) const;

        //////////////////////////////////////////
        void removeEntityNow(EntityId _id);

    protected:
        HashedString m_name;

        FastVector<EntityData> m_entities;
        Stack<S32> m_freeEntityIndices;

        
        Deque<EntityId> m_componentsChangedEntities;
        Deque<EntityId> m_activeChangedEntities;

        Vector<IEntitiesSamplePtr> m_samples;

        UnorderedMap<ClassUID, Vector<SimpleComponentSystemEventHandlerPtr>> m_eventHandlers;

    private:
        bool m_updatingNow = false;

        // Adding entities utils
        S32 m_newEntityIdsCount = 0;
        Deque<EntityPtr> m_addingEntities;
        FastVector<EntityId> m_newEntitiesAdded;

        // Removing entities utils
        Deque<EntityId> m_removingEntities;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsWorld_hpp_
//////////////////////////////////////////
