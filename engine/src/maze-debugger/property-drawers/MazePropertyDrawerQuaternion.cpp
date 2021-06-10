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
#include "maze-debugger/property-drawers/MazePropertyDrawerQuaternion.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
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
    // Class PropertyDrawerQuaternion
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerQuaternion, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerQuaternion);

    //////////////////////////////////////////
    PropertyDrawerQuaternion::PropertyDrawerQuaternion()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerQuaternion::~PropertyDrawerQuaternion()
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

        if (m_editBoxZ)
        {
            m_editBoxZ->eventTextInput.unsubscribe(this);
            m_editBoxZ->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerQuaternionPtr PropertyDrawerQuaternion::Create(String const& _label)
    {
        PropertyDrawerQuaternionPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerQuaternion, object, init(_label));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerQuaternion::init(String const& _label)
    {
        if (!PropertyDrawer::init(_label))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::buildUI(
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
            m_editBoxZ = createF32Edit("Z", layout2->getTransform());
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::setString(String const& _value)
    {
        Quaternion value;
        ValueFromString(value, _value.c_str(), _value.size());
        setValue(value);
    }

    //////////////////////////////////////////
    String PropertyDrawerQuaternion::getString()
    {
        String value;
        ValueToString(getValue(), value);
        return value;
    }

    //////////////////////////////////////////
    SystemTextEditBox2DPtr PropertyDrawerQuaternion::createF32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(53, 18),
            Vec2DF(0, 0),
            _parent,
            _parent->getEntityRaw()->getECSScene());
        layout->setSpacing(1.0f);

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            _name,
            6,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(6, 18),
            Vec2DF(0, 0),
            materialManager->getColorTextureMaterial(),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(ColorU32::c_black);

        SystemTextEditBox2DPtr editBox = UIHelper::CreateDefaultEditBox(
            "",
            Vec2DF(60, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        editBox->eventTextInput.subscribe(this, &PropertyDrawerQuaternion::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerQuaternion::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::notifyTextInput(SystemTextEditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::notifySelectedChanged(SystemTextEditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::setValue(Quaternion const& _value)
    {
        F32 roll = _value.getRoll(false);
        F32 pitch = _value.getPitch(false);
        F32 yaw = _value.getYaw(false);

        if (!m_editBoxX->getSelected())
            m_editBoxX->setText(StringHelper::F32ToStringFormatted(Math::RadiansToDegrees(roll), 4));

        if (!m_editBoxY->getSelected())
            m_editBoxY->setText(StringHelper::F32ToStringFormatted(Math::RadiansToDegrees(pitch), 4));

        if (!m_editBoxZ->getSelected())
            m_editBoxZ->setText(StringHelper::F32ToStringFormatted(Math::RadiansToDegrees(yaw), 4));
    }

    //////////////////////////////////////////
    Quaternion PropertyDrawerQuaternion::getValue() const
    {
        F32 roll = StringHelper::StringToF32(m_editBoxX->getText());
        F32 pitch = StringHelper::StringToF32(m_editBoxY->getText());
        F32 yaw = StringHelper::StringToF32(m_editBoxZ->getText());
        Quaternion q(roll, pitch, yaw);

        return q;
    }

} // namespace Maze
//////////////////////////////////////////
