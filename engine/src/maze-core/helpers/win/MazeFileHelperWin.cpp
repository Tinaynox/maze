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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool IsFileExists(CString _fullPath)
        {
            struct _stat st;

            if (_stat(_fullPath, &st) == 0)
                 return true;

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool IsDirectory(CString _fullPath)
        {
            struct _stat st;

            if (_stat(_fullPath, &st) == 0)
            {
                if (st.st_mode & _S_IFDIR)
                {
                    return true;
                }
            }
        
            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API U32 GetFileSize(CString _fullPath)
        {
            struct _stat st;

            if (_stat(_fullPath, &st) == 0)
            {
                return st.st_size;
            }

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetFileModificationTimestamp(CString _fullPath)
        {
            struct _stat st;

            if (_stat(_fullPath, &st) == 0)
            {
                return st.st_mtime;
            }

            return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetWorkingDirectory()
        {
            String result;
            result.resize(MAX_PATH);

            while (true)
            {
                DWORD r = GetCurrentDirectoryA((DWORD)result.size(), &result[0]);
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
                    return String();
                }
            }

            NormalizeFilePath(result);
        
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API WString GetWorkingDirectoryW()
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
                {
                    result.resize(result.size() * 2);
                } 
                else
                {
                    return WString();
                }
            }

            NormalizeFilePath(result);
        
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetBinaryFullPath()
        {
            String result;
            result.resize(MAX_PATH);
            HMODULE hModule = GetModuleHandleA(NULL);

            while (true)
            {
                DWORD r = GetModuleFileNameA(hModule, &result[0], MAX_PATH);
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
                    return String();
                }
            }

            NormalizeFilePath(result);

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
#if ((MAZE_ARCH == MAZE_ARCH_X86) || (MAZE_ARCH == MAZE_ARCH_X64))

            CHAR myDocuments[MAX_PATH];
            HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocuments);

            if (result != S_OK)
                return GetBinaryDirectory();

            return NormalizedFilePath(myDocuments);
#else

            MAZE_TODO;
            return String();

#endif

        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetDefaultTemporaryDirectory()
        {
            return GetBinaryDirectory() + "/temp";
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetDefaultLogDirectory()
        {
            return GetBinaryDirectory() + "/log";
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CreateDirectoryRecursive(CString _path)
        {
            String path = _path;
            NormalizeFilePath(path);

            String tempLocal;
            Vector<String> directories;
            StringHelper::SplitWords(path, directories, '/');
            for (S32 i = 0; i < (S32)directories.size(); ++i)
            {
                String directoryPath = tempLocal + directories[i];

                DWORD pathAttr = GetFileAttributesA(directoryPath.c_str());
                if (pathAttr == INVALID_FILE_ATTRIBUTES || !(pathAttr & FILE_ATTRIBUTE_DIRECTORY))
                {
                    if (!CreateDirectory(directoryPath.c_str(), NULL))
                        return false;
                }

                tempLocal += directories[i] + '/';
            }

            return true;
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API Vector<String> GetRegularFileNamesInPath(CString _localPath)
        {
            Vector<String> result;

            String fullPath = ConvertLocalPathToFullPath(_localPath);

            WIN32_FIND_DATA f;
            HANDLE h = FindFirstFile((fullPath + "/*").c_str(), &f);
            if (h != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if (strcmp(f.cFileName, ".") == 0)
                        continue;

                    if (strcmp(f.cFileName, "..") == 0)
                        continue;

                    result.push_back(f.cFileName);
                } 
                while (FindNextFile(h, &f));
            }

            FindClose(h);
        
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool CopyRegularFile(CString _sourceFullPath, CString _destFullPath)
        {
            char buf[BUFSIZ];
            Size size;

            FILE* source = StdHelper::OpenFile(_sourceFullPath, "rb");
            if (!source)
                return false;

            CreateDirectoryRecursive(FileHelper::GetDirectoryInPath(_destFullPath).c_str());

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
        MAZE_CORE_API bool CopyDirectory(CString _sourceFullPath, CString _destFullPath)
        {
            WIN32_FIND_DATA info;
            HANDLE handle;
            S8 temp[MAX_PATH] = "";
            S8 temp1[MAX_PATH] = "";
    
            S8 tempSrc[MAX_PATH];
            S8 tempDest[MAX_PATH];

            String srcDirectory = NormalizedFilePath(_sourceFullPath);
            if (srcDirectory.back() != '/')
                srcDirectory += '/';

            String destDirectory = NormalizedFilePath(_destFullPath);
            if (destDirectory.back() != '/')
                destDirectory += '/';
        


            strcpy_s(temp1, srcDirectory.c_str());
            strcat_s(temp1, "/*.*");
    
    
            CreateDirectoryRecursive(destDirectory.c_str());

            handle = FindFirstFile(temp1, &info);
            do
            {
                if (!strcmp(info.cFileName, ".")) 
                    continue;

                if (!strcmp(info.cFileName, ".."))
                    continue;

                strcpy_s(tempSrc, srcDirectory.c_str());
                strcat_s(tempSrc, "/");
                strcat_s(tempSrc, info.cFileName);
        
                if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    strcpy_s(tempDest, destDirectory.c_str());
                    strcat_s(tempDest, "/");
                    strcat_s(tempDest, info.cFileName);
                
                    CopyDirectory(tempSrc, tempDest);
                }
                else
                {
                    strcpy_s(temp, destDirectory.c_str());
                    strcat_s(temp, "/");
                    strcat_s(temp, info.cFileName);
    
                    char buf[BUFSIZ];
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
            while (FindNextFile(handle, &info));

            FindClose(handle);

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool DeleteRegularFile(CString _fileFullPath)
        {
            if (std::remove(_fileFullPath) != 0)
                return false;

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool DeleteDirectory(CString _path)
        {
            WIN32_FIND_DATA info;
            HANDLE handle;
            S8 temp[MAX_PATH] = "";
            S8 temp1[MAX_PATH] = "";
    
            S8 tempSrc[MAX_PATH];

            String srcDirectory = NormalizedFilePath(_path);

            strcpy_s(temp1, srcDirectory.c_str());
            strcat_s(temp1, "/*.*");
    

            handle = FindFirstFile(temp1, &info);
            do
            {
                if (!strcmp(info.cFileName, ".")) 
                    continue;

                if (!strcmp(info.cFileName, ".."))
                    continue;

                strcpy_s(tempSrc, srcDirectory.c_str());
                strcat_s(tempSrc, "/");
                strcat_s(tempSrc, info.cFileName);
        
                if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    DeleteDirectory(tempSrc);
            
                }
                else
                {
                    DeleteRegularFile(tempSrc);
                }
        
            }
            while (FindNextFile(handle, &info));

            FindClose(handle);


            bool result = (RemoveDirectory(srcDirectory.c_str()) == TRUE);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool MoveRegularFile(CString _sourceFullPath, CString _destFullPath)
        {
            /*
            if (!CopyRegularFile(_sourceFullPath, _destFullPath))
                return false;

            if (!DeleteRegularFile(_sourceFullPath))
                return false;

            return true;
            */
            return MoveFileA(_sourceFullPath, _destFullPath);
        }


        //////////////////////////////////////////
        MAZE_CORE_API bool MoveDirectory(CString _sourceFullPath, CString _destFullPath)
        {
            WIN32_FIND_DATA info;
            HANDLE handle;
            S8 temp[MAX_PATH] = "";
            S8 temp1[MAX_PATH] = "";
    
            S8 tempSrc[MAX_PATH];
            S8 tempDest[MAX_PATH];

            String srcDirectory = NormalizedFilePath(_sourceFullPath);
            if (srcDirectory.back() != '/')
                srcDirectory += '/';

            String destDirectory = NormalizedFilePath(_destFullPath);
            if (destDirectory.back() != '/')
                destDirectory += '/';

            strcpy_s(temp1, srcDirectory.c_str());
            strcat_s(temp1, "/*.*");
    
    
            CreateDirectoryRecursive(destDirectory.c_str());

            handle = FindFirstFile(temp1, &info);
            do
            {
                if (!strcmp(info.cFileName , ".")) 
                    continue;

                if (!strcmp(info.cFileName , ".."))
                    continue;

                strcpy_s(tempSrc, srcDirectory.c_str());
                strcat_s(tempSrc, "/");
                strcat_s(tempSrc, info.cFileName);
        
                if (info.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    strcpy_s(tempDest, destDirectory.c_str());
                    strcat_s(tempDest, "/");
                    strcat_s(tempDest, info.cFileName);
                
                    MoveDirectory(tempSrc, tempDest);
            
                }
                else
                {
                    strcpy_s(temp, destDirectory.c_str());
                    strcat_s(temp, "/");
                    strcat_s(temp, info.cFileName);

                    MoveRegularFile(tempSrc, temp);
                }
        
            }
            while (FindNextFile(handle, &info));

            FindClose(handle);

            RemoveDirectory(srcDirectory.c_str());

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ConvertLocalPathToFullPath(CString _localPath)
        {
            U32 size = MAX_PATH;
            String buffer;

            while (true)
            {
                buffer.resize(size + 1);
                DWORD r = GetFullPathNameA(_localPath, size, &buffer[0], nullptr);
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
                    return String();
                }
            }

            NormalizeFilePath(buffer);
        
            return buffer;
        }

        ////////////////////////////////////
        MAZE_CORE_API FileStats GetFileStats(CString _fileFullPath)
        {
            FileStats result;

            struct _stat st;
            if (_stat(_fileFullPath, &st) == 0)
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
