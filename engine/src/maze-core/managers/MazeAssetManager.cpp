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
        m_assetFilesByFileName.clear();

        m_assetFilesByFullPath.clear();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void AssetManager::Initialize(AssetManagerPtr& _assetManager)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(AssetManager, _assetManager, init());
    }

    //////////////////////////////////////////
    bool AssetManager::init()
    {    

        return true;
    }

    //////////////////////////////////////////
    void AssetManager::addAssetsDirectoryPath(Path const& _path)
    {
        m_assetDirectoryPathes.insert(_path);
        addAssetsDirectory(_path, true);
    }

    //////////////////////////////////////////
    void AssetManager::removeAssetsDirectoryPath(Path const& _path)
    {
        m_assetDirectoryPathes.erase(_path);
        removeAssetsDirectory(_path, true);
    }

    //////////////////////////////////////////
    void AssetManager::updateAssets()
    {
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
            updateFileInfo(addFile);
            eventAssetFileAdded(addFile);
        }

        for (AssetFilePtr const& removeFile : removedFiles)
        {
            processRemoveFile(removeFile);
            eventAssetFileRemoved(removeFile);
        }
    }
    
    //////////////////////////////////////////
    void AssetManager::addAssetsDirectory(Path const& _path, bool _recursive)
    {
        Path fullPath = FileHelper::ConvertLocalPathToFullPath(_path);
        
        MAZE_ERROR_RETURN_IF(!FileHelper::IsDirectory(fullPath), "%s is not a directory!", _path.c_str());

        AssetDirectoryPtr directory;

        UnorderedMap<Path, AssetFilePtr>::iterator it = m_assetFilesByFullPath.find(fullPath);
        if (it == m_assetFilesByFullPath.end())
        {
            directory = AssetDirectory::Create(fullPath);
            if (!directory)
                return;

            processAddFile(directory);
        }
        else
        {
            MAZE_ERROR_RETURN_IF(it->second->getClassUID() != ClassInfo<AssetDirectory>::UID(), "%s is not a AssetDirectory!", _path.toUTF8().c_str());
            directory = it->second->cast<AssetDirectory>();
        }

        Vector<AssetFilePtr> addedFiles;
        Vector<AssetFilePtr> removedFiles;
        directory->updateChildrenAssets(_recursive, &addedFiles, &removedFiles);

        for (AssetFilePtr const& addFile : addedFiles)
            processAddFile(addFile);

        for (AssetFilePtr const& addFile : addedFiles)
        {
            updateFileInfo(addFile);
            eventAssetFileAdded(addFile);
        }

        for (AssetFilePtr const& removeFile : removedFiles)
        {
            processRemoveFile(removeFile);
            eventAssetFileRemoved(removeFile);
        }
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
            info += assetFileByFileNameData.first + "\n";
        }
        
        return info;
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
        m_assetFilesByFileName[_file->getFileName()] = _file;
        m_assetFilesByFullPath[_file->getFullPath()] = _file;

        m_assetFilesUpdateTimeUTC[_file] = _file->getFileStats().modifiedTimeUTC;
    }

    //////////////////////////////////////////
    void AssetManager::processRemoveFile(AssetFilePtr const& _file)
    {
        m_assetFilesByFileName.erase(_file->getFileName());
        m_assetFilesByFullPath.erase(_file->getFullPath());

        m_assetFilesUpdateTimeUTC.erase(_file);
    }

    ////////////////////////////////////
    void AssetManager::updateFileInfo(AssetFilePtr const& _file)
    {
        Set<String> tags;

        StringKeyMap<String> metaData = getMetaData(_file);
        String const& tagsString = metaData["tags"];
        if (!tagsString.empty())
        {
            Vector<String> words;
            StringHelper::SplitWords(tagsString, words, ',');
            for (String const& word : words)
                tags.emplace(word);
        }

        _file->setTags(tags);
    }

    //////////////////////////////////////////
    bool AssetManager::openXMLDocumentAssetFile(
        tinyxml2::XMLDocument& _doc,
        Path const& _fileName,
        bool _warningIfNotExists)
    {
        AssetFilePtr const& assetFile = getAssetFileByFileName(_fileName);
        if (!assetFile)
        {
            MAZE_ERROR_IF(_warningIfNotExists, "Undefined XML file - %s", _fileName.c_str());
            return false;
        }

        return assetFile->readToXMLDocument(_doc);
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtension(String const& _extension)
    {
        return getAssetFilesWithExtension(_extension, [](AssetFilePtr const& _assetFile) { return true; });
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtension(
        String const& _extension,
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
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtensions(Set<String> const& _extensions)
    {
        return getAssetFilesWithExtensions(_extensions, [](AssetFilePtr const& _assetFile) { return true; });
    }

    //////////////////////////////////////////
    Vector<AssetFilePtr> AssetManager::getAssetFilesWithExtensions(
        Set<String> const& _extensions,
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
        return _assetFile->getFullPath() + ".meta";
    }

    //////////////////////////////////////////
    AssetFilePtr AssetManager::getMetaDataFile(AssetFilePtr const& _assetFile)
    {
        return getAssetFileByFullPath(getMetaDataFullPath(_assetFile));
    }

    //////////////////////////////////////////
    StringKeyMap<String> AssetManager::getMetaData(AssetFilePtr const& _assetFile)
    {
        StringKeyMap<String> metaData;

        AssetFilePtr metaFile = getMetaDataFile(_assetFile);
        if (metaFile)
        {
            String metaDataString;
            metaFile->readToString(metaDataString);

            Vector<String > metaDataLines;
            StringHelper::SplitWords(metaDataString, metaDataLines, '\n');
            for (Size i = 0; i < metaDataLines.size(); ++i)
            {
                if (!metaDataLines[i].size())
                    continue;
                StringHelper::RemoveSymbols(metaDataLines[i], "\r");

                Vector<String> metaDataValues;
                StringHelper::SplitWords(metaDataLines[i], metaDataValues, '=');

                if (metaDataValues.size() == 1)
                    metaData[metaDataValues[0]] = String();
                else
                if (metaDataValues.size() == 2)
                    metaData[metaDataValues[0]] = metaDataValues[1];
            }
        }

        return metaData;
    }

    //////////////////////////////////////////
    void AssetManager::saveMetaData(AssetFilePtr const& _assetFile, StringKeyMap<String> const& _metaData)
    {
        StringStream ss;

        for (auto it = _metaData.begin(), end = _metaData.end(); it != end; ++it)
            ss << it->first << "=" << it->second << "\n";

        String str = ss.str();
        if (str.empty())
            return;

        String assetFullPath = getMetaDataFullPath(_assetFile);

        std::ofstream file;
        file.open(assetFullPath.c_str(), std::ofstream::binary);
        if (!file.is_open())
            return;
        
        file << str;
        file.flush();
        file.close();
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


} // namespace Maze
//////////////////////////////////////////
