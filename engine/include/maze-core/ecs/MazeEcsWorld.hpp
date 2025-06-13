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
#include "maze-core/ecs/events/MazeEcsInputEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Event);
    MAZE_USING_SHARED_PTR(ComponentEntityLinker);
    MAZE_USING_SHARED_PTR(IEntitiesSample);
    MAZE_USING_SHARED_PTR(EcsWorldEventsQueue);


    //////////////////////////////////////////
    // Enum EcsWorldState
    //
    //////////////////////////////////////////
    enum class MAZE_CORE_API EcsWorldState
    {
        None = 0,
        Active,
        PreparingToDestroy,
        Destroying,
        Destroyed
    };


    //////////////////////////////////////////
    MAZE_CORE_API void SendEvent(EcsWorld* _ecsWorld, EntityId _id, EventPtr const& _event);

    //////////////////////////////////////////
    MAZE_CORE_API void SendEventImmediate(EcsWorld* _ecsWorld, EntityId _id, Event* _event);


    //////////////////////////////////////////
    // Class EcsWorld
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsWorld
        : public SharedObject<EcsWorld>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EcsWorld);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EcsWorld);

        //////////////////////////////////////////
        friend class Entity;
        friend class EcsWorldEventsQueue;

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
            bool _attachSystems = true,
            Set<HashedString> const& _tags = Set<HashedString>());

        //////////////////////////////////////////
        static EcsWorld* GetDefaultWorldRaw();

        //////////////////////////////////////////
        void reserveEntityIndices(Size _count);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline EcsWorldId getId() const { return m_id; }


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
        void processEntityAddedToSample(
            ComponentSystemEventHandlerPtr const& _handler,
            EntityId _id);

        //////////////////////////////////////////
        void processEntityRemovedFromSample(
            ComponentSystemEventHandlerPtr const& _handler,
            EntityId _id);


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline Set<HashedString> const& getTags() const { return m_tags; }


        //////////////////////////////////////////
        inline S32 getFrameNumber() const { return m_frameNumber; }


        //////////////////////////////////////////
        Size calculateEntitiesCount();


        //////////////////////////////////////////
        void processEntityComponentsChanged(EntityId _id);

        //////////////////////////////////////////
        void processEntityActiveChanged(EntityId _id);


        //////////////////////////////////////////
        bool addSystemEventHandler(ComponentSystemEventHandlerPtr const& _system);

        //////////////////////////////////////////
        void removeSystemEventHandler(ComponentSystemEventHandlerPtr const& _system);


        //////////////////////////////////////////
        EntitiesSamplePtr requestCommonSample(
            EntityAspect const& _aspect,
            U8 _flags = 0);

        //////////////////////////////////////////
        template<typename ...TComponents>
        inline SharedPtr<GenericInclusiveEntitiesSample<TComponents...>> requestInclusiveSample(
            U8 _flags = 0)
        {
            for (Size i = 0, in = m_samples.size(); i < in; ++i)
            {
                if (m_samples[i]->getType() != EntitiesSampleType::GenericInclusive)
                    continue;

                if (m_samples[i]->getFlags() != _flags)
                    continue;

                if (m_samples[i]->getAspect().isAspect<TComponents...>(EntityAspectType::HaveAllOfComponents))
                    return m_samples[i]->cast<GenericInclusiveEntitiesSample<TComponents...>>();
            }

            SharedPtr<GenericInclusiveEntitiesSample<TComponents...>> sample = GenericInclusiveEntitiesSample<TComponents...>::Create(
                getSharedPtr(),
                _flags);
            m_samples.push_back(sample);

            for (Size i = 0, in = m_entities.size(); i < in; ++i)
                if (m_entities[i].entity)
                    sample->processEntity(m_entities[i].entity.get());
            
            return sample;
        }

        //////////////////////////////////////////
        template<typename TComponent>
        inline SharedPtr<DynamicIdEntitiesSample<TComponent>> requestDynamicIdSample(
            ComponentId _dynamicId,
            U8 _flags = 0)
        {
            for (Size i = 0, in = m_samples.size(); i < in; ++i)
            {
                if (m_samples[i]->getType() != EntitiesSampleType::DynamicId)
                    continue;

                if (m_samples[i]->getFlags() != _flags)
                    continue;

                if (m_samples[i]->getAspect().getComponentIds().size() == 1 && m_samples[i]->getAspect().getComponentIds()[0] == _dynamicId)
                    return m_samples[i]->cast<DynamicIdEntitiesSample<TComponent>>();
            }

            SharedPtr<DynamicIdEntitiesSample<TComponent>> sample = DynamicIdEntitiesSample<TComponent>::Create(
                getSharedPtr(), _dynamicId, _flags);
            m_samples.push_back(sample);

            for (Size i = 0, in = m_entities.size(); i < in; ++i)
                if (m_entities[i].entity)
                    sample->processEntity(m_entities[i].entity.get());

            return sample;
        }

        //////////////////////////////////////////
        template<typename TEventType, typename ...TComponents>
        inline ComponentSystemEventHandlerPtr addSystemEventHandler(
            HashedCString _name,
            void (*_func)(TEventType&, Entity*, TComponents* ...),
            Set<HashedString> const& _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder(),
            U8 _sampleFlags = 0)
        {
            ComponentSystemEventHandlerPtr system = ComponentSystemEventHandler::Create(
                this,
                _name,
                ClassInfo<typename std::remove_const<TEventType>::type>::UID(),
                requestInclusiveSample<TComponents...>(_sampleFlags),
                (ComponentSystemEventHandler::Func)_func,
                _tags,
                _order);
            addSystemEventHandler(system);
            return system;
        }

        //////////////////////////////////////////
        template<typename TEventType, typename ...TComponents>
        inline ComponentSystemEventHandlerPtr addSystemEventHandlerGlobal(
            HashedCString _name,
            void(*_func)(TEventType&),
            Set<HashedString> const& _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
        {
            ComponentSystemEventHandlerPtr system = ComponentSystemEventHandler::Create(
                this,
                _name,
                ClassInfo<typename std::remove_const<TEventType>::type>::UID(),
                (ComponentSystemEventHandler::GlobalFunc)_func,
                _tags,
                _order);
            addSystemEventHandler(system);
            return system;
        }

        //////////////////////////////////////////
        template <typename TEventType>
        inline Vector<ComponentSystemEventHandlerPtr> const& getSystems()
        {
            static Vector<ComponentSystemEventHandlerPtr> nullValue;

            ClassUID eventUID = ClassInfo<typename std::remove_const<TEventType>::type>::UID();
            auto it = m_eventHandlers.find(eventUID);
            if (it == m_eventHandlers.end())
                return nullValue;
            return it->second;
        }


        //////////////////////////////////////////
        template <typename TEvent>
        inline void broadcastEventImmediate(TEvent* _event, EcsEventParams _params = EcsEventParams())
        {
            ClassUID eventUID = _event->getEventUID();
            Vector<ComponentSystemEventHandlerPtr> const& eventHandlers = m_eventHandlers[eventUID];

            for (ComponentSystemEventHandlerPtr const& _eventHandler : eventHandlers)
                _eventHandler->processEvent(_event, _params);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void broadcastEventImmediate(EcsEventParams _params, TArgs... _args)
        {
            TEvent evt(_args...);
            MAZE_DEBUG_ERROR_BP_IF(
                evt.getClassUID() != ClassInfo<TEvent>::UID(),
                "Event %s has wrong metadata!",
                ClassInfo<TEvent>::Name());
            broadcastEventImmediate(&evt, _params);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void broadcastEventImmediate(TArgs... _args)
        {
            broadcastEventImmediate<TEvent, TArgs...>(EcsEventParams(), _args...);
        }


        //////////////////////////////////////////
        void broadcastEvent(EventPtr const& _event);

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void broadcastEvent(TArgs... _args)
        {
            // #TODO: Rework to unique?
            SharedPtr<TEvent> evt = MakeShared<TEvent>(_args...);
            MAZE_DEBUG_ERROR_BP_IF(
                evt->getClassUID() != ClassInfo<TEvent>::UID(),
                "Event %s has wrong metadata!",
                ClassInfo<TEvent>::Name());
            broadcastEvent(evt);
        }



        //////////////////////////////////////////
        template <typename TEvent>
        inline void sendEventImmediate(EntityId _entityId, TEvent* _event, EcsEventParams _params = EcsEventParams())
        {
            ClassUID eventUID = _event->getEventUID();
            Vector<ComponentSystemEventHandlerPtr> const& eventHandlers = m_eventHandlers[eventUID];

            for (ComponentSystemEventHandlerPtr const& _eventHandler : eventHandlers)
                _eventHandler->processEvent(_entityId, _event, _params);
        }
        
        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void sendEventImmediate(EntityId _entityId, EcsEventParams _params, TArgs... _args)
        {
            TEvent evt(_args...);
            sendEventImmediate(_entityId, &evt, _params);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void sendEventImmediate(EntityId _entityId, TArgs... _args)
        {
            TEvent evt(_args...);
            sendEventImmediate(_entityId, &evt);
        }


        //////////////////////////////////////////
        void sendEvent(EntityId _entityId, EventPtr const& _event);

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void sendEvent(EntityId _entityId, TArgs... _args)
        {
            // #TODO: Rework to unique?
            SharedPtr<TEvent> evt = MakeShared<TEvent>(_args...);
            sendEvent(_entityId, evt);
        }


        //////////////////////////////////////////
        void processEntityForSamples(Entity* _entity);

    public:

        //////////////////////////////////////////
        static EcsWorld* GetEcsWorld(EcsWorldId _id);

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
            bool _attachSystems,
            Set<HashedString> const& _tags);

                
        //////////////////////////////////////////
        EntityId generateNewEntityId(EntityPtr const& _entity);

        //////////////////////////////////////////
        S32 convertEntityIdToIndex(EntityId _id) const;

        //////////////////////////////////////////
        S32 convertIndexToEntityIdIndex(S32 _index) const;


        //////////////////////////////////////////
        void addEntityNow(EntityPtr const& _entity);

        //////////////////////////////////////////
        void removeEntityNow(EntityId _id);


        //////////////////////////////////////////
        void notifyMouse(InputEventMouseData const& _data);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

        //////////////////////////////////////////
        void notifyTouch(InputEventTouchData const& _data);

        //////////////////////////////////////////
        void notifyVirtualCursor(InputEventVirtualCursorData const& _data);

    protected:
        HashedString m_name;
        Set<HashedString> m_tags;

        FastVector<EntityData> m_entities;
        Stack<S32> m_freeEntityIndices;

        Vector<IEntitiesSamplePtr> m_samples;

        UnorderedMap<ClassUID, Vector<ComponentSystemEventHandlerPtr>> m_eventHandlers;
        FastVector<ComponentSystemEntityAddedToSampleEventHandlerPtr> m_entityAddedToSampleEventHandlers;
        FastVector<ComponentSystemEntityRemovedFromSampleEventHandlerPtr> m_entityRemovedFromSampleEventHandlers;

    private:

        //////////////////////////////////////////
        static EcsWorldId GenerateNewEcsWorldId(EcsWorld* _world);

        //////////////////////////////////////////
        static void ReleaseEcsWorldId(EcsWorldId _id);

    private:
        static FastVector<EcsWorld*> s_worlds;
        static Stack<EcsWorldId> s_freeEcsWorldIndices;

    private:
        EcsWorldId m_id;
        bool m_updatingNow = false;
        EcsWorldState m_state = EcsWorldState::None;
        S32 m_newEntityIdsCount = 0;
        SwitchableContainer<EcsWorldEventsQueuePtr> m_eventHolders;

        S32 m_frameNumber = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsWorld_hpp_
//////////////////////////////////////////
