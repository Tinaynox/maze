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
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4S32.hpp"
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
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerVec4S32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerVec4S32, ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ShaderUniformVariantDrawerVec4S32);

    //////////////////////////////////////////
    ShaderUniformVariantDrawerVec4S32::ShaderUniformVariantDrawerVec4S32()
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerVec4S32::~ShaderUniformVariantDrawerVec4S32()
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

        if (m_editBoxW)
        {
            m_editBoxW->eventTextInput.unsubscribe(this);
            m_editBoxW->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerVec4S32Ptr ShaderUniformVariantDrawerVec4S32::Create(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        ShaderUniformVariantDrawerVec4S32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderUniformVariantDrawerVec4S32, object, init(_shaderUniformName, _shaderUniformType));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawerVec4S32::init(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        if (!ShaderUniformVariantDrawer::init(_shaderUniformName, _shaderUniformType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec4S32::buildUI(
        Transform2DPtr const& _parent,
        CString _label)
    {
        VerticalLayout2DPtr verticalLayout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Right,
            VerticalAlignment2D::Middle,
            Vec2F(_parent->getWidth(), 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        verticalLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        {
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

            AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
                m_shaderUniformName.c_str(),
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                EditorToolsStyles::GetInstancePtr()->getInspectorPropertyFontSize(),
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Middle,
                Vec2F(8, 18),
                Vec2F(0, 0),
                layout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
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
                    _parent->getEntityRaw()->getEcsScene());
                layout2->setSpacing(2.0f);

                m_editBoxX = createF32Edit("X", layout2->getTransform());
                m_editBoxY = createF32Edit("Y", layout2->getTransform());
            }
        }

        {
            HorizontalLayout2DPtr layout2 = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Right,
                VerticalAlignment2D::Middle,
                Vec2F(_parent->getWidth(), 18),
                Vec2F(0, 0),
                verticalLayout->getTransform(),
                _parent->getEntityRaw()->getEcsScene(),
                Vec2F(1.0f, 1.0f),
                Vec2F(1.0f, 1.0f));
            layout2->setSpacing(2.0f);

            m_editBoxZ = createF32Edit("W", layout2->getTransform());
            m_editBoxW = createF32Edit("H", layout2->getTransform());
        }
    }

    //////////////////////////////////////////
    EditBox2DPtr ShaderUniformVariantDrawerVec4S32::createF32Edit(
        CString _name,
        Transform2DPtr const& _parent)
    {
        HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(53, 18),
            Vec2F(0, 0),
            _parent,
            _parent->getEntityRaw()->getEcsScene());
        layout->setSpacing(1.0f);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            _name,
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(6, 18),
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
        editBox->eventTextInput.subscribe(this, &ShaderUniformVariantDrawerVec4S32::notifyTextInput);
        editBox->eventSelectedChanged.subscribe(this, &ShaderUniformVariantDrawerVec4S32::notifySelectedChanged);

        return editBox;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec4S32::notifyTextInput(EditBox2D* _editBox)
    {
        processDataFromUI();
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec4S32::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            processDataFromUI();
        }
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec4S32::processDataToUI()
    {
        ShaderUniformVariant value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        if (!m_editBoxX->getSelected())
            m_editBoxX->setText(StringHelper::ToString(value.getVec4S32().x));

        if (!m_editBoxY->getSelected())
            m_editBoxY->setText(StringHelper::ToString(value.getVec4S32().y));

        if (!m_editBoxZ->getSelected())
            m_editBoxZ->setText(StringHelper::ToString(value.getVec4S32().z));

        if (!m_editBoxW->getSelected())
            m_editBoxW->setText(StringHelper::ToString(value.getVec4S32().w));
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerVec4S32::processDataFromUI()
    {
        Vec4S value;
        value.x = StringHelper::StringToS32(m_editBoxX->getText());
        value.y = StringHelper::StringToS32(m_editBoxY->getText());
        value.z = StringHelper::StringToS32(m_editBoxZ->getText());
        value.w = StringHelper::StringToS32(m_editBoxW->getText());

        if (m_useEditorActions && EditorToolsActionManager::GetInstancePtr())
        {
            ShaderUniformVariant variant(RenderSystem::GetCurrentInstancePtr(), value);
            variant.setName(m_shaderUniformName);
            EditorActionHelper::SetMaterialsUniform(
                m_materials,
                variant);
        }
        else
            for (MaterialPtr const& material : m_materials)
                material->getUniform(m_shaderUniformName)->set(value);
    }

} // namespace Maze
//////////////////////////////////////////
