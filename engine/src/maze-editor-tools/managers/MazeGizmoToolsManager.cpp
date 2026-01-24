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
#include "maze-core/settings/MazeSettingsManager.hpp"
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
#include "maze-editor-tools/gizmo-tools/MazeGizmoTool.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolTranslation.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolRotation.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolScale.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
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

        if (SettingsManager::GetInstancePtr())
        {
            EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
            editorToolsSettings->getSelectedGizmoToolChangedEvent().unsubscribe(this);
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


        EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
        editorToolsSettings->getSelectedGizmoToolChangedEvent().subscribe(this, &GizmoToolsManager::notifySelectedGizmoToolChanged);

        updateSelectedGizmoTool();

        return true;
    }

    //////////////////////////////////////////
    void GizmoToolsManager::update(F32 dt)
    {
        MAZE_PROFILE_EVENT("GizmoToolsManager::update");

        Set<EntityPtr> const& selectedEntities = SelectionManager::GetInstancePtr()->getSelectedEntities();
        if (selectedEntities.size() == 1)
        {
            if (m_gizmoTool)
                m_gizmoTool->manipulate(selectedEntities, m_cursorPos);
        }

        if (GizmosManager::GetInstancePtr()->getCamera())
        {
            /*
            Ray r = GizmosManager::GetInstancePtr()->getCamera()->convertViewportCoordsToRay(m_cursorPos);

            F32 dist;

            Vec3F sphereCenter = Vec3F(4.0f, 6.0f, 1.0f);
            F32 sphereRadius = 1.0f;
            if (Math::RaycastSphere(r.getPoint(), r.getDirection(), sphereCenter, sphereRadius, dist))
                GizmosHelper::DrawSolidSphere(sphereCenter, sphereRadius, ColorF128::c_yellow);
            else
                GizmosHelper::DrawSolidSphere(sphereCenter, sphereRadius, ColorF128::c_red);

            Vec3F cylinderCenter = Vec3F(7.0f, 6.0f, 1.0f);
            Vec3F cylinderForward = (Vec3F::c_unitY + Vec3F::c_unitZ).normalizedCopy();
            F32 cylinderRadius = 1.5f;
            F32 cylinderHeight = 3.0f;
            if (Math::RaycastCylinder(r.getPoint(), r.getDirection(), cylinderCenter, cylinderForward, cylinderRadius, cylinderHeight, dist))
                GizmosHelper::DrawSolidCylinder(cylinderCenter, cylinderForward, cylinderRadius, cylinderHeight, ColorF128::c_yellow);
            else
                GizmosHelper::DrawSolidCylinder(cylinderCenter, cylinderForward, cylinderRadius, cylinderHeight, ColorF128::c_red);


            Vec3F coneCenter = Vec3F(1.0f, 6.0f, 1.0f);
            Vec3F coneForward = (Vec3F::c_unitY + Vec3F::c_unitZ).normalizedCopy();
            F32 coneRadius = 1.5f;
            F32 coneHeight = 3.0f;
            if (Math::RaycastCone(r.getPoint(), r.getDirection(), coneCenter, coneForward, coneRadius, coneHeight, dist))
                GizmosHelper::DrawSolidCone(coneCenter, coneForward, coneRadius, coneHeight, ColorF128::c_yellow);
            else
                GizmosHelper::DrawSolidCone(coneCenter, coneForward, coneRadius, coneHeight, ColorF128::c_red);


            Vec3F cubeCenter = Vec3F(-3.0f, 3.0f, 0.0f);
            Vec3F cubeForward = (Vec3F::c_unitY + Vec3F::c_unitZ).normalizedCopy();
            Vec3F cubeUp = cubeForward.crossProduct((Vec3F::c_unitY + Vec3F::c_unitX).normalizedCopy());
            Vec3F cubeScale = Vec3F(3.0f, 0.5f, 5.0f);
            if (Math::RaycastCube(r.getPoint(), r.getDirection(), cubeCenter, cubeForward, cubeUp, cubeScale, dist))
                GizmosHelper::DrawSolidCube(cubeCenter, cubeForward, cubeUp, cubeScale, ColorF128::c_yellow);
            else
                GizmosHelper::DrawSolidCube(cubeCenter, cubeForward, cubeUp, cubeScale, ColorF128::c_red);


            Vec3F torusCenter = Vec3F(-3.0f, -3.0f, 0.0f);
            Vec3F torusForward = (Vec3F::c_unitY + Vec3F::c_unitZ).normalizedCopy();
            F32 torusRadius = 0.5f;
            F32 torusCsRadius = 0.1f;
            if (Math::RaycastTorus(r.getPoint(), r.getDirection(), torusCenter, torusForward, torusRadius, torusCsRadius, dist))
                GizmosHelper::DrawSolidTorus(torusCenter, torusForward, torusRadius, ColorF128::c_yellow);
            else
                GizmosHelper::DrawSolidTorus(torusCenter, torusForward, torusRadius, ColorF128::c_red);


            GizmosHelper::SetColor(ColorF128::c_green);
            GizmosHelper::DrawLine(r.getPoint(), r.getPoint(10.0f), 1.0f);
            */
        }
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
        element->eventCursorPressIn.subscribe(this, &GizmoToolsManager::notifyCursorPressIn);
        element->eventCursorReleaseIn.subscribe(this, &GizmoToolsManager::notifyCursorReleaseIn);
        element->eventCursorReleaseOut.subscribe(this, &GizmoToolsManager::notifyCursorReleaseOut);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::unsubscribeCanvas(CanvasPtr const& _canvas)
    {
        if (!_canvas)
            return;

        if (_canvas->getEntityRaw())
        {
            UIElement2DPtr element = _canvas->getEntityRaw()->ensureComponent<UIElement2D>();
            element->eventCursorMoveIn.unsubscribe(this);
            element->eventCursorPressIn.unsubscribe(this);
            element->eventCursorReleaseIn.unsubscribe(this);
            element->eventCursorReleaseOut.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCursorMoveIn(Vec2F const& _positionOS, CursorInputEvent& _event)
    {
        processCursorMove(_positionOS);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _event)
    {
        if (_event.button == 0)
            processCursorPressIn(_positionOS);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent& _event)
    {
        if (_event.button == 0)
            processCursorRelease();
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifyCursorReleaseOut(CursorInputEvent& _event)
    {
        if (_event.button == 0)
            processCursorRelease();
    }

    //////////////////////////////////////////
    void GizmoToolsManager::processCursorMove(Vec2F const& _positionOS)
    {
        Camera3DPtr const& camera = GizmosManager::GetInstancePtr()->getCamera();
        if (!camera)
            return;

        m_cursorPos = _positionOS;
    }

    //////////////////////////////////////////
    void GizmoToolsManager::processCursorPressIn(Vec2F const& _positionOS)
    {
        if (m_gizmoTool)
            m_gizmoTool->processCursorPress(_positionOS);
    }

    //////////////////////////////////////////
    void GizmoToolsManager::processCursorRelease()
    {
        if (m_gizmoTool)
            m_gizmoTool->processCursorRelease();
    }

    //////////////////////////////////////////
    void GizmoToolsManager::notifySelectedGizmoToolChanged(GizmoToolType const& _tool)
    {
        updateSelectedGizmoTool();
    }

    //////////////////////////////////////////
    void GizmoToolsManager::updateSelectedGizmoTool()
    {
        EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();

        m_gizmoTool.reset();

        switch (editorToolsSettings->getSelectedGizmoTool())
        {
            case GizmoToolType::Translate: m_gizmoTool = GizmoToolTranslation::Create(); break;
            case GizmoToolType::Rotate: m_gizmoTool = GizmoToolRotation::Create(); break;
            case GizmoToolType::Scale: m_gizmoTool = GizmoToolScale::Create(); break;
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
