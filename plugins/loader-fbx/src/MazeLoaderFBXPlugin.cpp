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
#include "MazeLoaderFBXHeader.hpp"
#include "MazeLoaderFBXPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-plugin-loader-fbx/loaders/MazeLoaderFBX.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static LoaderFBXPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallLoaderFBXPlugin()
    {
        s_plugin = LoaderFBXPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallLoaderFBXPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_FBX_API void StartPlugin()
    {
        s_plugin = LoaderFBXPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_LOADER_FBX_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif

    //////////////////////////////////////////
    static HashedCString const c_meshExtension = MAZE_HASHED_CSTRING("fbx");

    //////////////////////////////////////////
    static inline void RegisterTextureLoader(RenderSystemPtr const& _renderSystem)
    {
        if (_renderSystem && _renderSystem->getTextureManager())
        {
            _renderSystem->getMeshManager()->registerMeshLoader(
                c_meshExtension,
                MeshLoaderData(
                    (LoadMeshAssetFileFunction)&LoadFBX,
                    (LoadMeshByteBufferFunction)&LoadFBX,
                    (IsMeshAssetFileFunction)&IsFBXFile,
                    (IsMeshByteBufferFunction)&IsFBXFile));
        }
    }


    //////////////////////////////////////////
    // Class LoaderFBXPlugin
    //
    //////////////////////////////////////////
    LoaderFBXPlugin::LoaderFBXPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderFBXPlugin::~LoaderFBXPlugin()
    {
    }

    //////////////////////////////////////////
    LoaderFBXPluginPtr LoaderFBXPlugin::Create()
    {
        LoaderFBXPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LoaderFBXPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool LoaderFBXPlugin::init()
    {
        return true;
    }

    //////////////////////////////////////////
    String const& LoaderFBXPlugin::getName()
    {
        static String s_pluginName = "LoaderFBX";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void LoaderFBXPlugin::install()
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
    void LoaderFBXPlugin::uninstall()
    {
        if (GraphicsManager::GetInstancePtr() && RenderSystem::GetCurrentInstancePtr())
        {
            MeshManagerPtr renderMeshManager = RenderSystem::GetCurrentInstancePtr()->getMeshManager();
            if (renderMeshManager)
            {
                renderMeshManager->clearMeshLoader(c_meshExtension);
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
