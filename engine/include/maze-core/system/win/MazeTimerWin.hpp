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
#pragma once
#if (!defined(_MazeTimerWin_hpp_))
#define _MazeTimerWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
#if (_WIN32_WINNT < _WIN32_WINNT_VISTA)
#    define MAZE_TIMER_AFFINITY_MASK_ENABLED (1)
#else
#    define MAZE_TIMER_AFFINITY_MASK_ENABLED (0)
#endif

    //////////////////////////////////////////
    // Class TimerWin
    //
    //////////////////////////////////////////
    class MAZE_CORE_API TimerWin
    {
    public:

        //////////////////////////////////////////
        TimerWin();

        //////////////////////////////////////////
        ~TimerWin();


        //////////////////////////////////////////
        TimerWin(TimerWin const&) = delete;
        
        //////////////////////////////////////////
        TimerWin(TimerWin&&) = delete;

        //////////////////////////////////////////
        TimerWin& operator=(TimerWin const&) = delete;
        
        //////////////////////////////////////////
        TimerWin& operator=(TimerWin&&) = delete;


        //////////////////////////////////////////
        void reset();

        //////////////////////////////////////////
        U32 getMilliseconds();

        //////////////////////////////////////////
        U32 getMicroseconds();

        //////////////////////////////////////////
        U32 getMillisecondsCPU();

        //////////////////////////////////////////
        U32 getMicrosecondsCPU();    

    protected:
        clock_t m_zeroClock;

        DWORD m_startTick;
        LONGLONG m_lastTime;
        LARGE_INTEGER m_startTime;
        LARGE_INTEGER m_frequency;

#if (MAZE_TIMER_AFFINITY_MASK_ENABLED)
        DWORD_PTR m_timerMask;
#endif // (MAZE_TIMER_AFFINITY_MASK_ENABLED)
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeTimerWin_hpp_
//////////////////////////////////////////
