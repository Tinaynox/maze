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
#include "MazeCSharpEditorToolsHeader.hpp"
#include "MazeCSharpEditorToolsPlugin.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMPDDefault.hpp"
#include "maze-plugin-csharp-editor-tools/inspectors/entities/MazeComponentEditorMonoBehaviour.hpp"
#include "maze-plugin-csharp-editor-tools/managers/MazeCSharpEditorToolsManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static CSharpEditorToolsPluginPtr s_plugin;
    static CSharpEditorToolsManagerPtr s_csharpEditorToolsManager;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallCSharpEditorToolsPlugin()
    {
        s_plugin = CSharpEditorToolsPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallCSharpEditorToolsPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API void StartPlugin()
    {
        s_plugin = CSharpEditorToolsPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class CSharpEditorToolsPlugin
    //
    //////////////////////////////////////////
    CSharpEditorToolsPlugin::CSharpEditorToolsPlugin()
    {
    }

    //////////////////////////////////////////
    CSharpEditorToolsPlugin::~CSharpEditorToolsPlugin()
    {
    }

    //////////////////////////////////////////
    CSharpEditorToolsPluginPtr CSharpEditorToolsPlugin::Create()
    {
        CSharpEditorToolsPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CSharpEditorToolsPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool CSharpEditorToolsPlugin::init()
    {

        return true;
    }

    //////////////////////////////////////////
    String const& CSharpEditorToolsPlugin::getName()
    {
        static String s_pluginName = "CSharpEditorTools";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void CSharpEditorToolsPlugin::install()
    {
        if (InspectorManager::GetInstancePtr())
        {
            InspectorManager::GetInstancePtr()->registerComponentEditor<MonoBehaviour, ComponentEditorMonoBehaviour>();
        }

        CSharpEditorToolsManager::Initialize(s_csharpEditorToolsManager);
    }

    //////////////////////////////////////////
    void CSharpEditorToolsPlugin::uninstall()
    {
        s_csharpEditorToolsManager.reset();
    }

} // namespace Maze
//////////////////////////////////////////
