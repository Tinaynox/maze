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
#include "maze-core/helpers/MazeTextHelper.hpp"
#include <cwchar>
#include <utf8/utf8.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace TextHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API String ConvertUTF8ToUTF32(CString _begin, CString _end)
        {
            String string;
            ConvertUTF8ToUTF32(_begin, _end, std::back_inserter(string));
            return string;
        }
        
    } // namespace TextHelper
    ////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
