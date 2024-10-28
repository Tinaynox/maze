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
#include "maze-render-system-opengl-core/MazeGPUBuffersControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUByteBufferGatewayOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGLEmulated.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGLEmulatedVBO.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGLReal.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-graphics/MazeGPUByteBuffer.hpp"
#include "maze-graphics/MazeGPUVertexBuffer.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"




//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class GPUBuffersControllerOpenGL
    //
    //////////////////////////////////////////
    GPUBuffersControllerOpenGL::GPUBuffersControllerOpenGL()
        : m_renderSystem(nullptr)
        , m_defaultPoolBufferObjectsSize{0}
    {

    }

    //////////////////////////////////////////
    GPUBuffersControllerOpenGL::~GPUBuffersControllerOpenGL()
    {

    }

    //////////////////////////////////////////
    GPUBuffersControllerOpenGLPtr GPUBuffersControllerOpenGL::Create(RenderSystemOpenGL* _renderSystem)
    {
        GPUBuffersControllerOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GPUBuffersControllerOpenGL, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool GPUBuffersControllerOpenGL::init(RenderSystemOpenGL* _renderSystem)
    {
        m_renderSystem = _renderSystem;

        // Immutable - 128MB
        m_defaultPoolBufferObjectsSize[GPUByteBufferAccessType::Immutable] = 128 * 1024 * 1024;

        // Dynamic - 64MB
        for (Size i = GPUByteBufferAccessType::Default; i <= GPUByteBufferAccessType::DynamicPersistent; ++i)
            m_defaultPoolBufferObjectsSize[i] = 64 * 1024 * 1024;

        return true;
    }

    //////////////////////////////////////////
    GPUVertexBufferPtr GPUBuffersControllerOpenGL::createGPUVertexBuffer(
        ContextOpenGL* _contextOpenGL,
        VertexDataDescription const& _vertexDataDescription,
        Size _vertexCount,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        Size elementsCount = _vertexCount;
        Size bytesPerElement = _vertexDataDescription.bytesPerVertex;

        Size vboIndex;
        Size bufferOffset;

        allocateVertexBufferObjectBlock(
            _contextOpenGL,
            elementsCount * bytesPerElement,
            bytesPerElement,
            _accessType,
            vboIndex,
            bufferOffset);

        VertexBufferObjectOpenGLPtr const& vbo = m_vertexBufferObjectsPool[_accessType][_contextOpenGL][vboIndex];

        GPUByteBufferGatewayOpenGL* bufferGateway = MAZE_NEW_WITH_ARGS(GPUByteBufferGatewayOpenGL, vbo);

        GPUVertexBufferPtr result =
            MakeShared<GPUVertexBuffer>(
                _vertexDataDescription,
                bufferGateway,
                bufferOffset,
                elementsCount,
                bytesPerElement,
                _accessType,
                _initialData);

        if (_initialData)
            result->upload(_initialData, 0, elementsCount);

        return result;
    }

    //////////////////////////////////////////
    GPUTextureBufferPtr GPUBuffersControllerOpenGL::createGPUTextureBuffer(
        ContextOpenGL* _contextOpenGL,
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        Size elementsCount = (Size)_size.x * (Size)_size.y;
        Size bytesPerElement = PixelFormat::GetBytesPerPixel(_pixelFormat);

        GPUTextureBufferPtr result;

        if (0)
        {
            Size vboIndex;
            Size bufferOffset;
            allocateVertexBufferObjectBlock(
                _contextOpenGL,
                elementsCount * bytesPerElement,
                bytesPerElement,
                _accessType,
                vboIndex,
                bufferOffset);

            //VertexBufferObjectOpenGLPtr const& vbo = m_vertexBufferObjectsPool[_accessType][_contextOpenGL][vboIndex];

            // GPUByteBufferGatewayOpenGL* bufferGateway = MAZE_NEW_WITH_ARGS(GPUByteBufferGatewayOpenGL, vbo);

            MAZE_TODO;
        }
        else
        {
            // Emulated VBO Texture Buffer
            VertexBufferObjectPtr vbo = m_renderSystem->createVertexBufferObject(_contextOpenGL, _accessType, true);
            vbo->resize(elementsCount * bytesPerElement);

            GPUByteBufferGatewayOpenGL* bufferGateway = MAZE_NEW_WITH_ARGS(GPUByteBufferGatewayOpenGL, vbo->cast<VertexBufferObjectOpenGL>());

            result =
                MakeShared<GPUTextureBufferOpenGLEmulatedVBO>(
                    _size,
                    _pixelFormat,
                    bufferGateway,
                    0,
                    elementsCount,
                    bytesPerElement,
                    _accessType,
                    _initialData);

        }

        if (_initialData)
            result->upload(_initialData, 0, elementsCount);

        return result;
    }

    //////////////////////////////////////////
    void GPUBuffersControllerOpenGL::allocateVertexBufferObjectBlock(
        ContextOpenGL* _contextOpenGL,
        Size _sizeBytes,
        Size _alignment,
        GPUByteBufferAccessType::Enum _accessType,
        Size& _outVertexBufferObjectIndex,
        Size& _outBufferOffset)
    {
        // ExtensionsOpenGLPtr const& extensions = _contextOpenGL->getExtensions();

        // bool isPersistentMappingEnabled = extensions->getSupportArbBufferStorage();

        MAZE_ERROR_IF(_alignment == 0, "Alignment should be above 0!");

        Vector<VertexBufferObjectOpenGLPtr>& vertexBufferObjectsPool = m_vertexBufferObjectsPool[_accessType][_contextOpenGL];

        Vector<VertexBufferObjectOpenGLPtr>::const_iterator it = vertexBufferObjectsPool.begin();
        Vector<VertexBufferObjectOpenGLPtr>::const_iterator end = vertexBufferObjectsPool.end();

        Size bestVertexBufferObjectIndex = (Size)~0;
        Size bestBlockIndex = (Size)~0;
        bool matchingStride = false;

        while (it != end && !matchingStride)
        {
            VertexBufferObjectOpenGLPtr const& vbo = *it;
            if (vbo->findSuitableBlock(_sizeBytes, _alignment, bestBlockIndex, matchingStride))
            {
                bestVertexBufferObjectIndex = it - vertexBufferObjectsPool.begin();
            }

            ++it;
        }

        if (bestBlockIndex == (Size)~0)
        {
            bestVertexBufferObjectIndex = vertexBufferObjectsPool.size();
            bestBlockIndex = 0;
            matchingStride = true;

            Size poolSize = Math::Max(m_defaultPoolBufferObjectsSize[_accessType], _sizeBytes);

            VertexBufferObjectPtr vbo = m_renderSystem->createVertexBufferObject(_contextOpenGL, _accessType);
            vbo->resize(poolSize);

            vertexBufferObjectsPool.emplace_back(vbo->cast<VertexBufferObjectOpenGL>());
        }

        VertexBufferObjectOpenGLPtr const& bestVBO = vertexBufferObjectsPool[bestVertexBufferObjectIndex];
        VertexBufferObjectBlockOpenGL& bestBlock = bestVBO->getFreeBlock(bestBlockIndex);

        Size newOffset = ((bestBlock.offset + _alignment - 1) / _alignment) * _alignment;
        Size padding = newOffset - bestBlock.offset;

        bestBlock.size -= _sizeBytes + padding;
        bestBlock.offset = newOffset + _sizeBytes;

        if (!matchingStride)
        {
            bestVBO->addPaddingInfo(newOffset, padding);
        }

        if (bestBlock.size == 0)
            bestVBO->eraseFreeBlock(bestBlockIndex);

        _outVertexBufferObjectIndex = bestVertexBufferObjectIndex;
        _outBufferOffset = newOffset;
    }

    //////////////////////////////////////////
    void GPUBuffersControllerOpenGL::deallocateVertexBufferObjectBlock(
        ContextOpenGL* _contextOpenGL,
        Size _vertexBufferObjectIndex,
        Size _bufferOffset,
        Size _sizeBytes,
        GPUByteBufferAccessType::Enum _accessType)
    {
        Vector<VertexBufferObjectOpenGLPtr>& vertexBufferObjectsPool = m_vertexBufferObjectsPool[_accessType][_contextOpenGL];

        VertexBufferObjectOpenGLPtr const& vbo = vertexBufferObjectsPool[_vertexBufferObjectIndex];

        VertexBufferObjectBlockOpenGL::PaddingInfo paddingInfo;
        if (vbo->erasePaddingInfo(_bufferOffset, paddingInfo))
        {
            _bufferOffset -= paddingInfo.padding;
            _sizeBytes += paddingInfo.padding;
        }

        vbo->addFreeBlock(_bufferOffset, _sizeBytes);
    }
    

} // namespace Maze
//////////////////////////////////////////
