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
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void NormalizeFilePath(Path& _path) noexcept
        {
            if (_path.empty())
                return;

            Path::StringType string = _path.getPath();
            std::replace(string.begin(), string.end(), '\\', '/');
            string.erase(string.find_last_not_of('/') + 1);
            static CString searchPath = "/./";
            static CString replacePath = "/";
            static Path::StringType const search = Path::StringType((Char*)searchPath, (Char*)searchPath + strlen(searchPath));
            static Path::StringType const replace = Path::StringType((Char*)replacePath, (Char*)replacePath + strlen(replacePath));
            StringHelper::ReplaceSubstring(string, search, replace);
            _path = string;
        }


        //////////////////////////////////////////
        MAZE_CORE_API Path NormalizedFilePath(Path const& _path) noexcept
        {
            Path result = _path;
            NormalizeFilePath(result);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDirectoryInPath(Path const& _path) noexcept
        {
            if (IsDirectory(_path))
            {
                return _path;
            }

            Size pos = _path.getPath().find_last_of('/');

            if (pos == Path::StringType::npos)
                return Path();

            return _path.getPath().substr(0, pos);
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetParentDirectoryInPath(Path const& _path) noexcept
        {
            Path directory = GetDirectoryInPath(_path);

            if (!IsDirectory(_path))
                return directory;

            Size pos = _path.getPath().find_last_of('/');

            if (pos == Path::StringType::npos)
                return Path();

            return _path.getPath().substr(0, pos);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API Path GetFileNameInPath(Path const& _path) noexcept
        {
            Size pos = _path.getPath().find_last_of('/');
            if (pos == Path::StringType::npos)
            {
                return _path;
            }
            
            return _path.getPath().substr(pos + 1, _path.size() - pos - 1);
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetFileExtension(Path const& _fileFullPath)
        {
            Path fileName;
            Size position = _fileFullPath.getPath().find_last_of('/');
            if (position != Path::StringType::npos)
                fileName = _fileFullPath.getPath().substr(position + 1, _fileFullPath.size() - position - 1);
            else
                fileName = _fileFullPath;

            position = fileName.getPath().find_last_of('.');
            if (position == Path::StringType::npos)
                return Path();

            return fileName.getPath().substr(position + 1, fileName.size() - position - 1);
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetFileNameWithoutExtension(Path const& _fileFullPath)
        {
            Path fileName;
            Size position =  _fileFullPath.getPath().find_last_of('/');
            if (position != Path::StringType::npos)
                fileName = _fileFullPath.getPath().substr(position + 1, _fileFullPath.size() - position - 1);
            else
                fileName = _fileFullPath;
        
            position =  fileName.getPath().find_last_of('.');
            if (position == Path::StringType::npos)
                return fileName;
        
            return fileName.getPath().substr(0, position);
        }

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToString(Path const& _fileFullPath, String& _string)
        {
            MAZE_PROFILE_EVENT("ReadFileToString");

            std::ifstream ifs(_fileFullPath.c_str());
            _string.assign(
                (std::istreambuf_iterator<S8>(ifs)),
                (std::istreambuf_iterator<S8>()));
        
            return _string.size();
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ReadFileAsString(Path const& _fileFullPath)
        {
            MAZE_PROFILE_EVENT("ReadFileAsString");

            String result;
            ReadFileToString(_fileFullPath, result);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToByteBuffer(Path const& _fileFullPath, ByteBuffer& _byteBuffer)
        {
            MAZE_PROFILE_EVENT("ReadFileToByteBuffer");

            InputFileStream file;
            file.open(_fileFullPath.c_str(), std::ios::binary | std::ios::ate);
            if (!file.is_open())
                return 0;
        
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
        
            _byteBuffer.resize((Size)size);
        
            file.read((S8*)_byteBuffer.getDataRW(), size);
        
            return (Size)size;
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API ByteBufferPtr ReadFileAsByteBuffer(Path const& _fileFullPath)
        {
            MAZE_PROFILE_EVENT("ReadFileAsByteBuffer");

            ByteBufferPtr byteBuffer = ByteBuffer::Create();
            ReadFileToByteBuffer(_fileFullPath, (*byteBuffer.get()));
            return byteBuffer; 
        }

    } // namespace FileHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
