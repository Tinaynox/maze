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
#include "maze-editor-tools/ecs/components/MazeTopBarController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-editor-tools/inspectors/entities/MazeEntitiesInspector.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TopBarController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TopBarController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TopBarController);

    //////////////////////////////////////////
    TopBarController::TopBarController()
        : m_canvas(nullptr)
    {
    }

    //////////////////////////////////////////
    TopBarController::~TopBarController()
    {
        if (UpdateManager::GetInstancePtr())
            UpdateManager::GetInstancePtr()->removeUpdatable(this);

        if (SettingsManager::GetInstancePtr())
        {
            EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
            debbugerSettings->getPauseChangedEvent().unsubscribe(this);
            debbugerSettings->getSelectedGizmoToolChangedEvent().unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    TopBarControllerPtr TopBarController::Create(Canvas* _canvas)
    {
        TopBarControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TopBarController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool TopBarController::init(Canvas* _canvas)
    {
        m_canvas = _canvas;

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void TopBarController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void TopBarController::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        Transform2DPtr canvasTransform = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            Vec2DF(0.0f, 0.0f),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        canvasTransform->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();

        m_bodyBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(
                canvasTransform->getSize().x, 
                canvasTransform->getSize().y),
            Vec2DF(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_bodyBackground->setColor(EditorToolsLayout::c_bodyBackgroundColor);
        m_bodyBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();
        
        m_leftLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            m_bodyBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_leftLayout->setAutoWidth(false);
        m_leftLayout->setAutoHeight(false);
        m_leftLayout->setSpacing(2.0f);

        SpritePtr gizmoToolsSprites[GizmoToolType::MAX] =
        {
            nullptr,
            GizmosManager::GetInstancePtr()->getDefaultGizmosSprite(DefaultGizmosSprite::GizmoToolTranslate),
            GizmosManager::GetInstancePtr()->getDefaultGizmosSprite(DefaultGizmosSprite::GizmoToolRotate),
            GizmosManager::GetInstancePtr()->getDefaultGizmosSprite(DefaultGizmosSprite::GizmoToolScale)
        };
        for (GizmoToolType tool = GizmoToolType(1); tool < GizmoToolType::MAX; ++tool)
        {
            m_gizmoToolButtons[tool] = EditorToolsUIHelper::CreateDefaultToggleButton(
                gizmoToolsSprites[tool],
                ColorU32(85, 85, 85),
                m_leftLayout->getTransform(),
                getEntityRaw()->getECSScene(),
                Vec2DF(16.0f, 16.0f));
            m_gizmoToolButtons[tool]->setCheckByClick(false);
            m_gizmoToolButtons[tool]->eventClick.subscribe(
                [tool](Button2D* _button, CursorInputEvent const& _event)
            {
                EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
                debbugerSettings->setSelectedGizmoTool(tool);
            });
        }

        
        m_layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            m_bodyBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_layout->setAutoWidth(false);
        m_layout->setAutoHeight(false);
        SizePolicy2DPtr layoutSizePolicy = m_layout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_layout->setSpacing(2.0f);

        m_pauseButton = EditorToolsUIHelper::CreateDefaultToggleButton(
            m_layout->getTransform(),
            getEntityRaw()->getECSScene());
        m_pauseButton->setCheckByClick(false);
        m_pauseButton->eventClick.subscribe(
            [](Button2D* _button, CursorInputEvent const& _event)
            {
                EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
                debbugerSettings->switchPause();
            });
        SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Pause),
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            MaterialManager::GetCurrentInstance()->getColorTextureMaterial(),
            m_pauseButton->getTransform(),
            getEntityRaw()->getECSScene())->setColor(85, 85, 85);

        m_stepButton = EditorToolsUIHelper::CreateDefaultClickButton(
            m_layout->getTransform(),
            getEntityRaw()->getECSScene());
        m_stepButton->eventClick.subscribe(
            [](Button2D* _button, CursorInputEvent const& _event)
            {
                TaskManager::GetInstancePtr()->addMainThreadTask(
                    []()
                    {
                        UpdateManager::GetInstancePtr()->processStep(1.0f / 60.0f);
                    });
            });
        SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::StepUpdate),
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            MaterialManager::GetCurrentInstance()->getColorTextureMaterial(),
            m_stepButton->getTransform(),
            getEntityRaw()->getECSScene())->setColor(85, 85, 85);

        updateUI();

        EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
        debbugerSettings->getPauseChangedEvent().subscribe(this, &TopBarController::notifyPauseChanged);
        debbugerSettings->getSelectedGizmoToolChangedEvent().subscribe(this, &TopBarController::notifySelectedGizmoToolChanged);
    }

    //////////////////////////////////////////
    void TopBarController::notifyPauseChanged(bool const& _value)
    {
        updateUI();
    }

    //////////////////////////////////////////
    void TopBarController::notifySelectedGizmoToolChanged(GizmoToolType const& _tool)
    {
        updateGizmoToolsButtons();
    }

    //////////////////////////////////////////
    void TopBarController::updateGizmoToolsButtons()
    {
        EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();

        for (GizmoToolType tool = GizmoToolType(1); tool < GizmoToolType::MAX; ++tool)
        {
            m_gizmoToolButtons[tool]->setChecked(debbugerSettings->getSelectedGizmoTool() == tool);
        }
    }

    //////////////////////////////////////////
    void TopBarController::updateUI()
    {
        EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
        m_pauseButton->setChecked(debbugerSettings->getPause());

        updateGizmoToolsButtons();
    }

    
} // namespace Maze
//////////////////////////////////////////
