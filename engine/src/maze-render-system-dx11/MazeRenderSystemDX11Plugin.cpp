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
#include "MazeRenderSystemDX11Header.hpp"
#include "MazeRenderSystemDX11Plugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Maze::RenderSystemDX11PluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallRenderSystemDX11Plugin(RenderSystemDX11Config const& _config)
    {
        s_plugin = Maze::RenderSystemDX11Plugin::Create(_config);
        Maze::PluginManager::GetInstancePtr()->installPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallRenderSystemDX11Plugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_RENDER_SYSTEM_DX11_API void StartPlugin()
    {
        s_plugin = Maze::RenderSystemDX11Plugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_RENDER_SYSTEM_DX11_API void StopPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class RenderSystemDX11Plugin
    //
    //////////////////////////////////////////
    RenderSystemDX11Plugin::RenderSystemDX11Plugin()
    {
    }

    //////////////////////////////////////////
    RenderSystemDX11Plugin::~RenderSystemDX11Plugin()
    {
    }

    //////////////////////////////////////////
    RenderSystemDX11PluginPtr RenderSystemDX11Plugin::Create(RenderSystemDX11Config const& _config)
    {
        RenderSystemDX11PluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemDX11Plugin, plugin, init(_config));
        return plugin;
    }

    //////////////////////////////////////////
    bool RenderSystemDX11Plugin::init(RenderSystemDX11Config const& _config)
    {
        m_config = _config;
        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemDX11Plugin::getName()
    {
        static String s_pluginName = "RenderSystemDX11";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void RenderSystemDX11Plugin::install()
    {
        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        MAZE_ERROR_RETURN_IF(graphicsManager == nullptr, "GraphicsManager is not exists!");

        RenderSystemDX11Ptr renderSystem = RenderSystemDX11::Create(m_config);
        MAZE_ERROR_RETURN_IF(!renderSystem, "RenderSystemDX11 cannot be created!");

        graphicsManager->addRenderSystem(renderSystem);
        m_renderSystem = renderSystem;
    }

    //////////////////////////////////////////
    void RenderSystemDX11Plugin::uninstall()
    {
        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        if (!graphicsManager)
            return;

        graphicsManager->removeRenderSystem(m_renderSystem.lock());
        m_renderSystem.reset();
    }

} // namespace Maze
//////////////////////////////////////////
