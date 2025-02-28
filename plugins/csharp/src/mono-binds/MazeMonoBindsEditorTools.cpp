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
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsEditorTools()
    {
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC_WITH_NAME(GizmosHelper::PushTransform, PushTransform);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC_WITH_NAME(GizmosHelper::PopTransform, PopTransform);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(SetColor);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawLine);
        MAZE_EDITOR_TOOLS_MONO_BIND_FUNC(DrawWireSphere);
    }

} // namespace Maze
//////////////////////////////////////////
