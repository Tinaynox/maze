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
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolConfig.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/math/MazeMathRaytracing.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
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
    void GizmoToolTranslation::manipulate(Set<EntityPtr> const& _entities, Vec2F const& _cursorPos)
    {
        Camera3DPtr const& camera = GizmosManager::GetInstancePtr()->getCamera();
        if (!camera)
            return;

        CanvasPtr const& canvas = GizmosManager::GetInstancePtr()->getCanvas();
        if (!canvas)
            return;

        if (_entities.empty())
            return;

        // #TODO: Multiselection
        EntityPtr const& entity = *_entities.begin();
        Transform3D* entityTransform = entity->getComponentRaw<Transform3D>();
        if (!entityTransform)
            return;

        TMat mat = entityTransform->getWorldTransform();

        Vec3F const& cameraWorldPosition = camera->getTransform()->getWorldPosition();

        Vec3F right = mat.getRight().normalizedCopy();
        Vec3F up = mat.getUp().normalizedCopy();
        Vec3F forward = mat.getForward().normalizedCopy();
        Vec3F pos = mat.getTranslation();

        F32 cameraDistance = (pos - camera->getTransform()->getLocalPosition()).length();
        F32 scale = cameraDistance * GizmoToolConfig::c_cameraScalePerDistance;

        TMat basisTransform = TMat::CreateBasis(right, up, forward);
        TMat transform = basisTransform * TMat::CreateTranslation(pos);

        GizmosDrawer::GizmosMode const gizmosMode = GizmosDrawer::GizmosMode::Debug;
        GizmosDrawer::MeshRenderMode const renderMode = GizmosDrawer::MeshRenderMode::TransparentTop;

        F32 const length = GizmoToolConfig::c_transformGizmoToolLength;

        auto drawAxis = [&](
            ColorF128 const& _color,
            Vec3F const& _axis)
        {
            GizmosHelper::SetColor(_color);

            GizmosHelper::DrawCylinder(
                scale * _axis * length * 0.5f,
                _axis,
                scale * GizmoToolConfig::c_transformGizmoToolArrowLineRadius,
                scale * length,
                _color,
                0.0f,
                gizmosMode,
                renderMode);
            GizmosHelper::DrawCone(
                scale * _axis * length,
                _axis,
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeHeight,
                _color,
                0.0f,
                gizmosMode,
                renderMode);
        };

        Ray ray = camera->convertViewportCoordsToRay(_cursorPos);

        auto checkAxis = [&](
            Vec3F const& _axis)
        {
#if 0
            GizmosHelper::DrawCylinder(
                transform.transform(scale * _axis * length * 0.5f),
                basisTransform.transform(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * length,
                ColorF128::c_cyan,
                0.0f,
                gizmosMode,
                renderMode);

            GizmosHelper::DrawCone(
                transform.transform(scale * _axis * length),
                basisTransform.transform(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius * 4.0f,
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeHeight * 2.0f,
                ColorF128::c_cyan,
                0.0f,
                gizmosMode,
                renderMode);
#endif

            F32 dist = 0.0;
            if (Math::RaycastCylinder(
                ray.getPoint(),
                ray.getDirection(),
                transform.transform(scale * _axis * length * 0.5f),
                basisTransform.transform(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * length,
                dist))
            {
                m_grabPoint = ray.getPoint() + dist * ray.getDirection();
                return true;
            }
            if (Math::RaycastCone(
                ray.getPoint(),
                ray.getDirection(),
                transform.transform(scale * _axis * length),
                basisTransform.transform(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius * 4.0f,
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeHeight * 2.0f,
                dist))
            {
                m_grabPoint = ray.getPoint() + dist * ray.getDirection();
                return true;
            }
            return false;
        };

        auto drawX = [&]() { drawAxis(m_selectedAxis == 0 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisXColor, Vec3F::c_unitX); };
        auto drawY = [&]() { drawAxis(m_selectedAxis == 1 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisYColor, Vec3F::c_unitY); };
        auto drawZ = [&]() { drawAxis(m_selectedAxis == 2 ? GizmoToolConfig::c_transformGizmoToolAxisSelectedColor : GizmoToolConfig::c_transformGizmoToolAxisZColor, Vec3F::c_unitZ); };

        auto checkX = [&]() { return checkAxis(Vec3F::c_unitX); };
        auto checkY = [&]() { return checkAxis(Vec3F::c_unitY); };
        auto checkZ = [&]() { return checkAxis(Vec3F::c_unitZ); };

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
            Axis(0, (pos + right).squaredDistance(cameraWorldPosition), drawX, checkX),
            Axis(1, (pos + up).squaredDistance(cameraWorldPosition), drawY, checkY),
            Axis(2, (pos + forward).squaredDistance(cameraWorldPosition), drawZ, checkZ)
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
            Vec3F axis = basisTransform.transform(getWorldAxis(m_usingAxis)).normalizedCopy();

            Vec3F norm;
            F32 d = 0.0f;
            for (S32 i = 0; i < 3; ++i)
            {
                if (m_usingAxis == i)
                    continue;

                Vec3F crossAxis = basisTransform.transform(getWorldAxis(i)).normalizedCopy();
                F32 dot = crossAxis.dotProduct(camera->getTransform()->getWorldForwardDirection());
                if (Math::Abs(dot) > d)
                {
                    d = Math::Abs(dot);
                    norm = crossAxis;
                }
            }

            Ray cursorRay = camera->convertViewportCoordsToRay(_cursorPos);

            if (m_useRequest)
            {
                m_useRequest = false;
                m_startPosition = pos;
                m_startPoint = Math::ClosestPointOnLineBToLineA(cursorRay, Ray(m_startPosition, axis));
            }
            else
            {
                Vec3F endPoint = Math::ClosestPointOnLineBToLineA(cursorRay, Ray(m_startPosition, axis));

                Vec3F delta = endPoint - m_startPoint;
                Vec3F newWorldPosition = m_startPosition + delta;
                
#if 0
                GizmosHelper::DrawSphere(m_startPoint, 0.1f, ColorF128::c_red, 0.1f);
                GizmosHelper::DrawSphere(endPoint, 0.1f, ColorF128::c_green, 0.1f);
#endif

                TMat parentWorldScale = entityTransform->getParent() ? entityTransform->getParent()->getWorldTransform()
                                                                     : TMat::c_identity;

                Vec3F newPosLS = parentWorldScale.inversed().transform(newWorldPosition);

                if (InputManager::GetInstancePtr()->getKeyState(KeyCode::LAlt) ||
                    InputManager::GetInstancePtr()->getKeyState(KeyCode::RAlt))
                {
                    newPosLS.x = Math::Round(newPosLS.x);
                    newPosLS.y = Math::Round(newPosLS.y);
                    newPosLS.z = Math::Round(newPosLS.z);
                }

                EditorActionHelper::Translate(entity, newPosLS);
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
    void GizmoToolTranslation::processCursorPress(Vec2F const& _cursorPos)
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
    Vec3F GizmoToolTranslation::getWorldAxis(S32 _axis)
    {
        switch (_axis)
        {
            case 0: return Vec3F::c_unitX;
            case 1: return Vec3F::c_unitY;
            case 2: return Vec3F::c_unitZ;
        }

        return Vec3F::c_zero;
    }

} // namespace Maze
//////////////////////////////////////////
