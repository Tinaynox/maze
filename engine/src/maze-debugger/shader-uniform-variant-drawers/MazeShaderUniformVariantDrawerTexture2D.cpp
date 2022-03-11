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
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeTexturePickerManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-debugger/layout/MazeDebuggerLayout.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class ShaderUniformVariantDrawerTexture2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderUniformVariantDrawerTexture2D, ShaderUniformVariantDrawer);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ShaderUniformVariantDrawerTexture2D);

    //////////////////////////////////////////
    ShaderUniformVariantDrawerTexture2D::ShaderUniformVariantDrawerTexture2D()
    {
        
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerTexture2D::~ShaderUniformVariantDrawerTexture2D()
    {
        if (m_selectAssetButton)
            m_selectAssetButton->eventClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    ShaderUniformVariantDrawerTexture2DPtr ShaderUniformVariantDrawerTexture2D::Create(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        ShaderUniformVariantDrawerTexture2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderUniformVariantDrawerTexture2D, object, init(_shaderUniformName, _shaderUniformType));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformVariantDrawerTexture2D::init(
        HashedCString _shaderUniformName,
        ShaderUniformType _shaderUniformType)
    {
        if (!ShaderUniformVariantDrawer::init(_shaderUniformName, _shaderUniformType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerTexture2D::buildUI(
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


        HorizontalLayout2DPtr textureBlockLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(_parent->getWidth(), 18),
            Vec2DF(0, 0),
            layout->getTransform(),
            _parent->getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        textureBlockLayout->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);
        textureBlockLayout->setAutoWidth(true);
        textureBlockLayout->setExpand(false);

        m_selectAssetButton = UIHelper::CreateClickButton(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::SelectAsset),
            Vec2DF(18, 18),
            Vec2DF::c_zero,
            textureBlockLayout->getTransform(),
            _parent->getEntityRaw()->getECSScene());
        m_selectAssetButton->setNormalColor({ 50, 50, 50 });
        m_selectAssetButton->setFocusedColor({ 100, 100, 100 });
        m_selectAssetButton->setSelectedColor({ 150, 150, 150 });
        m_selectAssetButton->setPressedColor({ 100, 150, 100 });
        m_selectAssetButton->eventClick.subscribe(this, &ShaderUniformVariantDrawerTexture2D::notifySelectAssetClick);

        SpriteRenderer2DPtr textureHolder = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(18, 18),
            Vec2DF::c_zero,
            materialManager->getColorMaterial(),
            textureBlockLayout->getTransform(),
            _parent->getEntityRaw()->getECSScene());
        textureHolder->setColor(ColorU32(50, 50, 50));
        
        m_sprite = Sprite::Create();

        m_textureRenderer = SpriteHelper::CreateSprite(
            m_sprite,
            Vec2DF(16, 16),
            Vec2DF::c_zero,
            materialManager->getColorTextureMaterial(),
            textureHolder->getTransform(),
            _parent->getEntityRaw()->getECSScene());
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerTexture2D::processDataToUI()
    {
        ShaderUniformVariant value;
        bool isMultiValue;
        fetchPropertyValue(value, isMultiValue);

        m_texture = value.getTexture2D();

        Texture2DPtr const& spriteTexture = m_texture ? m_texture : GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getTextureManager()->getErrorTexture();

        if (m_sprite->getTexture() != spriteTexture)
            m_sprite->set(spriteTexture);
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerTexture2D::processDataFromUI()
    {
        Texture2DPtr const& value = m_texture;

        for (MaterialPtr const& material : m_materials)
            material->getUniform(m_shaderUniformName)->set(value);
    }

    //////////////////////////////////////////
    void ShaderUniformVariantDrawerTexture2D::notifySelectAssetClick(Button2D* _button, CursorInputEvent const& _event)
    {
        TexturePickerManager::GetInstancePtr()->openTexturePicker(
            [this](Texture2DPtr const& _texture)
            {
                m_texture = _texture;

                if (!m_texture)
                    m_texture = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem()->getTextureManager()->getWhiteTexture();

                processDataFromUI();
            },
            m_texture);
    }

} // namespace Maze
//////////////////////////////////////////
