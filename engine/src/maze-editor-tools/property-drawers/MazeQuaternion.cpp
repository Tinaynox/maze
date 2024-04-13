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
#include "maze-editor-tools/property-drawers/MazeQuaternion.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


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
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(200, 18),
                Vec2F(0, 0),
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
    EditBox2DPtr PropertyDrawerQuaternion::createF32Edit(
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
        layout->setSpacing(2.0f);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            _name,
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(6, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F::c_zero);
        titleText->setColor(ColorU32::c_black);

        EditBox2DPtr editBox = UIHelper::CreateDefaultEditBox(
            "",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            Vec2F(60, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        editBox->eventTextInput.subscribe(this, &PropertyDrawerQuaternion::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerQuaternion::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::notifyTextInput(EditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerQuaternion::setValue(Quaternion const& _value)
    {
        F32 pitch = _value.getPitch();
        F32 yaw = _value.getYaw();
        F32 roll = _value.getRoll();

        if (!m_editBoxX->getSelected() && !m_editBoxY->getSelected() && !m_editBoxZ->getSelected())
        {
            F32 pitchDegrees = Math::RadiansToDegrees(pitch);
            F32 yawDegrees = Math::RadiansToDegrees(yaw);
            F32 rollDegrees = Math::RadiansToDegrees(roll);
            
            m_editBoxX->setText(StringHelper::F32ToStringFormatted(pitchDegrees, 4));
            m_editBoxY->setText(StringHelper::F32ToStringFormatted(yawDegrees, 4));
            m_editBoxZ->setText(StringHelper::F32ToStringFormatted(rollDegrees, 4));
        }
    }

    //////////////////////////////////////////
    Quaternion PropertyDrawerQuaternion::getValue() const
    {
        F32 pitchDegrees = StringHelper::StringToF32Safe(m_editBoxX->getText());
        F32 yawDegrees = StringHelper::StringToF32Safe(m_editBoxY->getText());
        F32 rollDegrees = StringHelper::StringToF32Safe(m_editBoxZ->getText());

        F32 pitch = Math::DegreesToRadians(pitchDegrees);
        F32 yaw = Math::DegreesToRadians(yawDegrees);
        F32 roll = Math::DegreesToRadians(rollDegrees);

        Quaternion q = Quaternion(pitch, yaw, roll);

        return q;
    }

} // namespace Maze
//////////////////////////////////////////
