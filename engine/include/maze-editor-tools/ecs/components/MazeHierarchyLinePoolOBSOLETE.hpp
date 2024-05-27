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
#if (!defined(_MazeHierarchyLinePoolOBSOLETE_hpp_))
#define _MazeHierarchyLinePoolOBSOLETE_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/utils/MazeSharedObjectPool.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLineOBSOLETE.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(HierarchyLinePoolOBSOLETE);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(HierarchyLineOBSOLETE);


    //////////////////////////////////////////
    // Class HierarchyLinePoolOBSOLETE
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API HierarchyLinePoolOBSOLETE
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(HierarchyLinePoolOBSOLETE, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(HierarchyLinePoolOBSOLETE);

    public:

        //////////////////////////////////////////
        virtual ~HierarchyLinePoolOBSOLETE();

        //////////////////////////////////////////
        static HierarchyLinePoolOBSOLETEPtr Create();


        //////////////////////////////////////////
        HierarchyLineOBSOLETEPtr createHierarchyLineOBSOLETE(HierarchyLineOBSOLETEType _type);

        //////////////////////////////////////////
        void releaseHierarchyLineOBSOLETE(HierarchyLineOBSOLETEPtr const& _hierarchyLine);

    protected:

        //////////////////////////////////////////
        HierarchyLinePoolOBSOLETE();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

    protected:
        SharedPtr<SharedObjectPool<HierarchyLineOBSOLETE>> m_pools[(Size)HierarchyLineOBSOLETEType::MAX];
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHierarchyLinePoolOBSOLETE_hpp_
//////////////////////////////////////////
