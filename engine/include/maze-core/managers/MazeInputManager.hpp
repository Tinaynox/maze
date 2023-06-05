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
#if (!defined(_MazeInputManager_hpp_))
#define _MazeInputManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(InputManager);


    //////////////////////////////////////////
    static Size const c_cursorsCountMax = 4;

    //////////////////////////////////////////
    static Size const c_cursorButtonsCountMax = 8;


    //////////////////////////////////////////
    // Class InputEventPool
    //
    //////////////////////////////////////////
    template <typename TEvent, Size TCapacity>
    struct InputEventPool
    {
        //////////////////////////////////////////
        InputEventPool()
            : count(0)
        {
        }

        TEvent events[TCapacity];
        Size count;

        //////////////////////////////////////////
        inline void generateEvent(TEvent&& _event)
        {
            if (count >= TCapacity)
            {
                return;
            }

            events[count] = _event;
            ++count;
        }

        //////////////////////////////////////////
        inline void clear()
        {
            count = 0;
        }
    };


    //////////////////////////////////////////
    // Class InputManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputManager
        : public SharedObject<InputManager>
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        using InputEventsPool = InputEventPool<InputEvent, 256>;

    public:

        //////////////////////////////////////////
        virtual ~InputManager();

        //////////////////////////////////////////
        static void Initialize(InputManagerPtr& _inputManager);

        //////////////////////////////////////////
        template <typename TInputEvent>
        inline void generateInputEvent(TInputEvent&& _event)
        {
            m_inputEventsPool[m_inputEventsIndex].generateEvent(std::move(_event));
        }

        //////////////////////////////////////////
        inline bool getKeyState(KeyCode const& _keyCode) { return m_keyStates[(Size)_keyCode]; }

        //////////////////////////////////////////
        inline bool getCursorButtonState(S32 _cursorId, S32 _buttonIndex) { return m_cursorStates[_cursorId][_buttonIndex]; }

        //////////////////////////////////////////
        inline void setCursorPosition(S32 _cursorId, Vec2DF const& _value) { m_cursorPositions[_cursorId] = _value; }

        //////////////////////////////////////////
        inline Vec2DF const& getCursorPosition(S32 _cursorId) const { return m_cursorPositions[_cursorId]; }

        //////////////////////////////////////////
        static inline InputManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline InputManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual S32 getUpdatableOrder() const MAZE_OVERRIDE { return -1000; }

    public:

        MultiDelegate<InputEventMouseData const&> eventMouse;
        MultiDelegate<InputEventKeyboardData const&> eventKeyboard;
        MultiDelegate<InputEventTouchData const&> eventTouch;
        MultiDelegate<InputEventVirtualCursorData const&> eventVirtualCursor;
        MultiDelegate<InputEventTextData const&> eventText;

    protected:

        //////////////////////////////////////////
        InputManager();

        //////////////////////////////////////////
        virtual bool init();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;
    
    protected:
        static InputManager* s_instance;

        Size m_inputEventsIndex;
        InputEventsPool m_inputEventsPool[2];

        bool m_keyStates[(Size)KeyCode::MAX];
        bool m_cursorStates[c_cursorsCountMax][c_cursorButtonsCountMax];
        Vec2DF m_cursorPositions[c_cursorsCountMax];
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInputManager_hpp_
//////////////////////////////////////////
