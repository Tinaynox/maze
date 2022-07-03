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
#include "maze-editor-tools/property-drawers/MazeRect2DF.hpp"
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
    // Class PropertyDrawerRect2DF
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerRect2DF, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerRect2DF);

    //////////////////////////////////////////
    PropertyDrawerRect2DF::PropertyDrawerRect2DF()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerRect2DF::~PropertyDrawerRect2DF()
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

        if (m_editBoxW)
        {
            m_editBoxW->eventTextInput.unsubscribe(this);
            m_editBoxW->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxH)
        {
            m_editBoxH->eventTextInput.unsubscribe(this);
            m_editBoxH->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerRect2DFPtr PropertyDrawerRect2DF::Create(String const& _label)
    {
        PropertyDrawerRect2DFPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerRect2DF, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerRect2DF::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerRect2DF::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        VerticalLayout2DPtr verticalLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Right,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        verticalLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        {
            HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2DF(_parent->getWidth(), 18),
                Vec2DF(0, 0),
                verticalLayout->getTransform(),
                _parent->getEntityRaw()->getECSScene(),
                Vec2DF(0.0f, 1.0f),
                Vec2DF(0.0f, 1.0f));
            layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
            layout->setAutoWidth(false);
            layout->setExpand(true);

            SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
                EditorToolsHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
                EditorToolsLayout::c_inspectorPropertyFontSize,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2DF(8, 18),
                Vec2DF(0, 0),
                layout->getTransform(),
                _parent->getEntityRaw()->getECSScene(),
                Vec2DF(0.0f, 0.5f),
                Vec2DF::c_zero);
            systemText->setColor(EditorToolsLayout::c_inspectorPropertyColor);

            {
                HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2DF(200, 18),
                    Vec2DF(0, 0),
                    layout->getTransform(),
                    _parent->getEntityRaw()->getECSScene());
                layout2->setSpacing(2.0f);

                m_editBoxX = createF32Edit("X", layout2->getTransform());
                m_editBoxY = createF32Edit("Y", layout2->getTransform());
            }
        }

        {
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Right,
                VerticalAlignment2D::Middle,
                Vec2DF(_parent->getWidth(), 18),
                Vec2DF(0, 0),
                verticalLayout->getTransform(),
                _parent->getEntityRaw()->getECSScene(),
                Vec2DF(1.0f, 1.0f),
                Vec2DF(1.0f, 1.0f));
            layout2->setSpacing(2.0f);

            m_editBoxW = createF32Edit("W", layout2->getTransform());
            m_editBoxH = createF32Edit("H", layout2->getTransform());
        }
    }

    //////////////////////////////////////////
    EditBox2DPtr PropertyDrawerRect2DF::createF32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(53, 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene());
        layout->setSpacing(1.0f);

        SystemTextRenderer2DPtr systemText = SystemUIHelper::CreateSystemText(
            _name,
            6,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(6, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(ColorU32::c_black);

        EditBox2DPtr editBox = SystemUIHelper::CreateDefaultEditBox(
            "",
            Vec2DF(60, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        editBox->eventTextInput.subscribe(this, &PropertyDrawerRect2DF::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerRect2DF::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void PropertyDrawerRect2DF::setString(String const& _value)
    {
        Rect2DF value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerRect2DF::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerRect2DF::notifyTextInput(EditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerRect2DF::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerRect2DF::setValue(Rect2DF const& _value)
    {
        if (!m_editBoxX->getSelected())
            m_editBoxX->setText(StringHelper::F32ToStringFormatted(_value.position.x, 4));

        if (!m_editBoxY->getSelected())
            m_editBoxY->setText(StringHelper::F32ToStringFormatted(_value.position.y, 4));

        if (!m_editBoxW->getSelected())
            m_editBoxW->setText(StringHelper::F32ToStringFormatted(_value.size.x, 4));

        if (!m_editBoxH->getSelected())
            m_editBoxH->setText(StringHelper::F32ToStringFormatted(_value.size.y, 4));
    }

    //////////////////////////////////////////
    Rect2DF PropertyDrawerRect2DF::getValue() const
    {
        Rect2DF value;
        value.position.x = StringHelper::StringToF32Safe(m_editBoxX->getText());
        value.position.y = StringHelper::StringToF32Safe(m_editBoxY->getText());
        value.size.x = StringHelper::StringToF32Safe(m_editBoxW->getText());
        value.size.y = StringHelper::StringToF32Safe(m_editBoxH->getText());

        return value;
    }

} // namespace Maze
//////////////////////////////////////////
