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
#include "maze-editor-tools/property-drawers/MazePDAABB3D.hpp"
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
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class PropertyDrawerAABB3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PropertyDrawerAABB3D, PropertyDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PropertyDrawerAABB3D);

    //////////////////////////////////////////
    PropertyDrawerAABB3D::PropertyDrawerAABB3D()
    {
        
    }

    //////////////////////////////////////////
    PropertyDrawerAABB3D::~PropertyDrawerAABB3D()
    {
        if (m_editBoxMinX)
        {
            m_editBoxMinX->eventTextInput.unsubscribe(this);
            m_editBoxMinX->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMinY)
        {
            m_editBoxMinY->eventTextInput.unsubscribe(this);
            m_editBoxMinY->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMinZ)
        {
            m_editBoxMinZ->eventTextInput.unsubscribe(this);
            m_editBoxMinZ->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMaxX)
        {
            m_editBoxMaxX->eventTextInput.unsubscribe(this);
            m_editBoxMaxX->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMaxY)
        {
            m_editBoxMaxY->eventTextInput.unsubscribe(this);
            m_editBoxMaxY->eventSelectedChanged.unsubscribe(this);
        }

        if (m_editBoxMaxZ)
        {
            m_editBoxMaxZ->eventTextInput.unsubscribe(this);
            m_editBoxMaxZ->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    PropertyDrawerAABB3DPtr PropertyDrawerAABB3D::Create(DataBlock const& _dataBlock)
    {
        PropertyDrawerAABB3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PropertyDrawerAABB3D, object, init(_dataBlock));
        return object;
    }

    //////////////////////////////////////////
    bool PropertyDrawerAABB3D::init(DataBlock const& _dataBlock)
    {
        if (!PropertyDrawer::init(_dataBlock))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void PropertyDrawerAABB3D::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        VerticalLayout2DPtr verticalLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        verticalLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        {
            AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
                EditorToolsHelper::BuildPropertyName(m_dataBlock.getCString(MAZE_HCS("label")), _label).c_str(),
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(8, 18),
                Vec2F(0, 0),
                verticalLayout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F::c_zero);
            titleText->setColor(EditorToolsStyles::GetInstancePtr()->getInspectorPropertyColor());

            HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(_parent->getWidth(), 18),
                Vec2F(0, 0),
                verticalLayout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 1.0f),
                Vec2F(0.0f, 1.0f));
            layout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
            layout->setAutoWidth(false);
            layout->setExpand(true);

            {
                HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Middle,
                    Vec2F(_parent->getWidth(), 18),
                    Vec2F(0, 0),
                    layout->getTransform(),
                    _parent->getEntityRaw()->getEcsScene());
                layout2->setSpacing(2.0f);

                m_editBoxMinX = createF32Edit("Min X", layout2->getTransform());
                m_editBoxMinY = createF32Edit("Y", layout2->getTransform());
                m_editBoxMinZ = createF32Edit("Z", layout2->getTransform());
            }
        }

        {
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(_parent->getWidth(), 18),
                Vec2F(0, 0),
                verticalLayout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f));
            layout2->setSpacing(2.0f);

            m_editBoxMaxX = createF32Edit("Max X", layout2->getTransform());
            m_editBoxMaxY = createF32Edit("Y", layout2->getTransform());
            m_editBoxMaxZ = createF32Edit("Z", layout2->getTransform());
        }
    }

    ////////////////////////////////////////////
    void PropertyDrawerAABB3D::unselectUI()
    {
        m_editBoxMinX->setSelected(false);
        m_editBoxMinY->setSelected(false);
        m_editBoxMinZ->setSelected(false);
        m_editBoxMaxX->setSelected(false);
        m_editBoxMaxY->setSelected(false);
        m_editBoxMaxZ->setSelected(false);
    }

    //////////////////////////////////////////
    EditBox2DPtr PropertyDrawerAABB3D::createF32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        Size nameLen = strlen(_name);

        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(47, 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene());
        layout->setSpacing(2.0f);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            _name,
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(8.0f * nameLen, 18.0f),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getEcsScene(),
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
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        editBox->eventTextInput.subscribe(this, &PropertyDrawerAABB3D::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &PropertyDrawerAABB3D::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void PropertyDrawerAABB3D::notifyTextInput(EditBox2D* _editBox)
    {
        eventUIData();
    }

    //////////////////////////////////////////
    void PropertyDrawerAABB3D::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            eventUIData();
        }
    }

    //////////////////////////////////////////
    void PropertyDrawerAABB3D::setValue(AABB3D const& _value)
    {
        if (!m_editBoxMinX->getSelected())
            m_editBoxMinX->setText(StringHelper::F32ToStringFormatted(_value.getMinX(), 4));

        if (!m_editBoxMinY->getSelected())
            m_editBoxMinY->setText(StringHelper::F32ToStringFormatted(_value.getMinY(), 4));

        if (!m_editBoxMinZ->getSelected())
            m_editBoxMinZ->setText(StringHelper::F32ToStringFormatted(_value.getMinZ(), 4));

        if (!m_editBoxMaxX->getSelected())
            m_editBoxMaxX->setText(StringHelper::F32ToStringFormatted(_value.getMaxX(), 4));

        if (!m_editBoxMaxY->getSelected())
            m_editBoxMaxY->setText(StringHelper::F32ToStringFormatted(_value.getMaxY(), 4));

        if (!m_editBoxMaxZ->getSelected())
            m_editBoxMaxZ->setText(StringHelper::F32ToStringFormatted(_value.getMaxZ(), 4));
    }

    //////////////////////////////////////////
    AABB3D PropertyDrawerAABB3D::getValue() const
    {
        AABB3D value;
        value.setMinX(StringHelper::StringToF32Safe(m_editBoxMinX->getText()));
        value.setMinY(StringHelper::StringToF32Safe(m_editBoxMinY->getText()));
        value.setMinZ(StringHelper::StringToF32Safe(m_editBoxMinZ->getText()));
        value.setMaxX(StringHelper::StringToF32Safe(m_editBoxMaxX->getText()));
        value.setMaxY(StringHelper::StringToF32Safe(m_editBoxMaxY->getText()));
        value.setMaxZ(StringHelper::StringToF32Safe(m_editBoxMaxZ->getText()));

        return value;
    }

} // namespace Maze
//////////////////////////////////////////
