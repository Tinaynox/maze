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
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <cwchar>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace StringHelper
    {
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(Char* _buffer, Size _buflen, CString _format, va_list _args)
        {
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
            return vsprintf_s(_buffer, _buflen, _format, _args); 
#else
            return vsprintf(_buffer, _format, _args);
#endif
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WChar* _buffer, Size _buflen, CWString _format, va_list _args)
        {
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
            return vswprintf_s(_buffer, _buflen, _format, _args); 
#else
            return vswprintf(_buffer, _buflen, _format, _args);
#endif
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(Char* _buffer, Size _buflen, CString _format, ...)
        {
            va_list args;
            va_start(args, _format);
            S32 result = FormatString(_buffer, _buflen, _format, args);
            va_end(args);  
            return result;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WChar* _buffer, Size _buflen, CWString _format, ...)
        {
            va_list args;
            va_start(args, _format);
            S32 result = FormatString(_buffer, _buflen, _format, args);
            va_end(args);  
            return result;
        }
        
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(String& _buffer, CString _format, ...)
        {
            S32 result = 0;
            
            va_list args;
            va_start(args, _format);
            
            S32 requiredSize = FormattedStringSize(_format, args);
            if (requiredSize > 0)
            {
                _buffer.resize((Size)requiredSize + 1);
                
                result = FormatString(&_buffer[0], _buffer.size(), _format, args);
            }
            
            va_end(args);
            
            return result;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormatString(WString& _buffer, CWString _format, ...)
        {
            S32 result = 0;
            
            va_list args;
            va_start(args, _format);
            
            S32 requiredSize = FormattedStringSize(_format, args);
            if (requiredSize > 0)
            {
                _buffer.resize((Size)requiredSize + 1);
                
                result = FormatString(&_buffer[0], _buffer.size(), _format, args);
            }
            
            va_end(args);
            
            return result;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CString _format, va_list _args)
        {
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
            return _vscprintf(_format, _args);
#else            
            S32 retval;
            va_list argcopy;
            va_copy(argcopy, _args);
            retval = vsnprintf(nullptr, 0, _format, argcopy);
            va_end(argcopy);
            return retval;
#endif            
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CWString _format, va_list _args)
        {
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
            return _vscwprintf(_format, _args);            
#else
            S32 bufSize = 1024;
            while (bufSize < 1024 * 1024)
            {
                va_list argcopy;
                va_copy(argcopy, _args);
                WChar buffer[bufSize];
                S32 fmtSize = vswprintf(
                    buffer,
                    sizeof(buffer) / sizeof(buffer[0]),
                    _format,
                    argcopy);
                va_end(argcopy);
                
                if (fmtSize >= 0)
                    return fmtSize;
                
                bufSize *= 2;
            }
            
            return -1;
            
#endif            
        }
        
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CString _format, ...)
        {
            va_list args;
            va_start(args, _format);
            S32 result = FormattedStringSize(_format, args);
            va_end(args);
            return result;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API S32 FormattedStringSize(CWString _format, ...)
        {
            va_list args;
            va_start(args, _format);
            S32 result = FormattedStringSize(_format, args);
            va_end(args);  
            return result;
        }
        
    } // namespace StringHelper
    ////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
