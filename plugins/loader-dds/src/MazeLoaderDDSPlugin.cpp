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
#include "MazeLoaderDDSHeader.hpp"
#include "MazeLoaderDDSPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-plugin-loader-dds/loaders/MazeLoaderDDS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static LoaderDDSPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallLoaderDDSPlugin()
    {
        s_plugin = LoaderDDSPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallLoaderDDSPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_DDS_API void StartPlugin()
    {
        s_plugin = LoaderDDSPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_DDS_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif

    //////////////////////////////////////////
    static HashedCString const c_textureExtension = MAZE_HASHED_CSTRING("dds");

    //////////////////////////////////////////
    static inline void RegisterTextureLoader(RenderSystemPtr const& _renderSystem)
    {
        if (_renderSystem && _renderSystem->getTextureManager())
        {
            _renderSystem->getTextureManager()->registerTextureLoader(
                c_textureExtension,
                TextureLoaderData(
                    (LoadTextureAssetFileFunction)&LoadDDS,
                    (LoadTextureByteBufferFunction)&LoadDDS,
                    (IsTextureAssetFileFunction)&IsDDSFile,
                    (IsTextureByteBufferFunction)&IsDDSFile));
        }
    }


    //////////////////////////////////////////
    // Class LoaderDDSPlugin
    //
    //////////////////////////////////////////
    LoaderDDSPlugin::LoaderDDSPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderDDSPlugin::~LoaderDDSPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderDDSPluginPtr LoaderDDSPlugin::Create()
    {
        LoaderDDSPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LoaderDDSPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool LoaderDDSPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& LoaderDDSPlugin::getName()
    {
        static String s_pluginName = "LoaderDDS";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void LoaderDDSPlugin::install()
    {
        if (GraphicsManager::GetInstancePtr())
        {
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.subscribe(
                [](RenderSystemPtr const& _renderSystem)
                {
                    RegisterTextureLoader(_renderSystem);
                });

            if (GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw())
                RegisterTextureLoader(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem());
        }
        
    }

    //////////////////////////////////////////
    void LoaderDDSPlugin::uninstall()
    {
        if (GraphicsManager::GetInstancePtr() && RenderSystem::GetCurrentInstancePtr())
        {
            TextureManagerPtr textureManager = TextureManager::GetCurrentInstancePtr();
            if (textureManager)
            {
                textureManager->clearTextureLoader(c_textureExtension);
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
