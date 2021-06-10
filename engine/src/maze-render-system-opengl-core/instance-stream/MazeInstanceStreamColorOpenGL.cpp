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
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamColorOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-core/math//MazeMathAlgebra.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static CString const c_colorsStreamUniformName = "u_colorsStream";
    static CString const c_colorsStreamTextureUniformName = "u_colorsStreamTexture";
    static CString const c_colorsStreamTextureSizeUniformName = "u_colorsStreamTextureSize";
    static CString const c_colorsStreamOffsetUniformName = "u_colorsStreamOffset";


    //////////////////////////////////////////
    InstanceStreamColorOpenGL::InstanceStreamColorOpenGL()
    {

    }

    //////////////////////////////////////////
    InstanceStreamColorOpenGL::~InstanceStreamColorOpenGL()
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
    InstanceStreamColorOpenGLPtr InstanceStreamColorOpenGL::Create(
        RenderSystemOpenGL* _rs,
        ContextOpenGL* _contextOpenGL,
        InstanceStreamModeOpenGL _mode)
    {
        InstanceStreamColorOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InstanceStreamColorOpenGL, object, init(_rs, _contextOpenGL, _mode));
        return object;
    }


    //////////////////////////////////////////
    bool InstanceStreamColorOpenGL::init(
        RenderSystemOpenGL* _rs,
        ContextOpenGL* _contextOpenGL,
        InstanceStreamModeOpenGL _mode)
    {
        if (!InstanceStreamColor::init())
            return false;

        m_context = _contextOpenGL;
        m_mode = _mode;

        switch (_mode)
        {
            case InstanceStreamModeOpenGL::UniformArray:
            {
                m_maxInstancesPerDrawCall = 128;
                break;
            }
            case InstanceStreamModeOpenGL::UniformTexture:
            {
                m_maxInstancesPerDrawCall = 16384;

                // Vec4DF per instance
                U32 textureBufferSize = (U32)Math::Ceil(sqrt(m_maxInstancesPerDrawCall));
                textureBufferSize = Math::GetNextPowerOfTwo(textureBufferSize);

                m_bufferInfo.buffer = _rs->createGPUTextureBuffer(
                    _contextOpenGL,
                    Vec2DU(textureBufferSize),
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

        m_colorsStreamUniformHash = _rs->getShaderSystem()->stringToHash(c_colorsStreamUniformName);
        m_colorsStreamTextureUniformHash = _rs->getShaderSystem()->stringToHash(c_colorsStreamTextureUniformName);
        m_colorsStreamTextureSizeUniformHash = _rs->getShaderSystem()->stringToHash(c_colorsStreamTextureSizeUniformName);
        m_colorsStreamOffsetUniformHash = _rs->getShaderSystem()->stringToHash(c_colorsStreamOffsetUniformName);

        m_maxInstancesPerDraw = 16384;
        m_data.resize(m_maxInstancesPerDraw);

        return true;
    }
    
    //////////////////////////////////////////
    void InstanceStreamColorOpenGL::prepareForRender(S32 _instancesCount)
    {
        switch (m_mode)
        {
            case InstanceStreamModeOpenGL::UniformArray:
            {
                ShaderUniformPtr const& colorsStreamUniform = m_context->getCurrentShader()->ensureUniform(c_colorsStreamUniformName, m_colorsStreamUniformHash);

                if (colorsStreamUniform)
                    colorsStreamUniform->set(&m_data[m_dataOffset], _instancesCount);

                break;
            }
            case InstanceStreamModeOpenGL::UniformTexture:
            {
                ShaderUniformPtr const& colorsStreamTextureUniform = m_context->getCurrentShader()->ensureUniform(
                    c_colorsStreamTextureUniformName,
                    m_colorsStreamTextureUniformHash);

                if (colorsStreamTextureUniform)
                {
                    ShaderUniformPtr const& colorsStreamTextureSizeUniform = m_context->getCurrentShader()->ensureUniform(
                        c_colorsStreamTextureSizeUniformName,
                        m_colorsStreamTextureSizeUniformHash);

                    ShaderUniformPtr const& colorsStreamOffsetUniform = m_context->getCurrentShader()->ensureUniform(
                        c_colorsStreamOffsetUniformName,
                        m_colorsStreamOffsetUniformHash);

                    Texture2DPtr const& bufferTexture = m_bufferInfo.buffer->getTexture();

                    colorsStreamTextureUniform->set(bufferTexture);
                    colorsStreamTextureSizeUniform->set(bufferTexture->getWidth());
                    colorsStreamOffsetUniform->set(m_dataOffset);
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
    void InstanceStreamColorOpenGL::bindRenderPass()
    {
        
    }

    //////////////////////////////////////////
    void InstanceStreamColorOpenGL::processDrawBegin()
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
                    m_dataOffset * sizeof(Vec4DF));

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
