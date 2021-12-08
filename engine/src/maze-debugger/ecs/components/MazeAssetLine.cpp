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
#include "maze-debugger/ecs/components/MazeAssetLine.hpp"
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
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
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
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/scenes/SceneDebugEditor.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


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
    AssetLine::AssetLine()
        : m_selected(false)
        , m_selectAssetFileByPress(false)
    {
    }

    //////////////////////////////////////////
    AssetLine::~AssetLine()
    {
        if (m_dropDownRenderer && m_dropDownRenderer->getEntityRaw())
        {
            ClickButton2D* button = m_dropDownRenderer->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
                button->eventClick.unsubscribe(this);
        }
        
        if (m_textRenderer && m_textRenderer->getEntityRaw())
        {
            ClickButton2D* button = m_textRenderer->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
                button->eventClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    AssetLinePtr AssetLine::Create(String const& _label)
    {
        AssetLinePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetLine, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool AssetLine::init(String const& _label)
    {
        m_label = _label;

        return true;
    }

    //////////////////////////////////////////
    void AssetLine::processEntityAwakened()
    {
        S32 const charSize = 8;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();


        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_transform->setSize(Vec2DF(100, charSize));
        m_transform->setAnchor(Vec2DF(0.0f, 1.0f));
        m_transform->setPivot(Vec2DF(0.0f, 1.0f));

        SizePolicy2DPtr sizePolicy = getEntityRaw()->ensureComponent<SizePolicy2D>();
        sizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        m_mainLayout = getEntityRaw()->ensureComponent<VerticalLayout2D>();
        m_mainLayout->setHorizontalAlignment(HorizontalAlignment2D::Right);
        m_mainLayout->setSpacing(5.0f);

        m_mainTransform = SpriteHelper::CreateTransform2D(
            m_transform->getSize(),
            Vec2DF(10.0f, 0.0f),
            m_transform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        SizePolicy2DPtr mainLayoutSizePolicy = m_mainTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        mainLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        mainLayoutSizePolicy->setSizeDelta(-10.0f, 0.0f);

        F32 x = 0;

        m_dropDownRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2DF(charSize, charSize) * 1.5f,
            Vec2DF(x, 0) + Vec2DF(charSize * 0.5f, -charSize * 0.5f + 0.5f),
            materialManager->getColorTextureMaterial(),
            m_mainTransform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.5f, 0.5f));
        m_dropDownRenderer->setColor(ColorU32::c_black);
        ClickButton2DPtr dropDownButton = m_dropDownRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
        dropDownButton->eventClick.subscribe(this, &AssetLine::notifyDropDownClick);
        m_dropDownRenderer->getEntityRaw()->setActiveSelf(true);
        x += (F32)charSize + 4;

        m_iconRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TextFile),
            Vec2DF(charSize, charSize) * 1.5f,
            Vec2DF(x, 0) + Vec2DF(charSize, -charSize) * 0.5f,
            materialManager->getColorTextureMaterial(),
            m_mainTransform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.5f, 0.5f));
        x += (F32)charSize + 4;

        m_textRenderer = SpriteHelper::CreateSystemText(
            m_label.c_str(),
            charSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2DF(100, charSize),
            Vec2DF(x, 0),
            m_mainTransform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.0f, 0.5f));
        m_textRenderer->setColor(ColorU32::c_black);
        ClickButton2DPtr textButton = m_textRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
        textButton->eventClick.subscribe(this, &AssetLine::notifyLinePressed);

        m_contextMenu = m_mainTransform->getEntityRaw()->ensureComponent<ContextMenu2D>();
        
        m_childrenLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2DF::c_zero,
            Vec2DF(10.0f, 0.0f),
            m_transform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        m_childrenLayout->setSpacing(5.0f);
        m_childrenTransform = m_childrenLayout->getTransform();

        SizePolicy2DPtr childrenLayoutSizePolicy = m_childrenLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        childrenLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        childrenLayoutSizePolicy->setSizeDelta(-10.0f, 0.0f);

        setExpanded(false);
        updateSelectedUI();
    }

    //////////////////////////////////////////
    void AssetLine::notifyDropDownClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        setExpanded(!isExpanded());

        eventDropDownClick(this);
    }

    //////////////////////////////////////////
    void AssetLine::notifyLinePressed(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (m_selectAssetFileByPress)
        {
            if (SelectionManager::GetInstancePtr()->isObjectSelected(m_assetFile))
                SelectionManager::GetInstancePtr()->unselectObject(m_assetFile);
            else
                SelectionManager::GetInstancePtr()->selectObject(m_assetFile);
        }

        eventLinePressed(this);
    }

    //////////////////////////////////////////
    void AssetLine::setIcon(SpritePtr const& _sprite)
    {
        if (m_iconRenderer)
            m_iconRenderer->setSprite(_sprite);
    }

    //////////////////////////////////////////
    void AssetLine::setDropDownVisible(bool _value)
    {
        if (m_dropDownRenderer)
            m_dropDownRenderer->getEntityRaw()->setActiveSelf(_value);
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

        updateSelectedUI();
    }

    //////////////////////////////////////////
    void AssetLine::setAssetFile(AssetFilePtr const& _line)
    {
        m_assetFile = _line;
    }

    //////////////////////////////////////////
    void AssetLine::updateSelectedUI()
    {
        if (!m_textRenderer)
            return;

        if (m_selected)
        {
            m_textRenderer->setColor(ColorU32(0, 255, 0, 255));
        }
        else
        {
            m_textRenderer->setColor(ColorU32(0, 0, 0, 255));
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
