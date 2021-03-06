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
#include "maze-editor-tools/gizmo-tools/MazeGizmoToolConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(GizmoToolType);


    //////////////////////////////////////////
    F32 const GizmoToolConfig::c_cameraScalePerDistance = 0.08f;
    F32 const GizmoToolConfig::c_transformGizmoToolLength = 2.0f;
    F32 const GizmoToolConfig::c_transformGizmoToolArrowLineRadius = 0.015f;
    F32 const GizmoToolConfig::c_transformGizmoToolArrowConeRadius = 0.135f;
    F32 const GizmoToolConfig::c_transformGizmoToolArrowConeHeight = 0.475f;
    F32 const GizmoToolConfig::c_transformGizmoToolArrowCubeSize = GizmoToolConfig::c_transformGizmoToolArrowConeRadius * 2.0f;
    F32 const GizmoToolConfig::c_transformGizmoToolArrowCenterCubeSize = GizmoToolConfig::c_transformGizmoToolArrowCubeSize * 1.33f;
    F32 const GizmoToolConfig::c_transformGizmoToolRotationRadius = 2.5f;
    ColorF128 const GizmoToolConfig::c_transformGizmoToolAxisXColor = ColorF128(1.0f, 0, 0, 1.0f);
    ColorF128 const GizmoToolConfig::c_transformGizmoToolAxisYColor = ColorF128(0, 1.0f, 0, 1.0f);
    ColorF128 const GizmoToolConfig::c_transformGizmoToolAxisZColor = ColorF128(0, 0, 1.0f, 1.0f);
    ColorF128 const GizmoToolConfig::c_transformGizmoToolAxisSelectedColor = ColorF128(1.0f, 1.0f, 0, 1.0f);

} // namespace Maze
//////////////////////////////////////////
