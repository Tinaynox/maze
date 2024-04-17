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
#include "MazeParticlesEditorToolsHeader.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazeParticleSystemBurst.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
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
    // Class PropertyDrawerParticleSystemBurst
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerParticleSystemBurst, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerParticleSystemBurst);

    //////////////////////////////////////////
    PropertyDrawerParticleSystemBurst::PropertyDrawerParticleSystemBurst()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemBurst::~PropertyDrawerParticleSystemBurst()
    {
        if (m_editBoxTime)
        {
            m_editBoxTime->eventTextInput.unsubscribe(this);
            m_editBoxTime->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMinCount)
        {
            m_editBoxMinCount->eventTextInput.unsubscribe(this);
            m_editBoxMinCount->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMaxCount)
        {
            m_editBoxMaxCount->eventTextInput.unsubscribe(this);
            m_editBoxMaxCount->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerParticleSystemBurstPtr PropertyDrawerParticleSystemBurst::Create(String const& _label)
    {
        PropertyDrawerParticleSystemBurstPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerParticleSystemBurst, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerParticleSystemBurst::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemBurst::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);
        layout->setSpacing(2.0f);
        m_rootEntity = layout->getEntity();

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            EditorToolsHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        titleText->setColor(EditorToolsStyles::GetInstancePtr()->getInspectorPropertyColor());

        m_layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_layout->setAutoWidth(true);
        m_layout->setAutoHeight(false);
        m_layout->setExpand(false);
        m_layout->setSpacing(4.0f);

        m_editBoxTime = createEdit("Tm", m_layout->getTransform());
        m_editBoxMinCount = createEdit("Mn", m_layout->getTransform());
        m_editBoxMaxCount = createEdit("Mx", m_layout->getTransform());
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemBurst::setValue(ParticleSystemBurst const& _value)
    {
        if (!m_editBoxTime->getSelected())
            m_editBoxTime->setText(StringHelper::F32ToStringFormatted(_value.time, 4));

        if (!m_editBoxMinCount->getSelected())
            m_editBoxMinCount->setText(StringHelper::S32ToString(_value.minCount));

        if (!m_editBoxMaxCount->getSelected())
            m_editBoxMaxCount->setText(StringHelper::S32ToString(_value.maxCount));
    }

    //////////////////////////////////////////
    ParticleSystemBurst PropertyDrawerParticleSystemBurst::getValue() const
    {
        ParticleSystemBurst value;
        value.time = StringHelper::StringToF32(m_editBoxTime->getText());
        value.minCount = StringHelper::StringToS32(m_editBoxMinCount->getText());
        value.maxCount = StringHelper::StringToS32(m_editBoxMaxCount->getText());
        return value;
    }

    //////////////////////////////////////////
    EditBox2DPtr PropertyDrawerParticleSystemBurst::createEdit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(60, 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene());
        layout->setSpacing(2.0f);

        AbstractTextRenderer2DPtr systemText = EditorToolsUIHelper::CreateText(
            _name,
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            10,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(12, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        systemText->setColor(ColorU32::c_black);

        EditBox2DPtr editBox = UIHelper::CreateDefaultEditBox(
            "",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            Vec2F(40, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        editBox->eventTextInput.subscribe(this, &PropertyDrawerParticleSystemBurst::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerParticleSystemBurst::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemBurst::notifyTextInput(EditBox2D* _edit)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemBurst::notifySelectedChanged(EditBox2D* _edit, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerParticleSystemBurst::resetSelection()
    {
        m_editBoxTime->setSelected(false);
        m_editBoxMinCount->setSelected(false);
        m_editBoxMaxCount->setSelected(false);
    }


} // namespace Maze
//////////////////////////////////////////
