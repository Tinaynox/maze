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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeGPUByteBuffer.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeGPUByteBufferGateway.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GPUByteBuffer
    //
    //////////////////////////////////////////
    GPUByteBuffer::GPUByteBuffer(
        GPUByteBufferGateway* gpuByteBufferGateway,
        Size blockOffsetBytes,
        Size elementsCount,
        Size bytesPerElement,
        GPUByteBufferAccessType::Enum accessType,
        void* initialData)
        : m_gpuByteBufferGateway(gpuByteBufferGateway)
        , m_blockOffsetBytes(blockOffsetBytes)
        , m_blockOffsetElements(blockOffsetBytes / bytesPerElement)
        , m_elementsCount(elementsCount)
        , m_bytesPerElement(bytesPerElement)
        , m_accessType(accessType)
        , m_mappingState(GPUByteBufferMappingState::Unmapped)
        , m_uploadsCount(0)
    {
        gpuByteBufferGateway->setBuffer(this);

        MAZE_ERROR_IF(m_accessType == GPUByteBufferAccessType::Immutable && !initialData, "Immutable blocks must provide initial data!");
    }

    //////////////////////////////////////////
    GPUByteBuffer::~GPUByteBuffer()
    {
        if (m_mappingState != GPUByteBufferMappingState::Unmapped)
        {
            MAZE_ERROR("Deleting mapped buffer without having it unmapped!");
            unmap(GPUByteBufferUnmapOptions::UnmapAll);
        }
    }

    //////////////////////////////////////////
    void* MAZE_RESTRICT_ALIAS_RETURN GPUByteBuffer::map(
        Size _elementStart,
        Size _elementsCount)
    {
        MAZE_ERROR_IF(m_accessType < GPUByteBufferAccessType::DynamicDefault, "Only dynamic buffers can be mapped! Use upload instead.");
        MAZE_ERROR_IF(_elementsCount == 0, "Mapping 0 bytes is forbidden!");
        MAZE_ERROR_IF(m_mappingState == GPUByteBufferMappingState::Mapped, "Buffer already mapped!");

        m_mappingState = GPUByteBufferMappingState::Mapped;
        return m_gpuByteBufferGateway->map(_elementStart, _elementsCount);
    }

    //////////////////////////////////////////
    void GPUByteBuffer::unmap(
        GPUByteBufferUnmapOptions _unmapOption,
        Size _flushElementStart,
        Size _flushElementsCount)
    {
        MAZE_ERROR_IF(m_mappingState == GPUByteBufferMappingState::Unmapped, "Buffer wasn't mapped!");

        m_gpuByteBufferGateway->unmap(_unmapOption, _flushElementStart, _flushElementsCount);

        m_mappingState = GPUByteBufferMappingState::Unmapped;
        /*
        if (   _unmapOption == GPUByteBufferUnmapOptions::UnmapAll 
            || getAccessType() == GPUByteBufferAccessType::DynamicDefault
            || !getSupportPersistentMapping()) // <<-- #TODO
        {
            m_mappingState = GPUByteBufferMappingState::Unmapped;
        }        
        */
    }

    //////////////////////////////////////////
    void GPUByteBuffer::upload(
        const void* data,
        Size elementStart,
        Size elementsCount)
    {
        // First upload
        if (m_uploadsCount == 0)
        {
            GPUByteBufferAccessType::Enum accessType = m_accessType;
            if (m_accessType == GPUByteBufferAccessType::Immutable)
                m_accessType = GPUByteBufferAccessType::Default;

            m_gpuByteBufferGateway->upload(data, elementStart, elementsCount);

            m_accessType = accessType;
        }
        else
        {
            m_gpuByteBufferGateway->upload(data, elementStart, elementsCount);
        }

        ++m_uploadsCount;
    }

} // namespace Maze
//////////////////////////////////////////
