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
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
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
    // Class ShaderUniformVariantDrawerS32
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerS32, ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ShaderUniformVariantDrawerS32);

    //////////////////////////////////////////
    ShaderUniformVariantDrawerS32::ShaderUniformVariantDrawerS32()
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerS32::~ShaderUniformVariantDrawerS32()
    {
        if (m_editBox)
        {
            m_editBox->eventTextInput.unsubscribe(this);
            m_editBox->eventSelectedChanged.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerS32Ptr ShaderUniformVariantDrawerS32::Create(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        ShaderUniformVariantDrawerS32Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderUniformVariantDrawerS32, object, init(_shaderUniformName, _shaderUniformType));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawerS32::init(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        if (!ShaderUniformVariantDrawer::init(_shaderUniformName, _shaderUniformType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerS32::buildUI(
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
            m_shaderUniformName.c_str(),
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

        m_editBox = SystemUIHelper::CreateDefaultEditBox(
            "",
            Vec2F(60, 18),
            Vec2F(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2F(0.5f, 0.5f),
            Vec2F::c_zero);
        m_editBox->eventTextInput.subscribe(this, &ShaderUniformVariantDrawerS32::notifyTextInput);
        m_editBox->eventSelectedChanged.subscribe(this, &ShaderUniformVariantDrawerS32::notifySelectedChanged);
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerS32::notifyTextInput(EditBox2D* _editBox)
    {
        processDataFromUI();
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerS32::notifySelectedChanged(EditBox2D* _editBox, bool _selected)
    {
        if (!_selected)
        {
            processDataFromUI();
        }
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerS32::processDataToUI()
    {
        ShaderUniformVariant value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        if (!m_editBox->getSelected())
            m_editBox->setText(StringHelper::S32ToString(value.getS32()));
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerS32::processDataFromUI()
    {
        S32 value = StringHelper::StringToS32(m_editBox->getText());

        for (MaterialPtr const& material : m_materials)
            material->getUniform(m_shaderUniformName)->set(value);
    }

} // namespace Maze
//////////////////////////////////////////
