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
#if (!defined(_MazeName_hpp_))
#define _MazeName_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Name);


    //////////////////////////////////////////
    // Class Name
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Name
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Name, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Name);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Name();

        //////////////////////////////////////////
        static NamePtr Create(String const& _name = String());


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }

    protected:

        //////////////////////////////////////////
        Name();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(String const& _name);

    protected:
        String m_name;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeName_hpp_
//////////////////////////////////////////
