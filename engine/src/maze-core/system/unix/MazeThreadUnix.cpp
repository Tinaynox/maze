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
#include "maze-core/system/unix/MazeThreadUnix.hpp"
#include <unistd.h>
#include <pthread.h>


//////////////////////////////////////////
namespace Maze
{
    ////////////////////////////////////
    static S32 s_threadPriorityNumbers[static_cast<Size>(ThreadPriority::MAX)] = {0};
    static bool s_threadPriorityNumbersInited = false;


    ////////////////////////////////////
    void SleepCurrentThread(U32 _ms)
    {
        usleep(_ms * 1000);
    }

    ////////////////////////////////////
    S32 ConvertUnixThreadPriority(ThreadPriority _priority)
    {
        switch (_priority)
        {
            case ThreadPriority::Critical:      return SCHED_OTHER;
            case ThreadPriority::Highest:       return SCHED_OTHER;
            case ThreadPriority::High:          return SCHED_OTHER;
            case ThreadPriority::Normal:        return SCHED_OTHER;
            case ThreadPriority::Low:           return SCHED_OTHER;
            case ThreadPriority::Lowest:        return SCHED_OTHER;
            case ThreadPriority::Idle:          return SCHED_OTHER;

            default:                            return SCHED_OTHER;
        }
    }

    ////////////////////////////////////
    F32 ConvertUnixThreadPriorityToUnit(ThreadPriority _priority)
    {
        switch (_priority)
        {
            case ThreadPriority::Critical:      return 1.0f;
            case ThreadPriority::Highest:       return 0.8f;
            case ThreadPriority::High:          return 0.65f;
            case ThreadPriority::Normal:        return 0.5f;
            case ThreadPriority::Low:           return 0.35f;
            case ThreadPriority::Lowest:        return 0.2f;
            case ThreadPriority::Idle:          return 0.0f;

            default:                            return 0.5f;
        }
    }

    ////////////////////////////////////
    S32 GetUnixThreadPriorityNumberMin(S32 _priority)
    {
        static S32 min = sched_get_priority_min(_priority);
        return min;
    }

    ////////////////////////////////////
    S32 GetUnixThreadPriorityNumberMax(S32 _priority)
    {
        static S32 max = sched_get_priority_max(_priority);
        return max;
    }

    ////////////////////////////////////
    void UpdateThreadPriorityNumbers()
    {
        if (s_threadPriorityNumbersInited)
            return;

        for (ThreadPriority e = ThreadPriority::None; e < ThreadPriority::MAX; e = ThreadPriority((S32)e + 1))
        {
            S32 sched = ConvertUnixThreadPriority(e);
            s_threadPriorityNumbers[(Maze::Size)e] = Math::Lerp(
               GetUnixThreadPriorityNumberMin(sched),
               GetUnixThreadPriorityNumberMax(sched),
               ConvertUnixThreadPriorityToUnit(e));
        }

        s_threadPriorityNumbersInited = true;
    }

    ////////////////////////////////////
    S32 GetUnixThreadPriorityNumber(ThreadPriority _priority)
    {
        UpdateThreadPriorityNumbers();
        return s_threadPriorityNumbers[(Maze::Size)_priority];
    }

    ////////////////////////////////////
    ThreadPriority GetUnixThreadPriorityFromNumber(S32 _priority)
    {
        UpdateThreadPriorityNumbers();

        for (ThreadPriority e = ThreadPriority::None; e < ThreadPriority::MAX; e = ThreadPriority((S32)e + 1))
            if (s_threadPriorityNumbers[(Maze::Size)e] == _priority)
                return e;

        return ThreadPriority::None;
    }

    ////////////////////////////////////
    void SetCurrentThreadPriority(ThreadPriority _priority)
    {
        sched_param param;
        memset(&param, 0, sizeof(sched_param));
        param.sched_priority = GetUnixThreadPriorityNumber(_priority);
        if (pthread_setschedparam(pthread_self(), ConvertUnixThreadPriority(_priority), &param) != 0)
        {
            MAZE_BP;
        }
    }

    ////////////////////////////////////
    ThreadPriority GetCurrentThreadPriority()
    {
        sched_param param;
        S32 priority = 0;
        if (pthread_getschedparam(pthread_self(), &priority, &param) != 0)
        {
            MAZE_BP;
        }
        return GetUnixThreadPriorityFromNumber(param.sched_priority);
    }


    //////////////////////////////////////////
    // Class ThreadUnix
    //
    //////////////////////////////////////////
    ThreadUnix::ThreadUnix()
        : m_thread(0)
        , m_isActive(false)
    {
    }

    //////////////////////////////////////////
    ThreadUnix::~ThreadUnix()
    {
        wait();
    }

    //////////////////////////////////////////
    bool ThreadUnix::run()
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

        m_isActive = pthread_create(&m_thread, 0, &ThreadUnix::EntryPoint, this) == 0;

        if (!m_thread)
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ThreadUnix::terminate()
    {
        if (m_isActive)
        {

#if (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
            pthread_kill(m_thread, SIGUSR1);
#else
            pthread_cancel(m_thread);
#endif

            m_isActive = false;
        }
    }

    //////////////////////////////////////////
    void ThreadUnix::wait()
    {
        if (m_isActive)
        {
            // A thread cannot wait for itself!
            MAZE_DEBUG_BP_RETURN_IF(pthread_equal(pthread_self(), m_thread) != 0);

            pthread_join(m_thread, nullptr);
        }
    }

    //////////////////////////////////////////
    bool ThreadUnix::isRunning()
    {
        return m_isActive;
    }

    //////////////////////////////////////////
    void ThreadUnix::setPriority(ThreadPriority _priority)
    {
        if (!m_thread)
            return;

        sched_param param;
        memset(&param, 0, sizeof(sched_param));
        param.sched_priority = GetUnixThreadPriorityNumber(_priority);
        if (pthread_setschedparam(m_thread, ConvertUnixThreadPriority(_priority), &param) != 0)
        {
            MAZE_BP;
        }
    }

    //////////////////////////////////////////
    ThreadPriority ThreadUnix::getPriority() const
    {
        sched_param param;
        S32 priority = 0;
        if (pthread_getschedparam(m_thread, &priority, &param) != 0)
        {
            MAZE_BP;
        }
        return GetUnixThreadPriorityFromNumber(param.sched_priority);
    }


    //////////////////////////////////////////
    void* ThreadUnix::EntryPoint(void* _userData)
    {
        ThreadUnix* owner = static_cast<ThreadUnix*>(_userData);

#if (MAZE_PLATFORM != MAZE_PLATFORM_ANDROID)
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
#endif

        S32 result = owner->m_entryPoint->run();
        MAZE_UNUSED(result);


        return nullptr;
    }
} // namespace Maze
//////////////////////////////////////////
