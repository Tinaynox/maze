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
#include "maze-render-system-opengl-core/MazeCompareFunctionOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum GetOpenGLCompareFunction(CompareFunction _compareFunction)
    {
        switch (_compareFunction)
        {
            case CompareFunction::Disabled:         return MAZE_GL_ALWAYS;
            case CompareFunction::Never:            return MAZE_GL_NEVER;
            case CompareFunction::Less:             return MAZE_GL_LESS;
            case CompareFunction::Equal:            return MAZE_GL_EQUAL;
            case CompareFunction::LessEqual:        return MAZE_GL_LEQUAL;
            case CompareFunction::Greater:          return MAZE_GL_GREATER;
            case CompareFunction::NotEqual:         return MAZE_GL_NOTEQUAL;
            case CompareFunction::GreaterEqual:     return MAZE_GL_GEQUAL;
            case CompareFunction::Always:           return MAZE_GL_ALWAYS;
            default:
            {
                MAZE_ERROR("Undefined CompareFunction: %d!", (S32)_compareFunction);
            }
        }

        return MAZE_GL_ALWAYS;
    }
    

} // namespace Maze
//////////////////////////////////////////
