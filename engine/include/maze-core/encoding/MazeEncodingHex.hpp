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
#if (!defined(_MazeEncodingHex_hpp_))
#define _MazeEncodingHex_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include <fstream>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_CORE_API String EncodeHex(U8 const* _data, Size _length);

    //////////////////////////////////////////
    MAZE_CORE_API inline String EncodeHex(Vector<U8> const& _buffer) { return EncodeHex(&_buffer[0], _buffer.size()); }

    //////////////////////////////////////////
    MAZE_CORE_API inline String EncodeHex(ByteBuffer const& _buffer) { return EncodeHex(_buffer.getDataRW(), _buffer.getSize()); }

    //////////////////////////////////////////
    MAZE_CORE_API Vector<U8> DecodeHex(CString _text, Size _length);

    //////////////////////////////////////////
    MAZE_CORE_API inline Vector<U8> DecodeHex(String const& _text) { return DecodeHex(_text.c_str(), _text.size()); }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEncodingHex_hpp_
//////////////////////////////////////////
