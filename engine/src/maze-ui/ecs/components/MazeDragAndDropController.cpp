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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/scenes/SceneDragAndDrop.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DragAndDropController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DragAndDropController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(DragAndDropController);

    //////////////////////////////////////////
    DragAndDropController::DragAndDropController()
    {
    }

    //////////////////////////////////////////
    DragAndDropController::~DragAndDropController()
    {

    }

    //////////////////////////////////////////
    DragAndDropControllerPtr DragAndDropController::Create(EcsSceneId _dragAndDropSceneId)
    {
        DragAndDropControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(DragAndDropController, object, init(_dragAndDropSceneId));
        return object;
    }

    //////////////////////////////////////////
    bool DragAndDropController::init(EcsSceneId _dragAndDropSceneId)
    {
        m_dragAndDropSceneId = _dragAndDropSceneId;

        return true;
    }

    //////////////////////////////////////////
    SceneDragAndDrop* DragAndDropController::getDragAndDropScene()
    {
        SceneDragAndDropPtr scene = SceneManager::GetInstancePtr()->getSceneInheritedFrom<SceneDragAndDrop>();
        if (scene)
            return scene.get();

        return nullptr;
    }


} // namespace Maze
//////////////////////////////////////////
