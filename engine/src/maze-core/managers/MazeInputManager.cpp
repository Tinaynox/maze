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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeInputManager)


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class InputManager
    //
    //////////////////////////////////////////
    InputManager* InputManager::s_instance = nullptr;

    //////////////////////////////////////////
    InputManager::InputManager()
        : m_inputEventsIndex(0)
        , m_keyStates{ false }
        , m_cursorStates{ false }
        , m_cursorPositions{ Vec2DF::c_zero }
    {
        s_instance = this;        
    }

    //////////////////////////////////////////
    InputManager::~InputManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void InputManager::Initialize(InputManagerPtr& _inputManager)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(InputManager, _inputManager, init());
    }

    //////////////////////////////////////////
    bool InputManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }


    //////////////////////////////////////////
    void InputManager::update(F32 _dt)
    {

        Size inputEventsIndex = m_inputEventsIndex;
        m_inputEventsIndex = (m_inputEventsIndex == 1) ? 0 : 1;


        InputEventsPool& inputEventsPool = m_inputEventsPool[inputEventsIndex];
        for (Size i = 0; i < inputEventsPool.count; ++i)
        {
            InputEvent const& event = inputEventsPool.events[i];

            switch (event.type)
            {
                case InputEventType::Mouse:            
                {
                    switch (event.mouse.type)
                    {
                        case InputEventMouseType::ButtonDown:
                        {
                            if (event.mouse.buttonId < c_cursorButtonsCountMax)
                                m_cursorStates[0][event.mouse.buttonId] = true;
                            break;
                        }
                        case InputEventMouseType::ButtonUp:
                        {
                            if (event.mouse.buttonId < c_cursorButtonsCountMax)
                                m_cursorStates[0][event.mouse.buttonId] = false;
                            break;
                        }
                        case InputEventMouseType::Move:
                        {
                            m_cursorPositions[0] = Vec2DF((F32)event.mouse.x, (F32)event.mouse.y);
                            break;
                        }
                        default:
                            break;
                    }

                    eventMouse(event.mouse);
                    break;
                }
                case InputEventType::Keyboard:        
                {
                    switch (event.keyboard.type)
                    {
                        case InputEventKeyboardType::KeyDown:
                        {
                            m_keyStates[(Size)event.keyboard.keyCode] = true;
                            break;
                        }
                        case InputEventKeyboardType::KeyUp:
                        {
                            m_keyStates[(Size)event.keyboard.keyCode] = false;
                            break;
                        }
                        default:
                            break;
                    }

                    eventKeyboard(event.keyboard); 
                    break;
                }
                case InputEventType::Touch:            
                {
                    switch (event.touch.type)
                    {
                        case InputEventTouchType::Press:
                        {
                            if (event.touch.index < c_cursorsCountMax)
                                m_cursorStates[event.touch.index][0] = true;
                            break;
                        }
                        case InputEventTouchType::Release:
                        {
                            if (event.touch.index < c_cursorsCountMax)
                                m_cursorStates[event.touch.index][0] = false;
                            break;
                        }
                        case InputEventTouchType::Move:
                        {
                            m_cursorPositions[event.touch.index] = Vec2DF((F32)event.touch.x, (F32)event.touch.y);
                            break;
                        }
                        default:
                            break;
                    }

                    eventTouch(event.touch);
                    break;
                }
                case InputEventType::Text:
                {
                    eventText(event.text);
                    break;
                }
                default:
                {
                    MAZE_ERROR("Unsupported input event: %d!", event.type);
                }
            }
        }
        inputEventsPool.clear();
    }


} // namespace Maze
//////////////////////////////////////////
