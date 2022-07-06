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
#include "maze-core/helpers/unix/MazeFileHelperUnix.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"

#if (defined(_XOPEN_SOURCE))
#   undef _XOPEN_SOURCE
#endif
#define _XOPEN_SOURCE 500
#include <ftw.h>

#include <strings.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>

#if (MAZE_PLATFORM != MAZE_PLATFORM_LINUX)
#    include <codecvt>
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool IsFileExists(Path const& _fullPath)
        {
            struct stat st;

            if (stat(_fullPath.c_str(), &st) == 0)
                return true;

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool IsDirectory(Path const& _fullPath)
        {
            struct stat st;

            if (stat(_fullPath.c_str(), &st) == 0)
            {
                if (S_ISDIR(st.st_mode))
                {
                    return true;
                }
            }

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API U32 GetFileSize(Path const& _fullPath)
        {
            struct stat st;

            if (stat(_fullPath.c_str(), &st) == 0)
            {
                return static_cast<U32>(st.st_size);
            }

            return 0;
        }

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetFileModificationTimestamp(Path const& _fullPath)
        {
            struct stat st;

            if (stat(_fullPath.c_str(), &st) == 0)
            {
                return static_cast<UnixTime>(st.st_mtime);
            }

            return 0;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CreateDirectoryRecursive(Path const& _fullPath)
        {
            String path = _fullPath;
            NormalizeFilePath(path);

            struct stat st;

            String tempLocal;
            Vector<String> directories;
            StringHelper::SplitWords(path, directories, '/');

            for (S32 i = 0; i < (S32)directories.size(); ++i)
            {
                String directoryPath = tempLocal + directories[i];
                do
                {
                    if (directoryPath.empty())
                        break;

                    if (stat(directoryPath.c_str(), &st) != 0)
                    {
                        if (mkdir(directoryPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) != 0)
                        {
                            int error = errno;
                            if (error == EEXIST || error == EROFS)
                            {
                                break;
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                while (false);

                tempLocal += directories[i] + '/';
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Vector<Path> GetRegularFileNamesInPath(Path const& _localPath)
        {

#if (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
            // Forbidden
            if (strcmp(_localPath.c_str(), "proc") == 0 ||
                strcmp(_localPath.c_str(), "/proc") == 0 )
                return Vector<Path>();
#endif


            Vector<Path> result;
            String fullPath = ConvertLocalPathToFullPath(_localPath);

            DIR* dp;
            struct dirent* ep;
            dp = opendir(fullPath.c_str());

            if (dp != nullptr)
            {
                ep = readdir(dp);

                while (ep)
                {
                    if (  !(strcmp(ep->d_name, ".") == 0)
                       && !(strcmp(ep->d_name, "..") == 0))
                    {
                        result.push_back(ep->d_name);
                    }

                    ep = readdir(dp);

                }

                closedir(dp);
            }

            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CopyRegularFile(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            char buf[BUFSIZ];
            size_t size;

            FILE* source = fopen(_sourceFullPath.c_str(), "rb");
            if (!source)
                return false;

            FILE* dest = fopen(_destFullPath.c_str(), "wb");
            if (!dest)
            {
                fclose(source);
                return false;
            }

            size = fread(buf, 1, BUFSIZ, source);
            while (size)
            {
                fwrite(buf, 1, size, dest);
                size = fread(buf, 1, BUFSIZ, source);
            }

            fclose(source);
            fclose(dest);

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CopyDirectory(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            MAZE_ERROR("Not implemented yet!");
            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool DeleteRegularFile(Path const& _fileFullPath)
        {
            if (std::remove(_fileFullPath) != 0)
                return false;

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API int DeleteFileHelper(const char* _fpath, const struct stat* _sb, int _typeflag, struct FTW* _ftwbuf)
        {
            int rv = remove(_fpath);

            if (rv)
            {
                perror(_fpath);
            }

            return rv;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool DeleteDirectory(Path const& _fullPath)
        {
            nftw(_fullPath.c_str(), DeleteFileHelper, 64, FTW_DEPTH | FTW_PHYS);
            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool MoveRegularFile(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            MAZE_ERROR("Not implemented yet!");
            return false;
        }



        //////////////////////////////////////////
        MAZE_CORE_API bool MoveDirectory(Path const& _fullPath, Path const& _destFullPath)
        {
            MAZE_ERROR("Not implemented yet!");
            return false;
        }


        //////////////////////////////////////////
        MAZE_CORE_API Path ConvertLocalPathToFullPath(Path const& _localPath)
        {
            S8 buff[PATH_MAX + 1];
            if (!realpath(_localPath.c_str(), buff))
            {
                return _localPath;
            }
            
            return buff;
        }
        
        ////////////////////////////////////
        MAZE_CORE_API FileStats GetFileStats(Path const& _fullPath)
        {
            FileStats result;

            struct stat st;
            if (stat(_fullPath.c_str(), &st) == 0)
            {
                result.creationTimeUTC = st.st_ctime;
                result.modifiedTimeUTC = st.st_mtime;
                result.accessedTimeUTC = st.st_atime;
                result.fileSize = st.st_size;
            }

            return result;
        }

    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
