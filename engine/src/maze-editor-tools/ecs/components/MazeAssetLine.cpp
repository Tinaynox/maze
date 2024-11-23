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
#include "maze-editor-tools/ecs/components/MazeAssetLine.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
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
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeUIDragElement2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"
#include "maze-ui/scenes/SceneDragAndDropDefault.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AssetLine
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetLine, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AssetLine);

    //////////////////////////////////////////
    ObservableValue<AssetLine*> AssetLine::s_pressedAssetLine = ObservableValue<AssetLine*>();

    //////////////////////////////////////////
    AssetLine::AssetLine()
        : m_selected(false)
        , m_selectAssetFileByClick(false)
    {
        s_pressedAssetLine.eventValueChanged.subscribe(this, &AssetLine::notifyPressedAssetLineChanged);
    }

    //////////////////////////////////////////
    AssetLine::~AssetLine()
    {
        s_pressedAssetLine.eventValueChanged.unsubscribe(this, &AssetLine::notifyPressedAssetLineChanged);
        if (s_pressedAssetLine == this)
            s_pressedAssetLine = nullptr;

        if (m_dropDownRenderer && m_dropDownRenderer->getEntityRaw())
        {
            ClickButton2D* button = m_dropDownRenderer->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
            {
                button->eventClick.unsubscribe(this);
                button->eventCursorPressIn.unsubscribe(this);
                button->eventDoubleClick.unsubscribe(this);
                button->eventFocusChanged.unsubscribe(this);
            }
        }
        
        if (m_mainTransform && m_mainTransform->getEntityRaw())
        {
            ClickButton2D* button = m_mainTransform->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
            {
                button->eventClick.unsubscribe(this);
                button->eventCursorPressIn.unsubscribe(this);
                button->eventDoubleClick.unsubscribe(this);
                button->eventFocusChanged.unsubscribe(this);
            }
        }

        if (m_textEdit)
        {
            m_textEdit->eventSelectedChanged.unsubscribe(this);
            m_textEdit->eventTextInput.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    AssetLinePtr AssetLine::Create(
        AssetsController* _assetsController,
        AssetFilePtr const& _assetFile)
    {
        AssetLinePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetLine, object, init(_assetsController, _assetFile));
        return object;
    }

    //////////////////////////////////////////
    bool AssetLine::init(
        AssetsController* _assetsController,
        AssetFilePtr const& _assetFile)
    {
        m_assetsController = _assetsController;
        m_assetFile = _assetFile;

        return true;
    }

    //////////////////////////////////////////
    void AssetLine::processEntityAwakened()
    {
        S32 const charSize = 8;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();


        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_transform->setSize(Vec2F(100, 14.0f));
        m_transform->setAnchor(Vec2F(0.0f, 1.0f));
        m_transform->setPivot(Vec2F(0.0f, 1.0f));

        SizePolicy2DPtr sizePolicy = getEntityRaw()->ensureComponent<SizePolicy2D>();
        sizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        m_mainLayout = getEntityRaw()->ensureComponent<VerticalLayout2D>();
        m_mainLayout->setHorizontalAlignment(HorizontalAlignment2D::Right);        
        // m_mainLayout->setSpacing(5.0f);

        m_mainTransform = SpriteHelper::CreateTransform2D(
            m_transform->getSize(),
            Vec2F(10.0f, 0.0f),
            m_transform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        SizePolicy2DPtr mainLayoutSizePolicy = m_mainTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        mainLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        mainLayoutSizePolicy->setSizeDelta(-10.0f, 0.0f);

        m_backgroundRenderer = SpriteHelper::CreateSprite(
            ColorU32::c_lightGray,
            m_transform->getSize(),
            Vec2F::c_zero,
            MaterialPtr(),
            m_mainTransform,
            getEntityRaw()->getEcsScene());
        m_backgroundRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();

        F32 x = 0;

        m_dropDownRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2F(charSize, charSize) * 1.5f,
            Vec2F(x, 0) + Vec2F(charSize * 0.5f, 0.0f),
            materialManager->getSpriteMaterial(),
            m_mainTransform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.5f, 0.5f));
        m_dropDownRenderer->setColor(ColorU32::c_black);
        ClickButton2DPtr dropDownButton = m_dropDownRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
        dropDownButton->eventClick.subscribe(this, &AssetLine::notifyDropDownClick);
        m_dropDownRenderer->getEntityRaw()->setActiveSelf(true);
        x += (F32)charSize + 4;

        m_iconRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TextFile),
            Vec2F(charSize, charSize) * 1.5f,
            Vec2F(x, 0) + Vec2F(charSize, 0.0f) * 0.5f,
            materialManager->getSpriteMaterial(),
            m_mainTransform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.5f, 0.5f));
        x += (F32)charSize + 4;

        String label = EditorToolsHelper::GetNameWithoutExtension(m_assetFile->getFileName());
        m_textRenderer = EditorToolsUIHelper::CreateText(
            label.c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(100, charSize + 6),
            Vec2F(x, 0),
            m_mainTransform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_textRenderer->setColor(ColorU32::c_black);

        m_textEdit = UIHelper::CreateDefaultEditBox(
            label.c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            Vec2F(200, (F32)charSize + 4.0f),
            Vec2F(x, 0),
            m_mainTransform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_textEdit->eventSelectedChanged.subscribe(this, &AssetLine::notifyTextEditSelectedChanged);
        m_textEdit->eventTextInput.subscribe(this, &AssetLine::notifyTextEditInput);
        m_textEdit->getEntityRaw()->setActiveSelf(false);

        m_textButton = m_mainTransform->getEntityRaw()->ensureComponent<ClickButton2D>();
        m_textButton->eventClick.subscribe(this, &AssetLine::notifyLineClick);
        m_textButton->eventCursorPressIn.subscribe(this, &AssetLine::notifyLineCursorPressIn);
        m_textButton->eventDoubleClick.subscribe(this, &AssetLine::notifyLineDoubleClick);
        m_textButton->eventFocusChanged.subscribe(this, &AssetLine::notifyLineFocusChanged);

        m_contextMenu = m_mainTransform->getEntityRaw()->ensureComponent<ContextMenu2D>();
        
        m_childrenLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2F::c_zero,
            Vec2F(10.0f, 0.0f),
            m_transform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        // m_childrenLayout->setSpacing(5.0f);
        m_childrenTransform = m_childrenLayout->getTransform();

        m_textButton->getEntityRaw()->ensureComponent<UIDragElement2D>();

        SizePolicy2DPtr childrenLayoutSizePolicy = m_childrenLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        childrenLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        childrenLayoutSizePolicy->setSizeDelta(-10.0f, 0.0f);

        setExpanded(m_assetsController->getAssetFileExpanded(m_assetFile));
        setEditMode(m_assetsController->getAssetFileRename(m_assetFile));

        updateState();
    }

    //////////////////////////////////////////
    void AssetLine::notifyDropDownClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        m_assetsController->setAssetFileExpanded(m_assetFile, !isExpanded());

        eventDropDownClick(this);
    }

    //////////////////////////////////////////
    void AssetLine::AssetLine::notifyLineClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (m_selectAssetFileByClick)
        {
            if (SelectionManager::GetInstancePtr()->isObjectSelected(m_assetFile))
            {
                // Do nothing
                // SelectionManager::GetInstancePtr()->unselectObject(m_assetFile);
            }
            else
            {
                EditorActionHelper::SelectObjects(m_assetFile);
            }
        }

        eventLineClick(this);
    }

    //////////////////////////////////////////
    void AssetLine::notifyLineCursorPressIn(Button2D* _button, Vec2F const& _pos, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;
        
        s_pressedAssetLine = this;
    }

    //////////////////////////////////////////
    void AssetLine::notifyLineDoubleClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (m_selectAssetFileByClick)
        {
            if (!SelectionManager::GetInstancePtr()->isObjectSelected(m_assetFile))
                SelectionManager::GetInstancePtr()->selectObject(m_assetFile);
        }

        eventLineDoubleClick(this);
    }

    //////////////////////////////////////////
    void AssetLine::notifyLineFocusChanged(Button2D* _button, bool _value)
    {
        updateState();
    }

    //////////////////////////////////////////
    void AssetLine::setIcon(
        SpritePtr const& _sprite,
        ColorU32 const& _color)
    {
        if (m_iconRenderer)
        {
            m_iconRenderer->setSprite(_sprite);
            m_iconRenderer->setColor(_color);
        }
    }

    //////////////////////////////////////////
    void AssetLine::setDropDownVisible(bool _value)
    {
        if (m_dropDownRenderer)
            m_dropDownRenderer->getEntityRaw()->setActiveSelf(_value);
    }

    //////////////////////////////////////////
    bool AssetLine::getDropDownVisible() const
    {
        if (m_dropDownRenderer)
            return m_dropDownRenderer->getEntityRaw()->getActiveSelf();

        return false;
    }

    //////////////////////////////////////////
    void AssetLine::setExpanded(bool _value)
    {
        if (m_dropDownRenderer)
        {
            m_dropDownRenderer->setSprite(
                _value ? UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded)
                       : UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonCollapsed));
        }

        if (m_childrenLayout)
        {
            m_childrenLayout->getEntityRaw()->setActiveSelf(_value);
        }

        eventExpandedChanged(this, _value);
    }

    //////////////////////////////////////////
    bool AssetLine::isExpanded() const
    {
        if (m_childrenLayout)
            return m_childrenLayout->getEntityRaw()->getActiveSelf();

        return false;
    }

    //////////////////////////////////////////
    void AssetLine::setSelected(bool _value)
    {
        if (m_selected == _value)
            return;

        m_selected = _value;

        updateState();
    }

    //////////////////////////////////////////
    void AssetLine::setEditMode(bool _value)
    {
        m_textRenderer->getEntityRaw()->setActiveSelf(!_value);
        m_textEdit->getEntityRaw()->setActiveSelf(_value);

        if (_value)
        {
            m_textEdit->setSelected(true);
        }
    }

    //////////////////////////////////////////
    void AssetLine::updateState()
    {
        if (!m_textRenderer)
            return;

        bool showSelected = m_selected;

        if (s_pressedAssetLine != nullptr)
            showSelected = (s_pressedAssetLine == this);

        if (showSelected)
        {
            m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorSelected());
            m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjecBackgroundColorSelected());
        }
        else
        {
            m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorDefault());

            if (m_textButton->getUIElement()->getFocused())
                m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectBackgroundColorFocused());
            else
                m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectBackgroundColorDefault());
        }

    }

    //////////////////////////////////////////
    void AssetLine::notifyTextEditSelectedChanged(EditBox2D* _edit, bool _value)
    {
        if (!_value)
            m_assetsController->setAssetFileRename(m_assetFile, false);
    }

    //////////////////////////////////////////
    void AssetLine::notifyTextEditInput(EditBox2D* _edit)
    {
        String newFileName = _edit->getText();

        bool cancelRename = true;

        String oldFileName = EditorToolsHelper::GetNameWithoutExtension(m_assetFile->getFileName());

        if (!newFileName.empty() && oldFileName != newFileName)
        {
            String dir = FileHelper::GetParentDirectoryInPath(m_assetFile->getFullPath());
            String extension = FileHelper::GetFileExtension(m_assetFile->getFileName());

            String newFullPath = dir + "/" + newFileName;
            if (!extension.empty())
                newFullPath += "." + extension;

            if (!FileHelper::IsFileExists(newFullPath))
            {
                AssetManager::GetInstancePtr()->moveAssetFile(m_assetFile, newFullPath);
            }
            else
            {
                cancelRename = false;
            }
        }

        if (cancelRename)
            m_assetsController->setAssetFileRename(m_assetFile, false);
    }

    //////////////////////////////////////////
    void AssetLine::notifyPressedAssetLineChanged(AssetLine* const& _line)
    {
        updateState();
    }

    //////////////////////////////////////////
    void AssetLine::processCursorRelease(InputCursorReleaseEvent const& _event)
    {
        if (s_pressedAssetLine == this)
            s_pressedAssetLine = nullptr;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(AssetLineOnCursorRelease,
        {},
        {},
        InputCursorReleaseEvent const& _event,
        Entity* _entity,
        AssetLine* _assetLine)
    {
        _assetLine->processCursorRelease(_event);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(AssetLineOnDragStartedEvent,
        {},
        {},
        UIDragElementDragStartedEvent const& _event,
        Entity* _entity,
        ClickButton2D* _clickButton)
    {
        AssetLine* assetLine = _clickButton->getTransform()->getParent()->getEntityRaw()->getComponentRaw<AssetLine>();
        if (!assetLine)
            return;

        SceneDragAndDropDefaultPtr dndScene = SceneManager::GetInstancePtr()->getScene<SceneDragAndDropDefault>();
        if (dndScene)
        {
            dndScene->startDrag(
                assetLine->getIconRenderer()->getEntityRaw()->getComponent<Transform2D>());
            _entity->getComponent<ClickButton2D>()->getUIElement()->setPressed(false);
            _entity->getComponent<ClickButton2D>()->getUIElement()->setFocused(false);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
