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
#include "maze-render-system-opengl-core/MazeVertexBufferObjectMappingControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeProfiler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexBufferObjectMappingControllerOpenGL
    //
    //////////////////////////////////////////
    VertexBufferObjectMappingControllerOpenGL::VertexBufferObjectMappingControllerOpenGL(
        VertexBufferObjectOpenGL* _vbo,
        bool _singleMapping)
        : m_vbo(_vbo)
        , m_mappedPtr(nullptr)
        , m_singleMapping(_singleMapping)
    {

    }

    //////////////////////////////////////////
    VertexBufferObjectMappingControllerOpenGL::~VertexBufferObjectMappingControllerOpenGL()
    {

    }

    //////////////////////////////////////////
    void* MAZE_RESTRICT_ALIAS_RETURN VertexBufferObjectMappingControllerOpenGL::map(
        Size _start,
        Size _count,
        Size& _outTicket)
    {
        MAZE_ERROR_IF(_start > m_vbo->getSizeBytes() || _start + _count > m_vbo->getSizeBytes(), "VBO out of bounds mapping!");


        if (m_mappedRanges.size() == m_freeRanges.size())
        {
            MZGLbitfield flags =
                  MAZE_GL_MAP_WRITE_BIT
                | MAZE_GL_MAP_FLUSH_EXPLICIT_BIT;

            
            bool isPersistentMappingEnabled = m_vbo->getContextOpenGL()->getExtensions()->getSupportArbBufferStorage();

            if (isPersistentMappingEnabled)
            {
                if (m_vbo->getAccessType() >= GPUByteBufferAccessType::DynamicPersistent)
                {
                    flags |= MAZE_GL_MAP_PERSISTENT_BIT;

                    if (m_vbo->getAccessType() == GPUByteBufferAccessType::DynamicPersistentCoherent)
                        flags |= MAZE_GL_MAP_COHERENT_BIT;
                }
                else
                {
                    // #TODO
                    // flags |= MAZE_GL_MAP_UNSYNCHRONIZED_BIT;
                }
            }

            if (m_singleMapping)
            {
                {
                    MAZE_GL_CALL(m_mappedPtr = mzglMapBufferRange(MAZE_GL_COPY_WRITE_BUFFER, _start, _count, flags));
                }
            }
            else
            {
                {
                    MAZE_GL_CALL(m_mappedPtr = mzglMapBufferRange(MAZE_GL_COPY_WRITE_BUFFER, 0, m_vbo->getSizeBytes(), flags));
                }
            }
        }
        else
        {
            if (m_vbo->getAccessType() <= GPUByteBufferAccessType::DynamicDefault)
            {
                MAZE_ERROR_IF(m_singleMapping, "Multimapping detected!");
            }
        }

        _outTicket = addMappedRange(_start, _count);

        return static_cast<U8*>(m_mappedPtr) + _start;
    }

    //////////////////////////////////////////
    void VertexBufferObjectMappingControllerOpenGL::flush(
        Size _ticket,
        Size _flushStart,
        Size _flushCount)
    {
        MAZE_ERROR_IF(_flushStart > m_mappedRanges[_ticket].count || _flushStart + _flushCount > m_mappedRanges[_ticket].count, "VBO out of bounds flush!");

        MAZE_GL_CALL(mzglFlushMappedBufferRange(
            MAZE_GL_COPY_WRITE_BUFFER,
            m_mappedRanges[_ticket].start + _flushStart,
            _flushCount));
    }

    //////////////////////////////////////////
    void VertexBufferObjectMappingControllerOpenGL::unmap(Size _ticket)
    {
        MAZE_ERROR_IF(_ticket >= m_mappedRanges.size(), "Invalid unmap ticket!");
        MAZE_ERROR_IF(m_mappedRanges.size() == m_freeRanges.size(), "Unmapping an already unmapped buffer! Did you call unmap with the same ticket twice?");

        m_freeRanges.emplace_back(_ticket);

        if (m_mappedRanges.size() == m_freeRanges.size())
        {
            MAZE_GL_CALL(mzglUnmapBuffer(MAZE_GL_COPY_WRITE_BUFFER));
            m_mappedPtr = 0;
        }
    }

    //////////////////////////////////////////
    void VertexBufferObjectMappingControllerOpenGL::upload(
        const void* _data,
        Size _offset,
        Size _length)
    {
        MAZE_GL_CALL(mzglBufferSubData(MAZE_GL_COPY_WRITE_BUFFER, _offset, _length, _data));

        // #TODO:
        /*
        if (m_mappedRanges.size() != 0 &&
            m_mappedRanges.size() == m_freeRanges.size())
        {
            MAZE_GL_CALL(mzglBufferSubData(MAZE_GL_COPY_WRITE_BUFFER, offset, length, data));
        }
        else
        {
            Size ticket;
            void* dstData = map(offset, length, ticket);
            memcpy(dstData, data, length);

            flush(
                ticket, offset, length );

            unmap(ticket);
        }
        */
    }


    //////////////////////////////////////////
    Size VertexBufferObjectMappingControllerOpenGL::addMappedRange(Size _start, Size _count)
    {
        Size ticket;

        if (!m_freeRanges.empty())
        {
            ticket = m_freeRanges.back();
            m_mappedRanges[ticket] = MappedRange(_start, _count);
            m_freeRanges.pop_back();
        }
        else
        {
            ticket = m_mappedRanges.size();
            m_mappedRanges.emplace_back(_start, _count);
        }

        return ticket;
    }
    
} // namespace Maze
//////////////////////////////////////////
