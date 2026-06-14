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
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityAdd.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectEntities.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/math/MazeMathRaytracing.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
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

        bool worldSpace = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>()->getGizmoWorldSpace();

        TMat basisTransform;
        TMat transform;
        if (worldSpace)
        {
            basisTransform = TMat::c_identity;
            transform = TMat::CreateTranslation(pos);
        }
        else
        {
            basisTransform = TMat::CreateBasis(right, up, forward);
            transform = basisTransform * TMat::CreateTranslation(pos);
        }

        GizmosDrawer::GizmosMode const gizmosMode = GizmosDrawer::GizmosMode::Debug;
        GizmosDrawer::MeshRenderMode const renderMode = GizmosDrawer::MeshRenderMode::TransparentTop;

        F32 const length = GizmoToolConfig::c_transformGizmoToolLength;

        auto drawAxis = [&](
            ColorF128 const& _color,
            Vec3F const& _axis)
        {
            GizmosHelper::SetColor(_color);

            GizmosHelper::DrawSolidCylinder(
                scale * _axis * length * 0.5f,
                _axis,
                scale * GizmoToolConfig::c_transformGizmoToolArrowLineRadius,
                scale * length,
                _color,
                0.0f,
                gizmosMode,
                renderMode);
            GizmosHelper::DrawSolidCone(
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
            GizmosHelper::DrawSolidCylinder(
                transform.transform(scale * _axis * length * 0.5f),
                basisTransform.transform(_axis).normalizedCopy(),
                scale * GizmoToolConfig::c_transformGizmoToolArrowConeRadius,
                scale * length,
                ColorF128::c_cyan,
                0.0f,
                gizmosMode,
                renderMode);

            GizmosHelper::DrawSolidCone(
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

        Vec3F sortRight   = worldSpace ? Vec3F::c_unitX : right;
        Vec3F sortUp      = worldSpace ? Vec3F::c_unitY : up;
        Vec3F sortForward = worldSpace ? Vec3F::c_unitZ : forward;

        Vector<Axis> drawFuncs =
        {
            Axis(0, (pos + sortRight).squaredDistance(cameraWorldPosition), drawX, checkX),
            Axis(1, (pos + sortUp).squaredDistance(cameraWorldPosition), drawY, checkY),
            Axis(2, (pos + sortForward).squaredDistance(cameraWorldPosition), drawZ, checkZ)
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

            Ray cursorRay = camera->convertViewportCoordsToRay(_cursorPos);

            // Pick the non-drag axis most face-on to the camera as the drag plane normal.
            // This avoids the numerical blow-up that occurs when the cursor ray is nearly
            // parallel to the drag axis (ClosestPointOnLineBToLineA divides by ~0 in that case).
            Vec3F norm;
            F32 bestDot = 0.0f;
            for (S32 i = 0; i < 3; ++i)
            {
                if (m_usingAxis == i)
                    continue;
                Vec3F crossAxis = basisTransform.transform(getWorldAxis(i)).normalizedCopy();
                F32 d = Math::Abs(crossAxis.dotProduct(camera->getTransform()->getWorldForwardDirection()));
                if (d > bestDot)
                {
                    bestDot = d;
                    norm = crossAxis;
                }
            }

            F32 dist;
            if (Math::RaycastPlane(cursorRay.getPoint(), cursorRay.getDirection(), pos, norm, dist))
            {
                Vec3F point = cursorRay.getPoint(dist);
                point = Math::ProjectionPointOnLine(point, pos, pos + axis);

                if (m_useRequest)
                {
                    m_useRequest = false;
                    m_startPosition = pos;
                    m_startPoint = point;

                    if (InputManager::GetInstancePtr()->getKeyState(KeyCode::LShift) ||
                        InputManager::GetInstancePtr()->getKeyState(KeyCode::RShift))
                    {
                        EntityPtr entityCopy = entity->createCopy();
                        entityCopy->ensureComponent<Transform3D>()->setParent(
                            entity->ensureComponent<Transform3D>()->getParent());

                        if (EditorToolsActionManager::GetInstancePtr())
                            EditorToolsActionManager::GetInstancePtr()->applyActions(
                                EditorActionEntityAdd::Create(entityCopy),
                                EditorActionSelectEntities::Create(true, entityCopy));
                    }
                }
                else
                {
                    Vec3F delta = point - m_startPoint;
                    Vec3F newWorldPosition = m_startPosition + delta;

#if 0
                    GizmosHelper::DrawSolidSphere(m_startPoint, 0.1f, ColorF128::c_red, 0.1f);
                    GizmosHelper::DrawSolidSphere(point, 0.1f, ColorF128::c_green, 0.1f);
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
