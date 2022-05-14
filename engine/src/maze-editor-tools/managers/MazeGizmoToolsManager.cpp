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
#include "maze-editor-tools/managers/MazeGizmoToolsManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeLight3DGizmos.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeCamera3DGizmos.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeMeshRendererGizmos.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class GizmoToolsManager
    //
    //////////////////////////////////////////
    GizmoToolsManager* GizmoToolsManager::s_instance = nullptr;

    //////////////////////////////////////////
    GizmoToolsManager::GizmoToolsManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    GizmoToolsManager::~GizmoToolsManager()
    {
        if (GizmosManager::GetInstancePtr())
        {
            GizmosManager::GetInstancePtr()->eventCanvasChanged.unsubscribe(this);
            GizmosManager::GetInstancePtr()->eventCanvasWillBeChanged.unsubscribe(this);
        }

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void GizmoToolsManager::Initialize(GizmoToolsManagerPtr& _graphicsManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(GizmoToolsManager, _graphicsManager, init());
    }

    //////////////////////////////////////////
    bool GizmoToolsManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        GizmosManager::GetInstancePtr()->eventCanvasChanged.subscribe(this, &GizmoToolsManager::notifyCanvasChanged);
        GizmosManager::GetInstancePtr()->eventCanvasWillBeChanged.subscribe(this, &GizmoToolsManager::notifyCanvasWillBeChanged);
        if (GizmosManager::GetInstancePtr()->getCanvas())
            subscribeCanvas(GizmosManager::GetInstancePtr()->getCanvas());

        return true;
    }

    //////////////////////////////////////////
    void GizmoToolsManager::update(F32 dt)
    {
        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();
        if (selectedEntities.size() == 1)
        {
            EntityPtr const& entity = *selectedEntities.begin();
            if (entity)
            {
                Transform3D* transform = entity->getComponentRaw<Transform3D>();
                if (transform)
                {
                    Mat4DF mat = transform->getWorldTransform();
                    drawTranslation(mat);
                }
            }
        }

        Camera3DPtr const& camera = GizmosManager::GetInstancePtr()->getCamera();
        if (camera)
        {
            Ray r = camera->convertViewportCoordsToRay(m_cursorPos);
            Debug::Log("%.1f %.1f %.1f", r.getDirection().x, r.getDirection().y, r.getDirection().z);

            F32 dist;

            Vec3DF sphereCenter = Vec3DF(4.0f, 6.0f, 1.0f);
            F32 sphereRadius = 1.0f;
            if (Math::RaycastSphere(r.getPoint(), r.getDirection(), sphereCenter, sphereRadius, dist))
                GizmosHelper::DrawSphere(sphereCenter, sphereRadius, ColorF128::c_yellow);
            else
                GizmosHelper::DrawSphere(sphereCenter, sphereRadius, ColorF128::c_red);


            Vec3DF cylinderCenter = Vec3DF(7.0f, 6.0f, 1.0f);
            Vec3DF cylinderForward = (Vec3DF::c_unitY + Vec3DF::c_unitZ).normalizedCopy();
            F32 cylinderRadius = 1.5f;
            F32 cylinderHeight = 3.0f;
            if (Math::RaycastCylinder(r.getPoint(), r.getDirection(), cylinderCenter, cylinderForward, cylinderRadius, cylinderHeight, dist))
                GizmosHelper::DrawCylinder(cylinderCenter, cylinderForward, cylinderRadius, cylinderHeight, ColorF128::c_yellow);
            else
                GizmosHelper::DrawCylinder(cylinderCenter, cylinderForward, cylinderRadius, cylinderHeight, ColorF128::c_red);

            GizmosHelper::SetColor(ColorF128::c_green);
            GizmosHelper::DrawLine(r.getPoint(), r.getPoint(10.0f), 1.0f);
        }
    }

    //////////////////////////////////////////
    void GizmoToolsManager::drawTranslation(Mat4DF& _mat)
    {
        Camera3DPtr const& camera = GizmosManager::GetInstancePtr()->getCamera();
        if (!camera)
            return;

        Vec3DF const& cameraWorldPosition = camera->getTransform()->getWorldPosition();

        Vec3DF right = { _mat[0][0], _mat[1][0], _mat[2][0] };
        Vec3DF up = { _mat[0][1], _mat[1][1], _mat[2][1] };
        Vec3DF forward = { _mat[0][2], _mat[1][2], _mat[2][2] };
        Vec3DF pos = { _mat[0][3], _mat[1][2], _mat[2][3] };
        
        F32 cameraDistance = (pos - camera->getTransform()->getLocalPosition()).length();
        GizmosHelper::PushTransform(
            _mat *
            Mat4DF::CreateScaleMatrix(cameraDistance * 0.08f / _mat.getAffineScale()));

        GizmosDrawer::MeshRenderMode const renderMode = GizmosDrawer::MeshRenderMode::Transparent;

        F32 const length = 2.0f;

        auto drawAxis = [&](
            ColorF128 const& _color,
            Vec3DF const& _axis)
        {
            GizmosHelper::SetColor(_color);
            
            // GizmosHelper::DrawLine(
            //     Vec3DF::c_zero,
            //     _axis * length,
            //     0.0f,
            //     renderMode);
            GizmosHelper::DrawCylinder(
                _axis * length * 0.5f,
                _axis,
                0.015f,
                length,
                _color,
                0.0f,
                renderMode);
            GizmosHelper::DrawCone(
                _axis * length,
                _axis,
                0.135f,
                0.475f,
                _color,
                0.0f,
                renderMode);
        };

        auto drawX = [&]() { drawAxis(ColorF128::c_red, Vec3DF::c_unitX); };
        auto drawY = [&]() { drawAxis(ColorF128::c_green, Vec3DF::c_unitY); };
        auto drawZ = [&]() { drawAxis(ColorF128::c_blue, Vec3DF::c_unitZ); };
        
        Vector<Pair<F32, std::function<void()>>> drawFuncs =
        {
            {(pos + right).squaredDistance(cameraWorldPosition), drawX},
            {(pos + up).squaredDistance(cameraWorldPosition), drawY},
            {(pos + forward).squaredDistance(cameraWorldPosition), drawZ}
        };
        std::sort(
            drawFuncs.begin(),
            drawFuncs.end(),
            [](Pair<F32, std::function<void()>> const& _a, Pair<F32, std::function<void()>> const& _b)
            {
                return _a.first > _b.first;
            });
        for (auto drawFunc : drawFuncs)
            drawFunc.second();

        GizmosHelper::PopTransform();
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCanvasChanged(CanvasPtr const& _canvas)
    {
        subscribeCanvas(_canvas);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCanvasWillBeChanged(CanvasPtr const& _canvas)
    {
        unsubscribeCanvas(_canvas);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::subscribeCanvas(CanvasPtr const& _canvas)
    {
        if (!_canvas)
            return;

        UIElement2DPtr element = _canvas->getEntityRaw()->ensureComponent<UIElement2D>();
        element->eventCursorMoveIn.subscribe(this, &GizmoToolsManager::notifyCursorMoveIn);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::unsubscribeCanvas(CanvasPtr const& _canvas)
    {
        if (!_canvas)
            return;

        UIElement2DPtr element = _canvas->getEntityRaw()->ensureComponent<UIElement2D>();
        element->eventCursorMoveIn.unsubscribe(this);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCursorMoveIn(Vec2DF const& _positionOS, CursorInputEvent const& _event)
    {
        processCursorMove(_positionOS);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::processCursorMove(Vec2DF const& _positionOS)
    {
        Camera3DPtr const& camera = GizmosManager::GetInstancePtr()->getCamera();
        if (!camera)
            return;

        m_cursorPos = _positionOS;
    }


} // namespace Maze
//////////////////////////////////////////
