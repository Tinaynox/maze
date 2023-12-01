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
#include "MazeCoreHeader.hpp"
#include "maze-core/data/MazeByteBufferWriteStream.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
// Namespace
namespace Maze
{
    //////////////////////////////////////////
    // ByteBufferWriteStream
    //
    //////////////////////////////////////////
    ByteBufferWriteStream::ByteBufferWriteStream(ByteBuffer* _byteBuffer)
    {
        setBuffer(_byteBuffer);
    }

    //////////////////////////////////////////
    ByteBufferWriteStream::ByteBufferWriteStream(ByteBuffer& _byteBuffer)
    {
        setBuffer(&_byteBuffer);
    }

    //////////////////////////////////////////
    ByteBufferWriteStream::~ByteBufferWriteStream()
    {

    }

    //////////////////////////////////////////
    void ByteBufferWriteStream::setBuffer(ByteBuffer* _byteBuffer)
    {
        if (m_byteBuffer == _byteBuffer)
            return;

        m_byteBuffer = _byteBuffer;
        m_offset = m_byteBuffer ? m_byteBuffer->getSize() : 0u;
    }

    //////////////////////////////////////////
    void ByteBufferWriteStream::write(U8 const* _src, Size _size)
    {
        MAZE_ASSERT(m_byteBuffer);

        Size requiredSize = m_offset + _size;
        if (m_byteBuffer->getSize() < requiredSize)
            m_byteBuffer->resize(requiredSize);

        memcpy(m_byteBuffer->getDataPointer() + m_offset, _src, _size);
        m_offset += _size;
    }

    //////////////////////////////////////////
    void ByteBufferWriteStream::setOffset(Size _value)
    {
        m_offset = _value;
    }


} // namespace Maze
//////////////////////////////////////////
