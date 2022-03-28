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
#include "MazeConsoleHeader.hpp"
#include "MazeConsolePlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static ConsolePluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallConsolePlugin()
    {
        s_plugin = ConsolePlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallConsolePlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_CONSOLE_API void StartPlugin()
    {
        s_plugin = ConsolePlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_CONSOLE_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class ConsolePlugin
    //
    //////////////////////////////////////////
    ConsolePlugin::ConsolePlugin()
    {
    }

    //////////////////////////////////////////
    ConsolePlugin::~ConsolePlugin()
    {
    }

    //////////////////////////////////////////
    ConsolePluginPtr ConsolePlugin::Create()
    {
        ConsolePluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ConsolePlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool ConsolePlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& ConsolePlugin::getName()
    {
        static String s_pluginName = "Console";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void ConsolePlugin::install()
    {
        ConsoleService::GetInstancePtr()->initialize();
    }

    //////////////////////////////////////////
    void ConsolePlugin::uninstall()
    {
        if (ConsoleService::GetInstancePtr())
            ConsoleService::GetInstancePtr()->shutdown();
    }

} // namespace Maze
//////////////////////////////////////////
