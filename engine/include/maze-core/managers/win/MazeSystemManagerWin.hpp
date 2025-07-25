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
#if (!defined(_MazeSystemManagerWin_hpp_))
#define _MazeSystemManagerWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerWin
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SystemManagerWin
        : public SystemManager
    {
    private:

        //////////////////////////////////////////
        friend class SystemManager;

    public:

        //////////////////////////////////////////
        virtual ~SystemManagerWin();

        //////////////////////////////////////////
        virtual bool initApplication() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual String getClipboardAsString() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setClipboardString(String const& _text) MAZE_OVERRIDE;


        

    protected:

        //////////////////////////////////////////
        SystemManagerWin();

        //////////////////////////////////////////
        virtual bool init(Vector<Maze::S8 const*> const& _commandLineArguments) MAZE_OVERRIDE;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemManagerWin_hpp_
//////////////////////////////////////////
