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
#if (!defined(_MazeWindow_hpp_))
#define _MazeWindow_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class Window;
    MAZE_USING_SHARED_PTR(Window);
    class Display;
    MAZE_USING_SHARED_PTR(Display);
    MAZE_USING_SHARED_PTR(SystemCursor);


    //////////////////////////////////////////
    enum class WindowMode
    {
        None = 0,
        Windowed,
        WindowedMaximized,
        Fullscreen,
        MAX
    };


    //////////////////////////////////////////
    namespace WindowStyleFlags
    {
        enum Enum
        {
            Titlebar          = MAZE_BIT(0),
            Resizable         = MAZE_BIT(1),
            Background        = MAZE_BIT(2),
            MinimizeButton    = MAZE_BIT(3),
            MaximizeButton    = MAZE_BIT(4),
            CloseButton       = MAZE_BIT(5),
            Topmost           = MAZE_BIT(6)
        };
    }


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowParams);


    //////////////////////////////////////////
    // Class WindowParams
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowParams
        : public SharedObject<WindowParams>
    {
    public:

        //////////////////////////////////////////
        virtual ~WindowParams() {};

        //////////////////////////////////////////
        using Flags = U8;

        //////////////////////////////////////////
        static WindowParamsPtr Create(
            Vec2U _clientSize = Vec2U(640, 480),
            U32 _bpp = 32,
            CString _windowTitle = "Window",
            WindowMode _windowMode = WindowMode::Windowed,
            bool _minimized = false,
            S32 _iconBig = 0,
            S32 _iconSmall = 0,
            Flags _flags = WindowStyleFlags::Titlebar | WindowStyleFlags::CloseButton | WindowStyleFlags::Background);

        //////////////////////////////////////////
        static WindowParamsPtr Create(WindowParamsPtr const& _windowParams);


    public:

        Vec2U clientSize;
        U32 bpp;
        WindowMode windowMode;

        bool minimized;

        String title;

        S32 iconBig;
        S32 iconSmall;
        SystemCursorPtr cursor;

        Flags flags;

    protected:

        //////////////////////////////////////////
        bool init(
            Vec2U _clientSize = Vec2U(640, 480),
            U32 _bpp = 32,
            CString _windowTitle = "Window",
            WindowMode _windowMode = WindowMode::Windowed,
            bool _minimized = false,
            S32 _iconBig = 0,
            S32 _iconSmall = 0,
            Flags _flags = WindowStyleFlags::Titlebar | WindowStyleFlags::CloseButton | WindowStyleFlags::Background);

        //////////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _windowParams);
    };


    //////////////////////////////////////////
    // Class Window
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Window
        : public SharedObject<Window>
    {
    public:

        //////////////////////////////////////////
        virtual ~Window();


        //////////////////////////////////////////
        static WindowPtr Create(WindowParamsPtr const& _params = WindowParamsPtr());

        //////////////////////////////////////////
        inline WindowParamsPtr const& getParams() const { return m_params; }

        //////////////////////////////////////////
        virtual bool isOpened() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool setTitle(String const& _title);

        //////////////////////////////////////////
        inline String const& getTitle() const { return m_params->title; }

        //////////////////////////////////////////
        virtual bool setFullscreen(bool _fullscreen);

        //////////////////////////////////////////
        inline bool getFullscreen() const { return m_params->windowMode == WindowMode::Fullscreen; }

        //////////////////////////////////////////
        virtual bool setMinimized(bool _minimized);

        //////////////////////////////////////////
        virtual bool getMinimized() const { return m_params->minimized; }

        //////////////////////////////////////////
        virtual bool setFlags(WindowParams::Flags _flags);

        //////////////////////////////////////////
        virtual WindowParams::Flags getFlags() const { return m_params->flags; }

        //////////////////////////////////////////
        inline WindowMode getWindowMode() const { return m_params->windowMode; }

        //////////////////////////////////////////
        virtual void maximizeFullscreen();

        //////////////////////////////////////////
        virtual void setClientSize(Vec2U const& _size) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Vec2U getClientSize() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Vec2U getFullSize() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setPosition(Vec2S const& _position) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Vec2S getPosition() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void close() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool getFocused() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void setFocused(bool _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual DisplayPtr const& getRelatedDisplay() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual Vec2F convertSystemScreenCoordsToWindowCoords(Vec2F const& _posDS) { return _posDS; }

        //////////////////////////////////////////
        virtual Vec2F convertWindowCoordsToSystemScreenCoords(Vec2F const& _posWS) { return _posWS; }

    public:

        MultiDelegate<Window*> eventWindowCreated;
        MultiDelegate<Window*> eventWindowPositionChanged;
        MultiDelegate<Window*> eventWindowSizeChanged;
        MultiDelegate<Window*> eventWindowWillClose;
        MultiDelegate<Window*> eventWindowClosed;
        MultiDelegate<Window*> eventWindowFocusChanged;
        MultiDelegate<Window*> eventWindowModeChanged;
        MultiDelegate<Window*> eventDestroyed;

    protected:

        //////////////////////////////////////////
        Window();

        //////////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _params);


        //////////////////////////////////////////
        virtual bool updateTitle() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool updateWindowMode() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool updateMinimized() MAZE_ABSTRACT;

        //////////////////////////////////////////
        void processWindowCreated();

        //////////////////////////////////////////
        void processWindowPositionChanged();

        //////////////////////////////////////////
        void processWindowSizeChanged();

        //////////////////////////////////////////
        void processWindowWillClose();

        //////////////////////////////////////////
        void processWindowClosed();

        //////////////////////////////////////////
        void processWindowFocusChanged();

    protected:

        WindowParamsPtr m_params;

        Vector<SystemCursorPtr> m_systemCursors;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeWindow_hpp_
//////////////////////////////////////////
