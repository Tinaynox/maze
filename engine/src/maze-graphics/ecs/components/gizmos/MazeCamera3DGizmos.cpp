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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeCamera3DGizmos.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Camera3DGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Camera3DGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Camera3DGizmos);


    //////////////////////////////////////////
    Camera3DGizmos::Camera3DGizmos()
    {

    }

    //////////////////////////////////////////
    Camera3DGizmos::~Camera3DGizmos()
    {

    }

    //////////////////////////////////////////
    Camera3DGizmosPtr Camera3DGizmos::Create()
    {
        Camera3DGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Camera3DGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Camera3DGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Camera3DGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        Camera3D* camera3D = _component->castRaw<Camera3D>();

        if (camera3D->getRenderMask() & (S32)DefaultRenderMask::Gizmos)
            return;

        Transform3DPtr const& transform3D = camera3D->getTransform();

        Vec3DF const& position = transform3D->getWorldPosition();

        _drawer->drawBillboard(
            position,
            GizmosManager::GetInstancePtr()->getDefaultGizmosSprite(DefaultGizmosSprite::CameraGizmo),
            ColorF128::c_white);
    }

    //////////////////////////////////////////
    void Camera3DGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component, 
        GizmosDrawer* _drawer)
    {
        Camera3D* camera3D = _component->castRaw<Camera3D>();

        if (camera3D->getRenderMask() & (S32)DefaultRenderMask::Gizmos)
            return;

        Transform3DPtr const& transform3D = camera3D->getTransform();

        Vec3DF position = transform3D->getWorldPosition();

        Mat4DF mat = transform3D->getWorldTransform();
        mat[0][3] = 0.0f;
        mat[1][3] = 0.0f;
        mat[2][3] = 0.0f;

        Vec3DF directionForward = mat.transformAffine(Vec3DF::c_unitZ).normalizedCopy();

        _drawer->setColor(ColorF128::c_lightGray);

        F32 aspectRatio = camera3D->getAspectRatio();
        F32 halfFOVY = camera3D->getFOV() * 0.5f;
        F32 halfFOVX = atan(aspectRatio * tan(halfFOVY));

        F32 tanHalfFOVY = tanf(halfFOVY);
        F32 tanHalfFOVX = tanf(halfFOVX);

        F32 halfNearH = tanHalfFOVY * camera3D->getNearZ();
        F32 halfNearW = tanHalfFOVX * camera3D->getNearZ();
        Vec3DF nearLB = transform3D->getWorldTransform().transformAffine(Vec3DF(-halfNearW, -halfNearH, camera3D->getNearZ()));
        Vec3DF nearLT = transform3D->getWorldTransform().transformAffine(Vec3DF(-halfNearW, +halfNearH, camera3D->getNearZ()));
        Vec3DF nearRT = transform3D->getWorldTransform().transformAffine(Vec3DF(+halfNearW, +halfNearH, camera3D->getNearZ()));
        Vec3DF nearRB = transform3D->getWorldTransform().transformAffine(Vec3DF(+halfNearW, -halfNearH, camera3D->getNearZ()));

        F32 halfFarH = tanHalfFOVY * camera3D->getFarZ();
        F32 halfFarW = tanHalfFOVX * camera3D->getFarZ();
        Vec3DF farLB = transform3D->getWorldTransform().transformAffine(Vec3DF(-halfFarW, -halfFarH, camera3D->getFarZ()));
        Vec3DF farLT = transform3D->getWorldTransform().transformAffine(Vec3DF(-halfFarW, +halfFarH, camera3D->getFarZ()));
        Vec3DF farRT = transform3D->getWorldTransform().transformAffine(Vec3DF(+halfFarW, +halfFarH, camera3D->getFarZ()));
        Vec3DF farRB = transform3D->getWorldTransform().transformAffine(Vec3DF(+halfFarW, -halfFarH, camera3D->getFarZ()));

        _drawer->drawLine(nearLB, nearLT);
        _drawer->drawLine(nearLT, nearRT);
        _drawer->drawLine(nearRT, nearRB);
        _drawer->drawLine(nearRB, nearLB);

        _drawer->drawLine(farLB, farLT);
        _drawer->drawLine(farLT, farRT);
        _drawer->drawLine(farRT, farRB);
        _drawer->drawLine(farRB, farLB);

        _drawer->drawLine(nearLB, farLB);
        _drawer->drawLine(nearLT, farLT);
        _drawer->drawLine(nearRT, farRT);
        _drawer->drawLine(nearRB, farRB);
    }

} // namespace Maze
//////////////////////////////////////////
