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
#if (!defined(_MazeByteBuffer_hpp_))
#define _MazeByteBuffer_hpp_


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include <cstring>
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Aliases
    MAZE_USING_SHARED_PTR(ByteBuffer);


    //////////////////////////////////////////
    // Class ByteBuffer
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ByteBuffer MAZE_FINAL
        : public IBinarySerializable
        , public IStringSerializable
        , public ISharedCopyable<ByteBuffer>
    {
    public:

        //////////////////////////////////////////
        ByteBuffer(Size _size = 0);

        //////////////////////////////////////////
        ByteBuffer(U8 const* _buffer, Size _size);

        //////////////////////////////////////////
        ByteBuffer(Size _size, U8 _fillSymbol);

        //////////////////////////////////////////
        ByteBuffer(ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        ByteBuffer(ByteBuffer&& _byteBuffer) noexcept;

        //////////////////////////////////////////
        ~ByteBuffer();

        //////////////////////////////////////////
        static ByteBufferPtr Create(Size _size = 0);

        //////////////////////////////////////////
        static ByteBufferPtr Create(Size _size, U8 _fillSymbol);

        //////////////////////////////////////////
        static ByteBufferPtr Create(U8 const* _buffer, Size _size);

        //////////////////////////////////////////
        static ByteBufferPtr Create(ByteBuffer const& _otherBuffer);

        //////////////////////////////////////////
        static ByteBufferPtr Create(ByteBufferPtr const& _otherBuffer);

        //////////////////////////////////////////
        inline U8 const* getDataRO() const { return m_data; }

        //////////////////////////////////////////
        inline U8* getDataRW() const { return m_data; }

        //////////////////////////////////////////
        void setData(U8 const* _data, Size _size);

        //////////////////////////////////////////
        inline U32 getSize() const { return m_size; }

        //////////////////////////////////////////
        inline U32 getCapacity() const { return m_capacity; }

        //////////////////////////////////////////
        void clear();

        //////////////////////////////////////////
        void reserve(Size _capacity);

        //////////////////////////////////////////
        void resize(Size _size);

        //////////////////////////////////////////
        U8* insert(U32 _at, Size _size);

        //////////////////////////////////////////
        U8* insertAt(U32 _at, Size _size, U8 const* _data);

        //////////////////////////////////////////
        void erase(U32 _at, Size _size);

        //////////////////////////////////////////
        void append(U8 const* _data, Size _size);

        //////////////////////////////////////////
        template <typename TValue>
        inline void append(TValue const& _data)
        {
            append((U8 const*)&_data, sizeof(TValue));
        }

        //////////////////////////////////////////
        void fill(U8 _byte);

        //////////////////////////////////////////
        bool isEqual(ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        inline U8 getByte(Size _i) const
        {
            MAZE_DEBUG_BP_IF(_i >= (U32)m_size);
            return m_data[_i];
        }

        //////////////////////////////////////////
        inline void setByte(Size _i, U8 _byte)
        {
            MAZE_DEBUG_BP_IF(_i >= (U32)m_size);
            m_data[_i] = _byte;
        }

        //////////////////////////////////////////
        inline void swapBytes(Size _i, Size _j)
        {
            MAZE_DEBUG_BP_IF(_i >= (U32)m_size);
            MAZE_DEBUG_BP_IF(_j >= (U32)m_size);
            std::swap(m_data[_i], m_data[_j]);
        }

        //////////////////////////////////////////
        inline U32 readUnsafe(U32 _srcOffset, void* _dst, U32 _size) const
        {
            MAZE_DEBUG_BP_IF((U32)_srcOffset + _size > m_size);
            memcpy(_dst, m_data + _srcOffset, _size);
            return _size;
        }

        //////////////////////////////////////////
        inline U32 read(U32 _srcOffset, void* _dst, U32 _size) const
        {
            _size = Math::Min(_size, m_size - _srcOffset);
            return readUnsafe(_srcOffset, _dst, _size);
        }

        //////////////////////////////////////////
        inline U8& operator [](Size _i)
        {
            MAZE_DEBUG_BP_IF((_i < 0) || ((U32)_i >= m_size));
            return m_data[_i];
        }

        //////////////////////////////////////////
        inline U8 const& operator [](Size _i) const
        {
            MAZE_DEBUG_BP_IF((_i < 0) || ((U32)_i >= m_size));
            return m_data[_i];
        }

        //////////////////////////////////////////
        inline ByteBuffer& operator=(ByteBuffer const& _byteBuffer)
        {
            setData(_byteBuffer.getDataRO(), _byteBuffer.getSize());

            return *this;
        }

        //////////////////////////////////////////
        inline ByteBuffer& operator=(ByteBuffer&& _v) noexcept
        {
            std::swap(m_data, _v.m_data);
            std::swap(m_size, _v.m_size);
            std::swap(m_capacity, _v.m_capacity);

            return *this;
        }

        //////////////////////////////////////////
        void upload(U8 const* _data, Size _size);

        //////////////////////////////////////////
        inline void copyFrom(U8 const* _data, Size _size)
        {
            MAZE_DEBUG_ERROR_RETURN_IF((U32)_size > m_size, "Buffer overload!");
            memcpy(m_data, _data, _size);
        }

        //////////////////////////////////////////
        void append(ByteBuffer const& _byteBuffer);



        //////////////////////////////////////////
        inline U8 const* getEnd() const { return m_data + m_size; }

        //////////////////////////////////////////
        template <typename TValue>
        inline void iterateAs(
            std::function<void(TValue&)> const& _cb,
            Size _startOffset = 0u)
        {
            TValue* start = (TValue*)(m_data + _startOffset);
            TValue const* end = (TValue const*)getEnd();

            MAZE_DEBUG_BP_IF(start > end);

            for (TValue* it = start; it < end; ++it)
                _cb(*it);
        }


        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual U32 getValueSerializationSize() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void serialize(U8* _data) const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void deserialize(U8 const* _data) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ByteBufferPtr createCopy() MAZE_OVERRIDE;

    private:

        //////////////////////////////////////////
        bool reallocate(Size _size);

    protected:
        U8* m_data = nullptr;
        U32 m_size = 0u;
        U32 m_capacity = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeByteBuffer_h_
//////////////////////////////////////////
