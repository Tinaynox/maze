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
#include "maze-core/utils/MazeEntityPool.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EntityPoolObject
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityPoolObject, Component);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EntityPoolObject);

    //////////////////////////////////////////
    EntityPoolObject::EntityPoolObject()
    {
    }

    //////////////////////////////////////////
    EntityPoolObject::~EntityPoolObject()
    {
    }

    //////////////////////////////////////////
    EntityPoolObjectPtr EntityPoolObject::Create(EntityPoolPtr const& _pool)
    {
        EntityPoolObjectPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EntityPoolObject, object, init(_pool));
        return object;
    }

    //////////////////////////////////////////
    bool EntityPoolObject::init(EntityPoolPtr const& _pool)
    {
        m_pool = _pool;
        return true;
    }

    //////////////////////////////////////////
    void EntityPoolObject::release()
    {
        if (EntityPoolPtr pool = m_pool.lock())
            pool->release(getEntityRaw()->getSharedPtr());
        else
        {
            MAZE_ERROR("Pool is not exists!");
        }
    }


} // namespace Maze
//////////////////////////////////////////
