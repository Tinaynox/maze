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
#include "ecs/components/EditorHierarchyControllerAgent.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "ecs/components/EditorHierarchyController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorHierarchyControllerAgent
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorHierarchyControllerAgent, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorHierarchyControllerAgent);

    //////////////////////////////////////////
    EditorHierarchyControllerAgent::EditorHierarchyControllerAgent()
    {
    }

    //////////////////////////////////////////
    EditorHierarchyControllerAgent::~EditorHierarchyControllerAgent()
    {
    }

    //////////////////////////////////////////
    EditorHierarchyControllerAgentPtr EditorHierarchyControllerAgent::Create(EditorHierarchyController* _controller)
    {
        EditorHierarchyControllerAgentPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorHierarchyControllerAgent, object, init(_controller));
        return object;
    }

    //////////////////////////////////////////
    bool EditorHierarchyControllerAgent::init(EditorHierarchyController* _controller)
    {
        m_controller = _controller;

        return true;
    }

    //////////////////////////////////////////
    void EditorHierarchyControllerAgent::processEntityParentChanged(
        EntityId _entityId,
        EntityId _prevParentEntityId,
        EntityId _newParentEntityId)
    {
        m_controller->processEntityParentChanged(
            _entityId,
            _prevParentEntityId,
            _newParentEntityId);
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(EditorHierarchyControllerAgentOnEntityParentChanged,
        {},
        {},
        EntityParentChangedEvent& _event,
        Entity* _entity,
        EditorHierarchyControllerAgent* _agent)
    {
        _agent->processEntityParentChanged(
            _event.entityId,
            _event.prevParentEntityId,
            _event.newParentEntityId);
    }


    
} // namespace Maze
//////////////////////////////////////////
