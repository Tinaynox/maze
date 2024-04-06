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
#include "maze-core/data/MazeByteBufferReadStream.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
// Namespace
namespace Maze
{
    //////////////////////////////////////////
    // ByteBufferReadStream
    //
    //////////////////////////////////////////
    ByteBufferReadStream::ByteBufferReadStream(ByteBuffer const* _byteBuffer)
        : m_byteBuffer(_byteBuffer)
    {

    }

    //////////////////////////////////////////
    ByteBufferReadStream::ByteBufferReadStream(ByteBuffer const& _byteBuffer)
        : ByteBufferReadStream(&_byteBuffer)
    {

    }

    //////////////////////////////////////////
    ByteBufferReadStream::~ByteBufferReadStream()
    {

    }

    //////////////////////////////////////////
    void ByteBufferReadStream::setBuffer(ByteBuffer const* _byteBuffer)
    {
        if (m_byteBuffer == _byteBuffer)
            return;

        m_byteBuffer = _byteBuffer;
        m_offset = 0u;
    }

    //////////////////////////////////////////
    Size ByteBufferReadStream::readNoRewind(U8* _dst, Size _size)
    {
        MAZE_ASSERT(m_byteBuffer);
        Size readSize = Math::Min(_size, m_byteBuffer->getSize() - m_offset);

        memcpy(_dst, m_byteBuffer->getDataRO() + m_offset, _size);
        return readSize;
    }

    //////////////////////////////////////////
    Size ByteBufferReadStream::read(U8* _dst, Size _size)
    {
        Size readSize = readNoRewind(_dst, _size);
        m_offset += readSize;
        return readSize;
    }

    //////////////////////////////////////////
    bool ByteBufferReadStream::canRead(Size _size)
    {
        return m_offset + _size <= m_byteBuffer->getSize();
    }

    //////////////////////////////////////////
    bool ByteBufferReadStream::setOffset(Size _value)
    {
        MAZE_ASSERT(m_byteBuffer);
        if (_value <= m_byteBuffer->getSize())
        {
            m_offset = _value;
            return true;
        }
        else
        {
            m_offset = m_byteBuffer->getSize();
            return false;
        }
    }

    //////////////////////////////////////////
    Size ByteBufferReadStream::rewind(Size _delta)
    {
        if (_delta > 0)
            setOffset(m_offset + _delta);
        else
        if (_delta >= m_offset)
            setOffset(m_offset - _delta);
        else
            setOffset(0);

        return m_offset;
    }

    //////////////////////////////////////////
    U8 const* ByteBufferReadStream::getDataRO() const
    {
        MAZE_ASSERT(m_byteBuffer);
        return m_byteBuffer->getDataRO();
    }

    //////////////////////////////////////////
    bool ByteBufferReadStream::isEndOfBuffer() const
    {
        MAZE_ASSERT(m_byteBuffer);
        return m_offset >= m_byteBuffer->getSize();
    }


} // namespace Maze
//////////////////////////////////////////
