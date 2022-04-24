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
#include "maze-editor-tools/property-drawers/MazePropertyDrawerSliderRadians.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerSliderRadians
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerSliderRadians, PropertyDrawerSliderF32);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerSliderRadians);

    //////////////////////////////////////////
    PropertyDrawerSliderRadians::PropertyDrawerSliderRadians()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerSliderRadians::~PropertyDrawerSliderRadians()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerSliderRadiansPtr PropertyDrawerSliderRadians::Create(String const& _label)
    {
        PropertyDrawerSliderRadiansPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerSliderRadians, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerSliderRadians::init(String const& _label)
    {
        if (!PropertyDrawerSliderF32::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderRadians::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        PropertyDrawerSliderF32::buildUI(
            _parent,
            _label);
    }

    //////////////////////////////////////////
    void PropertyDrawerSliderRadians::setValue(F32 const& _value)
    {
        F32 degrees = Math::RadiansToDegrees(_value);

        if (!m_editBox->getSelected())
            m_editBox->setText(StringHelper::F32ToStringFormatted(degrees, 4));

        if (!m_slider->getPressed())
            m_slider->setValue((degrees - m_fromValue) / (m_toValue - m_fromValue));
    }

    //////////////////////////////////////////
    F32 PropertyDrawerSliderRadians::getValue() const
    {
        F32 value = StringHelper::StringToF32Safe(m_editBox->getText());

        F32 radians = Math::DegreesToRadians(value);

        return radians;
    }

    //////////////////////////////////////////
    String PropertyDrawerSliderRadians::prepareValueForEditBox(F32 _sliderValue)
    {
        F32 degrees = Math::Lerp(m_fromValue, m_toValue, _sliderValue);
        return StringHelper::ToString(Math::Round((S32)degrees));
    }

} // namespace Maze
//////////////////////////////////////////
