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
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Display
    //
    //////////////////////////////////////////
    Display::Display()
        : m_flags(0)
        , m_userData(NULL)
    {
    
    }

    //////////////////////////////////////////
    Display::~Display()
    {
    }

    //////////////////////////////////////////
    DisplayPtr Display::Create(
        String const& _name,
        U64 _handle,
        U32 _flags,
        Vector<WindowVideoMode> const& _fullscreenModes)
    {
        DisplayPtr display;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Display, display, init(_name, _handle, _flags, _fullscreenModes));
        return display;
    }

    //////////////////////////////////////////
    bool Display::init(
        String const& _name,
        U64 _handle,
        U32 _flags,
        Vector<WindowVideoMode> const& _fullscreenModes)
    {
        m_name = _name;
        m_handle = _handle;
        m_flags = _flags;
        m_fullscreenModes = _fullscreenModes;

        return true;
    }

    //////////////////////////////////////////
    WindowVideoMode Display::getCurrentMode()
    {
        return WindowHelper::GetDisplayCurrentMode(this);
    }


} // namespace Maze
//////////////////////////////////////////
