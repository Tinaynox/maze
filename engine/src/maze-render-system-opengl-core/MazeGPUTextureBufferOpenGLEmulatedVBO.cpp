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
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGLEmulatedVBO.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectMappingControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUByteBufferGatewayOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeProfiler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GPUTextureBufferOpenGLEmulatedVBO
    //
    //////////////////////////////////////////
    GPUTextureBufferOpenGLEmulatedVBO::GPUTextureBufferOpenGLEmulatedVBO(
        Vec2DU const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferGateway* _gpuByteBufferGateway,
        Size _blockOffsetBytes,
        Size _elementsCount,
        Size _bytesPerElement,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
        : GPUTextureBufferOpenGL(
            _size,
            _pixelFormat,
            _gpuByteBufferGateway,
            _blockOffsetBytes,
            _elementsCount,
            _bytesPerElement,
            _accessType,
            _initialData)
    {

    }

    //////////////////////////////////////////
    GPUTextureBufferOpenGLEmulatedVBO::~GPUTextureBufferOpenGLEmulatedVBO()
    {

    }

    //////////////////////////////////////////
    void* MAZE_RESTRICT_ALIAS_RETURN GPUTextureBufferOpenGLEmulatedVBO::map(
        Size _elementStart,
        Size _elementsCount)
    {
        return GPUTextureBufferOpenGL::map(_elementStart, _elementsCount);
    }

    //////////////////////////////////////////
    void GPUTextureBufferOpenGLEmulatedVBO::unmap(
        GPUByteBufferUnmapOptions _unmapOption,
        Size _flushElementStart,
        Size _flushElementsCount)
    {
        GPUTextureBufferOpenGL::unmap(_unmapOption, _flushElementStart, _flushElementsCount);

        if (!_flushElementsCount)
            _flushElementsCount = m_gpuByteBufferGateway->getLastMappingCount() - _flushElementStart;

        Size elementStart = m_gpuByteBufferGateway->getLastMappingStart() + _flushElementStart;
        Size elementsCount = _flushElementsCount;

        flushData(elementStart, elementsCount);
    }

    //////////////////////////////////////////
    void GPUTextureBufferOpenGLEmulatedVBO::upload(
        void const* _data,
        Size _elementStart,
        Size _elementsCount)
    {
        GPUTextureBufferOpenGL::upload(_data, _elementStart, _elementsCount);

        flushData(_elementStart, _elementsCount);
    }

    //////////////////////////////////////////
    void GPUTextureBufferOpenGLEmulatedVBO::flushData(
        Size _elementStart,
        Size _elementsCount)
    {
        U32 row = (U32)_elementStart / m_size.x;
        U32 column = (U32)_elementStart % m_size.x;
        if (column + (U32)_elementsCount > m_size.x)
        {
            // Flush several rows
            U32 imageWidth = (U32)Math::Min((Size)m_size.x, _elementStart + _elementsCount);
            U32 imageHeight = (U32)Math::Ceil(F32(column + _elementsCount) / m_size.x);
            flushTextureData(_elementStart - (Size)column, (Size)imageHeight * m_size.x, imageWidth, imageHeight, 0, row);
        }
        else
        {
            // Flush one block in row
            flushTextureData((U32)_elementStart, (U32)_elementsCount, (U32)_elementsCount, 1, column, row);
        }
    }

    //////////////////////////////////////////
    void GPUTextureBufferOpenGLEmulatedVBO::flushTextureData(
        Size _elementStart,
        Size _elementsCount,
        U32 _width, U32 _height,
        U32 _x, U32 _y)
    {
        RenderSystemPtr const& rs = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        if (!rs)
            return;

        ContextOpenGL* context = rs->castRaw<RenderSystemOpenGL>()->ensureCurrentContext();

        MAZE_ERROR_IF(_elementStart >= getElementsCount(), "Invalid elementStart!");
        MAZE_ERROR_IF(_elementStart + _elementsCount > getElementsCount(), "Invalid elementStart!");

        Size offset = _elementStart * getBytesPerElement();

        GPUByteBufferGatewayOpenGL* gateway = static_cast<GPUByteBufferGatewayOpenGL*>(getGPUByteBufferGateway());

        ContextOpenGLScopeBind contextScopedBind(context);
        MAZE_GL_MUTEX_SCOPED_LOCK(context->getRenderSystemRaw());

        if ((getBytesPerElement() & 4) != 4)
        {
            MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_UNPACK_ALIGNMENT, 1));
        }

        MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_PIXEL_UNPACK_BUFFER, gateway->getVBO()->getGLVBO()));
        

        Size blockOffsetBytes = getBlockOffsetBytes();
        U8 const* dataPointer = reinterpret_cast<U8 const*>(blockOffsetBytes + offset);
        m_texture->copyImageFrom(
            dataPointer,
            m_pixelFormat,
            _width, _height,
            _x, _y);

        MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_PIXEL_UNPACK_BUFFER, 0));

        if ((getBytesPerElement() & 4) != 4)
        {
            MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_UNPACK_ALIGNMENT, 4));
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
