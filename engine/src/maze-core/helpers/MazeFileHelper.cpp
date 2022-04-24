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
        MAZE_CORE_API void NormalizeFilePath(String& _path) noexcept
        {
            if (_path.empty())
                return;

            std::replace(_path.begin(), _path.end(), '\\', '/');
            _path.erase(_path.find_last_not_of("/") + 1);

            StringHelper::ReplaceSubstring(_path, "/./", "/");
        }

        //////////////////////////////////////////
        MAZE_CORE_API void NormalizeFilePath(WString& _path) noexcept
        {
            if (_path.empty())
                return;

            std::replace(_path.begin(), _path.end(), L'\\', L'/');
            _path.erase(_path.find_last_not_of(L"/") + 1);

        }

        //////////////////////////////////////////
        MAZE_CORE_API String NormalizedFilePath(String const& _path) noexcept
        {
            String result = _path;
            NormalizeFilePath(result);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetDirectoryInPath(String const& _path) noexcept
        {
            if (IsDirectory(_path))
            {
                return _path;
            }

            Size pos = String(_path).find_last_of('/');

            if (pos == String::npos)
                return String();

            return _path.substr(0, pos);            
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetParentDirectoryInPath(String const& _path) noexcept
        {
            String directory = GetDirectoryInPath(_path);

            if (!IsDirectory(_path))
                return directory;

            Size pos = String(_path).find_last_of('/');

            if (pos == String::npos)
                return String();

            return _path.substr(0, pos);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API String GetFileNameInPath(String const& _path) noexcept
        {
            Size pos = _path.find_last_of('/');
            if (pos == String::npos)
            {
                return _path;
            }
            
            return _path.substr(pos + 1, _path.size() - pos - 1);
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetFileExtension(String const& _fileFullPath)
        {
            String fileName;
            Size position = _fileFullPath.find_last_of('/');
            if (position != String::npos)
                fileName = _fileFullPath.substr(position + 1, _fileFullPath.size() - position - 1);
            else
                fileName = _fileFullPath;

            position = fileName.find_last_of('.');
            if (position == String::npos)
                return String();

            return fileName.substr(position + 1, fileName.size() - position - 1);
        }

        //////////////////////////////////////////
        MAZE_CORE_API String GetFileNameWithoutExtension(String const& _fileFullPath)
        {
            String fileName;
            Size position =  _fileFullPath.find_last_of('/');
            if (position != String::npos)
                fileName = _fileFullPath.substr(position + 1, _fileFullPath.size() - position - 1);
            else
                fileName = _fileFullPath;
        
            position =  fileName.find_last_of('.');
            if (position == String::npos)
                return fileName;
        
            return fileName.substr(0, position);
        }

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToString(CString _fileFullPath, String& _string)
        {
            std::ifstream ifs(_fileFullPath);
            _string.assign(
                (std::istreambuf_iterator<S8>(ifs)),
                (std::istreambuf_iterator<S8>()));
        
            return _string.size();
        }

        //////////////////////////////////////////
        MAZE_CORE_API String ReadFileAsString(CString _fileFullPath)
        {
            String result;
            ReadFileToString(_fileFullPath, result);
            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Size ReadFileToByteBuffer(CString _fileFullPath, ByteBuffer& _byteBuffer)
        {
            InputFileStream file;
            file.open(_fileFullPath, std::ios::binary | std::ios::ate);
            if (!file.is_open())
                return 0;
        
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
        
            _byteBuffer.resize((Size)size);
        
            file.read((S8*)_byteBuffer.getDataPointer(), size);
        
            return (Size)size;
        }
    
        //////////////////////////////////////////
        MAZE_CORE_API ByteBufferPtr ReadFileAsByteBuffer(CString _fileFullPath)
        {
            ByteBufferPtr byteBuffer = ByteBuffer::Create();
            ReadFileToByteBuffer(_fileFullPath, (*byteBuffer.get()));
            return byteBuffer; 
        }

    } // namespace FileHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
