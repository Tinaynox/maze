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
#if (!defined(_MazeDynLibManager_hpp_))
#define _MazeDynLibManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(DynLibManager);
    MAZE_USING_SHARED_PTR(DynLib);


    //////////////////////////////////////////
    // Class DynLibManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API DynLibManager
    {
    public:

        //////////////////////////////////////////
        virtual ~DynLibManager();

        //////////////////////////////////////////
        static void Initialize(
            DynLibManagerPtr& _inputManager,
            DataBlock const& _config = DataBlock::c_empty);


        //////////////////////////////////////////
        DynLibPtr const& loadLibrary(Path const& _libraryFullPath);

        //////////////////////////////////////////
        void unloadLibrary(DynLib* _lib);

        //////////////////////////////////////////
        inline void unloadLibrary(DynLibPtr const& _lib) { return unloadLibrary(_lib.get()); }


        //////////////////////////////////////////
        static inline DynLibManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline DynLibManager& GetInstance() { return *s_instance; }

        
    protected:

        //////////////////////////////////////////
        DynLibManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);

    
    protected:
        static DynLibManager* s_instance;

        UnorderedMap<Path, DynLibPtr> m_loadedLibs;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDynLibManager_hpp_
//////////////////////////////////////////
