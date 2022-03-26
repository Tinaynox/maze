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
#include "MazeSoundSystemOpenALHeader.hpp"
#include "MazeSoundSystemOpenALPlugin.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenALConfig.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Maze::SoundSystemOpenALPluginPtr s_plugin;


#if (MAZE_STATIC)

    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API void InstallSoundSystemOpenALPlugin(SoundSystemOpenALConfig const& _config)
    {
        s_plugin = Maze::SoundSystemOpenALPlugin::Create(_config);
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    MAZE_SOUND_SYSTEM_OPENAL_API void UninstallSoundSystemOpenALPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#else

    //////////////////////////////////////////
    extern "C" MAZE_SOUND_SYSTEM_OPENAL_API void StartPlugin()
    {
        s_plugin = Maze::SoundSystemOpenALPlugin::Create();
        Maze::PluginManager::GetInstancePtr()->installPlugin(std::static_pointer_cast<Plugin>(s_plugin));
    }

    //////////////////////////////////////////
    extern "C" MAZE_SOUND_SYSTEM_OPENAL_API void StopPlugin()
    {
        Maze::PluginManager::GetInstancePtr()->uninstallPlugin(std::static_pointer_cast<Plugin>(s_plugin));
        s_plugin.reset();
    }

#endif


    //////////////////////////////////////////
    // Class SoundSystemOpenALPlugin
    //
    //////////////////////////////////////////
    SoundSystemOpenALPlugin::SoundSystemOpenALPlugin()
    {
    }

    //////////////////////////////////////////
    SoundSystemOpenALPlugin::~SoundSystemOpenALPlugin()
    {
    }

    //////////////////////////////////////////
    SoundSystemOpenALPluginPtr SoundSystemOpenALPlugin::Create(SoundSystemOpenALConfig const& _config)
    {
        SoundSystemOpenALPluginPtr plugin;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundSystemOpenALPlugin, plugin, init(_config));
        return plugin;
    }

    //////////////////////////////////////////
    bool SoundSystemOpenALPlugin::init(SoundSystemOpenALConfig const& _config)
    {
        m_config = _config;
        return true;
    }

    //////////////////////////////////////////
    String const& SoundSystemOpenALPlugin::getName()
    {
        static String s_pluginName = "SoundSystemOpenAL";
        return s_pluginName;
    }

    //////////////////////////////////////////
    void SoundSystemOpenALPlugin::install()
    {
        SoundManager* soundManager = SoundManager::GetInstancePtr();
        MAZE_ERROR_RETURN_IF(soundManager == nullptr, "SoundManager is not exists!");

        SoundSystemOpenALPtr soundSystem = SoundSystemOpenAL::Create(m_config);
        MAZE_ERROR_RETURN_IF(!soundSystem, "SoundSystemOpenAL cannot be created!");

        soundManager->addSoundSystem(soundSystem);
        m_soundSystem = soundSystem;
    }

    //////////////////////////////////////////
    void SoundSystemOpenALPlugin::uninstall()
    {
        SoundManager* soundManager = SoundManager::GetInstancePtr();
        if (!soundManager)
            return;

        soundManager->removeSoundSystem(m_soundSystem.lock());
        m_soundSystem.reset();
    }

} // namespace Maze
//////////////////////////////////////////
