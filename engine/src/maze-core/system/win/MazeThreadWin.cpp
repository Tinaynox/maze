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
#include "maze-core/system/win/MazeThreadWin.hpp"
#include <process.h>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    S32 ConvertToWinThreadPriority(ThreadPriority _priority)
    {
        switch (_priority)
        {
            case ThreadPriority::Critical:   return THREAD_PRIORITY_TIME_CRITICAL;
            case ThreadPriority::Highest:    return THREAD_PRIORITY_HIGHEST;
            case ThreadPriority::High:       return THREAD_PRIORITY_ABOVE_NORMAL;
            case ThreadPriority::Normal:     return THREAD_PRIORITY_NORMAL;
            case ThreadPriority::Low:        return THREAD_PRIORITY_BELOW_NORMAL;
            case ThreadPriority::Lowest:     return THREAD_PRIORITY_LOWEST;
            case ThreadPriority::Idle:       return THREAD_PRIORITY_IDLE;

            default:                         return THREAD_PRIORITY_NORMAL;
        }
    }

    //////////////////////////////////////////
    ThreadPriority ConvertFromWinThreadPriority(S32 _priority)
    {
        switch (_priority)
        {
            case THREAD_PRIORITY_TIME_CRITICAL:  return ThreadPriority::Critical;
            case THREAD_PRIORITY_HIGHEST:        return ThreadPriority::Highest;
            case THREAD_PRIORITY_ABOVE_NORMAL:   return ThreadPriority::High;
            case THREAD_PRIORITY_NORMAL:         return ThreadPriority::Normal;
            case THREAD_PRIORITY_BELOW_NORMAL:   return ThreadPriority::Low;
            case THREAD_PRIORITY_LOWEST:         return ThreadPriority::Lowest;
            case THREAD_PRIORITY_IDLE:           return ThreadPriority::Idle;

            default:                             return ThreadPriority::Normal;
        }
    }



    //////////////////////////////////////////
    // Class ThreadWin
    //
    //////////////////////////////////////////
    ThreadWin::ThreadWin()
        : m_thread(0)
        , m_threadId(U32(-1))
    {
    }

    //////////////////////////////////////////
    bool ThreadWin::run()
    {
        if (!m_entryPoint)
        {
            MAZE_TODO;
            return false;
        }

        if (m_thread)
        {
            MAZE_TODO;
            return false;
        }

        m_thread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, &ThreadWin::EntryPoint, this, 0, &m_threadId));

        if (!m_thread)
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ThreadWin::terminate()
    {
        if (m_thread)
        {
            TerminateThread(m_thread, 0);
            CloseHandle(m_thread);
            m_thread = 0;
            m_threadId = U32(-1);
            m_entryPoint.reset();
        }
    }

    //////////////////////////////////////////
    void ThreadWin::wait()
    {
        if (m_thread)
        {
            // A thread cannot wait for itself!
            MAZE_DEBUG_BP_RETURN_IF(m_threadId == GetCurrentThreadId());

            WaitForSingleObject(m_thread, INFINITE);
        }
    }

    //////////////////////////////////////////
    bool ThreadWin::isRunning()
    {
        return (m_thread != 0);
    }

    //////////////////////////////////////////
    void ThreadWin::setPriority(ThreadPriority _priority)
    {
        if (!m_thread)
            return;

        SetThreadPriority(m_thread, ConvertToWinThreadPriority(_priority));
    }

    //////////////////////////////////////////
    ThreadPriority ThreadWin::getPriority() const
    {
        return ConvertFromWinThreadPriority(GetThreadPriority(m_thread));
    }

    //////////////////////////////////////////
    U32 __stdcall ThreadWin::EntryPoint(void* _userData)
    {
        ThreadWin* owner = static_cast<ThreadWin*>(_userData);
        
        S32 result = owner->m_entryPoint->run();
            
        CloseHandle(owner->m_thread);
        owner->m_thread = 0;
        owner->m_threadId = U32(-1);

        _endthreadex(0);

        return result;
    }

} // namespace Maze
//////////////////////////////////////////
