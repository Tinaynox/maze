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
#include "maze-render-system-opengl-core/MazeGPUByteBufferGatewayOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectMappingControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"
#include "maze-graphics/MazeGPUByteBuffer.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeProfiler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GPUByteBufferGatewayOpenGL
    //
    //////////////////////////////////////////
    GPUByteBufferGatewayOpenGL::GPUByteBufferGatewayOpenGL(
        VertexBufferObjectOpenGLPtr const& _vbo)
        : m_vbo(_vbo)
        , m_mappedPtr(nullptr)
        , m_unmapTicket(0)
    {
    }

    //////////////////////////////////////////
    GPUByteBufferGatewayOpenGL::~GPUByteBufferGatewayOpenGL()
    {
    }

    //////////////////////////////////////////
    void* MAZE_RESTRICT_ALIAS_RETURN GPUByteBufferGatewayOpenGL::map(
        Size _elementStart,
        Size _elementsCount)
    {
        Size offset = getBlockOffsetElements() + _elementStart;
        Size length = _elementsCount;

        ContextOpenGL* context = m_vbo->getContextOpenGL();

        bool isPersistentMappingEnabled = context->getExtensions()->getSupportArbBufferStorage();

        if (getAccessType() >= GPUByteBufferAccessType::DynamicPersistent && isPersistentMappingEnabled)
        {
            // Persistent buffers map the *whole* assigned buffer,
            // we later care for the offsets and lengths
            offset = m_buffer->getBlockOffsetElements();
            length = m_buffer->getElementsCount();
        }

        MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_COPY_WRITE_BUFFER, m_vbo->getGLVBO()));

        m_mappedPtr = m_vbo->getMappingController()->map(
            offset * getBytesPerElement(),
            length * getBytesPerElement(),
            m_unmapTicket);

        m_lastMappingStart = _elementStart;
        m_lastMappingCount = _elementsCount;

        U8* result = (U8*)m_mappedPtr;

        /*
        if (getAccessType() >= GPUByteBufferAccessType::DynamicPersistent && isPersistentMappingEnabled)
        {
            Size lastMappingStart = _elementStart + m_buffer->getElementsCount();
            
            m_lastMappingStart = lastMappingStart;

            result += lastMappingStart * bytesPerElement;
        }
        */

        return result;
    }

    ////////////////////////////////////
    void GPUByteBufferGatewayOpenGL::unmap(
        GPUByteBufferUnmapOptions _unmapOption,
        Size _flushElementStart,
        Size _flushElementsCount)
    {
        MAZE_ERROR_IF(_flushElementStart > m_lastMappingCount,
            "Flush starts after the end of the mapped region!");

        MAZE_ERROR_IF(_flushElementStart + _flushElementsCount > m_lastMappingCount,
            "Flush region out of bounds!");


        ContextOpenGL* context = m_vbo->getContextOpenGL();

        bool isPersistentMappingEnabled = context->getExtensions()->getSupportArbBufferStorage();

        if (    getAccessType() <= GPUByteBufferAccessType::DynamicPersistent
            ||    _unmapOption == GPUByteBufferUnmapOptions::UnmapAll
            ||    !isPersistentMappingEnabled)
        {
            if (!_flushElementsCount)
                _flushElementsCount = m_lastMappingCount - _flushElementStart;

            MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_COPY_WRITE_BUFFER, m_vbo->getGLVBO()));

            m_vbo->getMappingController()->flush(
                m_unmapTicket,
                _flushElementStart * getBytesPerElement(),
                _flushElementsCount * getBytesPerElement());

            if (    _unmapOption == GPUByteBufferUnmapOptions::UnmapAll
                ||    !isPersistentMappingEnabled
                ||    getAccessType() == GPUByteBufferAccessType::DynamicDefault)
            {
                m_vbo->getMappingController()->unmap(m_unmapTicket);
                m_mappedPtr = 0;
            }
        }

        // ???
        // m_lastMappingStart = 0;
        // m_lastMappingCount = 0;
    }

    ////////////////////////////////////
    void GPUByteBufferGatewayOpenGL::upload(
        void const* _data,
        Size _elementStart,
        Size _elementsCount)
    {
        if (getAccessType() >= GPUByteBufferAccessType::Default)
        {
            Size offset = getBlockOffsetElements() + _elementStart;
            Size length = _elementsCount;

            MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_COPY_WRITE_BUFFER, m_vbo->getGLVBO()));
            m_vbo->getMappingController()->upload(
                _data,
                offset * getBytesPerElement(),
                length * getBytesPerElement());
            /*
            void* dstData = map(elementStart, elementsCount);
            memcpy(dstData, data, elementsCount * getBytesPerElement());
            unmap(GPUByteBufferUnmapOptions::UnmapAll);
            */
        }
        else
        {
            MAZE_TODO;
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
