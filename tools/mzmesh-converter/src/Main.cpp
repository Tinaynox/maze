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
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeAssetHelper.hpp"
#include "maze-core/assets/MazeAssetRegularFile.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderMZMESH.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-plugin-loader-fbx/MazeLoaderFBXPlugin.hpp"


//////////////////////////////////////////
using namespace Maze;


//////////////////////////////////////////
S32 main(S32 _argc, S8 const* _argv[])
{
    MAZE_ERROR_RETURN_VALUE_IF(_argc < 2, 1, "Incorrect count of params");

    MeshManagerPtr meshManager;
    MeshManager::Initialize(meshManager);
    MAZE_ERROR_RETURN_VALUE_IF(!meshManager, 3, "Mesh Manager initialization failed!");

    PluginManagerPtr pluginManager;
    PluginManager::Initialize(pluginManager);

    MAZE_LOAD_PLATFORM_PLUGIN(LoaderFBX);

    Path srcPath = FileHelper::ConvertLocalPathToFullPath(_argv[1]);
    Path destPath = _argc >= 3 ? FileHelper::ConvertLocalPathToFullPath(_argv[2]) : ".";

    Debug::Log("srcMesh=%s", srcPath.toUTF8().c_str());
    Debug::Log("destFolder=%s", destPath.toUTF8().c_str());

    AssetFilePtr assetFile = AssetRegularFile::Create(srcPath);
    MAZE_ERROR_RETURN_VALUE_IF(!assetFile, 4, "Invalid src file!");

    MeshPtr mesh = meshManager->loadMesh(assetFile);
    MAZE_ERROR_RETURN_VALUE_IF(!assetFile, 5, "Src file loading failed!");

    FileHelper::CreateDirectoryRecursive(destPath);
    MAZE_ERROR_RETURN_VALUE_IF(
        !SaveMZMESH(
            *mesh.get(),
            destPath + "/" + FileHelper::GetFileNameWithoutExtension(assetFile->getFileName()) + ".mzmesh"),
        6, "Failed to save mzmesh file!");

    return 0;
}