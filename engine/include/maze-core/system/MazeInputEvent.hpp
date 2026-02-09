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
#if (!defined(_MazeInputEvent_hpp_))
#define _MazeInputEvent_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeKeyCode.hpp"
#include "maze-core/utils/MazeIndexedResource.hpp"
#include <memory>
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Window);


    //////////////////////////////////////////
    enum class InputEventType : U8
    {
        None = 0,
        Mouse,
        Keyboard,
        Touch,
        VirtualCursor,
        Text,
        MAX
    };


    //////////////////////////////////////////
    enum class InputEventMouseType : U8
    {
        None = 0,
        Move,
        Drag,
        Wheel,
        ButtonDown,
        ButtonDoubleClick,
        ButtonUp,
        MAX
    };


    //////////////////////////////////////////
    enum class InputEventKeyboardType : U8
    {
        None = 0,
        KeyDown,
        KeyUp,
        MAX
    };


    //////////////////////////////////////////
    enum class InputEventTouchType : U8
    {
        None = 0,
        Move,
        Press,
        Release,
        Cancel,
        MAX
    };

    //////////////////////////////////////////
    enum class InputEventVirtualCursorType : U8
    {
        None = 0,
        Move,
        Drag,
        Press,
        Release,
        MAX
    };


    //////////////////////////////////////////
    enum class InputEventTextType : U8
    {
        None = 0,
        Input,
        MAX
    };


    //////////////////////////////////////////
    // Struct InputEventMouseData
    //
    //////////////////////////////////////////
    struct InputEventMouseData
    {
        InputEventMouseType type;
        S32 buttonId;
        S32 x;
        S32 y;
        S32 z;

        S32 windowId = c_invalidResourceId;
    };


    //////////////////////////////////////////
    namespace InputEventKeyboardModifiers
    {
        enum Enum
        {
            None            = MAZE_BIT(0),
            CapsLockOn      = MAZE_BIT(1),
            NumLockOn       = MAZE_BIT(2),
            ScrollLockOn    = MAZE_BIT(3),
            ShiftDown       = MAZE_BIT(4),
            ControlDown     = MAZE_BIT(5),
            AltDown         = MAZE_BIT(6),
            CommandDown     = MAZE_BIT(7),
            FnDown          = MAZE_BIT(8),
            IsKeyPad        = MAZE_BIT(9),
            IsLeft          = MAZE_BIT(10),
            IsRight         = MAZE_BIT(11),
            IsKeyRepeat     = MAZE_BIT(12),

            MAX
        };
    };


    //////////////////////////////////////////
    struct InputEventKeyboardData
    {
    public:

        //////////////////////////////////////////
        InputEventKeyboardData()
        {
            memset(textUtf8, 0, 5);
        }

        //////////////////////////////////////////
        inline bool isCapsLockOn() const { return (modifiers & InputEventKeyboardModifiers::CapsLockOn) != 0; }

        //////////////////////////////////////////
        inline bool isNumLockOn() const { return (modifiers & InputEventKeyboardModifiers::NumLockOn) != 0; }

        //////////////////////////////////////////
        inline bool isScrollLockOn() const { return (modifiers & InputEventKeyboardModifiers::ScrollLockOn) != 0; }

        //////////////////////////////////////////
        inline bool isShiftDown() const { return (modifiers & InputEventKeyboardModifiers::ShiftDown) != 0; }

        //////////////////////////////////////////
        inline bool isControlDown() const { return (modifiers & InputEventKeyboardModifiers::ControlDown) != 0; }

        //////////////////////////////////////////
        inline bool isAltDown() const { return (modifiers & InputEventKeyboardModifiers::AltDown) != 0; }

        //////////////////////////////////////////
        inline bool isCommandDown() const { return (modifiers & InputEventKeyboardModifiers::CommandDown) != 0; }

        //////////////////////////////////////////
        inline bool isFnDown() const { return (modifiers & InputEventKeyboardModifiers::FnDown) != 0; }

        //////////////////////////////////////////
        inline bool isKeyPad() const { return (modifiers & InputEventKeyboardModifiers::IsKeyPad) != 0; }

        //////////////////////////////////////////
        inline bool isLeft() const { return (modifiers & InputEventKeyboardModifiers::IsLeft) != 0; }

        //////////////////////////////////////////
        inline bool isRight() const { return (modifiers & InputEventKeyboardModifiers::IsRight) != 0; }

        //////////////////////////////////////////
        inline bool isKeyRepeat() const { return (modifiers & InputEventKeyboardModifiers::IsKeyRepeat) != 0; }

    public:
        // #TODO: Reorder fields for better alignment
        InputEventKeyboardType type = InputEventKeyboardType::None;
        S64 scanCode = 0;
        S64 virtualCode = 0;
        KeyCode keyCode = KeyCode::None;
        S8 textUtf8[5] = { 0 };
        U32 modifiers = 0u;

        S64 data = 0;
    };


    //////////////////////////////////////////
    struct InputEventTouchData
    {
        InputEventTouchType type;
        S32 index;
        S32 x;
        S32 y;

        S32 windowId = c_invalidResourceId;
    };

    //////////////////////////////////////////
    struct InputEventVirtualCursorData
    {
        InputEventVirtualCursorType type;        
        S32 x;
        S32 y;

        S32 windowId = c_invalidResourceId;
    };


    //////////////////////////////////////////
    struct InputEventTextData
    {
        InputEventTextType type;
        U32 utf8;
        S8 textUtf8[5];
    };


    //////////////////////////////////////////
    // Struct
    //
    //////////////////////////////////////////
    #pragma warning(push)
    #pragma warning(disable: 26495)
    struct InputEvent
    {
        //////////////////////////////////////////
        InputEvent()
            : type(InputEventType::None)
        {

        }

        //////////////////////////////////////////
        InputEvent(InputEventMouseData&& _event) noexcept
            : type(InputEventType::Mouse)
            , mouse(std::move(_event))
        {
        }

        //////////////////////////////////////////
        InputEvent(InputEventKeyboardData&& _event) noexcept
            : type(InputEventType::Keyboard)
            , keyboard(std::move(_event))
        {
        }

        //////////////////////////////////////////
        InputEvent(InputEventTouchData&& _event) noexcept
            : type(InputEventType::Touch)
            , touch(std::move(_event))
        {
        }

        //////////////////////////////////////////
        InputEvent(InputEventVirtualCursorData&& _event) noexcept
            : type(InputEventType::VirtualCursor)
            , virtualCursor(std::move(_event))
        {
        }

        //////////////////////////////////////////
        InputEvent(InputEventTextData&& _event) noexcept
            : type(InputEventType::Text)
            , text(std::move(_event))
        {
        }


        InputEventType type;

        union
        {
            InputEventMouseData mouse;
            InputEventKeyboardData keyboard;
            InputEventTouchData touch;
            InputEventVirtualCursorData virtualCursor;
            InputEventTextData text;
        };
    };
    #pragma warning(pop)

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInputEvent_hpp_
//////////////////////////////////////////
