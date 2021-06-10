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
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/linux/MazeFileHelperLinux.hpp"
#include <unistd.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API String GetWorkingDirectory()
        {
            S8 buff[PATH_MAX + 1];
            getcwd(buff, sizeof(buff));
            return buff;
        }

        //////////////////////////////////////////
        MAZE_CORE_API WString GetWorkingDirectoryW()
        {
            WString text;
            StringHelper::FormatString(text, L"%s", GetWorkingDirectory().c_str());
            return text;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetBinaryFullPath()
        {
            char result[PATH_MAX];
            ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetBinaryDirectory()
        {
            return GetDirectoryInPath(GetBinaryFullPath());
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetDocumentsDirectory()
        {
            MAZE_TODO;
            return "";
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetDefaultTemporaryDirectory()
        {
            return "/tmp";
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetDefaultLogDirectory()
        {
            return GetBinaryDirectory() + "/log";
        }


    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
