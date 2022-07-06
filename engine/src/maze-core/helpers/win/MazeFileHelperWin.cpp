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
#include "maze-core/helpers/win/MazeFileHelperWin.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include <ShlObj.h>
#include <cwchar>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool IsFileExists(Path const& _fullPath)
        {
            struct _stat st;

            if (_wstat(_fullPath.c_str(), &st) == 0)
                 return true;

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool IsDirectory(Path const& _fullPath)
        {
            struct _stat st;

            if (_wstat(_fullPath.c_str(), &st) == 0)
            {
                if (st.st_mode & _S_IFDIR)
                {
                    return true;
                }
            }
        
            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API U32 GetFileSize(Path const& _fullPath)
        {
            struct _stat st;

            if (_wstat(_fullPath.c_str(), &st) == 0)
            {
                return st.st_size;
            }

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetFileModificationTimestamp(Path const& _fullPath)
        {
            struct _stat st;

            if (_wstat(_fullPath.c_str(), &st) == 0)
            {
                return st.st_mtime;
            }

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetWorkingDirectory()
        {
            WString result;
            result.resize(MAX_PATH);

            while (true)
            {
                DWORD r = GetCurrentDirectoryW((DWORD)result.size(), &result[0]);
                if (r < result.size() && r != 0)
                {
                    result.resize(r);
                    break;
                }            

                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                    result.resize(result.size() * 2);
                else
                    return Path();
            }

            Path path = std::move(result);
            NormalizeFilePath(path);
        
            return path;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryFullPath()
        {
            WString result;
            result.resize(MAX_PATH);
            HMODULE hModule = GetModuleHandleW(NULL);

            while (true)
            {
                DWORD r = GetModuleFileNameW(hModule, &result[0], MAX_PATH);
                if (r < result.size() && r != 0) 
                {
                    result.resize(r);
                    break;
                }        

                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
                {
                    result.resize(result.size() * 2);
                } 
                else
                {
                    return Path();
                }
            }

            Path path = std::move(result);
            NormalizeFilePath(path);
            return path;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryDirectory()
        {
            return GetDirectoryInPath(GetBinaryFullPath());
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDocumentsDirectory()
        {
#if ((MAZE_ARCH == MAZE_ARCH_X86) || (MAZE_ARCH == MAZE_ARCH_X64))

            WCHAR myDocuments[MAX_PATH];
            HRESULT result = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocuments);

            if (result != S_OK)
                return GetBinaryDirectory();

            return NormalizedFilePath(myDocuments);
#else

            MAZE_TODO;
            return Path();

#endif

        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultTemporaryDirectory()
        {
            return GetBinaryDirectory() + L"/temp";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultLogDirectory()
        {
            return GetBinaryDirectory() + L"/log";
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CreateDirectoryRecursive(Path const& _path)
        {
            Path path = _path;
            NormalizeFilePath(path);

            Path tempLocal;
            Vector<WString> directories;
            StringHelper::SplitWords(path, directories, '/');
            for (S32 i = 0; i < (S32)directories.size(); ++i)
            {
                Path directoryPath = tempLocal + directories[i];

                DWORD pathAttr = GetFileAttributesW(directoryPath.c_str());
                if (pathAttr == INVALID_FILE_ATTRIBUTES || !(pathAttr & FILE_ATTRIBUTE_DIRECTORY))
                {
                    if (!CreateDirectoryW(directoryPath.c_str(), NULL))
                        return false;
                }

                tempLocal += directories[i] + L'/';
            }

            return true;
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API Vector<Path> GetRegularFileNamesInPath(Path const& _localPath)
        {
            Vector<Path> result;

            Path fullPath = ConvertLocalPathToFullPath(_localPath);

            WIN32_FIND_DATAW f;
            HANDLE h = FindFirstFileW((fullPath + L"/*").c_str(), &f);
            if (h != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if (wcscmp(f.cFileName, L".") == 0)
                        continue;

                    if (wcscmp(f.cFileName, L"..") == 0)
                        continue;

                    result.push_back(f.cFileName);
                } 
                while (FindNextFileW(h, &f));
            }

            FindClose(h);
        
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CopyRegularFile(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            CHAR buf[BUFSIZ];
            Size size;

            FILE* source = StdHelper::OpenFile(_sourceFullPath, "rb");
            if (!source)
                return false;

            CreateDirectoryRecursive(FileHelper::GetDirectoryInPath(_destFullPath));

            FILE* dest = StdHelper::OpenFile(_destFullPath, "wb");
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
            WIN32_FIND_DATAW info;
            HANDLE handle;
            WChar temp[MAX_PATH] = L"";
            WChar temp1[MAX_PATH] = L"";
    
            WChar tempSrc[MAX_PATH];
            WChar tempDest[MAX_PATH];

            Path srcDirectory = NormalizedFilePath(_sourceFullPath);
            if (srcDirectory.back() != '/')
                srcDirectory += '/';

            Path destDirectory = NormalizedFilePath(_destFullPath);
            if (destDirectory.back() != '/')
                destDirectory += '/';
        


            wcscpy_s(temp1, srcDirectory.c_str());
            wcscat_s(temp1, L"/*.*");
    
    
            CreateDirectoryRecursive(destDirectory.c_str());

            handle = FindFirstFileW(temp1, &info);
            do
            {
                if (!wcscmp(info.cFileName, L"."))
                    continue;

                if (!wcscmp(info.cFileName, L".."))
                    continue;

                wcscpy_s(tempSrc, srcDirectory.c_str());
                wcscat_s(tempSrc, L"/");
                wcscat_s(tempSrc, info.cFileName);
        
                if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    wcscpy_s(tempDest, destDirectory.c_str());
                    wcscat_s(tempDest, L"/");
                    wcscat_s(tempDest, info.cFileName);
                
                    CopyDirectory(tempSrc, tempDest);
                }
                else
                {
                    wcscpy_s(temp, destDirectory.c_str());
                    wcscat_s(temp, L"/");
                    wcscat_s(temp, info.cFileName);
    
                    Char buf[BUFSIZ];
                    Size size;

                    FILE* source = StdHelper::OpenFile(tempSrc, "rb");
                    if (!source)
                        return false;

                    FILE* dest = StdHelper::OpenFile(temp, "wb");
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
                }
        
            }
            while (FindNextFileW(handle, &info));

            FindClose(handle);

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool DeleteRegularFile(Path const& _fileFullPath)
        {
            if (_wremove(_fileFullPath.c_str()) != 0)
                return false;

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool DeleteDirectory(Path const& _path)
        {
            WIN32_FIND_DATAW info;
            HANDLE handle;
            WChar temp[MAX_PATH] = L"";
            WChar temp1[MAX_PATH] = L"";
    
            WChar tempSrc[MAX_PATH];

            Path srcDirectory = NormalizedFilePath(_path);

            wcscpy_s(temp1, srcDirectory.c_str());
            wcscat_s(temp1, L"/*.*");
    

            handle = FindFirstFileW(temp1, &info);
            do
            {
                if (!wcscmp(info.cFileName, L"."))
                    continue;

                if (!wcscmp(info.cFileName, L".."))
                    continue;

                wcscpy_s(tempSrc, srcDirectory.c_str());
                wcscat_s(tempSrc, L"/");
                wcscat_s(tempSrc, info.cFileName);
        
                if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    DeleteDirectory(tempSrc);
            
                }
                else
                {
                    DeleteRegularFile(tempSrc);
                }
        
            }
            while (FindNextFileW(handle, &info));

            FindClose(handle);


            bool result = (RemoveDirectoryW(srcDirectory.c_str()) == TRUE);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool MoveRegularFile(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            return MoveFileW(_sourceFullPath.c_str(), _destFullPath.c_str());
        }


        //////////////////////////////////////////
        MAZE_CORE_API bool MoveDirectory(Path const& _sourceFullPath, Path const& _destFullPath)
        {
            WIN32_FIND_DATAW info;
            HANDLE handle;
            WChar temp[MAX_PATH] = L"";
            WChar temp1[MAX_PATH] = L"";
    
            WChar tempSrc[MAX_PATH];
            WChar tempDest[MAX_PATH];

            Path srcDirectory = NormalizedFilePath(_sourceFullPath);
            if (srcDirectory.back() != '/')
                srcDirectory += '/';

            Path destDirectory = NormalizedFilePath(_destFullPath);
            if (destDirectory.back() != '/')
                destDirectory += '/';

            wcscpy_s(temp1, srcDirectory.c_str());
            wcscat_s(temp1, L"/*.*");
    
    
            CreateDirectoryRecursive(destDirectory.c_str());

            handle = FindFirstFileW(temp1, &info);
            do
            {
                if (!wcscmp(info.cFileName , L"."))
                    continue;

                if (!wcscmp(info.cFileName , L".."))
                    continue;

                wcscpy_s(tempSrc, srcDirectory.c_str());
                wcscat_s(tempSrc, L"/");
                wcscat_s(tempSrc, info.cFileName);
        
                if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    wcscpy_s(tempDest, destDirectory.c_str());
                    wcscat_s(tempDest, L"/");
                    wcscat_s(tempDest, info.cFileName);
                
                    MoveDirectory(tempSrc, tempDest);
            
                }
                else
                {
                    wcscpy_s(temp, destDirectory.c_str());
                    wcscat_s(temp, L"/");
                    wcscat_s(temp, info.cFileName);

                    MoveRegularFile(tempSrc, temp);
                }
        
            }
            while (FindNextFileW(handle, &info));

            FindClose(handle);

            RemoveDirectoryW(srcDirectory.c_str());

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path ConvertLocalPathToFullPath(Path const& _localPath)
        {
            U32 size = MAX_PATH;
            WString buffer;

            while (true)
            {
                buffer.resize(size + 1);
                DWORD r = GetFullPathNameW(_localPath.c_str(), size, &buffer[0], nullptr);
                if (r < size && r != 0) 
                {
                    buffer.resize(r);
                    break;
                }

                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
                {
                    size <<= 1;
                } 
                else
                {
                    return Path();
                }
            }

            Path path = std::move(buffer);
            NormalizeFilePath(path);
        
            return path;
        }

        ////////////////////////////////////
        MAZE_CORE_API FileStats GetFileStats(Path const& _fileFullPath)
        {
            FileStats result;

            struct _stat st;
            if (_wstat(_fileFullPath.c_str(), &st) == 0)
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
