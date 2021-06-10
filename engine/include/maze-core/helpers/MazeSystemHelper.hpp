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
#if (!defined(_MazeSystemHelper_hpp_))
#define _MazeSystemHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeGeoLocation.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern GeoLocation GetGeoLocation();

        //////////////////////////////////////////
        MAZE_CORE_API String ConstructSystemInfo();

        //////////////////////////////////////////
        MAZE_CORE_API extern void OpenURL(String const& _url);

        //////////////////////////////////////////
        MAZE_CORE_API extern String const& GetOSVersion();

        //////////////////////////////////////////
        MAZE_CORE_API extern String const& GetDeviceName();

        //////////////////////////////////////////
        MAZE_CORE_API extern void SetupMazeDynLibDirectoriesPolicy();


    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemHelper_hpp_
//////////////////////////////////////////
