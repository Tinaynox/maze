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
#if (!defined(_SceneDragAndDropDefault_hpp_))
#define _SceneDragAndDropDefault_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/ecs/components/MazeInputSystem2D.hpp"
#include "maze-ui/scenes/SceneDragAndDrop.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneDragAndDropDefault);


    //////////////////////////////////////////
    // Class SceneDragAndDropDefault
    //
    //////////////////////////////////////////
    class MAZE_UI_API SceneDragAndDropDefault
        : public SceneDragAndDrop
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDragAndDropDefault, SceneDragAndDrop);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SceneDragAndDropDefault);

    public:

        //////////////////////////////////////////
        static SceneDragAndDropDefaultPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneDragAndDropDefault();


    protected:

        //////////////////////////////////////////
        SceneDragAndDropDefault();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual EcsWorld* assignWorld() MAZE_OVERRIDE;


    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneDragAndDropDefault_hpp_
//////////////////////////////////////////
