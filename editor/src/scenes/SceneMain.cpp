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
#include "SceneMain.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-debugger/ecs/components/MazeHierarchyController.hpp"
#include "maze-debugger/ecs/components/MazeInspectorController.hpp"
#include "maze-debugger/ecs/components/MazeAssetsController.hpp"
#include "maze-debugger/ecs/components/MazePreviewController.hpp"
#include "maze-debugger/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "ecs/components/EditorHierarchyController.hpp"
#include "ecs/components/EditorMainCanvasController.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"
#include "scenes/SceneDebug.hpp"
#include "scenes/SceneDebugPreview.hpp"
#include "maze-ui/color-picker/SceneColorPicker.hpp"
#include "maze-ui/color-picker/SceneColorGradientPicker.hpp"
#include "maze-ui/curve-editor/SceneCurveEditor.hpp"
#include "maze-ui/material-picker/SceneMaterialPicker.hpp"
#include "maze-ui/texture-picker/SceneTexturePicker.hpp"
#include "maze-ui/render-mesh-picker/SceneRenderMeshPicker.hpp"
#include "helpers/EditorHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SceneMain
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneMain, ECSRenderScene);

    //////////////////////////////////////////
    SceneMain::SceneMain()
        : m_yawAngle(0.0f)
        , m_pitchAngle(0.0f)
        , m_cursorPositionLastFrame(Vec2DF::c_zero)
        , m_cursorDrag(false)
    {
    }

    //////////////////////////////////////////
    SceneMain::~SceneMain()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        if (Editor::GetInstancePtr())
        {
            if (Editor::GetInstancePtr()->getMainRenderWindow())
            {
                Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
            }
        }

        if (GizmosManager::GetInstancePtr())
        {
            GizmosManager::GetInstancePtr()->setCamera(nullptr);
        }
    }

    //////////////////////////////////////////
    SceneMainPtr SceneMain::Create()
    {
        SceneMainPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneMain, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneMain::init()
    {
        if (!ECSRenderScene::init(Editor::GetInstancePtr()->getMainRenderWindow()))
            return false;

        Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneMain::notifyMainRenderWindowResized);

        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &SceneMain::notifyMouse);

        create3D();
        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneMain::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

        if (!Editor::GetInstancePtr()->getMainRenderWindow()->getFocused())
            return;

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        if (m_camera3D)
        {
            Vec3DF cameraForwardDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3DF::c_unitZ;
            Vec3DF cameraRightDirection = m_camera3D->getTransform()->getLocalRotation() * Vec3DF::c_unitX;

            F32 speed = 2.0f;

            if (GetAsyncKeyState(87) & 0x8000)
            {
                m_camera3D->getTransform()->translate(cameraForwardDirection * _dt * speed);
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


        if (m_camera3D)
            m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));
    }

    //////////////////////////////////////////
    void SceneMain::notifyMouse(InputEventMouseData const& _data)
    {
        if (!Editor::GetInstancePtr()->getMainRenderWindow()->getFocused())
            return;

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
    void SceneMain::create3D()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        EntityManager* entityManager = EntityManager::GetInstancePtr();
        ECSWorldPtr const& world = entityManager->getDefaultWorld();

        RenderWindowPtr const& renderTarget = Editor::GetInstancePtr()->getMainRenderWindow();


        // Camera
        EntityPtr cameraEntity = createEntity();
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3DF(3.0f, 3.0f, -5.0f));
        m_yawAngle = Math::DegreesToRadians(-30.0f);
        m_pitchAngle = Math::DegreesToRadians(20.0f);
        m_camera3D->getTransform()->setLocalRotation(Quaternion(m_pitchAngle, m_yawAngle, 0.0f));

        m_camera3D->setFOV(Math::DegreesToRadians(60));
        m_camera3D->setRenderTarget(renderTarget);
        m_camera3D->setClearColor(ColorU32(99, 101, 140, 255));
        m_camera3D->getEntityRaw()->ensureComponent<Name>("Camera");
        Rect2DF sceneViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
        m_camera3D->setViewport(sceneViewport);
        m_camera3D->setRenderMask(m_camera3D->getRenderMask() | (S32)DefaultRenderMask::Gizmos);
        GizmosManager::GetInstancePtr()->setCamera(m_camera3D);

        // DebugGrid
        EntityPtr debugGridRendererEntity = createEntity();
        debugGridRendererEntity->createComponent<DebugGridRenderer>(m_camera3D);

        // Axes
        EntityPtr axesMeshRendererEntity = createEntity();
        axesMeshRendererEntity->createComponent<Transform3D>();
        MeshRendererPtr axesMeshRenderer = axesMeshRendererEntity->createComponent<MeshRenderer>();
        axesMeshRenderer->setRenderMesh(RenderMesh::Create(MeshHelper::CreateCoordinateAxes()));
        axesMeshRenderer->setMaterial(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getMaterialManager()->getDebugAxisMaterial());
        axesMeshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);
    }

    //////////////////////////////////////////
    void SceneMain::create2D()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();

        {
            EntityPtr topMenuBarCanvasEntity = createEntity();
            topMenuBarCanvasEntity->ensureComponent<Name>("Top Menu Canvas");
            m_topMenuBarCanvas = topMenuBarCanvasEntity->createComponent<Canvas>();
            m_topMenuBarCanvas->setClearColorFlag(false);
            m_topMenuBarCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF topMenuBarCanvasViewport(
                0.0f,
                (renderWindow->getRenderTargetHeight() - (EditorLayout::c_menuBarHeight + 1)) / renderWindow->getRenderTargetHeight(),
                1.0f,
                (EditorLayout::c_menuBarHeight + 1) / renderWindow->getRenderTargetHeight());
            m_topMenuBarCanvas->setViewport(topMenuBarCanvasViewport);
            m_topMenuBarCanvas->setRenderTarget(m_renderTarget);
            m_topMenuBarCanvas->setSortOrder(-100000);

            MenuBar2DPtr menuBar = UIHelper::CreateDefaultMenuBarList(
                Vec2DF((F32)renderWindow->getRenderTargetWidth(), EditorLayout::c_menuBarHeight + 1),
                Vec2DF::c_zero,
                m_topMenuBarCanvas->getTransform(),
                this,
                Vec2DF::c_zero,
                Vec2DF::c_zero);
            menuBar->getEntityRaw()->ensureComponent<SizePolicy2D>();

            // menuBar->addOption("File", "New Scene", [](String const& _text) { Log("New Scene"); });
            // menuBar->addOption("File", "Open Scene", [](String const& _text) { Log("Open Scene"); });
            menuBar->addOption("File", "Save", [](String const& _text) { EditorHelper::Save(); });
            menuBar->addOption("File", "Save As...", [](String const& _text) { EditorHelper::SaveAs(); });
            menuBar->addOption("File", "Load", [](String const& _text) { EditorHelper::Load(); });
            menuBar->addOption("File", "Clear", [](String const& _text) { EditorHelper::Clear(); });
            menuBar->addOption("File", "Exit", [](String const& _text) { Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->close(); });

            menuBar->addOption("Edit", "Play", [](String const& _text) { Debug::Log("Play"); });
            menuBar->addOption("Edit", "Pause", [](String const& _text) { Debug::Log("Pause"); });

            menuBar->addOption("Edit", "Create/Prefab/2D", [](String const& _text) { EditorHelper::CreateNewPrefab2D(); });
            menuBar->addOption("Edit", "Create/Prefab/3D", [](String const& _text) { EditorHelper::CreateNewPrefab3D(); });

            menuBar->addOption("Entity", "Create Empty", [](String const& _text) { EditorHelper::CreateEntity("Entity"); });
            menuBar->addOption("Entity", "Create 2D", [](String const& _text) { EditorHelper::CreateEntity2D("Entity"); });
            menuBar->addOption("Entity", "Create 3D", [](String const& _text) { EditorHelper::CreateEntity3D("Entity"); });
                        

            menuBar->addOption("Component", "Add", [](String const& _text) { Debug::Log("Add"); });

            menuBar->addOption("Help", "About", [](String const& _text) { Debug::Log("About"); });
        }

        {
            EntityPtr mainCanvasEntity = createEntity();
            m_mainCanvas = mainCanvasEntity->createComponent<Canvas>();
            m_mainCanvas->setClearColorFlag(false);
            m_mainCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF mainCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
            m_mainCanvas->setViewport(mainCanvasViewport);
            m_mainCanvas->setRenderTarget(m_renderTarget);
            m_mainCanvas->setSortOrder(-1000000);

            EntityPtr mainCanvasControllerEntity = createEntity();
            EditorMainCanvasControllerPtr mainCanvasController = EditorMainCanvasController::Create(m_mainCanvas.get());
            mainCanvasControllerEntity->addComponent(mainCanvasController);
        }

        {
            EntityPtr hierarchyCanvasEntity = createEntity();
            m_hierarchyCanvas = hierarchyCanvasEntity->createComponent<Canvas>();
            m_hierarchyCanvas->setClearColorFlag(false);
            m_hierarchyCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF hierarchyCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_hierarchyViewport);
            m_hierarchyCanvas->setViewport(hierarchyCanvasViewport);
            m_hierarchyCanvas->setRenderTarget(m_renderTarget);
            m_hierarchyCanvas->setSortOrder(-1000000);

            EntityPtr hierarchyControllerEntity = createEntity();
            EditorHierarchyControllerPtr hierarchyController = EditorHierarchyController::Create(m_hierarchyCanvas.get());
            hierarchyControllerEntity->addComponent(hierarchyController);


            hierarchyController->addIgnoreScene(SceneDebug::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneDebugPreview::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneColorPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneColorGradientPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneCurveEditor::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneMaterialPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneRenderMeshPicker::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneTexturePicker::GetMetaClass()->getClassUID());
        }

        {
            EntityPtr inspectorCanvasEntity = createEntity();
            m_inspectorCanvas = inspectorCanvasEntity->createComponent<Canvas>();
            m_inspectorCanvas->setClearColorFlag(false);
            m_inspectorCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF inspectorCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_inspectorViewport);
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
            Rect2DF assetsCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_assetsViewport);
            m_assetsCanvas->setViewport(assetsCanvasViewport);
            m_assetsCanvas->setRenderTarget(m_renderTarget);
            m_assetsCanvas->setSortOrder(-1000000);

            EntityPtr assetsControllerEntity = createEntity();
            AssetsControllerPtr assetsController = AssetsController::Create(m_assetsCanvas.get());
            assetsControllerEntity->addComponent(assetsController);
        }

        
        {
            EntityPtr previewCanvasEntity = createEntity();
            m_previewCanvas = previewCanvasEntity->createComponent<Canvas>();
            m_previewCanvas->setClearColorFlag(false);
            m_previewCanvas->setClearColor(ColorU32::c_zero);
            Rect2DF previewCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport);
            m_previewCanvas->setViewport(previewCanvasViewport);
            m_previewCanvas->setRenderTarget(m_renderTarget);
            m_previewCanvas->setSortOrder(-1000000);

            EntityPtr previewControllerEntity = createEntity();
            PreviewControllerPtr previewController = PreviewController::Create(m_previewCanvas.get());
            previewControllerEntity->addComponent(previewController);
        }
    }

    //////////////////////////////////////////
    void SceneMain::notifyMainRenderWindowResized(RenderTarget* _renderTarget)
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();

        if (m_topMenuBarCanvas)
        {
            Rect2DF topMenuBarCanvasViewport(
                0.0f,
                (renderWindow->getRenderTargetHeight() - (EditorLayout::c_menuBarHeight + 1)) / renderWindow->getRenderTargetHeight(),
                1.0f,
                (EditorLayout::c_menuBarHeight + 1) / renderWindow->getRenderTargetHeight());
            m_topMenuBarCanvas->setViewport(topMenuBarCanvasViewport);
        }

        if (m_mainCanvas)
        {
            Rect2DF mainCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
            m_mainCanvas->setViewport(mainCanvasViewport);
        }

        if (m_hierarchyCanvas)
        {
            Rect2DF hierarchyCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_hierarchyViewport);
            m_hierarchyCanvas->setViewport(hierarchyCanvasViewport);
        }

        if (m_inspectorCanvas)
        {
            Rect2DF inspectorCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_inspectorViewport);
            m_inspectorCanvas->setViewport(inspectorCanvasViewport);
        }

        if (m_assetsCanvas)
        {
            Rect2DF assetsCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_assetsViewport);
            m_assetsCanvas->setViewport(assetsCanvasViewport);
        }

        if (m_previewCanvas)
        {
            Rect2DF previewCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport);
            m_previewCanvas->setViewport(previewCanvasViewport);
        }

        if (m_camera3D)
        {
            Rect2DF sceneViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
            m_camera3D->setViewport(sceneViewport);
        }
    }

} // namespace Maze
//////////////////////////////////////////
