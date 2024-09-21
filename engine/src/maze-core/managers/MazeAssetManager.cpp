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
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeAssetManager)
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetManager
    //
    //////////////////////////////////////////
    AssetManager* AssetManager::s_instance = nullptr;

    //////////////////////////////////////////
    AssetManager::AssetManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    AssetManager::~AssetManager()
    {
        while (!m_assetFilesByFileName.empty())
        {
            AssetFilePtr assetFile = m_assetFilesByFileName.begin()->second;
            processRemoveFile(assetFile);
        }

        m_assetFilesById.clear();
        m_assetFilesByFullPath.clear();

        m_assetDirectoryPathes.clear();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void AssetManager::Initialize(AssetManagerPtr& _assetManager, DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(AssetManager, _assetManager, init(_config));
    }

    //////////////////////////////////////////
    bool AssetManager::init(DataBlock const& _config)
    {
        m_generateIdsForNewAssetFiles =
            _config.getBool(MAZE_HCS("generateIdsForNewAssetFiles"), m_generateIdsForNewAssetFiles);

        AssetUnitManager::Initialize(
            m_assetUnitManager,
            _config.getDataBlock(MAZE_HCS("assetUnitConfig"), DataBlock::c_empty));
        if (!m_assetUnitManager)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool AssetManager::addAssetsDirectoryPath(Path const& _path)
    {
        m_assetDirectoryPathes.insert(_path);
        if (!addAssetsDirectory(_path, true))
            return false;

        eventAssetsDirectoryPathAdded(_path);
        return true;
    }

    //////////////////////////////////////////
    void AssetManager::removeAssetsDirectoryPath(Path const& _path)
    {
        m_assetDirectoryPathes.erase(_path);
        removeAssetsDirectory(_path, true);

        eventAssetsDirectoryPathRemoved(_path);
    }

    //////////////////////////////////////////
    void AssetManager::updateAssets()
    {
        MAZE_PROFILE_EVENT("AssetManager::updateAssets");

        Vector<AssetFilePtr> addedFiles;
        Vector<AssetFilePtr> removedFiles;

        Set<Path> rootAssetDirectories = AssetManager::collectRootAssetDirectoryPathes();
        for (Path const& fullPath : rootAssetDirectories)
        {
            const AssetFilePtr& rootAsset = getAssetFileByFullPath(fullPath);
            if (rootAsset)
                rootAsset->updateChildrenAssets(true, &addedFiles, &removedFiles);
            else
            {
                MAZE_ERROR("Undefined rootAsset for path '%s'", fullPath.toUTF8().c_str());
            }
        }

        for (AssetFilePtr const& addFile : addedFiles)
            processAddFile(addFile);

        for (AssetFilePtr const& addFile : addedFiles)
        {
            HashedString extension = HashedString(addFile->getExtension().toUTF8());

            if (extension != MAZE_HCS("mzmeta"))
                loadAndUpdateFileMetaData(addFile);
            eventAssetFileAdded(addFile, extension);
        }

        for (AssetFilePtr const& removeFile : removedFiles)
        {
            processRemoveFile(removeFile);
            eventAssetFileRemoved(removeFile);
        }
    }
    
    //////////////////////////////////////////
    bool AssetManager::addAssetsDirectory(Path const& _path, bool _recursive)
    {
        MAZE_PROFILE_EVENT("AssetManager::addAssetsDirectory");

        String pathUTF8 = _path.toUTF8();
        Debug::Log("Adding assets directory '%s'...", pathUTF8.c_str());

        Path fullPath = FileHelper::ConvertLocalPathToFullPath(_path);
        
        MAZE_ERROR_RETURN_VALUE_IF(!FileHelper::IsDirectory(fullPath), false, "%s is not a directory!", _path.c_str());

        AssetDirectoryPtr directory;

        UnorderedMap<Path, AssetFilePtr>::iterator it = m_assetFilesByFullPath.find(fullPath);
        if (it == m_assetFilesByFullPath.end())
        {
            directory = AssetDirectory::Create(fullPath);
            if (!directory)
                return false;

            processAddFile(directory);
        }
        else
        {
            MAZE_ERROR_RETURN_VALUE_IF(it->second->getClassUID() != ClassInfo<AssetDirectory>::UID(), false, "%s is not a AssetDirectory!", _path.toUTF8().c_str());
            directory = it->second->cast<AssetDirectory>();
        }

        Vector<AssetFilePtr> addedFiles;
        Vector<AssetFilePtr> removedFiles;
        if (!directory->updateChildrenAssets(_recursive, &addedFiles, &removedFiles))
            return false;

        for (AssetFilePtr const& addFile : addedFiles)
            processAddFile(addFile);

        for (AssetFilePtr const& addFile : addedFiles)
        {
            HashedString extension = HashedString(addFile->getExtension().toUTF8());

            if (extension != MAZE_HCS("mzmeta"))
                loadAndUpdateFileMetaData(addFile);
            eventAssetFileAdded(addFile, extension);
        }

        for (AssetFilePtr const& removeFile : removedFiles)
        {
            processRemoveFile(removeFile);
            eventAssetFileRemoved(removeFile);
        }

        Debug::Log("Assets directory '%s' added.", pathUTF8.c_str());

        return true;
    }
    
    //////////////////////////////////////////
    void AssetManager::removeAssetsDirectory(Path const& _path, bool _recursive)
    {
        Path fullPath = FileHelper::ConvertLocalPathToFullPath(_path.c_str());
    
        Vector<AssetFilePtr> removedFiles;
        
        // #TODO:
        // removeFilesInPath(fullPath, _recursive, &removedFiles);
    }

    //////////////////////////////////////////
    String AssetManager::constructAssetsInfo()
    {
        String info;
        
        info += "[Assets Info]\n";
        info += "\tDefault Assets Directory: " + getDefaultAssetsDirectory().toUTF8() + "\n";
        
        info += "Assets:\n";
        for (auto const& assetFileByFileNameData : m_assetFilesByFileName)
        {
            info += "\t";
            if (assetFileByFileNameData.second->getClassUID() == ClassInfo<AssetDirectory>::UID())
                info += "[DIR] ";
            info += assetFileByFileNameData.first;
            info += " (Id:" + StringHelper::ToString(assetFileByFileNameData.second->getAssetFileId()) + ")\n";
        }
        
        return info;
    }

    //////////////////////////////////////////
    AssetFilePtr const& AssetManager::getAssetFile(AssetFileId _id) const
    {
        static AssetFilePtr nullPtr;

        UnorderedMap<AssetFileId, AssetFilePtr>::const_iterator it = m_assetFilesById.find(_id);
        if (it == m_assetFilesById.end())
            return nullPtr;

        return (*it).second;
    }

    //////////////////////////////////////////
    AssetFilePtr const& AssetManager::getAssetFileByFileName(Path const& _fileName)
    {
        static AssetFilePtr nullPtr;

        MAZE_ERROR_RETURN_VALUE_IF(strcmp(_fileName.toUTF8().c_str(), "") == 0, nullPtr, "Empty file name!");

        UnorderedMap<Path, AssetFilePtr>::iterator it = m_assetFilesByFileName.find(_fileName);
        if (it == m_assetFilesByFileName.end())
            return nullPtr;

        return (*it).second;
    }
        
    //////////////////////////////////////////
    AssetFilePtr const& AssetManager::getAssetFileByFullPath(Path const& _fileFullPath)
    {
        static AssetFilePtr nullPtr;

        UnorderedMap<Path, AssetFilePtr>::iterator it = m_assetFilesByFullPath.find(_fileFullPath);
        if (it == m_assetFilesByFullPath.end())
            return nullPtr;
        
        return (*it).second;
    }

    //////////////////////////////////////////
    const AssetFilePtr& AssetManager::getAssetFile(Path const& _string)
    {
        if (_string.getPath().find('/') != Path::StringType::npos)
            return getAssetFileByFullPath(_string);
        else
            return getAssetFileByFileName(_string);
    }

    //////////////////////////////////////////
    void AssetManager::deleteAssetFile(const AssetFilePtr& _assetFile)
    {
        if (!_assetFile)
            return;

        FileHelper::Delete(_assetFile->getFullPath().c_str());
        updateAssets();
    }

    //////////////////////////////////////////
    void AssetManager::moveAssetFile(const AssetFilePtr& _assetFile, Path const& _newFullPath)
    {
        if (!_assetFile)
            return;

        MAZE_ERROR_RETURN_IF(
            m_assetDirectoryPathes.find(_assetFile->getFullPath()) != m_assetDirectoryPathes.end(),
            "It's forbidden to move root asset directories!");

        Vector<Pair<Path, AssetFilePtr>> movedFiles;
        if (_assetFile->move(_newFullPath, movedFiles))
        {
            for (auto movedFileData : movedFiles)
            {
                Path const& fullPath = movedFileData.first;
                Path fileName = FileHelper::GetFileNameInPath(fullPath);

                AssetFilePtr const& assetFile = movedFileData.second;

                m_assetFilesByFileName.erase(fileName);
                m_assetFilesByFullPath.erase(fullPath);

                m_assetFilesByFileName[assetFile->getFileName()] = assetFile;
                m_assetFilesByFullPath[assetFile->getFullPath()] = assetFile;
            }

            for (auto movedFileData : movedFiles)
                eventAssetFileMoved(movedFileData.second, movedFileData.first);

            updateAssets();
        }
    }

    //////////////////////////////////////////
    void AssetManager::processAddFile(AssetFilePtr const& _file)
    {
        if (_file->getAssetFileId() > 0u)
            m_assetFilesById[_file->getAssetFileId()] = _file;

        m_assetFilesByFileName[_file->getFileName()] = _file;
        m_assetFilesByFullPath[_file->getFullPath()] = _file;

        m_assetFilesUpdateTimeUTC[_file] = _file->getFileStats().getLastChangeTimeUTC();

        _file->eventAssetFileIdChanged.subscribe(this, &AssetManager::notifyAssetFileIdChanged);
    }

    //////////////////////////////////////////
    void AssetManager::processRemoveFile(AssetFilePtr const& _file)
    {
        if (_file->getAssetFileId() > 0u)
            m_assetFilesById.erase(_file->getAssetFileId());

        m_assetFilesByFileName.erase(_file->getFileName());
        m_assetFilesByFullPath.erase(_file->getFullPath());

        m_assetFilesUpdateTimeUTC.erase(_file);

        _file->eventAssetFileIdChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    bool AssetManager::openXMLDocumentAssetFile(
        tinyxml2::XMLDocument& _doc,
        Path const& _fileName,
        bool _warningIfNotExists)
    {
        MAZE_PROFILE_EVENT("AssetManager::openXMLDocumentAssetFile");

        AssetFilePtr const& assetFile = getAssetFileByFileName(_fileName);
        if (!assetFile)
        {
            MAZE_ERROR_IF(_warningIfNotExists, "Undefined XML file - %s", _fileName.c_str());
            return false;
        }

        return assetFile->readToXMLDocument(_doc);
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtension(Path const& _extension)
    {
        return getAssetFilesWithExtension(_extension, [](AssetFilePtr const& _assetFile) { return true; });
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtension(
        Path const& _extension,
        std::function<bool(AssetFilePtr const&)> _pred)
    {
        Vector<AssetFilePtr> result;

        UnorderedMap<Path, AssetFilePtr>::iterator it = m_assetFilesByFullPath.begin();
        UnorderedMap<Path, AssetFilePtr>::iterator end = m_assetFilesByFullPath.end();
        for (; it != end; ++it)
        {
            if ((*it).second->getExtension() == _extension)
            {
                if (_pred((*it).second))
                    result.push_back((*it).second);
            }
        }

        return result;
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtensions(Set<Path> const& _extensions)
    {
        return getAssetFilesWithExtensions(_extensions, [](AssetFilePtr const& _assetFile) { return true; });
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtensions(
        Set<Path> const& _extensions,
        std::function<bool(AssetFilePtr const&)> _pred)
    {
        Vector<AssetFilePtr> result;

        UnorderedMap<Path, AssetFilePtr>::iterator it = m_assetFilesByFullPath.begin();
        UnorderedMap<Path, AssetFilePtr>::iterator end = m_assetFilesByFullPath.end();
        for (; it != end; ++it)
        {
            if (_extensions.count((*it).second->getExtension()))
            {
                if (_pred((*it).second))
                    result.push_back((*it).second);
            }
        }

        return result;
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFiles(ClassUID _uid)
    {
        Vector<AssetFilePtr> result;

        for (auto data : m_assetFilesByFullPath)
        {
            ClassUID uid = data.second->getClassUID();

            if (uid == _uid)
                result.emplace_back(data.second);
        }

        return result;
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesInFolder(Path const& _folderFullPath)
    {
        Vector<AssetFilePtr> result;
        for (auto data : m_assetFilesByFullPath)
        {
            if (data.second->getFullPath() != _folderFullPath &&
                data.second->getFullPath().size() > _folderFullPath.size() + 1u &&
                data.second->getFullPath()[_folderFullPath.size()] == '/' &&
                StringHelper::IsStartsWith(data.second->getFullPath().getPath(), _folderFullPath.getPath()))
            {
                Path relativePath = data.second->getFullPath().getPath().substr(
                    _folderFullPath.size() + 1, data.second->getFullPath().size() - _folderFullPath.size() - 1);

                if (relativePath.getPath().find('/') == Path::StringType::npos)
                    result.emplace_back(data.second);
            }
        }

        return result;
    }

    //////////////////////////////////////////
    Path AssetManager::getMetaDataFullPath(AssetFilePtr const& _assetFile)
    {
        return _assetFile->getFullPath() + ".mzmeta";
    }

    //////////////////////////////////////////
    AssetFilePtr AssetManager::getMetaDataFile(AssetFilePtr const& _assetFile)
    {
        return getAssetFileByFullPath(getMetaDataFullPath(_assetFile));
    }

    //////////////////////////////////////////
    bool AssetManager::loadMetaData(AssetFilePtr const& _assetFile, DataBlock& _metaData)
    {
        _metaData.clear();

        AssetFilePtr metaFile = getMetaDataFile(_assetFile);
        if (metaFile)
        {
            ByteBuffer byteBuffer;
            if (!metaFile->readToByteBuffer(byteBuffer))
                return false;

            return _metaData.loadFromByteBuffer(byteBuffer);
        }

        return false;
    }

    //////////////////////////////////////////
    void AssetManager::saveMetaData(AssetFilePtr const& _assetFile, DataBlock const& _metaData)
    {
        Path assetFullPath = getMetaDataFullPath(_assetFile);
        _metaData.saveTextFile(assetFullPath);
    }

    //////////////////////////////////////////
    void AssetManager::updateAndSaveMetaData(AssetFilePtr const& _assetFile)
    {
        DataBlock metaData;
        loadMetaData(_assetFile, metaData);
        _assetFile->updateDataFromAssetFile(metaData);
        saveMetaData(_assetFile, metaData);
    }

    //////////////////////////////////////////
    void AssetManager::loadAndUpdateFileMetaData(AssetFilePtr const& _assetFile)
    {
        _assetFile->loadInfoFromMetaData();
    }

    //////////////////////////////////////////
    Set<Path> AssetManager::collectRootAssetDirectoryPathes()
    {
        Set<Path> result = m_assetDirectoryPathes;

        // Remove sub directories
        for (auto it = result.begin(),
                  end = result.end();
                  it != end;)
        {
            Path const& fullPath = *it;
            bool isSubDirectory = std::find_if(
                result.begin(),
                result.end(),
                [&fullPath](Path const& _fullPath)
                {
                    if (fullPath == _fullPath)
                        return false;

                    return StringHelper::IsStartsWith(_fullPath.getPath(), fullPath.getPath());
                }) != result.end();

            if (isSubDirectory)
            {
                it = result.erase(it);
                end = result.end();
            }
            else
            {
                ++it;
            }
        }

        return result;
    }

    //////////////////////////////////////////
    AssetFileId AssetManager::generateAssetFileId() const
    {
        AssetFileId afid;

        do
        {
            afid = Random::RangeRandom(1u, U32_MAX);
        }
        while (m_assetFilesById.find(afid) != m_assetFilesById.end());

        return afid;
    }

    //////////////////////////////////////////
    void AssetManager::notifyAssetFileIdChanged(AssetFile* _assetFile, AssetFileId _prevAssetFileId, AssetFileId _newAssetFileId)
    {
        auto it = m_assetFilesById.find(_prevAssetFileId);
        if (it != m_assetFilesById.end())
        {
            MAZE_ERROR_IF(
                _assetFile != it->second.get(),
                "Two asset files have same AssetFileId! First=%s, Second=%s",
                it->second->getFullPath().toUTF8().c_str(),
                _assetFile->getFullPath().toUTF8().c_str());
            m_assetFilesById.erase(it);
        }
        
        m_assetFilesById[_newAssetFileId] = _assetFile->getSharedPtr();
    }


} // namespace Maze
//////////////////////////////////////////
