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
#include "maze-core/services/win/MazeLogServiceWin.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    U8 const c_defaultConsoleColor = U8((U8)LogServiceWinColor::Gray | ((U8)LogServiceWinColor::Black << 4));


    //////////////////////////////////////////
    // Class LogService
    //
    //////////////////////////////////////////
    LogService::LogService()
    {
        m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        applyConsoleColor(c_defaultConsoleColor);

        setPriorityColor(c_logPriority_Default, LogServiceWinColor::Gray, LogServiceWinColor::Black);
        setPriorityColor(c_logPriority_Warning, LogServiceWinColor::Yellow, LogServiceWinColor::Black);
        setPriorityColor(c_logPriority_Error, LogServiceWinColor::Red, LogServiceWinColor::Black);
        setPriorityColor(c_logPriority_Fatal, LogServiceWinColor::DarkRed, LogServiceWinColor::Black);
        setPriorityColor(c_logPriority_Minor, LogServiceWinColor::DarkGray, LogServiceWinColor::Black);
        setPriorityColor(c_logPriority_Temp, LogServiceWinColor::DarkBlue, LogServiceWinColor::Black);
    }

    //////////////////////////////////////////
    void LogService::applyConsoleColor(U8 _consoleColor)
    {
        if (m_currentConsoleColor == _consoleColor)
            return;

        m_currentConsoleColor = _consoleColor;
        SetConsoleTextAttribute(m_consoleHandle, _consoleColor);
    }

    //////////////////////////////////////////
    void LogService::setPriorityColor(S32 _priority, LogServiceWinColor _symbolColor, LogServiceWinColor _backgroundColor)
    {
        U8 consoleColor = U8((U8)_symbolColor | ((U8)_backgroundColor << 4));

        m_priorityColors.growToFit(_priority + 1);
        m_priorityColors.resize(m_priorityMarks.capacity(), c_defaultConsoleColor);
        m_priorityColors[_priority] = consoleColor;
    }

    //////////////////////////////////////////
    void LogService::appendToDefaultStream(S32 _priority, CString _text, Size _size)
    {
        U8 prevConsoleColor = m_currentConsoleColor;
        U8 newConsoleColor = m_priorityColors[_priority];

        applyConsoleColor(newConsoleColor);
        LogServiceBase::appendToDefaultStream(_priority, _text, _size);
        applyConsoleColor(prevConsoleColor);
    }

    //////////////////////////////////////////
    void LogService::appendToDefaultStream(S32 _priority, CWString _text, Size _size)
    {
        U8 prevConsoleColor = m_currentConsoleColor;
        U8 newConsoleColor = m_priorityColors[_priority];

        applyConsoleColor(newConsoleColor);
        LogServiceBase::appendToDefaultStream(_priority, _text, _size);
        applyConsoleColor(prevConsoleColor);
    }


} // namespace Maze
//////////////////////////////////////////
