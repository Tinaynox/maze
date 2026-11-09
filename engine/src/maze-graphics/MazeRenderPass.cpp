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
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeShaderManager.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderPass
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(RenderPass,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(RenderPassType, passType, RenderPassType::None, getPassType, setPassType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, renderQueueIndex, (U8)RenderQueueIndex::Opaque, getRenderQueueIndex, setRenderQueueIndex),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ShaderAssetRef, shader, ShaderAssetRef(), getShaderRef, setShaderRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(BlendOperation, blendOperation, BlendOperation::Add, getBlendOperation, setBlendOperation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(BlendFactor, blendSrcFactor, BlendFactor::One, getBlendSrcFactor, setBlendSrcFactor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(BlendFactor, blendDestFactor, BlendFactor::Zero, getBlendDestFactor, setBlendDestFactor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CompareFunction, depthTestCompareFunction, CompareFunction::Disabled, getDepthTestCompareFunction, setDepthTestCompareFunction),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, depthWriteEnabled, true, getDepthWriteEnabled, setDepthWriteEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CullMode, cullMode, CullMode::Off, getCullMode, setCullMode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, alphaToCoverageEnabled, false, getAlphaToCoverageEnabled, setAlphaToCoverageEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CompareFunction, stencilTestCompareFunction, CompareFunction::Disabled, getStencilTestCompareFunction, setStencilTestCompareFunction),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, stencilReferenceValue, 0, getStencilReferenceValue, setStencilReferenceValue),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, stencilReadMask, 0xFF, getStencilReadMask, setStencilReadMask),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, stencilWriteMask, 0xFF, getStencilWriteMask, setStencilWriteMask),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(StencilOperation, stencilFailOperation, StencilOperation::Keep, getStencilFailOperation, setStencilFailOperation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(StencilOperation, stencilDepthFailOperation, StencilOperation::Keep, getStencilDepthFailOperation, setStencilDepthFailOperation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(StencilOperation, stencilPassOperation, StencilOperation::Keep, getStencilPassOperation, setStencilPassOperation)
    )

    //////////////////////////////////////////
    RenderPass::RenderPass()
        : m_renderQueueIndex((U8)RenderQueueIndex::Opaque)
    {
    }

    //////////////////////////////////////////
    RenderPass::~RenderPass()
    {
    }

    //////////////////////////////////////////
    bool RenderPass::init(
        RenderSystem* _renderSystem,
        RenderPassType _passType)
    {
        m_renderSystem = _renderSystem;
        m_passType = _passType;

        return true;
    }

    //////////////////////////////////////////
    bool RenderPass::init(
        RenderPassPtr const& _renderPass)
    {
        m_renderSystem = _renderPass->m_renderSystem;

        if (!MetaClassHelper::CopyProperties(
            getMetaInstance(),
            _renderPass->getMetaInstance()))
            return false;

        m_shaderFeatures = _renderPass->m_shaderFeatures;
        m_shader = _renderPass->m_shader;
        dirtyShaderUniforms();

        return true;
    }

    //////////////////////////////////////////
    bool RenderPass::set(RenderPass const& _renderPass)
    {
        m_renderSystem = _renderPass.m_renderSystem;
        m_passType = _renderPass.m_passType;
        m_renderQueueIndex = _renderPass.m_renderQueueIndex;
        m_shaderRef = _renderPass.m_shaderRef;
        m_shaderFeatures = _renderPass.m_shaderFeatures;
        m_shader = _renderPass.m_shader;
        dirtyShaderUniforms();
        m_blendOperation = _renderPass.m_blendOperation;
        m_blendSrcFactor = _renderPass.m_blendSrcFactor;
        m_blendDestFactor = _renderPass.m_blendDestFactor;
        m_depthTestCompareFunction = _renderPass.m_depthTestCompareFunction;
        m_depthWriteEnabled = _renderPass.m_depthWriteEnabled;
        m_cullMode = _renderPass.m_cullMode;
        m_alphaToCoverageEnabled = _renderPass.m_alphaToCoverageEnabled;
        m_stencilTestCompareFunction = _renderPass.m_stencilTestCompareFunction;
        m_stencilReferenceValue = _renderPass.m_stencilReferenceValue;
        m_stencilReadMask = _renderPass.m_stencilReadMask;
        m_stencilWriteMask = _renderPass.m_stencilWriteMask;
        m_stencilFailOperation = _renderPass.m_stencilFailOperation;
        m_stencilDepthFailOperation = _renderPass.m_stencilDepthFailOperation;
        m_stencilPassOperation = _renderPass.m_stencilPassOperation;

        return true;
    }

    //////////////////////////////////////////
    bool RenderPass::isEqual(RenderPass const& _renderPass) const
    {
        if (m_shaderFeatures != _renderPass.m_shaderFeatures)
            return false;

        return MetaClassHelper::IsEqual(getMetaInstance(), _renderPass.getMetaInstance());
    }

    //////////////////////////////////////////
    void RenderPass::setShader(ShaderPtr const& _shader)
    {
        if (m_shaderRef.getShader() == _shader)
            return;

        m_shaderRef.setShader(_shader);
        updateShader();
    }

    //////////////////////////////////////////
    void RenderPass::setShaderRef(ShaderAssetRef const& _value)
    {
        setShader(_value.getShader());
    }

    //////////////////////////////////////////
    void RenderPass::setShaderFeatures(ShaderFeatures const& _features)
    {
        if (m_shaderFeatures == _features)
            return;

        m_shaderFeatures = _features;
        updateShader();
    }

    //////////////////////////////////////////
    void RenderPass::setShaderFeature(ShaderFeature const& _feature)
    {
        MAZE_ERROR_RETURN_IF(_feature.name.empty(), "Shader feature name is empty!");

        ShaderFeatures features = m_shaderFeatures;
        auto it = eastl::find_if(
            features.begin(),
            features.end(),
            [&_feature](ShaderFeature const& _f) { return _f.name == _feature.name; });
        if (it != features.end())
            *it = _feature;
        else
            features.push_back(_feature);

        setShaderFeatures(features);
    }

    //////////////////////////////////////////
    void RenderPass::setShaderFeatureEnabled(String const& _name, bool _enabled)
    {
        ShaderFeature const* feature = getShaderFeature(_name);
        MAZE_ERROR_RETURN_IF(!feature, "Undefined shader feature: %s", _name.c_str());

        ShaderFeature newFeature = *feature;
        newFeature.enabled = _enabled;
        setShaderFeature(newFeature);
    }

    //////////////////////////////////////////
    void RenderPass::removeShaderFeature(String const& _name)
    {
        ShaderFeatures features = m_shaderFeatures;
        auto it = eastl::find_if(
            features.begin(),
            features.end(),
            [&_name](ShaderFeature const& _f) { return _f.name == _name; });
        if (it == features.end())
            return;

        features.erase(it);
        setShaderFeatures(features);
    }

    //////////////////////////////////////////
    ShaderFeature const* RenderPass::getShaderFeature(String const& _name) const
    {
        for (ShaderFeature const& feature : m_shaderFeatures)
            if (feature.name == _name)
                return &feature;

        return nullptr;
    }

    //////////////////////////////////////////
    void RenderPass::updateShader()
    {
        ShaderPtr shader = m_shaderRef.getShader();
        if (shader && !m_shaderFeatures.empty() && m_renderSystem)
            shader = m_renderSystem->getShaderManager()->ensureShaderVariant(shader, m_shaderFeatures);

        if (m_shader == shader)
            return;

        m_shader = shader;
        dirtyShaderUniforms();
    }

    //////////////////////////////////////////
    void RenderPass::setShader(Path const& _shaderResourceName)
    {
        ShaderPtr const& shader = m_renderSystem->getShaderManager()->getOrLoadShader(_shaderResourceName);

        MAZE_ERROR_IF(!shader && !_shaderResourceName.empty(), "Failed to load RenderPass shader - %s", _shaderResourceName.toUTF8().c_str());

        setShader(shader);
    }

    //////////////////////////////////////////
    void RenderPass::applyRenderPassUniforms()
    {
        if (!m_material)
            return;

        if (m_shaderUniformsDirty)
            updateShaderUniforms();

        FastVector<ShaderUniformVariantPtr> const& uniforms = m_material->getUniforms();
        MAZE_DEBUG_ERROR_RETURN_IF(uniforms.size() != m_shaderUniforms.size(), "Uniforms mismatch!");

        for (Size i = 0, in = uniforms.size(); i != in; ++i)
        {
            ShaderUniformVariantPtr const& uniformVariant = uniforms[i];
            ShaderUniformPtr const& shaderUniform = m_shaderUniforms[i];
            
            if (uniformVariant && shaderUniform)
                shaderUniform->set(*uniformVariant.get());
        }

    }

    //////////////////////////////////////////
    void RenderPass::updateShaderUniforms()
    {
        m_shaderUniforms.clear();

        if (!m_material)
            return;

        // Keep the sizes in sync even without a shader - applyRenderPassUniforms indexes by material uniform
        FastVector<ShaderUniformVariantPtr> const& uniforms = m_material->getUniforms();
        m_shaderUniforms.resize(uniforms.size());
        m_shaderUniformsDirty = false;

        if (!m_shader)
        {
            MAZE_ERROR("RenderPass has no shader! Material=%s", m_material->getName().c_str());
            return;
        }

        for (Size i = 0, in = uniforms.size(); i != in; ++i)
        {
            ShaderUniformVariantPtr const& uniformVariant = uniforms[i];
            if (uniformVariant)
                m_shaderUniforms[i] = m_shader->getUniform(uniformVariant->getName());
        }
    }

    //////////////////////////////////////////
    HashedString const& RenderPass::getShaderName() const
    {
        static HashedString nullValue;

        if (!m_shader)
            return nullValue;

        return m_shader->getName();

    }

    //////////////////////////////////////////
    void RenderPass::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        XMLHelper::DeserializeMetaInstanceFromXMLElement(getMetaClass(), getMetaInstance(), _element);
    }

    //////////////////////////////////////////
    tinyxml2::XMLElement* RenderPass::toXMLElement(tinyxml2::XMLDocument& _doc) const
    {
        return XMLHelper::SerializeMetaInstanceToXMLElement(getMetaClass(), getMetaInstance(), _doc);
    }

    //////////////////////////////////////////
    bool RenderPass::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);

        ShaderFeature::LoadFromDataBlock(m_shaderFeatures, _dataBlock);
        updateShader();

        return true;
    }

    //////////////////////////////////////////
    void RenderPass::toDataBlock(DataBlock& _dataBlock) const
    {
        DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
        ShaderFeature::ToDataBlock(m_shaderFeatures, _dataBlock);
    }

    //////////////////////////////////////////
    void RenderPass::setMaterial(Material* _material)
    {
        m_material = _material;
    }

    //////////////////////////////////////////
    U32 RenderPass::calculateCRC32(U32 _seed) const
    {
        U32 crc32 = m_material->calculateUniformsCRC32(_seed);

        crc32 = Hash::CalculateCRC32((Char const*)&m_passType, sizeof(m_passType), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_renderQueueIndex, sizeof(m_renderQueueIndex), crc32);

        Shader* const shaderRaw = m_shader.get();
        crc32 = Hash::CalculateCRC32((Char const*)&shaderRaw, sizeof(Shader*), crc32);

        crc32 = Hash::CalculateCRC32((Char const*)&m_blendOperation, sizeof(m_blendOperation), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_blendSrcFactor, sizeof(m_blendSrcFactor), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_blendDestFactor, sizeof(m_blendDestFactor), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_depthTestCompareFunction, sizeof(m_depthTestCompareFunction), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_depthWriteEnabled, sizeof(m_depthWriteEnabled), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_cullMode, sizeof(m_cullMode), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_alphaToCoverageEnabled, sizeof(m_alphaToCoverageEnabled), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilTestCompareFunction, sizeof(m_stencilTestCompareFunction), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilReferenceValue, sizeof(m_stencilReferenceValue), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilReadMask, sizeof(m_stencilReadMask), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilWriteMask, sizeof(m_stencilWriteMask), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilFailOperation, sizeof(m_stencilFailOperation), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilDepthFailOperation, sizeof(m_stencilDepthFailOperation), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_stencilPassOperation, sizeof(m_stencilPassOperation), crc32);

        return crc32;
    }

    //////////////////////////////////////////
    bool RenderPass::hasUniform(HashedCString _uniformName)
    {
        if (!m_shader)
            return false;

        return m_shader->hasUniform(_uniformName);
    }

} // namespace Maze
//////////////////////////////////////////
