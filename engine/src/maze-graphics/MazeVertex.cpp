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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_GRAPHICS_API CString GetVertexAttributeName(VertexAttributeSemantic _semantic)
    {
        switch (_semantic)
        {
            case VertexAttributeSemantic::Position:      return "a_position";
            case VertexAttributeSemantic::Normal:        return "a_normal";
            case VertexAttributeSemantic::Tangent:       return "a_tangent";
            case VertexAttributeSemantic::Bitangent:     return "a_bitangent";
            case VertexAttributeSemantic::Color:         return "a_color";
            case VertexAttributeSemantic::TexCoords0:    return "a_texCoords0";
            case VertexAttributeSemantic::TexCoords1:    return "a_texCoords1";
            case VertexAttributeSemantic::TexCoords2:    return "a_texCoords2";
            case VertexAttributeSemantic::TexCoords3:    return "a_texCoords3";
            case VertexAttributeSemantic::TexCoords4:    return "a_texCoords4";
            case VertexAttributeSemantic::TexCoords5:    return "a_texCoords5";
            case VertexAttributeSemantic::TexCoords6:    return "a_texCoords6";
            case VertexAttributeSemantic::TexCoords7:    return "a_texCoords7";
            default:
            {
                MAZE_ERROR("Unsupported VertexAttributeSemantic: %d", (S32)_semantic);
            }
        }
    
        return nullptr;
    }
    
    //////////////////////////////////////////
    MAZE_GRAPHICS_API Size GetVertexAttributeTypeSize(VertexAttributeType _type)
    {
        switch (_type)
        {
            case VertexAttributeType::S8:         return 1;
            case VertexAttributeType::U8:         return 1;
            case VertexAttributeType::S16:        return 2;
            case VertexAttributeType::U16:        return 2;
            case VertexAttributeType::S32:        return 4;
            case VertexAttributeType::U32:        return 4;
            case VertexAttributeType::F16:        return 2;
            case VertexAttributeType::F32:        return 4;
        }

        MAZE_ERROR_RETURN_VALUE(0, "Unsupported VertexAttributeType - %d", (S32)_type);
    }

    //////////////////////////////////////////
    // Struct VertexAttributeDescription
    //
    //
    //////////////////////////////////////////
    VertexAttributeDescription::VertexAttributeDescription()
        : semantic(VertexAttributeSemantic::Position)
        , count(0u)
        , type(VertexAttributeType::S32)
        , normalized(false)
        , stride(0)
        , offset(0)
    {

    }

    //////////////////////////////////////////
    VertexAttributeDescription::VertexAttributeDescription(
        VertexAttributeSemantic _semantic,
        U8 _count,
        VertexAttributeType _type,
        bool _normalized,
        Size _stride,
        Size _offset)
        : semantic(_semantic)
        , count(_count)
        , type(_type)
        , normalized(_normalized)
        , stride(_stride)
        , offset(_offset)
    {

    }

} // namespace Maze
//////////////////////////////////////////
