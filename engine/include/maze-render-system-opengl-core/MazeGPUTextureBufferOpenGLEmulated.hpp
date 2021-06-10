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
#if (!defined(_MazeGPUTextureBufferOpenGLEmulated_hpp_))
#define _MazeGPUTextureBufferOpenGLEmulated_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUTextureBufferOpenGL.hpp"
#include "maze-graphics/MazeGPUByteBufferGateway.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GPUTextureBufferOpenGLEmulated);

    
    //////////////////////////////////////////
    // Class GPUTextureBufferOpenGLEmulated
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API GPUTextureBufferOpenGLEmulated
        : public GPUTextureBufferOpenGL
    {
    public:

        //////////////////////////////////////////
        GPUTextureBufferOpenGLEmulated(
            Vec2DU const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferGateway* _gpuByteBufferGateway,
            Size _blockOffsetBytes,
            Size _elementsCount,
            Size _bytesPerElement,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData);

        //////////////////////////////////////////
        virtual ~GPUTextureBufferOpenGLEmulated();
    };
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUTextureBufferOpenGLEmulated_hpp_
//////////////////////////////////////////
