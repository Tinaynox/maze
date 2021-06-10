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
#include "maze-render-system-opengl-core/MazeBlendModeOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum GetOpenGLBlendFactor(BlendFactor _blendFactor)
    {
        switch (_blendFactor)
        {
            case BlendFactor::One:                     return MAZE_GL_ONE;
            case BlendFactor::Zero:                    return MAZE_GL_ZERO;
            case BlendFactor::SrcColor:                return MAZE_GL_SRC_COLOR;
            case BlendFactor::SrcAlpha:                return MAZE_GL_SRC_ALPHA;
            case BlendFactor::DstColor:                return MAZE_GL_DST_COLOR;
            case BlendFactor::DstAlpha:                return MAZE_GL_DST_ALPHA;
            case BlendFactor::OneMinusSrcColor:        return MAZE_GL_ONE_MINUS_SRC_COLOR;
            case BlendFactor::OneMinusSrcAlpha:        return MAZE_GL_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::OneMinusDstColor:        return MAZE_GL_ONE_MINUS_DST_COLOR;
            case BlendFactor::OneMinusDstAlpha:        return MAZE_GL_ONE_MINUS_DST_ALPHA;
            default:
            {
                MAZE_ERROR("Undefined BlendFactor: %d!", (S32)_blendFactor);
            }
        }

        return MAZE_GL_ONE;
    }
    

} // namespace Maze
//////////////////////////////////////////
