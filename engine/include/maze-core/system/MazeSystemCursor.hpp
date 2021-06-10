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
#if (!defined(_MazeSystemCursor_hpp_))
#define _MazeSystemCursor_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class SystemCursor
    //
    //////////////////////////////////////////
    class SystemCursor
    {
    public:

        //////////////////////////////////////////
        virtual ~SystemCursor();

        //////////////////////////////////////////
        void setId(String const& _id) { m_id = _id; }

        //////////////////////////////////////////
        String const& getId() const { return m_id; }

    protected:

        //////////////////////////////////////////
        SystemCursor();

        //////////////////////////////////////////
        virtual bool init(const String& _id);

    protected:
        String m_id;
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeSystemCursor_hpp_
//////////////////////////////////////////
