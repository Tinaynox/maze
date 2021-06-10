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
#include "maze-core/helpers/win/MazeSystemDialogHelperWin.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/system/win/MazeWindowWin.hpp"
#include <commdlg.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemDialogHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API String OpenFile(
            CString _filter,
            Window const* _modalToWindow)
        {
            OPENFILENAMEA ofn;
            CHAR szFile[260] = { 0 };
            
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = _modalToWindow ? _modalToWindow->castRaw<WindowWin>()->getHandle() : NULL;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);

            CHAR currentDir[256] = { 0 };
            if (GetCurrentDirectoryA(256, currentDir))
                ofn.lpstrInitialDir = currentDir;

            ofn.lpstrFilter = _filter;
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

            if (GetOpenFileNameA(&ofn) == TRUE)
                return StringHelper::ToString(ofn.lpstrFile);

            return String();

        }

        //////////////////////////////////////////
        MAZE_CORE_API String SaveFile(
            CString _filter,
            Window const* _modalToWindow)
        {
            OPENFILENAMEA ofn;
            CHAR szFile[260] = { 0 };

            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = _modalToWindow ? _modalToWindow->castRaw<WindowWin>()->getHandle() : NULL;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);

            CHAR currentDir[256] = { 0 };
            if (GetCurrentDirectoryA(256, currentDir))
                ofn.lpstrInitialDir = currentDir;

            ofn.lpstrFilter = _filter;
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
            
            ofn.lpstrDefExt = strchr(_filter, '\0') + 1;

            if (GetSaveFileNameA(&ofn) == TRUE)
                return StringHelper::ToString(ofn.lpstrFile);

            return String();

        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
