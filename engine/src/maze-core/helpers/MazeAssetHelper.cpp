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
#include "maze-core/helpers/MazeAssetHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace AssetHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API Vector<AssetFilePtr> GetAllAssetFilesInDirectory(Path const& _path)
        {
            Vector<AssetFilePtr> result;

            Vector<Path> fileNames = FileHelper::GetRegularFileNamesInPath(_path);
            for (Path const& fileName : fileNames)
            {
                Path fullPath = _path + "/" + fileName;
                AssetFilePtr const& file = AssetManager::GetInstancePtr()->getAssetFileByFullPath(fullPath);
                MAZE_ERROR_CONTINUE_IF(!file, "File %s is not asset file!", fullPath.toUTF8().c_str());
                result.emplace_back(file);
            }

            return result;
        }

        //////////////////////////////////////////
        MAZE_CORE_API void CollectAllRegularAssetFilesInDirectoryRecursive(
            Path const& _path,
            Vector<AssetFilePtr>& _outResult)
        {
            Vector<AssetFilePtr> filesInDirectory = GetAllAssetFilesInDirectory(_path);

            for (AssetFilePtr const& assetFile : filesInDirectory)
            {
                if (assetFile->getMetaClass()->isInheritedFrom<AssetDirectory>())
                    CollectAllRegularAssetFilesInDirectoryRecursive(
                        assetFile->getFullPath(), _outResult);
                else
                    _outResult.push_back(assetFile);
            }
        }

    } // namespace AssetHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
