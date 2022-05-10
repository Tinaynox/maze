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
#if (!defined(_MazeMeshRendererGizmos_hpp_))
#define _MazeMeshRendererGizmos_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MeshRendererGizmos);


    //////////////////////////////////////////
    // Class MeshRendererGizmos
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API MeshRendererGizmos
        : public ComponentGizmos
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MeshRendererGizmos, ComponentGizmos);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MeshRendererGizmos);

    public:

        //////////////////////////////////////////
        virtual void drawGizmos(
            Entity* _entity,
            ComponentPtr const& _component,
            GizmosDrawer* _drawer) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void drawGizmosSelected(
            Entity* _entity,
            ComponentPtr const& _component,
            GizmosDrawer* _drawer) MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual ~MeshRendererGizmos();

        //////////////////////////////////////////
        static MeshRendererGizmosPtr Create();


    protected:

        //////////////////////////////////////////
        MeshRendererGizmos();

        //////////////////////////////////////////
        bool init();

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshRendererGizmos_hpp_
//////////////////////////////////////////
