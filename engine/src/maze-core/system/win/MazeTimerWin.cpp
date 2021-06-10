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
#include "maze-core/system/win/MazeTimerWin.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
    //////////////////////////////////////////
    static bool ProcessQPCAffinityWorkaroundRequired()
    {
#   if (MAZE_CPU == MAZE_CPU_X86)
        bool hasTSC = SystemManager::GetInstancePtr()->getCPUManager()->getCPUController()->hasTSC();

        SYSTEM_INFO sysInfo;
        return (DWORD)(LOBYTE(LOWORD(GetVersion()))) < 6 // version.major < Vista
            && ((GetSystemInfo(&sysInfo), sysInfo.dwNumberOfProcessors > 1)
            || !hasTSC);
#   else
        return false;
#   endif
    }

    //////////////////////////////////////////
    static bool IsQPCAffinityWorkaroundRequired()
    {
        static bool isQPCAffinityWorkaroundRequired = ProcessQPCAffinityWorkaroundRequired();
        return isQPCAffinityWorkaroundRequired;
    }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED



    //////////////////////////////////////////
    // Class TimerWin
    //
    //////////////////////////////////////////
    TimerWin::TimerWin()
#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
        : m_timerMask(0)
#endif
    {
        reset();
    }
    
    //////////////////////////////////////////
    TimerWin::~TimerWin()
    {

    }

    //////////////////////////////////////////
    void TimerWin::reset()
    {
#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
        if (IsQPCAffinityWorkaroundRequired())
        {
            // Get the current process core mask
            DWORD_PTR procMask;
            DWORD_PTR sysMask;
            GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

            // If procMask is 0, consider there is only one core available
            // (using 0 as procMask will cause an infinite loop below)
            if (procMask == 0)
                procMask = 1;

            // Find the lowest core that this process uses
            if (m_timerMask == 0)
            {
                m_timerMask = 1;
                while ((m_timerMask & procMask) == 0)
                {
                    m_timerMask <<= 1;
                }
            }
        }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED

        

        {
#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
            HANDLE thread;
            DWORD_PTR oldMask;
            if (IsQPCAffinityWorkaroundRequired())
            {
                // Set affinity to the first core
                thread = GetCurrentThread();
                oldMask = SetThreadAffinityMask(thread, m_timerMask);
            }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED

            // Get the constant frequency
            QueryPerformanceFrequency(&m_frequency);

            // Query the timer
            QueryPerformanceCounter(&m_startTime);
            m_startTick = GetTickCount();

#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
            if (IsQPCAffinityWorkaroundRequired())
            {
                // Reset affinity
                SetThreadAffinityMask(thread, oldMask);
            }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED
        }

        m_lastTime = 0;
        m_zeroClock = clock();
    }

    //////////////////////////////////////////
    U32 TimerWin::getMilliseconds()
    {
        LARGE_INTEGER curTime;

        {
#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
            HANDLE thread;
            DWORD_PTR oldMask;
            if (IsQPCAffinityWorkaroundRequired())
            {
                thread = GetCurrentThread();
                // Set affinity to the first core
                oldMask = SetThreadAffinityMask(thread, m_timerMask);
            }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED

            // Query the timer
            QueryPerformanceCounter(&curTime);

#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
            if (IsQPCAffinityWorkaroundRequired())
            {
                // Reset affinity
                SetThreadAffinityMask(thread, oldMask);
            }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED
        }

        LONGLONG newTime = curTime.QuadPart - m_startTime.QuadPart;

        // scale by 1000 for milliseconds
        U32 newTicks = (U32)(1000 * newTime / m_frequency.QuadPart);

        // detect and compensate for performance counter leaps
        // (surprisingly common, see Microsoft KB: Q274323)
        U32 check = GetTickCount() - m_startTick;
        S32 msecOff = (S32)(newTicks - check);
        if (msecOff < -100 || msecOff > 100)
        {
            // We must keep the timer running forward
            LONGLONG adjust = Math::Min(msecOff * m_frequency.QuadPart / 1000, newTime - m_lastTime);
            m_startTime.QuadPart += adjust;
            newTime -= adjust;

            // Re-calculate milliseconds
            newTicks = (U32)(1000 * newTime / m_frequency.QuadPart);
        }

        // Record last time for adjust
        m_lastTime = newTime;

        return newTicks;
    }

    //////////////////////////////////////////
    U32 TimerWin::getMicroseconds()
    {
        LARGE_INTEGER curTime;

        {
#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
            HANDLE thread;
            DWORD_PTR oldMask;
            if (IsQPCAffinityWorkaroundRequired())
            {
                thread = GetCurrentThread();
                // Set affinity to the first core
                oldMask = SetThreadAffinityMask(thread, m_timerMask);
            }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED

            // Query the timer
            QueryPerformanceCounter(&curTime);

#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
            if (IsQPCAffinityWorkaroundRequired())
            {
                // Reset affinity
                SetThreadAffinityMask(thread, oldMask);
            }
#endif // MAZE_TIMER_AFFINITY_MASK_ENABLED
        }

        LONGLONG newTime = curTime.QuadPart - m_startTime.QuadPart;

        // get milliseconds to check against GetTickCount
        U32 newTicks = (U32)(1000 * newTime / m_frequency.QuadPart);

        // detect and compensate for performance counter leaps
        // (surprisingly common, see Microsoft KB: Q274323)
        U32 check = GetTickCount() - m_startTick;
        S32 msecOff = (S32)(newTicks - check);
        if (msecOff < -100 || msecOff > 100)
        {
            // We must keep the timer running forward :)
            LONGLONG adjust = Math::Min(msecOff * m_frequency.QuadPart / 1000, newTime - m_lastTime);
            m_startTime.QuadPart += adjust;
            newTime -= adjust;
        }

        // Record last time for adjust
        m_lastTime = newTime;

        // scale by 1000000 for microseconds
        U32 newMicro = (U32)(1000000 * newTime / m_frequency.QuadPart);

        return newMicro;
    }

    //////////////////////////////////////////
    U32 TimerWin::getMillisecondsCPU()
    {
        clock_t newClock = clock();
        return (U32)((F32)(newClock - m_zeroClock) / ((F32)CLOCKS_PER_SEC / 1000.0));
    }

    //////////////////////////////////////////
    U32 TimerWin::getMicrosecondsCPU()
    {
        clock_t newClock = clock();
        return (U32)((F32)(newClock - m_zeroClock) / ((F32)CLOCKS_PER_SEC / 1000000.0));
    }

} // namespace Maze
//////////////////////////////////////////
