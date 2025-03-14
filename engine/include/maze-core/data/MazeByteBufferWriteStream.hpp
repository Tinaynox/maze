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
#if (!defined(_MazeByteBufferWriteStream_hpp_))
#define _MazeByteBufferWriteStream_hpp_


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ByteBufferWriteStream
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ByteBufferWriteStream MAZE_FINAL
    {
    public:

        //////////////////////////////////////////
        ByteBufferWriteStream(ByteBuffer* _byteBuffer = nullptr);

        //////////////////////////////////////////
        ByteBufferWriteStream(ByteBuffer& _byteBuffer);

        //////////////////////////////////////////
        ByteBufferWriteStream(ByteBufferWriteStream const& _stream) = delete;

        //////////////////////////////////////////
        ByteBufferWriteStream(ByteBufferWriteStream&& _stream) = delete;

        //////////////////////////////////////////
        ~ByteBufferWriteStream();

        //////////////////////////////////////////
        ByteBufferWriteStream& operator=(ByteBufferWriteStream const& _stream) = delete;

        //////////////////////////////////////////
        ByteBufferWriteStream& operator=(ByteBufferWriteStream&& _stream) = delete;


        //////////////////////////////////////////
        void setBuffer(ByteBuffer* _byteBuffer);

        //////////////////////////////////////////
        Size getBufferSize() const;

        //////////////////////////////////////////
        Size getBufferCapacity() const;

        //////////////////////////////////////////
        void reserve(Size _size);


        //////////////////////////////////////////
        void write(U8 const* _src, Size _size);

        //////////////////////////////////////////
        inline void write(CString _src, Size _size) { write((U8 const*)_src, _size); }


        //////////////////////////////////////////
        template <typename TValue>
        inline void write(TValue const& _value);

        //////////////////////////////////////////
        template <Size TSize>
        inline void write(S8 const(&_value)[TSize]);

        //////////////////////////////////////////
        template <>
        inline void write(CString const& _value);



        //////////////////////////////////////////
        template <typename TValue>
        inline ByteBufferWriteStream& operator<<(TValue const& _value);

        /////////////////////////////////////////
        template <Size TSize>
        inline ByteBufferWriteStream& operator<<(S8 const(&_value)[TSize]);

        /////////////////////////////////////////
        template <>
        inline ByteBufferWriteStream& operator<<(String const& _value);

        /////////////////////////////////////////
        template <>
        inline ByteBufferWriteStream& operator<<(CString const& _value);

        //////////////////////////////////////////
        void setOffset(Size _value);

        //////////////////////////////////////////
        inline Size getOffset() const { return m_offset; }

    protected:
        ByteBuffer* m_byteBuffer;
        Size m_offset = 0u;
    };


    //////////////////////////////////////////
    template <typename TValue>
    inline void ByteBufferWriteStream::write(TValue const& _value)
    {
        write((U8 const*)&_value, sizeof(TValue));
    }

    //////////////////////////////////////////
    template <Size TSize>
    inline void ByteBufferWriteStream::write(S8 const(&_value)[TSize])
    {
        Size size = strlen(_value);
        write((U8 const*)_value, size);
    }

    //////////////////////////////////////////
    template <>
    inline void ByteBufferWriteStream::write(CString const& _value)
    {
        Size size = strlen(_value);
        write((U8 const*)_value, size);
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline ByteBufferWriteStream& ByteBufferWriteStream::operator<<(TValue const& _value)
    {
        write(_value);
        return *this;
    }

    /////////////////////////////////////////
    template <Size TSize>
    inline ByteBufferWriteStream& ByteBufferWriteStream::operator<<(S8 const(&_value)[TSize])
    {
        write(_value);
        return *this;
    }

    /////////////////////////////////////////
    template <>
    inline ByteBufferWriteStream& ByteBufferWriteStream::operator<<(String const& _value)
    {
        write((U8 const*)&_value[0], _value.size());
        return *this;
    }

    /////////////////////////////////////////
    template <>
    inline ByteBufferWriteStream& ByteBufferWriteStream::operator<<(CString const& _value)
    {
        write(_value);
        return *this;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeByteBufferWriteStream_h_
//////////////////////////////////////////
