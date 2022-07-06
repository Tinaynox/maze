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
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetDirectory
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetDirectory, AssetFile);
    
    //////////////////////////////////////////
    AssetDirectory::AssetDirectory()
    {

    }

    //////////////////////////////////////////
    AssetDirectory::~AssetDirectory()
    {
    }

    ////////////////////////////////////
    AssetDirectoryPtr AssetDirectory::Create(Path const& _fullPath)
    {
        AssetDirectoryPtr result;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetDirectory, result, initAssetDirectory(_fullPath));
        return result;
    }

    //////////////////////////////////////////
    bool AssetDirectory::initAssetDirectory(Path const& _fullPath)
    {
        if (!AssetRegularFile::init(_fullPath, false))
            return false;

        MAZE_ERROR_RETURN_VALUE_IF(!FileHelper::IsDirectory(m_fullPath), false, "%s is not a directory!", _fullPath.c_str());

        return true;
    }

    //////////////////////////////////////////
    StringKeyMap<AssetFilePtr> const* AssetDirectory::getChildrenAssets() const
    {
        return &m_childrenAssets;
    }

    //////////////////////////////////////////
    bool AssetDirectory::move(Path const& _newFullPath, Vector<Pair<Path, AssetFilePtr>>& _movedFiles)
    {
        if (!AssetRegularFile::move(_newFullPath, _movedFiles))
            return false;

        for (auto childrenAssetData : m_childrenAssets)
        {
            Path newFullPath = m_fullPath + "/" + childrenAssetData.second->getFileName();
            if (!childrenAssetData.second->move(newFullPath, _movedFiles))
                return false;
        }

        return true;
    }
    
    //////////////////////////////////////////
    void AssetDirectory::updateChildrenAssets(
        bool _recursive,
        Vector<AssetFilePtr>* _addedFiles,
        Vector<AssetFilePtr>* _removedFiles)
    {
        Vector<Path> const fileNames = FileHelper::GetRegularFileNamesInPath(m_fullPath);

        Set<Path> confirmedFileFullPathes;

        for (Size i = 0, c = fileNames.size(); i < c; ++i)
        {
            Path const fileFullPath = FileHelper::ConvertLocalPathToFullPath(m_fullPath + '/' + fileNames[i]);

            confirmedFileFullPathes.emplace(fileFullPath);

            auto it = m_childrenAssets.find(fileFullPath);
            if (it == m_childrenAssets.end())
            {
                AssetFilePtr file = AssetManager::GetInstancePtr()->getAssetFileByFullPath(fileFullPath);
                if (file)
                {
                    m_childrenAssets.insert(
                        file->getFullPath(),
                        file);
                    continue;
                }

                if (FileHelper::IsDirectory(fileFullPath))
                {
                    AssetDirectoryPtr directory = AssetDirectory::Create(fileFullPath);

                    if (_recursive)
                        directory->updateChildrenAssets(_recursive, _addedFiles, _removedFiles);

                    file = directory;
                }
                else
                {
                    String const extension = FileHelper::GetFileExtension(fileFullPath);

                    FileChildrenProcessor processor = AssetManager::GetInstancePtr()->getFileChildrenProcessor(extension);
                    if (processor)
                    {
                        file = processor(fileFullPath, _addedFiles, _removedFiles);
                    }
                    else
                    {
                        file = AssetRegularFile::Create(fileFullPath);
                    }
                }

                if (file)
                {
                    m_childrenAssets.insert(
                        file->getFullPath(),
                        file);

                    if (_addedFiles)
                        _addedFiles->push_back(file);
                }
            }
            else
            {
                if (_recursive)
                    it->second->updateChildrenAssets(_recursive, _addedFiles, _removedFiles);
            }
        }

        for (StringKeyMap<AssetFilePtr>::iterator it = m_childrenAssets.begin(),
                                                  end = m_childrenAssets.end();
                                                  it != end;)
        {
            if (confirmedFileFullPathes.count((*it).first) == 0)
            {
                if (it->second->getFullPath() == it->first)
                {
                    Vector<AssetFilePtr> children = it->second->getChildrenAssets(true);
                    for (AssetFilePtr const& assetFile : children)
                    {
                        if (_removedFiles)
                            _removedFiles->push_back(assetFile);
                    }

                    if (_removedFiles)
                        _removedFiles->push_back(it->second);
                }

                it = m_childrenAssets.erase(it);
                end = m_childrenAssets.end();
            }
            else
            {
                ++it;
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
