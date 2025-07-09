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
#if (!defined(_MazeSystemCursorManagerWin_hpp_))
#define _MazeSystemCursorManagerWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/managers/MazeSystemCursorManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemCursorManagerWin
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemCursorManagerWin
        : public SystemCursorManager
    {
    private:

        //////////////////////////////////////////
        friend class SystemCursorManager;

    public:

        //////////////////////////////////////////
        virtual ~SystemCursorManagerWin();


        //////////////////////////////////////////
        virtual SystemCursorPtr createSystemCursor() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void showSystemCursor() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void hideSystemCursor() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setSystemCursorPosition(Vec2F _posSystemScreen) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2F getSystemCursorPosition() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SystemCursorManagerWin();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual SystemCursorPtr const& createBuiltinSystemCursor(BuiltinSystemCursorType _systemCursorType) MAZE_OVERRIDE;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemCursorManagerWin_hpp_
//////////////////////////////////////////
