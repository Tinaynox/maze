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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ByteBuffer);


    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void NormalizeFilePath(String& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API void NormalizeFilePath(WString& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API String NormalizedFilePath(String const& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API String GetDirectoryInPath(String const& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API String GetParentDirectoryInPath(String const& _path) noexcept;
        
        //////////////////////////////////////////
        MAZE_CORE_API String GetFileNameInPath(String const& _path) noexcept;

        //////////////////////////////////////////
        MAZE_CORE_API String GetFileExtension(String const& _fileFullPath); 

        //////////////////////////////////////////
        MAZE_CORE_API String GetFileNameWithoutExtension(String const& _fileFullPath); 

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToString(CString _fileFullPath, String& _string);
      
        //////////////////////////////////////////
        MAZE_CORE_API String ReadFileAsString(CString _fileFullPath);

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToByteBuffer(CString _fileFullPath, ByteBuffer& _byteBuffer);
        
        //////////////////////////////////////////
        inline Size ReadFileToByteBuffer(String const& _fileFullPath, ByteBuffer& _byteBuffer)
        { 
            return ReadFileToByteBuffer(_fileFullPath.c_str(), _byteBuffer); 
        }

        //////////////////////////////////////////
        MAZE_CORE_API ByteBufferPtr ReadFileAsByteBuffer(CString _fileFullPath);

        //////////////////////////////////////////
        inline ByteBufferPtr ReadFileAsByteBuffer(String const& _fileFullPath)
        { 
            return ReadFileAsByteBuffer(_fileFullPath.c_str()); 
        }


        ////////////////////////////////////
        MAZE_CORE_API extern bool IsFileExists(CString _fullPath);
        
        ////////////////////////////////////
        inline bool IsFileExists(String const& _fullPath) { return IsFileExists(_fullPath.c_str()); }
        
        ////////////////////////////////////
        MAZE_CORE_API extern bool IsDirectory(CString _fullPath);
        
        ////////////////////////////////////
        inline bool IsDirectory(String const& _fullPath) { return IsDirectory(_fullPath.c_str()); }

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
        MAZE_CORE_API extern bool CreateDirectoryRecursive(CString _path);

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
        inline bool Move(CString _sourceFullPath, CString _destFullPath)
        {
            if (IsDirectory(_sourceFullPath))
                return MoveDirectory(_sourceFullPath, _destFullPath);
            else
                return MoveRegularFile(_sourceFullPath, _destFullPath);
        }


        //////////////////////////////////////////
        MAZE_CORE_API extern String ConvertLocalPathToFullPath(CString _localPath);


        ////////////////////////////////////
        inline bool Delete(CString _fullPath)
        {
            if (IsDirectory(_fullPath))
                return DeleteDirectory(_fullPath);
            else
                return DeleteRegularFile(_fullPath);
        }


        ////////////////////////////////////
        MAZE_CORE_API extern FileStats GetFileStats(CString _fileFullPath);

    } // namespace FileHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFileHelper_hpp_
//////////////////////////////////////////
