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
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DU.hpp"
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
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerVec3DU
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerVec3DU, ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ShaderUniformVariantDrawerVec3DU);

    //////////////////////////////////////////
    ShaderUniformVariantDrawerVec3DU::ShaderUniformVariantDrawerVec3DU()
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerVec3DU::~ShaderUniformVariantDrawerVec3DU()
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
    ShaderUniformVariantDrawerVec3DUPtr ShaderUniformVariantDrawerVec3DU::Create(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        ShaderUniformVariantDrawerVec3DUPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderUniformVariantDrawerVec3DU, object, init(_shaderUniformName, _shaderUniformType));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawerVec3DU::init(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        if (!ShaderUniformVariantDrawer::init(_shaderUniformName, _shaderUniformType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec3DU::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
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
            m_shaderUniformName.c_str(),
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
            m_editBoxZ = createF32Edit("Z", layout2->getTransform());
        }
    }

    //////////////////////////////////////////
    SystemTextEditBox2DPtr ShaderUniformVariantDrawerVec3DU::createF32Edit(
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

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
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

        SystemTextEditBox2DPtr editBox = UIHelper::CreateDefaultEditBox(
            "",
            Vec2DF(60, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        editBox->eventTextInput.subscribe(this, &ShaderUniformVariantDrawerVec3DU::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &ShaderUniformVariantDrawerVec3DU::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec3DU::notifyTextInput(SystemTextEditBox2D* _editBox)
    {
        processDataFromUI();
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec3DU::notifySelectedChanged(SystemTextEditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            processDataFromUI();
        }
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec3DU::processDataToUI()
    {
        ShaderUniformVariant value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        if (!m_editBoxX->getSelected())
            m_editBoxX->setText(StringHelper::ToString(value.getVec3DU().x));

        if (!m_editBoxY->getSelected())
            m_editBoxY->setText(StringHelper::ToString(value.getVec3DU().y));

        if (!m_editBoxZ->getSelected())
            m_editBoxZ->setText(StringHelper::ToString(value.getVec3DU().z));
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec3DU::processDataFromUI()
    {
        Vec3DU value;
        value.x = StringHelper::StringToU32(m_editBoxX->getText());
        value.y = StringHelper::StringToU32(m_editBoxY->getText());
        value.z = StringHelper::StringToU32(m_editBoxZ->getText());

        for (MaterialPtr const& material : m_materials)
            material->getUniform(m_shaderUniformName)->set(value);
    }

} // namespace Maze
//////////////////////////////////////////