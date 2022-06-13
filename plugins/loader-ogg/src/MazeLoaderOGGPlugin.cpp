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
#include "MazeLoaderOGGHeader.hpp"
#include "MazeLoaderOGGPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-plugin-loader-ogg/loaders/MazeLoaderOGG.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static LoaderOGGPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallLoaderOGGPlugin()
    {
        s_plugin = LoaderOGGPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallLoaderOGGPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_OGG_API void StartPlugin()
    {
        s_plugin = LoaderOGGPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_OGG_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    inline void RegisterSoundLoader(SoundSystemPtr const& _soundSystem)
    {
        
    }


    //////////////////////////////////////////
    // Class LoaderOGGPlugin
    //
    //////////////////////////////////////////
    LoaderOGGPlugin::LoaderOGGPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderOGGPlugin::~LoaderOGGPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderOGGPluginPtr LoaderOGGPlugin::Create()
    {
        LoaderOGGPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LoaderOGGPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool LoaderOGGPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& LoaderOGGPlugin::getName()
    {
        static String s_pluginName = "LoaderOGG";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void LoaderOGGPlugin::install()
    {
        if (SoundManager::GetInstancePtr())
        {
            /*
            SoundManager::GetInstancePtr()->eventDefaultSoundSystemChanged.subscribe(
                [](SoundSystemPtr const& _soundSystem)
                {
                    RegisterSoundLoader(_soundSystem);
                });
            */

            if (SoundManager::GetInstancePtr()->getDefaultSoundSystemRaw())
                RegisterSoundLoader(SoundManager::GetInstancePtr()->getDefaultSoundSystem());
        }
        
    }

    //////////////////////////////////////////
    void LoaderOGGPlugin::uninstall()
    {
        if (SoundManager::GetInstancePtr() && SoundSystem::GetCurrentInstancePtr())
        {
            SoundManager* soundManager = SoundManager::GetInstancePtr();
            if (soundManager)
            {
                //soundManager->clearSoundLoader(
                //    MAZE_HASHED_CSTRING("ogg"));
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
