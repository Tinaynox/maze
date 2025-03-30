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
    // Class ButtonClickEvent
    //
    //////////////////////////////////////////
    class MAZE_UI_API ButtonClickEvent
        : public GenericEvent<ButtonClickEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ButtonClickEvent, Event);


        //////////////////////////////////////////
        ButtonClickEvent() = default;

        //////////////////////////////////////////
        ButtonClickEvent(
            EntityId const& _value,
            Vec2F const& _positionOS,
            CursorInputEvent _inputEvent)
            : buttonEid(_value)
            , positionOS(_positionOS)
            , inputEvent(_inputEvent)
        {}

        //////////////////////////////////////////
        inline EntityId const& getButtonEid() const { return buttonEid; }

        //////////////////////////////////////////
        inline void setButtonEid(EntityId const& _value) { buttonEid = _value; }

        //////////////////////////////////////////
        inline Vec2F const& getPositionOS() const { return positionOS; }

        //////////////////////////////////////////
        inline void setPositionOS(Vec2F const& _value) { positionOS = _value; }

        //////////////////////////////////////////
        inline CursorInputEvent const& getInputEvent() const { return inputEvent; }

        //////////////////////////////////////////
        inline void setInputEvent(CursorInputEvent const& _value) { inputEvent = _value; }

    public:
        EntityId buttonEid;
        Vec2F positionOS;
        CursorInputEvent inputEvent;
    };


    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIEvents_hpp_
//////////////////////////////////////////
