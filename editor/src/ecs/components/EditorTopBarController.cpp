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
#include "EditorTopBarController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
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
#include "maze-editor-tools/inspectors/asset-materials/MazeAssetMaterialsInspector.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "managers/EditorManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorTopBarController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorTopBarController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorTopBarController);

    //////////////////////////////////////////
    EditorTopBarController::EditorTopBarController()
        : m_canvas(nullptr)
    {
    }

    //////////////////////////////////////////
    EditorTopBarController::~EditorTopBarController()
    {
        if (UpdateManager::GetInstancePtr())
            UpdateManager::GetInstancePtr()->removeUpdatable(this);

        if (SettingsManager::GetInstancePtr())
        {
            EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
            debbugerSettings->getPauseChangedEvent().unsubscribe(this);
        }

        if (EditorManager::GetInstancePtr())
        {
            EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    EditorTopBarControllerPtr EditorTopBarController::Create(Canvas* _canvas)
    {
        EditorTopBarControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorTopBarController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool EditorTopBarController::init(Canvas* _canvas)
    {
        m_canvas = _canvas;

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void EditorTopBarController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void EditorTopBarController::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.subscribe(this, &EditorTopBarController::notifyPlaytestModeEnabledChanged);

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


        m_playButton = UIHelper::CreateToggleButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            m_layout->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f),
            { 200, 200, 200 },
            { 187, 187, 187 },
            { 161, 161, 161 },
            { 171, 171, 171 },
            { 151, 151, 151 });
        m_playButton->setCheckByClick(false);
        m_playButton->eventClick.subscribe(
            [](Button2D* _button, CursorInputEvent const& _event)
        {
            EditorManager::GetInstancePtr()->setPlaytestModeEnabled(
                !EditorManager::GetInstancePtr()->getPlaytestModeEnabled());
        });
        SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Play),
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            MaterialManager::GetCurrentInstance()->getColorTextureMaterial(),
            m_playButton->getTransform(),
            getEntityRaw()->getECSScene())->setColor(85, 85, 85);


        m_pauseButton = UIHelper::CreateToggleButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            m_layout->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f),
            { 200, 200, 200 },
            { 187, 187, 187 },
            { 161, 161, 161 },
            { 171, 171, 171 },
            { 151, 151, 151 });
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

        m_stepButton = UIHelper::CreateDefaultClickButton(
            "",
            Vec2DF(18.0f, 18.0f),
            Vec2DF::c_zero,
            m_layout->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        m_stepButton->setNormalColor({ 200, 200, 200 });
        m_stepButton->setFocusedColor({ 187, 187, 187 });
        m_stepButton->setSelectedColor({ 161, 161, 161 });
        m_stepButton->setPressedColor({ 171, 171, 171 });
        m_stepButton->eventClick.subscribe(
            [](Button2D* _button, CursorInputEvent const& _event)
            {
                UpdateManager::GetInstancePtr()->processStep(1.0f / 60.0f);
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
        debbugerSettings->getPauseChangedEvent().subscribe(this, &EditorTopBarController::notifyPauseChanged);
    }

    //////////////////////////////////////////
    void EditorTopBarController::notifyPauseChanged(bool const& _value)
    {
        updateUI();
    }

    //////////////////////////////////////////
    void EditorTopBarController::updateUI()
    {
        if (m_pauseButton)
        {
            EditorToolsSettings* debbugerSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
            m_pauseButton->setChecked(debbugerSettings->getPause());
        }
    }

    //////////////////////////////////////////
    void EditorTopBarController::notifyPlaytestModeEnabledChanged(bool _value)
    {
        m_playButton->setChecked(_value);
    }

    
} // namespace Maze
//////////////////////////////////////////
