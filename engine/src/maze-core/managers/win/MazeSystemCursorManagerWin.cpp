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
#include "maze-core/managers/win/MazeSystemCursorManagerWin.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/system/win/MazeSystemCursorWin.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemCursorManagerWin
    //
    //////////////////////////////////////////
    SystemCursorManagerWin::SystemCursorManagerWin()
    {
        
    }

    //////////////////////////////////////////
    SystemCursorManagerWin::~SystemCursorManagerWin()
    {
    }

    //////////////////////////////////////////
    bool SystemCursorManagerWin::init(DataBlock const& _config)
    {
        if (!SystemCursorManager::init(_config))
            return false;

        return true;
    }

    //////////////////////////////////////////
    SystemCursorPtr SystemCursorManagerWin::createSystemCursor()
    {
        return SystemCursorWin::Create();
    }

    //////////////////////////////////////////
    void SystemCursorManagerWin::showSystemCursor()
    {
        while (::ShowCursor(TRUE) < 0);
    }

    //////////////////////////////////////////
    void SystemCursorManagerWin::hideSystemCursor()
    {
        while (::ShowCursor(FALSE) >= 0);
    }

    //////////////////////////////////////////
    void SystemCursorManagerWin::setSystemCursorPosition(Vec2F _posSystemScreen)
    {
        ::SetCursorPos((S32)_posSystemScreen.x, (S32)_posSystemScreen.y);
    }

    //////////////////////////////////////////
    Vec2F SystemCursorManagerWin::getSystemCursorPosition()
    {
        ::POINT lp;
        GetCursorPos(&lp);
        return Vec2F((F32)lp.x, (F32)lp.y);
    }

    //////////////////////////////////////////
    SystemCursorPtr const& SystemCursorManagerWin::createBuiltinSystemCursor(BuiltinSystemCursorType _systemCursorType)
    {
        SystemCursorPtr& systemCursor = m_builtinSystemCursors[_systemCursorType];

        switch (_systemCursorType)
        {
            case BuiltinSystemCursorType::Arrow:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_ARROW), true);
                break;
            case BuiltinSystemCursorType::EditText:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_IBEAM), true);
                break;
            case BuiltinSystemCursorType::Wait:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_WAIT), true);
                break;
            case BuiltinSystemCursorType::Cross:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_CROSS), true);
                break;
            case BuiltinSystemCursorType::ArrowUp:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_UPARROW), true);
                break;
            case BuiltinSystemCursorType::SizeNWSE:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_SIZENWSE), true);
                break;
            case BuiltinSystemCursorType::SizeNESW:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_SIZENESW), true);
                break;
            case BuiltinSystemCursorType::SizeWE:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_SIZEWE), true);
                break;
            case BuiltinSystemCursorType::SizeNS:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_SIZENS), true);
                break;
            case BuiltinSystemCursorType::SizeAll:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_SIZEALL), true);
                break;
            case BuiltinSystemCursorType::No:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_NO), true);
                break;
            case BuiltinSystemCursorType::HandPress:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_HAND), true);
                break;
            case BuiltinSystemCursorType::Help:
                systemCursor = SystemCursorWin::Create(LoadCursorA(NULL, IDC_HELP), true);
                break;
            default:
                break;
        }
        return systemCursor;
    }

} // namespace Maze
//////////////////////////////////////////
