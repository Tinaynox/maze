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
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Bounds2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Bounds2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(AABB2D, bounds, AABB2D(), getBounds, setBounds));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Bounds2D);

    //////////////////////////////////////////
    Bounds2D::Bounds2D()
        : m_bounds(Maze::AABB2D::c_zero)
        , m_flags(BoundsDirty)
    {
    }

    //////////////////////////////////////////
    Bounds2D::~Bounds2D()
    {

    }

    //////////////////////////////////////////
    Bounds2DPtr Bounds2D::Create()
    {
        Bounds2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Bounds2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Bounds2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    AABB2D const& Bounds2D::getBounds()
    {
        if (m_flags & BoundsDirty)
            return calculateBounds();
        else
            return m_bounds;
    }

    //////////////////////////////////////////
    AABB2D const& Bounds2D::calculateBounds()
    {
        m_bounds = m_transform->calculateWorldAABB();
        m_flags &= ~BoundsDirty;

        eventBoundsCalculated(m_bounds);

        return m_bounds;
    }

    //////////////////////////////////////////
    void Bounds2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        dirtyBounds();
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Bounds2DSystem, {},
        UpdateEvent const& _event,
        Entity* _entity,
        Bounds2D* _bounds)
    {
        if (_bounds->getTransform()->isWorldTransformChanged())
            _bounds->dirtyBounds();
    }

    
} // namespace Maze
//////////////////////////////////////////
