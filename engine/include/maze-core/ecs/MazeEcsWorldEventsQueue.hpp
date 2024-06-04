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
#if (!defined(_MazeEcsWorldEventsQueue_hpp_))
#define _MazeEcsWorldEventsQueue_hpp_


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
    MAZE_USING_SHARED_PTR(EcsWorldEventsQueue);
    MAZE_USING_SHARED_PTR(Event);


    //////////////////////////////////////////
    enum class MAZE_CORE_API EcsWorldEventType
    {
        AddingEntity,
        RemovingEntity,
        AddingToSampleEntity,
        RemovingFromSampleEntity,
        ComponentsChanged,
        ActiveChanged,
        UsualEvent
    };


    //////////////////////////////////////////
    // Class EcsWorldEventsQueue
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsWorldEventsQueue
    {
    public:

        //////////////////////////////////////////
        ~EcsWorldEventsQueue();

        //////////////////////////////////////////
        static EcsWorldEventsQueuePtr Create(EcsWorld* _world);


        //////////////////////////////////////////
        void processEvents();


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
        void processEntityComponentsChanged(EntityId _id);

        //////////////////////////////////////////
        void processEntityActiveChanged(EntityId _id);


        //////////////////////////////////////////
        EntityPtr const& getAddingEntity(EntityId _id);

    protected:

        //////////////////////////////////////////
        EcsWorldEventsQueue();

        //////////////////////////////////////////
        bool init(EcsWorld* _world);


        //////////////////////////////////////////
        void invokeAddingEntityEvent();

        //////////////////////////////////////////
        void invokeRemovingEntityEvent();

        //////////////////////////////////////////
        void invokeAddingToSampleEntityEvent();

        //////////////////////////////////////////
        void invokeRemovingFromSampleEntityEvent();

        //////////////////////////////////////////
        void invokeComponentsChangedEvent();

        //////////////////////////////////////////
        void invokeActiveChangedEvent();

        //////////////////////////////////////////
        void invokeUsualEventEvent();

    private:
        EcsWorld* m_world = nullptr;

        FastVector<EcsWorldEventType> m_eventTypes;
        Deque<EntityPtr> m_addingEntities;
        Queue<EntityId> m_removingEntities;
        // Queue<Pair<ComponentSystemEventHandlerPtr, EntityId>> m_addingToSampleEntities;
        // Queue<Pair<ComponentSystemEventHandlerPtr, EntityId>> m_removingFromSampleEntities;
        Queue<EntityId> m_componentsChangedEntities;
        Queue<EntityId> m_activeChangedEntities;
        Queue<EventPtr> m_usualEvents;

        bool m_processingEvents = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsWorldEventsQueue_hpp_
//////////////////////////////////////////
