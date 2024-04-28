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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/ecs/components/MazeGizmosController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace GizmosHelper
    {
        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API GizmosDrawerPtr const& GetDrawer()
        {
            static GizmosDrawerPtr const nullPointer;
            if (EditorToolsManager::GetInstancePtr() &&
                EditorToolsManager::GetInstancePtr()->getGizmosController())
                return EditorToolsManager::GetInstancePtr()->getGizmosController()->getDrawer();
            return nullPointer;
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void SetColor(ColorF128 const& _color)
        {
            GetDrawer()->setColor(_color);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ColorF128 const& GetColor()
        {
            return GetDrawer()->getColor();
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void PushTransform(Mat4F const& _mat)
        {
            GetDrawer()->pushTransform(_mat);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void PopTransform()
        {
            GetDrawer()->popTransform();
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            Vec3F const& _point0,
            Vec3F const& _point1,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawLine(_point0, _point1, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            F32 _x0,
            F32 _y0,
            F32 _x1,
            F32 _y1,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawLine(_x0, _y0, _x1, _y1, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            Vec2F const& _point0,
            Vec2F const& _point1,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawLine(_point0, _point1, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawLine(
            F32 _x0,
            F32 _y0,
            F32 _z0,
            F32 _x1,
            F32 _y1,
            F32 _z1,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawLine(_x0, _y0, _z0, _x1, _y1, _z1, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawRay(
            Vec3F const& _point,
            Vec3F const& _delta,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawRay(_point, _delta, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireQuad(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            Vec2F const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireQuad(_position, _forward, _up, _scale, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCircle(
            Vec3F const& _position,
            Vec3F const& _direction,
            F32 _radius,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireCircle(_position, _direction, _radius, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireHemicircle(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            F32 _radius,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireHemicircle(_position, _forward, _up, _radius, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCube(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            Vec3F const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireCube(_position, _forward, _up, _scale, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireSphere(
            Vec3F const& _position,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireSphere(_position, _radius, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireHemisphere(
            Vec3F const& _position,
            Vec3F const& _direction,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireHemisphere(_position, _direction, _radius, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCylinder(
            Vec3F const& _start,
            Vec3F const& _end,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireCylinder(_start, _end, _radius, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCone(
            Vec3F const& _position,
            Vec3F const& _direction,
            F32 _angle,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireCone(_position, _direction, _angle, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireTruncatedCone(
            Vec3F const& _position,
            Vec3F const& _direction,
            F32 _radius,
            F32 _angle,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireTruncatedCone(_position, _direction, _radius, _angle, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireTorus(
            Vec3F const& _position,
            Vec3F const& _direction,
            F32 _radius,
            F32 _torusRadius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireTorus(_position, _direction, _radius, _torusRadius, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireArrow(
            Vec3F const& _position,
            Vec3F const& _direction,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireArrow(_position, _direction, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawWireCapsule(
            Vec3F const& _start,
            Vec3F const& _end,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawWireCapsule(_start, _end, _radius, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawAABB(
            AABB2D const& _aabb,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawAABB(_aabb, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawAABB(
            AABB3D const& _aabb,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawAABB(_aabb, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawTriangle(
            Vec3F const& _point0,
            Vec3F const& _point1,
            Vec3F const& _point2,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawTriangle(_point0, _point1, _point2, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawQuad(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            Vec2F const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawQuad(
                _position, _forward, _up, _scale, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawCube(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            Vec3F const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawCube(
                _position, _forward, _up, _scale, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawSphere(
            Vec3F const& _position,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawSphere(_position, _radius, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawCone(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawCone(_position, _forward, _up, _radius, _height, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawCylinder(
            Vec3F const& _position,
            Vec3F const& _forward,
            Vec3F const& _up,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawCylinder(_position, _forward, _up, _radius, _height, _color, _duration, _renderMode);
        }

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API void DrawTorus(
            Vec3F const& _position,
            Vec3F const& _forward,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            GizmosDrawer::MeshRenderMode _renderMode)
        {
            GetDrawer()->drawTorus(_position, _forward, _radius, _color, _duration, _renderMode);
        }


    } // namespace GizmosHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
