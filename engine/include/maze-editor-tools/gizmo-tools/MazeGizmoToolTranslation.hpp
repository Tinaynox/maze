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
#if (!defined(_MazeGizmoToolTranslation_hpp_))
#define _MazeGizmoToolTranslation_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-editor-tools/gizmo-tools/MazeGizmoTool.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmoToolTranslation);


    //////////////////////////////////////////
    // Class GizmoToolTranslation
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmoToolTranslation
        : public GizmoTool
    {
    public:

        //////////////////////////////////////////
        virtual ~GizmoToolTranslation() = default;

        //////////////////////////////////////////
        static GizmoToolTranslationPtr Create();

        //////////////////////////////////////////
        virtual void manipulate(Mat4DF& _mat, Vec2DF const& _cursorPos) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processCursorPress(Vec2DF const& _cursorPos) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processCursorRelease() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isUsing() MAZE_OVERRIDE { return m_usingAxis >= 0; }

    protected:

        //////////////////////////////////////////
        GizmoToolTranslation() = default;

        //////////////////////////////////////////
        Vec3DF getWorldAxis(S32 _axis);

        //////////////////////////////////////////
        Vec3DF getWorldAxisPerpendicular(Vec3DF const& _axis, Vec3DF const& _dir);

    protected:
        S32 m_selectedAxis = -1;
        S32 m_usingAxis = -1;

        bool m_useRequest = false;
        Vec3DF m_startPosition = Vec3DF::c_zero;
        Vec3DF m_startPoint = Vec3DF::c_zero;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmoToolTranslation_hpp_
//////////////////////////////////////////
