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
#pragma once
#if (!defined(_MazeVertex_hpp_))
#define _MazeVertex_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    enum class MAZE_GRAPHICS_API VertexAttributeSemantic : U8
    {
        Position = 0,
        Normal,
        Tangent,
        Bitangent,
        Color,
        TexCoords0,
        TexCoords1,
        TexCoords2,
        TexCoords3,
        TexCoords4,
        TexCoords5,
        TexCoords6,
        TexCoords7,

        MAX
    };

    //////////////////////////////////////////
    MAZE_GRAPHICS_API CString GetVertexAttributeName(VertexAttributeSemantic _semantic);


    //////////////////////////////////////////
    enum class MAZE_GRAPHICS_API VertexAttributeType : U8
    {
        S8,
        U8,
        S16,
        U16,
        S32,
        U32,
        F16,
        F32
    };

    //////////////////////////////////////////
    MAZE_GRAPHICS_API Size GetVertexAttributeTypeSize(VertexAttributeType _type);


    //////////////////////////////////////////
    // Struct VertexAttributeDescription
    //
    // BUFFER:    [    Vertex 1    ][    Vertex 2    ][    Vertex 3    ]
    //            [X][Y][Z][R][G][B][X][Y][Z][R][G][B][X][Y][Z][R][G][B]
    // POSITION:  |-----STRIDE---->|
    //            |OFFSET
    // COLOR:              |-----STRIDE---->|
    //            |OFFSET->|
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API VertexAttributeDescription
    {
        //////////////////////////////////////////
        VertexAttributeDescription();

        //////////////////////////////////////////
        VertexAttributeDescription(
            VertexAttributeSemantic _semantic,
            U8 _count,
            VertexAttributeType _type,
            bool _normalized,
            Size _stride,
            Size _offset);

        VertexAttributeSemantic semantic;
        U8 count;
        VertexAttributeType type;
        bool normalized;
        Size stride;
        Size offset;
    };

    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API VertexDataDescription
    {
        Size bytesPerVertex;
        VertexAttributeDescription const* vertexAttributesDescriptions;
        Size vertexElementsCount;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertex_hpp_
//////////////////////////////////////////
