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
#include "maze-editor-tools/preview-inspectors/MazeTexture2DPreviewInspector.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4F32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4S32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4U32.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-editor-tools/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Texture2DPreviewInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DPreviewInspector, PreviewInspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(Texture2DPreviewInspector);

    //////////////////////////////////////////
    Texture2DPreviewInspector::Texture2DPreviewInspector()
    {
        
    }

    //////////////////////////////////////////
    Texture2DPreviewInspector::~Texture2DPreviewInspector()
    {

    }

    //////////////////////////////////////////
    Texture2DPreviewInspectorPtr Texture2DPreviewInspector::Create(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        Texture2DPreviewInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Texture2DPreviewInspector, object, init(
            _parent2D,
            _scene));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DPreviewInspector::init(
        Transform2DPtr const& _parent2D,
        SceneDebugPreviewPtr const& _scene)
    {
        if (!PreviewInspector::init(
            _parent2D,
            _scene))
            return false;
        

        return true;
    }

    //////////////////////////////////////////
    void Texture2DPreviewInspector::update(F32 _dt)
    {
        if (m_texturesDirty)
        {
            buildTextures();
            m_texturesDirty = false;
        }

        
    }

    //////////////////////////////////////////
    void Texture2DPreviewInspector::setTextures(Set<Texture2DPtr> const& _textures)
    {
        m_textures = _textures;

        m_texturesDirty = true;
    }

    //////////////////////////////////////////
    void Texture2DPreviewInspector::buildTextures()
    {
        SceneDebugPreviewPtr scene = m_scene.lock();

        scene->clear();

        if (!m_textures.empty())
        {
            SpriteRenderer2DPtr spriteHolder = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                Vec2F(scene->getCanvas()->getTransform()->getHeight()),
                Vec2F::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                scene->getCanvas()->getTransform(),
                scene.get(),
                Vec2F(0.5f, 0.5f),
                Vec2F(1.0f, 0.5f));
            spriteHolder->getMaterial()->setUniform(
                MAZE_HCS("u_baseMapST"),
                Vec4F(
                    spriteHolder->getTransform()->getWidth() / 16.0f,
                    spriteHolder->getTransform()->getHeight() / 16.0f,
                    0.0f,
                    0.0f));

            Texture2DPtr texture = *m_textures.begin();
            F32 ratio = (F32)texture->getSize().x / (F32)texture->getSize().y;

            Vec2F spriteSize;
            if (ratio > 1.0f)
            {
                spriteSize = Vec2F(
                    spriteHolder->getTransform()->getSize().x,
                    spriteHolder->getTransform()->getSize().y / ratio);
            }
            else
            {
                spriteSize = Vec2F(
                    spriteHolder->getTransform()->getSize().x / ratio,
                    spriteHolder->getTransform()->getSize().y);
            }

            SpritePtr sprite = Sprite::Create(texture);
            sprite->setName(MAZE_HS("Texture2DPreview"));
            SpriteHelper::CreateSprite(
                sprite,
                spriteSize,
                Vec2F::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                spriteHolder->getTransform(),
                scene.get());

            String pixelFormat = PixelFormat::ToString(texture->getInternalPixelFormat());
            U32 bytes = (U32)PixelFormat::CalculateRequiredBytes(texture->getSize().x, texture->getSize().y, 1u, texture->getInternalPixelFormat());

            String text;
            StringHelper::FormatString(
                text,
                "Type: %s\n"
                "Size: %dx%d (%s KB)\n"
                "Format: %s\n",
                texture->getType().toCString(),
                texture->getSize().x, texture->getSize().y,
                StringHelper::F32ToStdStringFormatted((F32)bytes / 1024.0f, 1).c_str(),
                pixelFormat.c_str());

            AbstractTextRenderer2DPtr info = EditorToolsUIHelper::CreateText(
                text.c_str(),
                EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
                12,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2F(scene->getCanvas()->getTransform()->getHeight(), scene->getCanvas()->getTransform()->getHeight() - 4.0f),
                Vec2F(scene->getCanvas()->getTransform()->getHeight() * 0.5f + 2.0f, 0.0f),
                scene->getCanvas()->getTransform(),
                scene.get(),
                Vec2F(0.5f, 0.5f),
                Vec2F(0.5f, 0.5f));
            info->setColor(ColorU32::c_black);
        }
        
        m_texturesDirty = false;
    }

    //////////////////////////////////////////
    bool Texture2DPreviewInspector::setAssetFiles(Set<AssetFilePtr> const& _assetFiles)
    {
        Set<Texture2DPtr> textures;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        TextureManagerPtr const& textureManager = renderSystem->getTextureManager();

        for (AssetFilePtr const& assetFile : _assetFiles)
        {
            Texture2DPtr const& texture = textureManager->getOrLoadTexture2D(assetFile);
            if (texture)
                textures.insert(texture);
        }

        setTextures(textures);

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DPreviewInspector::setObjects(Set<ObjectPtr> const& _objects)
    {
        Set<Texture2DPtr> textures;
        for (ObjectPtr const& object : _objects)
            textures.insert(Maze::static_pointer_cast<Texture2D>(object));

        setTextures(textures);

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
