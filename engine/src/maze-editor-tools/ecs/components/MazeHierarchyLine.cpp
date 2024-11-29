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
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
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
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeUIDragElement2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropController.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"
#include "maze-ui/scenes/SceneDragAndDrop.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class HierarchyLine
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(HierarchyLine, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(HierarchyLine);

    //////////////////////////////////////////
    HierarchyLine::HierarchyLine()
    {
    }

    //////////////////////////////////////////
    HierarchyLine::~HierarchyLine()
    {
        if (m_dropDownRenderer && m_dropDownRenderer->getEntityRaw())
        {
            ClickButton2D* button = m_dropDownRenderer->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
            {
                button->eventClick.unsubscribe(this);
                button->eventCursorPressIn.unsubscribe(this);
            }
        }

        if (m_backgroundButton)
        {
            m_backgroundButton->eventClick.unsubscribe(this);
            m_backgroundButton->eventCursorPressIn.unsubscribe(this);
            m_backgroundButton->eventDoubleClick.unsubscribe(this);
            m_backgroundButton->eventFocusChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyLine::Create(
        HierarchyLineType _type)
    {
        HierarchyLinePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(HierarchyLine, object, init(_type));
        return object;
    }

    //////////////////////////////////////////
    bool HierarchyLine::init(
        HierarchyLineType _type)
    {
        m_type = _type;

        return true;
    }

    //////////////////////////////////////////
    void HierarchyLine::processEntityAwakened()
    {
        buildUI();
    }
    
    //////////////////////////////////////////
    void HierarchyLine::setUserData(void* _userData)
    {
        if (m_userData == _userData)
            return;

        m_userData = _userData;

    }

    //////////////////////////////////////////
    void HierarchyLine::setExpanded(bool _value)
    {
        if (m_childrenLayout)
        {
            m_childrenLayout->getEntityRaw()->setActiveSelf(_value);
            updateDropDownRenderer();
        }
    }

    //////////////////////////////////////////
    bool HierarchyLine::getExpanded() const
    {
        if (!m_childrenLayout)
            return false;

        return m_childrenLayout->getEntityRaw()->getActiveSelf();
    }

    /////////////////////////////////////////
    void HierarchyLine::setText(String const& _text)
    {
        if (m_textRenderer)
            m_textRenderer->setText(_text);
    }

    //////////////////////////////////////////
    void HierarchyLine::setSelected(bool _value)
    {
        if (m_selected == _value)
            return;

        m_selected = _value;

        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLine::setActive(bool _value)
    {
        if (m_active == _value)
            return;

        m_active = _value;

        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLine::updateState()
    {
        if (!m_textRenderer)
            return;

        if (m_selected)
        {
            if (m_active)
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorSelected());
            else
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorInactive());

            m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjecBackgroundColorSelected());
        }
        else
        {
            if (m_active)
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorDefault());
            else
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorInactive());

            if (m_backgroundButton->getUIElement()->getFocused())
                m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectBackgroundColorFocused());
            else
                m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectBackgroundColorDefault());
        }

        if (m_iconRenderer)
            m_iconRenderer->setColor(m_textRenderer->getColor());
    }

    //////////////////////////////////////////
    void HierarchyLine::buildUI()
    {
        MAZE_PROFILE_EVENT("HierarchyLine::buildUI");

        {
            m_transform = getEntityRaw()->ensureComponent<Transform2D>();
            m_verticalLayout = getEntityRaw()->ensureComponent<VerticalLayout2D>();
            m_verticalLayout->setAutoHeight(true);
            m_sizePolicy = getEntityRaw()->ensureComponent<SizePolicy2D>();
            m_sizePolicy->setFlag(SizePolicy2D::Height, false);
        }

        {
            m_backgroundRenderer = SpriteHelper::CreateSprite(
                ColorU32::c_red,
                Vec2F(m_transform->getWidth(), m_rowHeight),
                Vec2F::c_zero,
                MaterialPtr(),
                m_transform,
                getEntityRaw()->getEcsScene());
            SizePolicy2DPtr backgroundRendererSizePolicy = m_backgroundRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            backgroundRendererSizePolicy->setFlag(SizePolicy2D::Height, false);
        }

        {
            m_backgroundButton = m_backgroundRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
            m_backgroundButton->eventClick.subscribe(this, &HierarchyLine::notifyLineClick);
            m_backgroundButton->eventCursorPressIn.subscribe(this, &HierarchyLine::notifyLineCursorPressIn);
            m_backgroundButton->eventDoubleClick.subscribe(this, &HierarchyLine::notifyLineDoubleClick);
            m_backgroundButton->eventFocusChanged.subscribe(this, &HierarchyLine::notifyLineFocusChanged);
        }

        {
            m_contextMenu = m_backgroundButton->getEntityRaw()->ensureComponent<ContextMenu2D>();
        }

        {
            m_nodeContainer = SpriteHelper::CreateTransform2D(
                m_backgroundRenderer->getTransform()->getSize(),
                Vec2F::c_zero,
                m_backgroundRenderer->getTransform(),
                getEntityRaw()->getEcsScene(),
                Vec2F(1.0f, 0.5f),
                Vec2F(1.0f, 0.5f));
            m_nodeContainerSizePolicy = m_nodeContainer->getEntityRaw()->ensureComponent<SizePolicy2D>();
            m_nodeContainerSizePolicy->setFlag(SizePolicy2D::Height, false);
            updateNodeContainerIndent();
        }

        {
            HorizontalLayout2DPtr horizontalLayout = m_nodeContainer->getEntityRaw()->ensureComponent<HorizontalLayout2D>();
            horizontalLayout->setSpacing(2.0f);
            horizontalLayout->setPaddingLeft(2.0f);
        }

        {
            m_dropDownRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
                Vec2F(m_rowHeight),
                Vec2F::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                m_nodeContainer,
                getEntityRaw()->getEcsScene());
            m_dropDownRenderer->setColor(ColorU32::c_black);
            ClickButton2DPtr dropDownButton = m_dropDownRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
            dropDownButton->eventClick.subscribe(this, &HierarchyLine::notifyDropDownClick);
            m_dropDownRenderer->getMeshRenderer()->setEnabled(false);
        }

        if (m_type == HierarchyLineType::Entity)
        {
            m_iconRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::EntityObject3D),
                Vec2F(m_rowHeight),
                Vec2F::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                m_nodeContainer,
                getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.5f, 0.5f));
            m_iconRenderer->setColor(ColorU32::c_black);

            m_backgroundButton->getEntityRaw()->ensureComponent<UIDragElement2D>();
        }
        else
        if (m_type == HierarchyLineType::Scene)
        {
            m_iconRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Scene),
                Vec2F(m_rowHeight),
                Vec2F::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                m_nodeContainer,
                getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.5f, 0.5f));
            m_iconRenderer->setColor(ColorU32::c_black);
        }

        {
            m_textRenderer = EditorToolsUIHelper::CreateText(
                "",
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(m_rowHeight),
                Vec2F::c_zero,
                m_nodeContainer,
                getEntityRaw()->getEcsScene());
            m_textRenderer->setColor(ColorU32::c_black);
        }

        {
            m_childrenLayout = UIHelper::CreateVerticalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(m_transform->getWidth(), 0.0f),
                Vec2F::c_zero,
                m_transform,
                getEntityRaw()->getEcsScene());
            m_childrenLayout->setAutoHeight(true);
            SizePolicy2DPtr childrenLayoutSizePolicy = m_childrenLayout->getEntityRaw()->ensureComponent<SizePolicy2D>();
            childrenLayoutSizePolicy->setFlag(SizePolicy2D::Height, false);
            setExpanded(false);
        }

        {
            updateDropDownRenderer();
        }

        {
            updateState();
        }
    }

    //////////////////////////////////////////
    void HierarchyLine::release()
    {
        ComponentPoolObject<HierarchyLine>* poolObject = getEntityRaw()->getComponentRaw<ComponentPoolObject<HierarchyLine>>();
        if (poolObject->isPoolExists())
            poolObject->release(cast<HierarchyLine>());
    }

    //////////////////////////////////////////
    void HierarchyLine::prepare()
    {
        setExpanded(false);
    }

    //////////////////////////////////////////
    Size HierarchyLine::getChildrenCount() const
    {
        if (m_childrenLayout)
            return m_childrenLayout->getTransform()->getChildren().size();

        return 0u;
    }

    //////////////////////////////////////////
    void HierarchyLine::addChild(HierarchyLinePtr const& _hierarchyLine)
    {
        _hierarchyLine->setIndent(m_indent + 1);
        _hierarchyLine->getEntityRaw()->ensureComponent<Transform2D>()->setParent(m_childrenLayout->getTransform());
        updateDropDownRenderer();
    }

    //////////////////////////////////////////
    void HierarchyLine::clearChildren()
    {
        Vector<HierarchyLinePtr> childLines;
        for (Transform2D* transform : m_childrenLayout->getTransform()->getChildren())
        {
            HierarchyLinePtr childLine = transform->getEntityRaw()->getComponent<HierarchyLine>();
            childLines.push_back(childLine);
        }

        m_childrenLayout->getTransform()->removeAllChildren();

        for (HierarchyLinePtr const& childLine : childLines)
            childLine->release();

        updateDropDownRenderer();
    }

    //////////////////////////////////////////
    void HierarchyLine::processAwake(ComponentPoolObject<HierarchyLine>* _poolObject)
    {
        
    }

    //////////////////////////////////////////
    void HierarchyLine::processReleased()
    {
        eventRelease(this);
    }

    //////////////////////////////////////////
    void HierarchyLine::setIndent(S32 _indent)
    {
        if (m_indent == _indent)
            return;

        m_indent = _indent;

        updateNodeContainerIndent();
    }

    //////////////////////////////////////////
    void HierarchyLine::updateNodeContainerIndent()
    {
        if (m_nodeContainerSizePolicy)
            m_nodeContainerSizePolicy->setSizeDelta(-m_indent * m_indentWidth, 0.0f);
    }

    //////////////////////////////////////////
    void HierarchyLine::notifyDropDownClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        setExpanded(!getExpanded());
    }

    //////////////////////////////////////////
    void HierarchyLine::notifyLineClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventLineClick(this);
    }

    //////////////////////////////////////////
    void HierarchyLine::notifyLineCursorPressIn(Button2D* _button, Vec2F const& _pos, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventLineCursorPressIn(this);
    }

    //////////////////////////////////////////
    void HierarchyLine::notifyLineDoubleClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventLineDoubleClick(this);
    }

    //////////////////////////////////////////
    void HierarchyLine::notifyLineFocusChanged(Button2D* _button, bool _value)
    {
        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLine::updateDropDownRenderer()
    {
        if (!getEntityRaw() || !m_childrenLayout || !m_childrenLayout->getEntityRaw())
            return;

        if (!m_dropDownRenderer)
            return;

        m_dropDownRenderer->getMeshRenderer()->setEnabled(
            m_childrenLayout->getTransform()->getChildren().size() > 0);


        if (!UIManager::GetInstancePtr())
            return;

        if (m_childrenLayout->getEntityRaw()->getActiveSelf())
            m_dropDownRenderer->setSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded));
        else
            m_dropDownRenderer->setSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonCollapsed));
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(HierarchyLineAppear,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        HierarchyLine* _hierarchyLine,
        ComponentPoolObject<HierarchyLine>* _poolObject)
    {
        _hierarchyLine->processAwake(_poolObject);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(HierarchyLineRemoved,
        {},
        {},
        EntityRemovedEvent const& _event,
        Entity* _entity,
        HierarchyLine* _hierarchyLine,
        ComponentPoolObject<HierarchyLine>* _poolObject)
    {
        _hierarchyLine->release();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(HierarchyLineOnCursorRelease,
        {},
        {},
        InputCursorReleaseEvent const& _event,
        Entity* _entity,
        HierarchyLine* _hierarchyLine)
    {
        // _hierarchyLine->processCursorRelease(_event);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(HierarchyLineOnDragStartedEvent,
        {},
        {},
        UIDragElementDragStartedEvent const& _event,
        Entity* _entity,
        ClickButton2D* _clickButton)
    {
        HierarchyLine* hierarchyLine = _clickButton->getTransform()->getParent()->getEntityRaw()->getComponentRaw<HierarchyLine>();
        if (!hierarchyLine)
            return;

        if (!hierarchyLine->getDragAndDropEnabled())
            return;

        auto dragAndDropControllerSample = _entity->getEcsWorld()->requestInclusiveSample<DragAndDropController>();
        dragAndDropControllerSample->findQuery(
            [&](Entity* _entity, DragAndDropController* _controller)
        {
            SceneDragAndDrop* dndScene = _controller->getDragAndDropScene();
            if (dndScene)
            {
                HierarchyLineType type = hierarchyLine->getType();
                if (type == HierarchyLineType::Entity && hierarchyLine->getWorld())
                {
                    EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(hierarchyLine->getUserData()));
                    EcsWorldId worldId = hierarchyLine->getWorld()->getId();

                    DataBlock dataBlock;
                    dataBlock.setString(MAZE_HCS("type"), "entity");
                    dataBlock.setS8(MAZE_HCS("world"), (S8)worldId);
                    dataBlock.setS32(MAZE_HCS("eid"), (S32)entityId);

                    dndScene->startDrag(
                        hierarchyLine->getIconRenderer()->getEntityRaw()->getComponent<Transform2D>(),
                        dataBlock);
                    _clickButton->getUIElement()->setPressed(false);
                    _clickButton->getUIElement()->setFocused(false);
                }
            }

            return true;
        });
    }

} // namespace Maze
//////////////////////////////////////////
