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
#include "maze-core/helpers/emscripten/MazeInputHelperEmscripten.hpp"
#include "maze-core/helpers/MazeInputHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include <emscripten/key_codes.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace InputHelper
    {
        ////////////////////////////////////
        MAZE_CORE_API KeyCode ConvertVirtualCodeToKeyCode(S64 _virtualCode, U32 _modifiers)
        {
            switch (_virtualCode)
            {
                case DOM_VK_CANCEL:              MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_CANCEL");
                case DOM_VK_HELP:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_HELP");
                case DOM_VK_BACK_SPACE:          return KeyCode::Backspace;
                case DOM_VK_TAB:                 return KeyCode::Tab;
                case DOM_VK_CLEAR:               return KeyCode::Numpad5;
                case DOM_VK_RETURN:              return KeyCode::Enter;
                case DOM_VK_ENTER:               return KeyCode::NumpadEnter;
                case DOM_VK_SHIFT:               return KeyCode::LShift;
                case DOM_VK_CONTROL:             return KeyCode::LControl;
                case DOM_VK_ALT:                 return KeyCode::LAlt;
                case DOM_VK_PAUSE:               return KeyCode::Pause;
                case DOM_VK_CAPS_LOCK:           return KeyCode::CapsLock;
                case DOM_VK_KANA:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_KANA/DOM_VK_HANGUL");
                case DOM_VK_EISU:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_EISU");
                case DOM_VK_JUNJA:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_JUNJA");
                case DOM_VK_FINAL:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_FINAL");
                case DOM_VK_HANJA:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_HANJA/DOM_VK_KANJI");
                case DOM_VK_ESCAPE:              return KeyCode::Escape;
                case DOM_VK_CONVERT:             MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_CONVERT");
                case DOM_VK_NONCONVERT:          MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_NONCONVERT");
                case DOM_VK_ACCEPT:              MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_ACCEPT");
                case DOM_VK_MODECHANGE:          MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_MODECHANGE");
                case DOM_VK_SPACE:               return KeyCode::Space;
                case DOM_VK_PAGE_UP:             return KeyCode::PageUp;
                case DOM_VK_PAGE_DOWN:           return KeyCode::PageDown;
                case DOM_VK_END:                 return KeyCode::End;
                case DOM_VK_HOME:                return KeyCode::Home;
                case DOM_VK_LEFT:                return KeyCode::Left;
                case DOM_VK_UP:                  return KeyCode::Up;
                case DOM_VK_RIGHT:               return KeyCode::Right;
                case DOM_VK_DOWN:                return KeyCode::Down;
                case DOM_VK_SELECT:              MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_SELECT");
                case DOM_VK_PRINT:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_PRINT");
                case DOM_VK_EXECUTE:             MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_EXECUTE");
                case DOM_VK_PRINTSCREEN:         return KeyCode::PrintScreen;
                case DOM_VK_INSERT:              return KeyCode::Insert;
                case DOM_VK_DELETE:              return KeyCode::Delete;
                case DOM_VK_0:                   return KeyCode::Number0;
                case DOM_VK_1:                   return KeyCode::Number1;
                case DOM_VK_2:                   return KeyCode::Number2;
                case DOM_VK_3:                   return KeyCode::Number3;
                case DOM_VK_4:                   return KeyCode::Number4;
                case DOM_VK_5:                   return KeyCode::Number5;
                case DOM_VK_6:                   return KeyCode::Number6;
                case DOM_VK_7:                   return KeyCode::Number7;
                case DOM_VK_8:                   return KeyCode::Number8;
                case DOM_VK_9:                   return KeyCode::Number9;
                case DOM_VK_COLON:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_COLON");
                case DOM_VK_SEMICOLON:           return KeyCode::Semicolon;
                case DOM_VK_LESS_THAN:           MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_LESS_THAN");
                case DOM_VK_EQUALS:              return KeyCode::Equals;
                case DOM_VK_GREATER_THAN:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_GREATER_THAN");
                case DOM_VK_QUESTION_MARK:       MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_QUESTION_MARK");
                case DOM_VK_AT:                  MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_AT");
                case DOM_VK_A:                   return KeyCode::A;
                case DOM_VK_B:                   return KeyCode::B;
                case DOM_VK_C:                   return KeyCode::C;
                case DOM_VK_D:                   return KeyCode::D;
                case DOM_VK_E:                   return KeyCode::E;
                case DOM_VK_F:                   return KeyCode::F;
                case DOM_VK_G:                   return KeyCode::G;
                case DOM_VK_H:                   return KeyCode::H;
                case DOM_VK_I:                   return KeyCode::I;
                case DOM_VK_J:                   return KeyCode::J;
                case DOM_VK_K:                   return KeyCode::K;
                case DOM_VK_L:                   return KeyCode::L;
                case DOM_VK_M:                   return KeyCode::M;
                case DOM_VK_N:                   return KeyCode::N;
                case DOM_VK_O:                   return KeyCode::O;
                case DOM_VK_P:                   return KeyCode::P;
                case DOM_VK_Q:                   return KeyCode::Q;
                case DOM_VK_R:                   return KeyCode::R;
                case DOM_VK_S:                   return KeyCode::S;
                case DOM_VK_T:                   return KeyCode::T;
                case DOM_VK_U:                   return KeyCode::U;
                case DOM_VK_V:                   return KeyCode::V;
                case DOM_VK_W:                   return KeyCode::W;
                case DOM_VK_X:                   return KeyCode::X;
                case DOM_VK_Y:                   return KeyCode::Y;
                case DOM_VK_Z:                   return KeyCode::Z;
                case DOM_VK_WIN:                 return KeyCode::LCommand;
                case DOM_VK_CONTEXT_MENU:        return KeyCode::Apps;
                case DOM_VK_SLEEP:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_SLEEP");
                case DOM_VK_NUMPAD0:             return KeyCode::Numpad0;
                case DOM_VK_NUMPAD1:             return KeyCode::Numpad1;
                case DOM_VK_NUMPAD2:             return KeyCode::Numpad2;
                case DOM_VK_NUMPAD3:             return KeyCode::Numpad3;
                case DOM_VK_NUMPAD4:             return KeyCode::Numpad4;
                case DOM_VK_NUMPAD5:             return KeyCode::Numpad5;
                case DOM_VK_NUMPAD6:             return KeyCode::Numpad6;
                case DOM_VK_NUMPAD7:             return KeyCode::Numpad7;
                case DOM_VK_NUMPAD8:             return KeyCode::Numpad8;
                case DOM_VK_NUMPAD9:             return KeyCode::Numpad9;
                case DOM_VK_MULTIPLY:            return KeyCode::NumpadMultiply;
                case DOM_VK_ADD:                 return KeyCode::NumpadPlus;
                case DOM_VK_SEPARATOR:           MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_SEPARATOR");
                case DOM_VK_SUBTRACT:            return KeyCode::NumpadMinus;
                case DOM_VK_DECIMAL:             return KeyCode::NumpadDecimal;
                case DOM_VK_DIVIDE:              return KeyCode::NumpadDivide;
                case DOM_VK_F1:                  return KeyCode::F1;
                case DOM_VK_F2:                  return KeyCode::F2;
                case DOM_VK_F3:                  return KeyCode::F3;
                case DOM_VK_F4:                  return KeyCode::F4;
                case DOM_VK_F5:                  return KeyCode::F5;
                case DOM_VK_F6:                  return KeyCode::F6;
                case DOM_VK_F7:                  return KeyCode::F7;
                case DOM_VK_F8:                  return KeyCode::F8;
                case DOM_VK_F9:                  return KeyCode::F9;
                case DOM_VK_F10:                 return KeyCode::F10;
                case DOM_VK_F11:                 return KeyCode::F11;
                case DOM_VK_F12:                 return KeyCode::F12;
                case DOM_VK_F13:                 return KeyCode::F13;
                case DOM_VK_F14:                 return KeyCode::F14;
                case DOM_VK_F15:                 return KeyCode::F15;
                case DOM_VK_F16:                 return KeyCode::F16;
                case DOM_VK_F17:                 return KeyCode::F17;
                case DOM_VK_F18:                 return KeyCode::F18;
                case DOM_VK_F19:                 return KeyCode::F19;
                case DOM_VK_F20:                 return KeyCode::F20;
                case DOM_VK_F21:                 return KeyCode::F21;
                case DOM_VK_F22:                 return KeyCode::F22;
                case DOM_VK_F23:                 return KeyCode::F23;
                case DOM_VK_F24:                 return KeyCode::F24;
                case DOM_VK_NUM_LOCK:            return KeyCode::NumLock;
                case DOM_VK_SCROLL_LOCK:         return KeyCode::ScrollLock;
                case DOM_VK_WIN_OEM_FJ_JISHO:    MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_FJ_JISHO");
                case DOM_VK_WIN_OEM_FJ_MASSHOU:  MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_FJ_MASSHOU");
                case DOM_VK_WIN_OEM_FJ_TOUROKU:  MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_FJ_TOUROKU");
                case DOM_VK_WIN_OEM_FJ_LOYA:     MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_FJ_LOYA");
                case DOM_VK_WIN_OEM_FJ_ROYA:     MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_FJ_ROYA");
                case DOM_VK_CIRCUMFLEX:          MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_CIRCUMFLEX");
                case DOM_VK_EXCLAMATION:         MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_EXCLAMATION");
                case DOM_VK_DOUBLE_QUOTE:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_DOUBLE_QUOTE/DOM_VK_HASH");
                case DOM_VK_DOLLAR:              MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_DOLLAR");
                case DOM_VK_PERCENT:             MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_PERCENT");
                case DOM_VK_AMPERSAND:           MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_AMPERSAND");
                case DOM_VK_UNDERSCORE:          MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_UNDERSCORE");
                case DOM_VK_OPEN_PAREN:          MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_OPEN_PAREN");
                case DOM_VK_CLOSE_PAREN:         MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_CLOSE_PAREN");
                case DOM_VK_ASTERISK:            MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_ASTERISK");
                case DOM_VK_PLUS:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_PLUS");
                case DOM_VK_PIPE:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_PIPE");
                case DOM_VK_HYPHEN_MINUS:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_HYPHEN_MINUS");
                case DOM_VK_OPEN_CURLY_BRACKET:  MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_OPEN_CURLY_BRACKET");
                case DOM_VK_CLOSE_CURLY_BRACKET: MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_CLOSE_CURLY_BRACKET");
                case DOM_VK_TILDE:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_TILDE");
                case DOM_VK_VOLUME_MUTE:         return KeyCode::VolumeMute;
                case DOM_VK_VOLUME_DOWN:         return KeyCode::VolumeDown;
                case DOM_VK_VOLUME_UP:           return KeyCode::VolumeUp;
                case DOM_VK_COMMA:               return KeyCode::Comma;
                case DOM_VK_PERIOD:              return KeyCode::Period;
                case DOM_VK_SLASH:               return KeyCode::Slash;
                case DOM_VK_BACK_QUOTE:          return KeyCode::Grave;
                case DOM_VK_OPEN_BRACKET:        return KeyCode::LBracket;
                case DOM_VK_BACK_SLASH:          return KeyCode::Backslash;
                case DOM_VK_CLOSE_BRACKET:       return KeyCode::RBracket;
                case DOM_VK_QUOTE:               return KeyCode::Apostrophe;
                case DOM_VK_META:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_META");
                case DOM_VK_ALTGR:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_ALTGR");
                case DOM_VK_WIN_ICO_HELP:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_ICO_HELP");
                case DOM_VK_WIN_ICO_00:          MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_ICO_00");
                case DOM_VK_WIN_ICO_CLEAR:       MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_ICO_CLEAR");
                case DOM_VK_WIN_OEM_RESET:       MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_RESET");
                case DOM_VK_WIN_OEM_JUMP:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_JUMP");
                case DOM_VK_WIN_OEM_PA1:         MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_PA1");
                case DOM_VK_WIN_OEM_PA2:         MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_PA2");
                case DOM_VK_WIN_OEM_PA3:         MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_PA3");
                case DOM_VK_WIN_OEM_WSCTRL:      MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_WSCTRL");
                case DOM_VK_WIN_OEM_CUSEL:       MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_CUSEL");
                case DOM_VK_WIN_OEM_ATTN:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_ATTN");
                case DOM_VK_WIN_OEM_FINISH:      MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_FINISH");
                case DOM_VK_WIN_OEM_COPY:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_COPY");
                case DOM_VK_WIN_OEM_AUTO:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_AUTO");
                case DOM_VK_WIN_OEM_ENLW:        MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_ENLW");
                case DOM_VK_WIN_OEM_BACKTAB:     MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_BACKTAB");
                case DOM_VK_ATTN:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_ATTN");
                case DOM_VK_CRSEL:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_CRSEL");
                case DOM_VK_EXSEL:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_EXSEL");
                case DOM_VK_EREOF:               MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_EREOF");
                case DOM_VK_PLAY:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_PLAY");
                case DOM_VK_ZOOM:                MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_ZOOM");
                case DOM_VK_PA1:                 MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_PA1");
                case DOM_VK_WIN_OEM_CLEAR:       MAZE_ERROR_RETURN_VALUE(KeyCode::None, "DOM_VK_WIN_OEM_CLEAR");
                case 189:                        return KeyCode::Minus;
                case 187:                        return KeyCode::Equals;
                case 186:                        return KeyCode::Semicolon;
                case 226:                        return KeyCode::MoreLess;
                case 92:                         return KeyCode::RCommand;
                default:                         MAZE_ERROR_RETURN_VALUE(KeyCode::None, "Undefined KeyCode! code=%d", (S32)_virtualCode);
            }

            return KeyCode::None;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API U32 CollectInputEventKeyboardModifiers(EmscriptenKeyboardEvent const* _event)
        {
            U32 modifiers = InputEventKeyboardModifiers::None;

            if (_event->shiftKey)
                modifiers |= InputEventKeyboardModifiers::ShiftDown;

            if (_event->ctrlKey)
                modifiers |= InputEventKeyboardModifiers::ControlDown;

            if (_event->altKey)
                modifiers |= InputEventKeyboardModifiers::AltDown;

            if (_event->metaKey)
                modifiers |= InputEventKeyboardModifiers::CommandDown;

            if (_event->repeat)
                modifiers |= InputEventKeyboardModifiers::IsKeyRepeat;

            return modifiers;
        }

    } // namespace InputHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
