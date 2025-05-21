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
#if (!defined(_MazeSerializationIndex_hpp_))
#define _MazeSerializationIndex_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SerializationIndex);


    //////////////////////////////////////////
    // Class SerializationIndex
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SerializationIndex
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SerializationIndex, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SerializationIndex);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SerializationIndex();

        //////////////////////////////////////////
        static SerializationIndexPtr Create(S32 _index = -1);


        //////////////////////////////////////////
        inline S32 getSerializationIndex() const { return m_index; }

        //////////////////////////////////////////
        inline void setSerializationIndex(S32 _index) { m_index = _index; }


    protected:

        //////////////////////////////////////////
        SerializationIndex();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(S32 _index);

    protected:
        S32 m_index = -1;
    };



} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSerializationIndex_hpp_
//////////////////////////////////////////
