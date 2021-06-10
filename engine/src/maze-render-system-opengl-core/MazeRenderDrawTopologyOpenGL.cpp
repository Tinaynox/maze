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
#include "maze-render-system-opengl-core/MazeRenderDrawTopologyOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLenum GetRenderDrawTopologyOpenGL(RenderDrawTopology _topology)
    {
        switch (_topology)
        {
            case RenderDrawTopology::Points:       return MAZE_GL_POINTS;
            case RenderDrawTopology::Lines:        return MAZE_GL_LINES;
            case RenderDrawTopology::LinesLoop:    return MAZE_GL_LINE_LOOP;
            case RenderDrawTopology::LinesStrip:   return MAZE_GL_LINE_STRIP;
            case RenderDrawTopology::Triangles:    return MAZE_GL_TRIANGLES;
            case RenderDrawTopology::TrianglesFan: return MAZE_GL_TRIANGLE_FAN;
            case RenderDrawTopology::Quads:        return MAZE_GL_QUADS;
            default:
            {
                MAZE_ERROR("Unsupported RenderDrawTopology!");
            }
        }

        return 0;
    }


} // namespace Maze
//////////////////////////////////////////
