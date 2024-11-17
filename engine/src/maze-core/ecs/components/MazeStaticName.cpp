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
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class StaticName
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(StaticName, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(StaticName);

    //////////////////////////////////////////
    StaticName::StaticName()
    {
    }

    //////////////////////////////////////////
    StaticName::~StaticName()
    {
    }

    //////////////////////////////////////////
    StaticNamePtr StaticName::Create(CString _name)
    {
        StaticNamePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(StaticName, object, init(_name));
        return object;
    }

    //////////////////////////////////////////
    bool StaticName::init(CString _name)
    {
        m_name = _name;

        return true;
    }

    //////////////////////////////////////////
    bool StaticName::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _copyData))
            return false;

        m_name = _component->castRaw<StaticName>()->getName();

        return true;
    }

    
} // namespace Maze
//////////////////////////////////////////
