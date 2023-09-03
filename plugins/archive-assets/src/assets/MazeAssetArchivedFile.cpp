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
#include "MazeArchiveAssetsHeader.hpp"
#include "maze-plugin-archive-assets/assets/MazeAssetArchivedFile.hpp"
#include "maze-plugin-archive-assets/assets/MazeAssetRegularArchive.hpp"
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

    //////////////////////////////////////////
    AssetArchivedFilePtr AssetArchivedFile::Create(
        ArchiveFileZipPtr const& _archive,
        Path const& _zipArchiveFilePath)
    {
        AssetArchivedFilePtr result;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetArchivedFile, result, init(_archive, _zipArchiveFilePath));
        return result;
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::init(
        ArchiveFileZipPtr const& _archive,
        Path const& _zipArchiveFilePath)
    {
        m_archive = _archive;
        m_zipArchiveFilePath = _zipArchiveFilePath;
        m_fileName = FileHelper::GetFileNameInPath(_zipArchiveFilePath);
        m_fullPath = _archive->getFullPath() + _zipArchiveFilePath;
        
        return true;
    }

    //////////////////////////////////////////
    Size AssetArchivedFile::getFileSize() const
    {
        return m_archive->getArchivedFileLength(m_zipArchiveFilePath);
    }

    //////////////////////////////////////////
    FileStats AssetArchivedFile::getFileStats() const
    {
        return m_archive->getArchivedFileStats(m_zipArchiveFilePath);
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::isFileExists() const
    {
        return m_archive->isFileExists();
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _renamedFiles)
    {
        MAZE_NOT_IMPLEMENTED;
        return false;
    }

    //////////////////////////////////////////
    UnorderedMap<Path, AssetFilePtr> const* AssetArchivedFile::getChildrenAssets() const
    {
        return nullptr;
    }

    //////////////////////////////////////////
    void AssetArchivedFile::updateChildrenAssets(
        bool _recursive,
        Vector<AssetFilePtr>* _addedFiles,
        Vector<AssetFilePtr>* _removedFiles)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    Size AssetArchivedFile::readToString(String& _string) const
    {
        return m_archive->readArchivedFileToString(m_zipArchiveFilePath, _string);
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::readToXMLDocument(tinyxml2::XMLDocument& _doc) const
    {
        MAZE_PROFILE_EVENT("AssetArchivedFile::readToXMLDocument");

        ByteBufferPtr byteBuffer = m_archive->readArchivedFileAsByteBuffer(m_zipArchiveFilePath);

        if (!byteBuffer)
        {
            MAZE_ERROR("File %s unzipping error!", getFileName().toUTF8().c_str());
            return false;
        }
        else
        {
            tinyxml2::XMLError loadError = _doc.Parse((CString)byteBuffer->getDataPointer(), byteBuffer->getSize());

            if (tinyxml2::XML_SUCCESS != loadError)
            {
                MAZE_ERROR("File '%s' loading error - XMLError: %d!", getFileName().toUTF8().c_str(), (S32)loadError);
                return false;
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::readToByteBuffer(ByteBuffer& _byteBuffer) const
    {
        _byteBuffer.resize(getFileSize());
        return m_archive->readArchivedFileToBuffer(
            m_zipArchiveFilePath,
            _byteBuffer.getDataPointer(),
            _byteBuffer.getSize());
        return false;
    }

    //////////////////////////////////////////
    bool AssetArchivedFile::readHeaderToByteBuffer(ByteBuffer& _byteBuffer, Size _size) const
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
