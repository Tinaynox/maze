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
#if (!defined(_MazeFileHelperUnix_hpp_))
#define _MazeFileHelperUnix_hpp_


//////////////////////////////////////////
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern bool IsFileExists(CString _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool IsDirectory(CString _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern U32 GetFileSize(CString _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern UnixTime GetFileModificationTimestamp(CString _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern String GetWorkingDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern WString GetWorkingDirectoryW();

        //////////////////////////////////////////
        MAZE_CORE_API extern String GetBinaryFullPath();

        //////////////////////////////////////////
        MAZE_CORE_API extern String GetBinaryDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern String GetDocumentsDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern String GetDefaultTemporaryDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern String GetDefaultLogDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern bool CreateDirectoryRecursive(CString _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern Vector<String> GetRegularFileNamesInPath(CString _localPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool CopyRegularFile(CString _sourceFullPath, CString _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool CopyDirectory(CString _sourceFullPath, CString _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool DeleteRegularFile(CString _fileFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool DeleteDirectory(CString _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool MoveRegularFile(CString _sourceFullPath, CString _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool MoveDirectory(CString _sourceFullPath, CString _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern String ConvertLocalPathToFullPath(CString _localPath);


    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFileHelperUnix_hpp_
//////////////////////////////////////////
