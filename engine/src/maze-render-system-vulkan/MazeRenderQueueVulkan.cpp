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
#include "maze-render-system-vulkan/MazeRenderQueueVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeStateMachineVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderVulkan.hpp"
#include "maze-render-system-vulkan/MazeVertexArrayObjectVulkan.hpp"
#include "maze-render-system-vulkan/instance-stream/MazeInstanceStreamsVulkan.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static HashedCString const c_renderTargetFlipYUniformName = MAZE_HASHED_CSTRING("u_renderTargetFlipY");


    //////////////////////////////////////////
    // Class RenderQueueVulkan
    //
    //////////////////////////////////////////
    RenderQueueVulkan::RenderQueueVulkan()
    {
        for (Size i = 0; i < 8; ++i)
        {
            m_clipPlanes[i] = Vec4F::c_zero;
            m_clipPlaneEnabled[i] = false;
        }
    }

    //////////////////////////////////////////
    RenderQueueVulkan::~RenderQueueVulkan()
    {
    }

    //////////////////////////////////////////
    RenderQueueVulkanPtr RenderQueueVulkan::Create(RenderTarget* _renderTarget)
    {
        RenderQueueVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderQueueVulkan, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool RenderQueueVulkan::init(RenderTarget* _renderTarget)
    {
        if (!RenderQueue::init(_renderTarget))
            return false;

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();

        m_instanceStreamModelMatrix = InstanceStreamModelMatrixVulkan::Create(renderSystem);
        m_instanceStreamColor = InstanceStreamColorVulkan::Create(renderSystem);

        m_maxInstancesPerDrawCall = m_instanceStreamModelMatrix->getMaxInstancesPerDrawCall();
        m_maxInstancesPerDraw = m_instanceStreamModelMatrix->getMaxInstancePerDraw();

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
        {
            m_instanceStreamUVs[i] = InstanceStreamUVVulkan::Create(i, renderSystem);

            m_maxInstancesPerDrawCall = Math::Min(m_maxInstancesPerDrawCall, m_instanceStreamUVs[i]->getMaxInstancesPerDrawCall());
            m_maxInstancesPerDraw = Math::Min(m_maxInstancesPerDraw, m_instanceStreamUVs[i]->getMaxInstancePerDraw());
        }

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* RenderQueueVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderTarget->getRenderSystem()->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    void RenderQueueVulkan::draw()
    {
        MAZE_PROFILE_EVENT("RenderQueueVulkan::draw");

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        StateMachineVulkan* stateMachine = renderSystem->getStateMachine();

        m_drawTime = UpdateManager::GetInstancePtr()->getAppTime();

        processDrawBegin();
        m_instanceStreamModelMatrix->setOffset(0);
        m_instanceStreamColor->setOffset(0);

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->setOffset(0);

        m_renderCommandsBuffer.executeAndClear(
            [&](RenderCommand* _command)
            {
                switch (_command->type)
                {
                    case RenderCommandType::ClearCurrentRenderTarget:
                    {
                        MAZE_PROFILE_EVENT("Clear");

                        RenderCommandClearCurrentRenderTarget* command = static_cast<RenderCommandClearCurrentRenderTarget*>(_command);

                        // Dynamic rendering has no separate "clear bound
                        // target" call - StateMachineVulkan::bindRenderTarget
                        // always uses LOAD_OP_LOAD, so an explicit clear
                        // (requested mid-pass, e.g. after some draws already
                        // happened) is done via vkCmdClearAttachments instead
                        VkCommandBuffer cmd = renderSystem->getCurrentCommandBuffer();
                        if (cmd != VK_NULL_HANDLE && stateMachine->isRenderingActive())
                        {
                            Vector<VkClearAttachment> clears;
                            if (command->colorBuffer)
                            {
                                ColorU32 const& clearColor = m_renderTarget->getClearColor();
                                Vec4F c = clearColor.toVec4F32();
                                VkClearAttachment clearAttachment = {};
                                clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                                clearAttachment.colorAttachment = 0u;
                                clearAttachment.clearValue.color = { { c.x, c.y, c.z, c.w } };
                                clears.push_back(clearAttachment);
                            }
                            if (command->depthBuffer || command->stencilBuffer)
                            {
                                VkClearAttachment clearAttachment = {};
                                clearAttachment.aspectMask =
                                    (command->depthBuffer ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) |
                                    (command->stencilBuffer ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
                                clearAttachment.clearValue.depthStencil = { m_renderTarget->getClearDepth(), 0u };
                                clears.push_back(clearAttachment);
                            }

                            if (!clears.empty())
                            {
                                VkClearRect clearRect = {};
                                clearRect.rect.offset = { 0, 0 };
                                clearRect.rect.extent = { stateMachine->getRenderTargetSize().x, stateMachine->getRenderTargetSize().y };
                                clearRect.layerCount = 1u;
                                vkCmdClearAttachments(cmd, (U32)clears.size(), clears.data(), 1u, &clearRect);
                            }
                        }

                        break;
                    }
                    case RenderCommandType::SetRenderPass:
                    {
                        MAZE_PROFILE_EVENT("SetRenderPass");

                        RenderCommandSetRenderPass* command = static_cast<RenderCommandSetRenderPass*>(_command);

                        bindRenderPass(command->renderPass, command->bindTextures);
                        break;
                    }
                    case RenderCommandType::BindTextures:
                    {
                        MAZE_PROFILE_EVENT("BindTextures");

                        // Texture bindings are written into the shader's
                        // descriptor set lazily in ShaderUniformVulkan::processSimpleUniformChanged()
                        // as soon as a texture uniform is set/changed - there
                        // is no separate "bind all textures now" step needed
                        // for this backend, unlike DX11's SRV-slot model
                        break;
                    }
                    case RenderCommandType::DrawVAOInstanced:
                    {
                        MAZE_PROFILE_EVENT("DrawVAOInstanced");

                        RenderCommandDrawVAOInstanced* command = static_cast<RenderCommandDrawVAOInstanced*>(_command);

                        do
                        {
                            ShaderVulkan* shader = stateMachine->getCurrentShader();
                            if (!shader || !shader->isValid())
                                break;

                            S32 slotOffset = renderSystem->allocateInstanceStreamSlots(command->count);

                            m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixVulkan>()->prepareForRender(command->count, slotOffset);

                            if (command->useColorStream)
                                m_instanceStreamColor->castRaw<InstanceStreamColorVulkan>()->prepareForRender(command->count, slotOffset);

                            for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                                if (command->uvMask & (1 << i))
                                    m_instanceStreamUVs[i]->castRaw<InstanceStreamUVVulkan>()->prepareForRender(command->count, slotOffset);

                            VertexArrayObjectVulkan* vao = command->vao->castRaw<VertexArrayObjectVulkan>();

                            S32 indicesCount = (S32)vao->getIndicesCount();
                            // No active dynamic-rendering scope (e.g. the
                            // current render target's size was zero when
                            // bound - see StateMachineVulkan::bindRenderTarget's
                            // guard) means vkCmdDrawIndexed would be recorded
                            // outside a render pass instance, which is invalid
                            if (indicesCount > 0 && stateMachine->isRenderingActive())
                            {
#if (MAZE_DEBUG)
                                S32 renderSystemDrawCallsLimit = m_renderTarget->getRenderSystem()->getDrawCallsLimit();
                                if (   ((m_drawCallsLimit == 0) || (m_drawCalls < m_drawCallsLimit))
                                    && ((renderSystemDrawCallsLimit == 0) || (m_renderTarget->getRenderSystem()->getDrawCalls() < renderSystemDrawCallsLimit)))
                                {
#endif
                                    VkCommandBuffer cmd = renderSystem->getCurrentCommandBuffer();

                                    stateMachine->setCurrentVertexArrayObject(vao);
                                    stateMachine->flushPipeline();

                                    vkCmdPushConstants(
                                        cmd, shader->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT,
                                        0u, sizeof(S32), &slotOffset);

                                    vkCmdDrawIndexed(cmd, (U32)indicesCount, (U32)command->count, 0u, 0, 0u);
#if (MAZE_DEBUG)
                                }
#endif

                                ++m_drawCalls;
                                m_renderTarget->getRenderSystem()->incDrawCall();
                            }

                        }
                        while (false);

                        m_instanceStreamModelMatrix->setOffset(m_instanceStreamModelMatrix->getOffset() + command->count);

                        if (command->useColorStream)
                            m_instanceStreamColor->setOffset(m_instanceStreamColor->getOffset() + command->count);

                        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                            if (command->uvMask & (1 << i))
                                m_instanceStreamUVs[i]->setOffset(m_instanceStreamUVs[i]->getOffset() + command->count);

                        break;
                    }
                    case RenderCommandType::PushScissorRect:
                    {
                        RenderCommandPushScissorRect* command = static_cast<RenderCommandPushScissorRect*>(_command);

                        F32 posX = Math::Round(m_renderTarget->getRenderTargetWidth() * command->scissorRect.position.x);
                        F32 posY = Math::Round(m_renderTarget->getRenderTargetHeight() * command->scissorRect.position.y);
                        F32 sizeX = Math::Round(m_renderTarget->getRenderTargetWidth() * command->scissorRect.size.x);
                        F32 sizeY = Math::Round(m_renderTarget->getRenderTargetHeight() * command->scissorRect.size.y);

                        F32 fractX = Math::Fract(posX);
                        F32 fractY = Math::Fract(posY);

                        sizeX += fractX;
                        sizeY += fractY;

                        Rect2S scissorRect;
                        if (!m_scissorRects.empty())
                        {
                            scissorRect = m_scissorRects.top().intersectedCopy(
                                Rect2S(
                                    (S32)posX,
                                    (S32)posY,
                                    (S32)(sizeX + 0.5f),
                                    (S32)(sizeY + 0.5f)));
                        }
                        else
                        {
                            scissorRect = Rect2S(
                                (S32)(posX),
                                (S32)(posY),
                                (S32)(sizeX + 0.5f),
                                (S32)(sizeY + 0.5f));
                        }

                        m_scissorRects.push(scissorRect);
                        stateMachine->setScissorRect(scissorRect);
                        stateMachine->setScissorTestEnabled(true);

                        break;
                    }
                    case RenderCommandType::PopScissorRect:
                    {
                        m_scissorRects.pop();

                        if (m_scissorRects.empty())
                            stateMachine->setScissorTestEnabled(false);
                        else
                            stateMachine->setScissorRect(m_scissorRects.top());

                        break;
                    }
                    case RenderCommandType::EnableClipPlane:
                    {
                        RenderCommandEnableClipPlane* command = static_cast<RenderCommandEnableClipPlane*>(_command);

                        m_clipPlaneEnabled[command->index] = true;
                        m_clipPlanes[command->index] = command->plane;

                        break;
                    }
                    case RenderCommandType::DisableClipPlane:
                    {
                        RenderCommandDisableClipPlane* command = static_cast<RenderCommandDisableClipPlane*>(_command);

                        m_clipPlaneEnabled[command->index] = false;

                        break;
                    }
                    case RenderCommandType::SetShaderUniformVec2F:
                    {
                        RenderCommandSetShaderUniformVec2F* command = static_cast<RenderCommandSetShaderUniformVec2F*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->set(command->value);
                        }
                        break;
                    }
                    case RenderCommandType::SetShaderUniformTexture2D:
                    {
                        RenderCommandSetShaderUniformTexture2D* command = static_cast<RenderCommandSetShaderUniformTexture2D*>(_command);
                        if (Texture2D const* texture = Texture2D::GetResourceFast(command->texture2DId))
                        {
                            if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                            {
                                ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                                if (uniform)
                                    uniform->set(texture);
                            }
                        }
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformVec2F:
                    {
                        RenderCommandUploadShaderUniformVec2F* command = static_cast<RenderCommandUploadShaderUniformVec2F*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->upload(command->pointer, (Size)command->count);
                        }
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformVec3F:
                    {
                        RenderCommandUploadShaderUniformVec3F* command = static_cast<RenderCommandUploadShaderUniformVec3F*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->upload(command->pointer, (Size)command->count);
                        }
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformVec4F:
                    {
                        RenderCommandUploadShaderUniformVec4F* command = static_cast<RenderCommandUploadShaderUniformVec4F*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->upload(command->pointer, (Size)command->count);
                        }
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformMat3F:
                    {
                        RenderCommandUploadShaderUniformMat3F* command = static_cast<RenderCommandUploadShaderUniformMat3F*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->upload(command->pointer, (Size)command->count);
                        }
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformMat4F:
                    {
                        RenderCommandUploadShaderUniformMat4F* command = static_cast<RenderCommandUploadShaderUniformMat4F*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->upload(command->pointer, (Size)command->count);
                        }
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformTMat:
                    {
                        RenderCommandUploadShaderUniformTMat* command = static_cast<RenderCommandUploadShaderUniformTMat*>(_command);
                        if (ShaderVulkan* shader = stateMachine->getCurrentShader())
                        {
                            ShaderUniformPtr const& uniform = shader->getUniform(command->name);
                            if (uniform)
                                uniform->upload(command->pointer, (Size)command->count);
                        }
                        break;
                    }
                    default:
                    {
                        Debug::LogError("Unsupported RenderCommand: %d", (S32)_command->type);
                        break;
                    }
                }
            });

        clear();
    }

    //////////////////////////////////////////
    void RenderQueueVulkan::clear()
    {
        RenderQueue::clear();

        m_instanceStreamModelMatrix->setOffset(0);
        m_instanceStreamColor->setOffset(0);

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->setOffset(0);
    }

    //////////////////////////////////////////
    void RenderQueueVulkan::bindRenderPass(RenderPass* _renderPass, bool _bindTextures)
    {
        StateMachineVulkan* stateMachine = getRenderSystemVulkanRaw()->getStateMachine();

        // Blend Mode
        BlendFactor srcFactor = _renderPass->getBlendSrcFactor();
        BlendFactor destFactor = _renderPass->getBlendDestFactor();

        bool blendEnabled = !(srcFactor == BlendFactor::One && destFactor == BlendFactor::Zero);
        stateMachine->setBlendEnabled(blendEnabled);
        if (blendEnabled)
        {
            stateMachine->setBlendFactors(
                srcFactor,
                destFactor);
        }

        // Depth Test
        CompareFunction depthTestCompareFunction = _renderPass->getDepthTestCompareFunction();
        bool depthTestEnabled = (depthTestCompareFunction != CompareFunction::Disabled);
        stateMachine->setDepthTestEnabled(depthTestEnabled);
        if (depthTestEnabled)
        {
            stateMachine->setDepthTestCompareFunction(depthTestCompareFunction);
        }

        // Depth Write
        stateMachine->setDepthWriteEnabled(_renderPass->getDepthWriteEnabled());

        // Cull Mode
        CullMode cullMode = _renderPass->getCullMode();
        bool cullingEnabled = (cullMode != CullMode::Off);
        stateMachine->setCullEnabled(cullingEnabled);
        if (cullingEnabled)
        {
            stateMachine->setCullMode(cullMode);
        }

        // Stencil Test
        CompareFunction stencilTestCompareFunction = _renderPass->getStencilTestCompareFunction();
        bool stencilTestEnabled = (stencilTestCompareFunction != CompareFunction::Disabled);
        stateMachine->setStencilTestEnabled(stencilTestEnabled);
        if (stencilTestEnabled)
        {
            stateMachine->setStencilFunc(
                stencilTestCompareFunction,
                _renderPass->getStencilReferenceValue(),
                _renderPass->getStencilReadMask());
            stateMachine->setStencilOp(
                _renderPass->getStencilFailOperation(),
                _renderPass->getStencilDepthFailOperation(),
                _renderPass->getStencilPassOperation());
        }
        stateMachine->setStencilWriteMask(_renderPass->getStencilWriteMask());


        // Shader
        ShaderVulkan* shader = _renderPass->getShader()->castRaw<ShaderVulkan>();
        stateMachine->setCurrentShader(shader);

        // Shader uniforms
        _renderPass->applyRenderPassUniforms();

        // Bind textures - no-op for this backend, see RenderCommandType::BindTextures above
        MAZE_UNUSED(_bindTextures);


        // View matrix
        //
        // All the global (GlobalUniforms UBO) uniform set() calls below are
        // safe from the "shader A's cache masks shader B's overwrite of the
        // shared buffer" hazard - see ShaderUniform::setAlwaysForceUpdate()'s
        // banner comment - since ShaderUniformVulkan::setUniformData() sets
        // that flag automatically for every uniform reflected as global.
        if (shader->getViewMatrixUniform())
        {
            shader->getViewMatrixUniform()->set(m_renderTarget->getViewMatrix());
        }

        // Clip distances
        bool clipDistance0 = m_clipPlaneEnabled[0];

        if (shader->getClipDistanceEnableUniform())
            shader->getClipDistanceEnableUniform()->set(Vec4B{ clipDistance0, false, false, false });

        if (shader->getClipDistance0Uniform())
        {
            if (clipDistance0)
                shader->getClipDistance0Uniform()->set(m_clipPlanes[0]);
            else
                shader->getClipDistance0Uniform()->set(Vec4F::c_zero);
        }

        // Projection matrix
        if (shader->getProjectionMatrixUniform())
            shader->getProjectionMatrixUniform()->set(m_renderTarget->getProjectionMatrix());

        // Projection params
        if (shader->getProjectionParamsUniform())
        {
            shader->getProjectionParamsUniform()->set(
                Vec4F{ m_renderTarget->getNear(), m_renderTarget->getFar(), 0.0f, 0.0f });
        }

        // View direction
        if (shader->getViewPositionUniform())
        {
            shader->getViewPositionUniform()->set(m_renderTarget->getViewPosition());
        }

        // Time
        if (shader->getTimeUniform())
        {
            shader->getTimeUniform()->set(m_drawTime);
        }

        // Render target orientation (offscreen targets are rendered flipped)
        ShaderUniformPtr const& flipYUniform = shader->getUniform(c_renderTargetFlipYUniformName);
        if (flipYUniform)
            flipYUniform->set(stateMachine->getFlipY() ? -1.0f : 1.0f);
    }

    //////////////////////////////////////////
    void RenderQueueVulkan::processDrawBegin()
    {
        StateMachineVulkan* stateMachine = getRenderSystemVulkanRaw()->getStateMachine();

        stateMachine->setViewportRect(
            Rect2S(
                (S32)Math::Round(m_renderTarget->getRenderTargetWidth() * m_renderTarget->getViewport().position.x),
                (S32)Math::Round(m_renderTarget->getRenderTargetHeight() * m_renderTarget->getViewport().position.y),
                (S32)Math::Round(m_renderTarget->getRenderTargetWidth() * m_renderTarget->getViewport().size.x),
                (S32)Math::Round(m_renderTarget->getRenderTargetHeight() * m_renderTarget->getViewport().size.y)));

        m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixVulkan>()->processDrawBegin();
        m_instanceStreamColor->castRaw<InstanceStreamColorVulkan>()->processDrawBegin();

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->castRaw<InstanceStreamUVVulkan>()->processDrawBegin();
    }


} // namespace Maze
//////////////////////////////////////////
