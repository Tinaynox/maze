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
#include "MazePhysics2DHeader.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class BoxCollider2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(BoxCollider2D, Collider2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2DF, offset, Vec2DF::c_zero, getOffset, setOffset),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2DF, size, Vec2DF::c_one, getSize, setSize));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(BoxCollider2D);


    //////////////////////////////////////////
    BoxCollider2D::BoxCollider2D()
        : m_offset(Vec2DF::c_zero)
        , m_size(Vec2DF::c_one)
    {

    }

    //////////////////////////////////////////
    BoxCollider2D::~BoxCollider2D()
    {

    }

    //////////////////////////////////////////
    BoxCollider2DPtr BoxCollider2D::Create()
    {
        BoxCollider2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(BoxCollider2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool BoxCollider2D::init()
    {
        if (!Collider2D::init())
            return false;

        return true;
    }

    //////////////////////////////////////////
    void BoxCollider2D::processComponentAdded()
    {
        Collider2D::processComponentAdded();
    }


} // namespace Maze
//////////////////////////////////////////
