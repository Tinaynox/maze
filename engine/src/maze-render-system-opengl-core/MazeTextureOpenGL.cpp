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
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLuint GetOpenGLFilter(TextureFilter _value)
    {

        switch (_value)
        {
            case TextureFilter::Nearest: return MAZE_GL_NEAREST;
            case TextureFilter::Linear: return MAZE_GL_LINEAR;
            case TextureFilter::NearestMipmapNearest: return MAZE_GL_NEAREST_MIPMAP_NEAREST;
            case TextureFilter::LinearMipmapNearest: return MAZE_GL_LINEAR_MIPMAP_NEAREST;
            case TextureFilter::NearestMipmapLinear: return MAZE_GL_NEAREST_MIPMAP_LINEAR;
            case TextureFilter::LinearMipmapLinear: return MAZE_GL_LINEAR_MIPMAP_LINEAR;
            default:
            {
                MAZE_ERROR("Unsupported TextureFilter: %d", (S32)_value);
            }
        }

        return MAZE_GL_LINEAR;
    }

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLuint GetOpenGLWrap(TextureWrap _value)
    {
        switch (_value)
        {
            case TextureWrap::Repeat: return MAZE_GL_REPEAT;
            case TextureWrap::MirroredRepeat: return MAZE_GL_MIRRORED_REPEAT;
            case TextureWrap::ClampToEdge: return MAZE_GL_CLAMP_TO_EDGE;
            case TextureWrap::ClampToBorder: return MAZE_GL_CLAMP_TO_BORDER;

            default:
            {
                MAZE_ERROR("Unsupported TextureWrap: %d", (S32)_value);
                break;
            }
        }

        return MAZE_GL_REPEAT;
    }
    
} // namespace Maze
//////////////////////////////////////////
