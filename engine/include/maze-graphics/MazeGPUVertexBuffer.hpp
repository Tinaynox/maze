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
#if (!defined(_MazeGPUVertexBuffer_hpp_))
#define _MazeGPUVertexBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/MazeGPUByteBuffer.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GPUVertexBuffer);


    //////////////////////////////////////////
    // Class GPUVertexBuffer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GPUVertexBuffer
        : public GPUByteBuffer
    {
    public:

        ////////////////////////////////////
        GPUVertexBuffer(
            VertexDataDescription const& _vertexDataDescription,
            GPUByteBufferGateway* _gpuByteBufferGateway,
            Size _blockOffsetBytes,
            Size _elementsCount,
            Size _bytesPerElement,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData);

        ////////////////////////////////////
        virtual ~GPUVertexBuffer();

        ////////////////////////////////////
        virtual GPUByteBufferType getType() MAZE_OVERRIDE { return GPUByteBufferType::VertexBufferObject; }

    protected:
        const VertexDataDescription* m_vertexDataDescription;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUVertexBuffer_hpp_
//////////////////////////////////////////
