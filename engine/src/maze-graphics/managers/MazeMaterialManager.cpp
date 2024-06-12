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
    MaterialLibraryData const* MaterialManager::getMaterialLibraryData(HashedCString _materialName)
    {
        StringKeyMap<MaterialLibraryData>::const_iterator it = m_materialsLibrary.find(_materialName);
        if (it != m_materialsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::getMaterial(HashedCString _materialName)
    {
        static MaterialPtr const nullPointer;

        if (_materialName.empty())
            return nullPointer;

        MaterialLibraryData const* libraryData = getMaterialLibraryData(_materialName);
        if (libraryData != nullptr)
            return libraryData->material;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_materialName);
        if (!assetFile)
        {
            MAZE_ERROR("Undefined material: %s!", _materialName.str);
            return getErrorMaterial();
        }

        MaterialPtr material = Material::Create(assetFile);
        material->setName(_materialName.str);
        MaterialLibraryData* data = addMaterialToLibrary(material);
        if (data)
        {
            data->assetFile = assetFile;
            return data->material;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    MaterialPtr const& MaterialManager::getMaterial(AssetFilePtr const& _assetFile)
    {
        static MaterialPtr const nullPointer;

        MaterialLibraryData const* libraryData = getMaterialLibraryData(_assetFile->getFileName());
        if (libraryData != nullptr)
            return libraryData->material;

        MaterialPtr material = Material::Create(_assetFile);
        material->setName(_assetFile->getFileName());

        MaterialLibraryData* data = addMaterialToLibrary(material);
        if (data)
        {
            data->assetFile = _assetFile;
            return data->material;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    HashedCString MaterialManager::getMaterialName(Material const* _material)
    {
        for (StringKeyMap<MaterialLibraryData>::iterator it = m_materialsLibrary.begin(),
                                                         end = m_materialsLibrary.end();
                                                         it != end;
                                                         ++it)
        {
            if (it->second.material.get() == _material)
                return it.key();
        }

        return HashedCString();
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
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
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
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform(MAZE_HCS("u_baseMap"), m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform(MAZE_HCS("u_baseMapST"), Vec4F(1.0f, 1.0f, 0.0f, 0.0f));
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
                material->setUniform(MAZE_HCS("u_baseMap"), m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                break;
            }
            case BuiltinMaterialType::ColorTextureAdditive:
            {
                MaterialPtr const& originMaterial = ensureBuiltinMaterial(BuiltinMaterialType::ColorTexture);
                if (originMaterial)
                {
                    material = originMaterial->createCopy();
                    material->getFirstRenderPass()->setBlendSrcFactor(BlendFactor::One);
                    material->getFirstRenderPass()->setBlendDestFactor(BlendFactor::One);
                }

                break;
            }
            case BuiltinMaterialType::ColorTextureAdditivePA:
            {
                MaterialPtr const& originMaterial = ensureBuiltinMaterial(BuiltinMaterialType::ColorTexture);
                if (originMaterial)
                {
                    material = originMaterial->createCopy();
                    material->getFirstRenderPass()->setBlendSrcFactor(BlendFactor::SrcAlpha);
                    material->getFirstRenderPass()->setBlendDestFactor(BlendFactor::One);
                }

                break;
            }
            case BuiltinMaterialType::ColorStreamHDR:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();

                ShaderPtr shader = m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::ColorHDR)->createCopy();
                shader->addLocalFeature("MAZE_COLOR_STREAM", "(1)");
                shader->recompile();

                renderPass->setShader(shader);
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform(MAZE_HCS("u_intensity"), 0.0f);
                break;
            }
            case BuiltinMaterialType::Sprite:
            {
                MaterialPtr const& originMaterial = ensureBuiltinMaterial(BuiltinMaterialType::ColorTexture);
                if (originMaterial)
                {
                    material = originMaterial->createCopy();

                    RenderPassPtr const& renderPass = material->getFirstRenderPass();
                    ShaderPtr shader = renderPass->getShader()->createCopy();
                    shader->addLocalFeature("MAZE_COLOR_STREAM", "(1)");
                    shader->addLocalFeature("MAZE_UV0_STREAM", "(1)");
                    shader->recompile();
                    renderPass->setShader(shader);
                }
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
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
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
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
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
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform(MAZE_HCS("u_baseMap"), m_renderSystemRaw->getTextureManager()->getWhiteCubeTexture());
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
                material->setUniform(MAZE_HCS("u_baseMapST"), Vec4F(1.0f, 1.0f, 0.0f, 0.0f));
                material->setUniform(MAZE_HCS("u_baseMap"), m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform(MAZE_HCS("u_ambientLightColor"), ColorF128(0.1f, 0.1f, 0.1f, 1.0f));
                material->setUniform(MAZE_HCS("u_shininess"), 0.65f);
                material->setUniform(MAZE_HCS("u_specularColor"), ColorF128(0.3f, 0.3f, 0.3f, 1.0f));
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
                material->setUniform(MAZE_HCS("u_baseMapST"), Vec4F(1.0f, 1.0f, 0.0f, 0.0f));
                material->setUniform(MAZE_HCS("u_baseMap"), m_renderSystemRaw->getTextureManager()->getWhiteTexture());
                material->setUniform(MAZE_HCS("u_color"), ColorF128(0.77f, 0.77f, 0.77f, 1.0f));
                material->setUniform(MAZE_HCS("u_ambientLightColor"), ColorF128(0.1f, 0.1f, 0.1f, 1.0f));
                material->setUniform(MAZE_HCS("u_shininess"), 0.65f);
                material->setUniform(MAZE_HCS("u_specularColor"), ColorF128(0.3f, 0.3f, 0.3f, 1.0f));

                break;
            }
            case BuiltinMaterialType::Font:
            {
                material = Material::Create(m_renderSystemRaw);
                RenderPassPtr renderPass = material->createRenderPass();
                renderPass->setShader(m_renderSystemRaw->getShaderSystem()->ensureBuiltinShader(BuiltinShaderType::Font));
                renderPass->setBlendSrcFactor(BlendFactor::SrcAlpha);
                renderPass->setBlendDestFactor(BlendFactor::OneMinusSrcAlpha);
                renderPass->setDepthWriteEnabled(false);
                renderPass->setDepthTestCompareFunction(CompareFunction::Disabled);
                renderPass->setCullMode(CullMode::Off);
                renderPass->setRenderQueueIndex(3000);
                material->setUniform(MAZE_HCS("u_color"), ColorF128(1.0f, 1.0f, 1.0f, 1.0f));
                material->setUniform(MAZE_HCS("u_baseMap"), m_renderSystemRaw->getTextureManager()->getWhiteTexture());
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
            addMaterialToLibrary(material);
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
        MAZE_PROFILE_EVENT("MaterialManager::createBuiltinMaterials");

        for (BuiltinMaterialType t = BuiltinMaterialType(1); t < BuiltinMaterialType::MAX; ++t)
            ensureBuiltinMaterial(t);

        eventBuiltinMaterialsCreated();
    }

    //////////////////////////////////////////
    MaterialLibraryData* MaterialManager::addMaterialToLibrary(MaterialPtr const& _material)
    {
        auto it2 = m_materialsLibrary.insert(
            _material->getName(),
            _material);
        return it2;
    }

    //////////////////////////////////////////
    void MaterialManager::removeMaterialFromLibrary(HashedCString _materialName)
    {
        m_materialsLibrary.erase(_materialName);
    }

    //////////////////////////////////////////
    Vector<MaterialPtr> MaterialManager::getMaterialsSorted()
    {
        Vector<MaterialPtr> result;

        for (auto const& value : m_materialsLibrary)
            result.emplace_back(value.second.material);

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
        MAZE_PROFILE_EVENT("MaterialManager::loadAllAssetMaterials");

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtension("mzmaterial");
        for (AssetFilePtr const& assetFile : assetFiles)
        {
            if (m_materialsLibrary.find(assetFile->getFileName()) != m_materialsLibrary.end())
                continue;

            MaterialPtr material = Material::Create(assetFile);
            material->setName(assetFile->getFileName());
            MaterialLibraryData* libraryData = addMaterialToLibrary(material);
            if (libraryData)
            {
                libraryData->assetFile = assetFile;
            }
        }
    }

    //////////////////////////////////////////
    void MaterialManager::unloadAssetMaterials(Set<String> const& _tags)
    {
        MAZE_PROFILE_EVENT("MaterialManager::unloadAssetMaterials");

        StringKeyMap<MaterialLibraryData>::iterator it = m_materialsLibrary.begin();
        StringKeyMap<MaterialLibraryData>::iterator end = m_materialsLibrary.end();
        for (; it != end; )
        {
            if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
            {
                it = m_materialsLibrary.erase(it);
                end = m_materialsLibrary.end();
            }
            else
            {
                ++it;
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
