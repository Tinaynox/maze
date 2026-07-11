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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeStateMachineVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderVulkan.hpp"
#include "maze-render-system-vulkan/MazeVertexArrayObjectVulkan.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    bool PipelineKeyVulkan::operator==(PipelineKeyVulkan const& _other) const
    {
        return shader == _other.shader &&
               vertexLayoutHash == _other.vertexLayoutHash &&
               topology == _other.topology &&
               blendEnabled == _other.blendEnabled &&
               blendSrcFactor == _other.blendSrcFactor &&
               blendDestFactor == _other.blendDestFactor &&
               blendOperation == _other.blendOperation &&
               colorFormat == _other.colorFormat &&
               depthFormat == _other.depthFormat &&
               samples == _other.samples;
    }

    //////////////////////////////////////////
    Size PipelineKeyVulkanHash::operator()(PipelineKeyVulkan const& _key) const
    {
        U64 h = CombineHashVulkan(0u, (U64)(Size)_key.shader);
        h = CombineHashVulkan(h, _key.vertexLayoutHash);
        h = CombineHashVulkan(h, (U64)_key.topology);
        h = CombineHashVulkan(h, (U64)_key.blendEnabled);
        h = CombineHashVulkan(h, (U64)_key.blendSrcFactor);
        h = CombineHashVulkan(h, (U64)_key.blendDestFactor);
        h = CombineHashVulkan(h, (U64)_key.blendOperation);
        h = CombineHashVulkan(h, (U64)_key.colorFormat);
        h = CombineHashVulkan(h, (U64)_key.depthFormat);
        h = CombineHashVulkan(h, (U64)_key.samples);
        return (Size)h;
    }


    //////////////////////////////////////////
    // Class StateMachineVulkan
    //
    //////////////////////////////////////////
    StateMachineVulkan::StateMachineVulkan(RenderSystemVulkan* _renderSystem)
        : m_renderSystem(_renderSystem)
    {
        for (S32 i = 0; i < c_colorAttachmentsMax; ++i)
            m_colorViews[i] = VK_NULL_HANDLE;
    }

    //////////////////////////////////////////
    StateMachineVulkan::~StateMachineVulkan()
    {
        for (auto& pipelineData : m_pipelines)
            if (pipelineData.second != VK_NULL_HANDLE)
                vkDestroyPipeline(getDevice(), pipelineData.second, nullptr);
        m_pipelines.clear();
    }

    //////////////////////////////////////////
    VkDevice StateMachineVulkan::getDevice() const
    {
        return m_renderSystem->getDevice();
    }

    //////////////////////////////////////////
    VkCommandBuffer StateMachineVulkan::getCommandBuffer() const
    {
        return m_renderSystem->getCurrentCommandBuffer();
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setBlendEnabled(bool _value)
    {
        if (m_blendEnabled == _value)
            return;
        m_blendEnabled = _value;
        m_pipelineDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setBlendFactors(BlendFactor _srcFactor, BlendFactor _destFactor)
    {
        if (m_blendSrcFactor == _srcFactor && m_blendDestFactor == _destFactor)
            return;
        m_blendSrcFactor = _srcFactor;
        m_blendDestFactor = _destFactor;
        m_pipelineDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setBlendOperation(BlendOperation _value)
    {
        if (m_blendOperation == _value)
            return;
        m_blendOperation = _value;
        m_pipelineDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setDepthTestEnabled(bool _value)
    {
        if (m_depthTestEnabled == _value)
            return;
        m_depthTestEnabled = _value;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setDepthTestCompareFunction(CompareFunction _value)
    {
        if (m_depthTestCompareFunction == _value)
            return;
        m_depthTestCompareFunction = _value;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setDepthWriteEnabled(bool _value)
    {
        if (m_depthWriteEnabled == _value)
            return;
        m_depthWriteEnabled = _value;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setCullEnabled(bool _value)
    {
        if (m_cullEnabled == _value)
            return;
        m_cullEnabled = _value;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setCullMode(CullMode _value)
    {
        if (m_cullMode == _value)
            return;
        m_cullMode = _value;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setStencilTestEnabled(bool _value)
    {
        if (m_stencilTestEnabled == _value)
            return;
        m_stencilTestEnabled = _value;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setStencilFunc(CompareFunction _function, S32 _referenceValue, U32 _readMask)
    {
        m_stencilTestCompareFunction = _function;
        m_stencilReferenceValue = _referenceValue;
        m_stencilReadMask = _readMask;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setStencilOp(StencilOperation _fail, StencilOperation _depthFail, StencilOperation _pass)
    {
        m_stencilFailOperation = _fail;
        m_stencilDepthFailOperation = _depthFail;
        m_stencilPassOperation = _pass;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setStencilWriteMask(U32 _mask)
    {
        if (m_stencilWriteMask == _mask)
            return;
        m_stencilWriteMask = _mask;
        m_dynamicStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setScissorTestEnabled(bool _value)
    {
        m_scissorTestEnabled = _value;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setScissorRect(Rect2S const& _rect)
    {
        m_scissorRect = _rect;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setViewportRect(Rect2S const& _rect)
    {
        m_viewportRect = _rect;
        m_viewportDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setTopology(RenderDrawTopology _topology)
    {
        if (m_topology == _topology)
            return;
        m_topology = _topology;
        m_pipelineDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setCurrentShader(ShaderVulkan* _shader)
    {
        if (m_currentShader == _shader)
            return;
        m_currentShader = _shader;
        m_pipelineDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::setCurrentVertexArrayObject(VertexArrayObjectVulkan* _vao)
    {
        if (m_currentVAO == _vao)
            return;
        m_currentVAO = _vao;
        m_pipelineDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::bindRenderTarget(
        VkImageView const* _colorViews,
        S32 _colorViewsCount,
        VkImageView _depthView,
        Vec2U const& _renderTargetSize,
        bool _flipY,
        VkFormat _colorFormat,
        VkFormat _depthFormat,
        VkSampleCountFlagBits _samples)
    {
        if (m_renderingActive)
            unbindRenderTarget();

        m_colorViewsCount = Math::Min(_colorViewsCount, c_colorAttachmentsMax);
        for (S32 i = 0; i < m_colorViewsCount; ++i)
            m_colorViews[i] = _colorViews[i];
        for (S32 i = m_colorViewsCount; i < c_colorAttachmentsMax; ++i)
            m_colorViews[i] = VK_NULL_HANDLE;

        m_depthView = _depthView;
        m_renderTargetSize = _renderTargetSize;
        m_flipY = _flipY;
        m_colorFormat = _colorFormat;
        m_depthFormat = _depthFormat;
        m_samples = _samples;
        m_pipelineDirty = true;

        VkRenderingAttachmentInfo colorAttachments[c_colorAttachmentsMax] = {};
        for (S32 i = 0; i < m_colorViewsCount; ++i)
        {
            colorAttachments[i].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
            colorAttachments[i].imageView = m_colorViews[i];
            colorAttachments[i].imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            colorAttachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        }

        VkRenderingAttachmentInfo depthAttachment = {};
        depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        depthAttachment.imageView = m_depthView;
        depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        VkRenderingInfo renderingInfo = {};
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        renderingInfo.renderArea.offset = { 0, 0 };
        renderingInfo.renderArea.extent = { _renderTargetSize.x, _renderTargetSize.y };
        renderingInfo.layerCount = 1u;
        renderingInfo.colorAttachmentCount = (U32)m_colorViewsCount;
        renderingInfo.pColorAttachments = colorAttachments;
        renderingInfo.pDepthAttachment = (m_depthView != VK_NULL_HANDLE) ? &depthAttachment : nullptr;

        vkCmdBeginRendering(getCommandBuffer(), &renderingInfo);
        m_renderingActive = true;

        m_viewportDirty = true;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::unbindRenderTarget()
    {
        if (!m_renderingActive)
            return;

        vkCmdEndRendering(getCommandBuffer());
        m_renderingActive = false;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::invalidateDeviceState()
    {
        m_pipelineDirty = true;
        m_dynamicStateDirty = true;
        m_viewportDirty = true;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    VkPipeline StateMachineVulkan::ensurePipeline()
    {
        if (!m_currentShader || !m_currentVAO)
            return VK_NULL_HANDLE;

        PipelineKeyVulkan key;
        key.shader = m_currentShader;
        key.vertexLayoutHash = m_currentVAO->getLayoutHash();
        key.topology = m_topology;
        key.blendEnabled = m_blendEnabled;
        key.blendSrcFactor = m_blendSrcFactor;
        key.blendDestFactor = m_blendDestFactor;
        key.blendOperation = m_blendOperation;
        key.colorFormat = m_colorFormat;
        key.depthFormat = m_depthFormat;
        key.samples = m_samples;

        auto it = m_pipelines.find(key);
        if (it != m_pipelines.end())
            return it->second;

        Vector<VkVertexInputBindingDescription> bindingDescs;
        Vector<VkVertexInputAttributeDescription> attributeDescs;
        m_currentVAO->buildVertexInputState(m_currentShader, bindingDescs, attributeDescs);

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = (U32)bindingDescs.size();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescs.data();
        vertexInputInfo.vertexAttributeDescriptionCount = (U32)attributeDescs.size();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescs.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = GetRenderDrawTopologyVulkan(m_topology);

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1u;
        viewportState.scissorCount = 1u;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        // Cull mode/front face are set dynamically via vkCmdSetCullModeEXT /
        // vkCmdSetFrontFaceEXT (VK_EXT_extended_dynamic_state) - fixed here at
        // a neutral default
        rasterizer.cullMode = VK_CULL_MODE_NONE;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // Maze runs glFrontFace(GL_CW) equivalent, flipped when RT-flipped (see StateMachineVulkan::flushPipeline dynamic front-face)
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.rasterizationSamples = m_samples;
        multisampling.sampleShadingEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState blendAttachment = {};
        blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blendAttachment.blendEnable = m_blendEnabled ? VK_TRUE : VK_FALSE;
        blendAttachment.srcColorBlendFactor = GetBlendFactorVulkan(m_blendSrcFactor);
        blendAttachment.dstColorBlendFactor = GetBlendFactorVulkan(m_blendDestFactor);
        blendAttachment.colorBlendOp = GetBlendOperationVulkan(m_blendOperation);
        blendAttachment.srcAlphaBlendFactor = GetBlendFactorVulkan(m_blendSrcFactor);
        blendAttachment.dstAlphaBlendFactor = GetBlendFactorVulkan(m_blendDestFactor);
        blendAttachment.alphaBlendOp = GetBlendOperationVulkan(m_blendOperation);

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.attachmentCount = m_colorViewsCount > 0 ? 1u : 0u;
        colorBlending.pAttachments = &blendAttachment;

        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        // Depth/stencil test/write/compare/op are all set dynamically
        // (VK_EXT_extended_dynamic_state) - baked defaults here are
        // overridden every draw in flushPipeline()
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

        Vector<VkDynamicState> dynamicStates =
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_CULL_MODE,
            VK_DYNAMIC_STATE_FRONT_FACE,
            VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
            VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
            VK_DYNAMIC_STATE_DEPTH_COMPARE_OP,
            VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE,
            VK_DYNAMIC_STATE_STENCIL_OP,
            VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
            VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
            VK_DYNAMIC_STATE_STENCIL_REFERENCE,
        };

        VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.dynamicStateCount = (U32)dynamicStates.size();
        dynamicStateInfo.pDynamicStates = dynamicStates.data();

        VkPipelineShaderStageCreateInfo stages[2] = {};
        stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        stages[0].module = m_currentShader->getVertexModule();
        stages[0].pName = "main";
        stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        stages[1].module = m_currentShader->getFragmentModule();
        stages[1].pName = "main";

        VkFormat colorFormats[1] = { m_colorFormat };
        VkPipelineRenderingCreateInfo renderingCreateInfo = {};
        renderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        renderingCreateInfo.colorAttachmentCount = m_colorViewsCount > 0 ? 1u : 0u;
        renderingCreateInfo.pColorAttachmentFormats = colorFormats;
        renderingCreateInfo.depthAttachmentFormat = m_depthFormat;

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = &renderingCreateInfo;
        pipelineInfo.stageCount = 2u;
        pipelineInfo.pStages = stages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pDynamicState = &dynamicStateInfo;
        pipelineInfo.layout = m_currentShader->getPipelineLayout();

        VkPipeline pipeline = VK_NULL_HANDLE;
        VkResult result = vkCreateGraphicsPipelines(getDevice(), m_renderSystem->getPipelineCache(), 1u, &pipelineInfo, nullptr, &pipeline);
        MAZE_ERROR_IF(result != VK_SUCCESS, "vkCreateGraphicsPipelines failed! result=%d", (S32)result);

        m_pipelines.emplace(key, pipeline);
        return pipeline;
    }

    //////////////////////////////////////////
    void StateMachineVulkan::flushPipeline()
    {
        VkCommandBuffer cmd = getCommandBuffer();
        if (cmd == VK_NULL_HANDLE)
            return;

        VkPipeline pipeline = ensurePipeline();
        if (pipeline == VK_NULL_HANDLE)
            return;

        if (m_pipelineDirty)
        {
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            m_pipelineDirty = false;
        }

        if (m_viewportDirty)
        {
            VkViewport viewport = {};
            viewport.x = (F32)m_viewportRect.position.x;
            // Vulkan's viewport Y grows downward like D3D - flip when the
            // engine's rect convention is bottom-left-origin (matches how
            // DX11/GL both handle m_flipY for offscreen render targets)
            if (m_flipY)
            {
                viewport.y = (F32)m_viewportRect.position.y;
                viewport.height = (F32)m_viewportRect.size.y;
            }
            else
            {
                viewport.y = (F32)(m_viewportRect.position.y + m_viewportRect.size.y);
                viewport.height = -(F32)m_viewportRect.size.y;
            }
            viewport.width = (F32)m_viewportRect.size.x;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(cmd, 0u, 1u, &viewport);
            m_viewportDirty = false;
        }

        if (m_scissorDirty)
        {
            VkRect2D scissor = {};
            if (m_scissorTestEnabled)
            {
                scissor.offset = { m_scissorRect.position.x, m_scissorRect.position.y };
                scissor.extent = { (U32)m_scissorRect.size.x, (U32)m_scissorRect.size.y };
            }
            else
            {
                scissor.offset = { 0, 0 };
                scissor.extent = { m_renderTargetSize.x, m_renderTargetSize.y };
            }
            vkCmdSetScissor(cmd, 0u, 1u, &scissor);
            m_scissorDirty = false;
        }

        if (m_dynamicStateDirty)
        {
            VkCullModeFlags cullMode = m_cullEnabled ? GetCullModeVulkan(m_cullMode) : VK_CULL_MODE_NONE;
            vkCmdSetCullMode(cmd, cullMode);
            // Maze uses CW front-facing winding (glFrontFace(GL_CW)); flipped
            // to CCW when the render target is Y-flipped, matching the DX11
            // rasterizer-state convention (see project memory item 3)
            vkCmdSetFrontFace(cmd, m_flipY ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE);

            vkCmdSetDepthTestEnable(cmd, m_depthTestEnabled ? VK_TRUE : VK_FALSE);
            vkCmdSetDepthWriteEnable(cmd, m_depthWriteEnabled ? VK_TRUE : VK_FALSE);
            vkCmdSetDepthCompareOp(cmd, GetCompareFunctionVulkan(m_depthTestCompareFunction));

            vkCmdSetStencilTestEnable(cmd, m_stencilTestEnabled ? VK_TRUE : VK_FALSE);
            if (m_stencilTestEnabled)
            {
                VkStencilOp failOp = GetStencilOperationVulkan(m_stencilFailOperation);
                VkStencilOp depthFailOp = GetStencilOperationVulkan(m_stencilDepthFailOperation);
                VkStencilOp passOp = GetStencilOperationVulkan(m_stencilPassOperation);
                VkCompareOp compareOp = GetCompareFunctionVulkan(m_stencilTestCompareFunction);
                vkCmdSetStencilOp(cmd, VK_STENCIL_FACE_FRONT_AND_BACK, failOp, passOp, depthFailOp, compareOp);
                vkCmdSetStencilCompareMask(cmd, VK_STENCIL_FACE_FRONT_AND_BACK, m_stencilReadMask);
                vkCmdSetStencilWriteMask(cmd, VK_STENCIL_FACE_FRONT_AND_BACK, m_stencilWriteMask);
                vkCmdSetStencilReference(cmd, VK_STENCIL_FACE_FRONT_AND_BACK, (U32)m_stencilReferenceValue);
            }

            m_dynamicStateDirty = false;
        }

        // Descriptor sets: set 0 (global) is shared/constant for the frame,
        // set 1 (material) belongs to the current shader
        U32 frameIndex = m_renderSystem->getCurrentFrameIndex();
        m_currentShader->flushMaterialUniforms();
        VkDescriptorSet sets[2] =
        {
            m_renderSystem->getGlobalDescriptorSet(frameIndex),
            m_currentShader->getMaterialDescriptorSet(frameIndex)
        };
        vkCmdBindDescriptorSets(
            cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentShader->getPipelineLayout(),
            0u, 2u, sets, 0u, nullptr);

        m_currentVAO->bind(cmd, m_currentShader);
    }


} // namespace Maze
//////////////////////////////////////////
