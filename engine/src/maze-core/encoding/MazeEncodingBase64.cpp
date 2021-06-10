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
#include "maze-core/encoding/MazeEncodingBase64.hpp"
#include "maze-core/memory/MazeMemory.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static S8 const c_base64EncodingTable[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/' };

    //////////////////////////////////////////
    static U8 const* BuildBase64DecodingTable()
    {
        U8* base64DecodingTable = (U8*)malloc(256);

        for (U8 i = 0; i < 64; i++)
            base64DecodingTable[(U8)c_base64EncodingTable[i]] = i;

        return base64DecodingTable;
    }

    static U8 const* c_base64DecodingTable = BuildBase64DecodingTable();
    static U32 c_base64ModTable[] = { 0, 2, 1 };


    //////////////////////////////////////////
    MAZE_CORE_API String EncodeBase64(U8 const* _data, Size _length)
    {
        String result;
        result.resize(4 * ((_length + 2) / 3));

        for (Size i = 0, j = 0; i < _length;) 
        {

            U32 octet_a = i < _length ? _data[i++] : 0;
            U32 octet_b = i < _length ? _data[i++] : 0;
            U32 octet_c = i < _length ? _data[i++] : 0;

            U32 triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

            result[j++] = c_base64EncodingTable[(triple >> 3 * 6) & 0x3F];
            result[j++] = c_base64EncodingTable[(triple >> 2 * 6) & 0x3F];
            result[j++] = c_base64EncodingTable[(triple >> 1 * 6) & 0x3F];
            result[j++] = c_base64EncodingTable[(triple >> 0 * 6) & 0x3F];
        }

        for (Size i = 0; i < c_base64ModTable[_length % 3]; i++)
            result[result.size() - 1 - i] = '=';

        return result;
    }

    //////////////////////////////////////////
    MAZE_CORE_API Size GetDecodedBase64Size(CString _text, Size _length)
    {
        if (_length == 0)
            return 0;

        MAZE_ERROR_RETURN_VALUE_IF(_length % 4 != 0, 0, "Invalid base64 length!");

        Size outLength = _length / 4 * 3;
        if (_text[_length - 1] == '=') outLength--;
        if (_text[_length - 2] == '=') outLength--;

        return outLength;
    }

    //////////////////////////////////////////
    MAZE_CORE_API bool DecodeBase64(
        CString _text,
        Size _length,
        U8* _decodedData)
    {
        if (_length == 0)
            return false;

        Size decodedDataLength = GetDecodedBase64Size(_text, _length);
        if (decodedDataLength == 0)
            return false;

        for (U32 i = 0, j = 0; i < _length; )
        {
            uint32_t sextet_a = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];
            uint32_t sextet_b = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];
            uint32_t sextet_c = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];
            uint32_t sextet_d = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];

            uint32_t triple = (sextet_a << 3 * 6)
                + (sextet_b << 2 * 6)
                + (sextet_c << 1 * 6)
                + (sextet_d << 0 * 6);

            if (j < decodedDataLength) _decodedData[j++] = U8((triple >> 2 * 8) & 0xFF);
            if (j < decodedDataLength) _decodedData[j++] = U8((triple >> 1 * 8) & 0xFF);
            if (j < decodedDataLength) _decodedData[j++] = U8((triple >> 0 * 8) & 0xFF);
        }

        return true;
    }

    //////////////////////////////////////////
    MAZE_CORE_API Vector<U8> DecodeBase64(CString _text, Size _length)
    {
        if (_length == 0)
            return Vector<U8>();

        Size decodedDataLength = GetDecodedBase64Size(_text, _length);
        if (decodedDataLength == 0)
            return Vector<U8>();

        Vector<U8> result;
        result.resize(decodedDataLength);

        for (U32 i = 0, j = 0; i < _length; )
        {
            uint32_t sextet_a = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];
            uint32_t sextet_b = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];
            uint32_t sextet_c = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];
            uint32_t sextet_d = _text[i] == '=' ? 0 & i++ : c_base64DecodingTable[(U32)_text[i++]];

            uint32_t triple = (sextet_a << 3 * 6)
                + (sextet_b << 2 * 6)
                + (sextet_c << 1 * 6)
                + (sextet_d << 0 * 6);

            if (j < decodedDataLength) result[j++] = U8((triple >> 2 * 8) & 0xFF);
            if (j < decodedDataLength) result[j++] = U8((triple >> 1 * 8) & 0xFF);
            if (j < decodedDataLength) result[j++] = U8((triple >> 0 * 8) & 0xFF);
        }

        return result;
    }

} // namespace Maze
//////////////////////////////////////////
