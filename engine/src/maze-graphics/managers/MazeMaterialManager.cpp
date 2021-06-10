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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class MaterialManager
    //
    //////////////////////////////////////////
    MaterialManager::MaterialManager()
        : m_renderSystemRaw(nullptr)
    {
        
    }

    //////////////////////////////////////////
    MaterialManager::~MaterialManager()
    {
        if (m_renderSystemRaw)
        {
            m_renderSystemRaw->eventSystemInited.unsubscribe(this);
            m_renderSystemRaw->getShaderSystem()->eventSystemInited.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    void MaterialManager::Initialize(MaterialManagerPtr& _manager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(MaterialManager, _manager, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool MaterialManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        m_renderSystemRaw->eventSystemInited.subscribe(this, &MaterialManager::notifyRenderSystemInited);
        if (m_renderSystemRaw->getSystemInited())
            notifyRenderSystemInited();

        return true;
    }

    //////////////////////////////////////////
    void MaterialManager::notifyRenderSystemInited()
    {
        m_renderSystemRaw->getShaderSystem()->eventSystemInited.subscribe(this, &MaterialManager::notifyShaderSystemInited);
        if (m_renderSystemRaw->getShaderSystem()->getSystemInited())
            notifyShaderSystemInited();
    }

    //////////////////////////////////////////
    void MaterialManager::notifyShaderSystemInited()
    {
        createSpecialMaterials();
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::getMaterial(String const& _materialName)
    {
        static MaterialPtr nullPointer;

        UnorderedMap<String, MaterialPtr>::const_iterator it = m_materialsByName.find(_materialName);
        if (it != m_materialsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_materialName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined material: %s!", _materialName.c_str());
            return m_errorMaterial;
        }

        MaterialPtr material = Material::Create(assetFile);
        material->setName(_materialName);
        return addMaterial(material);
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::getMaterial(AssetFilePtr const& _assetFile)
    {
        UnorderedMap<String, MaterialPtr>::const_iterator it = m_materialsByName.find(_assetFile->getFileName());
        if (it != m_materialsByName.end())
            return it->second;

        MaterialPtr material = Material::Create(_assetFile);
        material->setName(_assetFile->getFileName());
        return addMaterial(material);
    }

    //////////////////////////////////////////
    String const& MaterialManager::getMaterialName(MaterialPtr const& _material)
    {
        static String nullPointer;

        for (auto const& materialData : m_materialsByName)
        {
            if (materialData.second == _material)
                return materialData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    void MaterialManager::createSpecialMaterials()
    {
        m_errorMaterial = Material::Create(m_renderSystemRaw);
        m_errorMaterial->setName("Error");
        RenderPassPtr renderPass = m_errorMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getErrorShader());

        m_uvMaterial = Material::Create(m_renderSystemRaw);
        m_uvMaterial->setName("UV");
        renderPass = m_uvMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getUVShader());
        addMaterial(m_uvMaterial);
                
        m_colorMaterial = Material::Create(m_renderSystemRaw);
        m_colorMaterial->setName("Color");
        renderPass = m_colorMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getColorShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
        m_colorMaterial->setUniform("u_color", Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));
        addMaterial(m_colorMaterial);

        m_colorTextureMaterial = Material::Create(m_renderSystemRaw);
        m_colorTextureMaterial->setName("ColorTexture");
        renderPass = m_colorTextureMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getColorTextureShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
        m_colorTextureMaterial->setUniform("u_color", Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));
        m_colorTextureMaterial->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteTexture());
        m_colorTextureMaterial->setUniform("u_baseMapST", Vec4DF(1.0f, 1.0f, 0.0f, 0.0f));
        addMaterial(m_colorTextureMaterial);

        m_colorTextureCustomUVMaterial = Material::Create(m_renderSystemRaw);
        m_colorTextureCustomUVMaterial->setName("ColorTextureCustomUV");
        renderPass = m_colorTextureCustomUVMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getColorTextureCustomUVShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
        m_colorTextureCustomUVMaterial->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteTexture());
        m_colorTextureCustomUVMaterial->setUniform("u_color", Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));

        m_colorHDRMaterial = Material::Create(m_renderSystemRaw);
        m_colorHDRMaterial->setName("ColorHDR");
        renderPass = m_colorHDRMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getColorHDRShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
        m_colorHDRMaterial->setUniform("u_color", Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));
        m_colorHDRMaterial->setUniform("u_intensity", 0.0f);

        m_hsvRectMaterial = Material::Create(m_renderSystemRaw);
        m_hsvRectMaterial->setName("HSVRect");
        renderPass = m_hsvRectMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getHSVRectShader());

        m_hsvBandMaterial = Material::Create(m_renderSystemRaw);
        m_hsvBandMaterial->setName("HSVBand");
        renderPass = m_hsvBandMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getHSVBandShader());

        m_colorPickerChannelMaterial = Material::Create(m_renderSystemRaw);
        m_colorPickerChannelMaterial->setName("ColorPickerChannelMaterial");
        renderPass = m_colorPickerChannelMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getColorPickerChannelShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);

        m_debugGridMaterial = Material::Create(m_renderSystemRaw);
        m_debugGridMaterial->setName("DebugGrid");
        renderPass = m_debugGridMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getDebugGridShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
        m_debugGridMaterial->setUniform("u_color", Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));

        m_debugAxisMaterial = Material::Create(m_renderSystemRaw);
        m_debugAxisMaterial->setName("DebugAxis");
        renderPass = m_debugAxisMaterial->createRenderPass();
        renderPass->setShader(m_renderSystemRaw->getShaderSystem()->getDebugAxisShader());
        renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
        renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
        m_debugAxisMaterial->setUniform("u_color", Vec4DF(1.0f, 1.0f, 1.0f, 1.0f));

    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::addMaterial(MaterialPtr const& _material)
    {
        auto it2 = m_materialsByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_material->getName()),
            std::forward_as_tuple(_material));
        return it2.first->second;
    }

    //////////////////////////////////////////
    Vector<MaterialPtr> MaterialManager::getMaterialsSorted()
    {
        Vector<MaterialPtr> result;

        for (auto const& value : m_materialsByName)
            result.emplace_back(value.second);

        std::sort(
            result.begin(),
            result.end(),
            [](MaterialPtr const& _a, MaterialPtr const& _b)
            {
                return _a->getName() < _b->getName();
            });

        return result;
    }

    //////////////////////////////////////////
    void MaterialManager::loadAllAssetMaterials()
    {
        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtension("mzmaterial");
        for (AssetFilePtr const& assetFile : assetFiles)
        {
            if (m_materialsByName.find(assetFile->getFileName()) != m_materialsByName.end())
                continue;

            MaterialPtr material = Material::Create(assetFile);
            material->setName(assetFile->getFileName());
            addMaterial(material);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
