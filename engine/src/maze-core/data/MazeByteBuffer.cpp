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
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/encoding/MazeEncodingBase64.hpp"
#include "maze-core/serialization/MazeBinarySerialization.hpp"


//////////////////////////////////////////
// Namespace
namespace Maze
{
    //////////////////////////////////////////
    // ByteBuffer
    //
    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(Size _size)
    {
        resize(_size);
    }

    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(U8 const* _buffer, Size _size)
        : ByteBuffer(_size)
    {
        setData(_buffer, _size);
    }

    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(Size _size, U8 _fillSymbol)
        : ByteBuffer(_size)
    {
        fill(_fillSymbol);
    }

    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(ByteBuffer const& _byteBuffer)
        : ByteBuffer(_byteBuffer.m_size)
    {
        setData(_byteBuffer.getDataRO(), _byteBuffer.getSize());
    }

    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(ByteBuffer&& _byteBuffer) noexcept
        : m_data(std::move(_byteBuffer.m_data))
        , m_size(std::move(_byteBuffer.m_size))
        , m_capacity(std::move(_byteBuffer.m_capacity))
    {
        _byteBuffer.m_data = nullptr;
        _byteBuffer.m_size = 0;
        _byteBuffer.m_capacity = 0;
    }

    //////////////////////////////////////////
    ByteBuffer::~ByteBuffer()
    {
        clear();
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(Size _size)
    {
        return MakeManagedShared<ByteBuffer>(_size);
    }

    ///////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(Size _size, U8 _fillSymbol)
    {
        return MakeManagedShared<ByteBuffer>(_size, _fillSymbol);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(U8 const* _buffer, Size _size)
    {
        return MakeManagedShared<ByteBuffer>(_buffer, _size);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(ByteBuffer const& _byteBuffer)
    {
        return MakeManagedShared<ByteBuffer>(_byteBuffer);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(ByteBufferPtr const& _byteBuffer)
    {
        ByteBuffer const& byteBuffer = *(_byteBuffer.get());
        return MakeManagedShared<ByteBuffer>(byteBuffer);
    }
    
    //////////////////////////////////////////
    void ByteBuffer::clear()
    {
        if (!m_data)
            return;

        MAZE_DELETE_ARRAY(m_data);
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    //////////////////////////////////////////
    void ByteBuffer::setData(U8 const* _data, Size _size)
    {
        resize(_size);
        memcpy(m_data, _data, _size);
    }

    //////////////////////////////////////////
    bool ByteBuffer::reallocate(Size _capacity)
    {
        if (m_capacity == (U32)_capacity)
            return false;

        if (_capacity == 0)
        {
            clear();
            return false;
        }

        m_capacity = (U32)_capacity;
        m_size = Math::Min(m_size, m_capacity);

        try
        {
            m_data = MAZE_NEW_ARRAY(U8, m_capacity);
        }
        catch (std::bad_alloc const& e)
        {
            MAZE_ERROR("Allocation failed: %s", e.what());
            throw;
        }

        return true;
    }

    //////////////////////////////////////////
    void ByteBuffer::reserve(Size _capacity)
    {
        if (_capacity <= m_capacity)
            return;

        U8* prevData = m_data;

        if (!reallocate(_capacity))
            return;

        if (prevData)
        {
            std::memcpy(m_data, prevData, m_size);
            MAZE_DELETE_ARRAY(prevData);
        }
    }

    //////////////////////////////////////////
    void ByteBuffer::resize(Size _size)
    {
        if (_size > m_capacity)
            reserve(_size);

        m_size = (U32)_size;
    }

    //////////////////////////////////////////
    U8* ByteBuffer::insert(U32 _at, Size _size)
    {
        MAZE_DEBUG_ASSERT(_at <= m_size);

        U32 requiredSize = m_size + (U32)_size;

        if (requiredSize <= m_capacity)
        {
            std::memmove(m_data + _at + _size, m_data + _at, m_size - _at);
        }
        else
        {
            U8* prevData = m_data;
            U32 prevSize = m_size;

            if (!reallocate(Math::Max(requiredSize, m_capacity + (m_capacity >> 1) + 1)))
                return nullptr;

            if (prevData)
            {
                std::memcpy(m_data, prevData, _at);
                std::memcpy(m_data + _at + _size, prevData + _at, prevSize - _at);
                MAZE_DELETE_ARRAY(prevData);
            }
        }

        m_size += (U32)_size;

        return m_data + _at;
    }

    //////////////////////////////////////////
    U8* ByteBuffer::insertAt(U32 _at, Size _size, U8 const* _data)
    {
        U8* destPtr = insert(_at, _size);
        memcpy(destPtr, _data, _size);
        return destPtr;
    }

    //////////////////////////////////////////
    void ByteBuffer::erase(U32 _at, Size _size)
    {
        if (_size == 0)
            return;

        MAZE_DEBUG_ASSERT(_at < m_size && _at + _size <= m_size);

        memmove(m_data + _at, m_data + _at + _size, m_size - (_at +_size));
        m_size -= (U32)_size;
    }

    //////////////////////////////////////////
    void ByteBuffer::append(U8 const* _data, Size _size)
    {
        U32 requiredSize = m_size + (U32)_size;

        if (requiredSize > m_capacity)
            reserve(Math::Max(requiredSize, m_capacity + (m_capacity >> 1) + 1));

        std::memcpy(m_data + m_size, _data, _size);
        m_size += (U32)_size;
    }

    //////////////////////////////////////////
    void ByteBuffer::fill(U8 _byte)
    {
        memset(m_data, _byte, m_size);
    }

    //////////////////////////////////////////
    bool ByteBuffer::isEqual(ByteBuffer const& _byteBuffer)
    {
        if (m_size != _byteBuffer.m_size)
            return false;

        return memcmp(m_data, _byteBuffer.m_data, m_size) == 0;
    }

    //////////////////////////////////////////
    void ByteBuffer::upload(U8 const* _data, Size _size)
    {
        resize(_size);
        copyFrom(_data, _size);
    }

    //////////////////////////////////////////
    void ByteBuffer::append(ByteBuffer const& _byteBuffer)
    {
        Size size = getSize();
        resize(size + _byteBuffer.getSize());
        memcpy(m_data + size, _byteBuffer.getDataRO(), _byteBuffer.getSize());
    }

    //////////////////////////////////////////
    String ByteBuffer::toString() const
    {
        return EncodeBase64(*this);
    }

    //////////////////////////////////////////
    void ByteBuffer::setString(CString _data, Size _count)
    {
        if (_count == 0)
            _count = strlen(_data);

        Size size = GetDecodedBase64Size(_data, _count);
        resize(size);
        DecodeBase64(_data, _count, getDataRW());
    }

    //////////////////////////////////////////
    U32 ByteBuffer::getValueSerializationSize() const
    {
        return sizeof(m_size) + m_size;
    }

    //////////////////////////////////////////
    void ByteBuffer::serialize(U8* _data) const
    {
        SerializeValue(m_size, _data);
        _data += sizeof(m_size);

        memcpy(_data, getDataRO(), m_size);
    }

    //////////////////////////////////////////
    void ByteBuffer::deserialize(U8 const* _data)
    {
        DeserializeValue(m_size, _data);
        _data += sizeof(m_size);

        memcpy(getDataRW(), _data, m_size);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::createCopy()
    {
        return ByteBuffer::Create(*this);
    }

} // namespace Maze
//////////////////////////////////////////
