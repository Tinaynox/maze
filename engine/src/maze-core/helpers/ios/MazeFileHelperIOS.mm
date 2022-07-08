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
#include "maze-core/helpers/ios/MazeFileHelperIOS.hpp"
#include <mach-o/dyld.h>


//////////////////////////////////////////
#import <UIKit/UIKit.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API Path GetWorkingDirectory()
        {
            S8 buff[PATH_MAX + 1];
            getcwd(buff, sizeof(buff));
            return buff;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryFullPath()
        {
            static Path s_executablePath = NormalizedFilePath([[[NSBundle mainBundle] executablePath] UTF8String]);
            return s_executablePath;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryDirectory()
        {
            return GetDirectoryInPath(GetBinaryFullPath());
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDocumentsDirectory()
        {
            static Path s_homeDirectory = getenv("HOME");
            return NormalizedFilePath(s_homeDirectory) + "/Documents";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultTemporaryDirectory()
        {
            static Path s_tempPath = NormalizedFilePath([NSTemporaryDirectory() UTF8String]);
            return s_tempPath;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultLogDirectory()
        {
            static Path s_homeDirectory = getenv("HOME");
            return NormalizedFilePath(s_homeDirectory) + "/Documents/Log";
        }
    
    
    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
