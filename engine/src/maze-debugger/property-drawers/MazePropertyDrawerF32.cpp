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
#include "maze-debugger/property-drawers/MazePropertyDrawerF32.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerF32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerF32, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerF32);

    //////////////////////////////////////////
    PropertyDrawerF32::PropertyDrawerF32()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerF32::~PropertyDrawerF32()
    {
        if (m_editBox)
        {
            m_editBox->eventTextInput.unsubscribe(this);
            m_editBox->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerF32Ptr PropertyDrawerF32::Create(String const& _label)
    {
        PropertyDrawerF32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerF32, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerF32::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerF32::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        layout->setAutoWidth(false);
        layout->setExpand(true);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            DebuggerHelper::BuildPropertyName(m_label.c_str(), _label).c_str(),
            DebuggerLayout::c_inspectorPropertyFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),
            materialManager->getColorTextureMaterial(),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(DebuggerLayout::c_inspectorPropertyColor);

        m_editBox = UIHelper::CreateDefaultEditBox(
            "",
            Vec2DF(60, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        m_editBox->eventTextInput.subscribe(this, &PropertyDrawerF32::notifyTextInput);
        m_editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerF32::notifySelectedChanged);
    }

    //////////////////////////////////////////
    void PropertyDrawerF32::setString(String const& _value)
    {
        F32 value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerF32::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    void PropertyDrawerF32::notifyTextInput(SystemTextEditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerF32::notifySelectedChanged(SystemTextEditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerF32::setValue(F32 const& _value)
    {
        if (!m_editBox->getSelected())
            m_editBox->setText(StringHelper::F32ToStringFormatted(_value, 4));
    }

    //////////////////////////////////////////
    F32 PropertyDrawerF32::getValue() const
    {
        F32 value = StringHelper::StringToF32(m_editBox->getText());
        return value;
    }

} // namespace Maze
//////////////////////////////////////////
