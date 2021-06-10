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
#include "MazeRenderSystemOpenGL3Header.hpp"


//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)


//////////////////////////////////////////
#include "maze-render-system-opengl3/win/wgl/MazeFunctionsOpenGL3WGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
MAZE_RENDER_SYSTEM_OPENGL3_API char const* (MAZE_GL_FUNCPTR* mzwglGetExtensionsString)(HDC) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API int (MAZE_GL_FUNCPTR* mzwglGetSwapInterval)()  = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API BOOL (MAZE_GL_FUNCPTR* mzwglSwapInterval)(int)  = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API BOOL (MAZE_GL_FUNCPTR* mzwglChoosePixelFormat)(HDC, const int*, const FLOAT*, UINT, int*, UINT*)  = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API BOOL (MAZE_GL_FUNCPTR* mzwglGetPixelFormatAttribfv)(HDC, int, int, UINT, const int*, FLOAT*) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API BOOL (MAZE_GL_FUNCPTR* mzwglGetPixelFormatAttribiv)(HDC, int, int, UINT, const int*, int*) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API HPBUFFERARB (MAZE_GL_FUNCPTR* mzwglCreatePbuffer)(HDC, int, int, int, const int*) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API BOOL (MAZE_GL_FUNCPTR* mzwglDestroyPbuffer)(HPBUFFERARB) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API HDC (MAZE_GL_FUNCPTR* mzwglGetPbufferDC)(HPBUFFERARB) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API BOOL (MAZE_GL_FUNCPTR* mzwglQueryPbuffer)(HPBUFFERARB, int, int*) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API int (MAZE_GL_FUNCPTR* mzwglReleasePbufferDC)(HPBUFFERARB, HDC) = nullptr;
MAZE_RENDER_SYSTEM_OPENGL3_API HGLRC (MAZE_GL_FUNCPTR* mzwglCreateContextAttribs)(HDC, HGLRC, const int*) = nullptr;


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignWGLFunction(ContextOpenGLPtr const& _renderContext, TGLFunction*& _functionPointer, CString _functionName)
    {
        _functionPointer = reinterpret_cast<TGLFunction*>(_renderContext->getFunction(_functionName));
        
        return _functionPointer != nullptr ? true
                                           : false;
    }
    
    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignWGLFunction(ContextOpenGLPtr const& _renderContext, TGLFunction*& _functionPointer, TGLFunction const* const _glFunctionPointer)
    {
        _functionPointer = _glFunctionPointer;
    }


    //////////////////////////////////////////
    bool AssignFunctionsOpenGL3WGL(ContextOpenGLPtr const& _renderContext)
    {
        if (!AssignFunctionsOpenGL3(_renderContext))
            return false;


        AssignWGLFunction(_renderContext, mzwglGetExtensionsString, "wglGetExtensionsStringARB");
        AssignWGLFunction(_renderContext, mzwglGetSwapInterval, "wglGetSwapIntervalEXT");
        AssignWGLFunction(_renderContext, mzwglSwapInterval, "wglSwapIntervalEXT");
        AssignWGLFunction(_renderContext, mzwglChoosePixelFormat, "wglChoosePixelFormatARB");
        AssignWGLFunction(_renderContext, mzwglGetPixelFormatAttribfv, "wglGetPixelFormatAttribfvARB");
        AssignWGLFunction(_renderContext, mzwglGetPixelFormatAttribiv, "wglGetPixelFormatAttribivARB");
        AssignWGLFunction(_renderContext, mzwglCreatePbuffer, "wglCreatePbufferARB");
        AssignWGLFunction(_renderContext, mzwglDestroyPbuffer, "wglDestroyPbufferARB");
        AssignWGLFunction(_renderContext, mzwglGetPbufferDC, "wglGetPbufferDCARB");
        AssignWGLFunction(_renderContext, mzwglQueryPbuffer, "wglQueryPbufferARB");
        AssignWGLFunction(_renderContext, mzwglReleasePbufferDC, "wglReleasePbufferDCARB");
        AssignWGLFunction(_renderContext, mzwglCreateContextAttribs, "wglCreateContextAttribsARB");

        return true;
    }
    

} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
//////////////////////////////////////////
