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
namespace Maze
{
    //////////////////////////////////////////
    namespace TextHelper
    {
        //////////////////////////////////////////
        template <typename In>
        static In DecodeUTF8ToUTF32(In _begin, In _end, U32& _output, U32 _replacement)
        {
            // Some useful precomputed data
            static const int c_trailing[256] =
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
            };
            static const U32 c_offsets[6] =
            {
                0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080
            };

            // Decode the character
            int trailingBytes = c_trailing[static_cast<U8>(*_begin)];
            if (_begin + trailingBytes < _end)
            {
                _output = 0;
                switch (trailingBytes)
                {
                    case 4: _output += static_cast<U8>(*_begin++); _output <<= 6;
                    case 3: _output += static_cast<U8>(*_begin++); _output <<= 6;
                    case 2: _output += static_cast<U8>(*_begin++); _output <<= 6;
                    case 1: _output += static_cast<U8>(*_begin++); _output <<= 6;
                    case 5: _output += static_cast<U8>(*_begin++); _output <<= 6;
                    case 0: _output += static_cast<U8>(*_begin++);
                }

                _output -= c_offsets[trailingBytes];
            }
            else
            {
                // Incomplete character
                _begin = _end;
                _output = _replacement;
            }

            return _begin;
        }

        //////////////////////////////////////////
        template <typename In, typename Out>
        static Out ConvertUTF8ToUTF32(In _begin, In _end, Out _output)
        {
            while (_begin < _end)
            {
                U32 codepoint;
                _begin = DecodeUTF8ToUTF32(_begin, _end, codepoint);
                *_output++ = codepoint;
            }

            return _output;
        }

        //////////////////////////////////////////
        template <typename Out>
        static Out EncodeUTF8ToUTF32(U32 _input, Out _output, U8 _replacement)
        {
            // Some useful precomputed data
            static const U8 c_firstBytes[7] =
            {
                0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
            };

            // Encode the character
            if ((_input > 0x0010FFFF) || ((_input >= 0xD800) && (_input <= 0xDBFF)))
            {
                // Invalid character

                if (_replacement)
                    *_output++ = _replacement;
            }
            else
            {
                // Valid character

                // Get the number of bytes to write
                Size bytestoWrite = 1;
                if      (_input <  0x80)       bytestoWrite = 1;
                else if (_input <  0x800)      bytestoWrite = 2;
                else if (_input <  0x10000)    bytestoWrite = 3;
                else if (_input <= 0x0010FFFF) bytestoWrite = 4;

                // Extract the bytes to write
                U8 bytes[4];
                switch (bytestoWrite)
                {
                    case 4: bytes[3] = static_cast<U8>((_input | 0x80) & 0xBF); _input >>= 6;
                    case 3: bytes[2] = static_cast<U8>((_input | 0x80) & 0xBF); _input >>= 6;
                    case 2: bytes[1] = static_cast<U8>((_input | 0x80) & 0xBF); _input >>= 6;
                    case 1: bytes[0] = static_cast<U8>(_input | c_firstBytes[bytestoWrite]);
                }

                // Add them to the output
                _output = std::copy(bytes, bytes + bytestoWrite, _output);
            }

            return _output;
        }

        //////////////////////////////////////////
        template <typename In>
        static In NextUTF8(In _begin, In _end)
        {
            U32 codepoint;
            return DecodeUTF8ToUTF32(_begin, _end, codepoint);
        }

        //////////////////////////////////////////
        template <typename In>
        static Size CountUTF8(In _begin, In _end)
        {
            Size length = 0;
            while (_begin < _end)
            {
                _begin = NextUTF8(_begin, _end);
                ++length;
            }

            return length;
        }



    } // namespace TextHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
