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
#include "maze-graphics/MazeGPUByteBufferGateway.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeGPUByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GPUByteBufferGateway
    //
    //////////////////////////////////////////
    GPUByteBufferGateway::GPUByteBufferGateway()
        : m_buffer(nullptr)
        , m_lastMappingStart(0)
        , m_lastMappingCount(0)
    {
    }

    //////////////////////////////////////////
    GPUByteBufferGateway::~GPUByteBufferGateway()
    {
    }

    //////////////////////////////////////////
    Size GPUByteBufferGateway::getBlockOffsetBytes() const
    {
        return m_buffer->getBlockOffsetBytes();
    }

    //////////////////////////////////////////
    Size GPUByteBufferGateway::getBlockOffsetElements() const
    {
        return m_buffer->getBlockOffsetElements();
    }

    //////////////////////////////////////////
    Size GPUByteBufferGateway::getElementsCount() const
    {
        return m_buffer->getElementsCount();
    }

    //////////////////////////////////////////
    Size GPUByteBufferGateway::getBytesPerElement() const
    {
        return m_buffer->getBytesPerElement();
    }

    //////////////////////////////////////////
    GPUByteBufferAccessType::Enum GPUByteBufferGateway::getAccessType() const
    {
        return m_buffer->getAccessType();
    }

    //////////////////////////////////////////
    GPUByteBufferMappingState GPUByteBufferGateway::getMappingState() const
    {
        return m_buffer->getMappingState();
    }

    //////////////////////////////////////////
    void GPUByteBufferGateway::setBuffer(GPUByteBuffer* _buffer)
    {
        m_buffer = _buffer;
    }

} // namespace Maze
//////////////////////////////////////////
