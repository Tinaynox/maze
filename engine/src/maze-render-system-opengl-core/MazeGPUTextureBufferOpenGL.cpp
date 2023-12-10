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
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectMappingControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GPUTextureBufferOpenGL
    //
    //////////////////////////////////////////
    GPUTextureBufferOpenGL::GPUTextureBufferOpenGL(
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferGateway* _gpuByteBufferGateway,
        Size _blockOffsetBytes,
        Size _elementsCount,
        Size _bytesPerElement,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
        : GPUTextureBuffer(
            _size,
            _pixelFormat,
            _gpuByteBufferGateway,
            _blockOffsetBytes,
            _elementsCount,
            _bytesPerElement,
            _accessType,
            _initialData)
    {
        Texture2DOpenGLPtr texture = Texture2D::Create()->cast<Texture2DOpenGL>();

        PixelSheet2D pixelSheet2D(_size, _pixelFormat);
        F32 value[] = { 128.0, 0.0f, 128.0f, 128.0f };
        pixelSheet2D.fill(value, 4);
        texture->loadTexture(pixelSheet2D);

        m_texture = texture;

        {
            ContextOpenGLScopeBind contextScopedBind(texture->getContextOpenGL());
            MAZE_GL_MUTEX_SCOPED_LOCK(texture->getContextOpenGL()->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(texture);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_BASE_LEVEL, 0));
            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_MAX_LEVEL, 0));
        }

        m_texture->setMagFilter(TextureFilter::Nearest);
        m_texture->setMinFilter(TextureFilter::Nearest);
    }

    //////////////////////////////////////////
    GPUTextureBufferOpenGL::~GPUTextureBufferOpenGL()
    {

    }
    
} // namespace Maze
//////////////////////////////////////////
