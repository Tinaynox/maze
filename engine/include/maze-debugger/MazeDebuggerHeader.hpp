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
#if (!defined(_MazeDebuggerHeader_hpp_))
#define _MazeDebuggerHeader_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_Platform.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"


//////////////////////////////////////////
#if (defined(MAZE_DEBUGGER_EXPORTS))
    #define MAZE_DEBUGGER_API MAZE_API_EXPORT
#else
    #define MAZE_DEBUGGER_API MAZE_API_IMPORT
#endif


//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN
#    endif
#    if !defined(NOMINMAX) && defined(_MSC_VER)
#        define NOMINMAX
#    endif
#    include "Windows.h"
#    undef far
#    undef near
#   undef NEAR
#   define NEAR
#   undef FAR
#   define FAR
#endif


#endif // _MazeDebuggerHeader_hpp_
//////////////////////////////////////////
