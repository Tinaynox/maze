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
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ComponentEditor
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ComponentEditor);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ComponentEditor);

    //////////////////////////////////////////
    ComponentEditor::ComponentEditor()
    {
        
    }

    //////////////////////////////////////////
    ComponentEditor::~ComponentEditor()
    {
        if (m_expandButton)
            m_expandButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    bool ComponentEditor::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ComponentEditor::buildEditor(Transform2DPtr const& _parent)
    {
        removeEditor();

        m_editorRoot = createDefaultEditorRoot(_parent);
        m_titleRoot = createDefaultTitle(m_editorRoot);
        m_bodyRoot = createDefaultBody(m_editorRoot);

        updateExpandButtonSprite();

        buildUI();
    }

    //////////////////////////////////////////
    void ComponentEditor::removeEditor()
    {
        if (!m_editorRoot)
            return;

        m_editorRoot->resetParent();
        m_editorRoot->getEntityRaw()->removeFromECSWorld();
        m_editorRoot.reset();

        m_propertyDrawers.clear();
    }

    //////////////////////////////////////////
    Transform2DPtr ComponentEditor::createDefaultEditorRoot(Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Top,
            Vec2DF(_parent->getWidth(), 100.0f),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setControlChildHeight(false);

        return layout->getTransform();
    }

    //////////////////////////////////////////
    Transform2DPtr ComponentEditor::createDefaultTitle(Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        SpriteRenderer2DPtr spriteRenderer = SpriteHelper::CreateSprite(
            ColorU32(200, 200, 200),
            Vec2DF(_parent->getWidth(), 20),
            Vec2DF(0, 0),
            materialManager->getColorTextureMaterial(),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        spriteRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        SpriteRenderer2DPtr lineRenderer = SpriteHelper::CreateSprite(
            ColorU32(127, 127, 127),
            Vec2DF(spriteRenderer->getTransform()->getWidth(), 1),
            Vec2DF(0, 0),
            materialManager->getColorTextureMaterial(),
            spriteRenderer->getTransform(),
            spriteRenderer->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        lineRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        SpriteRenderer2DPtr lineRenderer2 = SpriteHelper::CreateSprite(
            ColorU32(186, 186, 186),
            Vec2DF(spriteRenderer->getTransform()->getWidth(), 1),
            Vec2DF(0, 0),
            materialManager->getColorTextureMaterial(),
            spriteRenderer->getTransform(),
            spriteRenderer->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.0f),
            Vec2DF(0.0f, 0.0f));
        lineRenderer2->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        m_expandButtonSprite = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2DF(8, 8) * 1.75f,
            Vec2DF(10, 1),
            materialManager->getColorTextureMaterial(),
            spriteRenderer->getTransform(),
            spriteRenderer->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.5f, 0.5f));
        m_expandButtonSprite->setColor(ColorU32::c_black);
        m_expandButton = m_expandButtonSprite->getEntityRaw()->ensureComponent<ClickButton2D>();
        m_expandButton->eventClick.subscribe(this, &ComponentEditor::notifyExpandButtonClick);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            DebuggerHelper::BuildComponentName(getComponentMetaClass()->getName()).c_str(),
            9,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(20, 0),
            materialManager->getColorTextureMaterial(),
            spriteRenderer->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.0f, 0.5f));
        systemText->setColor(ColorU32::c_black);

        
        m_contextMenu = spriteRenderer->getEntityRaw()->ensureComponent<ContextMenu2D>();
        m_contextMenu->setCallbackFunction(
            [this](MenuListTree2DPtr const& _menuListTree)
            {
                _menuListTree->addItem(
                    "Remove Component",
                    [this](String const& _item)
                    {
                        eventRemoveComponentPressed(getComponentUID());
                    });
            });
        
        return spriteRenderer->getTransform();
    }

    //////////////////////////////////////////
    void ComponentEditor::notifyExpandButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        m_bodyRoot->getEntityRaw()->setActiveSelf(!m_bodyRoot->getEntityRaw()->getActiveSelf());

        updateExpandButtonSprite();
    }

    //////////////////////////////////////////
    void ComponentEditor::updateExpandButtonSprite()
    {
        if (m_bodyRoot->getEntityRaw()->getActiveSelf())
            m_expandButtonSprite->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded));
        else
            m_expandButtonSprite->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonCollapsed));
    }

    //////////////////////////////////////////
    Transform2DPtr ComponentEditor::createDefaultBody(Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        F32 const paddingLeft = 10.0f;
        F32 const paddingRight = 3.0f;
        F32 const paddingBottom = 5.0f;
        F32 const paddingTop = 5.0f;

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            Vec2DF(_parent->getWidth() - paddingLeft - paddingRight, 0.0f),
            Vec2DF(paddingLeft, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        SizePolicy2DPtr layoutSizePolicy = layout->getEntityRaw()->ensureComponent<SizePolicy2D>();
        layoutSizePolicy->setFlag(SizePolicy2D::Height, false);
        layoutSizePolicy->setSizeDelta(-paddingLeft - paddingRight, 0.0f);
        layout->setSpacing(2.0f);
        
        layout->setPadding(
            0, 0,
            paddingBottom, paddingTop);
        

        return layout->getTransform();
    }

    //////////////////////////////////////////
    void ComponentEditor::buildUI()
    {
        Vector<MetaClass*> const& componentSuperMetaClasses = getComponentMetaClass()->getAllSuperMetaClasses();
        for (MetaClass* metaClass : componentSuperMetaClasses)
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                MetaProperty* metaProperty = metaClass->getProperty(i);
                buildPropertyDrawer(metaProperty);
            }
        }

        m_expandButtonSprite->getEntityRaw()->setActiveSelf(!m_propertyDrawers.empty());
    }

    //////////////////////////////////////////
    MetaPropertyDrawerPtr ComponentEditor::buildPropertyDrawer(
        MetaProperty* _metaProperty,
        CString _label)
    {
        MetaPropertyDrawerPtr property;

        auto const& it = m_customPropertyDrawers.find(_metaProperty);
        if (it != m_customPropertyDrawers.end())
        {
            property = it->second(_metaProperty);
        }
        else
        {
            property = InspectorManager::GetInstancePtr()->createMetaPropertyDrawer(_metaProperty);
        }

        if (property)
        {
            property->buildUI(m_bodyRoot, _label);
            m_propertyDrawers.push_back(property);
        }

        return property;
    }

    //////////////////////////////////////////
    void ComponentEditor::updatePropertyValues(Set<ComponentPtr> const& _components)
    {
        Set<MetaInstance> metaInstances;
        for (ComponentPtr component : _components)
            metaInstances.insert(component->getMetaInstance());

        for (MetaPropertyDrawerPtr const& propertyDrawer : m_propertyDrawers)
        {
            propertyDrawer->linkMetaInstances(metaInstances);
            propertyDrawer->processDataToUI();
        }
    }

} // namespace Maze
//////////////////////////////////////////
