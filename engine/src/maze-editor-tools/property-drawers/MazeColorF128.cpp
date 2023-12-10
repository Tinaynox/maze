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
#include "maze-editor-tools/property-drawers/MazeColorF128.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerColorF128
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerColorF128, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerColorF128);

    //////////////////////////////////////////
    PropertyDrawerColorF128::PropertyDrawerColorF128()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerColorF128::~PropertyDrawerColorF128()
    {
        if (m_colorEdit)
            m_colorEdit->eventColorChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    PropertyDrawerColorF128Ptr PropertyDrawerColorF128::Create(String const& _label)
    {
        PropertyDrawerColorF128Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerColorF128, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerColorF128::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerColorF128::buildUI(
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

        m_colorEdit = UIHelper::CreateDefaultColorHDREdit(
            ColorF128::c_white,
            Vec2F(60, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        m_colorEdit->eventColorChanged.subscribe(this, &PropertyDrawerColorF128::notifyColorChanged);
    }

    //////////////////////////////////////////
    void PropertyDrawerColorF128::setString(String const& _value)
    {
        ColorF128 value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerColorF128::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerColorF128::setValue(ColorF128 const& _value)
    {
        m_colorEdit->setColor(_value);
    }

    //////////////////////////////////////////
    ColorF128 PropertyDrawerColorF128::getValue() const
    {
        ColorF128 color = m_colorEdit->getColor();
        return color;
    }

    //////////////////////////////////////////
    void PropertyDrawerColorF128::notifyColorChanged(ColorHDREdit2D* _colorEdit, ColorF128 const& _color)
    {
        eventUIData();
    }

} // namespace Maze
//////////////////////////////////////////
