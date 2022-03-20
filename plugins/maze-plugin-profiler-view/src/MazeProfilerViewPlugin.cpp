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
#include "MazeProfilerViewHeader.hpp"
#include "MazeProfilerViewPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-plugin-profiler-view/MazeProfilerViewService.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static ProfilerViewPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallProfilerViewPlugin()
    {
        s_plugin = ProfilerViewPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallProfilerViewPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" void MAZE_PLUGIN_PROFILER_VIEW_API StartPlugin()
    {
        s_plugin = ProfilerViewPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" void MAZE_PLUGIN_PROFILER_VIEW_API StopPlugin()
    {
        PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class ProfilerViewPlugin
    //
    //////////////////////////////////////////
    ProfilerViewPlugin::ProfilerViewPlugin()
    {
    }

    //////////////////////////////////////////
    ProfilerViewPlugin::~ProfilerViewPlugin()
    {
    }

    //////////////////////////////////////////
    ProfilerViewPluginPtr ProfilerViewPlugin::Create()
    {
        ProfilerViewPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ProfilerViewPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool ProfilerViewPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& ProfilerViewPlugin::getName()
    {
        static String s_pluginName = "ProfilerView";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void ProfilerViewPlugin::install()
    {
        ProfilerViewService::GetInstancePtr()->initialize();
    }

    //////////////////////////////////////////
    void ProfilerViewPlugin::uninstall()
    {
        ProfilerViewService::GetInstancePtr()->shutdown();
    }

} // namespace Maze
//////////////////////////////////////////
