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
#include "maze-core/system/unix/MazeTimerUnix.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TimerUnix
    //
    //////////////////////////////////////////
    TimerUnix::TimerUnix()
    {
        reset();
    }
    
    //////////////////////////////////////////
    TimerUnix::~TimerUnix()
    {
        
    }
    
    //////////////////////////////////////////
    void TimerUnix::reset()
    {
        m_zeroClock = clock();
        gettimeofday(&m_start, nullptr);
    }
    
    //////////////////////////////////////////
    U32 TimerUnix::getMilliseconds()
    {
        struct timeval now;
        gettimeofday(&now, nullptr);
        return static_cast<U32>((now.tv_sec - m_start.tv_sec) * 1000 + (now.tv_usec - m_start.tv_usec) / 1000);
    }
    
    //////////////////////////////////////////
    U32 TimerUnix::getMicroseconds()
    {
        struct timeval now;
        gettimeofday(&now, nullptr);
        return static_cast<U32>((now.tv_sec - m_start.tv_sec) * 1000000 + (now.tv_usec - m_start.tv_usec));
    }
    
    //////////////////////////////////////////
    U32 TimerUnix::getMillisecondsCPU()
    {
        clock_t newClock = clock();
        return (U32)((F32)(newClock - m_zeroClock) / ((F32)CLOCKS_PER_SEC/1000.0)) ;
    }
    
    //////////////////////////////////////////
    U32 TimerUnix::getMicrosecondsCPU()
    {
        clock_t newClock = clock();
        return (U32)((F32)(newClock - m_zeroClock) / ((F32)CLOCKS_PER_SEC/1000000.0)) ;
        
    }
    
} // namespace Maze
//////////////////////////////////////////
