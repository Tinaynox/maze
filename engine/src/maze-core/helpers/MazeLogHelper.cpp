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
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/MazeTypes.hpp"
#include <cwchar>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Debug
    {
        //////////////////////////////////////////
        MAZE_CORE_API void Log(CString _text, ...)
        {
            String newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Default, "%s\n", newText.c_str());
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API void Log(CWString _text, ...)
        {
            WString newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Default, L"%s\n", newText.c_str());
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API void LogMinor(CString _text, ...)
        {
            String newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Minor, "%s\n", newText.c_str());
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API void LogMinor(CWString _text, ...)
        {
            WString newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Minor, L"%s\n", newText.c_str());
        }
    
    
        //////////////////////////////////////////
        MAZE_CORE_API void LogWarning(CString _text, ...)
        {
            String newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Warning, "%s\n", newText.c_str());
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API void LogWarning(CWString _text, ...)
        {
            WString newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Warning, L"%s\n", newText.c_str());
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API void LogError(CString _text, ...)
        {
            String newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Error, "%s\n", newText.c_str());
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API void LogError(CWString _text, ...)
        {
            WString newText;
            MAZE_FORMAT_VA_STRING(_text, newText);
            LogService::GetInstancePtr()->logFormatted(c_logPriority_Error, L"%s\n", newText.c_str());
        }
    
    } // namespace Debug
    //////////////////////////////////////////
    
} // namespace Maze 
//////////////////////////////////////////
