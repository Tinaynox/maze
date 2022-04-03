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
#include "maze-core/managers/emscripten/MazeInputManagerEmscripten.hpp"
#include "maze-core/managers/emscripten/MazeWindowManagerEmscripten.hpp"
#include "maze-core/system/emscripten/MazeWindowEmscripten.hpp"
#include "maze-core/helpers/emscripten/MazeInputHelperEmscripten.hpp"


//////////////////////////////////////////
static Maze::InputManagerEmscripten* g_inputManagerEmscripten = nullptr;


//////////////////////////////////////////
EM_BOOL OnMouseEvent(Maze::S32 _eventType, EmscriptenMouseEvent const* _event, void* _userData)
{
    return g_inputManagerEmscripten->processMouseEvent(_eventType, _event, _userData);
}

//////////////////////////////////////////
EM_BOOL OnKeyboardEvent(Maze::S32 _eventType, EmscriptenKeyboardEvent const* _event, void* _userData)
{
    return g_inputManagerEmscripten->processKeyboardEvent(_eventType, _event, _userData);
}


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline S32 ConvertEmscriptenMouseButtonIdToMazeMouseButtonId(S32 _emscriptenMouseButtonId)
    {
        switch (_emscriptenMouseButtonId)
        {
            case 1: return 2;
            case 2: return 1;
        }
        
        return _emscriptenMouseButtonId;
    }
    
    //////////////////////////////////////////
    // Class InputManagerEmscripten
    //
    //////////////////////////////////////////
    InputManagerEmscripten::InputManagerEmscripten()
    {
        g_inputManagerEmscripten = this;
    }

    //////////////////////////////////////////
    InputManagerEmscripten::~InputManagerEmscripten()
    {
    }

    //////////////////////////////////////////
    bool InputManagerEmscripten::init()
    {
        if (!InputManager::init())
            return false;

        emscripten_set_click_callback(nullptr, nullptr, 1, OnMouseEvent);
        emscripten_set_mousedown_callback(nullptr, nullptr, 1, OnMouseEvent);
        emscripten_set_mouseup_callback(nullptr, nullptr, 1, OnMouseEvent);
        emscripten_set_dblclick_callback(nullptr, nullptr, 1, OnMouseEvent);
        emscripten_set_mousemove_callback(nullptr, nullptr, 1, OnMouseEvent);
        emscripten_set_mouseenter_callback(nullptr, nullptr, 1, OnMouseEvent);
        emscripten_set_mouseleave_callback(nullptr, nullptr, 1, OnMouseEvent);
        
        emscripten_set_keydown_callback(nullptr, nullptr, 1, OnKeyboardEvent);
        emscripten_set_keyup_callback(nullptr, nullptr, 1, OnKeyboardEvent);
        
        return true;
    }
    
    //////////////////////////////////////////
    EM_BOOL InputManagerEmscripten::processMouseEvent(
        Maze::S32 _eventType,
        EmscriptenMouseEvent const* _event,
        void* _userData)
    {
        WindowEmscriptenPtr const& windowEmscripten = WindowManagerEmscripten::GetInstancePtr()->castRaw<WindowManagerEmscripten>()->getWindow();
        
        switch (_eventType)
        {
            case EMSCRIPTEN_EVENT_MOUSEDOWN:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDown;
                event.buttonId = ConvertEmscriptenMouseButtonIdToMazeMouseButtonId(_event->button);
                event.x = _event->clientX;
                event.y = windowEmscripten->getClientSize().y - _event->clientY;
                event.window = windowEmscripten.get();
                generateInputEvent(event);
                break;
            }
            case EMSCRIPTEN_EVENT_MOUSEUP:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonUp;
                event.buttonId = ConvertEmscriptenMouseButtonIdToMazeMouseButtonId(_event->button);
                event.x = _event->clientX;
                event.y = windowEmscripten->getClientSize().y - _event->clientY;
                event.window = windowEmscripten.get();
                generateInputEvent(event);
                break;
            }            
            case EMSCRIPTEN_EVENT_DBLCLICK:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDoubleClick;
                event.buttonId = ConvertEmscriptenMouseButtonIdToMazeMouseButtonId(_event->button);
                event.x = _event->clientX;
                event.y = windowEmscripten->getClientSize().y - _event->clientY;
                event.window = windowEmscripten.get();
                generateInputEvent(event);
                break;
            }        
            case EMSCRIPTEN_EVENT_MOUSEMOVE:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::Move;
                event.x = _event->clientX;
                event.y = windowEmscripten->getClientSize().y - _event->clientY;
                event.window = windowEmscripten.get();
                generateInputEvent(event);
                
                break;
            }
            case EMSCRIPTEN_EVENT_WHEEL:
            {
                Debug::Log("TODO: EMSCRIPTEN_EVENT_WHEEL");
                
                break;
            }
            case EMSCRIPTEN_EVENT_MOUSEENTER:
            {
                Debug::Log("TODO: EMSCRIPTEN_EVENT_MOUSEENTER");
                
                break;
            }
            case EMSCRIPTEN_EVENT_MOUSELEAVE:
            {
                Debug::Log("TODO: EMSCRIPTEN_EVENT_MOUSELEAVE");
                
                break;
            }
            case EMSCRIPTEN_EVENT_MOUSEOVER:
            {
                Debug::Log("TODO: EMSCRIPTEN_EVENT_MOUSEOVER");
                
                break;
            }
            case EMSCRIPTEN_EVENT_MOUSEOUT:
            {
                Debug::Log("TODO: EMSCRIPTEN_EVENT_MOUSEOUT");
                
                break;
            }
            default:
            {
                break;
            }
        }
        
        return 0;
    }
    
    //////////////////////////////////////////
    EM_BOOL InputManagerEmscripten::processKeyboardEvent(
        Maze::S32 _eventType,
        EmscriptenKeyboardEvent const* _event,
        void* _userData)
    {
        WindowEmscriptenPtr const& windowEmscripten = WindowManagerEmscripten::GetInstancePtr()->castRaw<WindowManagerEmscripten>()->getWindow();
        
        switch (_eventType)
        {
            case EMSCRIPTEN_EVENT_KEYDOWN:
            {
                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyDown;
                event.scanCode = 0;
                event.virtualCode = _event->keyCode;
                event.modifiers = InputHelper::CollectInputEventKeyboardModifiers(_event);
                event.keyCode = InputHelper::ConvertVirtualCodeToKeyCode(event.virtualCode, event.modifiers);
                generateInputEvent(event);
                
                {
                    InputEventTextData event;
                    event.type = InputEventTextType::Input;
                    event.utf8 = _event->which;
                    for (Size i = 0; i < 5; ++i)
                        event.textUtf8[i] = (U8)_event->key[i];
                    generateInputEvent(event);
                }
                break;
            }
            case EMSCRIPTEN_EVENT_KEYUP:
            {
                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyUp;
                event.scanCode = 0;
                event.virtualCode = _event->keyCode;
                event.modifiers = InputHelper::CollectInputEventKeyboardModifiers(_event);
                event.keyCode = InputHelper::ConvertVirtualCodeToKeyCode(event.virtualCode, event.modifiers);
                generateInputEvent(event);
                
                break;
            }
        }
        
        return 0;
    }

} // namespace Maze
//////////////////////////////////////////
