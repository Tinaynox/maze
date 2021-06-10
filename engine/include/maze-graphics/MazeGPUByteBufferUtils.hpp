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
#if (!defined(_MazeGPUByteBufferUtils_hpp_))
#define _MazeGPUByteBufferUtils_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Enum GPUByteBufferAccessType
    //
    //////////////////////////////////////////
    namespace GPUByteBufferAccessType
    {
        enum Enum
        {
            None = 0,

            // GPU: Read
            // CPU: Inaccessible
            Immutable,

            // GPU: Read|Write
            // CPU: Inaccessible
            Default,

            // GPU: Read
            // CPU: Write
            DynamicDefault,

            // GPU: Read
            // CPU: Write (Persistent mapping)
            DynamicPersistent,

            // GPU: Read
            // CPU: Write (Persistent cache coherent mapping)
            DynamicPersistentCoherent,


            MAX
        };
    }


    //////////////////////////////////////////
    // Enum Class GPUByteBufferMappingState
    //
    //////////////////////////////////////////
    enum class GPUByteBufferMappingState
    {
        None = 0,
        Unmapped,
        Mapped,
        MAX
    };


    //////////////////////////////////////////
    // Enum Class GPUByteBufferUnmapOptions
    //
    //////////////////////////////////////////
    enum class GPUByteBufferUnmapOptions
    {
        UnmapAll = 0,
        KeepPersistent
    };


    //////////////////////////////////////////
    // Enum Class GPUByteBufferType
    //
    //////////////////////////////////////////
    enum class GPUByteBufferType
    {
        None = 0,

        VertexBufferObject,
        IndexBuffer,
        TextureBuffer,

        MAX
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUByteBufferUtils_hpp_
//////////////////////////////////////////
