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
#include "MazeArchiveAssetsPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-plugin-archive-assets/assets/MazeAssetRegularArchive.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static ArchiveAssetsPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallArchiveAssetsPlugin()
    {
        s_plugin = ArchiveAssetsPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallArchiveAssetsPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_ARCHIVE_ASSETS_API void StartPlugin()
    {
        s_plugin = ArchiveAssetsPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_ARCHIVE_ASSETS_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif

    //////////////////////////////////////////
    MAZE_PLUGIN_ARCHIVE_ASSETS_API AssetFilePtr ProcessMazeAssetPack(
        String const& _fileFullPath,
        Vector<AssetFilePtr>* _addedFiles,
        Vector<AssetFilePtr>* _removedFiles)
    {
        AssetRegularArchivePtr assetArchive = AssetRegularArchive::Create(_fileFullPath);
        MAZE_WARNING_RETURN_VALUE_IF(!assetArchive, AssetFilePtr(), "Zip archive %s is corrupted!", _fileFullPath.c_str());
        assetArchive->updateChildrenAssets(_addedFiles, _removedFiles);
        return assetArchive;
    }


    //////////////////////////////////////////
    // Class ArchiveAssetsPlugin
    //
    //////////////////////////////////////////
    ArchiveAssetsPlugin::ArchiveAssetsPlugin()
    {
    }

    //////////////////////////////////////////
    ArchiveAssetsPlugin::~ArchiveAssetsPlugin()
    {
    }

    //////////////////////////////////////////
    ArchiveAssetsPluginPtr ArchiveAssetsPlugin::Create()
    {
        ArchiveAssetsPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ArchiveAssetsPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool ArchiveAssetsPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& ArchiveAssetsPlugin::getName()
    {
        static String s_pluginName = "ArchiveAssets";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void ArchiveAssetsPlugin::install()
    {
        AssetManager::GetInstancePtr()->registerFileChildrenProcessor("mzap", &ProcessMazeAssetPack);
    }

    //////////////////////////////////////////
    void ArchiveAssetsPlugin::uninstall()
    {
        if (AssetManager::GetInstancePtr())
            AssetManager::GetInstancePtr()->clearFileChildrenProcessor("mzap");
    }

} // namespace Maze
//////////////////////////////////////////
