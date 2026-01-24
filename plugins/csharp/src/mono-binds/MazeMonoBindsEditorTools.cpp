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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsEditorTools.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline void SetColor(
        Vec4F const& _color)
    {
        GizmosHelper::SetColor(ColorF128(_color));
    }

    //////////////////////////////////////////
    inline void DrawLine(
        Vec3F const& _point0,
        Vec3F const& _point1,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawLine(
            _point0,
            _point1,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawRay(
        Vec3F const& _point,
        Vec3F const& _delta,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawRay(
            _point,
            _delta,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireQuad(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec2F const& _scale,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireQuad(
            _position,
            _forward,
            _up,
            _scale,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCircle(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCircle(
            _position,
            _direction,
            _radius,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireHemicircle(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireHemicircle(
            _position,
            _forward,
            _up,
            _radius,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCube(
        Vec3F const& _position,
        Vec3F const& _scale,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCube(
            _position,
            _scale,
            _forward,
            _up,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireSphere(
        Vec3F const& _position,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireSphere(
            _position,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireHemisphere(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireHemisphere(
            _position,
            _direction,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCylinder(
        Vec3F const& _start,
        Vec3F const& _end,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCylinder(
            _start,
            _end,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCone(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _angle,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCone(
            _position,
            _direction,
            _angle,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireTruncatedCone(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _angle,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireTruncatedCone(
            _position,
            _direction,
            _radius,
            _angle,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireTorus(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _torusRadius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireTorus(
            _position,
            _direction,
            _radius,
            _torusRadius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireArrow(
        Vec3F const& _position,
        Vec3F const& _direction,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireArrow(
            _position,
            _direction,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireCapsule(
        Vec3F const& _start,
        Vec3F const& _end,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireCapsule(
            _start,
            _end,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireAABB2D(
        AABB2D const& _aabb,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireAABB(
            _aabb,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawWireAABB3D(
        AABB3D const& _aabb,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawWireAABB(
            _aabb,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidAABB3D(
        AABB3D const& _aabb,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidAABB(
            _aabb,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidTriangle(
        Vec3F const& _point0,
        Vec3F const& _point1,
        Vec3F const& _point2,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidTriangle(
            _point0,
            _point1,
            _point2,
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidQuad(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec2F const& _scale,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidQuad(
            _position,
            _forward,
            _up,
            _scale,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidCube(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec3F const& _scale,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidCube(
            _position,
            _forward,
            _up,
            _scale,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidSphere(
        Vec3F const& _position,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidSphere(
            _position,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidCone(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _height,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidCone(
            _position,
            _forward,
            _up,
            _radius,
            _height,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidCylinder(
        Vec3F const& _position,
        Vec3F const& _forward,
        F32 _radius,
        F32 _height,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidCylinder(
            _position,
            _forward,
            _radius,
            _height,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    inline void DrawSolidTorus(
        Vec3F const& _position,
        Vec3F const& _forward,
        F32 _radius,
        Vec4F const& _color,
        F32 _duration = 0.0f,
        S32 _gizmosMode = 0,
        S32 _renderMode = 0)
    {
        GizmosHelper::DrawSolidTorus(
            _position,
            _forward,
            _radius,
            ColorF128(_color),
            _duration,
            GizmosDrawer::GizmosMode(_gizmosMode),
            GizmosDrawer::MeshRenderMode(_renderMode));
    }

    //////////////////////////////////////////
    bool MAZE_PLUGIN_CSHARP_API IsEntitySelected(S32 _entityId)
    {
        return SelectionManager::GetInstancePtr()->isObjectSelected(EntityId(_entityId));
    }


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsEditorTools()
    {
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC_WITH_NAME(GizmosHelper::PushTransform, PushTransform);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC_WITH_NAME(GizmosHelper::PopTransform, PopTransform);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(SetColor);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawLine);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawRay);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireQuad);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCircle);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireHemicircle);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCube);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireSphere);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireHemisphere);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCylinder);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCone);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireTruncatedCone);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireTorus);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireArrow);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireCapsule);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireAABB2D);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireAABB3D);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidAABB3D);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidTriangle);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidQuad);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidCube);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidSphere);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidCone);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidCylinder);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawSolidTorus);

        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(IsEntitySelected);
    }

} // namespace Maze
//////////////////////////////////////////
