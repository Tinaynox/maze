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
#if (!defined(_MazeInputHelperWin_hpp_))
#define _MazeInputHelperWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/helpers/MazeInputHelper.hpp"
#include "maze-core/system/MazeKeyCode.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    namespace InputHelper
    {
        ////////////////////////////////////
        MAZE_CORE_API KeyCode ConvertVirtualCodeToKeyCode(S64 _virtualCode, U32 _modifiers = 0u);

        ////////////////////////////////////
        MAZE_CORE_API S64 ConvertKeyCodeToVirtualCode(KeyCode _keyCode, U32 _modifiers = 0u);

        //////////////////////////////////////////
        MAZE_CORE_API String TranslateWin32Text(S32 _virtualCode, S32 _scanCode, U32 _modifiers = 0u);

        //////////////////////////////////////////
        MAZE_CORE_API bool IsKeyDown(WPARAM _wparam);

        //////////////////////////////////////////
        MAZE_CORE_API U32 CollectInputEventKeyboardModifiers(UINT _message, WPARAM _wParam, LPARAM _lParam);


    } // namespace InputHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInputHelperWin_hpp_
//////////////////////////////////////////
