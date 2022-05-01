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
#include "EditorSceneModeControllerPrefab.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
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
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-editor-tools/ecs/components/MazeDebugGridRenderer.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "Editor.hpp"
#include "scenes/SceneEditor.hpp"
#include "scenes/SceneWorkspaceTools.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "managers/EditorGizmosManager.hpp"
#include "settings/MazeEditorSceneSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorSceneModeControllerPrefab
    //
    //////////////////////////////////////////
    EditorSceneModeControllerPrefab::EditorSceneModeControllerPrefab()
    {

    }

    //////////////////////////////////////////
    EditorSceneModeControllerPrefab::~EditorSceneModeControllerPrefab()
    {
        if (SettingsManager::GetInstancePtr())
        {
            EditorSceneSettings* editorSceneSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>();
            editorSceneSettings->getDebugLightEnabledChangedEvent().unsubscribe(this);
            editorSceneSettings->getDebugAxesEnabledChangedEvent().unsubscribe(this);
            editorSceneSettings->getDebugGridEnabledChangedEvent().unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    EditorSceneModeControllerPrefabPtr EditorSceneModeControllerPrefab::Create()
    {
        EditorSceneModeControllerPrefabPtr obj;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorSceneModeControllerPrefab, obj, init());
        return obj;
    }

    //////////////////////////////////////////
    bool EditorSceneModeControllerPrefab::init()
    {
        if (!EditorSceneModeController::init())
            return false;

        SceneMainToolsPtr const& scene = EditorManager::GetInstancePtr()->getSceneMainTools();
        scene->getCamera3D()->getEntityRaw()->setActiveSelf(true);
        scene->getCamera3D()->setClearColor(ColorU32(99, 101, 140, 255));
        scene->getMainCanvas()->setClearColorFlag(false);


        m_canvasNode = SpriteHelper::CreateTransform2D(
            scene->getMainCanvas()->getTransform()->getSize(),
            Vec2DF::c_zero,
            scene->getMainCanvas()->getTransform(),
            scene.get(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_canvasNode->getEntityRaw()->ensureComponent<SizePolicy2D>();

        EntityPtr mainNodeEntity = scene->createEntity("Main Node");
        m_mainNode = mainNodeEntity->ensureComponent<Transform3D>();

        EntityPtr debugLightEntity = scene->createEntity("Debug Light");
        m_debugLight = debugLightEntity->createComponent<Light3D>();
        debugLightEntity->ensureComponent<Transform3D>()->setParent(m_mainNode);
        m_debugLight->setColor(ColorU32(255, 244, 214));
        m_debugLight->getTransform()->setLocalDirection(0.577f, -0.577f, 0.577f);
        m_debugLight->getTransform()->setLocalPosition(0.0f, 5.0f, -5.0f);



        m_topBarBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(
                m_canvasNode->getSize().x,
                20.0f),
            Vec2DF(0.0f, 0.0f),
            MaterialManager::GetCurrentInstance()->getColorTextureMaterial(),
            m_canvasNode,
            scene.get(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        m_topBarBackground->setColor(EditorToolsLayout::c_bodySubBackgroundColor);
        m_topBarBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);


        m_topBarLeftLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            m_topBarBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_topBarBackground->getTransform(),
            scene.get(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_topBarLeftLayout->setAutoWidth(false);
        m_topBarLeftLayout->setAutoHeight(false);
        m_topBarLeftLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_topBarLeftLayout->setSpacing(2.0f);

        m_lightButton = createBarButton(
            m_topBarLeftLayout->getTransform(),
            GizmosManager::GetInstancePtr()->getDefaultGizmosSprite(DefaultGizmosSprite::LightGizmo),
            ColorU32::c_yellow);
        m_lightButton->setChecked(true);
        m_lightButton->eventClick.subscribe(
            [this](Button2D* _button, CursorInputEvent const& _event)
            {
                SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->switchDebugLightEnabled();
                SettingsManager::GetInstancePtr()->saveSettings();
            });
        updateDebugLight();
        SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->getDebugLightEnabledChangedEvent().subscribe(
            this, &EditorSceneModeControllerPrefab::notifyDebugLightEnabledChanged);

        m_axesButton = createBarButton(
            m_topBarLeftLayout->getTransform(),
            EditorGizmosManager::GetInstancePtr()->getEditorGizmosSprite(EditorGizmosSprite::Axes),
            ColorU32::c_white);
        m_axesButton->setChecked(true);
        m_axesButton->eventClick.subscribe(
            [this](Button2D* _button, CursorInputEvent const& _event)
        {
            SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->switchDebugAxesEnabled();
            SettingsManager::GetInstancePtr()->saveSettings();
        });
        updateDebugAxes();
        SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->getDebugAxesEnabledChangedEvent().subscribe(
            this, &EditorSceneModeControllerPrefab::notifyDebugAxesEnabledChanged);

        m_gridButton = createBarButton(
            m_topBarLeftLayout->getTransform(),
            EditorGizmosManager::GetInstancePtr()->getEditorGizmosSprite(EditorGizmosSprite::Grid),
            ColorU32::c_white);
        m_gridButton->setChecked(true);
        m_gridButton->eventClick.subscribe(
            [this](Button2D* _button, CursorInputEvent const& _event)
        {
            SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->switchDebugGridEnabled();
            SettingsManager::GetInstancePtr()->saveSettings();
        });
        updateDebugGrid();
        SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->getDebugGridEnabledChangedEvent().subscribe(
            this, &EditorSceneModeControllerPrefab::notifyDebugGridEnabledChanged);




        m_topBarRightLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Right,
            VerticalAlignment2D::Middle,
            m_topBarBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_topBarBackground->getTransform(),
            scene.get(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_topBarRightLayout->setAutoWidth(false);
        m_topBarRightLayout->setAutoHeight(false);
        m_topBarRightLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_topBarRightLayout->setSpacing(2.0f);

        bool playtestModeEnabled = EditorManager::GetInstancePtr()->getPlaytestModeEnabled();

        if (!playtestModeEnabled)
        {
            ClickButton2DPtr saveButton = UIHelper::CreateDefaultClickButton(
                "Save",
                Vec2DF(42.0f, 18.0f),
                Vec2DF(-2.0f, -2.0f),
                m_topBarRightLayout->getTransform(),
                scene.get(),
                Vec2DF(1.0f, 1.0f),
                Vec2DF(1.0f, 1.0f));
            saveButton->eventClick.subscribe(
                [](Button2D* _button, CursorInputEvent const& _event)
                {
                    EditorPrefabManager::GetInstancePtr()->saveAssetFile();
                });
        }

        ClickButton2DPtr closeButton = UIHelper::CreateDefaultClickButton(
            "X",
            Vec2DF(18.0f, 18.0f),
            Vec2DF(-2.0f, -2.0f),
            m_topBarRightLayout->getTransform(),
            scene.get(),
            Vec2DF(1.0f, 1.0f),
            Vec2DF(1.0f, 1.0f));
        closeButton->eventClick.subscribe(
            [](Button2D* _button, CursorInputEvent const& _event)
            {
                EditorManager::GetInstancePtr()->clearMode();
            });

        return true;
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::shutdown()
    {
        if (m_canvasNode)
        {
            m_canvasNode->getEntityRaw()->removeFromECSWorld();
            m_canvasNode.reset();
        }

        if (m_mainNode)
        {
            m_mainNode->getEntityRaw()->removeFromECSWorld();
            m_mainNode.reset();
        }
    }

    //////////////////////////////////////////
    ToggleButton2DPtr EditorSceneModeControllerPrefab::createBarButton(
        Transform2DPtr const& _parent,
        SpritePtr const& _sprite,
        ColorU32 const& _spriteColor)
    {
        SceneMainToolsPtr const& scene = EditorManager::GetInstancePtr()->getSceneMainTools();

        ToggleButton2DPtr button = UIHelper::CreateToggleButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            _parent,
            scene.get(),
            Vec2DF::c_zero,
            Vec2DF::c_zero,
            { 200, 200, 200 },
            { 187, 187, 187 },
            { 161, 161, 161 },
            { 171, 171, 171 },
            { 151, 151, 151 });
        button->setCheckByClick(false);

        SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
            _sprite,
            Vec2DF(14.0f, 14.0f),
            Vec2DF::c_zero,
            nullptr,
            button->getTransform(),
            scene.get());
        sprite->setColor(_spriteColor);

        return button;
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::updateDebugLight()
    {
        bool value = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->getDebugLightEnabled();
        m_debugLight->getEntityRaw()->setActiveSelf(value);
        m_lightButton->setChecked(value);
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::updateDebugAxes()
    {
        SceneMainToolsPtr const& scene = EditorManager::GetInstancePtr()->getSceneMainTools();

        bool value = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->getDebugAxesEnabled();
        scene->getDebugAxesRenderer()->getEntityRaw()->setActiveSelf(value);
        m_axesButton->setChecked(value);
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::updateDebugGrid()
    {
        SceneMainToolsPtr const& scene = EditorManager::GetInstancePtr()->getSceneMainTools();

        bool value = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSceneSettings>()->getDebugGridEnabled();
        scene->getDebugGridRenderer()->getEntityRaw()->setActiveSelf(value);
        m_gridButton->setChecked(value);
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::notifyDebugLightEnabledChanged(bool const& _value)
    {
        updateDebugLight();
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::notifyDebugAxesEnabledChanged(bool const& _value)
    {
        updateDebugAxes();
    }

    //////////////////////////////////////////
    void EditorSceneModeControllerPrefab::notifyDebugGridEnabledChanged(bool const& _value)
    {
        updateDebugGrid();
    }

} // namespace Maze
//////////////////////////////////////////
