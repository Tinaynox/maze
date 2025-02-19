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
#if (!defined(_MazeRenderSystemOpenGL3Header_hpp_))
#define _MazeRenderSystemOpenGL3Header_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_Platform.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Profiler.hpp"


//////////////////////////////////////////
#if defined(MAZE_RENDER_SYSTEM_OPENGL3_EXPORTS)
#   define MAZE_RENDER_SYSTEM_OPENGL3_API MAZE_API_EXPORT
#else
#   define MAZE_RENDER_SYSTEM_OPENGL3_API MAZE_API_IMPORT
#endif


//////////////////////////////////////////
#include "maze-core/system/MazeSystemHeader.hpp"


#if (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
#   include <OpenGL/gl.h>
#   include <OpenGL/OpenGL.h>
#elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
#   include <GLES/gl.h>
#   include <GLES2/gl2.h>
#   include <GLES3/gl3.h>
#   include <GLES/glplatform.h>
#   include <GLES/glext.h>
#   include <GLES2/gl2platform.h>
#   include <GLES2/gl2ext.h>
#   include <GLES3/gl3platform.h>
#   include <GLES3/gl3ext.h>
#elif (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
#   include <GLES/gl.h>
#   include <GLES/glext.h>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#   include <GLES3/gl3.h>
#   include <GLES3/gl2ext.h>
#elif (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif


#endif // _MazeRenderSystemOpenGL3Header_hpp_
//////////////////////////////////////////
