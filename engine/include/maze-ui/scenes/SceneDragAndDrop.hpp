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
#if (!defined(_SceneDragAndDrop_hpp_))
#define _SceneDragAndDrop_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/ecs/components/MazeInputSystem2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneDragAndDrop);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class SceneDragAndDrop
    //
    //////////////////////////////////////////
    class MAZE_UI_API SceneDragAndDrop
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDragAndDrop, EcsRenderScene);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SceneDragAndDrop);

    public:

        //////////////////////////////////////////
        static SceneDragAndDropPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneDragAndDrop();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void startDrag(
            Transform2DPtr const& _viewEntity,
            DataBlock const& _data = DataBlock());

        //////////////////////////////////////////
        void cancelDrag();

        //////////////////////////////////////////
        void tryDrop();

    protected:

        //////////////////////////////////////////
        SceneDragAndDrop();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);


        //////////////////////////////////////////
        void setCurrentDropZone(EntityId _eid);

    protected:
        CanvasPtr m_canvas;
        UIElement2DPtr m_element;

        EntityId m_viewEntityId;
        Transform2DPtr m_dragAndDropView;

        DataBlock m_data;

        EntityId m_currentDropZoneEid;

        SharedPtr<GenericInclusiveEntitiesSample<DragAndDropZone>> m_dragAndDropZonesSample;
        SharedPtr<GenericInclusiveEntitiesSample<InputSystem2D>> m_inputSystemSample;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneDragAndDrop_hpp_
//////////////////////////////////////////
