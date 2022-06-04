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
#include "maze-plugin-archive-assets/assets/MazeArchiveFileZip.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ArchiveFileZip
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ArchiveFileZip, Object);
    
    //////////////////////////////////////////
    ArchiveFileZip::ArchiveFileZip()
    {

    }

    //////////////////////////////////////////
    ArchiveFileZip::~ArchiveFileZip()
    {
        closeZip();
    }
    

    //////////////////////////////////////////
    bool ArchiveFileZip::init(String const& _fullPath)
    {
        m_fullPath = _fullPath;

        if (!openZip(_fullPath))
            return false;
      
        return true;
    }

    //////////////////////////////////////////
    ArchiveFileZipPtr ArchiveFileZip::Create(String const& _fullPath)
    {
        ArchiveFileZipPtr result;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ArchiveFileZip, result, init(_fullPath));
        return result;
    }

    //////////////////////////////////////////
    bool ArchiveFileZip::openZip(String const& _fullPath)
    {
        closeZip();

        MAZE_MUTEX_SCOPED_LOCK(m_zipMutex);

        m_zipHandle = unzOpen(_fullPath.c_str());
        MAZE_ERROR_RETURN_VALUE_IF(!m_zipHandle, false, "%s is cannot be opened as zip archive!", _fullPath.c_str());

        m_zipNavigationMapDirty = true;

        return true;
    }

    //////////////////////////////////////////
    void ArchiveFileZip::closeZip()
    {
        if (!m_zipHandle)
            return;

        unzClose(m_zipHandle);
        m_zipHandle = nullptr;
    }

    //////////////////////////////////////////
    bool ArchiveFileZip::updateZipNavigationMap()
    {
        if (!m_zipNavigationMapDirty)
            return true;

        String archiveFileName = FileHelper::GetFileNameInPath(getFullPath());
        Debug::Log("Creating navigation map for zip archive: %s...", archiveFileName.c_str());

        m_zipNavigationMap.clear();

        S8 currentFileName[512];
        String fileName;
        String currentFilePathNormalized;
        
        S32 err = unzGoToFirstFile(m_zipHandle);

        while (err == UNZ_OK)
        {
            unz_file_info fileInfo;
            memset(&fileInfo, 0, sizeof(unz_file_info));

            if (unzGetCurrentFileInfo(m_zipHandle, &fileInfo, currentFileName, sizeof(currentFileName) - 1, nullptr, 0, nullptr, 0) != UNZ_OK)
            {
                MAZE_ERROR("unzGetCurrentFileInfo isn't OK!");
                return false;
            }

            currentFilePathNormalized = currentFileName;
            FileHelper::NormalizeFilePath(currentFilePathNormalized);
            fileName = FileHelper::GetFileNameInPath(currentFilePathNormalized);

            ZipFileInfo& zipFileInfo = m_zipNavigationMap[currentFilePathNormalized];
            zipFileInfo.fileName = fileName;
            zipFileInfo.fullPath = currentFilePathNormalized;
            zipFileInfo.uncompressedSize = fileInfo.uncompressed_size;
            unzGetFilePos(m_zipHandle, &zipFileInfo.filePos);

            err = unzGoToNextFile(m_zipHandle);
        }

        m_zipNavigationMapDirty = false;

        Debug::Log("Navigation map created.");

        return true;
    }

    //////////////////////////////////////////
    Vector<String> ArchiveFileZip::getArchivedFilePathes()
    {
        Vector<String> result;

        updateZipNavigationMap();
        for (ZipNavigationMap::const_iterator it = m_zipNavigationMap.begin(),
                                              end = m_zipNavigationMap.end();
                                              it != end;
                                              ++it)
        {
            result.emplace_back(it->second.fullPath);
        }

        return result;
    }

    //////////////////////////////////////////
    Size ArchiveFileZip::readArchivedFileToBuffer(String const& _filePath, U8* _bytes, Size _bufferSize)
    {
        updateZipNavigationMap();

        ZipNavigationMap::const_iterator it = m_zipNavigationMap.find(_filePath);
        MAZE_WARNING_RETURN_VALUE_IF(it == m_zipNavigationMap.end(), 0, "Cannot locate file in archive '%s'!", _filePath.c_str());
        const ZipFileInfo& zipFileInfo = it->second;
        Size bytesLoaded = 0;

        MAZE_MUTEX_SCOPED_LOCK(m_zipMutex);

        unzGoToFilePos(m_zipHandle, const_cast<unz_file_pos*>(&zipFileInfo.filePos));
        if (tryUnzOpenCurrentFile(zipFileInfo.fileName, m_zipHandle) == UNZ_OK)
        {
            S32 readBytes = unzReadCurrentFile(m_zipHandle, _bytes, uint32_t(_bufferSize));
            if (readBytes <= 0)
            {
                MAZE_ERROR("unzReadCurrentFile isn't OK! error=%d", readBytes);
            }
            else
            {
                bytesLoaded = (Size)readBytes;
            }

        }
        else
        {
            MAZE_ERROR("tryUnzOpenCurrentFile isn't OK!");
        }

        return bytesLoaded;
    }

    //////////////////////////////////////////
    Size ArchiveFileZip::readArchivedFileToString(String const& _filePath, String& _stringBuffer)
    {
        updateZipNavigationMap();

        ZipNavigationMap::const_iterator it = m_zipNavigationMap.find(_filePath);
        MAZE_WARNING_RETURN_VALUE_IF(it == m_zipNavigationMap.end(), 0, "Cannot locate file in archive '%s'!", _filePath.c_str());
        const ZipFileInfo& zipFileInfo = it->second;
        Size bytesLoaded = 0;

        MAZE_MUTEX_SCOPED_LOCK(m_zipMutex);

        unzGoToFilePos(m_zipHandle, const_cast<unz_file_pos*>(&zipFileInfo.filePos));
        if (tryUnzOpenCurrentFile(zipFileInfo.fileName, m_zipHandle) == UNZ_OK)
        {
            _stringBuffer.resize(zipFileInfo.uncompressedSize);

            S32 readBytes = unzReadCurrentFile(m_zipHandle, &_stringBuffer[0], (U32)_stringBuffer.size());
            if (readBytes <= 0)
            {
                MAZE_ERROR("unzReadCurrentFile isn't OK! error=%d", readBytes);
                _stringBuffer.clear();
            }
            else
            {
                bytesLoaded = (Size)readBytes;

            }
        }
        else
        {
            MAZE_ERROR("tryUnzOpenCurrentFile isn't OK!");
        }

        return bytesLoaded;
    }

    //////////////////////////////////////////
    ByteBufferPtr ArchiveFileZip::readArchivedFileAsByteBuffer(String const& _filePath)
    {
        updateZipNavigationMap();

        ZipNavigationMap::const_iterator it = m_zipNavigationMap.find(_filePath);
        MAZE_WARNING_RETURN_VALUE_IF(it == m_zipNavigationMap.end(), 0, "Cannot locate file in archive '%s'!", _filePath.c_str());
        const ZipFileInfo& zipFileInfo = it->second;
        ByteBufferPtr byteBuffer = ByteBuffer::Create();

        MAZE_MUTEX_SCOPED_LOCK(m_zipMutex);

        unzGoToFilePos(m_zipHandle, const_cast<unz_file_pos*>(&zipFileInfo.filePos));
        if (tryUnzOpenCurrentFile(zipFileInfo.fileName, m_zipHandle) == UNZ_OK)
        {
            Size bytes = zipFileInfo.uncompressedSize;

            byteBuffer->resize(bytes);

            S32 readBytes = unzReadCurrentFile(m_zipHandle, byteBuffer->getDataPointer(), (U32)bytes);
            if (readBytes <= 0)
            {
                MAZE_ERROR("unzReadCurrentFile isn't OK! error=%d", readBytes);
                byteBuffer->clear();
            }
        }
        else
        {
            MAZE_ERROR("tryUnzOpenCurrentFile isn't OK!");
        }
        return byteBuffer;
    }

    //////////////////////////////////////////
    Size ArchiveFileZip::getArchivedFileLength(String const& _filePath)
    {
        updateZipNavigationMap();

        ZipNavigationMap::const_iterator it = m_zipNavigationMap.find(_filePath);
        MAZE_WARNING_RETURN_VALUE_IF(it == m_zipNavigationMap.end(), 0, "Cannot locate file in archive '%s'!", _filePath.c_str());
        return it->second.uncompressedSize;
    }

    ////////////////////////////////////
    FileStats ArchiveFileZip::getArchivedFileStats(String const& _filePath)
    {
        // #TODO: Implement
        return FileStats();
    }

    ////////////////////////////////////
    bool ArchiveFileZip::isFileExists()
    {
        return FileHelper::IsFileExists(m_fullPath);
    }

    ////////////////////////////////////
    bool ArchiveFileZip::isFileExists(String const& _filePath)
    {
        if (!isFileExists())
            return false;

        return m_zipNavigationMap.find(_filePath) != m_zipNavigationMap.end();
    }

    //////////////////////////////////////////
    S32 ArchiveFileZip::tryUnzOpenCurrentFile(String const& _fileName, unzFile _file)
    {
        if (m_passwordFunction)
        {
            String password = m_passwordFunction(_fileName);
            if (password.empty())
                return unzOpenCurrentFile(m_zipHandle);
            else
                return unzOpenCurrentFilePassword(m_zipHandle, password.c_str());
        }
        else
        {
            return unzOpenCurrentFile(m_zipHandle);
        }
    }

} // namespace Maze
//////////////////////////////////////////
