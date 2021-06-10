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
#if (!defined(_MazeGPUByteBufferGateway_hpp_))
#define _MazeGPUByteBufferGateway_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GPUByteBufferGatewayGateway);
    MAZE_USING_SHARED_PTR(GPUByteBuffer);


    //////////////////////////////////////////
    // Class GPUByteBufferGateway
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GPUByteBufferGateway
    {
    public:
        //////////////////////////////////////////
        friend class GPUByteBuffer;

    public:

        //////////////////////////////////////////
        virtual ~GPUByteBufferGateway();

        //////////////////////////////////////////
        virtual void* MAZE_RESTRICT_ALIAS_RETURN map(
            Size _elementStart,
            Size _elementsCount) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void unmap(
            GPUByteBufferUnmapOptions _unmapOption,
            Size _elementStart = 0,
            Size _elementsCount = 0) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void upload(
            void const* _data,
            Size _elementStart,
            Size _elementsCount) MAZE_ABSTRACT;


        //////////////////////////////////////////
        Size getBlockOffsetBytes() const;

        //////////////////////////////////////////
        Size getBlockOffsetElements() const;

        //////////////////////////////////////////
        Size getElementsCount() const;

        //////////////////////////////////////////
        Size getBytesPerElement() const;

        //////////////////////////////////////////
        GPUByteBufferAccessType::Enum getAccessType() const;

        //////////////////////////////////////////
        GPUByteBufferMappingState getMappingState() const;

        //////////////////////////////////////////
        Size getLastMappingStart() const { return m_lastMappingStart; }

        //////////////////////////////////////////
        Size getLastMappingCount() const { return m_lastMappingCount; }

    protected:

        //////////////////////////////////////////
        GPUByteBufferGateway();

        //////////////////////////////////////////
        virtual void setBuffer(GPUByteBuffer* _buffer);        

    protected:
        GPUByteBuffer* m_buffer;
        Size m_lastMappingStart;
        Size m_lastMappingCount;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUByteBufferGateway_hpp_
//////////////////////////////////////////
