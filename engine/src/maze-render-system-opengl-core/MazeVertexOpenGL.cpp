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
#include "maze-render-system-opengl-core/MazeVertexOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum GetVertexAttributeTypeOpenGL(VertexAttributeType _type)
    {
        switch (_type)
        {
            case VertexAttributeType::S8:     return MAZE_GL_BYTE;
            case VertexAttributeType::U8:     return MAZE_GL_UNSIGNED_BYTE;
            case VertexAttributeType::S16:    return MAZE_GL_SHORT;
            case VertexAttributeType::U16:    return MAZE_GL_UNSIGNED_SHORT;
            case VertexAttributeType::S32:    return MAZE_GL_INT;
            case VertexAttributeType::U32:    return MAZE_GL_UNSIGNED_INT;
            case VertexAttributeType::F16:    return MAZE_GL_HALF_FLOAT;
            case VertexAttributeType::F32:    return MAZE_GL_FLOAT;
        }

        MAZE_ERROR_RETURN_VALUE(0, "Unsupported VertexAttributeType!");
    }


} // namespace Maze
//////////////////////////////////////////
