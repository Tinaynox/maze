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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeGraphicsEventRetranslator.hpp"
#include "maze-graphics/events/MazeGraphicsEvents.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GraphicsEventRetranslator
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(GraphicsEventRetranslator, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GraphicsEventRetranslator);

    //////////////////////////////////////////
    GraphicsEventRetranslator::GraphicsEventRetranslator()
    {
    }

    //////////////////////////////////////////
    GraphicsEventRetranslator::~GraphicsEventRetranslator()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->unsubscribeEvent<RenderWindowWillCloseEvent>(this);
    }

    //////////////////////////////////////////
    GraphicsEventRetranslatorPtr GraphicsEventRetranslator::Create()
    {
        GraphicsEventRetranslatorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GraphicsEventRetranslator, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool GraphicsEventRetranslator::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void GraphicsEventRetranslator::processAppear()
    {
        EventManager::GetInstancePtr()->subscribeEvent<RenderWindowWillCloseEvent>(this, &GraphicsEventRetranslator::retranslateEvent);
    }

    //////////////////////////////////////////
    void GraphicsEventRetranslator::processDisappear()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<RenderWindowWillCloseEvent>(this);
    }

    //////////////////////////////////////////
    void GraphicsEventRetranslator::retranslateEvent(ClassUID _eventUID, Event* _event)
    {
        getEntityRaw()->getEcsWorld()->broadcastEventImmediate(_event);
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(GraphicsEventRetranslatorAppear,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        GraphicsEventRetranslator* _retranslator)
    {
        _retranslator->processAppear();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(GraphicsEventRetranslatorDisappear,
        {},
        {},
        EntityRemovedFromSampleEvent const& _event,
        Entity* _entity,
        GraphicsEventRetranslator* _retranslator)
    {
        _retranslator->processDisappear();
    }
    
} // namespace Maze
//////////////////////////////////////////
