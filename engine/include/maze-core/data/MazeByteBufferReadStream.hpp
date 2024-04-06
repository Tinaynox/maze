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
#if (!defined(_MazeByteBufferReadStream_hpp_))
#define _MazeByteBufferReadStream_hpp_


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
    // Class ByteBufferReadStream
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ByteBufferReadStream MAZE_FINAL
    {
    public:

        //////////////////////////////////////////
        ByteBufferReadStream(ByteBuffer const* _byteBuffer = nullptr);

        //////////////////////////////////////////
        ByteBufferReadStream(ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        ByteBufferReadStream(ByteBufferReadStream const& _stream) = delete;

        //////////////////////////////////////////
        ByteBufferReadStream(ByteBufferReadStream&& _stream) = delete;

        //////////////////////////////////////////
        ~ByteBufferReadStream();

        //////////////////////////////////////////
        ByteBufferReadStream& operator=(ByteBufferReadStream const& _stream) = delete;

        //////////////////////////////////////////
        ByteBufferReadStream& operator=(ByteBufferReadStream&& _stream) = delete;


        //////////////////////////////////////////
        void setBuffer(ByteBuffer const* _byteBuffer);


        //////////////////////////////////////////
        Size readNoRewind(U8* _dst, Size _size);

        //////////////////////////////////////////
        Size read(U8* _dst, Size _size);

        //////////////////////////////////////////
        template <typename TValue>
        inline Size readNoRewind(TValue& _value);

        //////////////////////////////////////////
        template <typename TValue>
        inline Size read(TValue& _value);

        //////////////////////////////////////////
        bool canRead(Size _size);

        //////////////////////////////////////////
        template <typename TValue>
        inline ByteBufferReadStream& operator>>(TValue& _value);

        //////////////////////////////////////////
        bool setOffset(Size _value);

        //////////////////////////////////////////
        inline Size getOffset() const { return m_offset; }

        //////////////////////////////////////////
        Size rewind(Size _delta);

        //////////////////////////////////////////
        U8 const* getDataRO() const;

        //////////////////////////////////////////
        bool isEndOfBuffer() const;

    protected:
        ByteBuffer const* m_byteBuffer;
        Size m_offset = 0u;
    };


    //////////////////////////////////////////
    template <typename TValue>
    inline Size ByteBufferReadStream::readNoRewind(TValue& _value)
    {
        return readNoRewind((U8*)&_value, sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline Size ByteBufferReadStream::read(TValue& _value)
    {
        return read((U8*)&_value, sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline ByteBufferReadStream& ByteBufferReadStream::operator>>(TValue& _value)
    {
        read(_value);
        return *this;
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeByteBufferReadStream_h_
//////////////////////////////////////////
