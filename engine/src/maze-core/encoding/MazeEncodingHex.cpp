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
#include "maze-core/encoding/MazeEncodingHex.hpp"
#include "maze-core/memory/MazeMemory.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static CString const c_hexLut = "0123456789ABCDEF";

    //////////////////////////////////////////
    MAZE_CORE_API String EncodeHex(U8 const* _data, Size _length)
    {
        String output;
        output.reserve(2 * _length);
        for (Size i = 0; i < _length; ++i)
        {
            U8 const c = _data[i];
            output.push_back(c_hexLut[c >> 4]);
            output.push_back(c_hexLut[c & 15]);
        }

        return output;
    }

    //////////////////////////////////////////
    MAZE_CORE_API Vector<U8> DecodeHex(CString _text, Size _length)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_length & 1, Vector<U8>(), "Odd length!");

        Vector<U8> output;
        output.reserve(_length / 2);
        for (Size i = 0; i < _length; i += 2)
        {
            S8 a = _text[i];
            CString p = std::lower_bound(c_hexLut, c_hexLut + 16, a);
            MAZE_ERROR_RETURN_VALUE_IF(*p != a, Vector<U8>(), "Not a hex digit!");

            S8 b = _text[i + 1];
            CString q = std::lower_bound(c_hexLut, c_hexLut + 16, b);
            MAZE_ERROR_RETURN_VALUE_IF(*q != b, Vector<U8>(), "Not a hex digit!");

            output.push_back(static_cast<U8>(((p - c_hexLut) << 4) | (q - c_hexLut)));
        }

        return output;
    }

} // namespace Maze
//////////////////////////////////////////
