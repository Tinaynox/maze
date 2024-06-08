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
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamModelMatrixOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-core/math//MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    InstanceStreamModelMatrixOpenGL::InstanceStreamModelMatrixOpenGL()
    {

    }

    //////////////////////////////////////////
    InstanceStreamModelMatrixOpenGL::~InstanceStreamModelMatrixOpenGL()
    {
        if (m_bufferInfo.mappedPointer)
        {
            m_bufferInfo.buffer->unmap(
                GPUByteBufferUnmapOptions::UnmapAll,
                0,
                m_dataOffset * 4);
        }
    }

    //////////////////////////////////////////
    InstanceStreamModelMatrixOpenGLPtr InstanceStreamModelMatrixOpenGL::Create(
        RenderSystemOpenGL* _rs,
        ContextOpenGL* _contextOpenGL,
        InstanceStreamModeOpenGL _mode)
    {
        InstanceStreamModelMatrixOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamModelMatrixOpenGL, object, init(_rs, _contextOpenGL, _mode));
        return object;
    }


    //////////////////////////////////////////
    bool InstanceStreamModelMatrixOpenGL::init(
        RenderSystemOpenGL* _rs,
        ContextOpenGL* _contextOpenGL,
        InstanceStreamModeOpenGL _mode)
    {
        if (!InstanceStreamModelMatrix::init())
            return false;

        m_context = _contextOpenGL;
        m_mode = _mode;

        switch (_mode)
        {
            case InstanceStreamModeOpenGL::UniformArray:
            {
                m_maxInstancesPerDrawCall = 32;
                break;
            }
            case InstanceStreamModeOpenGL::UniformTexture:
            {
                m_maxInstancesPerDrawCall = 16384;

                // Matrix4x3 (4 * Vec3F) per instance
                U32 textureBufferSize = (U32)Math::Ceil(sqrt(m_maxInstancesPerDrawCall * 4));
                textureBufferSize = Math::GetNextPowerOfTwo(textureBufferSize);

                m_bufferInfo.buffer = _rs->createGPUTextureBuffer(
                    _contextOpenGL,
                    Vec2U(textureBufferSize),
                    PixelFormat::RGB_F32,
                    GPUByteBufferAccessType::DynamicPersistent,
                    nullptr);

                break;
            }
            default:
            {
                break;
            }
        }

        m_maxInstancesPerDraw = 16384;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }
    
    //////////////////////////////////////////
    void InstanceStreamModelMatrixOpenGL::prepareForRender(S32 _instancesCount)
    {
        switch (m_mode)
        {
            case InstanceStreamModeOpenGL::UniformArray:
            {
                if (m_context->getCurrentShader()->getModelMatricesUniform())
                {
                    m_context->getCurrentShader()->getModelMatricesUniform()->upload(&m_data[m_dataOffset], _instancesCount);
                }

                break;
            }
            case InstanceStreamModeOpenGL::UniformTexture:
            {
                if (m_context->getCurrentShader()->getModelMatricesTextureUniform())
                {
                    Texture2DPtr const& bufferTexture = m_bufferInfo.buffer->getTexture();

                    m_context->getCurrentShader()->getModelMatricesTextureUniform()->set(bufferTexture);
                    m_context->getCurrentShader()->getModelMatricesTextureSizeUniform()->set(bufferTexture->getWidth());
                    m_context->getCurrentShader()->getModelMatriciesOffsetUniform()->set(m_dataOffset);
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void InstanceStreamModelMatrixOpenGL::bindRenderPass()
    {
        
    }

    //////////////////////////////////////////
    void InstanceStreamModelMatrixOpenGL::processDrawBegin()
    {
        switch (m_mode)
        {
            case InstanceStreamModeOpenGL::UniformTexture:
            {
                if (m_dataOffset == 0)
                    return;

                m_bufferInfo.mappedPointer = m_bufferInfo.buffer->map(0, m_bufferInfo.buffer->getElementsCount());

                memcpy(
                    (U8*)m_bufferInfo.mappedPointer,
                    &m_data[0],
                    m_dataOffset * sizeof(TMat));

                m_bufferInfo.buffer->unmap(
                    GPUByteBufferUnmapOptions::KeepPersistent,
                    0,
                    m_dataOffset * 4);

                m_bufferInfo.mappedPointer = nullptr;

                break;
            }
            default:
            {
                break;
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
