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
#include "maze-core/system/MazeFileStats.hpp"
#include "maze-core/system/MazePath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern bool IsFileExists(Path const& _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool IsDirectory(Path const& _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern U32 GetFileSize(Path const& _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern UnixTime GetFileModificationTimestamp(Path const& _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern Path GetWorkingDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern Path GetBinaryFullPath();

        //////////////////////////////////////////
        MAZE_CORE_API extern Path GetBinaryDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern Path GetDocumentsDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern Path GetDefaultTemporaryDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern Path GetDefaultLogDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API extern bool CreateDirectoryRecursive(Path const& _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern Vector<Path> GetRegularFileNamesInPath(Path const& _localPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool CopyRegularFile(Path const& _sourceFullPath, Path const& _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool CopyDirectory(Path const& _sourceFullPath, Path const& _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool DeleteRegularFile(Path const& _fileFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool DeleteDirectory(Path const& _fullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool MoveRegularFile(Path const& _sourceFullPath, Path const& _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern bool MoveDirectory(Path const& _sourceFullPath, Path const& _destFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API extern Path ConvertLocalPathToFullPath(Path const& _localPath);
        
        ////////////////////////////////////
        MAZE_CORE_API extern FileStats GetFileStats(Path const& _fileFullPath);


    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFileHelperUnix_hpp_
//////////////////////////////////////////
