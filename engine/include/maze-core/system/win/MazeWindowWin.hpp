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
#if (!defined(_MazeWindowWin_hpp_))
#define _MazeWindowWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"


//////////////////////////////////////////
namespace Maze
{
    ///////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowWin);
    MAZE_USING_SHARED_PTR(WindowParamsWin);


    //////////////////////////////////////////
    // Class WindowParamsWin
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowParamsWin
        : public WindowParams
    {
    public:

        //////////////////////////////////////////
        friend class WindowParams;

    public:


    protected:
        WindowParamsWin();

        ////////////////////////////////////
        virtual bool init(WindowParamsPtr const& _windowParams);

    };
    
    
    //////////////////////////////////////////
    // Class WindowWin
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowWin
        : public Window
        , public Updatable
    {
    private:

        //////////////////////////////////////////
        friend class Window;
        friend class WindowManagerWin;

    public:

        //////////////////////////////////////////
        virtual ~WindowWin();

        //////////////////////////////////////////
        virtual void update(F32 dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool processEvent(UINT message, WPARAM wParam, LPARAM lParam);

        //////////////////////////////////////////
        virtual bool isOpened() MAZE_OVERRIDE;
        
        ////////////////////////////////////
        virtual void setClientSize(Vec2DU const& _size) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2DU getClientSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2DU getFullSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setPosition(Vec2DS const& _position) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2DS getPosition() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void close() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool getFocused() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setFocused(bool _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void maximizeFullscreen() MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline HWND getHandle() const { return m_handle; }

        //////////////////////////////////////////
        virtual DisplayPtr const& getRelatedDisplay() MAZE_OVERRIDE;

    protected:


        //////////////////////////////////////////
        WindowWin();

        //////////////////////////////////////////
        virtual bool init(WindowParamsPtr const& params);

        //////////////////////////////////////////
        virtual bool updateTitle() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateWindowMode() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool updateMinimized() MAZE_OVERRIDE;

        //////////////////////////////////////////
        U32 getWindowStyle(WindowMode mode);

        //////////////////////////////////////////
        U32 getWindowExStyle(WindowMode mode);

        //////////////////////////////////////////
        bool updateWindowStyle();

        //////////////////////////////////////////
        void updateClientSize();

        //////////////////////////////////////////
        bool registerClass();

        //////////////////////////////////////////
        bool createWindow();

    protected:
        HWND m_handle;

        U16 m_surrogateCharacter;

        bool m_clientSizeDirty;

        PAINTSTRUCT m_paintStruct;

        String m_className;
    };
    
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeWindowWin_hpp_
//////////////////////////////////////////
