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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeUIDragElement2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UIDragElement2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIDragElement2D, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UIDragElement2D);

    //////////////////////////////////////////
    UIDragElement2D::UIDragElement2D()
    {
    }

    //////////////////////////////////////////
    UIDragElement2D::~UIDragElement2D()
    {

    }

    //////////////////////////////////////////
    UIDragElement2DPtr UIDragElement2D::Create()
    {
        UIDragElement2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UIDragElement2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UIDragElement2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void UIDragElement2D::processEntityAwakened()
    {
        m_element = getEntityRaw()->ensureComponent<UIElement2D>();
    }        


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(UIDragElement2DOnUIElementCursorPressIn,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        UIElementCursorPressInEvent const& _event,
        Entity* _entity,
        UIDragElement2D* _dragElement)
    {
        Debug::Log("Press %d!", _entity->getId());
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(UIDragElement2DOnUIElementCursorRelease,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        InputCursorReleaseEvent const& _event,
        Entity* _entity,
        UIDragElement2D* _dragElement)
    {
        if (_event.getCursorId() != 0 || _event.getButtonId() != 0)
            return;

        Debug::Log("Release %d!", _entity->getId());
    }    

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(UIDragElement2DOnUIElementCursorMoveIn,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        UIElementCursorMoveInEvent const& _event,
        Entity* _entity,
        UIDragElement2D* _dragElement)
    {
        // Debug::Log("Move In %d!", _entity->getId());
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(UIDragElement2DOnUIElementCursorMoveOut,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        UIElementCursorMoveOutEvent const& _event,
        Entity* _entity,
        UIDragElement2D* _dragElement)
    {
        // Debug::Log("Move Out %d!", _entity->getId());
    }
    
    
} // namespace Maze
//////////////////////////////////////////
