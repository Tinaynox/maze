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
#include "maze-core/helpers/win/MazeInputHelperWin.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include <utf8/utf8.h>
#include <ShlObj.h>
#include <shellapi.h>
#include <intrin.h>       
#include <iphlpapi.h>
#include <lm.h>


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
                case VK_ESCAPE:               return KeyCode::Escape;
                case VK_F1:                   return KeyCode::F1;
                case VK_F2:                   return KeyCode::F2;
                case VK_F3:                   return KeyCode::F3;
                case VK_F4:                   return KeyCode::F4;
                case VK_F5:                   return KeyCode::F5;
                case VK_F6:                   return KeyCode::F6;
                case VK_F7:                   return KeyCode::F7;
                case VK_F8:                   return KeyCode::F8;
                case VK_F9:                   return KeyCode::F9;
                case VK_F10:                  return KeyCode::F10;
                case VK_F11:                  return KeyCode::F11;
                case VK_F12:                  return KeyCode::F12;
                case VK_F13:                  return KeyCode::F13;
                case VK_F14:                  return KeyCode::F14;
                case VK_F15:                  return KeyCode::F15;
                case VK_OEM_3:                return KeyCode::Grave;
                case 0x30:                    return KeyCode::Number0;
                case 0x31:                    return KeyCode::Number1;
                case 0x32:                    return KeyCode::Number2;
                case 0x33:                    return KeyCode::Number3;
                case 0x34:                    return KeyCode::Number4;
                case 0x35:                    return KeyCode::Number5;
                case 0x36:                    return KeyCode::Number6;
                case 0x37:                    return KeyCode::Number7;
                case 0x38:                    return KeyCode::Number8;
                case 0x39:                    return KeyCode::Number9;
                case VK_OEM_MINUS:            return KeyCode::Minus;
                case VK_OEM_PLUS:             return KeyCode::Equals;
                case VK_BACK:                 return KeyCode::Backspace;
                case VK_TAB:                  return KeyCode::Tab;
                case 0x51:                    return KeyCode::Q;
                case 0x57:                    return KeyCode::W;
                case 0x45:                    return KeyCode::E;
                case 0x52:                    return KeyCode::R;
                case 0x54:                    return KeyCode::T;
                case 0x59:                    return KeyCode::Y;
                case 0x55:                    return KeyCode::U;
                case 0x49:                    return KeyCode::I;
                case 0x4F:                    return KeyCode::O;
                case 0x50:                    return KeyCode::P;
                case VK_OEM_4:                return KeyCode::LBracket; 
                case VK_OEM_6:                return KeyCode::RBracket;
                case VK_OEM_5:                return KeyCode::Backslash;
                case VK_CAPITAL:              return KeyCode::CapsLock;
                case 0x41:                    return KeyCode::A;
                case 0x53:                    return KeyCode::S;
                case 0x44:                    return KeyCode::D;
                case 0x46:                    return KeyCode::F;
                case 0x47:                    return KeyCode::G;
                case 0x48:                    return KeyCode::H;
                case 0x4A:                    return KeyCode::J;
                case 0x4B:                    return KeyCode::K;
                case 0x4C:                    return KeyCode::L;
                case VK_OEM_1:                return KeyCode::Semicolon;
                case VK_OEM_7:                return KeyCode::Apostrophe;
                case VK_RETURN:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::NumpadEnter;
                    else
                        return KeyCode::Enter;
                }
                case VK_LSHIFT:               return KeyCode::LShift;
                case VK_RSHIFT:               return KeyCode::RShift;
                case VK_SHIFT:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsRight)
                        return KeyCode::RShift;
                    else
                        return KeyCode::LShift;
                }
                case 0x5A:                    return KeyCode::Z;
                case 0x58:                    return KeyCode::X;
                case 0x43:                    return KeyCode::C;
                case 0x56:                    return KeyCode::V;
                case 0x42:                    return KeyCode::B;
                case 0x4E:                    return KeyCode::N;
                case 0x4D:                    return KeyCode::M;
                case VK_OEM_COMMA:            return KeyCode::Comma;
                case VK_OEM_PERIOD:           return KeyCode::Period;
                case VK_OEM_2:                return KeyCode::Slash;
                case VK_LCONTROL:             return KeyCode::LControl;
                case VK_RCONTROL:             return KeyCode::RControl;
                case VK_CONTROL:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsRight)
                        return KeyCode::RControl;
                    else
                        return KeyCode::LControl;
                }
                case VK_LWIN:                 return KeyCode::LCommand;
                case VK_RWIN:                 return KeyCode::RCommand;
                case VK_LMENU:                return KeyCode::LAlt;
                case VK_RMENU:                return KeyCode::RAlt;
                case VK_MENU:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsRight)
                        return KeyCode::RAlt;
                    else
                        return KeyCode::LAlt;
                }
                case VK_APPS:
                {
                    return KeyCode::Apps;
                    break;
                }
                case VK_SPACE:                return KeyCode::Space;


                // Central
                case VK_SNAPSHOT:             return KeyCode::PrintScreen;
                case VK_SCROLL:               return KeyCode::ScrollLock;
                case VK_PAUSE:                return KeyCode::Pause;            
                case VK_INSERT:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad0;
                    else
                        return KeyCode::Insert;
                }
                case VK_HOME:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad7;
                    else
                        return KeyCode::Home;
                }
                case VK_PRIOR:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad9;
                    else
                        return KeyCode::PageUp;
                }
                case VK_DELETE:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::NumpadDecimal;
                    else
                        return KeyCode::Delete;
                }
                case VK_END:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad1;
                    else                
                        return KeyCode::End;
                }
                case VK_NEXT:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad3;
                    else                
                        return KeyCode::PageDown;
                }
                case VK_UP:                    
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad8;
                    else
                        return KeyCode::Up;
                }
                case VK_LEFT:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad4;
                    else
                        return KeyCode::Left;
                }
                case VK_DOWN:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad2;
                    else
                        return KeyCode::Down;
                }
                case VK_RIGHT:
                {
                    if (_modifiers & InputEventKeyboardModifiers::IsKeyPad)
                        return KeyCode::Numpad6;
                    else
                        return KeyCode::Right;
                }

                // Numpad
                case VK_NUMLOCK:              return KeyCode::NumLock;
                case VK_DIVIDE:               return KeyCode::NumpadDivide;
                case VK_MULTIPLY:             return KeyCode::NumpadMultiply;
                case VK_SUBTRACT:             return KeyCode::NumpadMinus;
                case VK_NUMPAD7:              return KeyCode::Numpad7;
                case VK_NUMPAD8:              return KeyCode::Numpad8;
                case VK_NUMPAD9:              return KeyCode::Numpad9;
                case VK_NUMPAD4:              return KeyCode::Numpad4;
                case VK_NUMPAD5:              return KeyCode::Numpad5;
                case VK_NUMPAD6:              return KeyCode::Numpad6;
                case VK_NUMPAD1:              return KeyCode::Numpad1;
                case VK_NUMPAD2:              return KeyCode::Numpad2;
                case VK_NUMPAD3:              return KeyCode::Numpad3;
                case VK_NUMPAD0:              return KeyCode::Numpad0;
                case VK_ADD:                  return KeyCode::NumpadPlus;
                case VK_DECIMAL:              return KeyCode::NumpadDecimal;
                case VK_CLEAR:                return KeyCode::Numpad5;

                case VK_LAUNCH_MAIL:          return KeyCode::LaunchMail;
                case VK_LAUNCH_MEDIA_SELECT:  return KeyCode::LaunchMedia;
                case VK_LAUNCH_APP1:          return KeyCode::LaunchApp1;
                case VK_LAUNCH_APP2:          return KeyCode::LaunchApp2;
            
                case VK_VOLUME_MUTE:          return KeyCode::VolumeMute;
                case VK_VOLUME_DOWN:          return KeyCode::VolumeDown;
                case VK_VOLUME_UP:            return KeyCode::VolumeUp;

                case VK_MEDIA_NEXT_TRACK:     return KeyCode::MediaNextTrack;
                case VK_MEDIA_PREV_TRACK:     return KeyCode::MediaPrevTrack;
                case VK_MEDIA_STOP:           return KeyCode::MediaStop;
                case VK_MEDIA_PLAY_PAUSE:     return KeyCode::MediaPlayPause;
            
                case VK_BROWSER_BACK:         return KeyCode::WebBack;
                case VK_BROWSER_FORWARD:      return KeyCode::WebForward;
                case VK_BROWSER_REFRESH:      return KeyCode::WebRefresh;
                case VK_BROWSER_STOP:         return KeyCode::WebStop;
                case VK_BROWSER_SEARCH:       return KeyCode::WebSearch;
                case VK_BROWSER_FAVORITES:    return KeyCode::WebFavorites;
                case VK_BROWSER_HOME:         return KeyCode::WebHome;
        
                case VK_OEM_102:              return KeyCode::MoreLess;
                case VK_OEM_8:                return KeyCode::ExclamationMark;
            }

            return KeyCode::None;
        }

        ////////////////////////////////////
        MAZE_CORE_API S64 ConvertKeyCodeToVirtualCode(KeyCode _keyCode, U32 _modifiers)
        {
            switch (_keyCode)
            {
                case KeyCode::Escape:         return VK_ESCAPE;
                case KeyCode::F1:             return VK_F1;
                case KeyCode::F2:             return VK_F2;
                case KeyCode::F3:             return VK_F3;
                case KeyCode::F4:             return VK_F4;
                case KeyCode::F5:             return VK_F5;
                case KeyCode::F6:             return VK_F6;
                case KeyCode::F7:             return VK_F7;
                case KeyCode::F8:             return VK_F8;
                case KeyCode::F9:             return VK_F9;
                case KeyCode::F10:            return VK_F10;
                case KeyCode::F11:            return VK_F11;
                case KeyCode::F12:            return VK_F12;
                case KeyCode::F13:            return VK_F13;
                case KeyCode::F14:            return VK_F14;
                case KeyCode::F15:            return VK_F15;
                case KeyCode::Grave:          return VK_OEM_3;
                case KeyCode::Number0:        return 0x30;
                case KeyCode::Number1:        return 0x31;
                case KeyCode::Number2:        return 0x32;
                case KeyCode::Number3:        return 0x33;
                case KeyCode::Number4:        return 0x34;
                case KeyCode::Number5:        return 0x35;
                case KeyCode::Number6:        return 0x36;
                case KeyCode::Number7:        return 0x37;
                case KeyCode::Number8:        return 0x38;
                case KeyCode::Number9:        return 0x39;
                case KeyCode::Minus:          return VK_OEM_MINUS;
                case KeyCode::Equals:         return VK_OEM_PLUS;
                case KeyCode::Backspace:      return VK_BACK;
                case KeyCode::Tab:            return VK_TAB;
                case KeyCode::Q:              return 0x51;
                case KeyCode::W:              return 0x57;
                case KeyCode::E:              return 0x45;
                case KeyCode::R:              return 0x52;
                case KeyCode::T:              return 0x54;
                case KeyCode::Y:              return 0x59;
                case KeyCode::U:              return 0x55;
                case KeyCode::I:              return 0x49;
                case KeyCode::O:              return 0x4F;
                case KeyCode::P:              return 0x50;
                case KeyCode::LBracket:       return VK_OEM_4; 
                case KeyCode::RBracket:       return VK_OEM_6;
                case KeyCode::Backslash:      return VK_OEM_5;
                case KeyCode::CapsLock:       return VK_CAPITAL;
                case KeyCode::A:              return 0x41;
                case KeyCode::S:              return 0x53;
                case KeyCode::D:              return 0x44;
                case KeyCode::F:              return 0x46;
                case KeyCode::G:              return 0x47;
                case KeyCode::H:              return 0x48;
                case KeyCode::J:              return 0x4A;
                case KeyCode::K:              return 0x4B;
                case KeyCode::L:              return 0x4C;
                case KeyCode::Semicolon:      return VK_OEM_1;
                case KeyCode::Apostrophe:     return VK_OEM_7;
                case KeyCode::Enter:          return VK_RETURN;
                case KeyCode::LShift:         return VK_LSHIFT;
                case KeyCode::RShift:         return VK_RSHIFT;
                case KeyCode::Z:              return 0x5A;
                case KeyCode::X:              return 0x58;
                case KeyCode::C:              return 0x43;
                case KeyCode::V:              return 0x56;
                case KeyCode::B:              return 0x42;
                case KeyCode::N:              return 0x4E;
                case KeyCode::M:              return 0x4D;
                case KeyCode::Comma:          return VK_OEM_COMMA;
                case KeyCode::Period:         return VK_OEM_PERIOD;
                case KeyCode::Slash:          return VK_OEM_2;
                case KeyCode::LControl:       return VK_LCONTROL;
                case KeyCode::RControl:       return VK_RCONTROL;
                case KeyCode::LCommand:       return VK_LWIN;
                case KeyCode::RCommand:       return VK_RWIN;
                case KeyCode::LAlt:           return VK_LMENU;
                case KeyCode::RAlt:           return VK_RMENU;
                case KeyCode::Space:          return VK_SPACE;

                // Central
                case KeyCode::PrintScreen:    return VK_SNAPSHOT;
                case KeyCode::ScrollLock:     return VK_SCROLL;
                case KeyCode::Pause:          return VK_PAUSE;
                case KeyCode::Insert:         return VK_INSERT;
                case KeyCode::Home:           return VK_HOME;
                case KeyCode::PageUp:         return VK_PRIOR;
                case KeyCode::Delete:         return VK_DELETE;
                case KeyCode::End:            return VK_END;
                case KeyCode::PageDown:       return VK_NEXT;
                case KeyCode::Up:             return VK_UP;
                case KeyCode::Left:           return VK_LEFT;
                case KeyCode::Down:           return VK_DOWN;
                case KeyCode::Right:          return VK_RIGHT;

                // Numpad
                case KeyCode::NumLock:        return VK_NUMLOCK;
                case KeyCode::NumpadDivide:   return VK_DIVIDE;
                case KeyCode::NumpadMultiply: return VK_MULTIPLY;
                case KeyCode::NumpadMinus:    return VK_SUBTRACT;
                case KeyCode::Numpad7:        return VK_NUMPAD7;
                case KeyCode::Numpad8:        return VK_NUMPAD8;
                case KeyCode::Numpad9:        return VK_NUMPAD9;
                case KeyCode::Numpad4:        return VK_NUMPAD4;
                case KeyCode::Numpad5:        return VK_NUMPAD5;
                case KeyCode::Numpad6:        return VK_NUMPAD6;
                case KeyCode::Numpad1:        return VK_NUMPAD1;
                case KeyCode::Numpad2:        return VK_NUMPAD2;
                case KeyCode::Numpad3:        return VK_NUMPAD3;
                case KeyCode::Numpad0:        return VK_NUMPAD0;
                case KeyCode::NumpadPlus:     return VK_ADD;
                case KeyCode::NumpadDecimal:  return VK_DECIMAL;

                case KeyCode::LaunchMail:     return VK_LAUNCH_MAIL;
                case KeyCode::LaunchMedia:    return VK_LAUNCH_MEDIA_SELECT;
                case KeyCode::LaunchApp1:     return VK_LAUNCH_APP1;
                case KeyCode::LaunchApp2:     return VK_LAUNCH_APP2;
            
                case KeyCode::VolumeMute:     return VK_VOLUME_MUTE;
                case KeyCode::VolumeDown:     return VK_VOLUME_DOWN;
                case KeyCode::VolumeUp:       return VK_VOLUME_UP;

                case KeyCode::MediaNextTrack: return VK_MEDIA_NEXT_TRACK;
                case KeyCode::MediaPrevTrack: return VK_MEDIA_PREV_TRACK;
                case KeyCode::MediaStop:      return VK_MEDIA_STOP;
                case KeyCode::MediaPlayPause: return VK_MEDIA_PLAY_PAUSE;
            
                case KeyCode::WebBack:        return VK_BROWSER_BACK;
                case KeyCode::WebForward:     return VK_BROWSER_FORWARD;
                case KeyCode::WebRefresh:     return VK_BROWSER_REFRESH;
                case KeyCode::WebStop:        return VK_BROWSER_STOP;
                case KeyCode::WebSearch:      return VK_BROWSER_SEARCH;
                case KeyCode::WebFavorites:   return VK_BROWSER_FAVORITES;
                case KeyCode::WebHome:        return VK_BROWSER_HOME;

                case KeyCode::MoreLess:       return VK_OEM_102;
                case KeyCode::ExclamationMark: return VK_OEM_8;
            }

            return 0;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String TranslateWin32Text(S32 _virtualCode, S32 _scanCode, U32 _modifiers)
        {
            static WCHAR deadKey = 0;

            BYTE keyState[256];
            HKL  layout = GetKeyboardLayout(0);
        
            if (GetKeyboardState(keyState) == 0)
                return String();

            switch (_virtualCode)
            {
                case VK_INSERT:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD0;

                    break;
                }
                case VK_HOME:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD7;

                    break;
                }
                case VK_PRIOR:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD9;

                    break;
                }
                case VK_DELETE:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_DECIMAL;

                    break;
                }
                case VK_END:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD1;

                    break;
                }
                case VK_NEXT:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD3;

                    break;
                }
                case VK_UP:                    
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD8;

                    break;
                }
                case VK_LEFT:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD4;

                    break;
                }
                case VK_DOWN:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD2;

                    break;
                }
                case VK_RIGHT:
                {
                    if (   (_modifiers & InputEventKeyboardModifiers::NumLockOn)
                        && (_modifiers & InputEventKeyboardModifiers::IsKeyPad))
                        _virtualCode = VK_NUMPAD6;

                    break;
                }
            }


            WCHAR buff[3] = {0, 0, 0};
            S32 ascii = ToUnicodeEx(_virtualCode, (UINT)_scanCode, keyState, buff, 3, 0, layout);
            if (ascii == 1 && deadKey != '\0')
            {
                // A dead key is stored and we have just converted a character key
                // Combine the two into a single character
                WCHAR wcBuff[3] = {buff[0], deadKey, '\0'};
                WCHAR out[3];

                deadKey = '\0';
                if (FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
                {
                    String utf8line;
                    utf8::utf16to8(&out[0], &out[0], std::back_inserter(utf8line));

                    MAZE_ERROR_RETURN_VALUE_IF(utf8line.size() > 4, String(), "invalid utf8 symbol");

                    return utf8line;
                }
            }
            else 
            if (ascii == 1)
            {
                // We have a single character
                deadKey = '\0';

                String utf8line;
                utf8::utf16to8(&buff[0], &buff[1], std::back_inserter(utf8line));
            
                MAZE_ERROR_RETURN_VALUE_IF(utf8line.size() > 4, String(), "invalid utf8 symbol");

                return utf8line;
            }
            else 
            if (ascii == 2)
            {
                // Convert a non-combining diacritical mark into a combining diacritical mark
                // Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
                // http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
                switch (buff[0])
                {
                case 0x5E:
                    deadKey = 0x302;
                    break;
                case 0x60:
                    deadKey = 0x300;
                    break;
                case 0xA8: 
                    deadKey = 0x308;
                    break;
                case 0xB4:
                    deadKey = 0x301;
                    break;
                case 0xB8:
                    deadKey = 0x327;
                    break;
                default:
                    deadKey = buff[0];
                    break;
                }
            }

            return String();
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool IsKeyDown(WPARAM _wparam)
        {
            return (GetKeyState(static_cast<S32>(_wparam)) & 0x8000) != 0;
        }

        //////////////////////////////////////////
        MAZE_CORE_API U32 CollectInputEventKeyboardModifiers(UINT _message, WPARAM _wParam, LPARAM _lParam)
        {
            U32 modifiers = InputEventKeyboardModifiers::None;

            if (IsKeyDown(VK_SHIFT))
                modifiers |= InputEventKeyboardModifiers::ShiftDown;

            if (IsKeyDown(VK_CONTROL))
                modifiers |= InputEventKeyboardModifiers::ControlDown;

            if (IsKeyDown(VK_MENU))
                modifiers |= InputEventKeyboardModifiers::AltDown;

            if (IsKeyDown(VK_LWIN) || IsKeyDown(VK_RWIN))
                modifiers |= InputEventKeyboardModifiers::CommandDown;

            if (::GetKeyState(VK_NUMLOCK) & 1)
                modifiers |= InputEventKeyboardModifiers::NumLockOn;

            if (::GetKeyState(VK_CAPITAL) & 1)
                modifiers |= InputEventKeyboardModifiers::CapsLockOn;

            if (::GetKeyState(VK_SCROLL) & 1)
                modifiers |= InputEventKeyboardModifiers::ScrollLockOn;

            switch (_wParam)
            {
                case VK_RETURN:
                {
                    if ((_lParam >> 16) & KF_EXTENDED)
                        modifiers |= InputEventKeyboardModifiers::IsKeyPad;
                    break;
                }
                case VK_INSERT:
                case VK_DELETE:
                case VK_HOME:
                case VK_END:
                case VK_PRIOR:
                case VK_NEXT:
                case VK_UP:
                case VK_DOWN:
                case VK_LEFT:
                case VK_RIGHT:
                {
                    if (!((_lParam >> 16) & KF_EXTENDED))
                        modifiers |= InputEventKeyboardModifiers::IsKeyPad;
                    break;
                }
                case VK_NUMLOCK:
                case VK_NUMPAD0:
                case VK_NUMPAD1:
                case VK_NUMPAD2:
                case VK_NUMPAD3:
                case VK_NUMPAD4:
                case VK_NUMPAD5:
                case VK_NUMPAD6:
                case VK_NUMPAD7:
                case VK_NUMPAD8:
                case VK_NUMPAD9:
                case VK_DIVIDE:
                case VK_MULTIPLY:
                case VK_SUBTRACT:
                case VK_ADD:
                case VK_DECIMAL:
                case VK_CLEAR:
                {
                    modifiers |= InputEventKeyboardModifiers::IsKeyPad;
                    break;
                }
                case VK_SHIFT:
                {
                    if (MapVirtualKey((_lParam & 0x00ff0000) >> 16, MAPVK_VSC_TO_VK_EX) == VK_RSHIFT)
                        modifiers |= InputEventKeyboardModifiers::IsRight;
                    else 
                        modifiers |= InputEventKeyboardModifiers::IsLeft;
                    break;
                }
                case VK_CONTROL:
                {
                    if ((_lParam & 0x01000000) != 0)
                        modifiers |= InputEventKeyboardModifiers::IsRight;
                    else 
                        modifiers |= InputEventKeyboardModifiers::IsLeft;
                    break;
                }
                case VK_MENU:
                {
                    if ((_lParam & 0x01000000) != 0)
                        modifiers |= InputEventKeyboardModifiers::IsRight;
                    else
                        modifiers |= InputEventKeyboardModifiers::IsLeft;

                    break;
                }
                case VK_LWIN:
                {
                    modifiers |= InputEventKeyboardModifiers::IsLeft;
                    break;
                }
                case VK_RWIN:
                {
                    modifiers |= InputEventKeyboardModifiers::IsRight;
                    break;
                }
            }

            switch (_message)
            {
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                {
                    if ((_lParam >> 16) & KF_REPEAT)
                        modifiers |= InputEventKeyboardModifiers::IsKeyRepeat;

                    break;
                }
            }

            return modifiers;
        }

    } // namespace InputHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
