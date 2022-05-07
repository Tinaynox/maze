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
#include "maze-physics2d/ecs/components/gizmos/MazeCircleCollider2DGizmos.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class CircleCollider2DGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(CircleCollider2DGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(CircleCollider2DGizmos);


    //////////////////////////////////////////
    CircleCollider2DGizmos::CircleCollider2DGizmos()
    {

    }

    //////////////////////////////////////////
    CircleCollider2DGizmos::~CircleCollider2DGizmos()
    {

    }

    //////////////////////////////////////////
    CircleCollider2DGizmosPtr CircleCollider2DGizmos::Create()
    {
        CircleCollider2DGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CircleCollider2DGizmos, object, init());
        return object;
    }

    ///////////////////////////////////////////
    bool CircleCollider2DGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void CircleCollider2DGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {

    }

    //////////////////////////////////////////
    void CircleCollider2DGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        CircleCollider2D* circleCollider = _component->castRaw<CircleCollider2D>();

        Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
        if (transform3D)
        {
            F32 radius = circleCollider->getRadius();
            Vec2DF offset = circleCollider->getOffset();

            Vec2DF scale = transform3D->getWorldTransform().getAffineScale2D();
            F32 maxScale = Math::Max(scale.x, scale.y);

            _drawer->setColor(ColorF128::c_green);
            _drawer->drawWireCircle(
                transform3D->getWorldPosition(Vec3DF(offset, 0.0f)),
                transform3D->getWorldForwardDirection(),
                maxScale * radius);
        }
    }

} // namespace Maze
//////////////////////////////////////////
