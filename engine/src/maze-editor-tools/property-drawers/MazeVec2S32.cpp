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
#include "maze-editor-tools/property-drawers/MazeVec2S32.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerVec2S32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerVec2S32, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerVec2S32);

    //////////////////////////////////////////
    PropertyDrawerVec2S32::PropertyDrawerVec2S32()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerVec2S32::~PropertyDrawerVec2S32()
    {
        if (m_editBoxX)
        {
            m_editBoxX->eventTextInput.unsubscribe(this);
            m_editBoxX->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxY)
        {
            m_editBoxY->eventTextInput.unsubscribe(this);
            m_editBoxY->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerVec2S32Ptr PropertyDrawerVec2S32::Create(String const& _label)
    {
        PropertyDrawerVec2S32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerVec2S32, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerVec2S32::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerVec2S32::buildUI(
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
        m_rootEntity = layout->getEntity();

        SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
            EditorToolsHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
            EditorToolsLayout::c_inspectorPropertyFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        systemText->setColor(EditorToolsLayout::c_inspectorPropertyColor);

        {
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(200, 18),
                Vec2F(0, 0),
                layout->getTransform(),
                _parent->getEntityRaw()->getECSScene());
            layout2->setSpacing(2.0f);

            m_editBoxX = createS32Edit("X", layout2->getTransform());
            m_editBoxY = createS32Edit("Y", layout2->getTransform());
        }
    }

    //////////////////////////////////////////
    EditBox2DPtr PropertyDrawerVec2S32::createS32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(53, 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene());
        layout->setSpacing(1.0f);
        

        SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
            _name,
            6,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(6, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        systemText->setColor(ColorU32::c_black);

        EditBox2DPtr editBox = SystemUIHelper::CreateDefaultEditBox(
            "",
            Vec2F(60, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        editBox->eventTextInput.subscribe(this, &PropertyDrawerVec2S32::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerVec2S32::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void PropertyDrawerVec2S32::setString(String const& _value)
    {
        Vec2S value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerVec2S32::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerVec2S32::notifyTextInput(EditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerVec2S32::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerVec2S32::setValue(Vec2S const& _value)
    {
        if (!m_editBoxX->getSelected())
            m_editBoxX->setText(StringHelper::S32ToString(_value.x));

        if (!m_editBoxY->getSelected())
            m_editBoxY->setText(StringHelper::S32ToString(_value.y));
    }

    //////////////////////////////////////////
    Vec2S PropertyDrawerVec2S32::getValue() const
    {
        Vec2S value;
        value.x = StringHelper::StringToS32(m_editBoxX->getText());
        value.y = StringHelper::StringToS32(m_editBoxY->getText());
        return value;
    }

} // namespace Maze
//////////////////////////////////////////
