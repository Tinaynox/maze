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
#include "MazeParticlesEditorToolsHeader.hpp"
#include "MazeParticlesEditorToolsPlugin.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerDefault.hpp"
#include "maze-plugin-particles-editor-tools/inspectors/entities/MazeComponentEditorParticleSystem3D.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazePropertyDrawerParticleSystemParameterF32.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazePropertyDrawerParticleSystemParameterColor.hpp"
#include "maze-plugin-particles-editor-tools/property-drawers/MazePropertyDrawerParticleSystemBurst.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DMainModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DRendererModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DShapeModule.hpp"
#include "maze-plugin-particles-editor-tools/meta-property-drawers/MazeMetaPropertyDrawerParticleSystem3DZoneData.hpp"
#include "maze-plugin-particles-editor-tools/ecs/components/gizmos/MazeParticleSystem3DGizmos.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static ParticlesEditorToolsPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallParticlesEditorToolsPlugin()
    {
        s_plugin = ParticlesEditorToolsPlugin::Create();
        PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    void UninstallParticlesEditorToolsPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API void StartPlugin()
    {
        s_plugin = ParticlesEditorToolsPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API void StopPlugin()
    {
        if (PluginManager::GetInstancePtr())
            PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterF32 =
        MetaPropertyDrawerDefault<
        ParticleSystemParameterF32,
        PropertyDrawerParticleSystemParameterF32>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32);
    template class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API MetaPropertyDrawerDefault<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32>;

    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterF32Positive =
        MetaPropertyDrawerDefault<
        ParticleSystemParameterF32,
        PropertyDrawerParticleSystemParameterF32Positive>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Positive, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Positive);
    template class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API MetaPropertyDrawerDefault<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32Positive>;

    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterF32Degrees =
        MetaPropertyDrawerDefault<
        ParticleSystemParameterF32,
        PropertyDrawerParticleSystemParameterF32Degrees>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Degrees, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterF32Degrees);
    template class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API MetaPropertyDrawerDefault<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32Degrees>;

    //////////////////////////////////////////
    using MetaPropertyDraweParticleSystemParameterColor =
        MetaPropertyDrawerDefault<
        ParticleSystemParameterColor,
        PropertyDrawerParticleSystemParameterColor>;
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(MetaPropertyDraweParticleSystemParameterColor, MetaPropertyDrawer);
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(MetaPropertyDraweParticleSystemParameterColor);
    template class MAZE_PLUGIN_PARTICLES_EDITOR_TOOLS_API MetaPropertyDrawerDefault<ParticleSystemParameterColor, PropertyDrawerParticleSystemParameterColor>;



    //////////////////////////////////////////
    // Class ParticlesEditorToolsPlugin
    //
    //////////////////////////////////////////
    ParticlesEditorToolsPlugin::ParticlesEditorToolsPlugin()
    {
    }

    //////////////////////////////////////////
    ParticlesEditorToolsPlugin::~ParticlesEditorToolsPlugin()
    {
    }

    //////////////////////////////////////////
    ParticlesEditorToolsPluginPtr ParticlesEditorToolsPlugin::Create()
    {
        ParticlesEditorToolsPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticlesEditorToolsPlugin, plugin, init());
        return plugin;
    }

    //////////////////////////////////////////
    bool ParticlesEditorToolsPlugin::init()
    {

        return true;
    }

    //////////////////////////////////////////
    String const& ParticlesEditorToolsPlugin::getName()
    {
        static String s_pluginName = "ParticlesEditorTools";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void ParticlesEditorToolsPlugin::install()
    {
        InspectorManager::GetInstancePtr()->registerPropertyDrawer<ParticleSystemParameterF32, PropertyDrawerParticleSystemParameterF32>();
        InspectorManager::GetInstancePtr()->registerPropertyDrawer<ParticleSystemParameterColor, PropertyDrawerParticleSystemParameterColor>();

        InspectorManager::GetInstancePtr()->registerPropertyDrawer<ParticleSystemBurst, PropertyDrawerParticleSystemBurst>();

        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystemParameterF32, MetaPropertyDraweParticleSystemParameterF32>();
        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystemParameterColor, MetaPropertyDraweParticleSystemParameterColor>();

        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DMainModule, MetaPropertyDrawerParticleSystem3DMainModule>();
        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DRendererModule, MetaPropertyDrawerParticleSystem3DRendererModule>();
        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DShapeModule, MetaPropertyDrawerParticleSystem3DShapeModule>();

        InspectorManager::GetInstancePtr()->registerMetaPropertyDrawer<ParticleSystem3DZoneData, MetaPropertyDrawerParticleSystem3DZoneData>();

        InspectorManager::GetInstancePtr()->registerComponentEditor<ParticleSystem3D, ComponentEditorParticleSystem3D>();

        GizmosManager::GetInstancePtr()->registerGizmos<ParticleSystem3D, ParticleSystem3DGizmos>();
    }

    //////////////////////////////////////////
    void ParticlesEditorToolsPlugin::uninstall()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
