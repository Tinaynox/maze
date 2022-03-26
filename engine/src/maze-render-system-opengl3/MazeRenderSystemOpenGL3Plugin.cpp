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
#include "MazeRenderSystemOpenGL3Header.hpp"
#include "MazeRenderSystemOpenGL3Plugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Maze::RenderSystemOpenGL3PluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallRenderSystemOpenGL3Plugin(RenderSystemOpenGLConfig const& _config)
    {
        s_plugin = Maze::RenderSystemOpenGL3Plugin::Create(_config);
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallRenderSystemOpenGL3Plugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_RENDER_SYSTEM_OPENGL3_API void StartPlugin()
    {
        s_plugin = Maze::RenderSystemOpenGL3Plugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_RENDER_SYSTEM_OPENGL3_API void StopPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class RenderSystemOpenGL3Plugin
    //
    //////////////////////////////////////////
    RenderSystemOpenGL3Plugin::RenderSystemOpenGL3Plugin()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3Plugin::~RenderSystemOpenGL3Plugin()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3PluginPtr RenderSystemOpenGL3Plugin::Create(RenderSystemOpenGLConfig const& _config)
    {
        RenderSystemOpenGL3PluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemOpenGL3Plugin, plugin, init(_config));
        return plugin;
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL3Plugin::init(RenderSystemOpenGLConfig const& _config)
    {
        m_config = _config;
        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemOpenGL3Plugin::getName()
    {
        static String s_pluginName = "RenderSystemOpenGL3";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3Plugin::install()
    {
        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        MAZE_ERROR_RETURN_IF(graphicsManager == nullptr, "GraphicsManager is not exists!");

        RenderSystemOpenGL3Ptr renderSystem = RenderSystemOpenGL3::Create(m_config);
        MAZE_ERROR_RETURN_IF(!renderSystem, "RenderSystemOpenGL3 cannot be created!");

        graphicsManager->addRenderSystem(renderSystem);
        m_renderSystem = renderSystem;
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3Plugin::uninstall()
    {
        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        if (!graphicsManager)
            return;

        graphicsManager->removeRenderSystem(m_renderSystem.lock());
        m_renderSystem.reset();
    }

} // namespace Maze
//////////////////////////////////////////
