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
#include "maze-core/system/linux/MazeWindowLinux.hpp"
#include "maze-core/managers/linux/MazeWindowManagerLinux.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/system/MazeKeyCode.hpp"
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>


//////////////////////////////////////////
namespace
{
    ::Display* g_sharedXDisplay = nullptr;
    Maze::U32 g_sharedXDisplayRefCount = 0;
    Maze::Mutex g_sharedXDisplayMutex;

    using AtomMap = Maze::UnorderedMap<Maze::String, Atom>;
    AtomMap g_atoms;

}
//////////////////////////////////////////


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String g_xWindowManagerName;

    String g_wmAbsPosGood[] = { "Enlightenment", "FVWM", "i3" };

    static const unsigned long c_eventMask =    FocusChangeMask      | ButtonPressMask     |
                                                ButtonReleaseMask    | ButtonMotionMask    |
                                                PointerMotionMask    | KeyPressMask        |
                                                KeyReleaseMask       | StructureNotifyMask |
                                                EnterWindowMask      | LeaveWindowMask     |
                                                VisibilityChangeMask | PropertyChangeMask;


    //////////////////////////////////////////
    // Struct KeyRepeatFinder
    //
    //////////////////////////////////////////
    struct KeyRepeatFinder
    {
        //////////////////////////////////////////
        KeyRepeatFinder(U32 _keycode, Time _time)
            : keycode( _keycode )
            , time( _time )
        {}

        //////////////////////////////////////////
        bool operator()(XEvent const& _event)
        {
            return ((_event.type == KeyPress) && (_event.xkey.keycode == keycode) && (_event.xkey.time - time < 2));
        }

        U32 keycode;
        Time time;
    };


    //////////////////////////////////////////
    ::Display* RetainSharedXDisplay()
    {
        MAZE_MUTEX_SCOPED_LOCK(g_sharedXDisplayMutex)

        if (g_sharedXDisplayRefCount == 0)
        {
            g_sharedXDisplay = XOpenDisplay(NULL);

            if (!g_sharedXDisplay)
            {
                MAZE_ERROR("Failed to open X11 display; make sure the DISPLAY environment variable is set correctly");
                std::abort();
            }
        }

        g_sharedXDisplayRefCount++;
        return g_sharedXDisplay;
    }

    //////////////////////////////////////////
    void ReleaseSharedXDisplay(::Display* _xDisplay)
    {
        MAZE_MUTEX_SCOPED_LOCK(g_sharedXDisplayMutex)

        MAZE_ERROR_RETURN_IF(_xDisplay != g_sharedXDisplay, "Display pointers mismatch!");

        --g_sharedXDisplayRefCount;
        if (g_sharedXDisplayRefCount == 0)
        {
            XCloseDisplay(_xDisplay);
            g_sharedXDisplay = nullptr;
        }
    }

    //////////////////////////////////////////
    Atom GetAtom(String const& _name, bool _onlyIfExists)
    {
        AtomMap::const_iterator it = g_atoms.find( _name );

        if (it != g_atoms.end())
            return it->second;

        ::Display* sharedXDisplay = RetainSharedXDisplay();

        Atom atom = XInternAtom(sharedXDisplay, _name.c_str(), _onlyIfExists ? True : False);

        ReleaseSharedXDisplay(sharedXDisplay);

        g_atoms[_name] = atom;

        return atom;
    }


    //////////////////////////////////////////
    bool IsEWMHSupported()
    {
        static bool s_checked = false;
        static bool s_EWMHSupported = false;

        if (s_checked)
            return s_EWMHSupported;

        s_checked = true;

        Atom netSupportingWmCheck = GetAtom("_NET_SUPPORTING_WM_CHECK", true);
        Atom netSupported = GetAtom("_NET_SUPPORTED", true);

        if (!netSupportingWmCheck || !netSupported)
            return false;

        ::Display* xDisplay = RetainSharedXDisplay();

        Atom actualType;
        S32 actualFormat;
        unsigned long numItems;
        unsigned long numBytes;
        unsigned char* data;

        S32 result = XGetWindowProperty(
            xDisplay,
            DefaultRootWindow( xDisplay ),
            netSupportingWmCheck,
            0,
            1,
            False,
            XA_WINDOW,
            &actualType,
            &actualFormat,
            &numItems,
            &numBytes,
            &data );

        if (result != Success || actualType != XA_WINDOW || numItems != 1)
        {
            if (result == Success)
                XFree(data);

            ReleaseSharedXDisplay(xDisplay);
            return false;
        }

        ::Window rootWindow = *reinterpret_cast<::Window*>(data);

        XFree(data);

        if (!rootWindow)
        {
            ReleaseSharedXDisplay(xDisplay);
            return false;
        }

        result = XGetWindowProperty(
            xDisplay,
            rootWindow,
            netSupportingWmCheck,
            0,
            1,
            False,
            XA_WINDOW,
            &actualType,
            &actualFormat,
            &numItems,
            &numBytes,
            &data);

        if (result != Success || actualType != XA_WINDOW || numItems != 1)
        {
            if (result == Success)
                XFree(data);

            ReleaseSharedXDisplay(xDisplay);
            return false;
        }

        ::Window childWindow = *reinterpret_cast<::Window*>(data);

        XFree(data);

        if (!childWindow)
        {
            ReleaseSharedXDisplay(xDisplay);
            return false;
        }

        s_EWMHSupported = true;

        Atom netWmName = GetAtom("_NET_WM_NAME", true);

        if (!netWmName)
        {
            ReleaseSharedXDisplay(xDisplay);
            return true;
        }

        Atom utf8StringType = GetAtom("UTF8_STRING");

        if (!utf8StringType)
            utf8StringType = XA_STRING;

        result = XGetWindowProperty(
            xDisplay,
            rootWindow,
            netWmName,
            0,
            0x7fffffff,
            False,
            utf8StringType,
            &actualType,
            &actualFormat,
            &numItems,
            &numBytes,
            &data );

        if (actualType && numItems)
        {
            CString begin = reinterpret_cast<CString>(data);
            CString end = begin + numItems;

            g_xWindowManagerName = TextHelper::ConvertUTF8ToUTF32(begin, end);
        }

        if (result == Success)
            XFree(data);

        ReleaseSharedXDisplay(xDisplay);

        return true;
    }

    //////////////////////////////////////////
    ::Window GetParentWindow(::Display* _disp, ::Window _win)
    {
        ::Window root, parent;
        ::Window* children = NULL;
        unsigned int numChildren;

        XQueryTree(_disp, _win, &root, &parent, &children, &numChildren);

        if (children != NULL)
            XFree(children);

        return parent;
    }

    //////////////////////////////////////////
    bool GetEWMHFrameExtents(
        ::Display* _disp,
        ::Window _win,
        long& _xFrameExtent,
        long& _yFrameExtent)
    {
        if (!IsEWMHSupported())
            return false;

        Atom frameExtents = GetAtom("_NET_FRAME_EXTENTS", true);

        if (frameExtents == 0)
            return false;

        bool gotFrameExtents = false;
        Atom actualType;
        S32 actualFormat;
        unsigned long numItems;
        unsigned long numBytesLeft;
        unsigned char* data = NULL;

        S32 result = XGetWindowProperty(
            _disp,
            _win,
            frameExtents,
            0,
            4,
            False,
            XA_CARDINAL,
            &actualType,
            &actualFormat,
            &numItems,
            &numBytesLeft,
            &data);

        if ((result == Success) && (actualType == XA_CARDINAL) &&
            (actualFormat == 32) && (numItems == 4) && (numBytesLeft == 0) &&
            (data != NULL))
        {
            gotFrameExtents = true;

            long* extents = (long*) data;

            // Left
            _xFrameExtent = extents[0]; 
            
            // Top
            _yFrameExtent = extents[2];
        }

        if (data != NULL)
            XFree(data);

        return gotFrameExtents;
    }

    //////////////////////////////////////////
    String FindExecutableName()
    {
        S32 file = ::open("/proc/self/cmdline", O_RDONLY | O_NONBLOCK);

        if (file < 0)
            return "maze";

        Vector<Char> buffer(256, 0);
        Size offset = 0;
        ssize_t result = 0;

        while ((result = read(file, &buffer[offset], 256)) > 0)
        {
            buffer.resize( buffer.size() + result, 0 );
            offset += result;
        }

        ::close(file);

        if (offset)
        {
            buffer[offset] = 0;

            return basename(&buffer[0]);
        }

        return "maze";
    }

    //////////////////////////////////////////
    bool IsWMAbsolutePositionGood()
    {
        if (!IsEWMHSupported())
            return false;

        for (Size i = 0; i < (sizeof(g_wmAbsPosGood) / sizeof(g_wmAbsPosGood[0])); i++)
        {
            if (g_wmAbsPosGood[i] == g_xWindowManagerName)
                return true;
        }

        return false;
    }

    //////////////////////////////////////////
    KeyCode ConvertKeySymToKeyCode(KeySym _symbol)
    {
        switch (_symbol)
        {
            case XK_Shift_L:      return KeyCode::LShift;
            case XK_Shift_R:      return KeyCode::RShift;
            case XK_Control_L:    return KeyCode::LControl;
            case XK_Control_R:    return KeyCode::RControl;
            case XK_Alt_L:        return KeyCode::LAlt;
            case XK_Alt_R:        return KeyCode::RAlt;
            case XK_Super_L:      return KeyCode::LCommand;
            case XK_Super_R:      return KeyCode::RCommand;
            case XK_Menu:         return KeyCode::Apps;
            case XK_Escape:       return KeyCode::Escape;
            case XK_semicolon:    return KeyCode::Semicolon;
            case XK_slash:        return KeyCode::Slash;
            case XK_equal:        return KeyCode::Equals;
            case XK_minus:        return KeyCode::Minus;
            case XK_bracketleft:  return KeyCode::LBracket;
            case XK_bracketright: return KeyCode::RBracket;
            case XK_comma:        return KeyCode::Comma;
            case XK_period:       return KeyCode::Period;
            case XK_apostrophe:   return KeyCode::Apostrophe;
            case XK_backslash:    return KeyCode::Backslash;
            case XK_grave:        return KeyCode::Grave;
            case XK_space:        return KeyCode::Space;
            case XK_Return:       return KeyCode::Enter;
            case XK_KP_Enter:     return KeyCode::Enter;
            case XK_BackSpace:    return KeyCode::Backspace;
            case XK_Tab:          return KeyCode::Tab;
            case XK_Prior:        return KeyCode::PageUp;
            case XK_Next:         return KeyCode::PageDown;
            case XK_End:          return KeyCode::End;
            case XK_Home:         return KeyCode::Home;
            case XK_Insert:       return KeyCode::Insert;
            case XK_Delete:       return KeyCode::Delete;
            case XK_KP_Add:       return KeyCode::NumpadPlus;
            case XK_KP_Subtract:  return KeyCode::NumpadMinus;
            case XK_KP_Multiply:  return KeyCode::NumpadMultiply;
            case XK_KP_Divide:    return KeyCode::NumpadDivide;
            case XK_Pause:        return KeyCode::Pause;
            case XK_F1:           return KeyCode::F1;
            case XK_F2:           return KeyCode::F2;
            case XK_F3:           return KeyCode::F3;
            case XK_F4:           return KeyCode::F4;
            case XK_F5:           return KeyCode::F5;
            case XK_F6:           return KeyCode::F6;
            case XK_F7:           return KeyCode::F7;
            case XK_F8:           return KeyCode::F8;
            case XK_F9:           return KeyCode::F9;
            case XK_F10:          return KeyCode::F10;
            case XK_F11:          return KeyCode::F11;
            case XK_F12:          return KeyCode::F12;
            case XK_F13:          return KeyCode::F13;
            case XK_F14:          return KeyCode::F14;
            case XK_F15:          return KeyCode::F15;
            case XK_Left:         return KeyCode::Left;
            case XK_Right:        return KeyCode::Right;
            case XK_Up:           return KeyCode::Up;
            case XK_Down:         return KeyCode::Down;
            case XK_KP_Insert:    return KeyCode::Numpad0;
            case XK_KP_End:       return KeyCode::Numpad1;
            case XK_KP_Down:      return KeyCode::Numpad2;
            case XK_KP_Page_Down: return KeyCode::Numpad3;
            case XK_KP_Left:      return KeyCode::Numpad4;
            case XK_KP_Begin:     return KeyCode::Numpad5;
            case XK_KP_Right:     return KeyCode::Numpad6;
            case XK_KP_Home:      return KeyCode::Numpad7;
            case XK_KP_Up:        return KeyCode::Numpad8;
            case XK_KP_Page_Up:   return KeyCode::Numpad9;
            case XK_a:            return KeyCode::A;
            case XK_b:            return KeyCode::B;
            case XK_c:            return KeyCode::C;
            case XK_d:            return KeyCode::D;
            case XK_e:            return KeyCode::E;
            case XK_f:            return KeyCode::F;
            case XK_g:            return KeyCode::G;
            case XK_h:            return KeyCode::H;
            case XK_i:            return KeyCode::I;
            case XK_j:            return KeyCode::J;
            case XK_k:            return KeyCode::K;
            case XK_l:            return KeyCode::L;
            case XK_m:            return KeyCode::M;
            case XK_n:            return KeyCode::N;
            case XK_o:            return KeyCode::O;
            case XK_p:            return KeyCode::P;
            case XK_q:            return KeyCode::Q;
            case XK_r:            return KeyCode::R;
            case XK_s:            return KeyCode::S;
            case XK_t:            return KeyCode::T;
            case XK_u:            return KeyCode::U;
            case XK_v:            return KeyCode::V;
            case XK_w:            return KeyCode::W;
            case XK_x:            return KeyCode::X;
            case XK_y:            return KeyCode::Y;
            case XK_z:            return KeyCode::Z;
            case XK_0:            return KeyCode::Numpad0;
            case XK_1:            return KeyCode::Numpad1;
            case XK_2:            return KeyCode::Numpad2;
            case XK_3:            return KeyCode::Numpad3;
            case XK_4:            return KeyCode::Numpad4;
            case XK_5:            return KeyCode::Numpad5;
            case XK_6:            return KeyCode::Numpad6;
            case XK_7:            return KeyCode::Numpad7;
            case XK_8:            return KeyCode::Numpad8;
            case XK_9:            return KeyCode::Numpad9;
        }

        return KeyCode::None;
    }


    //////////////////////////////////////////
    // Class WindowParamsLinux
    //
    //////////////////////////////////////////
    WindowParamsLinux::WindowParamsLinux()
        : fetchVisualInfoCallback(nullptr)
    {
    }

    ////////////////////////////////////
    bool WindowParamsLinux::init(WindowParamsPtr const& _windowParams)
    {
        if (!WindowParams::init(_windowParams))
            return false;

        WindowParamsLinux* windowParamsLinux = _windowParams->castRaw<WindowParamsLinux>();

        fetchVisualInfoCallback = windowParamsLinux->fetchVisualInfoCallback;

        return true;
    }


    //////////////////////////////////////////
    // Class WindowLinux
    //
    //////////////////////////////////////////
    WindowLinux::WindowLinux()
        : m_xWindow(0)
        , m_xDisplay(0)
        , m_xScreen(0)
        , m_useSizeHints(false)
        , m_inputMethod(0)
        , m_inputContext(0)
        , m_windowMapped(false)
        , m_lastInputTime(0)
        , m_keyRepeat(true)
        , m_previousSize(Vec2U32::c_zero)
    {
        m_xDisplay = RetainSharedXDisplay();

        IsEWMHSupported();

    }

    //////////////////////////////////////////
    WindowLinux::~WindowLinux()
    {
        if (m_inputContext)
            XDestroyIC(m_inputContext);

        if (m_xWindow)
        {
            XDestroyWindow(m_xDisplay, m_xWindow);
            XFlush(m_xDisplay);
        }

        if (m_inputMethod)
            XCloseIM(m_inputMethod);

        ReleaseSharedXDisplay(m_xDisplay);
    }


    //////////////////////////////////////////
    bool WindowLinux::init(WindowParamsPtr const& _params)
    {
        if (!Window::init(_params))
            return false;

        WindowParamsLinux* paramsLinux = m_params->castRaw<WindowParamsLinux>();


        m_xScreen = DefaultScreen(m_xDisplay);

        bool isFullscreen = (paramsLinux->windowMode == WindowMode::Fullscreen);

        Vec2S32 windowPosition;
        if (isFullscreen)
        {
            windowPosition = getPrimaryMonitorPosition();
        }
        else
        {
            windowPosition.x = (DisplayWidth(m_xDisplay, m_xScreen)  - paramsLinux->clientSize.x) / 2;
            windowPosition.y = (DisplayWidth(m_xDisplay, m_xScreen)  - paramsLinux->clientSize.y) / 2;
        }

        S32 width  = paramsLinux->clientSize.x;
        S32 height = paramsLinux->clientSize.y;

        Visual* visual = NULL;
        S32 depth = 0;


        if (paramsLinux->fetchVisualInfoCallback)
        {
            XVisualInfo visualInfo = paramsLinux->fetchVisualInfoCallback(this);

            visual = visualInfo.visual;
            depth = visualInfo.depth;
        }
        else
        {
            visual = DefaultVisual(m_xDisplay, m_xScreen);
            depth = DefaultDepth(m_xDisplay, m_xScreen);
        }

        XSetWindowAttributes attributes;
        attributes.colormap = XCreateColormap(m_xDisplay, DefaultRootWindow(m_xDisplay), visual, AllocNone);
        attributes.event_mask = c_eventMask;
        attributes.override_redirect = (isFullscreen && !IsEWMHSupported()) ? True : False;

        m_xWindow = XCreateWindow(
            m_xDisplay,
            DefaultRootWindow(m_xDisplay),
            windowPosition.x,
            windowPosition.y,
            width,
            height,
            0,
            depth,
            InputOutput,
            visual,
            CWEventMask | CWOverrideRedirect | CWColormap,
            &attributes);

        if (!m_xWindow)
        {
            MAZE_ERROR("Failed to create window");
            return false;
        }

        setupProtocols();

        XWMHints* hints = XAllocWMHints();
        hints->flags         = StateHint;
        hints->initial_state = NormalState;
        XSetWMHints(m_xDisplay, m_xWindow, hints);
        XFree(hints);


        if (!isFullscreen)
        {
            Atom wmHintsAtom = GetAtom("_MOTIF_WM_HINTS", false);
            if (wmHintsAtom)
            {
                static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
                static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;

                // static const unsigned long MWM_DECOR_ALL      = 1 << 0;
                static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
                static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
                static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
                static const unsigned long MWM_DECOR_MENU        = 1 << 4;
                static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
                static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;

                // static const unsigned long MWM_FUNC_ALL       = 1 << 0;
                static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
                static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
                static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
                static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
                static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

                struct WMHints
                {
                    unsigned long flags;
                    unsigned long functions;
                    unsigned long decorations;
                    long          inputMode;
                    unsigned long state;
                };

                WMHints hints;
                std::memset(&hints, 0, sizeof(hints));
                hints.flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
                hints.decorations = 0;
                hints.functions   = 0;

                if (m_params->flags & WindowStyleFlags::Titlebar)
                {
                    hints.decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
                    hints.functions   |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
                }
                
                if (m_params->flags & WindowStyleFlags::Resizable)
                {
                    hints.decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
                    hints.functions   |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
                }
                
                if (m_params->flags & WindowStyleFlags::CloseButton)
                {
                    hints.decorations |= 0;
                    hints.functions   |= MWM_FUNC_CLOSE;
                }

                XChangeProperty(
                    m_xDisplay,
                    m_xWindow,
                    wmHintsAtom,
                    wmHintsAtom,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&hints),
                    5);
            }
        }

        if (!(m_params->flags & WindowStyleFlags::Resizable))
        {
            m_useSizeHints = true;
            XSizeHints* sizeHints = XAllocSizeHints();
            sizeHints->flags = PMinSize | PMaxSize | USPosition;
            sizeHints->min_width = sizeHints->max_width = width;
            sizeHints->min_height = sizeHints->max_height = height;
            sizeHints->x = windowPosition.x;
            sizeHints->y = windowPosition.y;
            XSetWMNormalHints(m_xDisplay, m_xWindow, sizeHints);
            XFree(sizeHints);
        }

        XClassHint* hint = XAllocClassHint();

        String executableName = FindExecutableName();
        Vector<Char> windowInstance(executableName.size() + 1, 0);
        std::copy(executableName.begin(), executableName.end(), windowInstance.begin());
        hint->res_name = &windowInstance[0];


        String ansiTitle = m_params->title;
        Vector<Char> windowClass(ansiTitle.size() + 1, 0);
        std::copy(ansiTitle.begin(), ansiTitle.end(), windowClass.begin());
        hint->res_class = &windowClass[0];

        XSetClassHint(m_xDisplay, m_xWindow, hint);

        XFree(hint);

        updateTitle();

        initializeWindowLinux();

        if (isFullscreen)
        {
            XSizeHints* sizeHints = XAllocSizeHints();
            long flags = 0;
            XGetWMNormalHints(m_xDisplay, m_xWindow, sizeHints, &flags);
            sizeHints->flags &= ~(PMinSize | PMaxSize);
            XSetWMNormalHints(m_xDisplay, m_xWindow, sizeHints);
            XFree(sizeHints);

            MAZE_TODO;
            // setVideoMode( mode );
            // switchToFullscreen();
        }


        UpdateManager::GetInstancePtr()->addUpdatable(this);


        return true;
    }

    //////////////////////////////////////////
    void WindowLinux::update(F32 _dt)
    {
        processEvents();
    }

    //////////////////////////////////////////
    bool WindowLinux::isOpened()
    {
        return m_xWindow != 0;
    }

    ////////////////////////////////////
    void WindowLinux::setClientSize(Vec2U32 const& _size)
    {
        if (m_useSizeHints)
        {
            XSizeHints* sizeHints = XAllocSizeHints();
            sizeHints->flags = PMinSize | PMaxSize;
            sizeHints->min_width = sizeHints->max_width = _size.x;
            sizeHints->min_height = sizeHints->max_height = _size.y;
            XSetWMNormalHints( m_xDisplay, m_xWindow, sizeHints );
            XFree( sizeHints );
        }

        XResizeWindow(m_xDisplay, m_xWindow, _size.x, _size.y);
        XFlush(m_xDisplay);
    }

    //////////////////////////////////////////
    Vec2U32 WindowLinux::getClientSize()
    {
        XWindowAttributes attributes;
        XGetWindowAttributes(m_xDisplay, m_xWindow, &attributes);
        return Vec2U32(attributes.width, attributes.height);
    }

    //////////////////////////////////////////
    Vec2U32 WindowLinux::getFullSize()
    {
        MAZE_TODO;
        return Vec2U32::c_zero;
    }

    //////////////////////////////////////////
    void WindowLinux::setPosition(Vec2S32 const& _position)
    {
        XMoveWindow(m_xDisplay, m_xWindow, _position.x, _position.y);
        XFlush(m_xDisplay);
    }

    //////////////////////////////////////////
    Vec2S32 WindowLinux::getPosition()
    {
        ::Window child;
        S32 xAbsRelToRoot, yAbsRelToRoot;

        XTranslateCoordinates(
            m_xDisplay,
            m_xWindow,
            DefaultRootWindow(m_xDisplay),
            0,
            0,
            &xAbsRelToRoot,
            &yAbsRelToRoot,
            &child );

        if (IsWMAbsolutePositionGood())
            return Vec2S32(xAbsRelToRoot, yAbsRelToRoot);

        long xFrameExtent, yFrameExtent;

        if (GetEWMHFrameExtents(m_xDisplay, m_xWindow, xFrameExtent, yFrameExtent))
        {
            return Vec2S32((xAbsRelToRoot - xFrameExtent), (yAbsRelToRoot - yFrameExtent));
        }

        ::Window ancestor = m_xWindow;
        ::Window root = DefaultRootWindow(m_xDisplay);

        while (GetParentWindow(m_xDisplay, ancestor) != root)
        {
            ancestor = GetParentWindow(m_xDisplay, ancestor);
        }

        S32 xRelToRoot, yRelToRoot;
        U32 width, height, borderWidth, depth;

        XGetGeometry(
            m_xDisplay,
            ancestor,
            &root,
            &xRelToRoot,
            &yRelToRoot,
            &width,
            &height,
            &borderWidth,
            &depth);

        return Vec2S32(xRelToRoot, yRelToRoot);
    }

    //////////////////////////////////////////
    void WindowLinux::close()
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    bool WindowLinux::getFocused()
    {
        ::Window focusedWindow = 0;
        S32 revertToReturn = 0;
        XGetInputFocus(
            m_xDisplay,
            &focusedWindow,
            &revertToReturn);

        return (m_xWindow == focusedWindow);
    }

    //////////////////////////////////////////
    void WindowLinux::setFocused(bool _value)
    {
        if (_value)
        {
            grabFocus();
        }
        else
        {

        }
    }

    //////////////////////////////////////////
    void WindowLinux::maximizeFullscreen()
    {
        MAZE_TODO;
    }

    //////////////////////////////////////////
    DisplayPtr const& WindowLinux::getRelatedDisplay()
    {
        static DisplayPtr nullPointer;

        WindowManager* windowManager = WindowManager::GetInstancePtr();
        
        if (!windowManager || windowManager->getDisplays().empty())
            return nullPointer;

        MAZE_TODO;

        return windowManager->getDisplays().front();
    }

    //////////////////////////////////////////
    bool WindowLinux::updateTitle()
    {
        std::basic_string<U8> utf8Title;
        for (S8 c : m_params->title)
            utf8Title.push_back(c);

        Atom useUtf8 = GetAtom("UTF8_STRING", false);

        Atom wmName = GetAtom("_NET_WM_NAME", false);
        XChangeProperty(
            m_xDisplay,
            m_xWindow,
            wmName,
            useUtf8,
            8,
            PropModeReplace,
            utf8Title.c_str(),
            utf8Title.size());

        Atom wmIconName = GetAtom("_NET_WM_ICON_NAME", false);
        XChangeProperty(
            m_xDisplay,
            m_xWindow,
            wmIconName,
            useUtf8,
            8,
            PropModeReplace,
            utf8Title.c_str(),
            utf8Title.size());

#if (defined(X_HAVE_UTF8_STRING))

        Xutf8SetWMProperties(
            m_xDisplay,
            m_xWindow,
            m_params->title.c_str(),
            m_params->title.c_str(),
            NULL,
            0,
            NULL,
            NULL,
            NULL);

#else

        XmbSetWMProperties(
            m_xDisplay,
            m_xWindow,
            m_params->title.c_str(),
            m_params->title.c_str(),
            NULL,
            0,
            NULL,
            NULL,
            NULL);

#endif

        return false;
    }

    //////////////////////////////////////////
    bool WindowLinux::updateWindowMode()
    {
        MAZE_TODO;

        return false;
    }

    //////////////////////////////////////////
    bool WindowLinux::updateMinimized()
    {
        return false;
    }

    //////////////////////////////////////////
    Vec2S32 WindowLinux::getPrimaryMonitorPosition()
    {
        Vec2S32 monitorPosition;
        
        ::Window rootWindow = RootWindow(m_xDisplay, m_xScreen);

        XRRScreenResources* res = XRRGetScreenResources(m_xDisplay, rootWindow);
        if (!res)
        {
            MAZE_ERROR("Failed to get the current screen resources for.primary monitor position");
            return monitorPosition;
        }

        int xRandRMajor, xRandRMinor;
        if (!checkXRandR(xRandRMajor, xRandRMinor))
            xRandRMajor = xRandRMinor = 0;

        RROutput output = getOutputPrimary(rootWindow, res, xRandRMajor, xRandRMinor);

        XRROutputInfo* outputInfo = XRRGetOutputInfo(m_xDisplay, res, output);
        if (!outputInfo || outputInfo->connection == RR_Disconnected)
        {
            XRRFreeScreenResources(res);

            if (outputInfo)
                XRRFreeOutputInfo(outputInfo);

            MAZE_ERROR("Failed to get output info for.primary monitor position");
            return monitorPosition;
        }

        XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(m_xDisplay, res, outputInfo->crtc);
        if (!crtcInfo)
        {
            XRRFreeScreenResources(res);
            XRRFreeOutputInfo(outputInfo);
            MAZE_ERROR("Failed to get crtc info for.primary monitor position");
            return monitorPosition;
        }

        monitorPosition.x = crtcInfo->x;
        monitorPosition.y = crtcInfo->y;

        XRRFreeCrtcInfo(crtcInfo);
        XRRFreeOutputInfo(outputInfo);
        XRRFreeScreenResources(res);

        return monitorPosition;
    }

    //////////////////////////////////////////
    bool WindowLinux::checkXRandR(S32& _xRandRMajor, S32& _xRandRMinor)
    {
        S32 version;
        if (!XQueryExtension(m_xDisplay, "RANDR", &version, &version, &version))
        {
            MAZE_ERROR("XRandR extension is not supported");
            return false;
        }

        if (!XRRQueryVersion(m_xDisplay, &_xRandRMajor, &_xRandRMinor) || _xRandRMajor < 1 || (_xRandRMajor == 1 && _xRandRMinor < 2))
        {
            MAZE_ERROR("XRandR is too old");
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    RROutput WindowLinux::getOutputPrimary(::Window& rootWindow, XRRScreenResources* res, S32 _xRandRMajor, S32 _xRandRMinor)
    {
        if ((_xRandRMajor == 1 && _xRandRMinor >= 3) || _xRandRMajor > 1)
        {
            RROutput output = XRRGetOutputPrimary(m_xDisplay, rootWindow);

            if (output == 0)
                return res->outputs[0];
            else
                return output;
        }

        return res->outputs[0];
    }

    //////////////////////////////////////////
    void WindowLinux::setupProtocols()
    {
        Atom wmProtocols = GetAtom("WM_PROTOCOLS");
        Atom wmDeleteWindow = GetAtom("WM_DELETE_WINDOW");

        if (!wmProtocols)
        {
            MAZE_ERROR("Failed to request WM_PROTOCOLS atom");
            return;
        }

        Vector<Atom> atoms;

        if (wmDeleteWindow)
        {
            atoms.push_back(wmDeleteWindow);
        }
        else
        {
            MAZE_ERROR("Failed to request WM_DELETE_WINDOW atom");
        }

        Atom netWmPing = 0;
        Atom netWmPid = 0;

        if (IsEWMHSupported())
        {
            netWmPing = GetAtom("_NET_WM_PING", true);
            netWmPid = GetAtom("_NET_WM_PID", true);
        }

        if (netWmPing && netWmPid)
        {
            long const pid = getpid();

            XChangeProperty(
                m_xDisplay,
                m_xWindow,
                netWmPid,
                XA_CARDINAL,
                32,
                PropModeReplace,
                reinterpret_cast<unsigned char const*>(&pid),
                1);

            atoms.push_back(netWmPing);
        }

        if (!atoms.empty())
        {
            XChangeProperty(
                m_xDisplay,
                m_xWindow,
                wmProtocols,
                XA_ATOM,
                32,
                PropModeReplace,
                reinterpret_cast<unsigned char const*>(&atoms[0]),
                atoms.size());
        }
        else
        {
            MAZE_ERROR("Didn't set any window protocols");
        }
    }


    //////////////////////////////////////////
    void WindowLinux::initializeWindowLinux()
    {
        m_inputMethod = XOpenIM(m_xDisplay, NULL, NULL, NULL);

        if (m_inputMethod)
        {
            m_inputContext = XCreateIC(
                m_inputMethod,
                XNClientWindow,
                m_xWindow,
                XNFocusWindow,
                m_xWindow,
                XNInputStyle,
                XIMPreeditNothing | XIMStatusNothing,
                NULL);
        }
        else
        {
            m_inputContext = NULL;
        }

        MAZE_ERROR_IF(!m_inputContext, "Failed to create input context for window -- TextEntered event won't be able to return unicode");

        Atom wmWindowType = GetAtom("_NET_WM_WINDOW_TYPE", false);
        Atom wmWindowTypeNormal = GetAtom("_NET_WM_WINDOW_TYPE_NORMAL", false);


        if (wmWindowType && wmWindowTypeNormal)
        {
            XChangeProperty(
                m_xDisplay,
                m_xWindow,
                wmWindowType,
                XA_ATOM,
                32,
                PropModeReplace,
                reinterpret_cast<const unsigned char*>(&wmWindowTypeNormal),
                1);
        }

        setXWindowVisible(true);

        grabFocus();

        XFlush(m_xDisplay);

    }

    //////////////////////////////////////////
    void WindowLinux::setXWindowVisible(bool _visible)
    {
        if (_visible)
        {
            XMapWindow(m_xDisplay, m_xWindow);

            if (m_params->windowMode == WindowMode::Fullscreen)
            {
                // #TODO:
                // switchToFullscreen();
            }

            XFlush(m_xDisplay);

            while (!m_windowMapped)
            {
                processEvents();
            }
        }
        else
        {
            XUnmapWindow(m_xDisplay, m_xWindow);

            XFlush(m_xDisplay);

            while (m_windowMapped)
            {
                processEvents();
            }
        }
    }

    //////////////////////////////////////////
    Bool CheckEvent(::Display* _display, XEvent* _event, XPointer _userData)
    {
        return _event->xany.window == reinterpret_cast<::Window>(_userData);
    }

    //////////////////////////////////////////
    void WindowLinux::processEvents()
    {
        XEvent event;

        while (XCheckIfEvent(m_xDisplay, &event, &CheckEvent, reinterpret_cast<XPointer>(m_xWindow)))
        {
            m_events.push_back(event);
        }

        while (!m_events.empty())
        {
            event = m_events.front();
            m_events.pop_front();
            processEvent(event);
        }

    }

    //////////////////////////////////////////
    bool WindowLinux::processEvent(XEvent& windowEvent)
    {
        if (windowEvent.type == KeyRelease)
        {
            Deque<XEvent>::iterator iter = std::find_if(
                m_events.begin(),
                m_events.end(),
                KeyRepeatFinder(windowEvent.xkey.keycode, windowEvent.xkey.time));

            if (iter != m_events.end())
            {
                if (!m_keyRepeat)
                    m_events.erase(iter);

                return false;
            }
        }

        switch (windowEvent.type)
        {

            case DestroyNotify:
            {
                // #TODO
                Debug::log << "TODO: Destroy!" << endl;
                break;
            }

            case FocusIn:
            {
                if (m_inputContext)
                    XSetICFocus(m_inputContext);

                XWMHints* hints = XGetWMHints(m_xDisplay, m_xWindow);
                if (hints != NULL)
                {
                    hints->flags &= ~XUrgencyHint;
                    XSetWMHints(m_xDisplay, m_xWindow, hints);
                    XFree(hints);
                }

                processWindowFocusChanged();

                break;
            }

            case FocusOut:
            {
                if (m_inputContext)
                    XUnsetICFocus(m_inputContext);


                processWindowFocusChanged();
                break;
            }

            case ConfigureNotify:
            {
                if ((windowEvent.xconfigure.width != m_previousSize.x) || (windowEvent.xconfigure.height != m_previousSize.y))
                {
                    processWindowSizeChanged();

                    m_previousSize.x = windowEvent.xconfigure.width;
                    m_previousSize.y = windowEvent.xconfigure.height;
                }
                break;
            }

            case ClientMessage:
            {
                static Atom wmProtocols = GetAtom("WM_PROTOCOLS");

                if (windowEvent.xclient.message_type == wmProtocols)
                {
                    static Atom wmDeleteWindow = GetAtom("WM_DELETE_WINDOW");
                    static Atom netWmPing = IsEWMHSupported() ? GetAtom("_NET_WM_PING", true) : 0;

                    if ((windowEvent.xclient.format == 32) && (windowEvent.xclient.data.l[0]) == static_cast<long>(wmDeleteWindow))
                    {
                        processWindowWillClose();
                    }
                    else
                    if (netWmPing && (windowEvent.xclient.format == 32) && (windowEvent.xclient.data.l[0]) == static_cast<long>(netWmPing))
                    {
                        windowEvent.xclient.window = DefaultRootWindow(m_xDisplay);

                        XSendEvent(m_xDisplay, DefaultRootWindow(m_xDisplay), False, SubstructureNotifyMask | SubstructureRedirectMask, &windowEvent);
                    }
                }
                break;
            }

            case KeyPress:
            {
                InputManager* inputManager = InputManager::GetInstancePtr();

                KeyCode key = KeyCode::None;

                for (S32 i = 0; i < 4; ++i)
                {
                    key = ConvertKeySymToKeyCode(XLookupKeysym(&windowEvent.xkey, i));

                    if (key != KeyCode::None)
                        break;
                }

                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyDown;

                event.keyCode = key;

                event.modifiers = InputEventKeyboardModifiers::None;

                if (windowEvent.xkey.state & Mod1Mask)
                    event.modifiers |= InputEventKeyboardModifiers::AltDown;

                if (windowEvent.xkey.state & ControlMask);
                    event.modifiers |= InputEventKeyboardModifiers::ControlDown;

                if (windowEvent.xkey.state & ShiftMask)
                    event.modifiers |= InputEventKeyboardModifiers::ShiftDown;

                if (windowEvent.xkey.state & Mod4Mask)
                    event.modifiers |= InputEventKeyboardModifiers::CommandDown;

                inputManager->generateInputEvent(event);

                if (!XFilterEvent(&windowEvent, 0))
                {
#if (defined(X_HAVE_UTF8_STRING))
                    if (m_inputContext)
                    {
                        Status status;
                        U8 keyBuffer[16];

                        int length = Xutf8LookupString(
                            m_inputContext,
                            &windowEvent.xkey,
                            reinterpret_cast<char*>(keyBuffer),
                            sizeof(keyBuffer),
                            NULL,
                            &status);

                        if (length > 0)
                        {
                            U32 unicode = 0;
                            TextHelper::DecodeUTF8ToUTF32(keyBuffer, keyBuffer + length, unicode, 0);
                            if (unicode != 0)
                            {
                                // #TODO:
                                /*
                                InputEventTextData event;
                                event.type = InputEventTextType::Text;
                                event.text = unicode;
                                inputManager->generateInputEvent(event);
                                */
                            }
                        }
                    }
                    else
#endif
                    {
                        static XComposeStatus status;
                        char keyBuffer[16];
                        if (XLookupString(&windowEvent.xkey, keyBuffer, sizeof(keyBuffer), NULL, &status))
                        {
                            // #TODO:
                            /*
                            InputEventTextData event;
                            event.type = InputEventTextType::Text;
                            event.text = static_cast<Uint32>(keyBuffer[0]);
                            inputManager->generateInputEvent(event);
                            */
                        }
                    }
                }

                updateLastInputTime(windowEvent.xkey.time);

                break;
            }

            case KeyRelease:
            {
                InputManager* inputManager = InputManager::GetInstancePtr();

                KeyCode key = KeyCode::None;

                for (S32 i = 0; i < 4; ++i)
                {
                    key = ConvertKeySymToKeyCode(XLookupKeysym(&windowEvent.xkey, i));

                    if (key != KeyCode::None)
                        break;
                }

                InputEventKeyboardData event;
                event.type = InputEventKeyboardType::KeyUp;

                event.keyCode = key;

                event.modifiers = InputEventKeyboardModifiers::None;

                if (windowEvent.xkey.state & Mod1Mask)
                    event.modifiers |= InputEventKeyboardModifiers::AltDown;

                if (windowEvent.xkey.state & ControlMask);
                    event.modifiers |= InputEventKeyboardModifiers::ControlDown;

                if (windowEvent.xkey.state & ShiftMask)
                    event.modifiers |= InputEventKeyboardModifiers::ShiftDown;

                if (windowEvent.xkey.state & Mod4Mask)
                    event.modifiers |= InputEventKeyboardModifiers::CommandDown;

                inputManager->generateInputEvent(event);

                break;
            }

            case ButtonPress:
            {
                InputManager* inputManager = InputManager::GetInstancePtr();

                U32 button = windowEvent.xbutton.button;
                if ((button == Button1) ||
                    (button == Button2) ||
                    (button == Button3) ||
                    (button == 8) ||
                    (button == 9))
                {
                    InputEventMouseData event;
                    event.type = InputEventMouseType::ButtonDown;

                    event.x = windowEvent.xbutton.x;
                    event.y = windowEvent.xbutton.y;

                    event.buttonId = button - 1;
                    
                    inputManager->generateInputEvent(event);
                }

                updateLastInputTime(windowEvent.xbutton.time);

                break;
            }

            case ButtonRelease:
            {
                InputManager* inputManager = InputManager::GetInstancePtr();

                unsigned int button = windowEvent.xbutton.button;

                if ((button == Button1) ||
                    (button == Button2) ||
                    (button == Button3) ||
                    (button == 8) ||
                    (button == 9))
                {
                    InputEventMouseData event;
                    event.type = InputEventMouseType::ButtonUp;

                    event.x = windowEvent.xbutton.x;
                    event.y = windowEvent.xbutton.y;

                    event.buttonId = button - 1;

                    inputManager->generateInputEvent(event);
                }
                else
                if ((button == Button4) || (button == Button5))
                {
                    InputEventMouseData event;
                    event.type = InputEventMouseType::Wheel;

                    event.z = (button == Button4) ? 1 : -1;
                    event.x = windowEvent.xbutton.x;
                    event.y = windowEvent.xbutton.y;
                    inputManager->generateInputEvent(event);
                }
                else
                if ((button == 6) || (button == 7))
                {
                    InputEventMouseData event;
                    event.type = InputEventMouseType::Wheel;
                    event.z = (button == 6) ? 1 : -1;
                    event.x = windowEvent.xbutton.x;
                    event.y = windowEvent.xbutton.y;
                    inputManager->generateInputEvent(event);
                }
                break;
            }

            case MotionNotify:
            {
                InputManager* inputManager = InputManager::GetInstancePtr();

                InputEventMouseData event;
                event.type = InputEventMouseType::Move;
                event.x = windowEvent.xmotion.x;
                event.y = windowEvent.xmotion.y;
                inputManager->generateInputEvent(event);
                break;
            }

            case EnterNotify:
            {
                if (windowEvent.xcrossing.mode == NotifyNormal)
                {
                    
                }
                break;
            }

            case LeaveNotify:
            {
                if (windowEvent.xcrossing.mode == NotifyNormal)
                {
                    
                }
                break;
            }

            case UnmapNotify:
            {
                if (windowEvent.xunmap.window == m_xWindow)
                    m_windowMapped = false;

                break;
            }

            case VisibilityNotify:
            {
                if (windowEvent.xvisibility.window == m_xWindow)
                {
                    if (windowEvent.xvisibility.state != VisibilityFullyObscured)
                        m_windowMapped = true;
                }

                break;
            }

            case PropertyNotify:
            {
                if (!m_lastInputTime)
                    m_lastInputTime = windowEvent.xproperty.time;

                break;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    void WindowLinux::grabFocus()
    {
        Atom netActiveWindow = 0;

        if (IsEWMHSupported())
            netActiveWindow = GetAtom("_NET_ACTIVE_WINDOW");

        XWindowAttributes attr;

        XGetWindowAttributes(m_xDisplay, m_xWindow, &attr);

        if (attr.map_state == IsUnmapped)
            return;

        if (netActiveWindow)
        {
            XEvent event;
            std::memset(&event, 0, sizeof(event));

            event.type = ClientMessage;
            event.xclient.window = m_xWindow;
            event.xclient.format = 32;
            event.xclient.message_type = netActiveWindow;
            event.xclient.data.l[0] = 1;
            event.xclient.data.l[1] = m_lastInputTime;
            event.xclient.data.l[2] = 0;

            int result = XSendEvent(
                m_xDisplay,
                DefaultRootWindow(m_xDisplay),
                False,
                SubstructureNotifyMask | SubstructureRedirectMask,
                &event);

            XFlush(m_xDisplay);

            MAZE_ERROR_IF(!result, "Setting fullscreen failed, could not send \"_NET_ACTIVE_WINDOW\" event");
        }
        else
        {
            XRaiseWindow(m_xDisplay, m_xWindow);
            XSetInputFocus(m_xDisplay, m_xWindow, RevertToPointerRoot, CurrentTime);
            XFlush(m_xDisplay);
        }
    }

    //////////////////////////////////////////
    void WindowLinux::updateLastInputTime(::Time _time)
    {
        if (_time && (_time != m_lastInputTime))
        {
            Atom netWmUserTime = GetAtom("_NET_WM_USER_TIME", true);

            if (netWmUserTime)
            {
                XChangeProperty(
                    m_xDisplay,
                    m_xWindow,
                    netWmUserTime,
                    XA_CARDINAL,
                    32,
                    PropModeReplace,
                    reinterpret_cast<unsigned char const*>(&_time),
                    1);
            }

            m_lastInputTime = _time;
        }
    }


} // namespace Maze
//////////////////////////////////////////
