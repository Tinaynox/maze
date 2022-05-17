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
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolTranslation.hpp"
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
    GizmoToolTranslationPtr GizmoToolTranslation::Create()
    {
        return MAZE_CREATE_SHARED_PTR(GizmoToolTranslation);
    }

    //////////////////////////////////////////
    void GizmoToolTranslation::manipulate(Set<EntityPtr> const& _entities, Vec2DF const& _cursorPos)
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

        Vec3DF affineScale = mat.getAffineScaleSignless(); // #TODO: Signed scale!

        F32 cameraDistance = (pos - camera->getTransform()->getLocalPosition()).length();
        F32 scale = cameraDistance * GizmoToolConfig::c_cameraScalePerDistance;
        Mat4DF transform =
            mat *
            Mat4DF::CreateScaleMatrix(scale / affineScale);
        Mat4DF basisTransform = transform;
        basisTransform[0][3] = 0.0f;
        basisTransform[1][3] = 0.0f;
        basisTransform[2][3] = 0.0f;

        GizmosDrawer::MeshRenderMode const renderMode = GizmosDrawer::MeshRenderMode::Transparent;

        F32 const length = GizmoToolConfig::c_transformGizmoToolLength;

        auto drawAxis = [&](
            ColorF128 const& _color,
            Vec3DF const& _axis)
        {
            GizmosHelper::SetColor(_color);

            GizmosHelper::DrawCylinder(
                _axis * length * 0.5f,
                _axis,
                GizmoToolConfig::c_transformGizmoToolArrowLineRadius,
                length,
                _color,
                0.0f,
                renderMode);
            GizmosHelper::DrawCone(
                _axis * length,
                _axis,
                GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                GizmoToolConfig::c_transformGizmoToolArrowConeHeight,
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

            GizmosHelper::DrawCone(
                transform.transformAffine(_axis * length),
                basisTransform.transformAffine(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeHeight,
                ColorF128::c_cyan,
                0.0f,
                renderMode);
            */

            F32 dist = 0.0;
            if (Math::RaycastCylinder(
                ray.getPoint(),
                ray.getDirection(),
                transform.transformAffine(_axis * length * 0.5f),
                basisTransform.transformAffine(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * length,
                dist))
                return true;
            if (Math::RaycastCone(
                ray.getPoint(),
                ray.getDirection(),
                transform.transformAffine(_axis * length),
                basisTransform.transformAffine(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeHeight,
                dist))
                return true;
            return false;
        };

        auto drawX = [&]() { drawAxis(m_selectedAxis == 0 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisXColor, Vec3DF::c_unitX); };
        auto drawY = [&]() { drawAxis(m_selectedAxis == 1 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisYColor, Vec3DF::c_unitY); };
        auto drawZ = [&]() { drawAxis(m_selectedAxis == 2 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisZColor, Vec3DF::c_unitZ); };

        auto checkX = [&]() { return checkAxis(Vec3DF::c_unitX); };
        auto checkY = [&]() { return checkAxis(Vec3DF::c_unitY); };
        auto checkZ = [&]() { return checkAxis(Vec3DF::c_unitZ); };

        struct Axis
        {
            S32 index = -1;
            F32 sqDist = 0.0f;
            std::function<void()> drawFunc;
            std::function<bool()> checkFunc;
        };

        Vector<Axis> drawFuncs =
        {
            {0, (pos + right).squaredDistance(cameraWorldPosition), drawX, checkX},
            {1, (pos + up).squaredDistance(cameraWorldPosition), drawY, checkY},
            {2, (pos + forward).squaredDistance(cameraWorldPosition), drawZ, checkZ}
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
            Vec3DF axis = basisTransform.transformAffine(getWorldAxis(m_usingAxis)).normalizedCopy();

            Vec3DF norm;
            F32 d = 0.0f;
            for (S32 i = 0; i < 3; ++i)
            {
                if (m_usingAxis == i)
                    continue;

                Vec3DF crossAxis = basisTransform.transformAffine(getWorldAxis(i)).normalizedCopy();
                F32 dot = crossAxis.dotProduct(ray.getDirection());
                if (Math::Abs(dot) > d)
                {
                    d = Math::Abs(dot);
                    norm = crossAxis;
                }
            }

            F32 dist;
            if (Math::RaycastPlane(ray.getPoint(), ray.getDirection(), pos, norm, dist))
            {
                Vec3DF point = ray.getPoint(dist);
                point = Math::ClosestPointOnLine(pos, pos + axis, point);

                if (m_useRequest)
                {
                    m_useRequest = false;
                    m_startPosition = pos;
                    m_startPoint = point;
                }
                else
                {
                    Vec3DF delta = point - m_startPoint;

                    mat[0][3] = m_startPosition.x + delta.x;
                    mat[1][3] = m_startPosition.y + delta.y;
                    mat[2][3] = m_startPosition.z + delta.z;

                    entityTransform->setWorldTransform(mat);
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
            drawFunc.drawFunc();
        GizmosHelper::PopTransform();
    }

    //////////////////////////////////////////
    void GizmoToolTranslation::processCursorPress(Vec2DF const& _cursorPos)
    {
        if (m_selectedAxis >= 0)
        {
            m_usingAxis = m_selectedAxis;
            m_useRequest = true;
        }
    }

    //////////////////////////////////////////
    void GizmoToolTranslation::processCursorRelease()
    {
        m_usingAxis = -1;
        m_selectedAxis = -1;
    }

    //////////////////////////////////////////
    Vec3DF GizmoToolTranslation::getWorldAxis(S32 _axis)
    {
        switch (_axis)
        {
            case 0: return Vec3DF::c_unitX;
            case 1: return Vec3DF::c_unitY;
            case 2: return Vec3DF::c_unitZ;
        }

        return Vec3DF::c_zero;
    }

} // namespace Maze
//////////////////////////////////////////
