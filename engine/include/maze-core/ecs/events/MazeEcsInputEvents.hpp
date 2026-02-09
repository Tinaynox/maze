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
#include "maze-core/utils/MazeIndexedResource.hpp"


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
        inline InputKeyboardEvent(
            InputEventKeyboardType _type = InputEventKeyboardType::None,
            S64 _scanCode = 0,
            S64 _virtualCode = 0,
            KeyCode _keyCode = KeyCode::None,
            S8 const* _textUtf8 = "",
            U32 _modifiers = 0u,
            S64 _data = 0)
            : m_type(_type)
            , m_scanCode(_scanCode)
            , m_virtualCode(_virtualCode)
            , m_keyCode(_keyCode)
            , m_modifiers(_modifiers)
            , m_data(_data)
        {
            memcpy(m_textUtf8, _textUtf8, 5);
        }

        //////////////////////////////////////////
        inline InputEventKeyboardType getType() const { return m_type; }

        //////////////////////////////////////////
        inline void setType(InputEventKeyboardType _value) { m_type = _value; }

        //////////////////////////////////////////
        inline S64 getScanCode() const { return m_scanCode; }

        //////////////////////////////////////////
        inline void setScanCode(S64 _value) { m_scanCode = _value; }

        //////////////////////////////////////////
        inline S64 getVirtualCode() const { return m_virtualCode; }

        //////////////////////////////////////////
        inline void setVirtualCode(S64 _value) { m_virtualCode = _value; }

        //////////////////////////////////////////
        inline KeyCode getKeyCode() const { return m_keyCode; }

        //////////////////////////////////////////
        inline void setKeyCode(KeyCode _value) { m_keyCode = _value; }

        //////////////////////////////////////////
        inline S8 const* getTextUtf8() const { return &m_textUtf8[0]; }

        //////////////////////////////////////////
        inline void setTextUtf8(S8 const* _textUtf8) { memcpy(m_textUtf8, _textUtf8, 5); }

        //////////////////////////////////////////
        inline U32 getModifiers() const { return m_modifiers; }

        //////////////////////////////////////////
        inline void setModifiers(U32 _value) { m_modifiers = _value; }

        //////////////////////////////////////////
        inline S64 getData() const { return m_data; }

        //////////////////////////////////////////
        inline void setData(S64 _value) { m_data = _value; }

    private:
        InputEventKeyboardType m_type = InputEventKeyboardType::None;
        S64 m_scanCode = 0;
        S64 m_virtualCode = 0;
        KeyCode m_keyCode = KeyCode::None;
        S8 m_textUtf8[5] = { 0 };
        U32 m_modifiers = 0u;

        S64 m_data = 0;
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
            S32 _windowId = c_invalidResourceId)
            : m_cursorId(_cursorId)
            , m_buttonId(_buttonId)
            , m_position(_position)
            , m_windowId(_windowId)
        {}

        //////////////////////////////////////////
        inline S32 getCursorId() const { return m_cursorId; }

        //////////////////////////////////////////
        inline void setCursorId(S32 _value) { m_cursorId = _value; }

        //////////////////////////////////////////
        inline S32 getButtonId() const { return m_buttonId; }

        //////////////////////////////////////////
        inline void setButtonId(S32 _value) { m_buttonId = _value; }

        //////////////////////////////////////////
        inline Vec2S const& getPosition() const { return m_position; }

        //////////////////////////////////////////
        inline void setPosition(Vec2S const& _value) { m_position = _value; }

        //////////////////////////////////////////
        inline S32 getWindowId() const { return m_windowId; }

        //////////////////////////////////////////
        inline void setWindowId(S32 _value) { m_windowId = _value; }

    private:
        S32 m_cursorId = 0;
        S32 m_buttonId = 0;
        Vec2S m_position = Vec2S::c_zero;
        S32 m_windowId = c_invalidResourceId;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsInputEvents_hpp_
//////////////////////////////////////////
