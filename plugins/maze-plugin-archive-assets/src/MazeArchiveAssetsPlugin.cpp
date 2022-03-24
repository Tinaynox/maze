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
    extern "C" void MAZE_PLUGIN_ARCHIVE_ASSETS_API StartPlugin()
    {
        s_plugin = ArchiveAssetsPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" void MAZE_PLUGIN_ARCHIVE_ASSETS_API StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


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
        
    }

    //////////////////////////////////////////
    void ArchiveAssetsPlugin::uninstall()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
