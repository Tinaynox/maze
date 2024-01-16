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
#if (!defined(_MazeHeaderOpenAL_hpp_))
#define _MazeHeaderOpenAL_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#   include <al.h>
#   include <alc.h>
#elif (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN || MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
#   include <AL/al.h>
#   include <AL/alc.h>
#else
#   include <OpenAL/al.h>
#   include <OpenAL/alc.h>
#endif


//////////////////////////////////////////
#ifndef MAZE_AL_FUNCPTR
#   define MAZE_AL_REMOVE_FUNCPTR
#   if defined(_WIN32)
#       define MAZE_AL_FUNCPTR
#   else
#       define MAZE_AL_FUNCPTR
#   endif
#endif // MAZE_AL_FUNCPTR


using MZALboolean = Maze::S8;
using MZALchar = Maze::S8;
using MZALbyte = Maze::S8;
using MZALubyte = Maze::U8;
using MZALshort = Maze::S16;
using MZALushort = Maze::U16;
using MZALint = Maze::S32;
using MZALuint = Maze::U32;
using MZALsizei = Maze::S32;
using MZALenum = Maze::S32;
using MZALfloat = Maze::F32;
using MZALdouble = Maze::F64;
using MZALvoid = void;


#endif // _MazeHeaderOpenAL_hpp_
//////////////////////////////////////////
