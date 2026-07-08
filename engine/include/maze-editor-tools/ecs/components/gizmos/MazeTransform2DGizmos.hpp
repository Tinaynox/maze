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
#if (!defined(_MazeTransform2DGizmos_hpp_))
#define _MazeTransform2DGizmos_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Transform2DGizmos);


    //////////////////////////////////////////
    // Class Transform2DGizmos
    //
    // Draws a Unity RectTransform-style overlay for the selected 2D entity:
    // the rect outline, its pivot and, if parented, a marker for its anchor
    // point plus a connecting line from the anchor to the pivot.
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API Transform2DGizmos
        : public ComponentGizmos
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Transform2DGizmos, ComponentGizmos);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Transform2DGizmos);

    public:

        //////////////////////////////////////////
        virtual void drawGizmosSelected2D(
            Entity* _entity,
            ComponentPtr const& _component,
            GizmosDrawer2D* _drawer) MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual ~Transform2DGizmos();

        //////////////////////////////////////////
        static Transform2DGizmosPtr Create();


    protected:

        //////////////////////////////////////////
        Transform2DGizmos();

        //////////////////////////////////////////
        bool init();

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTransform2DGizmos_hpp_
//////////////////////////////////////////
