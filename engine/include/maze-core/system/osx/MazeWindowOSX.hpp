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
#if (!defined(_MazeWindowOSX_hpp_))
#define _MazeWindowOSX_hpp_


//////////////////////////////////////////
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeWindow.hpp"


//////////////////////////////////////////
#ifdef __OBJC__
#   import <AppKit/AppKit.h>

@class NSOpenGLContext;
typedef NSOpenGLContext* NSOpenGLContextRef;

@class ViewOSX;
typedef ViewOSX* ViewOSXRef;

@class WindowDelegateOSX;
typedef WindowDelegateOSX* WindowDelegateOSXRef;

#else

typedef void* NSOpenGLContextRef;
typedef void* ViewOSXRef;
typedef void* WindowDelegateOSXRef;
typedef void* CVDisplayLinkRef;
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowOSX);
    MAZE_USING_SHARED_PTR(WindowParamsOSX);


    //////////////////////////////////////////
    // Class WindowParamsOSX
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowParamsOSX
        : public WindowParams
    {
    public:

        //////////////////////////////////////////
        friend class WindowParams;

    public:


    protected:
        WindowParamsOSX();

        ////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _windowParams);

    };


    //////////////////////////////////////////
    // Class WindowOSX
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowOSX
        : public Window
    {
    public:
        
        //////////////////////////////////////////
        friend class Window;
        
    public:
        
        //////////////////////////////////////////
        virtual bool isOpened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void setClientSize(Vec2U32 const& _size) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual Vec2U32 getClientSize() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual Vec2U32 getFullSize() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void setPosition(Vec2S32 const& _position ) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual Vec2S32 getPosition() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void close() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool getFocused() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual void setFocused(bool _value) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual DisplayPtr const& getRelatedDisplay() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void processWindowDelegateOSXDidResize();
        
        //////////////////////////////////////////
        void processWindowDelegateOSXDidMove();
        
        //////////////////////////////////////////
        void processWindowDelegateOSXWillClose();
        
        //////////////////////////////////////////
        void processWindowDelegateOSXDidBecomeKey();
        
        //////////////////////////////////////////
        void processWindowDelegateOSXDidResignKey();
        
        //////////////////////////////////////////
        inline ViewOSXRef getViewOSX() { return m_viewOSX; }
        
    public:
        
        //////////////////////////////////////////
        virtual ~WindowOSX();
        
        
    protected:
        
        //////////////////////////////////////////
        WindowOSX();
        
        //////////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _params) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool updateTitle() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool updateWindowMode() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool updateMinimized() MAZE_OVERRIDE;
        
        
        //////////////////////////////////////////
        bool createWindowOSX();
        
        //////////////////////////////////////////
        void updateWindowStyle();
        
    protected:
        WindowDelegateOSXRef m_windowDelegateOSX;
        ViewOSXRef m_viewOSX;
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeWindowOSX_hpp_
//////////////////////////////////////////
