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
#if (!defined(_MazeFunctionsOpenGL3WGL_hpp_))
#define _MazeFunctionsOpenGL3WGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeFunctionsOpenGL3.hpp"


//////////////////////////////////////////
DECLARE_HANDLE(HPBUFFERARB);


//////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    
//////////////////////////////////////////
MAZE_RENDER_SYSTEM_OPENGL3_API extern char const* (MAZE_GL_FUNCPTR* mzwglGetExtensionsString)(HDC);
MAZE_RENDER_SYSTEM_OPENGL3_API extern int (MAZE_GL_FUNCPTR* mzwglGetSwapInterval)();
MAZE_RENDER_SYSTEM_OPENGL3_API extern BOOL (MAZE_GL_FUNCPTR* mzwglSwapInterval)(int);
MAZE_RENDER_SYSTEM_OPENGL3_API extern BOOL (MAZE_GL_FUNCPTR* mzwglChoosePixelFormat)(HDC, const int*, const FLOAT*, UINT, int*, UINT*);
MAZE_RENDER_SYSTEM_OPENGL3_API extern BOOL (MAZE_GL_FUNCPTR* mzwglGetPixelFormatAttribfv)(HDC, int, int, UINT, const int*, FLOAT*);
MAZE_RENDER_SYSTEM_OPENGL3_API extern BOOL (MAZE_GL_FUNCPTR* mzwglGetPixelFormatAttribiv)(HDC, int, int, UINT, const int*, int*);
MAZE_RENDER_SYSTEM_OPENGL3_API extern HPBUFFERARB (MAZE_GL_FUNCPTR* mzwglCreatePbuffer)(HDC, int, int, int, const int*);
MAZE_RENDER_SYSTEM_OPENGL3_API extern BOOL (MAZE_GL_FUNCPTR* mzwglDestroyPbuffer)(HPBUFFERARB);
MAZE_RENDER_SYSTEM_OPENGL3_API extern HDC (MAZE_GL_FUNCPTR* mzwglGetPbufferDC)(HPBUFFERARB);
MAZE_RENDER_SYSTEM_OPENGL3_API extern BOOL (MAZE_GL_FUNCPTR* mzwglQueryPbuffer)(HPBUFFERARB, int, int*);
MAZE_RENDER_SYSTEM_OPENGL3_API extern int (MAZE_GL_FUNCPTR* mzwglReleasePbufferDC)(HPBUFFERARB, HDC);
MAZE_RENDER_SYSTEM_OPENGL3_API extern HGLRC (MAZE_GL_FUNCPTR* mzwglCreateContextAttribs)(HDC, HGLRC, const int*);


#ifdef __cplusplus
}
#endif // __cplusplus


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    bool AssignFunctionsOpenGL3WGL(ContextOpenGLPtr const& _renderContext);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFunctionsOpenGL3WGL_hpp_
//////////////////////////////////////////
