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
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeColorHDREdit2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerColorF128
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerColorF128, ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ShaderUniformVariantDrawerColorF128);

    //////////////////////////////////////////
    ShaderUniformVariantDrawerColorF128::ShaderUniformVariantDrawerColorF128()
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerColorF128::~ShaderUniformVariantDrawerColorF128()
    {
        if (m_colorEdit)
            m_colorEdit->eventColorChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerColorF128Ptr ShaderUniformVariantDrawerColorF128::Create(
        String const& _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        ShaderUniformVariantDrawerColorF128Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderUniformVariantDrawerColorF128, object, init(_shaderUniformName, _shaderUniformType));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawerColorF128::init(
        String const& _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        if (!ShaderUniformVariantDrawer::init(_shaderUniformName, _shaderUniformType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerColorF128::buildUI(
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
            m_shaderUniformName.c_str(),
            DebuggerLayout::c_inspectorPropertyFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(8, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF::c_zero);
        systemText->setColor(DebuggerLayout::c_inspectorPropertyColor);

        m_colorEdit = UIHelper::CreateDefaultColorHDREdit(
            ColorF128::c_white,
            Vec2DF(60, 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.5f, 0.5f),
            Vec2DF::c_zero);
        m_colorEdit->eventColorChanged.subscribe(this, &ShaderUniformVariantDrawerColorF128::notifyColorChanged);
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerColorF128::notifyColorChanged(ColorHDREdit2D* _colorEdit, ColorF128 const& _color)
    {
        processDataFromUI();
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerColorF128::processDataToUI()
    {
        ShaderUniformVariant value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_colorEdit->setColor(value.getColorF128());
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerColorF128::processDataFromUI()
    {
        ColorF128 color = m_colorEdit->getColor();

        for (MaterialPtr const& material : m_materials)
            material->getUniform(m_shaderUniformName)->set(color);
    }

} // namespace Maze
//////////////////////////////////////////
