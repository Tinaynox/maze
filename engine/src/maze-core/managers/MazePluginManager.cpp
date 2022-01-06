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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/MazePluginManager.hpp"
#include "maze-core/managers/MazeDynLibManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/system/MazeDynLib.hpp"
#include "maze-core/system/MazePlugin.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class PluginManager
    //
    //////////////////////////////////////////
    PluginManager* PluginManager::s_instance = nullptr;

    //////////////////////////////////////////
    PluginManager::PluginManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    PluginManager::~PluginManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void PluginManager::Initialize(PluginManagerPtr& _pluginManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(PluginManager, _pluginManager, init());
    }

    //////////////////////////////////////////
    bool PluginManager::init()
    {
    

        return true;
    }

    //////////////////////////////////////////
    bool PluginManager::loadPlugin(String const& _pluginLibraryFullPath)
    {
        DynLibManager* dynLibManager = DynLibManager::GetInstancePtr();
        MAZE_ERROR_RETURN_VALUE_IF(!dynLibManager, false, "DynLibManager is not exists!");

        DynLibPtr const& pluginLibrary = dynLibManager->loadLibrary(_pluginLibraryFullPath);
        MAZE_ERROR_RETURN_VALUE_IF(!pluginLibrary, false, "Failed to load plugin - %s", _pluginLibraryFullPath.c_str());

        if (std::find(m_pluginLibs.begin(), m_pluginLibs.end(), pluginLibrary) == m_pluginLibs.end())
        {
            m_pluginLibs.push_back(pluginLibrary);

            // Call startup function
#if (defined(__GNUC__))
            __extension__
#endif
            PluginDynLibStartFunction func = (PluginDynLibStartFunction)pluginLibrary->getSymbol("StartPlugin");

            MAZE_ERROR_RETURN_VALUE_IF(!func, false, "Cannot find symbol StartPlugin in library: %s", _pluginLibraryFullPath.c_str());

            // This must call installPlugin
            func();
        }

        return true;
    }

    //////////////////////////////////////////
    void PluginManager::unloadPlugin(String const& _pluginLibraryFullPath)
    {
        Vector<DynLibPtr>::iterator it;

        for (it = m_pluginLibs.begin(); it != m_pluginLibs.end(); ++it)
        {
            if ((*it)->getLibraryFullPath() == _pluginLibraryFullPath)
            {
                // Call startup function
#if (defined(__GNUC__))
                __extension__
#endif
                PluginDynLibStopFunction func = (PluginDynLibStopFunction)(*it)->getSymbol("StopPlugin");

                MAZE_ERROR_RETURN_IF(!func, "Cannot find symbol StopPlugin in library: %s", (*it)->getLibraryFullPath().c_str());

                // This must call installPlugin
                func();

                DynLibManager::GetInstancePtr()->unloadLibrary((*it));

                m_pluginLibs.erase(it);

                return;
            }

        }
    }

    //////////////////////////////////////////
    bool PluginManager::loadPlatformPlugin(String const& _pluginName)
    {
        String pluginName = _pluginName;

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#   if (MAZE_ARCH == MAZE_ARCH_X86)
        pluginName += "-x86";
#   else
        pluginName += "-x64";
#   endif
#elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        pluginName = "lib" + pluginName;
#elif (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
        pluginName = "lib" + pluginName;
#endif
        
#if MAZE_DEBUG
        pluginName += "-d";
#endif

        return loadPlugin(pluginName);
    }

    //////////////////////////////////////////
    void PluginManager::installPlugin(PluginPtr const& _plugin)
    {
        Debug::log << "Installing plugin: " << _plugin->getName() << "..." << endl;

        m_plugins.push_back(_plugin);

        _plugin->install();

        Debug::log << "Plugin installed: " << _plugin->getName() << "." << endl;
    }

    //////////////////////////////////////////
    void PluginManager::uninstallPlugin(Plugin* _plugin)
    {
        Vector<PluginPtr>::iterator it = 
            std::find_if(
                m_plugins.begin(),
                m_plugins.end(),
                [_plugin](PluginPtr const& _value) -> bool
                {
                    return _value.get() == _plugin;
                });

        if (it != m_plugins.end())
        {
            Debug::log << "Uninstalling plugin: " << _plugin->getName() << "..." << endl;
            _plugin->uninstall();
            m_plugins.erase(it);
            Debug::log << "Plugin uninstalled: " << _plugin->getName() << "." << endl;
        }
    }


} // namespace Maze
//////////////////////////////////////////
