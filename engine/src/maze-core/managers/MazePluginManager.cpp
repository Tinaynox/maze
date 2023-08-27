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
    static Path BuildPlatformPluginPrefix()
    {
#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        return "lib";
#elif (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
        return "lib";
#endif
        return Path();
    }

    //////////////////////////////////////////
    static Path const& GetPlatformPluginPrefix()
    {
        static Path const prefix = BuildPlatformPluginPrefix();
        return prefix;
    }

    //////////////////////////////////////////
    static Path BuildPlatformPluginPostfix()
    {
        Path postfix;
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#   if (MAZE_ARCH == MAZE_ARCH_X86)
        postfix += "-x86";
#   else
        postfix += "-x64";
#   endif
#endif

#if MAZE_DEBUG
        postfix += "-d";
#endif
        return postfix;
    }

    //////////////////////////////////////////
    static Path const& GetPlatformPluginPostfix()
    {
        static Path const postfix = BuildPlatformPluginPostfix();
        return postfix;
    }


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
    Path PluginManager::GetPlatformPluginName(Path const& _pluginName)
    {
        return GetPlatformPluginPrefix() + _pluginName + GetPlatformPluginPostfix();
    }

    //////////////////////////////////////////
    Path PluginManager::GetPluginNameFromPlatformName(Path const& _pluginPlatformName)
    {
        Path::StringType const& prefixString = GetPlatformPluginPrefix().getPath();
        Path::StringType const& postfixString = GetPlatformPluginPostfix().getPath();
        if (!StringHelper::IsStartsWith(_pluginPlatformName.getPath(), prefixString))
            return Path();
        if (!StringHelper::IsEndsWith(_pluginPlatformName.getPath(), postfixString))
            return Path();

        Path::StringType stringPath = _pluginPlatformName.getPath().substr(
            prefixString.size(),
            _pluginPlatformName.size() - prefixString.size() - postfixString.size());
        return stringPath;
    }

    //////////////////////////////////////////
    bool PluginManager::loadPlugin(Path const& _pluginLibraryFullPath)
    {
        MAZE_PROFILE_EVENT("PluginManager::loadPlugin");

        DynLibManager* dynLibManager = DynLibManager::GetInstancePtr();
        MAZE_ERROR_RETURN_VALUE_IF(!dynLibManager, false, "DynLibManager is not exists!");

        DynLibPtr const& pluginLibrary = dynLibManager->loadLibrary(_pluginLibraryFullPath);
        MAZE_ERROR_RETURN_VALUE_IF(!pluginLibrary, false, "Failed to load plugin - %s", _pluginLibraryFullPath.toUTF8().c_str());

        if (m_pluginLibs.find(_pluginLibraryFullPath) == m_pluginLibs.end())
        {
            m_pluginLibs.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_pluginLibraryFullPath),
                std::forward_as_tuple(pluginLibrary));

            // Call startup function
#if (defined(__GNUC__))
            __extension__
#endif
            PluginDynLibStartFunction func = (PluginDynLibStartFunction)pluginLibrary->getSymbol("StartPlugin");

            MAZE_ERROR_RETURN_VALUE_IF(!func, false, "Cannot find symbol StartPlugin in library: %s", _pluginLibraryFullPath.toUTF8().c_str());

            // This must call installPlugin
            func();
        }

        return true;
    }

    //////////////////////////////////////////
    void PluginManager::unloadPlugin(Path const& _pluginLibraryFullPath)
    {
        MAZE_PROFILE_EVENT("PluginManager::unloadPlugin");

        UnorderedMap<Path, DynLibPtr>::iterator it;

        for (it = m_pluginLibs.begin(); it != m_pluginLibs.end(); ++it)
        {
            if ((*it).first == _pluginLibraryFullPath)
            {
                // Call startup function
#if (defined(__GNUC__))
                __extension__
#endif
                PluginDynLibStopFunction func = (PluginDynLibStopFunction)(*it).second->getSymbol("StopPlugin");

                MAZE_ERROR_RETURN_IF(!func, "Cannot find symbol StopPlugin in library: %s", (*it).first.toUTF8().c_str());

                // This must call installPlugin
                func();

                DynLibManager::GetInstancePtr()->unloadLibrary((*it).second);

                m_pluginLibs.erase(it);

                return;
            }

        }
    }

    //////////////////////////////////////////
    bool PluginManager::loadPlatformPlugin(Path const& _pluginName)
    {
        Path pluginName = GetPlatformPluginName(_pluginName);
        return loadPlugin(pluginName);
    }

    //////////////////////////////////////////
    void PluginManager::unloadAllPlugins()
    {
        while (!m_pluginLibs.empty())
        {
            unloadPlugin(m_pluginLibs.begin()->first);
        }
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
