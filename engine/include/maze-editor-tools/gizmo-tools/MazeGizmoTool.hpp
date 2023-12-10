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
#if (!defined(_MazeGizmoTool_hpp_))
#define _MazeGizmoTool_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeVec2.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmoTool);
    MAZE_USING_SHARED_PTR(Entity);


    //////////////////////////////////////////
    // Class GizmoTool
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmoTool
    {
    public:

        //////////////////////////////////////////
        virtual ~GizmoTool() = default;

        //////////////////////////////////////////
        virtual void manipulate(Set<EntityPtr> const& _entities, Vec2F const& _cursorPos) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processCursorPress(Vec2F const& _cursorPos) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processCursorRelease() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isUsing() MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        GizmoTool() = default;

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmoTool_hpp_
//////////////////////////////////////////
