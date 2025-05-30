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
#include "maze-editor-tools/property-drawers/MazePDComponentPtr.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerComponentPtr
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerComponentPtr, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerComponentPtr);

    //////////////////////////////////////////
    PropertyDrawerComponentPtr::PropertyDrawerComponentPtr()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerComponentPtr::~PropertyDrawerComponentPtr()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerComponentPtrPtr PropertyDrawerComponentPtr::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerComponentPtrPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerComponentPtr, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerComponentPtr::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        m_componentId = _dataBlock.getS32(MAZE_HCS("componentId"), c_invalidComponentId);

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerComponentPtr::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            EditorToolsHelper::BuildPropertyName(m_dataBlock.getCString(MAZE_HCS("label")), _label).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        titleText->setColor(EditorToolsStyles::GetInstancePtr()->getInspectorPropertyColor());

        m_panelRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Default),
            Vec2F(180, 18),
            Vec2F(0, 0),
            nullptr,
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);

        m_dragAndDropFrame = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel00Focused),
            m_panelRenderer->getTransform()->getSize(),
            Vec2F::c_zero,
            nullptr,
            m_panelRenderer->getTransform(),
            _parent->getEntityRaw()->getEcsScene());
        m_dragAndDropFrame->setColor(255, 200, 40);
        m_dragAndDropFrame->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_dragAndDropFrame->getMeshRenderer()->setEnabled(false);

        m_dragAndDropZone = m_panelRenderer->getEntityRaw()->ensureComponent<DragAndDropZone>();
        m_dragAndDropZone->eventDragAndDropValidate.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid, bool& _outDropAllowed)
        {
            if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("entity"))
            {
                EntityId eid(_data.getS32(MAZE_HCS("eid")));
                if (eid == c_invalidEntityId)
                    return;

                EcsWorldId worldId(_data.getS8(MAZE_HCS("world")));
                EcsWorld* world = EcsWorld::GetEcsWorld(worldId);
                if (!world)
                    return;

                EntityPtr const& entity = world->getEntity(eid);
                if (!entity)
                    return;

                ComponentPtr const& component = entity->getComponentInheritedFrom(m_componentId);
                if (!component)
                    return;

                _outDropAllowed = true;
            }
        });
        m_dragAndDropZone->eventDragAndDrop.subscribe(
            [this](DataBlock const& _data, EntityId _viewEid)
        {
            if (_data.getHashedCString(MAZE_HCS("type")) == MAZE_HCS("entity"))
            {
                EntityId eid(_data.getS32(MAZE_HCS("eid")));
                if (eid == c_invalidEntityId)
                    return;

                EcsWorldId worldId(_data.getS8(MAZE_HCS("world")));
                EcsWorld* world = EcsWorld::GetEcsWorld(worldId);
                if (!world)
                    return;

                EntityPtr const& entity = world->getEntity(eid);
                if (!entity)
                    return;

                ComponentPtr const& component = entity->getComponentInheritedFrom(m_componentId);
                setValue(component);
                eventUIData();
            }
        });
        m_dragAndDropZone->eventDragAndDropZoneOnDragAndDropCurrentZoneChanged.subscribe(
            [this](bool _active)
        {
            if (this->m_dragAndDropFrame->getMeshRenderer())
                this->m_dragAndDropFrame->getMeshRenderer()->setEnabled(_active);
        });

        m_text = EditorToolsUIHelper::CreateText(
            EditorToolsHelper::BuildPropertyName(m_dataBlock.getCString(MAZE_HCS("label")), _label).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(0, 18),
            Vec2F(3, 0),
            m_panelRenderer->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_text->setColor(ColorU32::c_black);

        SizePolicy2DPtr textSizePolicy = m_text->getEntityRaw()->ensureComponent<SizePolicy2D>();
        textSizePolicy->setSizeDelta(-10.0f, 0.0f);
    }

    ////////////////////////////////////////////
    void PropertyDrawerComponentPtr::unselectUI()
    {
        
    }

    //////////////////////////////////////////
    //void PropertyDrawerComponentPtr::notifyTextInput(EditBox2D* _editBox)
    //{
    //    eventUIData();
    //}

    ////////////////////////////////////////////
    //void PropertyDrawerComponentPtr::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    //{
    //    if (!_selected)
    //    {
    //        eventUIData();
    //    }
    //}

    //////////////////////////////////////////
    void PropertyDrawerComponentPtr::setValue(ComponentPtr const& _value)
    {
        if (_value && m_componentId != c_invalidComponentId && 
            !(_value->getComponentId() == m_componentId || _value->getMetaClass()->isInheritedFrom(m_componentId)))
            return;

        bool isValid = _value && _value->getEntityRaw() && _value && _value->getEntityRaw()->getEcsWorld();
        m_worldId = isValid ? _value->getEntityRaw()->getEcsWorld()->getId() : EcsWorldId(0);
        m_entityId = isValid ? _value->getEntityRaw()->getId() : c_invalidEntityId;
        CString name = isValid ? EcsHelper::GetName(_value->getEntityRaw()) : "None";

        m_text->setTextFormatted("%s [%d]", name, (S32)m_entityId);
    }

    //////////////////////////////////////////
    ComponentPtr PropertyDrawerComponentPtr::getValue() const
    {
        EcsWorld* world = EcsWorld::GetEcsWorld(m_worldId);
        if (!world)
            return ComponentPtr();

        EntityPtr const& entity = world->getEntity(m_entityId);
        if (!entity)
            return ComponentPtr();

        return entity->getComponentInheritedFrom(m_componentId);
    }


} // namespace Maze
//////////////////////////////////////////
