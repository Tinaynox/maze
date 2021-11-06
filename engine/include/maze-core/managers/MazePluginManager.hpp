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
#pragma once
#if (!defined(_MazePluginManager_hpp_))
#define _MazePluginManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PluginManager);
    MAZE_USING_SHARED_PTR(DynLib);
    MAZE_USING_SHARED_PTR(Plugin);


    //////////////////////////////////////////
    // Class PluginManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PluginManager
    {
    public:

        //////////////////////////////////////////
        virtual ~PluginManager();

        //////////////////////////////////////////
        static void Initialize(PluginManagerPtr& _inputManager);


        //////////////////////////////////////////
        bool loadPlugin(String const& _pluginLibraryFullPath);

        //////////////////////////////////////////
        void unloadPlugin(String const& _pluginLibraryFullPath);

        //////////////////////////////////////////
        bool loadPlatformPlugin(String const& _pluginName);


        //////////////////////////////////////////
        static inline PluginManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline PluginManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        void installPlugin(PluginPtr const& _plugin);

        //////////////////////////////////////////
        void uninstallPlugin(Plugin* _plugin);

        //////////////////////////////////////////
        inline void uninstallPlugin(PluginPtr const& _plugin) { return uninstallPlugin(_plugin.get()); }
        
    protected:

        //////////////////////////////////////////
        PluginManager();

        //////////////////////////////////////////
        virtual bool init();

    
    protected:
        static PluginManager* s_instance;

        Vector<DynLibPtr> m_pluginLibs;
        Vector<PluginPtr> m_plugins;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePluginManager_hpp_
//////////////////////////////////////////
