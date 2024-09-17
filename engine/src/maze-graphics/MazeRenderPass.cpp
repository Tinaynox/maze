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
#include "maze-graphics/MazeShaderSystem.hpp"
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
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, renderQueueIndex, (S32)RenderQueueIndex::Opaque, getRenderQueueIndex, setRenderQueueIndex),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ShaderAssetRef, shader, ShaderAssetRef(), getShaderRef, setShaderRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(BlendOperation, blendOperation, BlendOperation::Add, getBlendOperation, setBlendOperation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(BlendFactor, blendSrcFactor, BlendFactor::One, getBlendSrcFactor, setBlendSrcFactor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(BlendFactor, blendDestFactor, BlendFactor::Zero, getBlendDestFactor, setBlendDestFactor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CompareFunction, depthTestCompareFunction, CompareFunction::Disabled, getDepthTestCompareFunction, setDepthTestCompareFunction),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, depthWriteEnabled, true, getDepthWriteEnabled, setDepthWriteEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CullMode, cullMode, CullMode::Off, getCullMode, setCullMode)
    )

        //////////////////////////////////////////
        RenderPass::RenderPass()
        : m_renderSystem(nullptr)
        , m_material(nullptr)
        , m_passType(RenderPassType::None)
        , m_renderQueueIndex((S32)RenderQueueIndex::Opaque)
        , m_blendOperation(BlendOperation::Add)
        , m_blendSrcFactor(BlendFactor::One)
        , m_blendDestFactor(BlendFactor::Zero)
        , m_depthTestCompareFunction(CompareFunction::Disabled)
        , m_depthWriteEnabled(true)
        , m_cullMode(CullMode::Off)
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

        return true;
    }

    //////////////////////////////////////////
    bool RenderPass::set(RenderPass const& _renderPass)
    {
        m_renderSystem = _renderPass.m_renderSystem;
        m_passType = _renderPass.m_passType;
        m_renderQueueIndex = _renderPass.m_renderQueueIndex;
        m_shaderRef = _renderPass.m_shaderRef;
        m_blendOperation = _renderPass.m_blendOperation;
        m_blendSrcFactor = _renderPass.m_blendSrcFactor;
        m_blendDestFactor = _renderPass.m_blendDestFactor;
        m_depthTestCompareFunction = _renderPass.m_depthTestCompareFunction;
        m_depthWriteEnabled = _renderPass.m_depthWriteEnabled;
        m_cullMode = _renderPass.m_cullMode;

        return true;
    }

    //////////////////////////////////////////
    bool RenderPass::isEqual(RenderPass const& _renderPass) const
    {
        return MetaClassHelper::IsEqual(getMetaInstance(), _renderPass.getMetaInstance());
    }

    //////////////////////////////////////////
    void RenderPass::setShader(ShaderPtr const& _shader)
    {
        if (m_shaderRef.getShader() == _shader)
            return;

        m_shaderRef.setShader(_shader);
    }

    //////////////////////////////////////////
    void RenderPass::setShader(Path const& _shaderResourceName)
    {
        ShaderPtr const& shader = m_renderSystem->getShaderSystem()->getOrLoadShader(_shaderResourceName);
        setShader(shader);
    }

    //////////////////////////////////////////
    void RenderPass::applyRenderPassUniforms()
    {
        if (!m_material)
            return;

        m_material->applyRenderPassUniforms(this);
    }

    //////////////////////////////////////////
    String const& RenderPass::getShaderName() const
    {
        static String nullValue;

        if (!m_shaderRef.getShader())
            return nullValue;

        return m_shaderRef.getShader()->getName();

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
        return true;
    }

    //////////////////////////////////////////
    void RenderPass::toDataBlock(DataBlock& _dataBlock) const
    {
        DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
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

        Shader* const shaderRaw = m_shaderRef.getShader().get();
        crc32 = Hash::CalculateCRC32((Char const*)&shaderRaw, sizeof(Shader*), crc32);

        crc32 = Hash::CalculateCRC32((Char const*)&m_blendOperation, sizeof(m_blendOperation), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_blendSrcFactor, sizeof(m_blendSrcFactor), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_blendDestFactor, sizeof(m_blendDestFactor), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_depthTestCompareFunction, sizeof(m_depthTestCompareFunction), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_depthWriteEnabled, sizeof(m_depthWriteEnabled), crc32);
        crc32 = Hash::CalculateCRC32((Char const*)&m_cullMode, sizeof(m_cullMode), crc32);

        return crc32;
    }

    //////////////////////////////////////////
    bool RenderPass::hasUniform(HashedCString _uniformName)
    {
        if (!m_shaderRef.getShader())
            return false;

        return m_shaderRef.getShader()->hasUniform(_uniformName);
    }

} // namespace Maze
//////////////////////////////////////////
