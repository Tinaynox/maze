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
#include "maze-core/serialization/MazeValueSerialization.hpp"


//////////////////////////////////////////
// Namespace
namespace Maze
{
    //////////////////////////////////////////
    // ByteBuffer
    //
    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(Size _size)
        : m_data(nullptr)
        , m_size(0)
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
        setData(_byteBuffer.getData(), _byteBuffer.getSize());
    }

    //////////////////////////////////////////
    ByteBuffer::ByteBuffer(ByteBuffer&& _byteBuffer) noexcept
        : m_data(std::move(_byteBuffer.m_data))
        , m_size(std::move(_byteBuffer.m_size))
    {
        _byteBuffer.m_data = nullptr;
        _byteBuffer.m_size = 0;
    }

    //////////////////////////////////////////
    ByteBuffer::~ByteBuffer()
    {
        clear();
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(Size _size)
    {
        return std::make_shared<ByteBuffer>(_size);
    }

    ///////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(Size _size, U8 _fillSymbol)
    {
        return std::make_shared<ByteBuffer>(_size, _fillSymbol);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(U8 const* _buffer, Size _size)
    {
        return std::make_shared<ByteBuffer>(_buffer, _size);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(ByteBuffer const& _byteBuffer)
    {
        return std::make_shared<ByteBuffer>(_byteBuffer);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::Create(ByteBufferPtr const& _byteBuffer)
    {
        ByteBuffer const& byteBuffer = *(_byteBuffer.get());
        return std::make_shared<ByteBuffer>(byteBuffer);
    }
    
    //////////////////////////////////////////
    void ByteBuffer::clear()
    {
        if (!m_data)
            return;

        MAZE_DELETE_ARRAY(m_data);
        m_data = nullptr;
        m_size = 0;
    }

    //////////////////////////////////////////
    void ByteBuffer::setData(U8 const* _data, Size _size)
    {
        resize(_size);
        memcpy(m_data, _data, _size);
    }

    //////////////////////////////////////////
    void ByteBuffer::resize(Size _size)
    {
        if (m_size == (U32)_size)
            return;

        clear();

        m_size = (U32)_size;

        if (m_size == 0)
            return;

        try 
        {
            m_data = MAZE_NEW_ARRAY(U8, m_size);
        }
        catch (std::bad_alloc const& e)
        {
            MAZE_ERROR("Allocation failed: %s", e.what());
            throw;
        }
        
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
        DecodeBase64(_data, _count, getDataPointer());
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

        memcpy(_data, getData(), m_size);
    }

    //////////////////////////////////////////
    void ByteBuffer::deserialize(U8 const* _data)
    {
        DeserializeValue(m_size, _data);
        _data += sizeof(m_size);

        memcpy(getDataPointer(), _data, m_size);
    }

    //////////////////////////////////////////
    ByteBufferPtr ByteBuffer::createCopy()
    {
        return ByteBuffer::Create(*this);
    }

} // namespace Maze
//////////////////////////////////////////
