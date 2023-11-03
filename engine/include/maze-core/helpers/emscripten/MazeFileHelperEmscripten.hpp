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
#if (!defined(_MazeFileHelperEmscripten_hpp_))
#define _MazeFileHelperEmscripten_hpp_


//////////////////////////////////////////
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #define MAZE_EMSCRIPTEN_LOCAL_STORAGE_DIR "/localStorage"
    
    
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API Path GetWorkingDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryFullPath();

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDocumentsDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultTemporaryDirectory();

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultLogDirectory();
        
        //////////////////////////////////////////
        MAZE_CORE_API void MountEmscriptenLocalStorage(std::function<void(bool)> _mountCb = nullptr);
        
        //////////////////////////////////////////
        MAZE_CORE_API void SyncEmscriptenLocalStorage(std::function<void(bool)> _syncCb = nullptr);
        
        //////////////////////////////////////////
        MAZE_CORE_API bool IsSyncEmscriptenLocalStorageInProgress();

    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFileHelperEmscripten_hpp_
//////////////////////////////////////////
