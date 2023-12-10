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
#include "maze-render-system-opengl-core/MazeShaderUniformVariantOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API ShaderUniformType GetShaderUniformTypeFromOpenGL(MZGLenum _uniformType)
    {
        switch (_uniformType)
        {
            case MAZE_GL_INT:               return ShaderUniformType::UniformS32;
            case MAZE_GL_FLOAT:             return ShaderUniformType::UniformF32;
            case MAZE_GL_DOUBLE:            return ShaderUniformType::UniformF64;
            case MAZE_GL_BOOL:              return ShaderUniformType::UniformBool;
            case MAZE_GL_SAMPLER_2D:        return ShaderUniformType::UniformTexture2D;
            case MAZE_GL_SAMPLER_CUBE:      return ShaderUniformType::UniformTextureCube;
            case MAZE_GL_INT_VEC2:          return ShaderUniformType::UniformVec2S32;
            case MAZE_GL_INT_VEC3:          return ShaderUniformType::UniformVec3S32;
            case MAZE_GL_INT_VEC4:          return ShaderUniformType::UniformVec4S32;
            case MAZE_GL_UNSIGNED_INT_VEC2: return ShaderUniformType::UniformVec2U32;
            case MAZE_GL_UNSIGNED_INT_VEC3: return ShaderUniformType::UniformVec3U32;
            case MAZE_GL_UNSIGNED_INT_VEC4: return ShaderUniformType::UniformVec4U32;
            case MAZE_GL_FLOAT_VEC2:        return ShaderUniformType::UniformVec2F32;
            case MAZE_GL_FLOAT_VEC3:        return ShaderUniformType::UniformVec3F32;
            case MAZE_GL_FLOAT_VEC4:        return ShaderUniformType::UniformVec4F32;
            case MAZE_GL_BOOL_VEC2:         return ShaderUniformType::UniformVec2B;
            case MAZE_GL_BOOL_VEC3:         return ShaderUniformType::UniformVec3B;
            case MAZE_GL_BOOL_VEC4:         return ShaderUniformType::UniformVec4B;
            case MAZE_GL_FLOAT_MAT3:        return ShaderUniformType::UniformMat3F32;
            case MAZE_GL_FLOAT_MAT4:        return ShaderUniformType::UniformMat4F32;
            default:
            {
                MAZE_ERROR_RETURN_VALUE(ShaderUniformType::None, "Undefined ShaderUniformType: %d!", (S32)_uniformType);
            }
        }

        return ShaderUniformType::None;
    }
    

} // namespace Maze
//////////////////////////////////////////
