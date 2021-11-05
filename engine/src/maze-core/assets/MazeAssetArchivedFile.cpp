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
#include "maze-core/assets/MazeAssetArchivedFile.hpp"
#include "maze-core/assets/MazeAssetRegularArchive.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetFile
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetArchivedFile, AssetFile);
    
    //////////////////////////////////////////
    AssetArchivedFile::AssetArchivedFile()
    {

    }

    //////////////////////////////////////////
    AssetArchivedFile::~AssetArchivedFile()
    {
    }

    ////////////////////////////////////
    AssetArchivedFilePtr AssetArchivedFile::Create(
        ArchiveFileZipPtr const& _archive,
        String const& _zipArchiveFilePath)
    {
        AssetArchivedFilePtr result;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetArchivedFile, result, init(_archive, _zipArchiveFilePath));
        return result;
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::init(
        ArchiveFileZipPtr const& _archive,
        String const& _zipArchiveFilePath)
    {
        m_archive = _archive;
        m_zipArchiveFilePath = _zipArchiveFilePath;
        m_fileName = FileHelper::GetFileNameInPath(_zipArchiveFilePath);
        m_fullPath = _archive->getFullPath() + _zipArchiveFilePath;
        
        return true;
    }

    //////////////////////////////////////////
    Size AssetArchivedFile::getFileSize()
    {
        return m_archive->getArchivedFileLength(m_zipArchiveFilePath);
    }

    //////////////////////////////////////////
    UnorderedMap<String, AssetFilePtr> const* AssetArchivedFile::getChildrenAssets() const
    {
        return nullptr;
    }

    //////////////////////////////////////////
    Size AssetArchivedFile::readToString(String& _string)
    {
        return m_archive->readArchivedFileToString(m_zipArchiveFilePath, _string);
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::readToXMLDocument(tinyxml2::XMLDocument& _doc)
    {
        ByteBufferPtr byteBuffer = m_archive->readArchivedFileAsByteBuffer(m_zipArchiveFilePath);

        if (!byteBuffer)
        {
            MAZE_ERROR("File %s unzipping error!", getFileName().c_str());
            return false;
        }
        else
        {
            tinyxml2::XMLError loadError = _doc.Parse((CString)byteBuffer->getDataPointer(), byteBuffer->getSize());

            if (tinyxml2::XML_SUCCESS != loadError)
            {
                MAZE_ERROR("File '%s' loading error - XMLError: %d!", getFileName().c_str(), (S32)loadError);
                return false;
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::readToByteBuffer(ByteBuffer& _byteBuffer)
    {
        _byteBuffer.resize(getFileSize());
        return m_archive->readArchivedFileToBuffer(
            m_zipArchiveFilePath,
            _byteBuffer.getDataPointer(),
            _byteBuffer.getSize());
        return false;
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size)
    {
        _byteBuffer.resize(_size);
        return m_archive->readArchivedFileToBuffer(
            m_zipArchiveFilePath,
            _byteBuffer.getDataPointer(),
            _byteBuffer.getSize());
        return false;
    }

} // namespace Maze
//////////////////////////////////////////
