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
#if (!defined(_MazeMeshHelper_hpp_))
#define _MazeMeshHelper_hpp_


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
    namespace MeshHelper
    {
        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateQuadSubMesh(
            Vec2F const& _size = Vec2F::c_one,
            Vec2F const& _positionShift = Vec2F::c_zero,
            bool _frontFaceToForward = false,
            Vec4F const& _uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f),
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateQuadMesh(
            Vec2F const& _size = Vec2F::c_one,
            Vec2F const& _positionShift = Vec2F::c_zero,
            bool _frontFaceToForward = false,
            Vec4F const& _uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f),
            Vec4F const& _color = Vec4F::c_one);


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateSlicedPanelSubMesh(
            Vec2F const& _size = Vec2F::c_one,
            Vec2F const& _positionShift = Vec2F::c_zero,
            SpriteSliceBorder const& _sliceBorder = SpriteSliceBorder(),
            Vec2F const& _originalSize = Vec2F::c_one,
            Vec4F const& _uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f),
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateSlicedPanelMesh(
            Vec2F const& _size = Vec2F::c_one,
            Vec2F const& _positionShift = Vec2F::c_zero,
            SpriteSliceBorder const& _sliceBorder = SpriteSliceBorder(),
            Vec2F const& _originalSize = Vec2F::c_one,
            Vec4F const& _uv = Vec4F(0.0f, 0.0f, 1.0f, 1.0f),
            Vec4F const& _color = Vec4F::c_one);


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateCubeSubMesh(
            Vec3F const& _size = Vec3F::c_one,
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateCubeMesh(
            Vec3F const& _size = Vec3F::c_one,
            Vec4F const& _color = Vec4F::c_one);


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateSpherifiedCubeSubMesh(
            F32 _radius = 0.5f,
            U32 _quality = 3,
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateSpherifiedCubeMesh(
            F32 _radius = 0.5f,
            U32 _quality = 3,
            Vec4F const& _color = Vec4F::c_one);


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateConeSubMesh(
            F32 _radius = 0.5f,
            F32 _height = 1.0f,
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateConeMesh(
            F32 _radius = 0.5f,
            F32 _height = 1.0f,
            Vec4F const& _color = Vec4F::c_one);


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateCylinderSubMesh(
            F32 _radius = 0.5f,
            F32 _height = 1.0f,
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateCylinderMesh(
            F32 _radius = 0.5f,
            F32 _height = 1.0f,
            Vec4F const& _color = Vec4F::c_one);


        //////////////////////////////////////////
        SubMeshPtr MAZE_GRAPHICS_API CreateTorusSubMesh(
            F32 _radius = 0.5f,
            F32 _csRadius = 0.1f,
            S32 _sides = 20,
            S32 _csSides = 20,
            Vec4F const& _color = Vec4F::c_one);

        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateTorusMesh(
            F32 _radius = 0.5f,
            F32 _csRadius = 0.1f,
            S32 _sides = 20,
            S32 _csSides = 20,
            Vec4F const& _color = Vec4F::c_one);



        //////////////////////////////////////////
        MeshPtr MAZE_GRAPHICS_API CreateCoordinateAxes(F32 _length = 100000.0f);

    } // namespace MeshHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshHelper_hpp_
//////////////////////////////////////////
