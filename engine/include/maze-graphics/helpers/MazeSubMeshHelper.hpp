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
#if (!defined(_MazeSubMeshHelper_hpp_))
#define _MazeSubMeshHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);
    MAZE_USING_MANAGED_SHARED_PTR(SubMesh);


    //////////////////////////////////////////
    namespace SubMeshHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool GenerateTangentsAndBitangents(
            U16 const* _indices,
            Size _indicesCount,
            Vec3F const* _positions,
            Vec2F const* _uvs,
            Vec3F const* _normals,
            Size _verticesCount,
            Vector<Vec3F>& _outTangents,
            Vector<Vec3F>& _outBitangents);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void FlipX(
            RenderDrawTopology _drawTopology,
            Vector<U16>& _indices,
            Vector<Vec3F>* _positions,
            Vector<Vec3F>* _normals,
            Vector<Vec3F>* _tangents);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec2F NormalizeUV(Vec2F const& _uv);

    } // namespace SubMeshHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSubMeshHelper_hpp_
//////////////////////////////////////////
