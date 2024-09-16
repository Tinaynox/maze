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
#include "MazeLoaderTTFHeader.hpp"
#include "MazeLoaderTTFPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-ui/managers/MazeTrueTypeFontManager.hpp"
#include "maze-plugin-loader-ttf/loaders/MazeLoaderTTF.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static LoaderTTFPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallLoaderTTFPlugin()
    {
        s_plugin = LoaderTTFPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallLoaderTTFPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_TTF_API void StartPlugin()
    {
        s_plugin = LoaderTTFPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_TTF_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif



    //////////////////////////////////////////
    // Class LoaderTTFPlugin
    //
    //////////////////////////////////////////
    LoaderTTFPlugin::LoaderTTFPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderTTFPlugin::~LoaderTTFPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderTTFPluginPtr LoaderTTFPlugin::Create()
    {
        LoaderTTFPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LoaderTTFPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool LoaderTTFPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& LoaderTTFPlugin::getName()
    {
        static String s_pluginName = "LoaderTTF";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void LoaderTTFPlugin::install()
    {
        TrueTypeFontManager::GetInstancePtr()->setTrueTypeFontLoader(
            TrueTypeFontLoaderData(
                (CreateTrueTypeFontFunction)&CreateTrueTypeFont,
                (IsTrueTypeFontAssetFileFunction)&IsTTFFile,
                (IsTrueTypeFontByteBufferFunction)&IsTTFFile));
    }

    //////////////////////////////////////////
    void LoaderTTFPlugin::uninstall()
    {
        if (TrueTypeFontManager::GetInstancePtr())
            TrueTypeFontManager::GetInstancePtr()->setTrueTypeFontLoader(TrueTypeFontLoaderData());
    }

} // namespace Maze
//////////////////////////////////////////
