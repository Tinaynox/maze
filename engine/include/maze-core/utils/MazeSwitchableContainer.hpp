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
#if (!defined(_MazeSwitchableContainer_hpp_))
#define _MazeSwitchableContainer_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SwitchableContainer
    //
    //////////////////////////////////////////
    template <typename TContainer>
    class MAZE_CORE_API SwitchableContainer
    {
    public:

        //////////////////////////////////////////
        inline SwitchableContainer()
            : m_isFirst(true)
        {
        }

        //////////////////////////////////////////
        inline TContainer& current()
        {
            TContainer& currentContainer = m_containers[m_isFirst ? 0 : 1];
            return currentContainer;
        }

        //////////////////////////////////////////
        inline TContainer& other()
        {
            TContainer& currentContainer = m_containers[m_isFirst ? 1 : 0];
            return currentContainer;
        }

        //////////////////////////////////////////
        inline TContainer& switchContainer()
        {
            m_isFirst = !m_isFirst;
            return m_containers[m_isFirst ? 0 : 1];
        }

    protected:
        TContainer m_containers[2];
        bool m_isFirst;
    };


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeSwitchableContainer_hpp_
