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
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeXMLHelper.hpp"



//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetRegularFile
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetRegularFile, AssetFile);
    
    //////////////////////////////////////////
    AssetRegularFile::AssetRegularFile()
    {

    }

    //////////////////////////////////////////
    AssetRegularFile::~AssetRegularFile()
    {
    }

    //////////////////////////////////////////
    AssetRegularFilePtr AssetRegularFile::Create(Path const& _fullPath)
    {
        AssetRegularFilePtr result;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetRegularFile, result, init(_fullPath));
        return result;
    }

    //////////////////////////////////////////
    bool AssetRegularFile::init(Path const& _fullPath, bool _normalizePath)
    {
        if (!AssetFile::init())
            return false;
        
        setFullPath(_fullPath, _normalizePath);

        return true;
    }
    
    //////////////////////////////////////////
    void AssetRegularFile::setFullPath(Path const& _fullPath, bool _normalizePath)
    {
        m_fullPath = _fullPath;
        
        if (_normalizePath)
        {
            m_fullPath = FileHelper::NormalizedFilePath(m_fullPath);
        }
        
        m_fileName = FileHelper::GetFileNameInPath(m_fullPath);
    }

    //////////////////////////////////////////
    Size AssetRegularFile::getFileSize() const
    {
        return getFileStats().fileSize;
    }

    //////////////////////////////////////////
    FileStats AssetRegularFile::getFileStats() const
    {
        return FileHelper::GetFileStats(m_fullPath);
    }

    //////////////////////////////////////////
    bool AssetRegularFile::isFileExists() const
    {
        return FileHelper::IsFileExists(m_fullPath);
    }

    //////////////////////////////////////////
    bool AssetRegularFile::move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _movedFiles)
    {
        MAZE_PROFILE_EVENT("AssetRegularFile::move");

        if (m_fullPath == _newFullPath)
            return false;

        bool isFullPathExists = FileHelper::IsFileExists(m_fullPath);
        bool isNewFullPathExists = FileHelper::IsFileExists(_newFullPath);

        MAZE_WARNING_RETURN_VALUE_IF(isFullPathExists && isNewFullPathExists, false, "File is already exists - %s!", _newFullPath.toUTF8().c_str());
        MAZE_WARNING_RETURN_VALUE_IF(!isFullPathExists && !isNewFullPathExists, false, "File is not exists - %s!", m_fullPath.toUTF8().c_str());

        if (isFullPathExists && !isNewFullPathExists)
        {
            if (!FileHelper::Move(m_fullPath, _newFullPath))
                return false;
        }
        
        _movedFiles.push_back(Pair<Path, AssetFilePtr>(m_fullPath, getSharedPtr()));
        setFullPath(_newFullPath);

        return true;
    }

    //////////////////////////////////////////
    Size AssetRegularFile::calculateFileSize() const
    {
        FILE* fileHandler = StdHelper::OpenFile(m_fullPath, "rb");
        if (!fileHandler)
            return 0;

        Size size = 0;

        U32 prevPos = ftell(fileHandler);
        fseek(fileHandler, 0, SEEK_END);
        size = ftell(fileHandler);
        fseek(fileHandler, prevPos, SEEK_SET);

        fclose(fileHandler);

        return size;
    }
    
    //////////////////////////////////////////
    Size AssetRegularFile::readToString(String& _string) const
    {
        FileHelper::ReadFileToString(m_fullPath, _string);
        return _string.size();
    }

    //////////////////////////////////////////
    bool AssetRegularFile::readToXMLDocument(tinyxml2::XMLDocument& _doc) const
    {
        MAZE_PROFILE_EVENT("AssetRegularFile::readToXMLDocument");

        tinyxml2::XMLDocument doc;
        tinyxml2::XMLError loadError = XMLHelper::LoadXMLFile(getFullPath(), _doc);
        if (tinyxml2::XML_SUCCESS != loadError)
        {
            MAZE_WARNING("File '%s' loading error - XMLError: %d!", getFileName().toUTF8().c_str(), (S32)loadError);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    bool AssetRegularFile::readToByteBuffer(ByteBuffer& _byteBuffer) const
    {
        MAZE_PROFILE_EVENT("AssetRegularFile::readToByteBuffer");

        FILE* fileHandler = StdHelper::OpenFile(m_fullPath, "rb");
        if (!fileHandler)
            return false;

        Size size = 0;

        fseek(fileHandler, 0, SEEK_END);
        size = ftell(fileHandler);
        fseek(fileHandler, 0, SEEK_SET);

        _byteBuffer.resize(size);
        if (size > 0)
        {
            U8* dataPointer = _byteBuffer.getDataRW();
            Size totalBytesRead = 0;
            while (!feof(fileHandler))
            {
                Size bytesRead = fread(dataPointer, sizeof(U8), 1024, fileHandler);
                totalBytesRead += bytesRead;
                dataPointer += bytesRead;
            }
        }

        fclose(fileHandler);

        return true;
    }

    //////////////////////////////////////////
    bool AssetRegularFile::readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size) const
    {
        MAZE_PROFILE_EVENT("AssetRegularFile::readHeaderToByteBuffer");

        FILE* fileHandler = StdHelper::OpenFile(m_fullPath, "rb");
        if (!fileHandler)
            return false;

        _byteBuffer.resize(_size);
        if (_size > 0)
            fread(_byteBuffer.getDataRW(), sizeof(U8), _size, fileHandler);

        fclose(fileHandler);

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
