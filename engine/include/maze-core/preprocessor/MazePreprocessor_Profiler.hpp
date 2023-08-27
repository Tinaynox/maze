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
#if (!defined(_MazePreprocessor_Profiler_hpp_))
#define _MazePreprocessor_Profiler_hpp_


//////////////////////////////////////////
#include "MazePreprocessor_Macro.hpp"


//////////////////////////////////////////
// Maze Profiler
//
//////////////////////////////////////////
#if (MAZE_PROFILER_OPTICK_ENABLED)
#   include "optick.h"
#   define MAZE_PROFILE_FRAME(FRAME_NAME, ...) OPTICK_FRAME(FRAME_NAME, __VA_ARGS__)
#   define MAZE_PROFILE_EVENT(...) OPTICK_EVENT(__VA_ARGS__)
#   define MAZE_PROFILE_THREAD(THREAD_NAME) OPTICK_THREAD(THREAD_NAME)
#else
#   define MAZE_PROFILE_FRAME(FRAME_NAME, ...)
#   define MAZE_PROFILE_EVENT(...)
#   define MAZE_PROFILE_THREAD(THREAD_NAME)
#endif


#endif // _MazePreprocessor_Profiler_hpp_
//////////////////////////////////////////
