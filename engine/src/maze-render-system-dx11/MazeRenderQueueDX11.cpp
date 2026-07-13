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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeRenderQueueDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-render-system-dx11/MazeVertexArrayObjectDX11.hpp"
#include "maze-render-system-dx11/instance-stream/MazeInstanceStreamsDX11.hpp"
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
    // Class RenderQueueDX11
    //
    //////////////////////////////////////////
    RenderQueueDX11::RenderQueueDX11()
    {
        for (Size i = 0; i < 8; ++i)
        {
            m_clipPlanes[i] = Vec4F::c_zero;
            m_clipPlaneEnabled[i] = false;
        }
    }

    //////////////////////////////////////////
    RenderQueueDX11::~RenderQueueDX11()
    {
    }

    //////////////////////////////////////////
    RenderQueueDX11Ptr RenderQueueDX11::Create(RenderTarget* _renderTarget)
    {
        RenderQueueDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderQueueDX11, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool RenderQueueDX11::init(RenderTarget* _renderTarget)
    {
        if (!RenderQueue::init(_renderTarget))
            return false;

        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();

        m_instanceStreamModelMatrix = InstanceStreamModelMatrixDX11::Create(renderSystem);
        m_instanceStreamColor = InstanceStreamColorDX11::Create(renderSystem);

        m_maxInstancesPerDrawCall = m_instanceStreamModelMatrix->getMaxInstancesPerDrawCall();
        m_maxInstancesPerDraw = m_instanceStreamModelMatrix->getMaxInstancePerDraw();

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
        {
            m_instanceStreamUVs[i] = InstanceStreamUVDX11::Create(i, renderSystem);

            m_maxInstancesPerDrawCall = Math::Min(m_maxInstancesPerDrawCall, m_instanceStreamUVs[i]->getMaxInstancesPerDrawCall());
            m_maxInstancesPerDraw = Math::Min(m_maxInstancesPerDraw, m_instanceStreamUVs[i]->getMaxInstancePerDraw());
        }

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* RenderQueueDX11::getRenderSystemDX11Raw() const
    {
        return m_renderTarget->getRenderSystem()->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    void RenderQueueDX11::draw()
    {
        MAZE_PROFILE_EVENT("RenderQueueDX11::draw");

        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();
        StateMachineDX11* stateMachine = renderSystem->getStateMachine();

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

                        renderSystem->clearCurrentRenderTarget(
                            command->colorBuffer,
                            command->depthBuffer,
                            command->stencilBuffer);

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

                        if (stateMachine->getCurrentShader())
                            stateMachine->getCurrentShader()->bindTextures();
                        break;
                    }
                    case RenderCommandType::DrawVAOInstanced:
                    {
                        MAZE_PROFILE_EVENT("DrawVAOInstanced");

                        RenderCommandDrawVAOInstanced* command = static_cast<RenderCommandDrawVAOInstanced*>(_command);

                        do
                        {
                            ShaderDX11* shader = stateMachine->getCurrentShader();
                            if (!shader || !shader->isValid())
                                break;

                            m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixDX11>()->prepareForRender(command->count);

                            if (command->useColorStream)
                                m_instanceStreamColor->castRaw<InstanceStreamColorDX11>()->prepareForRender(command->count);

                            for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                                if (command->uvMask & (1 << i))
                                    m_instanceStreamUVs[i]->castRaw<InstanceStreamUVDX11>()->prepareForRender(command->count);

                            VertexArrayObjectDX11* vao = command->vao->castRaw<VertexArrayObjectDX11>();

                            S32 indicesCount = (S32)vao->getIndicesCount();
                            if (indicesCount > 0)
                            {
#if (MAZE_DEBUG)
                                S32 renderSystemDrawCallsLimit = m_renderTarget->getRenderSystem()->getDrawCallsLimit();
                                if (   ((m_drawCallsLimit == 0) || (m_drawCalls < m_drawCallsLimit))
                                    && ((renderSystemDrawCallsLimit == 0) || (m_renderTarget->getRenderSystem()->getDrawCalls() < renderSystemDrawCallsLimit)))
                                {
#endif
                                    vao->bind(shader);

                                    stateMachine->flushPipeline();
                                    shader->flushConstantBuffers();

                                    renderSystem->getDeviceContext()->DrawIndexedInstanced(
                                        (UINT)indicesCount,
                                        (UINT)command->count,
                                        0,
                                        0,
                                        0);
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                            if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
                        if (ShaderDX11* shader = stateMachine->getCurrentShader())
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
    void RenderQueueDX11::clear()
    {
        RenderQueue::clear();

        m_instanceStreamModelMatrix->setOffset(0);
        m_instanceStreamColor->setOffset(0);

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->setOffset(0);
    }

    //////////////////////////////////////////
    void RenderQueueDX11::bindRenderPass(RenderPass* _renderPass, bool _bindTextures)
    {
        StateMachineDX11* stateMachine = getRenderSystemDX11Raw()->getStateMachine();

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
        ShaderDX11* shader = _renderPass->getShader()->castRaw<ShaderDX11>();
        stateMachine->setCurrentShader(shader);

        // Shader uniforms
        _renderPass->applyRenderPassUniforms();

        // Bind textures
        if (_bindTextures)
            shader->bindTextures();


        // View matrix
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
            // SV_ClipDistance is always active - a zero plane disables the clipping
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
    void RenderQueueDX11::processDrawBegin()
    {
        StateMachineDX11* stateMachine = getRenderSystemDX11Raw()->getStateMachine();

        stateMachine->setViewportRect(
            Rect2S(
                (S32)Math::Round(m_renderTarget->getRenderTargetWidth() * m_renderTarget->getViewport().position.x),
                (S32)Math::Round(m_renderTarget->getRenderTargetHeight() * m_renderTarget->getViewport().position.y),
                (S32)Math::Round(m_renderTarget->getRenderTargetWidth() * m_renderTarget->getViewport().size.x),
                (S32)Math::Round(m_renderTarget->getRenderTargetHeight() * m_renderTarget->getViewport().size.y)));

        m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixDX11>()->processDrawBegin();
        m_instanceStreamColor->castRaw<InstanceStreamColorDX11>()->processDrawBegin();

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->castRaw<InstanceStreamUVDX11>()->processDrawBegin();
    }


} // namespace Maze
//////////////////////////////////////////
