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
#include "SceneEditor.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
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
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-editor-tools/ecs/components/MazePreviewController.hpp"
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "ecs/components/EditorHierarchyController.hpp"
#include "ecs/components/EditorMainCanvasController.hpp"
#include "ecs/components/EditorTopBarController.hpp"
#include "ecs/components/EditorEventRetranslator.hpp"
#include "Editor.hpp"
#include "layout/EditorLayout.hpp"
#include "scenes/SceneDebug.hpp"
#include "scenes/SceneDebugPreview.hpp"
#include "maze-editor-tools/color-picker/MazeSceneColorPicker.hpp"
#include "maze-editor-tools/color-picker/MazeSceneColorGradientPicker.hpp"
#include "maze-editor-tools/curve-editor/MazeSceneCurveEditor.hpp"
#include "maze-editor-tools/material-picker/MazeSceneMaterialPicker.hpp"
#include "maze-editor-tools/texture-picker/MazeSceneTexturePicker.hpp"
#include "maze-editor-tools/render-mesh-picker/MazeSceneRenderMeshPicker.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectEntities.hpp"
#include "helpers/EditorHelper.hpp"
#include "helpers/EditorAssetHelper.hpp"
#include "helpers/EditorProjectHelper.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "managers/EditorEntityManager.hpp"
#include "managers/EditorUIManager.hpp"
#include "editor/EditorSceneModeController.hpp"
#include "editor/scene-mode-none/EditorSceneModeControllerNone.hpp"
#include "editor/scene-mode-prefab/EditorSceneModeControllerPrefab.hpp"
#include "editor/scene-mode-scene/EditorSceneModeControllerScene.hpp"
#include "scenes/SceneWorkspaceTools.hpp"
#include "scenes/ScenePlaytest.hpp"
#include "scenes/ScenePlaytestTools.hpp"
#include "settings/MazeEditorSettings.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SceneEditor
    //
    ////////////////////////////////////////// 
    SceneEditor* SceneEditor::s_instance = nullptr;
    
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneEditor, EcsRenderScene);

    //////////////////////////////////////////
    SceneEditor::SceneEditor()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SceneEditor::~SceneEditor()
    {
        if (Editor::GetInstancePtr())
        {
            if (Editor::GetInstancePtr()->getMainRenderWindow())
            {
                Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
            }
        }

        if (EditorManager::GetInstancePtr())
        {
            EditorManager::GetInstancePtr()->eventSceneModeChanged.unsubscribe(this);
            EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.unsubscribe(this);
        }

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    SceneEditorPtr SceneEditor::Create()
    {
        SceneEditorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneEditor, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneEditor::init()
    {
        if (!EcsRenderScene::init(Editor::GetInstancePtr()->getMainRenderWindow()))
            return false;

        Editor::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneEditor::notifyMainRenderWindowResized);

        create3D();
        create2D();
        createSceneModeController();


        // Event retranslators
        {
            EntityPtr editorEventRetranslator = createEntity();
            editorEventRetranslator->createComponent<StaticName>("EditorEventRetranslator");
            editorEventRetranslator->createComponent<EditorEventRetranslator>();
        }

        EditorManager::GetInstancePtr()->eventSceneModeChanged.subscribe(this, &SceneEditor::notifySceneModeChanged);
        EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.subscribe(this, &SceneEditor::notifyPlaytestModeEnabledChanged);

        return true;
    }

    //////////////////////////////////////////
    void SceneEditor::update(F32 _dt)
    {

    }

    //////////////////////////////////////////
    void SceneEditor::create3D()
    {
        
    }

    //////////////////////////////////////////
    void SceneEditor::create2D()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();

        {
            EntityPtr topMenuBarCanvasEntity = createEntity();
            topMenuBarCanvasEntity->ensureComponent<Name>("Top Menu Canvas");
            m_topMenuBarCanvas = topMenuBarCanvasEntity->createComponent<Canvas>();
            m_topMenuBarCanvas->setClearColorFlag(false);
            m_topMenuBarCanvas->setClearColor(ColorU32::c_zero);
            Rect2F topMenuBarCanvasViewport(
                0.0f,
                (renderWindow->getRenderTargetHeight() - (EditorLayout::c_menuBarHeight + 1)) / renderWindow->getRenderTargetHeight(),
                1.0f,
                (EditorLayout::c_menuBarHeight + 1) / renderWindow->getRenderTargetHeight());
            m_topMenuBarCanvas->setViewport(topMenuBarCanvasViewport);
            m_topMenuBarCanvas->setRenderTarget(m_renderTarget);
            m_topMenuBarCanvas->setSortOrder(-100000);

            // Menu Bar
            m_menuBar = UIHelper::CreateDefaultMenuBarList(
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                Vec2F32((F32)renderWindow->getRenderTargetWidth(), EditorLayout::c_menuBarHeight + 1),
                Vec2F32::c_zero,
                m_topMenuBarCanvas->getTransform(),
                this,
                Vec2F32::c_zero,
                Vec2F32::c_zero);
            m_menuBar->getEntityRaw()->ensureComponent<SizePolicy2D>();
            
            m_menuBar->addOption(
                "File", "Save",
                [](String const& _text) { EditorHelper::Save(); },
                EditorHelper::SaveValidate);
            m_menuBar->addOption(
                "File", "Save As...",
                [](String const& _text) { EditorHelper::SaveAs(); },
                EditorHelper::SaveAsValidate);
            m_menuBar->addOption(
                "File", "Load",
                [](String const& _text) { EditorHelper::Load(); },
                EditorHelper::LoadValidate);
            m_menuBar->addOption(
                "File", "Clear",
                [](String const& _text) { EditorHelper::Clear(); },
                EditorHelper::ClearValidate);
            m_menuBar->addOption(
                "File", "Close Project",
                [](String const& _text) { EditorHelper::SelectProject(); },
                []() { return true; });
            m_menuBar->addOption(
                "File", "Exit",
                [](String const& _text) { Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->close(); });

            m_menuBar->addOption(
                "Edit", "Play",
                [](String const& _text) { Debug::Log("Play"); },
                EditorHelper::IsValidSceneMode);
            m_menuBar->addOption(
                "Edit", "Pause",
                [](String const& _text) { Debug::Log("Pause"); },
                EditorHelper::IsValidSceneMode);

            m_menuBar->addOption(
                "Edit", "Create/Prefab/2D",
                [](String const& _text) { EditorHelper::CreateNewPrefab2D(); },
                EditorHelper::IsValidSceneMode);
            m_menuBar->addOption(
                "Edit", "Create/Prefab/3D",
                [](String const& _text) { EditorHelper::CreateNewPrefab3D(); },
                EditorHelper::IsValidSceneMode);

            m_menuBar->addOption(
                "Entity", "Create/2D/Empty",
                [](String const& _text) { EditorHelper::CreateEntity2D("Entity"); },
                EditorHelper::IsValidSceneMode);
            m_menuBar->addOption(
                "Entity", "Create/2D/Sprite",
                [](String const& _text) { EditorHelper::CreateSprite2D("Sprite"); },
                EditorHelper::IsValidSceneMode);
            m_menuBar->addOption(
                "Entity", "Create/2D/Toggle Button",
                [](String const& _text) { EditorHelper::CreateToggleButton2D("ToggleButton"); },
                EditorHelper::IsValidSceneMode);
            m_menuBar->addOption(
                "Entity", "Create/2D/Click Button",
                [](String const& _text) { EditorHelper::CreateClickButton2D("ClickButton"); },
                EditorHelper::IsValidSceneMode);

            m_menuBar->addOption(
                "Entity", "Create/3D/Empty",
                [](String const& _text) { EditorHelper::CreateEntity3D("Entity"); },
                EditorHelper::IsValidSceneMode);
            m_menuBar->addOption(
                "Entity", "Create/3D/Light/Directional",
                [](String const& _text) { EditorHelper::CreateDirectionalLight("Directional Light"); },
                EditorHelper::IsValidSceneMode);

            for (BuiltinRenderMeshType meshType = BuiltinRenderMeshType(1); meshType != BuiltinRenderMeshType::MAX; ++meshType)
            {
                m_menuBar->addOption(
                    "Entity", "Create/3D/Mesh/" + meshType.toString(),
                    [meshType](String const& _text)
                    {
                        EntityPtr child = EditorHelper::CreateBuiltinMesh(meshType);
                    },
                    EditorHelper::IsValidSceneMode);
            }
            m_menuBar->addOption(
                "Entity", "Create/3D/FX/Particle System",
                [](String const& _text)
                {
                    EntityPtr child = EditorHelper::CreateNewParticleSystem3D("Particle System");
                },
                EditorHelper::IsValidSceneMode);

            for (TopBarMenuData const& optionData : EditorUIManager::GetInstancePtr()->getTopBarMenuData())
            {
                m_menuBar->addOption(
                    optionData.menuName,
                    optionData.option,
                    optionData.callback,
                    optionData.validate);
            }
                        
            


            // Hierarchy line - Entity
            EditorToolsManager::GetInstancePtr()->eventHierarchyLineEntityContextMenu.subscribe(
                [](MenuListTree2DPtr const& _menuListTree, Entity* _entity)
                {                 
                    Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
                    if (transform3D)
                    {
                        _menuListTree->addItem(
                            "Add Child/3D/Light/Directional",
                            [_entity, transform3D](String const& _text)
                            {
                                EntityPtr child = EditorHelper::CreateDirectionalLight(
                                    "Directional Light",
                                    transform3D->cast<Transform3D>());
                            });

                        for (BuiltinRenderMeshType meshType = BuiltinRenderMeshType(1); meshType != BuiltinRenderMeshType::MAX; ++meshType)
                        {
                            _menuListTree->addItem(
                                "Add Child/3D/Mesh/" + meshType.toString(),
                                [_entity, transform3D, meshType](String const& _text)
                                {
                                    EntityPtr child = EditorHelper::CreateBuiltinMesh(
                                        meshType,
                                        transform3D->cast<Transform3D>());
                                });
                        }
                        _menuListTree->addItem(
                            "Add Child/3D/FX/Particle System",
                            [_entity, transform3D](String const& _text)
                            {
                                EntityPtr child = EditorHelper::CreateNewParticleSystem3D(
                                    "Particle System",
                                    transform3D->cast<Transform3D>());
                            });
                    }
                    
                });

            // Hierarchy line - Scene
            EditorToolsManager::GetInstancePtr()->eventHierarchyLineSceneContextMenu.subscribe(
                [](MenuListTree2DPtr const& _menuListTree, EcsScene* _scene)
                {
                    _menuListTree->addItem(
                        "Add Child/3D/Light/Directional",
                        [](String const& _text)
                        {
                            EditorHelper::CreateDirectionalLight("Directional Light");
                        });

                    _menuListTree->addItem(
                        "Add Child/3D/Camera 3D",
                        [](String const& _text)
                        {
                            EditorHelper::CreateCamera3D("Camera");
                        });

                    for (BuiltinRenderMeshType meshType = BuiltinRenderMeshType(1); meshType != BuiltinRenderMeshType::MAX; ++meshType)
                    {
                        _menuListTree->addItem(
                            "Add Child/3D/Mesh/" + meshType.toString(),
                            [meshType](String const& _text)
                            {
                                EntityPtr child = EditorHelper::CreateBuiltinMesh(meshType);
                            });
                    }
                    
                    _menuListTree->addItem(
                        "Add Child/3D/FX/Particle System",
                        [](String const& _text)
                        {
                            EntityPtr child = EditorHelper::CreateNewParticleSystem3D("Particle System");
                        });
                });
        }

        {
            EntityPtr mainCanvasEntity = createEntity();
            m_workspaceCanvas = mainCanvasEntity->createComponent<Canvas>();
            m_workspaceCanvas->setClearColorFlag(true);
            m_workspaceCanvas->setClearColor(ColorU32::c_white);
            Rect2F mainCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
            m_workspaceCanvas->setViewport(mainCanvasViewport);
            m_workspaceCanvas->setRenderTarget(m_renderTarget);
            m_workspaceCanvas->setSortOrder(-1000000);

            MaterialPtr const& spriteMaterial = SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial();
            MaterialPtr workspaceSpriteMaterial = spriteMaterial->createCopy();
            workspaceSpriteMaterial->getFirstRenderPass()->setBlendFactors(BlendFactor::One, BlendFactor::Zero);

            RenderBufferPtr const& workspaceRenderBuffer = EditorWorkspaceManager::GetInstancePtr()->getWorkspaceRenderBuffer();
            m_workspaceSprite = Sprite::Create(workspaceRenderBuffer->getColorTexture()->cast<Texture2D>());
            m_workspaceSpriteRenderer = SpriteHelper::CreateSprite(
                m_workspaceSprite,
                m_workspaceCanvas->getTransform()->getSize(),
                Vec2F32(0.0f, 0.0f),
                workspaceSpriteMaterial,
                m_workspaceCanvas->getTransform(),
                this,
                Vec2F32::c_zero,
                Vec2F32::c_zero);
            m_workspaceSpriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            m_workspaceSpriteRenderer->getEntityRaw()->setActiveSelf(true);
        }

        {
            EntityPtr hierarchyCanvasEntity = createEntity();
            m_hierarchyCanvas = hierarchyCanvasEntity->createComponent<Canvas>();
            m_hierarchyCanvas->setClearColorFlag(false);
            m_hierarchyCanvas->setClearColor(ColorU32::c_zero);
            Rect2F hierarchyCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_hierarchyViewport);
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
            hierarchyController->addIgnoreScene(SceneWorkspace::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(SceneWorkspaceTools::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(ScenePlaytest::GetMetaClass()->getClassUID());
            hierarchyController->addIgnoreScene(ScenePlaytestTools::GetMetaClass()->getClassUID());
        }

        {
            EntityPtr inspectorCanvasEntity = createEntity();
            m_inspectorCanvas = inspectorCanvasEntity->createComponent<Canvas>();
            m_inspectorCanvas->setClearColorFlag(false);
            m_inspectorCanvas->setClearColor(ColorU32::c_zero);
            Rect2F inspectorCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_inspectorViewport);
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
            Rect2F assetsCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_assetsViewport);
            m_assetsCanvas->setViewport(assetsCanvasViewport);
            m_assetsCanvas->setRenderTarget(m_renderTarget);
            m_assetsCanvas->setSortOrder(-1000000);

            EntityPtr assetsControllerEntity = createEntity();
            m_assetsController = AssetsController::Create(m_assetsCanvas.get());
            assetsControllerEntity->addComponent(m_assetsController);
            m_assetsController->eventAssetDoubleClick.subscribe(
                [](String const& _assetFullPath)
                {
                    EditorAssetHelper::Edit(_assetFullPath);
                });

            updateAssetsController();
        }

        
        createPreviewController();

        {
            EntityPtr topBarCanvasEntity = createEntity();
            m_topBarCanvas = topBarCanvasEntity->createComponent<Canvas>();
            m_topBarCanvas->setClearColorFlag(false);
            m_topBarCanvas->setClearColor(ColorU32::c_zero);
            Rect2F topBarCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_topBarViewport);
            m_topBarCanvas->setViewport(topBarCanvasViewport);
            m_topBarCanvas->setRenderTarget(m_renderTarget);
            m_topBarCanvas->setSortOrder(-1000000);

            EntityPtr topBarControllerEntity = createEntity();
            EditorTopBarControllerPtr topBarController = EditorTopBarController::Create(m_topBarCanvas.get());
            topBarControllerEntity->addComponent(topBarController);
        }

        {
            EntityPtr editorMonoControllerEntity = createEntity("MonoController");
            m_editorMonoController = editorMonoControllerEntity->createComponent<MonoBehaviour>(
                MAZE_HCS("Maze.Editor.EditorMonoController"));
        }
    }

    //////////////////////////////////////////
    void SceneEditor::notifyMainRenderWindowResized(RenderTarget* _renderTarget)
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (renderWindow->getWindow()->getMinimized())
            return;

        if (m_topBarCanvas)
        {
            Rect2F topBarCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_topBarViewport);
            m_topBarCanvas->setViewport(topBarCanvasViewport);
        }

        if (m_topMenuBarCanvas)
        {
            Rect2F topMenuBarCanvasViewport(
                0.0f,
                (renderWindow->getRenderTargetHeight() - (EditorLayout::c_menuBarHeight + 1)) / renderWindow->getRenderTargetHeight(),
                1.0f,
                (EditorLayout::c_menuBarHeight + 1) / renderWindow->getRenderTargetHeight());
            m_topMenuBarCanvas->setViewport(topMenuBarCanvasViewport);
        }

        if (m_workspaceCanvas)
        {
            Rect2F mainCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_sceneViewport);
            m_workspaceCanvas->setViewport(mainCanvasViewport);
        }

        if (m_hierarchyCanvas)
        {
            Rect2F hierarchyCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_hierarchyViewport);
            m_hierarchyCanvas->setViewport(hierarchyCanvasViewport);
        }

        if (m_inspectorCanvas)
        {
            Rect2F inspectorCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_inspectorViewport);
            m_inspectorCanvas->setViewport(inspectorCanvasViewport);
        }

        if (m_assetsCanvas)
        {
            Rect2F assetsCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_assetsViewport);
            m_assetsCanvas->setViewport(assetsCanvasViewport);
        }

        if (m_previewCanvas)
        {
            Rect2F previewCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport);
            m_previewCanvas->setViewport(previewCanvasViewport);
        }
    }

    //////////////////////////////////////////
    void SceneEditor::notifyCurrentAssetsFullPath(String const& _currentAssetsFullPath)
    {
        updateAssetsController();
    }

    //////////////////////////////////////////
    void SceneEditor::notifySceneModeChanged(EditorSceneMode _mode)
    {
        createSceneModeController();
    }

    //////////////////////////////////////////
    void SceneEditor::notifyPlaytestModeEnabledChanged(bool _value)
    {
        createSceneModeController();

        if (m_workspaceSpriteRenderer)
            m_workspaceSpriteRenderer->getEntityRaw()->setActiveSelf(!_value);

        if (m_workspaceCanvas)
            m_workspaceCanvas->setClearColorFlag(!_value);

        if (!_value)
            EditorEntityManager::GetInstancePtr()->getWorkspaceWorld()->update(0.0f);

        updatePreviewController();
    }

    //////////////////////////////////////////
    void SceneEditor::updateAssetsController()
    {
        if (!m_assetsController)
            return;

        Path assetsFolder = EditorHelper::GetProjectAssetsFolder();
        m_assetsController->setAssetsFullPath(assetsFolder);
    }

    //////////////////////////////////////////
    void SceneEditor::destroySceneModeController()
    {
        if (!m_sceneModeController)
            return;

        m_sceneModeController->shutdown();
        m_sceneModeController.reset();
    }

    //////////////////////////////////////////
    void SceneEditor::createSceneModeController()
    {
        destroySceneModeController();

        EditorSceneMode sceneMode = EditorManager::GetInstancePtr()->getSceneMode();
        switch (sceneMode)
        {
            case EditorSceneMode::Prefab:
            {
                m_sceneModeController = EditorSceneModeControllerPrefab::Create();
                break;
            }
            case EditorSceneMode::Scene:
            {
                m_sceneModeController = EditorSceneModeControllerScene::Create();
                break;
            }
            default:
            {
                m_sceneModeController = EditorSceneModeControllerNone::Create();
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneEditor::updatePreviewController()
    {
        bool playtestModeEnabled = EditorManager::GetInstancePtr()->getPlaytestModeEnabled();
        bool isSceneMode = (EditorManager::GetInstancePtr()->getSceneMode() == EditorSceneMode::Scene);

        bool previewRequired = !playtestModeEnabled || !isSceneMode;

        if (!previewRequired && m_previewController)
            destroyPreviewController();
        else
        if (previewRequired && !m_previewController)
            createPreviewController();

    }

    //////////////////////////////////////////
    void SceneEditor::createPreviewController()
    {
        destroyPreviewController();

        EntityPtr previewCanvasEntity = createEntity();
        m_previewCanvas = previewCanvasEntity->createComponent<Canvas>();
        m_previewCanvas->setClearColorFlag(false);
        m_previewCanvas->setClearColor(ColorU32::c_zero);
        Rect2F previewCanvasViewport = EditorLayout::CalculateWorkViewport(EditorLayout::c_previewViewport);
        m_previewCanvas->setViewport(previewCanvasViewport);
        m_previewCanvas->setRenderTarget(m_renderTarget);
        m_previewCanvas->setSortOrder(-1000000);

        EntityPtr previewControllerEntity = createEntity();
        m_previewController = PreviewController::Create(m_previewCanvas.get());
        previewControllerEntity->addComponent(m_previewController);
    }

    //////////////////////////////////////////
    void SceneEditor::destroyPreviewController()
    {
        if (m_previewCanvas)
        {
            m_previewCanvas->getEntityRaw()->removeFromEcsWorld();
            m_previewCanvas.reset();
        }

        if (m_previewController)
        {
            m_previewController->getEntityRaw()->removeFromEcsWorld();
            m_previewController.reset();
        }
    }

} // namespace Maze
//////////////////////////////////////////
