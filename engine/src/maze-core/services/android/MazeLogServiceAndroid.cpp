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
#include "maze-core/services/android/MazeLogServiceAndroid.hpp"
#include <android/log.h>


//////////////////////////////////////////
namespace Maze
{
    
    
    ////////////////////////////////////
    android_LogPriority LogPriorityToAndroidLogPriority(S32 _priority)
    {
        switch (_priority)
        {
            case c_logPriority_Warning: return ANDROID_LOG_WARN;
            case c_logPriority_Error: return ANDROID_LOG_ERROR;
            default: return ANDROID_LOG_INFO;
        }
    }
    
    //////////////////////////////////////////
    // Class LogService
    //
    //////////////////////////////////////////
    template<>
    SharedPtr<LogService> Singleton<LogService>::s_instance{};
    template<>
    Mutex Singleton<LogService>::s_mutex{};
    

    //////////////////////////////////////////
    LogService::LogService()
        : m_prevLogPriority(-1)
    {
        m_eventLog.subscribe(this, &LogService::notifyLog);
    }

    //////////////////////////////////////////
    void LogService::notifyLog(S32 _priority, CString _text, Size _size)
    {
        if (m_prevLogPriority != _priority && !m_androidBuffer.empty())
        {
            android_LogPriority androidLogPriority = LogPriorityToAndroidLogPriority(m_prevLogPriority);
            ((void)__android_log_write(androidLogPriority, "MazeEngine", m_androidBuffer.c_str()));
            m_androidBuffer.clear();
        }

        m_androidBuffer.append(_text, _size);

        android_LogPriority androidLogPriority = LogPriorityToAndroidLogPriority(_priority);

        Size pos = String::npos;
        while ((pos = m_androidBuffer.find_first_of('\n')) != String::npos)
        {
            String t = m_androidBuffer.substr(0, pos);
            m_androidBuffer.erase(0, pos + 1);

            ((void)__android_log_write(androidLogPriority, "MazeEngine", t.c_str()));
        }
        m_prevLogPriority = _priority;
    }

} // namespace Maze
//////////////////////////////////////////
