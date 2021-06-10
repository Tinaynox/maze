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
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Name
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Name, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, name, String(), getName, setName));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Name);

    //////////////////////////////////////////
    Name::Name()
    {
    }

    //////////////////////////////////////////
    Name::~Name()
    {
    }

    //////////////////////////////////////////
    NamePtr Name::Create(String const& _name)
    {
        NamePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Name, object, init(_name));
        return object;
    }

    //////////////////////////////////////////
    bool Name::init(String const& _name)
    {
        setName(_name);

        return true;
    }

    
} // namespace Maze
//////////////////////////////////////////
