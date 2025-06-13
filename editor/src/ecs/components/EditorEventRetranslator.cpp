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
#include "ecs/components/EditorEventRetranslator.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "ecs/components/EditorHierarchyController.hpp"
#include "managers/EditorManager.hpp"
#include "events/EditorEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorEventRetranslator
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorEventRetranslator, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorEventRetranslator);

    //////////////////////////////////////////
    EditorEventRetranslator::EditorEventRetranslator()
    {
    }

    //////////////////////////////////////////
    EditorEventRetranslator::~EditorEventRetranslator()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->unsubscribeEvent<PlaytestModeEnabledChangedEvent>(this);
    }

    //////////////////////////////////////////
    EditorEventRetranslatorPtr EditorEventRetranslator::Create()
    {
        EditorEventRetranslatorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorEventRetranslator, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool EditorEventRetranslator::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void EditorEventRetranslator::processAppear()
    {
        EventManager::GetInstancePtr()->subscribeEvent<PlaytestModeEnabledChangedEvent>(this, &EditorEventRetranslator::retranslateEvent);
    }

    //////////////////////////////////////////
    void EditorEventRetranslator::processDisappear()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<PlaytestModeEnabledChangedEvent>(this);
    }

    //////////////////////////////////////////
    void EditorEventRetranslator::retranslateEvent(ClassUID _eventUID, Event* _event)
    {
        getEntityRaw()->getEcsWorld()->broadcastEventImmediate(_event);
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(EditorEventRetranslatorAppear,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        EditorEventRetranslator* _retranslator)
    {
        _retranslator->processAppear();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(EditorEventRetranslatorDisappear,
        {},
        {},
        EntityRemovedFromSampleEvent const& _event,
        Entity* _entity,
        EditorEventRetranslator* _retranslator)
    {
        _retranslator->processDisappear();
    }
    
} // namespace Maze
//////////////////////////////////////////
