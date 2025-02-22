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
#include "maze-plugin-archive-assets/assets/MazeAssetRegularArchive.hpp"
#include "maze-plugin-archive-assets/assets/MazeAssetObfuscation.hpp"
#include "maze-plugin-archive-assets/assets/MazeAssetArchivedFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetRegularArchive
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetRegularArchive, AssetRegularFile);
    
    //////////////////////////////////////////
    AssetRegularArchive::AssetRegularArchive()
    {

    }

    //////////////////////////////////////////
    AssetRegularArchive::~AssetRegularArchive()
    {
    }

    //////////////////////////////////////////
    AssetRegularArchivePtr AssetRegularArchive::Create(Path const& _fullPath)
    {
        AssetRegularArchivePtr result;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(AssetRegularArchive, result, init(_fullPath));
        return result;
    }

    //////////////////////////////////////////
    bool AssetRegularArchive::init(Path const& _fullPath, bool _normalizePath)
    {
        if (!AssetRegularFile::init(_fullPath, _normalizePath))
            return false;

        m_archive = ArchiveFileZip::Create(_fullPath);
        if (!m_archive)
            return false;
               
        m_archive->setPasswordFunction(CallObfuscationPasswordFunction);

        return true;
    }

    //////////////////////////////////////////
    void AssetRegularArchive::updateChildrenAssets(
        Vector<AssetFilePtr>* _addedFiles,
        Vector<AssetFilePtr>* _removedFiles)
    {
        MAZE_PROFILE_EVENT("AssetRegularArchive::updateChildrenAssets");

        Set<Path> confirmedFileFullPathes;

        Vector<Path> archivedFilePathes = m_archive->getArchivedFilePathes();

        for (Size j = 0, jn = archivedFilePathes.size(); j < jn; ++j)
        {
            Path const& archivedFilePath = archivedFilePathes[j];

            auto it = m_childrenAssets.find(archivedFilePath);
            if (it == m_childrenAssets.end())
            {
                AssetArchivedFilePtr archivedFile = AssetArchivedFile::Create(
                    m_archive,
                    archivedFilePath);

                if (archivedFile)
                {
                    confirmedFileFullPathes.emplace(archivedFile->getFullPath());

                    m_childrenAssets.emplace(
                        std::piecewise_construct,
                        std::forward_as_tuple(archivedFile->getFullPath()),
                        std::forward_as_tuple(archivedFile));

                    if (_addedFiles)
                        _addedFiles->push_back(archivedFile);
                }
            }
        }

        for (UnorderedMap<Path, AssetFilePtr>::iterator it = m_childrenAssets.begin(),
                                                        end = m_childrenAssets.end();
                                                        it != end;)
        {
            if (confirmedFileFullPathes.count((*it).first) == 0)
            {
                if (_removedFiles)
                    _removedFiles->push_back(it->second);

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
