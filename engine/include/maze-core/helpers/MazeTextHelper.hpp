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
#if (!defined(_MazeTextHelper_hpp_))
#define _MazeTextHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include <utf8.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace TextHelper
    {
        //////////////////////////////////////////
        template <typename In>
        static In DecodeUTF8ToUTF32(In _begin, In _end, U32& _output, U32 _replacement = 0);

        //////////////////////////////////////////
        template <typename In, typename Out>
        static Out ConvertUTF8ToUTF32(In _begin, In _end, Out _output);

        //////////////////////////////////////////
        template <typename Out>
        static Out EncodeUTF8ToUTF32(U32 _input, Out _output, U8 _replacement);

        //////////////////////////////////////////
        MAZE_CORE_API String ConvertUTF8ToUTF32(CString _begin, CString _end);

        //////////////////////////////////////////
        template <typename In>
        static In NextUTF8(In _begin, In _end);

        //////////////////////////////////////////
        template <typename In>
        static Size CountUTF8(In _begin, In _end);

        //////////////////////////////////////////
        inline StdString UTF8CodePointToStdString(U32 _codepoint) noexcept
        {    
            if (!_codepoint)
                return StdString();

            if (!utf8::internal::is_code_point_valid(_codepoint))
                return StdString();

            S8 u[5] = {0};
            utf8::append(_codepoint, u);
            return (CString)u;
        }

        //////////////////////////////////////////
        inline String UTF8CodePointToString(U32 _codepoint) noexcept
        {    
            if (!_codepoint)
                return String();

            if (!utf8::internal::is_code_point_valid(_codepoint))
                return String();

            S8 u[5] = {0};
            utf8::append(_codepoint, u);
            return (CString)u;
        }

        //////////////////////////////////////////
        inline StdString PopOneSymbolUTF8(StdString const& _text) noexcept
        {
            if (_text.empty())
                return StdString();

            StdString::const_iterator it = _text.end();
            utf8::previous(it, _text.begin());

            Size symbols = it - _text.begin();
            return _text.substr(0, symbols);
        }

        //////////////////////////////////////////
        inline String PopOneSymbolUTF8(String const& _text) noexcept
        {
            if (_text.empty())
                return String();

            String::const_iterator it = _text.end();
            utf8::previous(it, _text.begin());

            Size symbols = it - _text.begin();
            return _text.substr(0, symbols);
        }

        //////////////////////////////////////////
        inline Size GetSymbolsCountUTF8(StdString const& _text) noexcept
        {
            return utf8::distance(_text.begin(), _text.end());
        }

        //////////////////////////////////////////
        inline Size GetSymbolsCountUTF8(String const& _text) noexcept
        {
            return utf8::distance(_text.begin(), _text.end());
        }


    } // namespace TextHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeTextHelper.inl"


//////////////////////////////////////////
#endif // _MazeTextHelper_hpp_
