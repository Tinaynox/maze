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
#include "maze-core/helpers/win/MazeTextHelperWin.hpp"
#include <commdlg.h>
#include <ShlObj.h>


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
            CHAR szFile[MAX_PATH] = { 0 };
            
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = _modalToWindow ? _modalToWindow->castRaw<WindowWin>()->getHandle() : NULL;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);

            CHAR currentDir[MAX_PATH] = { 0 };
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
            CHAR szFile[MAX_PATH] = { 0 };

            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = _modalToWindow ? _modalToWindow->castRaw<WindowWin>()->getHandle() : NULL;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile);

            CHAR currentDir[MAX_PATH] = { 0 };
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

        //////////////////////////////////////////
        static S32 CALLBACK OpenFolderCallbackProc(HWND _hwnd, UINT _msg, LPARAM _lParam, LPARAM _lpData)
        {
            LPITEMIDLIST pidlNavigate;
            switch (_msg)
            {
                case BFFM_INITIALIZED:
                {
                    pidlNavigate = (LPITEMIDLIST)_lpData;
                    if (pidlNavigate != NULL)
                        SendMessage(_hwnd, BFFM_SETSELECTION, (WPARAM)FALSE, (LPARAM)pidlNavigate);
                    break;
                }
                default:
                {
                    break;
                }
            }
            return 0;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String OpenFolder(
            CString _dialogTitle,
            Window const* _modalToWindow)
        {
            String workingDirectory = FileHelper::GetWorkingDirectory();
            StringHelper::ReplaceSubstring(workingDirectory, "/", "\\");

            LPITEMIDLIST pidlStart;
            SHParseDisplayName(
                TextHelper::ConvertUTF8ToUCS2(
                    workingDirectory.c_str(),
                    workingDirectory.size()).c_str(),
                NULL,
                &pidlStart,
                0,
                NULL);

            BROWSEINFO bi;
            CHAR displayName[MAX_PATH];

            ZeroMemory(&bi, sizeof(bi));
            bi.hwndOwner = _modalToWindow ? _modalToWindow->castRaw<WindowWin>()->getHandle() : NULL;
            bi.pszDisplayName = NULL;
            bi.lpszTitle = _dialogTitle ? _dialogTitle : "Select folder";
            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
            bi.lParam = (LPARAM)pidlStart;
            bi.lpfn = OpenFolderCallbackProc;
            LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
            if (pidl)
            {
                SHGetPathFromIDList(pidl, displayName);
                return StringHelper::ToString(displayName);
            }

            return String();
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void ShowMessageBox(
            SystemMessageBoxType _type,
            CString _title,
            CString _text,
            ...)
        {
            String formattedText;
            MAZE_FORMAT_VA_STRING(_text, formattedText);

            UINT flags = MB_OK;

            switch (_type)
            {
                case SystemMessageBoxType::Warning: flags |= MB_ICONEXCLAMATION; break;
                case SystemMessageBoxType::Error: flags |= MB_ICONERROR; break;
            }

            MessageBoxA(NULL, formattedText.c_str(), _title, flags);
        }


    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
