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
#pragma once
#if (!defined(_MazeVertexBufferObjectMappingControllerOpenGL_hpp_))
#define _MazeVertexBufferObjectMappingControllerOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexBufferObjectOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class VertexBufferObjectMappingControllerOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API VertexBufferObjectMappingControllerOpenGL
    {
    public:

        //////////////////////////////////////////
        // Struct MappedRange
        //
        //////////////////////////////////////////
        struct MappedRange
        {
            //////////////////////////////////////////
            MappedRange(
                Size _start,
                Size _count)
                : start(_start)
                , count(_count)
            {}

            Size start;
            Size count;
        };

    public:

        //////////////////////////////////////////
        VertexBufferObjectMappingControllerOpenGL(
            VertexBufferObjectOpenGL* _vbo,
            bool _singleMapping = false);

        //////////////////////////////////////////
        ~VertexBufferObjectMappingControllerOpenGL();

        //////////////////////////////////////////
        inline VertexBufferObjectOpenGL* getVertexBufferObject() const { return m_vbo; }

        //////////////////////////////////////////
        // Assumes VBO is already bound to GL_COPY_WRITE_BUFFER
        //
        //////////////////////////////////////////
        void* MAZE_RESTRICT_ALIAS_RETURN map(Size _start, Size _count, Size& _outTicket);

        //////////////////////////////////////////
        void flush(Size _ticket, Size _start, Size _count);

        //////////////////////////////////////////
        void unmap(Size _ticket);

        //////////////////////////////////////////
        void upload(
            void const* _data,
            Size _offset,
            Size _length);

        //////////////////////////////////////////
        inline bool isMapped() { return (m_mappedPtr != nullptr); }

    protected:

        //////////////////////////////////////////
        Size addMappedRange(Size _start, Size _count);

    protected:
        VertexBufferObjectOpenGL* m_vbo;

        void* m_mappedPtr;
        Vector<MappedRange> m_mappedRanges;
        Vector<Size> m_freeRanges;
        bool m_singleMapping;
    };
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexBufferObjectMappingControllerOpenGL_hpp_
//////////////////////////////////////////
