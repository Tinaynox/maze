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
#if (!defined(_MazeComponentSystem_hpp_))
#define _MazeComponentSystem_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeHashedCString.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentSystemEventHandler);
    MAZE_USING_SHARED_PTR(ComponentSystemEntityAddedToSampleEventHandler);
    MAZE_USING_SHARED_PTR(ComponentSystemEntityRemovedFromSampleEventHandler);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(IEntitiesSample);


    //////////////////////////////////////////
    // Class ComponentSystemOrder
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API ComponentSystemOrder
    {
        //////////////////////////////////////////
        ComponentSystemOrder(
            Set<HashedString> const& _after = Set<HashedString>(),
            Set<HashedString> const& _before = Set<HashedString>())
            : after(_after)
            , before(_before)
        {}

        //////////////////////////////////////////
        ComponentSystemOrder(
            Set<HashedString>&& _after,
            Set<HashedString>&& _before)
            : after(std::move(_after))
            , before(std::move(_before))
        {}

        Set<HashedString> after;
        Set<HashedString> before;
    };


    //////////////////////////////////////////
    #define MAZE_ECS_ORDER_AFTER(...) Set<HashedString>{__VA_ARGS__}
    #define MAZE_ECS_ORDER_BEFORE(...) Set<HashedString>{__VA_ARGS__}
    #define MAZE_ECS_ORDER(DAfter, DBefore) ComponentSystemOrder(DAfter, DBefore)

    //////////////////////////////////////////
    #define MAZE_ECS_TAGS(...) Set<HashedString>{__VA_ARGS__}


    //////////////////////////////////////////
    // Class ComponentSystemEventHandler
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ComponentSystemEventHandler
    {
    public:

        //////////////////////////////////////////
        using Func = void (*)();
        using GlobalFunc = void(*)(Event&);

        //////////////////////////////////////////
        static inline ComponentSystemEventHandlerPtr Create(
            EcsWorld* _world,
            HashedCString _name,
            ClassUID _eventUID,
            IEntitiesSamplePtr _sample,
            Func _func,
            Set<HashedString> const& _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
        {
            MAZE_ASSERT(_sample);
            return MAZE_CREATE_SHARED_PTR_WITH_ARGS(
                ComponentSystemEventHandler, _world, _name, _eventUID, _sample, _func, _tags, _order);
        }

        //////////////////////////////////////////
        static inline ComponentSystemEventHandlerPtr Create(
            EcsWorld* _world,
            HashedCString _name,
            ClassUID _eventUID,
            GlobalFunc _func,
            Set<HashedString> const& _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
        {
            return MAZE_CREATE_SHARED_PTR_WITH_ARGS(
                ComponentSystemEventHandler, _world, _name, _eventUID, nullptr, (Func)_func, _tags, _order);
        }

        //////////////////////////////////////////
        inline void processEvent(Event* _event, EcsEventParams _params = EcsEventParams())
        {
            if (m_sample)
            {
                m_sample->processEvent(_event, _params, m_func);
            }
            else
            {
                GlobalFunc rawFunc = (GlobalFunc)m_func;
                rawFunc(*_event);
            }
        }

        //////////////////////////////////////////
        inline void processEvent(EntityId _entityId, Event* _event, EcsEventParams _params = EcsEventParams())
        {
            if (m_sample)
            {
                m_sample->processEvent(_entityId, _event, _params, m_func);
            }
        }


        //////////////////////////////////////////
        inline EcsWorld* getWorld() const { return m_world; }

        //////////////////////////////////////////
        inline IEntitiesSamplePtr const& getSample() const { return m_sample; }


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline ClassUID getEventUID() const { return m_eventUID; }


        //////////////////////////////////////////
        inline Set<HashedString> const& getTags() const { return m_tags; }

        //////////////////////////////////////////
        inline void setTags(Set<HashedString> const& _tags) { m_tags = _tags; }


        //////////////////////////////////////////
        inline ComponentSystemOrder const& getOrder() const { return m_order; }

        //////////////////////////////////////////
        inline void setOrder(ComponentSystemOrder const& _order) { m_order = _order; }

        
    protected:

        //////////////////////////////////////////
        ComponentSystemEventHandler(
            EcsWorld* _world,
            HashedCString _name,
            ClassUID _eventUID,
            IEntitiesSamplePtr _sample = nullptr,
            Func _func = nullptr,
            Set<HashedString> const& _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
            : m_world(_world)
            , m_name(_name)
            , m_eventUID(_eventUID)
            , m_sample(_sample)
            , m_func(_func)
            , m_tags(_tags)
            , m_order(_order)
        {
        }

    protected:
        EcsWorld* m_world = nullptr;
        HashedString m_name;
        ClassUID m_eventUID = 0;
        IEntitiesSamplePtr m_sample;
        Func m_func = nullptr;

        Set<HashedString> m_tags;
        ComponentSystemOrder m_order;
    };


    //////////////////////////////////////////
    // Class ComponentSystemEntityAddedToSampleEventHandler
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ComponentSystemEntityAddedToSampleEventHandler
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        static inline ComponentSystemEntityAddedToSampleEventHandlerPtr Create(
            ComponentSystemEventHandlerPtr const& _eventHandler)
        {
            ComponentSystemEntityAddedToSampleEventHandlerPtr object;
            MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentSystemEntityAddedToSampleEventHandler, object, init(_eventHandler));
            return object;
        }

        //////////////////////////////////////////
        ~ComponentSystemEntityAddedToSampleEventHandler();


        //////////////////////////////////////////
        void processEntitiesAddedToSample();

    protected:

        //////////////////////////////////////////
        ComponentSystemEntityAddedToSampleEventHandler();

        //////////////////////////////////////////
        bool init(ComponentSystemEventHandlerPtr const& _eventHandler);

        //////////////////////////////////////////
        void notifyEntityAdded(Entity* _entity);

    protected:
        ComponentSystemEventHandlerPtr m_eventHandler;
        SwitchableContainer<FastVector<EntityId>> m_addedEntities;
    };


    //////////////////////////////////////////
    // Class ComponentSystemEntityRemovedFromSampleEventHandler
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ComponentSystemEntityRemovedFromSampleEventHandler
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        static inline ComponentSystemEntityRemovedFromSampleEventHandlerPtr Create(
            ComponentSystemEventHandlerPtr const& _eventHandler)
        {
            ComponentSystemEntityRemovedFromSampleEventHandlerPtr object;
            MAZE_CREATE_AND_INIT_SHARED_PTR(ComponentSystemEntityRemovedFromSampleEventHandler, object, init(_eventHandler));
            return object;
        }

        //////////////////////////////////////////
        ~ComponentSystemEntityRemovedFromSampleEventHandler();


    protected:

        //////////////////////////////////////////
        ComponentSystemEntityRemovedFromSampleEventHandler();

        //////////////////////////////////////////
        bool init(ComponentSystemEventHandlerPtr const& _eventHandler);

        //////////////////////////////////////////
        void notifyEntityWillBeRemoved(Entity* _entity);

    protected:
        ComponentSystemEventHandlerPtr m_eventHandler;
        SwitchableContainer<FastVector<EntityId>> m_removedEntities;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentSystem_hpp_
//////////////////////////////////////////
