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
#if (!defined(_MazeWindowManager_hpp_))
#define _MazeWindowManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowManager);
    MAZE_USING_SHARED_PTR(Display);
    MAZE_USING_SHARED_PTR(Window);


    //////////////////////////////////////////
    // Class WindowManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowManager
        : public SharedObject<WindowManager>
    {
    public:

        //////////////////////////////////////////
        using DisplaysList = Vector<DisplayPtr>;

        //////////////////////////////////////////
        using WindowsList = Vector<WindowWPtr>;

    public:

        //////////////////////////////////////////
        virtual ~WindowManager();

        //////////////////////////////////////////
        static void Initialize(
            WindowManagerPtr& _windowManager,
            DataBlock const& _config = DataBlock::c_empty);

        //////////////////////////////////////////
        DisplayPtr const& getPrimaryDisplay() const;

        //////////////////////////////////////////
        WindowVideoMode getPrimaryDisplayVideoMode() const;

        //////////////////////////////////////////
        DisplaysList const& getDisplays() const { return m_displays; }

        //////////////////////////////////////////
        WindowsList const& getWindows() const { return m_windows; }

        //////////////////////////////////////////
        Window* getFirstOpenedWindow() const;

        //////////////////////////////////////////
        virtual void updateDisplaysList() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool processWindowCanBeCreated(WindowParamsPtr const& _params);

        //////////////////////////////////////////
        virtual bool processWindowCreated(WindowPtr const& _window);

        //////////////////////////////////////////
        virtual void processWindowDestroyed(Window const* _window);

        //////////////////////////////////////////
        String constructDisplaysInfo();

        //////////////////////////////////////////
        String constructWindowsInfo();


        //////////////////////////////////////////
        static inline WindowManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline WindowManager& GetInstance() { return *s_instance; }


    protected:

        //////////////////////////////////////////
        WindowManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);


    protected:
        static WindowManager* s_instance;

        DisplaysList m_displays;
        WindowsList m_windows;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeWindowManager_hpp_
//////////////////////////////////////////
