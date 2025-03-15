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
#include "maze-ui/events/MazeUIEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DragAndDropEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DragAndDropValidateEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DragAndDropCurrentZoneChangedEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementClickEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, positionOS, Vec2F::c_zero, getPositionOS, setPositionOS),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementSingleClickEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, positionOS, Vec2F::c_zero, getPositionOS, setPositionOS),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementDoubleClickEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, positionOS, Vec2F::c_zero, getPositionOS, setPositionOS),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementCursorPressInEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, positionOS, Vec2F::c_zero, getPositionOS, setPositionOS),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementCursorMoveInEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, positionOS, Vec2F::c_zero, getPositionOS, setPositionOS),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementCursorMoveOutEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElementCursorReleaseEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CursorInputEvent, inputEvent, CursorInputEvent(), getInputEvent, setInputEvent));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIDragElementDragStartedEvent, Event);

} // namespace Maze
//////////////////////////////////////////
