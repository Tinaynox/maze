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
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamUVOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-core/math//MazeMathAlgebra.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static HashedCString const c_uvStreamUniformName[MAZE_UV_CHANNELS_MAX] =
    {
        MAZE_HASHED_CSTRING("u_uv0Stream"),
        MAZE_HASHED_CSTRING("u_uv1Stream"),
        MAZE_HASHED_CSTRING("u_uv2Stream"),
        MAZE_HASHED_CSTRING("u_uv3Stream"),
        MAZE_HASHED_CSTRING("u_uv4Stream"),
        MAZE_HASHED_CSTRING("u_uv5Stream"),
        MAZE_HASHED_CSTRING("u_uv6Stream"),
        MAZE_HASHED_CSTRING("u_uv7Stream"),
    };
    static HashedCString const c_uvStreamTextureUniformName[MAZE_UV_CHANNELS_MAX] =
    {
        MAZE_HASHED_CSTRING("u_uv0StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv1StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv2StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv3StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv4StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv5StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv6StreamTexture"),
        MAZE_HASHED_CSTRING("u_uv7StreamTexture"),
    };
    static HashedCString const c_uvStreamTextureSizeUniformName[MAZE_UV_CHANNELS_MAX] =
    {
        MAZE_HASHED_CSTRING("u_uv0StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv1StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv2StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv3StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv4StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv5StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv6StreamTextureSize"),
        MAZE_HASHED_CSTRING("u_uv7StreamTextureSize"),
    };
    static HashedCString const c_uvStreamOffsetUniformName[MAZE_UV_CHANNELS_MAX] =
    {
        MAZE_HASHED_CSTRING("u_uv0StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv1StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv2StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv3StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv4StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv5StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv6StreamOffset"),
        MAZE_HASHED_CSTRING("u_uv7StreamOffset"),
    };


    //////////////////////////////////////////
    InstanceStreamUVOpenGL::InstanceStreamUVOpenGL()
    {

    }

    //////////////////////////////////////////
    InstanceStreamUVOpenGL::~InstanceStreamUVOpenGL()
    {
        if (m_bufferInfo.mappedPointer)
        {
            m_bufferInfo.buffer->unmap(
                GPUByteBufferUnmapOptions::UnmapAll,
                0,
                m_dataOffset);
        }
    }

    //////////////////////////////////////////
    InstanceStreamUVOpenGLPtr InstanceStreamUVOpenGL::Create(
        S32 _index,
        RenderSystemOpenGL* _rs,
        ContextOpenGL* _contextOpenGL,
        InstanceStreamModeOpenGL _mode)
    {
        InstanceStreamUVOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamUVOpenGL, object, init(_index, _rs, _contextOpenGL, _mode));
        return object;
    }


    //////////////////////////////////////////
    bool InstanceStreamUVOpenGL::init(
        S32 _index,
        RenderSystemOpenGL* _rs,
        ContextOpenGL* _contextOpenGL,
        InstanceStreamModeOpenGL _mode)
    {
        if (!InstanceStreamUV::init(_index))
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

                // Vec4F per instance
                U32 textureBufferSize = (U32)Math::Ceil(sqrt(m_maxInstancesPerDrawCall));
                textureBufferSize = Math::GetNextPowerOfTwo(textureBufferSize);

                m_bufferInfo.buffer = _rs->createGPUTextureBuffer(
                    _contextOpenGL,
                    Vec2U(textureBufferSize),
                    PixelFormat::RGBA_F32,
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
    void InstanceStreamUVOpenGL::prepareForRender(S32 _instancesCount)
    {
        switch (m_mode)
        {
            case InstanceStreamModeOpenGL::UniformArray:
            {
                ShaderUniformPtr const& uvStreamUniform = m_context->getCurrentShader()->ensureUniform(c_uvStreamUniformName[m_index]);

                if (uvStreamUniform)
                    uvStreamUniform->upload(&m_data[m_dataOffset], _instancesCount);

                break;
            }
            case InstanceStreamModeOpenGL::UniformTexture:
            {
                ShaderUniformPtr const& uvStreamTextureUniform = m_context->getCurrentShader()->ensureUniform(c_uvStreamTextureUniformName[m_index]);

                if (uvStreamTextureUniform)
                {
                    ShaderUniformPtr const& uvStreamTextureSizeUniform = m_context->getCurrentShader()->ensureUniform(c_uvStreamTextureSizeUniformName[m_index]);
                    ShaderUniformPtr const& uvStreamOffsetUniform = m_context->getCurrentShader()->ensureUniform(c_uvStreamOffsetUniformName[m_index]);

                    Texture2DPtr const& bufferTexture = m_bufferInfo.buffer->getTexture();

                    uvStreamTextureUniform->set(bufferTexture);
                    uvStreamTextureSizeUniform->set(bufferTexture->getWidth());
                    uvStreamOffsetUniform->set(m_dataOffset);
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
    void InstanceStreamUVOpenGL::bindRenderPass()
    {
        
    }

    //////////////////////////////////////////
    void InstanceStreamUVOpenGL::processDrawBegin()
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
                    m_dataOffset * sizeof(Vec4F));

                m_bufferInfo.buffer->unmap(
                    GPUByteBufferUnmapOptions::KeepPersistent,
                    0,
                    m_dataOffset);

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
