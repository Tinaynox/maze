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
#include "MazeCSharpHeader.hpp"
#include "MazeCSharpPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-plugin-csharp/MazeCSharpService.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static CSharpPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallCSharpPlugin()
    {
        s_plugin = CSharpPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallCSharpPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_CSHARP_API void StartPlugin()
    {
        s_plugin = CSharpPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_CSHARP_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class CSharpPlugin
    //
    //////////////////////////////////////////
    CSharpPlugin::CSharpPlugin()
    {
    }

    //////////////////////////////////////////
    CSharpPlugin::~CSharpPlugin()
    {
    }

    //////////////////////////////////////////
    CSharpPluginPtr CSharpPlugin::Create()
    {
        CSharpPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CSharpPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool CSharpPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& CSharpPlugin::getName()
    {
        static String s_pluginName = "CSharp";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void CSharpPlugin::install()
    {
        CSharpService::GetInstancePtr()->initialize();
    }

    //////////////////////////////////////////
    void CSharpPlugin::uninstall()
    {
        if (CSharpService::GetInstancePtr())
            CSharpService::GetInstancePtr()->shutdown();
    }

} // namespace Maze
//////////////////////////////////////////
