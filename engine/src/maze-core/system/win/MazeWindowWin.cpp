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
#include "maze-core/system/win/MazeWindowWin.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/helpers/win/MazeInputHelperWin.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/system/win/MazeSystemCursorWin.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/events/MazeSystemEvents.hpp"
#include <utf8/utf8.h>
#include <dwmapi.h>
#include <WinUser.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
#if (!defined(WM_MOUSEWHEEL))
#    define WM_MOUSEWHEEL         0x020A
#    define __WM_REALMOUSELAST    WM_MOUSEWHEEL
#else
#    define __WM_REALMOUSELAST    WM_MOUSELAST
#endif

    //////////////////////////////////////////
#if (!defined(GET_X_LPARAM))
#    define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#if (!defined(GET_Y_LPARAM))
#    define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

    //////////////////////////////////////////
    static String const c_WindowClassName = "EngineWindow";
    static S32 c_snapshotHotKeyId = 0;
    static S32 c_altSnapshotHotKeyId = 1;
    

    //////////////////////////////////////////
    LRESULT CALLBACK WndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
    {
        WindowWin* window = _hWnd ? reinterpret_cast<WindowWin*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA)) : nullptr;

        if (window)
        {
            if (!window->processEvent(_msg, _wParam, _lParam))
                return 0;
        }

        return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
    }


    //////////////////////////////////////////
    enum ProcessDpiAwareness
    {
        ProcessDpiUnaware         = 0,
        ProcessSystemDpiAware     = 1,
        ProcessPerMonitorDpiAware = 2
    };


    //////////////////////////////////////////
    using SetProcessDpiAwarenessFuncType        = HRESULT (WINAPI*)(ProcessDpiAwareness);
    using SetProcessDPIAwareFuncType            = BOOL (WINAPI*)(void);


    //////////////////////////////////////////
    void SetProcessDpiAware()
    {
        // Try SetProcessDpiAwareness first
        HINSTANCE shCoreDll = LoadLibraryA("Shcore.dll");

        if (shCoreDll)
        {
            
            SetProcessDpiAwarenessFuncType SetProcessDpiAwarenessFunc = reinterpret_cast<SetProcessDpiAwarenessFuncType>(GetProcAddress(shCoreDll, "SetProcessDpiAwareness"));

            if (SetProcessDpiAwarenessFunc)
            {
                // We only check for E_INVALIDARG because we would get
                // E_ACCESSDENIED if the DPI was already set previously
                // and S_OK means the call was successful
                if (SetProcessDpiAwarenessFunc(ProcessSystemDpiAware) == E_INVALIDARG)
                {
                    MAZE_WARNING("Failed to set process DPI awareness");
                }
                else
                {
                    FreeLibrary(shCoreDll);
                    return;
                }
            }

            FreeLibrary(shCoreDll);
        }

        // Fall back to SetProcessDPIAware if SetProcessDpiAwareness
        // is not available on this system
        HINSTANCE user32Dll = LoadLibrary("user32.dll");

        if (user32Dll)
        {
            SetProcessDPIAwareFuncType SetProcessDPIAwareFunc = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(user32Dll, "SetProcessDPIAware"));

            if (SetProcessDPIAwareFunc)
            {
                if (!SetProcessDPIAwareFunc())
                {
                    MAZE_WARNING("Failed to set process DPI awareness");
                }
            }

            FreeLibrary(user32Dll);
        }
    }


    //////////////////////////////////////////
    // Class WindowParamsWin
    //
    //////////////////////////////////////////
    WindowParamsWin::WindowParamsWin()
    {
    }

    //////////////////////////////////////////
    bool WindowParamsWin::init(WindowParamsPtr const& _windowParams)
    {
        if (!WindowParams::init(_windowParams))
            return false;

        WindowParamsWin* windowParamsWin = _windowParams->castRaw<WindowParamsWin>();


        return true;
    }


    //////////////////////////////////////////
    // Class WindowWin
    //
    //////////////////////////////////////////
    WindowWin::WindowWin()
        : m_handle(NULL)
        , m_surrogateCharacter(0)
        , m_clientSizeDirty(false)
        , m_paintStruct{0}
    {
    }


    //////////////////////////////////////////
    WindowWin::~WindowWin()
    {
        m_destroying = true;
        close();
    }

    //////////////////////////////////////////
    bool WindowWin::init(WindowParamsPtr const& _params)
    {
        if (!Window::init(_params))
            return false;

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        SetProcessDpiAware();

        registerClass();
        createWindow();

        return true;
    }

    //////////////////////////////////////////
    void WindowWin::update(F32 _dt)
    {
        if (m_clientSizeDirty)
        {
            m_clientSizeDirty = false;
            updateClientSize();
        }
    }

    //////////////////////////////////////////
    bool WindowWin::isOpened()
    {
        if (m_handle == 0)
            return false;

        if (m_closing)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool WindowWin::updateTitle()
    {
        if (!isOpened())
            return true;

        SetWindowTextA((HWND)m_handle, m_params->title.c_str());

        return false;
    }

    //////////////////////////////////////////
    bool WindowWin::updateWindowMode()
    {
        if (!isOpened())
            return true;

        updateWindowStyle();

        return false;
    }

    //////////////////////////////////////////
    bool WindowWin::updateMinimized()
    {
        if (!isOpened())
            return true;

        if (m_params->minimized)
            ShowWindow((HWND)m_handle, SW_MINIMIZE);
        else
            ShowWindow((HWND)m_handle, SW_RESTORE);

        return false;
    }

    //////////////////////////////////////////
    inline void CaptureMouseInput(HWND _hwnd)
    {
        ReleaseCapture();
        SetCapture(_hwnd);
    }

    //////////////////////////////////////////
    inline void ReleaseMouseInput(HWND _hwnd)
    {
        HWND captureHWND = GetCapture();
        if (captureHWND == _hwnd)
            ReleaseCapture();
    }

    
    //////////////////////////////////////////
    bool WindowWin::processEvent(UINT _message, WPARAM _wParam, LPARAM _lParam)
    {
        if (m_handle == 0)
            return true;

        InputManager* inputManager = InputManager::GetInstancePtr();

        switch (_message)
        {   
            case WM_MOUSEMOVE:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::Move;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);

                for (S32 i = 0; i < 3; ++i)
                {
                    if (inputManager->getCursorButtonState(0, i))
                    {
                        InputEventMouseData event2;
                        event2.type = InputEventMouseType::Drag;
                        event2.buttonId = i;
                        event2.x = event.x;
                        event2.y = event.y;
                        event2.window = this;
                        inputManager->generateInputEvent(event2);
                    }
                }


                break;
            }
            
            case WM_MOUSEWHEEL:
            {
                POINT pt;
                pt.x = GET_X_LPARAM(_lParam);
                pt.y = GET_Y_LPARAM(_lParam);
                ScreenToClient(m_handle, &pt);

                InputEventMouseData event;
                event.type = InputEventMouseType::Wheel;
                event.x = pt.x;
                event.y = m_params->clientSize.y - pt.y;
                event.z = GET_WHEEL_DELTA_WPARAM(_wParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }
            
            case WM_LBUTTONDOWN:
            {
                CaptureMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDown;
                event.buttonId = 0;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_LBUTTONDBLCLK:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDoubleClick;
                event.buttonId = 0;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_LBUTTONUP:
            {
                ReleaseMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonUp;
                event.buttonId = 0;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_RBUTTONDOWN:
            {
                CaptureMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDown;
                event.buttonId = 1;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_RBUTTONDBLCLK:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDoubleClick;
                event.buttonId = 1;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_RBUTTONUP:
            {
                ReleaseMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonUp;
                event.buttonId = 1;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_MBUTTONDOWN:
            {
                CaptureMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDown;
                event.buttonId = 2;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_MBUTTONDBLCLK:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDoubleClick;
                event.buttonId = 2;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }
            

            case WM_MBUTTONUP:
            {
                ReleaseMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonUp;
                event.buttonId = 2;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

#if (_WIN32_WINNT >= 0x0500)
            case WM_XBUTTONDOWN:
            {
                CaptureMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDown;
                event.buttonId = GET_XBUTTON_WPARAM(_wParam) + 2;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_XBUTTONDBLCLK:
            {
                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonDoubleClick;
                event.buttonId = GET_XBUTTON_WPARAM(_wParam) + 2;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }
            

            case WM_XBUTTONUP:
            {
                ReleaseMouseInput((HWND)m_handle);

                InputEventMouseData event;
                event.type = InputEventMouseType::ButtonUp;
                event.buttonId = GET_XBUTTON_WPARAM(_wParam) + 2;
                event.x = GET_X_LPARAM(_lParam);
                event.y = m_params->clientSize.y - GET_Y_LPARAM(_lParam);
                event.window = this;
                inputManager->generateInputEvent(event);
                break;
            }
#endif
            
            case WM_KEYDOWN:
            {
                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyDown;
                event.data = _lParam;
                event.scanCode = S64((_lParam >> 16) & 0xFF);
                event.virtualCode = _wParam;
                event.modifiers = InputHelper::CollectInputEventKeyboardModifiers(_message, _wParam, _lParam);
                event.keyCode = InputHelper::ConvertVirtualCodeToKeyCode(event.virtualCode, event.modifiers);
                inputManager->generateInputEvent(event);

                /*
                {
                    
                    String text = InputHelper::TranslateWin32Text(
                        (S32)event.virtualCode,
                        (S32)event.scanCode,
                        event.modifiers);
                    S8 tempBuffer[5] = {0};

                    strcpy_s(tempBuffer, text.c_str());

                    InputEventTextData event;

                    for (Size i = 0; i < 5; ++i)
                        event.textUtf8[i] = (U8)tempBuffer[i];

                    inputManager->generateInputEvent(event);
                }
                */

                break;
            }
            
            case WM_KEYUP:
            {
                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyUp;
                event.data = _lParam;
                event.modifiers = InputHelper::CollectInputEventKeyboardModifiers(_message, _wParam, _lParam);
                event.virtualCode = _wParam;
                event.keyCode = InputHelper::ConvertVirtualCodeToKeyCode(event.virtualCode, event.modifiers); 
                inputManager->generateInputEvent(event);


                if (_wParam == VK_SHIFT)
                {
                    if ((event.modifiers & InputEventKeyboardModifiers::IsLeft))
                        event.keyCode = KeyCode::RShift;
                    else
                        event.keyCode = KeyCode::LShift;

                    inputManager->generateInputEvent(event);
                }
                
                break;
            }
            
            case WM_SYSKEYDOWN:
            {
                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyDown;
                event.data = _lParam;
                event.scanCode = S64((_lParam >> 16) & 0xFF);
                event.virtualCode = _wParam;
                event.modifiers = InputHelper::CollectInputEventKeyboardModifiers(_message, _wParam, _lParam);
                event.keyCode = InputHelper::ConvertVirtualCodeToKeyCode(event.virtualCode, event.modifiers); 
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_SYSKEYUP:
            {
                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyUp;
                event.data = _lParam;
                event.scanCode = S64((_lParam >> 16) & 0xFF);
                event.virtualCode = _wParam;
                event.modifiers = InputHelper::CollectInputEventKeyboardModifiers(_message, _wParam, _lParam);
                event.keyCode = InputHelper::ConvertVirtualCodeToKeyCode(event.virtualCode, event.modifiers); 
                inputManager->generateInputEvent(event);
                break;
            }

            case WM_HOTKEY:
            {
                if (_wParam == c_snapshotHotKeyId || _wParam == c_altSnapshotHotKeyId)
                {
                    InputEventKeyboardData event;
                    event.type = InputEventKeyboardType::KeyDown;
                    event.data = 0;
                    event.scanCode = 0;
                    event.virtualCode = VK_SNAPSHOT;

                    if (_lParam & MOD_SHIFT)
                        event.modifiers |= InputEventKeyboardModifiers::ShiftDown;

                    if (_lParam & MOD_CONTROL)
                        event.modifiers |= InputEventKeyboardModifiers::ControlDown;

                    if (_lParam & MOD_ALT)
                        event.modifiers |= InputEventKeyboardModifiers::AltDown;

                    if (_lParam & MOD_WIN)
                        event.modifiers |= InputEventKeyboardModifiers::CommandDown;

                    event.keyCode = KeyCode::PrintScreen;
                    inputManager->generateInputEvent(event);
                }

                break;
            }
            
            case WM_CHAR:
            {
                U32 character = static_cast<U32>(_wParam);
                if ((character >= 0xD800) && (character <= 0xDBFF))
                {
                    m_surrogateCharacter = static_cast<U16>(character);
                }
                else
                {
                    if ((character >= 0xDC00) && (character <= 0xDFFF))
                    {
                        U16 utf16[] = { m_surrogateCharacter, static_cast<U16>(character) };

                        utf8::utf16to8(&utf16[0], &utf16[0], &character);
                        m_surrogateCharacter = 0;
                    }
                    

                    InputEventTextData event;
                    event.type = InputEventTextType::Input;
                    event.utf8 = character;
                    memset(event.textUtf8, 0, sizeof(event.textUtf8));
                    utf8::append(character, event.textUtf8);
                    inputManager->generateInputEvent(event);

                }

                break;
            }

            

            case WM_CREATE:
            {
                break;
            }

            case WM_CLOSE:
            {
                close();
                return false;
            }

            case WM_DESTROY:
            {
                break;
            }

            case WM_MOVE:
            {
                processWindowPositionChanged();

                break;
            }
            
            case WM_SIZE:
            {
                if (_wParam == SIZE_MINIMIZED)
                {
                    setMinimized(true);
                }
                else
                if (_wParam == SIZE_RESTORED)
                {
                    setMinimized(false);
                }

                if (!getFullscreen() && m_params->flags & WindowStyleFlags::Resizable)
                {
                    S32 width = GET_X_LPARAM(_lParam);
                    S32 height = GET_Y_LPARAM(_lParam);

                    m_params->clientSize = Vec2U(width, height);
                }

                updateClientSize();

                processWindowSizeChanged();

                break;
            }

            
            case WM_STYLECHANGED:
            {
                break;
            }

            case WM_SHOWWINDOW:
            {
                break;
            }

            case WM_DISPLAYCHANGE:
            {
                break;
            }

            
            case WM_ERASEBKGND:
            {
                if (!getFullscreen())
                    m_clientSizeDirty = true;
                break;
            }

            case WM_NCPAINT:
            {
                break;
            }
            
            case WM_SETFOCUS:
            {
                if (!isOpened())
                    return true;

                processWindowFocusChanged();

                break;
            }

            case WM_KILLFOCUS:
            {
                if (!isOpened())
                    return true;

                processWindowFocusChanged();

                break;
            }
            
            case WM_PAINT:
            {
                if (!(m_params->flags & WindowStyleFlags::Background)) 
                {
                    HDC hdc = BeginPaint((HWND)m_handle, &m_paintStruct);
                    SetBkMode(hdc, TRANSPARENT);
                    EndPaint((HWND)m_handle, &m_paintStruct);
                    return false;
                }
            }
            
            case WM_SYSCOMMAND:
            {
                switch (_wParam)
                {
                    case SC_MINIMIZE:
                    {
                        setMinimized(true);
                        return false;
                    }

                    case SC_RESTORE:
                    {
                        setMinimized(false);
                        return false;
                    }

                    case SC_MAXIMIZE:
                    {
                        maximizeFullscreen();
                        return false;
                    }

                    // Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
                    case SC_KEYMENU:
                        return false;
                }

                break;
            }
#if (_WIN32_WINNT >= 0x0600)
            case WM_DWMCOMPOSITIONCHANGED:
            {
                break;
            }
            case WM_DWMNCRENDERINGCHANGED:
            {
                break;
            }
            case WM_DWMCOLORIZATIONCOLORCHANGED:
            {
                break;
            }
            case WM_DWMWINDOWMAXIMIZEDCHANGE:
            {
                break;
            }
#endif
            case WM_DEVICECHANGE:
            {
                if (_wParam == 0x0007) // DBT_DEVNODES_CHANGED
                {
                    if (EventManager::GetInstancePtr())
                        EventManager::GetInstancePtr()->generateEvent<SystemDevicesChangedEvent>();
                }
                
                break;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    void WindowWin::setClientSize(Vec2U const& _size)
    {
        m_params->clientSize = _size;

        RECT rectangle = {
            0,
            0,
            static_cast<long>(m_params->clientSize.x),
            static_cast<long>(m_params->clientSize.y)};
        LONG_PTR gwlStyle = GetWindowLongPtr(m_handle, GWL_STYLE);
        AdjustWindowRect(&rectangle, static_cast<DWORD>(gwlStyle), false);

        updateWindowStyle();
    }

    //////////////////////////////////////////
    Vec2U WindowWin::getClientSize()
    {
        RECT clientRC;
        ::GetClientRect((HWND)m_handle, &clientRC);    
        Vec2U size(clientRC.right - clientRC.left, clientRC.bottom - clientRC.top);

        return size;
    }

    //////////////////////////////////////////
    Vec2U WindowWin::getFullSize()
    {
        RECT clientRC;
        ::GetWindowRect((HWND)m_handle, &clientRC);    
        Vec2U size(clientRC.right - clientRC.left, clientRC.bottom - clientRC.top);

        return size;
    }

    //////////////////////////////////////////
    void WindowWin::setPosition(Vec2S const& _position)
    {
        SetWindowPos((HWND)m_handle, 0, _position.x, _position.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }

    //////////////////////////////////////////
    Vec2S WindowWin::getPosition()
    {
        RECT rc;
        GetWindowRect ((HWND)m_handle, &rc) ;

        return Vec2S(rc.left, rc.top);
    }

    //////////////////////////////////////////
    bool WindowWin::registerClass()
    {        
        WNDCLASSEXA wc;
        memset(&wc, 0, sizeof(WNDCLASSEXA));

        HINSTANCE hinstance = GetModuleHandle(0);

        wc.cbSize        = sizeof(WNDCLASSEXA);
        wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW ;
        wc.lpfnWndProc   = WndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hinstance;
        wc.hIcon         = (HICON)LoadImage(hinstance, MAKEINTRESOURCE(m_params->iconBig), IMAGE_ICON, 256, 256, 0);
        wc.hIconSm       = (HICON)LoadImage(hinstance, MAKEINTRESOURCE(m_params->iconSmall), IMAGE_ICON, 32, 32, 0);

        if (m_params->cursor)
        {
            wc.hCursor = std::static_pointer_cast<SystemCursorWin>(m_params->cursor)->getCursorHandle();
        }
        else
        {
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        }

        if (m_params->flags & WindowStyleFlags::Background)
        {
            HBRUSH brush = CreateSolidBrush(0);
            wc.hbrBackground = brush;
        }
        else
        {
            HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
            wc.hbrBackground = brush;
        }

        wc.lpszMenuName  = 0;

        static S32 s_classNumber = 0;

        m_className = c_WindowClassName + StringHelper::ToString(++s_classNumber) + '\0';
        wc.lpszClassName = &m_className[0];
        wc.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

        if (!RegisterClassExA(&wc))
            return false;
        
        return true;
    }

    //////////////////////////////////////////
    bool WindowWin::createWindow()
    {
        close();

        if (m_params->windowMode == WindowMode::Fullscreen)
        {
            DEVMODEA dmScreenSettings;                                
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);        
            dmScreenSettings.dmPelsWidth = m_params->clientSize.x;                
            dmScreenSettings.dmPelsHeight = m_params->clientSize.y;                
            dmScreenSettings.dmBitsPerPel = m_params->bpp;                    
            dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

            if (ChangeDisplaySettingsA(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                m_params->windowMode = WindowMode::Windowed;
                MAZE_ERROR("Failed to change display mode for fullscreen!");
                eventWindowModeChanged(this);
            }
        }

        bool isFullscreen = (m_params->windowMode == WindowMode::Fullscreen);

        DWORD dwExStyle = getWindowExStyle(m_params->windowMode);
        DWORD dwStyle = getWindowStyle(m_params->windowMode);


        // Save title
        setTitle(m_params->title);

        // Create
        m_handle = CreateWindowExA(
                        dwExStyle,
                        &m_className[0],
                        m_params->title.c_str(),
                        dwStyle,
                        CW_USEDEFAULT, CW_USEDEFAULT, 
                        m_params->clientSize.x  + (isFullscreen ? (GetSystemMetrics(SM_CXFIXEDFRAME) * 2) : 0), 
                        m_params->clientSize.y + (isFullscreen ? (GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)) : 0),
                        0, 0, 
                        GetModuleHandle(NULL), this);
        
        updateClientSize();


        if (m_params->windowMode == WindowMode::Windowed)
        {
            RECT rc;
            GetWindowRect ((HWND)m_handle, &rc) ;
            S32 xPos = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left))/2;
            S32 yPos = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) /2;
            SetWindowPos((HWND)m_handle, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }

                
        SetWindowLongPtrA((HWND)m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

        
        if (m_handle == 0)
            return false;

        HWND hwnd = (HWND)m_handle;
        
        ShowWindow(hwnd, SW_SHOW);
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        
        MAZE_WARNING_IF(!RegisterHotKey(hwnd, c_snapshotHotKeyId, MOD_NOREPEAT, VK_SNAPSHOT), "SNAPSHOT hotkey registration failed.");
        MAZE_WARNING_IF(!RegisterHotKey(hwnd, c_altSnapshotHotKeyId, MOD_NOREPEAT | MOD_ALT, VK_SNAPSHOT), "ALT+SNAPSHOT hotkey registration failed.");

        HICON hIconSmall = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(m_params->iconSmall));
        SendMessage(hwnd, WM_SETICON,ICON_SMALL, (LPARAM)hIconSmall);
        HICON hIconBig = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(m_params->iconBig));
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIconBig);


        if (!(m_params->flags & WindowStyleFlags::Background))
        {
            SetLayeredWindowAttributes(hwnd, RGB(0,255,0), 0, LWA_COLORKEY);

            DWM_BLURBEHIND bb = {0};
            bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
            bb.fEnable = true;
            bb.hRgnBlur = CreateRectRgn(0, 0, 1, 1);
            DwmEnableBlurBehindWindow(hwnd, &bb);
        }
        
        processWindowCreated();

        return true;
    }

    //////////////////////////////////////////
    void WindowWin::close()
    {
        MAZE_PROFILE_EVENT("WindowWin::close");

        if (!isOpened())
            return;

        m_closing = true;

        HWND handle = (HWND)m_handle;

        WindowPtr window;
        if (!m_destroying)
            window = cast<Window>();

        processWindowWillClose();

        m_handle = 0;

        UnregisterHotKey(handle, c_snapshotHotKeyId);
        UnregisterHotKey(handle, c_altSnapshotHotKeyId);

        DestroyWindow(handle);

        if (getFullscreen())
            ChangeDisplaySettings(NULL, 0);

        processWindowClosed();

        m_closing = false;
    }

    //////////////////////////////////////////
    bool WindowWin::getFocused()
    {
        HWND focusedWindow = GetFocus();
        return m_handle == focusedWindow;
    }

    //////////////////////////////////////////
    void WindowWin::setFocused(bool value)
    {
        if (value)
        {
            if (!getFocused())
            {
                SetFocus(m_handle);
            }
        }
        else
        {
            if (getFocused())
            {
                SetFocus(NULL);
            }
        }
    }

    //////////////////////////////////////////
    void WindowWin::maximizeFullscreen()
    {
        DisplayPtr const& relatedDisplay = getRelatedDisplay();

        WindowVideoMode windowVideoMode = WindowHelper::GetDisplayCurrentMode(relatedDisplay);
        m_params->clientSize.x = windowVideoMode.width;
        m_params->clientSize.y = windowVideoMode.height;
        m_params->bpp = windowVideoMode.bpp;
    
        setFullscreen(true);
    }


    //////////////////////////////////////////
    U32 WindowWin::getWindowStyle(WindowMode mode)
    {
        DWORD dwStyle = 0;

        if (mode == WindowMode::Fullscreen)
        {
            dwStyle =   WS_POPUP                | 
                        WS_VISIBLE;
        }
        else
        {
            dwStyle = WS_VISIBLE;

            if (m_params->flags & WindowStyleFlags::Titlebar)
            {
                dwStyle |=  WS_OVERLAPPED       |
                            WS_CAPTION          |
                            WS_BORDER;//        |
                            //WS_DLGFRAME;

                if (  m_params->flags & WindowStyleFlags::CloseButton
                   || m_params->flags & WindowStyleFlags::MinimizeButton
                   || m_params->flags & WindowStyleFlags::MaximizeButton)
                {
                    dwStyle |= WS_SYSMENU;
                }

                if (m_params->flags & WindowStyleFlags::MinimizeButton)
                {
                    dwStyle |= WS_MINIMIZEBOX;
                }

                if (m_params->flags & WindowStyleFlags::MaximizeButton)
                {
                    dwStyle |= WS_MAXIMIZEBOX;
                }
            }
            else
            {
                dwStyle |= WS_POPUP;
            }

            if (m_params->flags & WindowStyleFlags::Resizable)
            {
                dwStyle |= WS_THICKFRAME;
            }
        }

        return dwStyle;
    }

    //////////////////////////////////////////
    U32 WindowWin::getWindowExStyle(WindowMode mode)
    {
        DWORD dwExStyle = 0;

        
        if (mode == WindowMode::Fullscreen)
        {
            dwExStyle = WS_EX_APPWINDOW;
        }
        else
        {
            if (m_params->flags & WindowStyleFlags::Titlebar)
            {
                dwExStyle |=    WS_EX_APPWINDOW     |
                                WS_EX_WINDOWEDGE    | 
                                WS_EX_CLIENTEDGE;


                if (m_params->flags & WindowStyleFlags::CloseButton)
                {
                    
                }
                
            }

            if (!(m_params->flags & WindowStyleFlags::Background))
            {
                dwExStyle |= WS_EX_LAYERED;
            }

            if (m_params->flags & WindowStyleFlags::Topmost)
            {
                dwExStyle |= WS_EX_TOPMOST;
            }
        }
        
            
        return dwExStyle;
    }

    //////////////////////////////////////////
    bool WindowWin::updateWindowStyle()
    {
        // Get the monitor handle where the window is currently located
        HMONITOR hMonitor = MonitorFromWindow(m_handle, MONITOR_DEFAULTTONEAREST);

        // Get the monitor's screen rectangle
        MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
        GetMonitorInfo(hMonitor, &monitorInfo);
        RECT monitorRect = monitorInfo.rcMonitor;

        if (m_params->windowMode == WindowMode::Fullscreen)
        {
            DEVMODEA currentMode = {0};
            currentMode.dmSize = sizeof(DEVMODEA);
            if (!EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &currentMode))
            {
                MAZE_ERROR("Failed to retrieve current display settings!");
                return false;
            }

            DEVMODE devMode = {0};
            devMode.dmSize       = sizeof(devMode);
            devMode.dmPelsWidth  = currentMode.dmPelsWidth;
            devMode.dmPelsHeight = currentMode.dmPelsHeight;
            devMode.dmBitsPerPel = currentMode.dmBitsPerPel;
            devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

            // Reset display settings because of some bug
            ChangeDisplaySettingsA(NULL, 0);

            // Apply fullscreen mode
            auto result = ChangeDisplaySettingsA(&devMode, CDS_FULLSCREEN);
            if (result != DISP_CHANGE_SUCCESSFUL)
            {
                MAZE_ERROR("Failed to change display mode for fullscreen! Error=%d", result);
                m_params->windowMode = WindowMode::Windowed;
                eventWindowModeChanged(this);
                return false;
            }
        }
        else
        {
            // Apply window mode
            auto result = ChangeDisplaySettingsA(NULL, 0);
            if (result != DISP_CHANGE_SUCCESSFUL)
            {
                MAZE_ERROR("Failed to change display mode for windowed! Error=%d", result);
                m_params->windowMode = WindowMode::Fullscreen;
                eventWindowModeChanged(this);
                return false;
            }
        }

            
        SetWindowLongPtr((HWND)m_handle, GWL_EXSTYLE, getWindowExStyle(m_params->windowMode));
        SetWindowLongPtr((HWND)m_handle, GWL_STYLE, getWindowStyle(m_params->windowMode));

        SetWindowPos(
            (HWND)m_handle,
            HWND_TOP,
            monitorRect.left,
            monitorRect.top,
            m_params->clientSize.x,
            m_params->clientSize.y,
            SWP_FRAMECHANGED);


        updateClientSize();

        ShowWindow((HWND)m_handle, SW_SHOW);
        SetForegroundWindow((HWND)m_handle);
        SetFocus((HWND)m_handle);


        return true;
    }

    //////////////////////////////////////////
    void WindowWin::updateClientSize()
    {
        if (IsWindow((HWND)m_handle))
        {
            if (getMinimized())
                return;

            RECT winRC;
            RECT prevClientRC;

            // Get the current window rect
            ::GetWindowRect((HWND)m_handle, &winRC);

            // Get the current client rect
            ::GetClientRect((HWND)m_handle, &prevClientRC);
            Vec2U prevSize(prevClientRC.right - prevClientRC.left, prevClientRC.bottom - prevClientRC.top);
            
            // Calculate difference between current client width
            int dx = (prevClientRC.right - prevClientRC.left) - m_params->clientSize.x; 
            int dy = (prevClientRC.bottom - prevClientRC.top) - m_params->clientSize.y; 

            S32 cx = winRC.right - winRC.left - dx;
            S32 cy = winRC.bottom - winRC.top - dy;

            // Adjust the size
            SetWindowPos((HWND)m_handle, 0, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);

            ::GetWindowRect((HWND)m_handle, &winRC);

            RECT clientRC;
            ::GetClientRect((HWND)m_handle, &clientRC);
            
            Vec2U newSize(clientRC.right - clientRC.left, clientRC.bottom - clientRC.top);

        }
    }

    //////////////////////////////////////////
    DisplayPtr const& WindowWin::getRelatedDisplay()
    {
        static DisplayPtr const nullPointer;
        if (!isOpened())
            return nullPointer;

        HMONITOR monitor = MonitorFromWindow(
          m_handle,
          0);

        if (monitor == NULL)
            return nullPointer;

        WindowManager* windowManager = WindowManager::GetInstancePtr();

        WindowManager::DisplaysList const& displayList = windowManager->getDisplays();

        for (DisplayPtr const& display : displayList)
        {
            if (display->getUserData() == monitor)
                return display;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    Vec2F WindowWin::convertSystemScreenCoordsToWindowCoords(Vec2F const& _posDS)
    {
        POINT pos = { (LONG)_posDS.x, (LONG)_posDS.y };
        ScreenToClient((HWND)m_handle, &pos);
        return { (F32)pos.x, (F32)getClientSize().y - (F32)pos.y };
    }

    //////////////////////////////////////////
    Vec2F WindowWin::convertWindowCoordsToSystemScreenCoords(Vec2F const& _posWS)
    {
        POINT pos = { (LONG)_posWS.x, (LONG)getClientSize().y - (LONG)_posWS.y };
        ClientToScreen((HWND)m_handle, &pos);
        return Vec2F((F32)pos.x, (F32)pos.y);
    }

} // namespace Maze
//////////////////////////////////////////
