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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStencilOperationOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum GetOpenGLStencilOperation(StencilOperation _stencilOperation)
    {
        switch (_stencilOperation)
        {
            case StencilOperation::Keep:             return MAZE_GL_KEEP;
            case StencilOperation::Zero:             return MAZE_GL_ZERO;
            case StencilOperation::Replace:          return MAZE_GL_REPLACE;
            case StencilOperation::Increment:        return MAZE_GL_INCR;
            case StencilOperation::Decrement:        return MAZE_GL_DECR;
            case StencilOperation::Invert:           return MAZE_GL_INVERT;
            case StencilOperation::IncrementWrap:    return MAZE_GL_INCR_WRAP;
            case StencilOperation::DecrementWrap:    return MAZE_GL_DECR_WRAP;
            default:
            {
                MAZE_ERROR("Undefined StencilOperation: %d!", (S32)_stencilOperation);
            }
        }

        return MAZE_GL_KEEP;
    }


} // namespace Maze
//////////////////////////////////////////
