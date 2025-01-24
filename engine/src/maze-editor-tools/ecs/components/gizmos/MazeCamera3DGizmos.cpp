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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeCamera3DGizmos.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
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

        Vec3F const& position = transform3D->getWorldPosition();

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

        TMat mat = transform3D->getWorldTransform();

        _drawer->setColor(ColorF128::c_lightGray);

        F32 aspectRatio = camera3D->getAspectRatio();
        if (camera3D->getProjectionMode() == CameraProjectionMode::Perspective)
        {
            F32 halfFOVY = camera3D->getFOV() * 0.5f;
            F32 halfFOVX = atan(aspectRatio * tan(halfFOVY));

            F32 tanHalfFOVY = tanf(halfFOVY);
            F32 tanHalfFOVX = tanf(halfFOVX);

            F32 halfNearH = tanHalfFOVY * camera3D->getNearZ();
            F32 halfNearW = tanHalfFOVX * camera3D->getNearZ();
            Vec3F nearLB = transform3D->getWorldTransform().transform(Vec3F(-halfNearW, -halfNearH, camera3D->getNearZ()));
            Vec3F nearLT = transform3D->getWorldTransform().transform(Vec3F(-halfNearW, +halfNearH, camera3D->getNearZ()));
            Vec3F nearRT = transform3D->getWorldTransform().transform(Vec3F(+halfNearW, +halfNearH, camera3D->getNearZ()));
            Vec3F nearRB = transform3D->getWorldTransform().transform(Vec3F(+halfNearW, -halfNearH, camera3D->getNearZ()));

            F32 halfFarH = tanHalfFOVY * camera3D->getFarZ();
            F32 halfFarW = tanHalfFOVX * camera3D->getFarZ();
            Vec3F farLB = transform3D->getWorldTransform().transform(Vec3F(-halfFarW, -halfFarH, camera3D->getFarZ()));
            Vec3F farLT = transform3D->getWorldTransform().transform(Vec3F(-halfFarW, +halfFarH, camera3D->getFarZ()));
            Vec3F farRT = transform3D->getWorldTransform().transform(Vec3F(+halfFarW, +halfFarH, camera3D->getFarZ()));
            Vec3F farRB = transform3D->getWorldTransform().transform(Vec3F(+halfFarW, -halfFarH, camera3D->getFarZ()));

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
        else
        if (camera3D->getProjectionMode() == CameraProjectionMode::Orthographic)
        {
            F32 top = camera3D->getOrthographicSize();
            F32 bottom = -top;
            F32 right = camera3D->getOrthographicSize() * aspectRatio;
            F32 left = -right;

            F32 farZ = camera3D->getFarZ();
            F32 nearZ = camera3D->getNearZ();

            Vec3F rt = mat[3] + mat[0] * right + mat[1] * top;
            Vec3F lt = mat[3] + mat[0] * left + mat[1] * top;
            Vec3F rb = mat[3] + mat[0] * right + mat[1] * bottom;
            Vec3F lb = mat[3] + mat[0] * left + mat[1] * bottom;

            Vec3F nearOffset = mat[2] * nearZ;
            _drawer->drawLine(rt + nearOffset, lt + nearOffset);
            _drawer->drawLine(rt + nearOffset, rb + nearOffset);
            _drawer->drawLine(rb + nearOffset, lb + nearOffset);
            _drawer->drawLine(lb + nearOffset, lt + nearOffset);

            Vec3F farOffset = mat[2] * farZ;
            _drawer->drawLine(rt + farOffset, lt + farOffset);
            _drawer->drawLine(rt + farOffset, rb + farOffset);
            _drawer->drawLine(rb + farOffset, lb + farOffset);
            _drawer->drawLine(lb + farOffset, lt + farOffset);

            _drawer->drawLine(rt + nearOffset, rt + farOffset);
            _drawer->drawLine(lt + nearOffset, lt + farOffset);
            _drawer->drawLine(rb + nearOffset, rb + farOffset);
            _drawer->drawLine(lb + nearOffset, lb + farOffset);
        }
    }

} // namespace Maze
//////////////////////////////////////////
