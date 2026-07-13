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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "MazeRenderSystemVulkanPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Maze::RenderSystemVulkanPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallRenderSystemVulkanPlugin(RenderSystemVulkanConfig const& _config)
    {
        s_plugin = Maze::RenderSystemVulkanPlugin::Create(_config);
        Maze::PluginManager::GetInstancePtr()->installPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallRenderSystemVulkanPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_RENDER_SYSTEM_VULKAN_API void StartPlugin()
    {
        s_plugin = Maze::RenderSystemVulkanPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_RENDER_SYSTEM_VULKAN_API void StopPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(eastl::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class RenderSystemVulkanPlugin
    //
    //////////////////////////////////////////
    RenderSystemVulkanPlugin::RenderSystemVulkanPlugin()
    {
    }

    //////////////////////////////////////////
    RenderSystemVulkanPlugin::~RenderSystemVulkanPlugin()
    {
    }

    //////////////////////////////////////////
    RenderSystemVulkanPluginPtr RenderSystemVulkanPlugin::Create(RenderSystemVulkanConfig const& _config)
    {
        RenderSystemVulkanPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemVulkanPlugin, plugin, init(_config));
        return plugin;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkanPlugin::init(RenderSystemVulkanConfig const& _config)
    {
        m_config = _config;
        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemVulkanPlugin::getName()
    {
        static String s_pluginName = "RenderSystemVulkan";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void RenderSystemVulkanPlugin::install()
    {
        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        MAZE_ERROR_RETURN_IF(graphicsManager == nullptr, "GraphicsManager is not exists!");

        RenderSystemVulkanPtr renderSystem = RenderSystemVulkan::Create(m_config);
        MAZE_ERROR_RETURN_IF(!renderSystem, "RenderSystemVulkan cannot be created!");

        graphicsManager->addRenderSystem(renderSystem);
        m_renderSystem = renderSystem;
    }

    //////////////////////////////////////////
    void RenderSystemVulkanPlugin::uninstall()
    {
        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        if (!graphicsManager)
            return;

        graphicsManager->removeRenderSystem(m_renderSystem.lock());
        m_renderSystem.reset();
    }

} // namespace Maze
//////////////////////////////////////////
