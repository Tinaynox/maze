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
#if (!defined(_MazeSystemDialogHelper_hpp_))
#define _MazeSystemDialogHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Window);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_CORE_API, SystemMessageBoxType,
        Info,
        Warning,
        Error);


    //////////////////////////////////////////
    namespace SystemDialogHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern String OpenFile(
            CString _filter,
            Window const* _modalToWindow = nullptr);

        //////////////////////////////////////////
        MAZE_CORE_API extern String SaveFile(
            CString _filter,
            Window const* _modalToWindow = nullptr);

        //////////////////////////////////////////
        MAZE_CORE_API extern String OpenFolder(
            CString _dialogTitle = "Select",
            Window const* _modalToWindow = nullptr);

        //////////////////////////////////////////
        MAZE_CORE_API extern void ShowMessageBox(
            SystemMessageBoxType _type,
            CString _title,
            CString _text,
            ...);


    } // namespace SystemDialogHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemDialogHelper_hpp_
//////////////////////////////////////////
