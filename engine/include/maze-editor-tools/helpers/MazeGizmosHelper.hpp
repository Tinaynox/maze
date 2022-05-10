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
#if (!defined(_MazeGizmosHelper_hpp_))
#define _MazeGizmosHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-editor-tools/ecs/systems/MazeGizmosDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Mesh);
    MAZE_USING_SHARED_PTR(SubMesh);


    //////////////////////////////////////////
    namespace GizmosHelper
    {
        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SetColor(ColorF128 const& _color);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ColorF128 const& GetColor();

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void PushTransform(Mat4DF const& _mat);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void PopTransform();

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            Vec3DF const& _point0,
            Vec3DF const& _point1,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            F32 _x0,
            F32 _y0,
            F32 _x1,
            F32 _y1,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            Vec2DF const& _point0,
            Vec2DF const& _point1,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            F32 _x0,
            F32 _y0,
            F32 _z0,
            F32 _x1,
            F32 _y1,
            F32 _z1,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawRay(
            Vec3DF const& _point,
            Vec3DF const& _delta,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireQuad(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec2DF const& _scale,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCircle(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireHemicircle(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCube(
            Vec3DF const& _position,
            Vec3DF const& _scale,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireSphere(
            Vec3DF const& _position,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireHemisphere(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCylinder(
            Vec3DF const& _start,
            Vec3DF const& _end,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCone(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _angle,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireTruncatedCone(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            F32 _angle,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireTorus(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            F32 _torusRadius,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireArrow(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCapsule(
            Vec3DF const& _start,
            Vec3DF const& _end,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawAABB(
            AABB2D const& _aabb,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawAABB(
            AABB3D const& _aabb,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawTriangle(
            Vec3DF const& _point0,
            Vec3DF const& _point1,
            Vec3DF const& _point2,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawQuad(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec2DF const& _scale,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawSphere(
            Vec3DF const& _position,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawCone(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void DrawCone(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque)
        {
            DrawCone(_position, _forward, _forward.perpendicular(), _radius, _height, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawCylinder(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void DrawCylinder(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            GizmosDrawer::MeshRenderMode _renderMode = GizmosDrawer::MeshRenderMode::Opaque)
        {
            DrawCylinder(_position, _forward, _forward.perpendicular(), _radius, _height, _color, _duration, _renderMode);
        }

    } // namespace GizmosHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmosHelper_hpp_
//////////////////////////////////////////
