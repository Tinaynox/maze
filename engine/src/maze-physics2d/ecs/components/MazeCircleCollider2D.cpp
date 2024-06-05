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
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class CircleCollider2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(CircleCollider2D, Collider2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, offset, Vec2F::c_zero, getOffset, setOffset),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, radius, 0.5f, getRadius, setRadius));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(CircleCollider2D);


    //////////////////////////////////////////
    CircleCollider2D::CircleCollider2D()
        : m_offset(Vec2F::c_zero)
        , m_radius(0.5f)
    {

    }

    //////////////////////////////////////////
    CircleCollider2D::~CircleCollider2D()
    {

    }

    //////////////////////////////////////////
    CircleCollider2DPtr CircleCollider2D::Create()
    {
        CircleCollider2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CircleCollider2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool CircleCollider2D::init()
    {
        if (!Collider2D::init())
            return false;

        return true;
    }



} // namespace Maze
//////////////////////////////////////////
