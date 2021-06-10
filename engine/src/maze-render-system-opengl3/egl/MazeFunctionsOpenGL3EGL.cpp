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
#if (  (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)       \
    || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)    \
    || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))


//////////////////////////////////////////
#include "maze-render-system-opengl3/egl/MazeFunctionsOpenGL3EGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignEGLFunction(ContextOpenGLPtr const& _renderContext, TGLFunction*& functionPointer, CString _functionName)
    {
        functionPointer = reinterpret_cast<TGLFunction*>(_renderContext->getFunction(_functionName));
        
        return functionPointer != nullptr ? true
                                          : false;
    }
    
    //////////////////////////////////////////
    template <typename TGLFunction>
    inline bool AssignEGLFunction(ContextOpenGLPtr const& renderContext, TGLFunction*& _functionPointer, TGLFunction const* const _glFunctionPointer)
    {
        _functionPointer = _glFunctionPointer;
    }


    //////////////////////////////////////////
    bool AssignFunctionsOpenGL3EGL(ContextOpenGLPtr const& _renderContext)
    {
        if (!AssignFunctionsOpenGL3(_renderContext))
            return false;


        return true;
    }

    
} // namespace Maze
//////////////////////////////////////////


#endif  // if (  (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)       \
        //    || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)    \
        //    || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))
//////////////////////////////////////////
