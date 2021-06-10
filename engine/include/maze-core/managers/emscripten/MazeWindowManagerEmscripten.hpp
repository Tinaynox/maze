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
#if (!defined(_MazeWindowManagerEmscripten_hpp_))
#define _MazeWindowManagerEmscripten_hpp_


//////////////////////////////////////////
#include "maze-core/managers/unix/MazeWindowManagerUnix.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(WindowEmscripten);
    
    
    //////////////////////////////////////////
    // Class WindowManagerEmscripten
    //
    //////////////////////////////////////////
    class MAZE_CORE_API WindowManagerEmscripten
        : public WindowManagerUnix
    {
    private:

        //////////////////////////////////////////
        friend class WindowManager;

    public:

        //////////////////////////////////////////
        virtual ~WindowManagerEmscripten();

        //////////////////////////////////////////
        virtual void updateDisplaysList() MAZE_OVERRIDE;
        
        
        //////////////////////////////////////////
        WindowEmscriptenPtr const& getWindow() const { return m_window; }
        
    protected:

        //////////////////////////////////////////
        WindowManagerEmscripten();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool processWindowCanBeCreated(WindowParamsPtr const& _params) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        virtual bool processWindowCreated(WindowPtr const& _window) MAZE_OVERRIDE;
        
    protected:
        WindowEmscriptenPtr m_window;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeWindowManagerEmscripten_hpp_
//////////////////////////////////////////
