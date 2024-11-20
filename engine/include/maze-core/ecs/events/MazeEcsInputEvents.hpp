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
#if (!defined(_MazeEcsInputEvents_hpp_))
#define _MazeEcsInputEvents_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class InputMouseEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputMouseEvent
        : public GenericEvent<InputMouseEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InputMouseEvent, Event);

    public:

        //////////////////////////////////////////
        inline InputMouseEvent(InputEventMouseData const& _data = InputEventMouseData())
            : m_data(_data)
        {}

        //////////////////////////////////////////
        inline InputEventMouseData const& getData() const { return m_data; }

    private:
        InputEventMouseData m_data;
    };


    //////////////////////////////////////////
    // Class InputKeyboardEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputKeyboardEvent
        : public GenericEvent<InputKeyboardEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InputKeyboardEvent, Event);

    public:

        //////////////////////////////////////////
        inline InputKeyboardEvent(InputEventKeyboardData const& _data = InputEventKeyboardData())
            : m_data(_data)
        {}

        //////////////////////////////////////////
        inline InputEventKeyboardData const& getData() const { return m_data; }

    private:
        InputEventKeyboardData m_data;
    };


    //////////////////////////////////////////
    // Class InputTouchEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputTouchEvent
        : public GenericEvent<InputTouchEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InputTouchEvent, Event);

    public:

        //////////////////////////////////////////
        inline InputTouchEvent(InputEventTouchData const& _data = InputEventTouchData())
            : m_data(_data)
        {}

        //////////////////////////////////////////
        inline InputEventTouchData const& getData() const { return m_data; }

    private:
        InputEventTouchData m_data;
    };


    //////////////////////////////////////////
    // Class InputVirtualCursorEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputVirtualCursorEvent
        : public GenericEvent<InputVirtualCursorEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InputVirtualCursorEvent, Event);

    public:

        //////////////////////////////////////////
        inline InputVirtualCursorEvent(InputEventVirtualCursorData const& _data = InputEventVirtualCursorData())
            : m_data(_data)
        {}

        //////////////////////////////////////////
        inline InputEventVirtualCursorData const& getData() const { return m_data; }

    private:
        InputEventVirtualCursorData m_data;
    };


    //////////////////////////////////////////
    // Class InputCursorReleaseEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputCursorReleaseEvent
        : public GenericEvent<InputCursorReleaseEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InputCursorReleaseEvent, Event);

    public:

        //////////////////////////////////////////
        inline InputCursorReleaseEvent(
            S32 _cursorId = 0,
            S32 _buttonId = 0,
            Vec2S const& _position = Vec2S(),
            Window* _window = nullptr)
            : m_cursorId(_cursorId)
            , m_buttonId(_buttonId)
            , m_position(_position)
            , m_window(_window)
        {}

        //////////////////////////////////////////
        inline S32 getCursorId() const { return m_cursorId; }

        //////////////////////////////////////////
        inline S32 getButtonId() const { return m_buttonId; }

        //////////////////////////////////////////
        inline Vec2S const& getPosition() const { return m_position; }

        //////////////////////////////////////////
        inline Window* getWindow() const { return m_window; }

    private:
        S32 m_cursorId = 0;
        S32 m_buttonId = 0;
        Vec2S m_position = Vec2S::c_zero;
        Window* m_window = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsInputEvents_hpp_
//////////////////////////////////////////
