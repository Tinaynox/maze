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
#include "maze-core/system/emscripten/MazeTimerEmscripten.hpp"
#include <emscripten/emscripten.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TimerEmscripten
    //
    //////////////////////////////////////////
    TimerEmscripten::TimerEmscripten()
    {
        reset();
    }
    
    //////////////////////////////////////////
    TimerEmscripten::~TimerEmscripten()
    {
        
    }
    
    //////////////////////////////////////////
    void TimerEmscripten::reset()
    {
        m_zeroClock = clock();
        m_start = emscripten_get_now();
    }
    
    //////////////////////////////////////////
    U32 TimerEmscripten::getMilliseconds()
    {
        return emscripten_get_now() - m_start;
    }
    
    //////////////////////////////////////////
    U32 TimerEmscripten::getMicroseconds()
    {
        return (emscripten_get_now() - m_start) * 1000.0;
    }
    
    //////////////////////////////////////////
    U32 TimerEmscripten::getMillisecondsCPU()
    {
        clock_t newClock = clock();
        return (U32)((F32)( newClock - m_zeroClock) / ((F32)CLOCKS_PER_SEC / 1000.0));
    }
    
    //////////////////////////////////////////
    U32 TimerEmscripten::getMicrosecondsCPU()
    {
        clock_t newClock = clock();
        return (U32)((F32)(newClock - m_zeroClock) / ((F32)CLOCKS_PER_SEC / 1000000.0));
        
    }

} // namespace Maze
//////////////////////////////////////////
