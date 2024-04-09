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
#include "maze-editor-tools/property-drawers/MazeSliderF32.hpp"
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
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerSliderF32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerSliderF32, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerSliderF32);

    //////////////////////////////////////////
    PropertyDrawerSliderF32::PropertyDrawerSliderF32()
        : m_fromValue(1.0f)
        , m_toValue(100.0f)
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerSliderF32::~PropertyDrawerSliderF32()
    {
        if (m_editBox)
        {
            m_editBox->eventTextInput.unsubscribe(this);
            m_editBox->eventSelectedChanged.unsubscribe(this);
        }

        if (m_slider)
        {
            m_slider->eventValueChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerSliderF32Ptr PropertyDrawerSliderF32::Create(String const& _label)
    {
        PropertyDrawerSliderF32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerSliderF32, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerSliderF32::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderF32::buildUI(
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

        {
            HorizontalLayout2DPtr sliderLayout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(200.0f, 18.0f),
                Vec2F(0, 0),
                layout->getTransform(),
                _parent->getEntityRaw()->getECSScene(),
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            sliderLayout->setAutoWidth(true);
            sliderLayout->setExpand(false);
            sliderLayout->setSpacing(10.0f);

            m_slider = UIHelper::CreateDefaultSlider(
                0.0f,
                Vec2F(100, 18),
                Vec2F(0, 0),
                sliderLayout->getTransform(),
                _parent->getEntityRaw()->getECSScene());
            m_slider->eventValueChanged.subscribe(this, &PropertyDrawerSliderF32::notifySliderValueChanged);

            m_editBox = SystemUIHelper::CreateDefaultEditBox(
                "",
                Vec2F(60, 18),
                Vec2F(0, 0),
                sliderLayout->getTransform(),
                _parent->getEntityRaw()->getECSScene(),
                Vec2F(0.5f, 0.5f),
                Vec2F::c_zero);
            m_editBox->eventTextInput.subscribe(this, &PropertyDrawerSliderF32::notifyTextInput);
            m_editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerSliderF32::notifySelectedChanged);
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderF32::setString(String const& _value)
    {
        F32 value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerSliderF32::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderF32::notifyTextInput(EditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderF32::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderF32::notifySliderValueChanged(Slider2D* _slider, F32 _value)
    {
        String value = prepareValueForEditBox(_value);
        m_editBox->setText(value);
        eventUIData();
    }

    //////////////////////////////////////////
    String PropertyDrawerSliderF32::prepareValueForEditBox(F32 _sliderValue)
    {
        F32 degrees = Math::Lerp(m_fromValue, m_toValue, _sliderValue);
        return StringHelper::F32ToStringFormatted(degrees);
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderF32::setValue(F32 const& _value)
    {
        if (!m_editBox->getSelected())
            m_editBox->setText(StringHelper::F32ToStringFormatted(_value, 4));

        if (!m_slider->getPressed())
            m_slider->setValue((_value - m_fromValue) / (m_toValue - m_fromValue));
    }

    //////////////////////////////////////////
    F32 PropertyDrawerSliderF32::getValue() const
    {
        F32 value = StringHelper::StringToF32Safe(m_editBox->getText());
        return value;
    }


} // namespace Maze
//////////////////////////////////////////
