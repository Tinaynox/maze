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
#include "MazePhysics2DEditorToolsHeader.hpp"
#include "maze-plugin-physics2d-editor-tools/ecs/components/gizmos/MazeBoxCollider2DGizmos.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class BoxCollider2DGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(BoxCollider2DGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(BoxCollider2DGizmos);


    //////////////////////////////////////////
    BoxCollider2DGizmos::BoxCollider2DGizmos()
    {

    }

    //////////////////////////////////////////
    BoxCollider2DGizmos::~BoxCollider2DGizmos()
    {

    }

    //////////////////////////////////////////
    BoxCollider2DGizmosPtr BoxCollider2DGizmos::Create()
    {
        BoxCollider2DGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(BoxCollider2DGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool BoxCollider2DGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void BoxCollider2DGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {

    }

    //////////////////////////////////////////
    void BoxCollider2DGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        BoxCollider2D* boxCollider = _component->castRaw<BoxCollider2D>();

        Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
        if (transform3D)
        {
            F32 halfWidth = boxCollider->getSize().x * 0.5f;
            F32 halfHeight = boxCollider->getSize().y * 0.5f;

            Vec3DF positionLB = transform3D->getWorldTransform().transformAffine(
                boxCollider->getOffset() + Vec2DF{ -halfWidth, -halfHeight });
            Vec3DF positionLT = transform3D->getWorldTransform().transformAffine(
                boxCollider->getOffset() + Vec2DF{ -halfWidth, +halfHeight });
            Vec3DF positionRT = transform3D->getWorldTransform().transformAffine(
                boxCollider->getOffset() + Vec2DF{ +halfWidth, +halfHeight });
            Vec3DF positionRB = transform3D->getWorldTransform().transformAffine(
                boxCollider->getOffset() + Vec2DF{ +halfWidth, -halfHeight });

            _drawer->setColor(ColorF128::c_green);
            _drawer->drawLine(positionLB, positionLT);
            _drawer->drawLine(positionLT, positionRT);
            _drawer->drawLine(positionRT, positionRB);
            _drawer->drawLine(positionRB, positionLB);
        }
    }

} // namespace Maze
//////////////////////////////////////////
