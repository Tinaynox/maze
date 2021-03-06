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
        static void Initialize(EventManagerPtr& _eventManager);

        
        //////////////////////////////////////////
        static inline EventManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EventManager& GetInstance() { return *s_instance; }
    

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline UniquePtr<TEvent> generateAndSendEventImmediate(
            IEventReceiver* _receiver,
            TArgs... _args)
        {
            TEvent* newEvent = new TEvent(_args...);
            _receiver->processEvent(newEvent);
            return UniquePtr<TEvent>(newEvent);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline UniquePtr<TEvent> generateAndSendEventImmediate(
            IEventReceiverPtr const& _receiver,
            TArgs... _args)
        {
            return generateAndSendEventImmediate<TEvent, TArgs...>(
                _receiver.get(),
                _args...);
        }

        //////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        inline void generateEvent(TArgs... _args)
        {
            Map<ClassUID, Vector<SharedPtr<Event>>>& allEvents = m_events.current();
            Vector<SharedPtr<Event>>& events = allEvents[ClassInfo<TEvent>::UID()];
            events.emplace_back(SharedPtr<Event>(MAZE_NEW_WITH_ARGS(TEvent, _args...)));
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

    protected:

        //////////////////////////////////////////
        EventManager();

        //////////////////////////////////////////
        virtual bool init();

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
