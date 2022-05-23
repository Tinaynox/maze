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
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolScale.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolConfig.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    GizmoToolScalePtr GizmoToolScale::Create()
    {
        return MAZE_CREATE_SHARED_PTR(GizmoToolScale);
    }

    //////////////////////////////////////////
    void GizmoToolScale::manipulate(Set<EntityPtr> const& _entities, Vec2DF const& _cursorPos)
    {
        Camera3DPtr const& camera = GizmosManager::GetInstancePtr()->getCamera();
        if (!camera)
            return;

        if (_entities.empty())
            return;

        // #TODO: Multiselection
        EntityPtr const& entity = *_entities.begin();
        Transform3D* entityTransform = entity->getComponentRaw<Transform3D>();
        if (!entityTransform)
            return;

        Mat4DF mat = entityTransform->getWorldTransform();

        Vec3DF const& cameraWorldPosition = camera->getTransform()->getWorldPosition();

        Vec3DF right = { mat[0][0], mat[1][0], mat[2][0] };
        Vec3DF up = { mat[0][1], mat[1][1], mat[2][1] };
        Vec3DF forward = { mat[0][2], mat[1][2], mat[2][2] };
        Vec3DF pos = { mat[0][3], mat[1][3], mat[2][3] };

        //Vec3DF affineScale = mat.getAffineScaleSignless();
        Vec3DF affineScale = entityTransform->getWorldScale();

        F32 cameraDistance = (pos - camera->getTransform()->getLocalPosition()).length();
        F32 scale = cameraDistance * GizmoToolConfig::c_cameraScalePerDistance;
        Mat4DF transform =
            mat *
            Mat4DF::CreateScaleMatrix(scale / affineScale);
        Mat4DF basisTransform = transform;
        basisTransform[0][3] = 0.0f;
        basisTransform[1][3] = 0.0f;
        basisTransform[2][3] = 0.0f;

        GizmosDrawer::MeshRenderMode const renderMode = GizmosDrawer::MeshRenderMode::TransparentTop;

        F32 const length = GizmoToolConfig::c_transformGizmoToolLength;

        auto drawAxis = [&, this](
            ColorF128 const& _color,
            Vec3DF const& _axis)
        {
            GizmosHelper::SetColor(_color);

            F32 len = length;
            if (m_usingAxis >= 0)
            {
                // if (getWorldAxis(m_usingAxis) == _axis)
                //     len += m_deltaLength / scale;
            }

            GizmosHelper::DrawCylinder(
                _axis * ((len - GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize * 0.5f) * 0.5f + GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize * 0.5f),
                _axis,
                GizmoToolConfig::c_transformGizmoToolArrowLineRadius,
                len - GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize * 0.5f,
                _color,
                0.0f,
                renderMode);
            GizmosHelper::DrawCube(
                _axis * (len + GizmoToolConfig::c_transformGizmoToolArrowCubeSize * 0.5f),
                _axis,
                _axis.perpendicular(),
                Vec3DF(GizmoToolConfig::c_transformGizmoToolArrowCubeSize),
                _color,
                0.0f,
                renderMode);
        };

        auto drawCenterAll = [&, this](ColorF128 const& _color)
        {
            GizmosHelper::DrawCube(
                Vec3DF::c_zero,
                Vec3DF::c_unitZ,
                Vec3DF::c_unitY,
                Vec3DF(GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize),
                _color,
                0.0f,
                renderMode);
        };

        Ray ray = camera->convertViewportCoordsToRay(_cursorPos);

        auto checkAxis = [&](
            Vec3DF const& _axis)
        {
            /*
            GizmosHelper::DrawCylinder(
                transform.transformAffine(_axis * length * 0.5f),
                basisTransform.transformAffine(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * length,
                ColorF128::c_cyan,
                0.0f,
                renderMode);

            GizmosHelper::DrawCube(
                transform.transformAffine(_axis * (length + GizmoToolConfig::c_transformGizmoToolArrowCubeSize * 0.5f)),
                basisTransform.transformAffine(_axis).normalizedCopy(),
                basisTransform.transformAffine(_axis.perpendicular()).normalizedCopy(),
                Vec3DF(scale * GizmoToolConfig::c_transformGizmoToolArrowCubeSize),
                ColorF128::c_cyan,
                0.0f,
                renderMode);
            */            

            F32 dist = 0.0;
            if (!Math::RaycastCube(
                ray.getPoint(),
                ray.getDirection(),
                transform.transformAffine(Vec3DF::c_zero),
                basisTransform.transformAffine(Vec3DF::c_unitZ).normalizedCopy(),
                basisTransform.transformAffine(Vec3DF::c_unitY).normalizedCopy(),
                Vec3DF(scale * GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize),
                dist))
            {
                if (Math::RaycastCylinder(
                    ray.getPoint(),
                    ray.getDirection(),
                    transform.transformAffine(_axis * length * 0.5f),
                    basisTransform.transformAffine(_axis).normalizedCopy(),
                    scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                    scale * length,
                    dist))
                    return true;
            }

            if (Math::RaycastCube(
                ray.getPoint(),
                ray.getDirection(),
                transform.transformAffine(_axis * (length + GizmoToolConfig::c_transformGizmoToolArrowCubeSize * 0.5f)),
                basisTransform.transformAffine(_axis).normalizedCopy(),
                basisTransform.transformAffine(_axis.perpendicular()).normalizedCopy(),
                Vec3DF(scale * GizmoToolConfig::c_transformGizmoToolArrowCubeSize * 2.0f),
                dist))
                return true;
            return false;
        };

        auto checkC = [&]()
        {
            F32 dist = 0.0;
            if (Math::RaycastCube(
                ray.getPoint(),
                ray.getDirection(),
                transform.transformAffine(Vec3DF::c_zero),
                basisTransform.transformAffine(Vec3DF::c_unitZ).normalizedCopy(),
                basisTransform.transformAffine(Vec3DF::c_unitY).normalizedCopy(),
                Vec3DF(scale * GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize * 2.0f),
                dist))
                return true;
            return false;
        };

        auto drawX = [&]() { drawAxis(m_selectedAxis == 0 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisXColor, Vec3DF::c_unitX); };
        auto drawY = [&]() { drawAxis(m_selectedAxis == 1 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisYColor, Vec3DF::c_unitY); };
        auto drawZ = [&]() { drawAxis(m_selectedAxis == 2 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisZColor, Vec3DF::c_unitZ); };
        auto drawC = [&]() { drawCenterAll(m_selectedAxis == 3 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : ColorF128::c_lightGray); };

        auto checkX = [&]() { return checkAxis(Vec3DF::c_unitX); };
        auto checkY = [&]() { return checkAxis(Vec3DF::c_unitY); };
        auto checkZ = [&]() { return checkAxis(Vec3DF::c_unitZ); };

        struct Axis
        {
            //////////////////////////////////////////
            Axis(
                S32 _index = -1,
                F32 _sqDist = 0.0f,
                std::function<void()> _drawFunc = nullptr,
                std::function<bool()> _checkFunc = nullptr)
                : index(_index)
                , sqDist(_sqDist)
                , drawFunc(_drawFunc)
                , checkFunc(_checkFunc)
            {}

            S32 index = -1;
            F32 sqDist = 0.0f;
            std::function<void()> drawFunc;
            std::function<bool()> checkFunc;
        };

        Vector<Axis> drawFuncs =
        {
            Axis(0, (pos + right * scale).squaredDistance(cameraWorldPosition), drawX, checkX),
            Axis(1, (pos + up * scale).squaredDistance(cameraWorldPosition), drawY, checkY),
            Axis(2, (pos + forward * scale).squaredDistance(cameraWorldPosition), drawZ, checkZ),
            Axis(3, (pos).squaredDistance(cameraWorldPosition), drawC, checkC)
        };

        std::sort(
            drawFuncs.begin(),
            drawFuncs.end(),
            [](Axis const& _a, Axis const& _b)
        {
            return _a.sqDist > _b.sqDist;
        });

        if (m_usingAxis >= 0)
        {
            Vec3DF norm;
            if (m_usingAxis == 3)
            {
                norm = -camera->getTransform()->getWorldForwardDirection();
            }
            else
            {
                F32 d = 0.0f;
                for (S32 i = 0; i < 3; ++i)
                {
                    if (m_usingAxis == i)
                        continue;

                    Vec3DF crossAxis = basisTransform.transformAffine(getWorldAxis(i)).normalizedCopy();
                    F32 dot = crossAxis.dotProduct(camera->getTransform()->getWorldForwardDirection());
                    if (Math::Abs(dot) > d)
                    {
                        d = Math::Abs(dot);
                        norm = crossAxis;
                    }
                }
            }

            F32 dist;
            if (Math::RaycastPlane(ray.getPoint(), ray.getDirection(), pos, norm, dist))
            {
                Vec3DF point = ray.getPoint(dist);

                Vec3DF axis;

                if (m_usingAxis < 3)
                {
                    axis = basisTransform.transformAffine(getWorldAxis(m_usingAxis)).normalizedCopy();
                    point = Math::ClosestPointOnLine(pos, pos + axis, point);
                    
                }
                else
                {
                    axis = camera->getTransform()->getWorldRightDirection();
                }

                if (m_useRequest)
                {
                    m_useRequest = false;
                    m_startScale = entityTransform->getLocalScale();
                    m_startTransform = mat;
                    m_startPoint = point;
                }
                else
                {
                    
                    Vec3DF delta = point - m_startPoint;

                    if (m_usingAxis == 3)
                    {
                        m_deltaLength = 
                            delta.dotProduct(camera->getTransform()->getWorldRightDirection()) +
                            delta.dotProduct(camera->getTransform()->getWorldUpDirection());
                    }
                    else
                        m_deltaLength = delta.dotProduct(axis);

                    Vec3DF parentWorldScale = entityTransform->getParent() ? entityTransform->getParent()->getWorldScale()
                                                                           : Vec3DF::c_one;
                    Vec3DF newWorldScale = m_deltaLength * getWorldAxis(m_usingAxis) + parentWorldScale * m_startScale;
                    Vec3DF newLocalScale = newWorldScale / parentWorldScale;
                    entityTransform->setLocalScale(newLocalScale);
                }
            }
        }
        else
        {
            m_selectedAxis = -1;
            for (auto it = drawFuncs.rbegin(), end = drawFuncs.rend(); it != end; ++it)
            {
                if (it->checkFunc())
                {
                    m_selectedAxis = it->index;
                    break;
                }
            }
        }

        GizmosHelper::PushTransform(transform);
        for (auto drawFunc : drawFuncs)
            if (drawFunc.drawFunc)
                drawFunc.drawFunc();
        GizmosHelper::PopTransform();
    }

    //////////////////////////////////////////
    void GizmoToolScale::processCursorPress(Vec2DF const& _cursorPos)
    {
        if (m_selectedAxis >= 0)
        {
            m_usingAxis = m_selectedAxis;
            m_useRequest = true;
        }
    }

    //////////////////////////////////////////
    void GizmoToolScale::processCursorRelease()
    {
        m_usingAxis = -1;
        m_selectedAxis = -1;
        m_deltaLength = 0.0f;
    }

    //////////////////////////////////////////
    Vec3DF GizmoToolScale::getWorldAxis(S32 _axis)
    {
        switch (_axis)
        {
            case 0: return Vec3DF::c_unitX;
            case 1: return Vec3DF::c_unitY;
            case 2: return Vec3DF::c_unitZ;
            case 3: return m_startScale.normalizedCopy();
        }

        return Vec3DF::c_zero;
    }

} // namespace Maze
//////////////////////////////////////////
