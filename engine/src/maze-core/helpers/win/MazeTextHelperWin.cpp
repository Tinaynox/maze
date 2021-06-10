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
            std::wstring_convert< 
                std::codecvt_utf8<wchar_t>,
                wchar_t,
                WStringAllocator,
                StringAllocator> myconv;
            return myconv.to_bytes(_text);
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ConvertUCS2ToUTF8(CWString _text, Size _size)
        {
            WString w(_text, _size);
            std::wstring_convert<
                std::codecvt_utf8<wchar_t>,
                wchar_t,
                WStringAllocator,
                StringAllocator> myconv;
            return myconv.to_bytes(w);
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API WString ConvertUTF8ToUCS2(CString _text)
        {
            std::wstring_convert< 
                std::codecvt_utf8_utf16<wchar_t>,
                wchar_t,
                WStringAllocator,
                StringAllocator> myconv;
            return myconv.from_bytes(_text);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API WString ConvertUTF8ToUCS2(CString _text, Size _size)
        {
            String w(_text, _size);
            std::wstring_convert< 
                std::codecvt_utf8_utf16<wchar_t>,
                wchar_t,
                WStringAllocator,
                StringAllocator> myconv;
            return myconv.from_bytes(w);
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
