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
#if (!defined(_MazeGPUByteBuffer_hpp_))
#define _MazeGPUByteBuffer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-graphics/MazeGPUByteBufferUtils.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GPUByteBufferGateway);


    //////////////////////////////////////////
    // Class GPUByteBuffer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GPUByteBuffer
    {
    public:

        //////////////////////////////////////////
        GPUByteBuffer(
            GPUByteBufferGateway* _gpuByteBufferGateway,
            Size _blockOffsetBytes,
            Size _elementsCount,
            Size _bytesPerElement,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData);

        //////////////////////////////////////////
        virtual ~GPUByteBuffer();

        //////////////////////////////////////////
        virtual GPUByteBufferType getType() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void* MAZE_RESTRICT_ALIAS_RETURN map(
            Size _elementStart,
            Size _elementsCount);

        //////////////////////////////////////////
        virtual void unmap(
            GPUByteBufferUnmapOptions unmapOption,
            Size flushElementStart = 0,
            Size flushElementsCount = 0);

        //////////////////////////////////////////
        virtual void upload(
            void const* data,
            Size elementStart,
            Size elementsCount);

        //////////////////////////////////////////
        inline Size getBlockOffsetBytes() const { return m_blockOffsetBytes; }

        //////////////////////////////////////////
        inline Size getBlockOffsetElements() const { return m_blockOffsetElements; }

        //////////////////////////////////////////
        inline Size getElementsCount() const { return m_elementsCount; }

        //////////////////////////////////////////
        inline Size getBytesPerElement() const { return m_bytesPerElement; }

        //////////////////////////////////////////
        inline GPUByteBufferAccessType::Enum getAccessType() const { return m_accessType; }

        //////////////////////////////////////////
        inline GPUByteBufferMappingState getMappingState() const { return m_mappingState; }

    protected:

        //////////////////////////////////////////
        inline GPUByteBufferGateway* getGPUByteBufferGateway() const { return m_gpuByteBufferGateway.get(); }

    protected:
        UniquePtr<GPUByteBufferGateway> m_gpuByteBufferGateway;

        Size m_blockOffsetBytes;
        Size m_blockOffsetElements;
        Size m_elementsCount;
        Size m_bytesPerElement;

        GPUByteBufferAccessType::Enum m_accessType;
        GPUByteBufferMappingState m_mappingState;

        Size m_uploadsCount;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGPUByteBuffer_hpp_
//////////////////////////////////////////
