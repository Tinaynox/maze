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
#include "maze-ui/scenes/SceneDragAndDropDefault.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneDragAndDropDefault
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDragAndDropDefault, SceneDragAndDrop);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(SceneDragAndDropDefault);

    //////////////////////////////////////////
    SceneDragAndDropDefault::SceneDragAndDropDefault()
    {
    }

    //////////////////////////////////////////
    SceneDragAndDropDefault::~SceneDragAndDropDefault()
    {

    }

    //////////////////////////////////////////
    SceneDragAndDropDefaultPtr SceneDragAndDropDefault::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneDragAndDropDefaultPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDragAndDropDefault, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneDragAndDropDefault::init(RenderTargetPtr const& _renderTarget)
    {
        if (!SceneDragAndDrop::init(_renderTarget))
            return false;

        return true;
    }

    //////////////////////////////////////////
    EcsWorld* SceneDragAndDropDefault::assignWorld()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorldRaw();
    }


} // namespace Maze
//////////////////////////////////////////
