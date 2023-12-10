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
#if (!defined(_MazeGPUTextureBuffer_hpp_))
#define _MazeGPUTextureBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"
#include "maze-graphics/MazeGPUByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(GPUTextureBuffer);
    MAZE_USING_SHARED_PTR(GPUByteBufferGateway);


    //////////////////////////////////////////
    // Class GPUTextureBuffer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GPUTextureBuffer
        : public GPUByteBuffer
    {
    public:

        //////////////////////////////////////////
        friend class ShaderUniform;

    public:

        //////////////////////////////////////////
        GPUTextureBuffer(
            const Vec2U& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferGateway* _gpuByteBufferGateway,
            Size _blockOffsetBytes,
            Size _elementsCount,
            Size _bytesPerElement,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData);

        //////////////////////////////////////////
        virtual ~GPUTextureBuffer();

        //////////////////////////////////////////
        virtual GPUByteBufferType getType() MAZE_OVERRIDE { return GPUByteBufferType::TextureBuffer; }

        //////////////////////////////////////////
        Texture2DPtr const& getTexture() const { return m_texture; }

    protected:
        Vec2U m_size;
        PixelFormat::Enum m_pixelFormat;

        Texture2DPtr m_texture;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUTextureBuffer_hpp_
//////////////////////////////////////////
