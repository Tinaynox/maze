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
#if (!defined(_MazeWindowLinux_hpp_))
#define _MazeWindowLinux_hpp_


//////////////////////////////////////////
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <functional>


//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)

#    if (defined(None))
#        undef None
#    endif

#endif // (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
//////////////////////////////////////////



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    ::Display* RetainSharedXDisplay();

    //////////////////////////////////////////
    void ReleaseSharedXDisplay(::Display* _xDisplay);

    //////////////////////////////////////////
    Atom GetAtom(String const& _name, bool _onlyIfExists = false);

    //////////////////////////////////////////
    ::Window GetParentWindow(::Display* _disp, ::Window _win);

    //////////////////////////////////////////
    bool GetEWMHFrameExtents(
        ::Display* _disp,
        ::Window _win,
        long& _xFrameExtent,
        long& _yFrameExtent);

    //////////////////////////////////////////
    String FindExecutableName();

    //////////////////////////////////////////
    bool IsWMAbsolutePositionGood();


    ///////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowLinux);
    MAZE_USING_SHARED_PTR(WindowParamsLinux);


    //////////////////////////////////////////
    // Class WindowParamsLinux
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowParamsLinux
        : public WindowParams
    {
    public:

        //////////////////////////////////////////
        friend class WindowParams;

    public:

        using FetchXVisualInfoCallback = std::function<XVisualInfo(WindowLinux*)>;
        FetchXVisualInfoCallback fetchVisualInfoCallback;

    protected:
        WindowParamsLinux();

        ////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _windowParams);

    };


    //////////////////////////////////////////
    // Class WindowLinux
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowLinux
        : public Window
        , public MultiDelegateCallbackReceiver
        , public Updatable
    {
    private:

        //////////////////////////////////////////
        friend class Window;

    public:

        //////////////////////////////////////////
        virtual ~WindowLinux();

        //////////////////////////////////////////
        virtual void update(F32 _dt);

        //////////////////////////////////////////
        virtual bool isOpened() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void setClientSize(Vec2U32 const& _size) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2U32 getClientSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2U32 getFullSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setPosition(Vec2S32 const& _position) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2S32 getPosition() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void close() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool getFocused() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setFocused(bool _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void maximizeFullscreen() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual DisplayPtr const& getRelatedDisplay() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline ::Window getXWindow() const { return m_xWindow; }

        //////////////////////////////////////////
        inline ::Display* getXDisplay() const { return m_xDisplay; }

        //////////////////////////////////////////
        void processEvents();

    protected:

        //////////////////////////////////////////
        WindowLinux();

        //////////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _params) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateTitle() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateWindowMode() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateMinimized() MAZE_OVERRIDE;


        //////////////////////////////////////////
        Vec2S32 getPrimaryMonitorPosition();

        //////////////////////////////////////////
        bool checkXRandR(S32& _xRandRMajor, S32& _xRandRMinor);

        ////////////////////////////////////////////////////////////
        RROutput getOutputPrimary(::Window& _rootWindow, XRRScreenResources* _res, S32 _xRandRMajor, S32 _xRandRMinor);

        //////////////////////////////////////////
        void setupProtocols();

        //////////////////////////////////////////
        void initializeWindowLinux();

        //////////////////////////////////////////
        void setXWindowVisible(bool _visible);

        //////////////////////////////////////////
        bool processEvent(XEvent& _windowEvent);

        //////////////////////////////////////////
        void grabFocus();

        //////////////////////////////////////////
        void updateLastInputTime(::Time _time);


    private:

        ::Window m_xWindow;
        ::Display* m_xDisplay;
        S32 m_xScreen;

        bool m_useSizeHints;

        XIM m_inputMethod;
        XIC m_inputContext;

        bool m_windowMapped;

        Deque<XEvent> m_events;

        ::Time m_lastInputTime;

        bool m_keyRepeat;

        Vec2U32 m_previousSize;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeWindowLinux_hpp_
//////////////////////////////////////////
