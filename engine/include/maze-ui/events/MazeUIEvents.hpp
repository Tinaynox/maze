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
#if (!defined(_MazeUIEvents_hpp_))
#define _MazeUIEvents_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DragAndDropEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API DragAndDropEvent
        : public GenericEvent<DragAndDropEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DragAndDropEvent, Event);


        //////////////////////////////////////////
        DragAndDropEvent() = default;

        //////////////////////////////////////////
        DragAndDropEvent(
            DataBlock const& _data,
            EntityId _viewEid)
            : data(_data)
            , viewEid(_viewEid)
        {}

    public:
        DataBlock data;
        EntityId viewEid;
    };


    //////////////////////////////////////////
    // Class DragAndDropValidateEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API DragAndDropValidateEvent
        : public GenericEvent<DragAndDropValidateEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DragAndDropValidateEvent, Event);


        //////////////////////////////////////////
        DragAndDropValidateEvent() = default;

        //////////////////////////////////////////
        DragAndDropValidateEvent(
            DataBlock const& _data,
            EntityId _viewEid)
            : data(_data)
            , viewEid(_viewEid)
        {}

    public:
        DataBlock data;
        EntityId viewEid;
        bool dropAllowed = false;
    };


    //////////////////////////////////////////
    // Class DragAndDropCurrentZoneChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API DragAndDropCurrentZoneChangedEvent
        : public GenericEvent<DragAndDropCurrentZoneChangedEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DragAndDropCurrentZoneChangedEvent, Event);


        //////////////////////////////////////////
        DragAndDropCurrentZoneChangedEvent() = default;

        //////////////////////////////////////////
        DragAndDropCurrentZoneChangedEvent(
            EntityId _zoneEid)
            : zoneEid(_zoneEid)
        {}

    public:
        EntityId zoneEid;
    };



    //////////////////////////////////////////
    // Class UIElementClickEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElementClickEvent
        : public GenericEvent<UIElementClickEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElementClickEvent, Event);


        //////////////////////////////////////////
        UIElementClickEvent() = default;

        //////////////////////////////////////////
        UIElementClickEvent(
            Vec2F const& _positionOS,
            CursorInputEvent _inputEvent)
            : positionOS(_positionOS)
            , inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline Vec2F const& getPositionOS() const { return positionOS; }

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

    public:
        Vec2F positionOS;
        CursorInputEvent inputEvent;
    };


    //////////////////////////////////////////
    // Class UIElementSingleClickEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElementSingleClickEvent
        : public GenericEvent<UIElementSingleClickEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElementSingleClickEvent, Event);


        //////////////////////////////////////////
        UIElementSingleClickEvent() = default;

        //////////////////////////////////////////
        UIElementSingleClickEvent(
            Vec2F const& _positionOS,
            CursorInputEvent _inputEvent)
            : positionOS(_positionOS)
            , inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline Vec2F const& getPositionOS() const { return positionOS; }

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

    public:
        Vec2F positionOS;
        CursorInputEvent inputEvent;
    };


    //////////////////////////////////////////
    // Class UIElementDoubleClickEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElementDoubleClickEvent
        : public GenericEvent<UIElementDoubleClickEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElementDoubleClickEvent, Event);


        //////////////////////////////////////////
        UIElementDoubleClickEvent() = default;

        //////////////////////////////////////////
        UIElementDoubleClickEvent(
            Vec2F const& _positionOS,
            CursorInputEvent _inputEvent)
            : positionOS(_positionOS)
            , inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline Vec2F const& getPositionOS() const { return positionOS; }

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

    public:
        Vec2F positionOS;
        CursorInputEvent inputEvent;
    };


    //////////////////////////////////////////
    // Class UIElementCursorPressInEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElementCursorPressInEvent
        : public GenericEvent<UIElementCursorPressInEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElementCursorPressInEvent, Event);


        //////////////////////////////////////////
        UIElementCursorPressInEvent() = default;

        //////////////////////////////////////////
        UIElementCursorPressInEvent(
            Vec2F const& _positionOS,
            CursorInputEvent _inputEvent)
            : positionOS(_positionOS)
            , inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline Vec2F const& getPositionOS() const { return positionOS; }

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

    public:
        Vec2F positionOS;
        CursorInputEvent inputEvent;
    };


    //////////////////////////////////////////
    // Class UIElementCursorMoveInEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElementCursorMoveInEvent
        : public GenericEvent<UIElementCursorMoveInEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElementCursorMoveInEvent, Event);


        //////////////////////////////////////////
        UIElementCursorMoveInEvent() = default;

        //////////////////////////////////////////
        UIElementCursorMoveInEvent(
            Vec2F const& _positionOS,
            CursorInputEvent _inputEvent)
            : positionOS(_positionOS)
            , inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline Vec2F const& getPositionOS() const { return positionOS; }

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

    public:
        Vec2F positionOS;
        CursorInputEvent inputEvent;
    };


    //////////////////////////////////////////
    // Class UIElementCursorMoveOutEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElementCursorMoveOutEvent
        : public GenericEvent<UIElementCursorMoveOutEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElementCursorMoveOutEvent, Event);


        //////////////////////////////////////////
        UIElementCursorMoveOutEvent() = default;

        //////////////////////////////////////////
        UIElementCursorMoveOutEvent(
            CursorInputEvent _inputEvent)
            : inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

    public:
        CursorInputEvent inputEvent;
    };


    //////////////////////////////////////////
    // Class UIDragElementDragStartedEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIDragElementDragStartedEvent
        : public GenericEvent<UIDragElementDragStartedEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIDragElementDragStartedEvent, Event);


        //////////////////////////////////////////
        UIDragElementDragStartedEvent() = default;

    public:
    };
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIEvents_hpp_
//////////////////////////////////////////
