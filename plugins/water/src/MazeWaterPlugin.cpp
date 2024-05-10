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
#include "MazeWaterHeader.hpp"
#include "MazeWaterPlugin.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static WaterPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallWaterPlugin()
    {
        s_plugin = WaterPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallWaterPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_WATER_API void StartPlugin()
    {
        s_plugin = WaterPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_WATER_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class WaterPlugin
    //
    //////////////////////////////////////////
    WaterPlugin::WaterPlugin()
    {
    }

    //////////////////////////////////////////
    WaterPlugin::~WaterPlugin()
    {
    }

    //////////////////////////////////////////
    WaterPluginPtr WaterPlugin::Create()
    {
        WaterPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(WaterPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool WaterPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& WaterPlugin::getName()
    {
        static String s_pluginName = "Water";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void WaterPlugin::install()
    {
        
    }

    //////////////////////////////////////////
    void WaterPlugin::uninstall()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
