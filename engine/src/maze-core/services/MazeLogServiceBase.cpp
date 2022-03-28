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
#include <iostream>
#include <cstdio>
#include <cstdarg>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class LogServiceBase
    //
    //////////////////////////////////////////
    LogServiceBase::LogServiceBase()
    {

    }

    //////////////////////////////////////////
    LogServiceBase::~LogServiceBase()
    {
    }

    //////////////////////////////////////////
    bool LogServiceBase::setLogFile(CString _fullPath)
    {
        logFormatted(c_logPriority_Default, "Log File: %s\n", _fullPath);

        String logDir = FileHelper::GetDirectoryInPath(_fullPath);
        FileHelper::CreateDirectoryRecursive(logDir.c_str());
        
        m_logFile.open(_fullPath, std::ofstream::binary | std::ofstream::trunc);
        if (!m_logFile.is_open())
        {
            logFormatted(c_logPriority_Error, "Log File cannot be opened!\n");
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
    void LogServiceBase::log(S32 _priority, CString _text, Size _size)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        std::cout.write(_text, _size);

        if (m_logFile.is_open())
        {
            m_logFile.write(_text, _size);
            m_logFile.flush();
        }
        else
            m_tempLogBuffer.append(_text, _size);

        eventLog(_priority, _text, _size);
    }

    //////////////////////////////////////////
    void LogServiceBase::log(S32 _priority, CWString _text, Size _size)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        std::wcout.write(_text, _size);
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


} // namespace Maze
//////////////////////////////////////////
