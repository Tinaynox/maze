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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/scenes/SceneDebugEditor.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-debugger/ecs/components/MazeHierarchyController.hpp"
#include "maze-debugger/ecs/components/MazeInspectorController.hpp"
#include "maze-debugger/ecs/components/MazeAssetsController.hpp"
#include "maze-debugger/ecs/components/MazeTopBarController.hpp"
#include "maze-debugger/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-ui/color-picker/SceneColorPicker.hpp"
#include "maze-ui/color-picker/SceneColorGradientPicker.hpp"
#include "maze-ui/curve-editor/SceneCurveEditor.hpp"
#include "maze-ui/material-picker/SceneMaterialPicker.hpp"
#include "maze-ui/texture-picker/SceneTexturePicker.hpp"
#include "maze-ui/render-mesh-picker/SceneRenderMeshPicker.hpp"
#include "layout/MazeDebuggerLayout.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneDebugEditor
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDebugEditor, ECSRenderScene);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(SceneDebugEditor);

    //////////////////////////////////////////
    SceneDebugEditor::SceneDebugEditor()
        : m_hierarchyViewport(DebuggerLayout::c_hierarchyViewport)
        , m_inspectorViewport(DebuggerLayout::c_inspectorViewport)
        , m_assetsViewport(DebuggerLayout::c_assetsViewport)
        , m_topBarViewport(DebuggerLayout::c_topBarViewport)
        , m_sceneViewport(DebuggerLayout::c_sceneViewport)
        , m_yawAngle(0.0f)
        , m_pitchAngle(0.0f)
        , m_cursorPositionLastFrame(Vec2DF::c_zero)
        , m_cursorDrag(false)
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneDebugEditor::~SceneDebugEditor()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        if (GizmosManager::GetInstancePtr())
        {
            GizmosManager::GetInstancePtr()->setCamera(nullptr);
        }
    }

    //////////////////////////////////////////
    SceneDebugEditorPtr SceneDebugEditor::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneDebugEditorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDebugEditor, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneDebugEditor::init(RenderTargetPtr const& _renderTarget)
    {
        if (!ECSRenderScene::init(_renderTarget))
            return false;

        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &SceneDebugEditor::notifyMouse);

        
        create3D();
        create2D();
        createSystems();

        return true;
    }

    //////////////////////////////////////////
    void SceneDebugEditor::update(F32 _dt)
    {
        _dt = UpdateManager::GetInstancePtr()->getUnscaledDeltaTime();

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        
        Rect2DF viewportRect = m_camera3D->getViewport();
        viewportRect.position *= (Vec2DF)m_camera3D->getRenderTarget()->getRenderTargetSize();
        viewportRect.size *= (Vec2DF)m_camera3D->getRenderTarget()->getRenderTargetSize();

        AABB2D aabb = AABB2D::FromRect2D(viewportRect);

        if (aabb.contains(InputManager::GetInstancePtr()->getCursorPosition(0)))
        {
            Vec3DF cameraForwardDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3DF::c_unitZ;
            Vec3DF cameraRightDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3DF::c_unitX;

            F32 speed = 2.0f;

            if (GetAsyncKeyState(87) & 0x8000)
            {
                m_camera3D->getTransform()->translate(cameraForwardDirection *_dt * speed);
            }
            else
            if (GetAsyncKeyState(83) & 0x8000)
            {
                m_camera3D->getTransform()->translate(-cameraForwardDirection * _dt * speed);
            }

            if (GetAsyncKeyState(65) & 0x8000)
            {
                m_camera3D->getTransform()->translate(-cameraRightDirection * _dt * speed);
            }
            else
            if (GetAsyncKeyState(68) & 0x8000)
            {
                m_camera3D->getTransform()->translate(cameraRightDirection * _dt * speed);
            }
            else
            if (GetAsyncKeyState('Q') & 0x8000)
            {
                m_pitchAngle -= _dt * Math::c_pi;
            }
            else
            if (GetAsyncKeyState('E') & 0x8000)
            {
                m_pitchAngle += _dt * Math::c_pi;
            }
        }
#endif

        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
    }

    //////////////////////////////////////////
    void SceneDebugEditor::notifyMouse(InputEventMouseData const& _data)
    {
        switch (_data.type)
        {
            case InputEventMouseType::Move:
            {
                Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);

                if (m_cursorDrag)
                {
                    Vec2DF deltaPosition = cursorPosition - m_cursorPositionLastFrame;

                    m_yawAngle += deltaPosition.x * 0.0075f;
                    m_pitchAngle -= deltaPosition.y * 0.0075f;
                }

                m_cursorPositionLastFrame = cursorPosition;
                break;
            }
            case InputEventMouseType::ButtonDown:
            {
                if (_data.buttonId == 1)
                {
                    Vec2DF cursorPosition = Vec2DF((F32)_data.x, (F32)_data.y);
                    Rect2DF viewportRect(
                        m_camera3D->getViewport().position.x * m_renderTarget->getRenderTargetSize().x,
                        m_camera3D->getViewport().position.y * m_renderTarget->getRenderTargetSize().y,
                        m_camera3D->getViewport().size.x * m_renderTarget->getRenderTargetSize().x,
                        m_camera3D->getViewport().size.y * m_renderTarget->getRenderTargetSize().y);

                    if (viewportRect.contains(cursorPosition))
                    {
                        m_cursorDrag = true;
                    }
                }
                break;
            }
            case InputEventMouseType::ButtonUp:
            {
                if (_data.buttonId == 1)
                {
                    m_cursorDrag = false;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneDebugEditor::create3D()
    {
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3DF(3.0f, 3.0f, -5.0f));
        m_yawAngle = Math::DegreesToRadians(-30.0f);
        m_pitchAngle = Math::DegreesToRadians(20.0f);
        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
        m_camera3D->setRenderMask(0xFFFFFFFF);

        Rect2DF cameraViewport = m_sceneViewport;
        m_camera3D->setViewport(cameraViewport);
        m_camera3D->setRenderTarget(m_renderTarget);
        m_camera3D->setClearColor(ColorU32(99, 101, 140, 255));
        m_camera3D->setFOV(Maze::Math::DegreesToRadians(60));
        GizmosManager::GetInstancePtr()->setCamera(m_camera3D);        


        // DebugGrid
        EntityPtr debugGridRendererEntity = createEntity();
        debugGridRendererEntity->createComponent<DebugGridRenderer>(m_camera3D);
        debugGridRendererEntity->getComponent<RenderMask>()->setMask(DefaultRenderMask::Gizmos);
    
        // Axes
        EntityPtr axesMeshRendererEntity = createEntity();
        axesMeshRendererEntity->createComponent<Transform3D>();
        MeshRendererPtr axesMeshRenderer = axesMeshRendererEntity->createComponent<MeshRenderer>();
        axesMeshRenderer->setRenderMesh(RenderMesh::Create(MeshHelper::CreateCoordinateAxes()));
        axesMeshRenderer->setMaterial(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getDebugAxisMaterial());
        axesMeshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);
    }

    //////////////////////////////////////////
    void SceneDebugEditor::create2D()
    {
        {
            EntityPtr hierarchyCanvasEntity = createEntity();
            m_hierarchyCanvas = hierarchyCanvasEntity->createComponent<Canvas>();
            m_hierarchyCanvas->setClearColorFlag(false);
            m_hierarchyCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF hierarchyCanvasViewport = m_hierarchyViewport;
            m_hierarchyCanvas->setViewport(hierarchyCanvasViewport);
            m_hierarchyCanvas->setRenderTarget(m_renderTarget);
            m_hierarchyCanvas->setSortOrder(-1000000);

            EntityPtr hierarchyControllerEntity = createEntity();
            HierarchyControllerPtr hierarchyController = HierarchyController::Create(m_hierarchyCanvas.get());
            hierarchyControllerEntity->addComponent(hierarchyController);

            hierarchyController->addIgnoreScene(SceneColorPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneColorGradientPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneCurveEditor::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneMaterialPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneTexturePicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneRenderMeshPicker::GetMetaClass()->getClassUID());
        }

        {
            EntityPtr inspectorCanvasEntity = createEntity();
            m_inspectorCanvas = inspectorCanvasEntity->createComponent<Canvas>();
            m_inspectorCanvas->setClearColorFlag(false);
            m_inspectorCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF inspectorCanvasViewport = m_inspectorViewport;
            m_inspectorCanvas->setViewport(inspectorCanvasViewport);
            m_inspectorCanvas->setRenderTarget(m_renderTarget);
            m_inspectorCanvas->setSortOrder(-1000000);

            EntityPtr inspectorControllerEntity = createEntity();
            InspectorControllerPtr inspectorController = InspectorController::Create(m_inspectorCanvas.get());
            inspectorControllerEntity->addComponent(inspectorController);
        }
        
        {
            EntityPtr assetsCanvasEntity = createEntity();
            m_assetsCanvas = assetsCanvasEntity->createComponent<Canvas>();
            m_assetsCanvas->setClearColorFlag(false);
            m_assetsCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF assetsCanvasViewport = m_assetsViewport;
            m_assetsCanvas->setViewport(assetsCanvasViewport);
            m_assetsCanvas->setRenderTarget(m_renderTarget);
            m_assetsCanvas->setSortOrder(-1000000);

            EntityPtr assetsControllerEntity = createEntity();
            AssetsControllerPtr assetsController = AssetsController::Create(m_assetsCanvas.get());
            assetsControllerEntity->addComponent(assetsController);
        }

        {
            EntityPtr topBarCanvasEntity = createEntity();
            m_topBarCanvas = topBarCanvasEntity->createComponent<Canvas>();
            m_topBarCanvas->setClearColorFlag(false);
            m_topBarCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF topBarCanvasViewport = m_topBarViewport;
            m_topBarCanvas->setViewport(topBarCanvasViewport);
            m_topBarCanvas->setRenderTarget(m_renderTarget);
            m_topBarCanvas->setSortOrder(-1000000);

            EntityPtr topBarControllerEntity = createEntity();
            TopBarControllerPtr topBarController = TopBarController::Create(m_topBarCanvas.get());
            topBarControllerEntity->addComponent(topBarController);
        }
    }

    //////////////////////////////////////////
    void SceneDebugEditor::createSystems()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
