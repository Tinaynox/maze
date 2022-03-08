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
    void AssetManager::addAssetsDirectory(String const& _path, bool _recursive)
    {
        String fullPath = FileHelper::ConvertLocalPathToFullPath(_path.c_str());
        
        MAZE_ERROR_RETURN_IF(!FileHelper::IsDirectory(fullPath), "%s is not a directory!", _path.c_str());

        AssetDirectoryPtr directory;

        StringKeyMap<AssetFilePtr>::iterator it = m_assetFilesByFullPath.find(fullPath);
        if (it == m_assetFilesByFullPath.end())
        {
            directory = AssetDirectory::Create(fullPath);
            if (!directory)
                return;

            processAddFile(directory);
        }
        else
        {
            MAZE_ERROR_RETURN_IF(it->second->getClassUID() != ClassInfo<AssetDirectory>::UID(), "%s is not a AssetDirectory!", _path.c_str());
            directory = it->second->cast<AssetDirectory>();
        }

        Vector<AssetFilePtr> addedFiles;
        Vector<AssetFilePtr> removedFiles;
        directory->updateChildrenAssets(_recursive, &addedFiles, &removedFiles);

        for (AssetFilePtr const& addFile : addedFiles)
            processAddFile(addFile);

        for (AssetFilePtr const& addFile : addedFiles)
            updateFileInfo(addFile);

        for (AssetFilePtr const& removeFile : removedFiles)
            processRemoveFile(removeFile);
    }
    
    //////////////////////////////////////////
    void AssetManager::removeAssetsDirectory(String const& _path, bool _recursive)
    {
        String fullPath = FileHelper::ConvertLocalPathToFullPath(_path.c_str());
    
        Vector<AssetFilePtr> removedFiles;
        
        // #TODO:
        // removeFilesInPath(fullPath, _recursive, &removedFiles);
    }

    //////////////////////////////////////////
    String AssetManager::constructAssetsInfo()
    {
        String info;
        
        info += "[Assets Info]\n";
        info += "\tDefault Assets Directory: " + getDefaultAssetsDirectory() + "\n";
        
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
    AssetFilePtr const& AssetManager::getAssetFileByFileName(HashedCString _fileName)
    {
        static AssetFilePtr nullPtr;

        StringKeyMap<AssetFilePtr>::iterator it = m_assetFilesByFileName.find(_fileName);
        if (it == m_assetFilesByFileName.end())
            return nullPtr;

        return (*it).second;
    }
        
    //////////////////////////////////////////
    AssetFilePtr const& AssetManager::getAssetFileByFullPath(HashedCString _fileFullPath)
    {
        static AssetFilePtr nullPtr;
        
        StringKeyMap<AssetFilePtr>::iterator it = m_assetFilesByFullPath.find(_fileFullPath);
        if (it == m_assetFilesByFullPath.end())
            return nullPtr;
        
        return (*it).second;
    }

    //////////////////////////////////////////
    void AssetManager::processAddFile(AssetFilePtr const& _file)
    {
        m_assetFilesByFileName[_file->getFileName()] = _file;
        m_assetFilesByFullPath[_file->getFullPath()] = _file;
    }

    //////////////////////////////////////////
    void AssetManager::processRemoveFile(AssetFilePtr const& _file)
    {
        m_assetFilesByFileName.erase(_file->getFileName());
        m_assetFilesByFileName.erase(_file->getFullPath());
    }

    ////////////////////////////////////
    void AssetManager::updateFileInfo(AssetFilePtr const& _file)
    {
        Set<String> tags;

        UnorderedMap<String, String> metaData = getMetaData(_file);
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
        String const& _fileName,
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

        StringKeyMap<AssetFilePtr>::iterator it = m_assetFilesByFullPath.begin();
        StringKeyMap<AssetFilePtr>::iterator end = m_assetFilesByFullPath.end();
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
        Vector<AssetFilePtr> result;

        StringKeyMap<AssetFilePtr>::iterator it = m_assetFilesByFullPath.begin();
        StringKeyMap<AssetFilePtr>::iterator end = m_assetFilesByFullPath.end();
        for (; it != end; ++it)
        {
            if (_extensions.count((*it).second->getExtension()))
                result.push_back((*it).second);
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
    Vector<AssetFilePtr> AssetManager::getAssetFilesInFolder(String const& _folderFullPath)
    {
        Vector<AssetFilePtr> result;
        for (auto data : m_assetFilesByFullPath)
        {
            if (   data.second->getFullPath() != _folderFullPath 
                && StringHelper::IsStartsWith(data.second->getFullPath(), _folderFullPath))
            {
                String relativePath = data.second->getFullPath().substr(
                    _folderFullPath.size() + 1, data.second->getFullPath().size() - _folderFullPath.size() - 1);

                if (relativePath.find('/') == String::npos)
                    result.emplace_back(data.second);
            }
        }

        return result;
    }

    //////////////////////////////////////////
    UnorderedMap<String, String> AssetManager::getMetaData(AssetFilePtr const& _assetFile)
    {
        UnorderedMap<String, String> metaData;

        AssetFilePtr metaFile = getAssetFileByFileName(_assetFile->getFileName() + ".meta");
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


} // namespace Maze
//////////////////////////////////////////
