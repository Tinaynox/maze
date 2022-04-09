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
#include "maze-debugger/preview-inspectors/MazeTexture2DPreviewInspector.hpp"
#include "maze-debugger/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerS32.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF32.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerF64.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DF.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DS.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec2DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec3DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerVec4DU.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerColorF128.hpp"
#include "maze-debugger/shader-uniform-variant-drawers/MazeShaderUniformVariantDrawerTexture2D.hpp"


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
        m_scene->clear();

        if (!m_textures.empty())
        {
            SpriteRenderer2DPtr spriteHolder = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TransparentChess),
                Vec2DF(m_scene->getCanvas()->getTransform()->getHeight()),
                Vec2DF::c_zero,
                MaterialManager::GetCurrentInstance()->getColorTextureMaterial(),
                m_scene->getCanvas()->getTransform(),
                m_scene.get(),
                Vec2DF(0.5f, 0.5f),
                Vec2DF(1.0f, 0.5f));
            spriteHolder->getMaterial()->setUniform(
                "u_baseMapST",
                Vec4DF(
                    spriteHolder->getTransform()->getWidth() / 16.0f,
                    spriteHolder->getTransform()->getHeight() / 16.0f,
                    0.0f,
                    0.0f));

            Texture2DPtr texture = *m_textures.begin();
            F32 ratio = (F32)texture->getSize().x / (F32)texture->getSize().y;

            Vec2DF spriteSize;
            if (ratio > 1.0f)
            {
                spriteSize = Vec2DF(
                    spriteHolder->getTransform()->getSize().x,
                    spriteHolder->getTransform()->getSize().y / ratio);
            }
            else
            {
                spriteSize = Vec2DF(
                    spriteHolder->getTransform()->getSize().x / ratio,
                    spriteHolder->getTransform()->getSize().y);
            }

            SpritePtr sprite = Sprite::Create(texture);
            SpriteHelper::CreateSprite(
                sprite,
                spriteSize,
                Vec2DF::c_zero,
                MaterialManager::GetCurrentInstance()->getColorTextureMaterial(),
                spriteHolder->getTransform(),
                m_scene.get());

            String pixelFormat = PixelFormat::ToString(texture->getInternalPixelFormat());
            U32 bytes = (U32)PixelFormat::CalculateRequiredBytes(texture->getSize().x, texture->getSize().y, 1u, texture->getInternalPixelFormat());

            String text;
            StringHelper::FormatString(
                text,
                "Type: %s\n"
                "Size: %dx%d (%s KB)\n"
                "Format: %s\n"
                "Min: %s\n"
                "Mag: %s\n"
                "WS: %s\n"
                "WT: %s",
                texture->getType().toCString(),
                texture->getSize().x, texture->getSize().y,
                StringHelper::F32ToStdStringFormatted((F32)bytes / 1024.0f, 1).c_str(),
                pixelFormat.c_str(),
                texture->getMinFilter().toCString(),
                texture->getMagFilter().toCString(),
                texture->getWrapS().toCString(),
                texture->getWrapT().toCString());

            SystemTextRenderer2DPtr info = SpriteHelper::CreateSystemText(
                text.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                Vec2DF(m_scene->getCanvas()->getTransform()->getHeight(), m_scene->getCanvas()->getTransform()->getHeight() - 4.0f),
                Vec2DF(m_scene->getCanvas()->getTransform()->getHeight() * 0.5f + 2.0f, 0.0f),
                m_scene->getCanvas()->getTransform(),
                m_scene.get(),
                Vec2DF(0.5f, 0.5f),
                Vec2DF(0.5f, 0.5f));
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
            Texture2DPtr const& texture = textureManager->getTexture2D(assetFile);
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
            textures.insert(std::static_pointer_cast<Texture2D>(object));

        setTextures(textures);

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
