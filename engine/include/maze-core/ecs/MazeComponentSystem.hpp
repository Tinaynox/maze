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
#include "maze-core/ecs/MazeECSTypes.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/events/MazeECSEvents.hpp"
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
    MAZE_USING_SHARED_PTR(ComponentSystem);
    MAZE_USING_SHARED_PTR(SimpleComponentSystem);
    MAZE_USING_SHARED_PTR(SimpleComponentSystemEventHandler);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(IEntitiesSample);


    //////////////////////////////////////////
    class MAZE_CORE_API ComponentSystem
        : public SharedObject<ComponentSystem>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ComponentSystem);

        //////////////////////////////////////////
        friend class ECSWorld;

    public:

        //////////////////////////////////////////
        virtual ~ComponentSystem();

        //////////////////////////////////////////
        void update(UpdateEvent const& _event);

        //////////////////////////////////////////
        virtual S32 getOrder() const { return 0; }


        //////////////////////////////////////////
        virtual CString getName() const { return getClassName(); }


        //////////////////////////////////////////
        ECSWorldPtr getWorld() const { return m_world.lock(); }

        //////////////////////////////////////////
        ECSWorld* getWorldRaw() const { return m_worldRaw; }

    protected:

        //////////////////////////////////////////
        ComponentSystem();

        //////////////////////////////////////////
        void processBeginUpdate(UpdateEvent const& _event) {};

        //////////////////////////////////////////
        virtual void processUpdate(UpdateEvent const& _event) {}

        //////////////////////////////////////////
        void processEndUpdate(UpdateEvent const& _event) {};

        //////////////////////////////////////////
        void setWorld(ECSWorldPtr const& _world);

        //////////////////////////////////////////
        virtual void processSystemAdded();

        //////////////////////////////////////////
        virtual void processSystemRemoved();


    protected:
        ECSWorldWPtr m_world;
        ECSWorld* m_worldRaw;
    };


    //////////////////////////////////////////
    // Class SimpleComponentSystem
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SimpleComponentSystem
        : public ComponentSystem
    {
    public:

        //////////////////////////////////////////
        using Func = void (*)();

        //////////////////////////////////////////
        static inline SimpleComponentSystemPtr Create(
            HashedCString _name,
            IEntitiesSamplePtr _sample,
            Func _func,
            S32 _order = 0)
        {
            return MAZE_CREATE_SHARED_PTR_WITH_ARGS(SimpleComponentSystem, _name, _sample, _func, _order);
        }

        //////////////////////////////////////////
        virtual Maze::ClassUID getClassUID() const MAZE_OVERRIDE { return m_name.hash; }

        //////////////////////////////////////////
        virtual CString getName() const MAZE_OVERRIDE { return m_name; }

    protected:

        //////////////////////////////////////////
        SimpleComponentSystem(
            HashedCString _name = HashedCString(),
            IEntitiesSamplePtr _sample = nullptr,
            Func _func = nullptr,
            S32 _order = 0)
            : m_name(_name)
            , m_sample(_sample)
            , m_func(_func)
            , m_order(_order)
        {}

        //////////////////////////////////////////
        virtual void processUpdate(UpdateEvent const& _event) MAZE_OVERRIDE
        {
            m_sample->processUpdate(_event, m_func);
        }

        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return m_order; }

    protected:
        HashedCString m_name;
        IEntitiesSamplePtr m_sample;
        Func m_func = nullptr;
        S32 m_order = 0;
    };


    //////////////////////////////////////////
    // Class SimpleComponentSystemEventHandler
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SimpleComponentSystemEventHandler
    {
    public:

        //////////////////////////////////////////
        using Func = void (*)();

        //////////////////////////////////////////
        static inline SimpleComponentSystemEventHandlerPtr Create(
            HashedCString _name,
            ClassUID _eventUID,
            IEntitiesSamplePtr _sample,
            Func _func,
            S32 _order = 0)
        {
            return MAZE_CREATE_SHARED_PTR_WITH_ARGS(SimpleComponentSystemEventHandler, _name, _eventUID, _sample, _func, _order);
        }

        //////////////////////////////////////////
        inline void processEvent(Event* _event)
        {
            m_sample->processEvent(_event, m_func);
        }

        //////////////////////////////////////////
        inline void processEvent(EntityId _entityId, Event* _event)
        {
            m_sample->processEvent(_entityId, _event, m_func);
        }

        //////////////////////////////////////////
        inline ClassUID getEventUID() const { return m_eventUID; }

        //////////////////////////////////////////
        inline S32 getOrder() const { return m_order; }

    protected:

        //////////////////////////////////////////
        SimpleComponentSystemEventHandler(
            HashedCString _name,
            ClassUID _eventUID,
            IEntitiesSamplePtr _sample = nullptr,
            Func _func = nullptr,
            S32 _order = 0)
            : m_name(_name)
            , m_eventUID(_eventUID)
            , m_sample(_sample)
            , m_func(_func)
            , m_order(_order)
        {}

    protected:
        HashedCString m_name;
        ClassUID m_eventUID = 0;
        IEntitiesSamplePtr m_sample;
        Func m_func = nullptr;
        S32 m_order = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentSystem_hpp_
//////////////////////////////////////////
