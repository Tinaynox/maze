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
#include "MazePhysics2DEditorToolsHeader.hpp"
#include "MazePhysics2DEditorToolsPlugin.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-plugin-physics2d-editor-tools/ecs/components/gizmos/MazeBoxCollider2DGizmos.hpp"
#include "maze-plugin-physics2d-editor-tools/ecs/components/gizmos/MazeCircleCollider2DGizmos.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Physics2DEditorToolsPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallPhysics2DEditorToolsPlugin()
    {
        s_plugin = Physics2DEditorToolsPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallPhysics2DEditorToolsPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_PHYSICS2D_EDITOR_TOOLS_API void StartPlugin()
    {
        s_plugin = Physics2DEditorToolsPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_PHYSICS2D_EDITOR_TOOLS_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif

    //////////////////////////////////////////
    // Class Physics2DEditorToolsPlugin
    //
    //////////////////////////////////////////
    Physics2DEditorToolsPlugin::Physics2DEditorToolsPlugin()
    {
    }

    //////////////////////////////////////////
    Physics2DEditorToolsPlugin::~Physics2DEditorToolsPlugin()
    {
    }

    //////////////////////////////////////////
    Physics2DEditorToolsPluginPtr Physics2DEditorToolsPlugin::Create()
    {
        Physics2DEditorToolsPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Physics2DEditorToolsPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool Physics2DEditorToolsPlugin::init()
    {

        return true;
    }

    //////////////////////////////////////////
    String const& Physics2DEditorToolsPlugin::getName()
    {
        static String s_pluginName = "Physics2DEditorTools";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void Physics2DEditorToolsPlugin::install()
    {
        if (GizmosManager::GetInstancePtr())
        {
            GizmosManager::GetInstancePtr()->registerGizmos<BoxCollider2D, BoxCollider2DGizmos>();
            GizmosManager::GetInstancePtr()->registerGizmos<CircleCollider2D, CircleCollider2DGizmos>();
        }
    }

    //////////////////////////////////////////
    void Physics2DEditorToolsPlugin::uninstall()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
