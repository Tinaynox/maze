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
#include "maze-core/services/MazeLogServiceBase.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <chrono>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class LogServiceBase
    //
    //////////////////////////////////////////
    LogServiceBase::LogServiceBase()
    {
        m_prefixBuffer.reserve(32);

        setPriorityMark(c_logPriority_Default, ' ');
        setPriorityMark(c_logPriority_Warning, 'W');
        setPriorityMark(c_logPriority_Error, 'E');
        setPriorityMark(c_logPriority_Minor, ' ');
    }

    //////////////////////////////////////////
    LogServiceBase::~LogServiceBase()
    {
    }

    //////////////////////////////////////////
    bool LogServiceBase::setLogFile(Path const& _fullPath)
    {
        logFormatted(c_logPriority_Default, "Log File: %s", _fullPath.toUTF8().c_str());

        Path logDir = FileHelper::GetDirectoryInPath(_fullPath);
        FileHelper::CreateDirectoryRecursive(logDir);
        
        m_logFile.open(_fullPath.c_str(), std::ofstream::binary | std::ofstream::trunc);
        if (!m_logFile.is_open())
        {
            logFormatted(c_logPriority_Error, "Log File cannot be opened!");
            return false;
        }

        if (!m_tempLogBuffer.empty())
        {
            m_logFile << m_tempLogBuffer;
            m_logFile.flush();
            m_tempLogBuffer.clear();
        }

        return true;
    }

    //////////////////////////////////////////
    bool LogServiceBase::setLogErrorFile(Path const& _fullPath)
    {
        logFormatted(c_logPriority_Default, "Log Error File: %s", _fullPath.toUTF8().c_str());

        Path logDir = FileHelper::GetDirectoryInPath(_fullPath);
        FileHelper::CreateDirectoryRecursive(logDir);

        m_logErrorFile.open(_fullPath.c_str(), std::ofstream::binary | std::ofstream::trunc);
        if (!m_logErrorFile.is_open())
        {
            logFormatted(c_logPriority_Error, "Log File cannot be opened!");
            return false;
        }

        if (!m_tempLogErrorBuffer.empty())
        {
            m_logErrorFile << m_tempLogErrorBuffer;
            m_logErrorFile.flush();
            m_tempLogErrorBuffer.clear();
        }

        return true;
    }

    //////////////////////////////////////////
    void LogServiceBase::log(S32 _priority, CString _text)
    {
        log(_priority, _text, strlen(_text));
    }

    //////////////////////////////////////////
    void LogServiceBase::log(S32 _priority, CWString _text)
    {
        log(_priority, _text, wcslen(_text));
    }

    //////////////////////////////////////////
    void LogServiceBase::append(S32 _priority, CString _text, Size _size)
    {
        if (_size == 0)
            return;

        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        appendToDefaultStream(_priority, _text, _size);
        appendToLogFile(_priority, _text, _size);
        m_lastLogEndsWithEndline = (_text[_size - 1] == '\n');

        eventLog(_priority, _text, _size);
    }

    //////////////////////////////////////////
    void LogServiceBase::append(S32 _priority, CWString _text, Size _size)
    {
        if (_size == 0)
            return;

        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        appendToDefaultStream(_priority, _text, _size);
        appendToLogFile(_priority, _text, _size);
        m_lastLogEndsWithEndline = (_text[_size - 1] == L'\n');

        eventLogW(_priority, _text, _size);
    }

    //////////////////////////////////////////
    void LogServiceBase::appendPrefix(S32 _priority)
    {
        preparePrefix(_priority);
        append(_priority, m_prefixBuffer.c_str(), m_prefixBuffer.size());
    }

    //////////////////////////////////////////
    void LogServiceBase::log(S32 _priority, CString _text, Size _size)
    {
        appendPrefix(_priority);
        append(_priority, _text, _size);
        append(_priority, "\n", 1);
    }

    //////////////////////////////////////////
    void LogServiceBase::log(S32 _priority, CWString _text, Size _size)
    {
        // Not implemented properly

        append(_priority, _text, _size);
        append(_priority, L"\n", 1);
    }

    //////////////////////////////////////////
    void LogServiceBase::logFormatted(S32 _priority, CString _text, ...)
    {
        
        Char buff[16536];
        va_list args;
        va_start(args, _text);
        StringHelper::FormatString(buff, 16536, _text, args);
        va_end(args);

        log(_priority, (CString)buff);
    }

    //////////////////////////////////////////
    void LogServiceBase::logFormatted(CString _text, ...)
    {
        Char buff[16536];
        va_list args;
        va_start(args, _text);
        StringHelper::FormatString(buff, 16536, _text, args);
        va_end(args);

        log(c_logPriority_Default, (CString)buff);
    }

    //////////////////////////////////////////
    void LogServiceBase::logFormatted(S32 _priority, CWString _text, ...)
    {
        WChar buff[16536];
        va_list args;
        va_start(args, _text);
        StringHelper::FormatString(buff, 16536, _text, args);
        va_end(args);

        log(_priority, (CWString)buff);
    }

    //////////////////////////////////////////
    void LogServiceBase::logFormatted(CWString _text, ...)
    {
        WChar buff[16536];
        va_list args;
        va_start(args, _text);
        StringHelper::FormatString(buff, 16536, _text, args);
        va_end(args);

        log(c_logPriority_Default, (CWString)buff);
    }

    //////////////////////////////////////////
    void LogServiceBase::preparePrefix(S32 _priority)
    {
        auto timeNow = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(timeNow);

        S8 timeBuff[16];
        strftime(timeBuff, 16, "%H:%M:%S", localtime(&timeT));

        Char mark = m_priorityMarks[_priority];
        bool haveMark = (mark != ' ');

        Size const buffSize = 32;
        S8 buff[buffSize];
        Size offs = MAZE_SNPRINTF(
            buff,
            buffSize,
            "%-8s| %c%c%c |: ",
            timeBuff,
            haveMark ? '[' : ' ',
            mark,
            haveMark ? ']' : ' ');
        
        m_prefixBuffer.assign(buff, offs);
    }

    //////////////////////////////////////////
    void LogServiceBase::appendToDefaultStream(S32 _priority, CString _text, Size _size)
    {
        switch (_priority)
        {
            case c_logPriority_Warning:
            case c_logPriority_Error:
                std::cerr.write(_text, _size);
                break;
            default:
                std::cout.write(_text, _size);
                break;
        }
    }

    //////////////////////////////////////////
    void LogServiceBase::appendToDefaultStream(S32 _priority, CWString _text, Size _size)
    {
        switch (_priority)
        {
            case c_logPriority_Warning:
            case c_logPriority_Error:
                std::wcerr.write(_text, _size);
                break;
            default:
                std::wcout.write(_text, _size);
                break;
        }
    }

    //////////////////////////////////////////
    void LogServiceBase::appendToLogFile(S32 _priority, CString _text, Size _size)
    {
        if (_priority == c_logPriority_Warning || _priority == c_logPriority_Error)
        {
            if (m_logErrorFile.is_open())
            {
                m_logErrorFile.write(_text, _size);
                m_logErrorFile.flush();
            }
            else
            {
                m_tempLogErrorBuffer.append(_text, _size);
            }
        }

        if (m_logFile.is_open())
        {
            m_logFile.write(_text, _size);
            m_logFile.flush();
        }
        else
        {
            m_tempLogBuffer.append(_text, _size);
        }
    }

    //////////////////////////////////////////
    void LogServiceBase::appendToLogFile(S32 _priority, CWString _text, Size _size)
    {
        // Not implemented
    }

    //////////////////////////////////////////
    void LogServiceBase::splitAndLog(S32 _priority, CString _text, Size _size, Char _separator)
    {
        Vector<String> words;
        StringHelper::SplitWords(String(_text, _size), words, _separator);
        for (String const& word : words)
        {
            log(_priority, word.c_str(), word.size());
        }
    }

    //////////////////////////////////////////
    void LogServiceBase::setPriorityMark(S32 _priority, Char _mark)
    {
        m_priorityMarks.growToFit(_priority + 1);
        m_priorityMarks.resize(m_priorityMarks.capacity(), ' ');
        m_priorityMarks[_priority] = _mark;
    }


} // namespace Maze
//////////////////////////////////////////
