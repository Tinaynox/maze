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
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinMaterialType);


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
            if (m_renderSystemRaw->getShaderSystem())
                m_renderSystemRaw->getShaderSystem()->eventSystemInited.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    void MaterialManager::Initialize(MaterialManagerPtr& _manager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(MaterialManager, _manager, init(_renderSystem));
    }

    //////////////////////////////////////////
    MaterialManagerPtr const& MaterialManager::GetCurrentInstance()
    {
        return RenderSystem::GetCurrentInstancePtr()->getMaterialManager();
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
        // createBuiltinMaterials();
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::getMaterial(HashedCString _materialName)
    {
        static MaterialPtr nullPointer;

        StringKeyMap<MaterialPtr>::const_iterator it = m_materialsByName.find(_materialName);
        if (it != m_materialsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_materialName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined material: %s!", _materialName.str);
            return getErrorMaterial();
        }

        MaterialPtr material = Material::Create(assetFile);
        material->setName(_materialName.str);
        return addMaterial(material);
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::getMaterial(AssetFilePtr const& _assetFile)
    {
        StringKeyMap<MaterialPtr>::const_iterator it = m_materialsByName.find(_assetFile->getFileName());
        if (it != m_materialsByName.end())
            return it->second;

        MaterialPtr material = Material::Create(_assetFile);
        material->setName(_assetFile->getFileName());
        return addMaterial(material);
    }

    //////////////////////////////////////////
    String const& MaterialManager::getMaterialName(Material const* _material)
    {
        static String nullPointer;

        for (auto const& materialData : m_materialsByName)
        {
            if (materialData.second.get() == _material)
                return materialData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::createBuiltinMaterial(BuiltinMaterialType _materialType)
    {
        MaterialPtr& material = m_builtinMaterials[_materialType];

        switch (_materialType)
        {
            case BuiltinMaterialType::Error:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::Error));
                renderPass->setDepthWriteEnabled(true);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setRenderQueueIndex(2000);
                break;
            }
            case BuiltinMaterialType::UV:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::UV));
                renderPass->setDepthWriteEnabled(true);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(2000);
                break;
            }
            case BuiltinMaterialType::Normal:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::Normal));
                renderPass->setDepthWriteEnabled(true);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(2000);
                break;
            }
            case BuiltinMaterialType::Color:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::Color));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                break;
            }
            case BuiltinMaterialType::ColorTexture:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::ColorTexture));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform("u_baseMapST", Vec4DF(1.0f, 1.0f, 0.0f, 0.0f));
                break;
            }
            case BuiltinMaterialType::ColorTextureCustomUV:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::ColorTextureCustomUV));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                material->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                break;
            }
            case BuiltinMaterialType::ColorHDR:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::ColorHDR));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform("u_intensity", 0.0f);
                break;
            }
            case BuiltinMaterialType::HSVRect:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::HSVRect));
                break;
            }
            case BuiltinMaterialType::HSVBand:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::HSVBand));
                break;
            }
            case BuiltinMaterialType::ColorPickerChannel:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::ColorPickerChannel));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                break;
            }
            case BuiltinMaterialType::DebugGrid:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::DebugGrid));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(2998);
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                break;
            }
            case BuiltinMaterialType::DebugAxis:
            {
                material = Material::Create(m_renderSystemRaw);
                
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::DebugAxis));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(2999);
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                break;
            }
            case BuiltinMaterialType::Skybox:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::Skybox));
                renderPass->setBlendSrcFactor(BlendFactor::One);
                renderPass->setBlendDestFactor(BlendFactor::Zero);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Back);
                renderPass->setRenderQueueIndex(1000);
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteCubeTexture());
                break;
            }
            case BuiltinMaterialType::Specular:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::Specular));
                renderPass->setBlendSrcFactor(BlendFactor::One);
                renderPass->setBlendDestFactor(BlendFactor::Zero);
                renderPass->setDepthWriteEnabled(true);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setCullMode(CullMode::Back);
                renderPass->setRenderQueueIndex(2000);
                material->setUniform("u_baseMapST", Vec4DF(1.0f, 1.0f, 0.0f, 0.0f));
                material->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform("u_color", ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform("u_ambientLightColor", ColorF128(0.1f, 0.1f, 0.1f, 1.0f));
                material->setUniform("u_shininess", 0.65f);
                material->setUniform("u_specularColor", ColorF128(0.3f, 0.3f, 0.3f, 1.0f));
                break;
            }
            case BuiltinMaterialType::SpecularDS:
            {
                MaterialPtr const& specularMaterial = ensureBuiltinMaterial(BuiltinMaterialType::Specular);
                if (specularMaterial)
                {
                    material = specularMaterial->createCopy();
                    material->getFirstRenderPass()->setCullMode(CullMode::Off);
                }

                break;
            }
            case BuiltinMaterialType::MeshPreview:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::MeshPreview));
                renderPass->setBlendSrcFactor(BlendFactor::One);
                renderPass->setBlendDestFactor(BlendFactor::Zero);
                renderPass->setDepthWriteEnabled(true);
                renderPass->setDepthTestCompareFunction(CompareFunction::LessEqual);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(2000);
                material->setUniform("u_baseMapST", Vec4DF(1.0f, 1.0f, 0.0f, 0.0f));
                material->setUniform("u_baseMap", m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform("u_color", ColorF128(0.77f, 0.77f, 0.77f, 1.0f));
                material->setUniform("u_ambientLightColor", ColorF128(0.1f, 0.1f, 0.1f, 1.0f));
                material->setUniform("u_shininess", 0.65f);
                material->setUniform("u_specularColor", ColorF128(0.3f, 0.3f, 0.3f, 1.0f));

                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        if (material)
        {
            material->setName(_materialType.toCString());
            addMaterial(material);
        }

        return material;
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::ensureBuiltinMaterial(BuiltinMaterialType _materialType)
    {
        MaterialPtr const& material = getBuiltinMaterial(_materialType);
        if (material)
            return material;

        return createBuiltinMaterial(_materialType);
    }

    //////////////////////////////////////////
    void MaterialManager::createBuiltinMaterials()
    {
        for (BuiltinMaterialType t = BuiltinMaterialType(1); t < BuiltinMaterialType::MAX; ++t)
            ensureBuiltinMaterial(t);

        eventBuiltinMaterialsCreated();
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::addMaterial(MaterialPtr const& _material)
    {
        auto it2 = m_materialsByName.insert(_material->getName(), _material);
        return *it2;
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
