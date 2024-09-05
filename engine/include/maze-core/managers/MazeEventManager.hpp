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
#if (!defined(_MazeEventManager_hpp_))
#define _MazeEventManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/utils/MazeSwitchableContainer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EventManager);
    MAZE_USING_SHARED_PTR(IEventReceiver);
    class Event;


    //////////////////////////////////////////
    class MAZE_CORE_API IEventReceiver
    {
    public:
        //////////////////////////////////////////
        virtual void processEvent(Event* _event) {}
    };


    //////////////////////////////////////////
    // Class EventManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EventManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        struct EventsBlock
        {
            MultiDelegate<ClassUID, Event*> event;
            Vector<SharedPtr<Event>> events;
        };


    public:

        //////////////////////////////////////////
        virtual ~EventManager();

        //////////////////////////////////////////
        static void Initialize(
            EventManagerPtr& _eventManager,
            DataBlock const& _config = DataBlock::c_empty);

        
        //////////////////////////////////////////
        static inline EventManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EventManager& GetInstance() { return *s_instance; }
    

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline UniquePtr<TEvent, Maze::DefaultDelete<TEvent>> sendEventImmediate(
            IEventReceiver* _receiver,
            TArgs... _args)
        {
            TEvent* newEvent = MAZE_NEW_WITH_ARGS(TEvent, _args...);
            _receiver->processEvent(newEvent);
            return UniquePtr<TEvent, Maze::DefaultDelete<TEvent>>(newEvent);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline UniquePtr<TEvent, Maze::DefaultDelete<TEvent>> sendEventImmediate(
            IEventReceiverPtr const& _receiver,
            TArgs... _args)
        {
            return sendEventImmediate<TEvent, TArgs...>(
                _receiver.get(),
                _args...);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void broadcastEvent(TArgs... _args)
        {
            Map<ClassUID, Vector<SharedPtr<Event>>>& allEvents = m_events.current();
            Vector<SharedPtr<Event>>& events = allEvents[ClassInfo<TEvent>::UID()];
            events.emplace_back(MAZE_CREATE_SHARED_PTR_WITH_ARGS(TEvent, _args...));
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void broadcastEventImmediate(TArgs... _args)
        {
            TEvent evt(_args...);
            auto& cb = getEventCallbacks<TEvent>();
            cb(ClassInfo<TEvent>::UID(), &evt);
        }

        //////////////////////////////////////////
        template <typename TEvent>
        inline MultiDelegate<ClassUID, Event*>& getEventCallbacks()
        {
            return m_eventCallbacks[ClassInfo<TEvent>::UID()];
        }

        //////////////////////////////////////////
        template <typename TEvent>
        inline void subscribeEvent(void (*_functor)(TEvent*))
        {
            getEventCallbacks<TEvent>().subscribe(
                [_functor](ClassUID _eventUID, Event* _event) { _functor((TEvent*)_event); });
        }

        //////////////////////////////////////////
        template <typename TEvent, typename C>
        inline void subscribeEvent(C* _object, void(C::* const _method)(ClassUID, Event*))
        {
            getEventCallbacks<TEvent>().subscribe(_object, _method);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename C>
        inline void unsubscribeEvent(C* _object, void(C::* const _method)(ClassUID, Event*))
        {
            getEventCallbacks<TEvent>().unsubscribe(_object, _method);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename C>
        inline void unsubscribeEvent(C* _object)
        {
            getEventCallbacks<TEvent>().unsubscribe(_object);
        }

    protected:

        //////////////////////////////////////////
        EventManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    private:

        static EventManager* s_instance;
        SwitchableContainer<Map<ClassUID, Vector<SharedPtr<Event>>>> m_events;
        Map<ClassUID, MultiDelegate<ClassUID, Event*>> m_eventCallbacks;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEventManager_hpp_
//////////////////////////////////////////
