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
#include "maze-core/system/MazeDynLib.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeDynLib)


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DynLib
    //
    //////////////////////////////////////////
    DynLib::DynLib()
    {
    }

    //////////////////////////////////////////
    DynLib::~DynLib()
    {
    }

    //////////////////////////////////////////
    DynLibPtr DynLib::Create(Path const& _libraryFullPath)
    {
        DynLibPtr lib;
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(DynLib, lib, init(_libraryFullPath));
        return lib;
    }

    //////////////////////////////////////////
    bool DynLib::init(Path const& _libraryFullPath)
    {
        m_libraryFullPath = _libraryFullPath;

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
