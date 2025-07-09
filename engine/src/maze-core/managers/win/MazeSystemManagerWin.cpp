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
#include "maze-core/managers/win/MazeSystemManagerWin.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerWin
    //
    //////////////////////////////////////////
    SystemManagerWin::SystemManagerWin()
    {
        m_deviceCategory = DeviceCategory::Desktop;
    }

    //////////////////////////////////////////
    SystemManagerWin::~SystemManagerWin()
    {
    }

    //////////////////////////////////////////
    bool SystemManagerWin::initApplication()
    {
        processApplicationInit();
        return true;
    }

    //////////////////////////////////////////
    bool SystemManagerWin::init(Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        if (!SystemManager::init(_commandLineArguments))
            return false;

        return true;
    }

    //////////////////////////////////////////
    String SystemManagerWin::getClipboardAsString()
    {
        if (!OpenClipboard(nullptr))
            return String();

        HANDLE data = GetClipboardData(CF_TEXT);
        if (data == nullptr)
        {
            CloseClipboard();
            return String();
        }

        String text;

        S8* pszText = static_cast<S8*>(GlobalLock(data));
        if (pszText != nullptr)
            text = pszText;

        GlobalUnlock(data);
        CloseClipboard();

        return text;
    }

    //////////////////////////////////////////
    void SystemManagerWin::setClipboardString(String const& _text)
    {
        if (!OpenClipboard(nullptr))
            return;

        Size const len = _text.size() + 1;
        HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len);
        if (mem == 0)
            return;
        void* m = GlobalLock(mem);
        if (m != nullptr)
            memcpy(m, &_text[0], len);
        GlobalUnlock(mem);

        EmptyClipboard();
        SetClipboardData(CF_TEXT, mem);

        CloseClipboard();
    }

} // namespace Maze
//////////////////////////////////////////
