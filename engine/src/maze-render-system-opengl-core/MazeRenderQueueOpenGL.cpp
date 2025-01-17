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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderDrawTopologyOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class RenderQueueOpenGL
    //
    //////////////////////////////////////////
    RenderQueueOpenGL::RenderQueueOpenGL()
        : m_context(nullptr)
        , m_drawTime(0.0f)
    {
    }

    //////////////////////////////////////////
    RenderQueueOpenGL::~RenderQueueOpenGL()
    {
        
    }

    //////////////////////////////////////////
    RenderQueueOpenGLPtr RenderQueueOpenGL::Create(
        RenderTarget* _renderTarget,
        ContextOpenGL* _contextOpenGL)
    {
        RenderQueueOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderQueueOpenGL, object, init(_renderTarget, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    bool RenderQueueOpenGL::init(
        RenderTarget* _renderTarget,
        ContextOpenGL* _contextOpenGL)
    {
        if (!RenderQueue::init(_renderTarget))
            return false;

        m_context = _contextOpenGL;
        if (!m_context)
            return false;

        Vector<InstanceStreamPtr> instanceStreams;

        RenderSystemOpenGL* rsOpenGL = _renderTarget->getRenderSystem()->castRaw<RenderSystemOpenGL>();

        if (m_context->getModelMatricesArchitecture() == ModelMatricesArchitectureOpenGL::UniformTexture)
        {
            m_instanceStreamModelMatrix = InstanceStreamModelMatrixOpenGL::Create(rsOpenGL, _contextOpenGL, InstanceStreamModeOpenGL::UniformTexture);
            m_instanceStreamColor = InstanceStreamColorOpenGL::Create(rsOpenGL, _contextOpenGL, InstanceStreamModeOpenGL::UniformTexture);

            instanceStreams.push_back(m_instanceStreamModelMatrix);
            instanceStreams.push_back(m_instanceStreamColor);

            for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            {
                m_instanceStreamUVs[i] = InstanceStreamUVOpenGL::Create(i, rsOpenGL, _contextOpenGL, InstanceStreamModeOpenGL::UniformTexture);
                instanceStreams.push_back(m_instanceStreamUVs[i]);
            }
        }
        else
        if (m_context->getModelMatricesArchitecture() == ModelMatricesArchitectureOpenGL::UniformArray)
        {
            m_instanceStreamModelMatrix = InstanceStreamModelMatrixOpenGL::Create(rsOpenGL, _contextOpenGL, InstanceStreamModeOpenGL::UniformArray);
            m_instanceStreamColor = InstanceStreamColorOpenGL::Create(rsOpenGL, _contextOpenGL, InstanceStreamModeOpenGL::UniformArray);

            instanceStreams.push_back(m_instanceStreamModelMatrix);
            instanceStreams.push_back(m_instanceStreamColor);

            for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            {
                m_instanceStreamUVs[i] = InstanceStreamUVOpenGL::Create(i, rsOpenGL, _contextOpenGL, InstanceStreamModeOpenGL::UniformArray);
                instanceStreams.push_back(m_instanceStreamUVs[i]);
            }
        }

        m_maxInstancesPerDrawCall = m_instanceStreamModelMatrix->getMaxInstancesPerDrawCall();
        m_maxInstancesPerDraw = m_instanceStreamModelMatrix->getMaxInstancePerDraw();

        for (InstanceStreamPtr const& instanceStream : instanceStreams)
        {
            m_maxInstancesPerDrawCall = Math::Min(m_maxInstancesPerDrawCall, instanceStream->getMaxInstancesPerDrawCall());
            m_maxInstancesPerDraw = Math::Min(m_maxInstancesPerDraw, instanceStream->getMaxInstancePerDraw());
        }

        return true;
    }

    //////////////////////////////////////////
    void RenderQueueOpenGL::draw()
    {
        MAZE_PROFILE_EVENT("RenderQueueOpenGL::draw");

        Maze::ContextOpenGLScopeBind contextOpenGLScopedLock(m_context);

#if 0
        m_drawTime = (F32)m_timer.getMilliseconds() / 1000.0f;
#else
        m_drawTime = UpdateManager::GetInstancePtr()->getAppTime();
#endif

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
                        RenderCommandClearCurrentRenderTarget* command = static_cast<RenderCommandClearCurrentRenderTarget*>(_command);

                        m_context->getRenderSystem()->clearCurrentRenderTarget(
                            command->colorBuffer,
                            command->depthBuffer);

                        break;
                    }
                    case RenderCommandType::SetRenderPass:
                    {
                        RenderCommandSetRenderPass* command = static_cast<RenderCommandSetRenderPass*>(_command);

                        bindRenderPass(command->renderPass);
                        break;
                    }
                    case RenderCommandType::DrawVAOInstanced:
                    {
                        RenderCommandDrawVAOInstanced* command = static_cast<RenderCommandDrawVAOInstanced*>(_command);

                        do
                        {
                            m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixOpenGL>()->prepareForRender(command->count);

                            if (command->useColorStream)
                                m_instanceStreamColor->castRaw<InstanceStreamColorOpenGL>()->prepareForRender(command->count);

                            for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
                                if (command->uvMask & (1 << i))
                                    m_instanceStreamUVs[i]->castRaw<InstanceStreamUVOpenGL>()->prepareForRender(command->count);
                        
                            ShaderOpenGL* shaderOpenGL = static_cast<ShaderOpenGL*>(m_context->getCurrentShader());
                            shaderOpenGL->bindTextures();

                            VertexArrayObjectOpenGL* vao = command->vao->castRaw<VertexArrayObjectOpenGL>();

                            MZGLsizei indicesCount = (MZGLsizei)vao->getIndicesCount();
                            if (indicesCount > 0)
                            {
#if (MAZE_DEBUG)
                                S32 renderSystemDrawCallsLimit = m_renderTarget->getRenderSystem()->getDrawCallsLimit();
                                if (   ((m_drawCallsLimit == 0) || (m_drawCalls < m_drawCallsLimit))
                                    && ((renderSystemDrawCallsLimit == 0) || (m_renderTarget->getRenderSystem()->getDrawCalls() < renderSystemDrawCallsLimit)))
                                {
#endif
                                    MZGLenum indicesType = GetVertexAttributeTypeOpenGL(vao->getIndicesType());
                                    MZGLenum drawTopology = GetRenderDrawTopologyOpenGL(vao->getRenderDrawTopology());

                                    bindVertexArrayObject(vao);

                                    MAZE_GL_CALL(mzglDrawElementsInstanced(
                                        drawTopology,
                                        indicesCount,
                                        indicesType,
                                        0,
                                        command->count));
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
                        m_context->setScissorRect(scissorRect);

                        m_context->setScissorTestEnabled(true);
                        
                        break;
                    }
                    case RenderCommandType::PopScissorRect:
                    {
                        m_scissorRects.pop();

                        if (m_scissorRects.empty())
                            m_context->setScissorTestEnabled(false);
                        else
                            m_context->setScissorRect(m_scissorRects.top());

                        break;
                    }
                    case RenderCommandType::EnableClipPlane:
                    {
                        RenderCommandEnableClipPlane* command = static_cast<RenderCommandEnableClipPlane*>(_command);

                        m_context->setClipDistanceEnabled(command->index, true);
                        m_clipPlanes[command->index] = command->plane;

                        break;
                    }
                    case RenderCommandType::DisableClipPlane:
                    {
                        RenderCommandEnableClipPlane* command = static_cast<RenderCommandEnableClipPlane*>(_command);

                        m_context->setClipDistanceEnabled(command->index, false);

                        break;
                    }
                    case RenderCommandType::UploadShaderUniformVec2F:
                    {
                        RenderCommandUploadShaderUniformVec2F* command = static_cast<RenderCommandUploadShaderUniformVec2F*>(_command);
                        ShaderOpenGL* shaderOpenGL = m_context->getCurrentShader()->castRaw<ShaderOpenGL>();
                        ShaderUniformPtr const& uniform = shaderOpenGL->ensureUniform(command->name);
                        MAZE_DEBUG_ASSERT(uniform);
                        uniform->upload(command->pointer, (Size)command->count);
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformVec3F:
                    {
                        RenderCommandUploadShaderUniformVec3F* command = static_cast<RenderCommandUploadShaderUniformVec3F*>(_command);
                        ShaderOpenGL* shaderOpenGL = m_context->getCurrentShader()->castRaw<ShaderOpenGL>();
                        ShaderUniformPtr const& uniform = shaderOpenGL->ensureUniform(command->name);
                        MAZE_DEBUG_ASSERT(uniform);
                        uniform->upload(command->pointer, (Size)command->count);
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformVec4F:
                    {
                        RenderCommandUploadShaderUniformVec4F* command = static_cast<RenderCommandUploadShaderUniformVec4F*>(_command);
                        ShaderOpenGL* shaderOpenGL = m_context->getCurrentShader()->castRaw<ShaderOpenGL>();
                        ShaderUniformPtr const& uniform = shaderOpenGL->ensureUniform(command->name);
                        MAZE_DEBUG_ASSERT(uniform);
                        uniform->upload(command->pointer, (Size)command->count);
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformMat3F:
                    {
                        RenderCommandUploadShaderUniformMat3F* command = static_cast<RenderCommandUploadShaderUniformMat3F*>(_command);
                        ShaderOpenGL* shaderOpenGL = m_context->getCurrentShader()->castRaw<ShaderOpenGL>();
                        ShaderUniformPtr const& uniform = shaderOpenGL->ensureUniform(command->name);
                        MAZE_DEBUG_ASSERT(uniform);
                        uniform->upload(command->pointer, (Size)command->count);
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformMat4F:
                    {
                        RenderCommandUploadShaderUniformMat4F* command = static_cast<RenderCommandUploadShaderUniformMat4F*>(_command);
                        ShaderOpenGL* shaderOpenGL = m_context->getCurrentShader()->castRaw<ShaderOpenGL>();
                        ShaderUniformPtr const& uniform = shaderOpenGL->ensureUniform(command->name);
                        MAZE_DEBUG_ASSERT(uniform);
                        uniform->upload(command->pointer, (Size)command->count);
                        break;
                    }
                    case RenderCommandType::UploadShaderUniformTMat:
                    {
                        RenderCommandUploadShaderUniformTMat* command = static_cast<RenderCommandUploadShaderUniformTMat*>(_command);
                        ShaderOpenGL* shaderOpenGL = m_context->getCurrentShader()->castRaw<ShaderOpenGL>();
                        ShaderUniformPtr const& uniform = shaderOpenGL->ensureUniform(command->name);
                        MAZE_DEBUG_ASSERT(uniform);
                        uniform->upload(command->pointer, (Size)command->count);
                        break;
                    }
                    default:
                    {
                        Debug::LogError("Unsupported RenderCommand: %d", (S32)_command->type);
                        break;
                    }
                }
            });

        m_context->getStateMachine()->bindVertexArrayObject(0);
        clear();
    }

    //////////////////////////////////////////
    void RenderQueueOpenGL::clear()
    {
        RenderQueue::clear();

        m_instanceStreamModelMatrix->setOffset(0);
        m_instanceStreamColor->setOffset(0);

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->setOffset(0);
    }

    //////////////////////////////////////////
    void RenderQueueOpenGL::bindRenderPass(RenderPass* _renderPass)
    {
        MAZE_DEBUG_ERROR_RETURN_IF(!m_context->_validateIsCurrentGLContext(), "Context is not current!");

        // Blend Mode
        BlendFactor srcFactor = _renderPass->getBlendSrcFactor();
        BlendFactor destFactor = _renderPass->getBlendDestFactor();

        bool blendEnabled = !(srcFactor == BlendFactor::One && destFactor == BlendFactor::Zero);
        m_context->setBlendEnabled(blendEnabled);
        if (blendEnabled)
        {
            m_context->setBlendFactors(
                srcFactor,
                destFactor);
        }

        // Depth Test
        CompareFunction depthTestCompareFunction = _renderPass->getDepthTestCompareFunction();
        bool depthTestEnabled = (depthTestCompareFunction != CompareFunction::Disabled);
        m_context->setDepthTestEnabled(depthTestEnabled);
        if (depthTestEnabled)
        {
            m_context->setDepthTestCompareFunction(depthTestCompareFunction);
        }

        // Depth Write
        m_context->setDepthWriteEnabled(_renderPass->getDepthWriteEnabled());

        // Cull Mode
        CullMode cullMode = _renderPass->getCullMode();
        bool cullingEnabled = (cullMode != CullMode::Off);
        m_context->setCullEnabled(cullingEnabled);
        if (cullingEnabled)
        {
            m_context->setCullMode(cullMode);
        }


        // Shader
        ShaderOpenGL* shaderOpenGL = _renderPass->getShader()->castRaw<ShaderOpenGL>();
        m_context->setCurrentShader(shaderOpenGL);

        // Shader uniforms
        _renderPass->applyRenderPassUniforms();


        // Stream data
        m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixOpenGL>()->bindRenderPass();
        m_instanceStreamColor->castRaw<InstanceStreamColorOpenGL>()->bindRenderPass();

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->castRaw<InstanceStreamUVOpenGL>()->bindRenderPass();
        

        // View matrix
        if (shaderOpenGL->getViewMatrixUniform())
        {
            shaderOpenGL->getViewMatrixUniform()->set(m_renderTarget->getViewMatrix());
        }

        // Clip distances
        bool clipDistance0 = m_context->getClipDistanceEnabled(0);

        if (shaderOpenGL->getClipDistanceEnableUniform())
            shaderOpenGL->getClipDistanceEnableUniform()->set(Vec4B{ clipDistance0, false, false, false });

        if (clipDistance0)
        {
            if (shaderOpenGL->getClipDistance0Uniform())
                shaderOpenGL->getClipDistance0Uniform()->set(m_clipPlanes[0]);
        }

        // Projection matrix
        shaderOpenGL->getProjectionMatrixUniform()->set(m_renderTarget->getProjectionMatrix());

        // Projection params
        if (shaderOpenGL->getProjectionParamsUniform())
        {
            shaderOpenGL->getProjectionParamsUniform()->set(
                Vec4F{ m_renderTarget->getNear(), m_renderTarget->getFar(), 0.0f, 0.0f });
        }

        // View direction
        if (shaderOpenGL->getViewPositionUniform())
        {
            shaderOpenGL->getViewPositionUniform()->set(m_renderTarget->getViewPosition());
        }

        // Time
        if (shaderOpenGL->getTimeUniform())
        {
            shaderOpenGL->getTimeUniform()->set(m_drawTime);
        }

    }

    //////////////////////////////////////////
    void RenderQueueOpenGL::bindVertexArrayObject(VertexArrayObject* _vao)
    {
        MAZE_DEBUG_ERROR_RETURN_IF(!m_context->_validateIsCurrentGLContext(), "Context is not current!");

        _vao->castRaw<VertexArrayObjectOpenGL>()->bind();
    }

    //////////////////////////////////////////
    void RenderQueueOpenGL::processDrawBegin()
    {
        m_context->setViewportRect(
            Rect2S(
                (S32)Math::Round(m_renderTarget->getRenderTargetWidth() * m_renderTarget->getViewport().position.x),
                (S32)Math::Round(m_renderTarget->getRenderTargetHeight() * m_renderTarget->getViewport().position.y),
                (S32)Math::Round(m_renderTarget->getRenderTargetWidth() * m_renderTarget->getViewport().size.x),
                (S32)Math::Round(m_renderTarget->getRenderTargetHeight() * m_renderTarget->getViewport().size.y)));

        m_instanceStreamModelMatrix->castRaw<InstanceStreamModelMatrixOpenGL>()->processDrawBegin();
        m_instanceStreamColor->castRaw<InstanceStreamColorOpenGL>()->processDrawBegin();

        for (S32 i = 0; i < MAZE_UV_CHANNELS_MAX; ++i)
            m_instanceStreamUVs[i]->castRaw<InstanceStreamUVOpenGL>()->processDrawBegin();
        
    }

    
} // namespace Maze
//////////////////////////////////////////
