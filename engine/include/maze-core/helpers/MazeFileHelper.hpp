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
#if (!defined(_MazeFileHelper_hpp_))
#define _MazeFileHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeFileStats.hpp"
#include "maze-core/system/MazePath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ByteBuffer);


    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void NormalizeFilePath(Path& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API Path NormalizedFilePath(Path const& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDirectoryInPath(Path const& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API Path GetParentDirectoryInPath(Path const& _path) noexcept;
        
        //////////////////////////////////////////
        MAZE_CORE_API Path GetFileNameInPath(Path const& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API Path GetFileExtension(Path const& _fileFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API Path GetFileNameWithoutExtension(Path const& _fileFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToString(Path const& _fileFullPath, String& _string);
      
        //////////////////////////////////////////
        MAZE_CORE_API String ReadFileAsString(Path const& _fileFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToByteBuffer(Path const& _fileFullPath, ByteBuffer& _byteBuffer);
        
        //////////////////////////////////////////
        MAZE_CORE_API ByteBufferPtr ReadFileAsByteBuffer(Path const& _fileFullPath);


        ////////////////////////////////////
        MAZE_CORE_API extern bool IsFileExists(Path const& _fullPath);
                
        ////////////////////////////////////
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
        MAZE_CORE_API extern bool CreateDirectoryRecursive(Path const& _path);

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
        inline bool Move(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            if (IsDirectory(_sourceFullPath))
                return MoveDirectory(_sourceFullPath, _destFullPath);
            else
                return MoveRegularFile(_sourceFullPath, _destFullPath);
        }


        //////////////////////////////////////////
        MAZE_CORE_API extern Path ConvertLocalPathToFullPath(Path const& _localPath);


        ////////////////////////////////////
        inline bool Delete(Path const& _fullPath)
        {
            if (IsDirectory(_fullPath))
                return DeleteDirectory(_fullPath);
            else
                return DeleteRegularFile(_fullPath);
        }


        ////////////////////////////////////
        MAZE_CORE_API extern FileStats GetFileStats(Path const& _fileFullPath);

    } // namespace FileHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFileHelper_hpp_
//////////////////////////////////////////
