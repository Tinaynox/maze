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
#include "maze-core/managers/MazeDynLibManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/system/MazeDynLib.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeDynLibManager)


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class DynLibManager
    //
    //////////////////////////////////////////
    DynLibManager* DynLibManager::s_instance = nullptr;

    //////////////////////////////////////////
    DynLibManager::DynLibManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    DynLibManager::~DynLibManager()
    {
        for (auto const& lib : m_loadedLibs)
        {
            lib.second->unload();
        }
        m_loadedLibs.clear();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void DynLibManager::Initialize(DynLibManagerPtr& inputManager)
    {
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(DynLibManager, inputManager, init());
    }

    //////////////////////////////////////////
    bool DynLibManager::init()
    {
    

        return true;
    }

    //////////////////////////////////////////
    DynLibPtr const& DynLibManager::loadLibrary(Path const& _libraryFullPath)
    {
        static DynLibPtr const nullPtr;

        UnorderedMap<Path, DynLibPtr>::iterator it = m_loadedLibs.find(_libraryFullPath);
        if (it != m_loadedLibs.end())
        {
            return it->second;
        }
        else
        {
            DynLibPtr dynLib = DynLib::Create(_libraryFullPath);
            if (!dynLib)
                return nullPtr;

            if (!dynLib->load())
                return nullPtr;

            DynLibPtr* insertedDynLib = &m_loadedLibs.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_libraryFullPath),
                std::forward_as_tuple(dynLib)).first->second;
            return *insertedDynLib;
        }
    }

    //////////////////////////////////////////
    void DynLibManager::unloadLibrary(DynLib* _lib)
    {
        UnorderedMap<Path, DynLibPtr>::iterator it =
            std::find_if(
                m_loadedLibs.begin(),
                m_loadedLibs.end(),
                [_lib](Pair<Path, DynLibPtr> const& _value) -> bool
                {
                    return _value.second.get() == _lib;
                });

        if (it == m_loadedLibs.end())
            return;

        it->second->unload();
    }


} // namespace Maze
//////////////////////////////////////////
