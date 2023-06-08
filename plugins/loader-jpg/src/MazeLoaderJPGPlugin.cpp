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
#include "MazeLoaderJPGHeader.hpp"
#include "MazeLoaderJPGPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-plugin-loader-jpg/loaders/MazeLoaderJPG.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static LoaderJPGPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallLoaderJPGPlugin()
    {
        s_plugin = LoaderJPGPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallLoaderJPGPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_JPG_API void StartPlugin()
    {
        s_plugin = LoaderJPGPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_JPG_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif

    //////////////////////////////////////////
    static HashedCString const c_textureExtension = MAZE_HASHED_CSTRING("jpg");

    //////////////////////////////////////////
    static inline void RegisterTextureLoader(RenderSystemPtr const& _renderSystem)
    {
        if (_renderSystem && _renderSystem->getTextureManager())
        {
            _renderSystem->getTextureManager()->registerTextureLoader(
                c_textureExtension,
                TextureLoaderData(
                    (LoadTextureAssetFileFunction)&LoadJPG,
                    (LoadTextureByteBufferFunction)&LoadJPG,
                    (IsTextureAssetFileFunction)&IsJPGFile,
                    (IsTextureByteBufferFunction)&IsJPGFile));
        }
    }


    //////////////////////////////////////////
    // Class LoaderJPGPlugin
    //
    //////////////////////////////////////////
    LoaderJPGPlugin::LoaderJPGPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderJPGPlugin::~LoaderJPGPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderJPGPluginPtr LoaderJPGPlugin::Create()
    {
        LoaderJPGPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LoaderJPGPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool LoaderJPGPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& LoaderJPGPlugin::getName()
    {
        static String s_pluginName = "LoaderJPG";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void LoaderJPGPlugin::install()
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
    void LoaderJPGPlugin::uninstall()
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
