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
#if (!defined(_MazeGPUBuffersControllerOpenGL_hpp_))
#define _MazeGPUBuffersControllerOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"
#include "maze-graphics/MazePixelFormat.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct ContextOpenGLConfig;


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GPUBuffersControllerOpenGL);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(GPUVertexBuffer);
    MAZE_USING_SHARED_PTR(GPUTextureBuffer);
    MAZE_USING_SHARED_PTR(VertexBufferObjectOpenGL);


    //////////////////////////////////////////
    // Class GPUBuffersControllerOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API GPUBuffersControllerOpenGL 
    {
    public:

        //////////////////////////////////////////
        ~GPUBuffersControllerOpenGL();

        //////////////////////////////////////////
        static GPUBuffersControllerOpenGLPtr Create(RenderSystemOpenGL* _renderSystem);

        //////////////////////////////////////////
        GPUVertexBufferPtr createGPUVertexBuffer(
            ContextOpenGL* _contextOpenGL,
            VertexDataDescription const& _vertexDataDescription,
            Size _vertexCount,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr);

        //////////////////////////////////////////
        GPUTextureBufferPtr createGPUTextureBuffer(
            ContextOpenGL* _contextOpenGL,
            Vec2DU const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr);

    protected:

        //////////////////////////////////////////
        GPUBuffersControllerOpenGL();

        //////////////////////////////////////////
        bool init(RenderSystemOpenGL* _renderSystem);

        //////////////////////////////////////////
        void allocateVertexBufferObjectBlock(
            ContextOpenGL* _contextOpenGL,
            Size _sizeBytes,
            Size _alignment,
            GPUByteBufferAccessType::Enum _accessType,
            Size& _outVertexBufferObjectIndex,
            Size& _outBufferOffset);

        //////////////////////////////////////////
        void deallocateVertexBufferObjectBlock(
            ContextOpenGL* _contextOpenGL,
            Size _vertexBufferObjectIndex,
            Size _bufferOffset,
            Size _sizeBytes,
            GPUByteBufferAccessType::Enum _accessType);

    private:
        RenderSystemOpenGL* m_renderSystem;

        Size m_defaultPoolBufferObjectsSize[GPUByteBufferAccessType::MAX];
        Map<ContextOpenGL*, Vector<VertexBufferObjectOpenGLPtr>> m_vertexBufferObjectsPool[GPUByteBufferAccessType::MAX];
    };

    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUBuffersControllerOpenGL_hpp_
//////////////////////////////////////////
