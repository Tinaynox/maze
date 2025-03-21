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
#include "maze-core/helpers/win/MazeStdHelperWin.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <ShlObj.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace StdHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API FILE* OpenFile(Path const& _fullPath, Path const& _mode)
        {
            MAZE_PROFILE_EVENT("OpenFile");

            FILE* file = nullptr;
            _wfopen_s(&file, _fullPath.c_str(), _mode.c_str());
            return file;
        }

    } // namespace StdHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
