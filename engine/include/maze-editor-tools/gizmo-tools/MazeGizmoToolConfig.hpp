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
#if (!defined(_MazeGizmoToolConfig_hpp_))
#define _MazeGizmoToolConfig_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-graphics/MazeColorF128.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GizmoToolConfig
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmoToolConfig
    {
    public:

        //////////////////////////////////////////
        static F32 const c_cameraScalePerDistance;
        static F32 const c_transformGizmoToolLength;
        static F32 const c_transformGizmoToolArrowLineRadius;
        static F32 const c_transformGizmoToolArrowConeRadius;
        static F32 const c_transformGizmoToolArrowConeHeight;
        static ColorF128 const c_transformGizmoToolAxisXColor;
        static ColorF128 const c_transformGizmoToolAxisYColor;
        static ColorF128 const c_transformGizmoToolAxisZColor;
        static ColorF128 const c_transformGizmoToolAxisSelectedColor;
        
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmoToolConfig_hpp_
//////////////////////////////////////////
