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
#include MAZE_INCLUDE_OS_FILE(maze-core/helpers, MazeTextHelper)
#include <codecvt>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace TextHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API String ConvertUCS2ToUTF8(CWString _text)
        {
            // Deprecated?
            //std::wstring_convert< 
            //    std::codecvt_utf8<wchar_t>,
            //    wchar_t,
            //    WStringAllocator,
            //    StringAllocator> myconv;
            //return myconv.to_bytes(_text);
            if (!_text)
                return String();

            // Calculate required buffer size (using -1 for null-terminated length, no null terminator in output)
            int cbMulti = WideCharToMultiByte(CP_UTF8, 0, _text, -1, nullptr, 0, nullptr, nullptr);
            if (cbMulti == 0)
                return String();

            // Allocate exact size (cbMulti includes space for output null, but we trim it)
            String str(static_cast<size_t>(cbMulti - 1), '\0');

            // Perform the conversion (includes null terminator in output buffer)
            if (WideCharToMultiByte(CP_UTF8, 0, _text, -1, &str[0], cbMulti, nullptr, nullptr) == 0)
                return String();  // Conversion failed

            // Trim the null terminator
            str.resize(cbMulti - 1);
            return str;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ConvertUCS2ToUTF8(CWString _text, Size _size)
        {
            if (!_text || _size == 0u)
                return String();

            // Calculate required buffer size (no null terminator)
            int cbMulti = WideCharToMultiByte(CP_UTF8, 0, _text, static_cast<S32>(_size), nullptr, 0, nullptr, nullptr);
            if (cbMulti == 0)
                return String();

            // Allocate exact size
            String str(static_cast<size_t>(cbMulti), '\0');

            // Perform the conversion
            if (WideCharToMultiByte(CP_UTF8, 0, _text, static_cast<S32>(_size), &str[0], cbMulti, nullptr, nullptr) == 0)
                return String();

            return str;
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API WString ConvertUTF8ToUCS2(CString _text)
        {
            // Deprecated?
            //std::wstring_convert< 
            //    std::codecvt_utf8_utf16<wchar_t>,
            //    wchar_t,
            //    WStringAllocator,
            //    StringAllocator> myconv;
            //return myconv.from_bytes(_text);
            
            if (!_text)
                return WString();

            // Calculate required buffer size (including null terminator)
            int cbWide = MultiByteToWideChar(CP_UTF8, 0, _text, -1, nullptr, 0);
            if (cbWide == 0)
                return WString();

            WString wstr(static_cast<size_t>(cbWide), L'\0');

            // Perform the conversion (writes null-terminated string)
            if (MultiByteToWideChar(CP_UTF8, 0, _text, -1, &wstr[0], cbWide) == 0)
                return WString();

            // Trim the null terminator
            wstr.resize(cbWide - 1);
            return wstr;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API WString ConvertUTF8ToUCS2(CString _text, Size _size)
        {
            //String w(_text, _size);
            //std::wstring_convert< 
            //    std::codecvt_utf8_utf16<wchar_t>,
            //    wchar_t,
            //    WStringAllocator,
            //    StringAllocator> myconv;
            //return myconv.from_bytes(w);
            if (!_text || _size == 0)
                return WString();

            // Calculate required buffer size (no null terminator)
            int cbWide = MultiByteToWideChar(CP_UTF8, 0, _text, static_cast<S32>(_size), nullptr, 0);
            if (cbWide == 0)
                return WString();

            WString wstr(static_cast<Size>(cbWide), L'\0');

            if (MultiByteToWideChar(CP_UTF8, 0, _text, static_cast<S32>(_size), &wstr[0], cbWide) == 0)
                return WString();

            return wstr;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String UTF8StringToLower(String const& _string)
        {
            WString wString = ConvertUTF8ToUCS2(_string.c_str());

            auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
            std::for_each(
                wString.begin(),
                wString.end(),
                [&f](wchar_t& c)
            {
                c = f.tolower(c);
            });

            String result = ConvertUCS2ToUTF8(wString.c_str());
            return result;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API String UTF8StringToUpper(String const& _string)
        {
            WString wString = ConvertUTF8ToUCS2(_string.c_str());

             auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
             std::for_each(
                wString.begin(),
                wString.end(),
                [&f](wchar_t& c)
                 {
                    c = f.toupper(c);
                 });

             String result = ConvertUCS2ToUTF8(wString.c_str());
             return result;
        }
        
    } // namespace TextHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
