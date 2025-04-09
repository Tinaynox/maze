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
#include "MazeEngineHeader.hpp"
#include "maze-engine/ecs/components/MazePlayerCanvas.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-engine/MazeEngine.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PlayerCanvas
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PlayerCanvas, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PlayerCanvas);

    //////////////////////////////////////////
    PlayerCanvas::PlayerCanvas()
    {
    }

    //////////////////////////////////////////
    PlayerCanvas::~PlayerCanvas()
    {

    }

    //////////////////////////////////////////
    PlayerCanvasPtr PlayerCanvas::Create()
    {
        PlayerCanvasPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PlayerCanvas, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool PlayerCanvas::init()
    {
        return true;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(PlayerCanvasOnCreate,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        PlayerCanvas* _playerCanvas,
        Canvas* _canvas)
    {
        EcsRenderScene* scene = _entity->getEcsScene()->castRaw<EcsRenderScene>();
        RenderTargetPtr const& sceneRenderTarget = scene->getRenderTarget();

        if (_canvas)
            _canvas->setRenderTarget(sceneRenderTarget);
    }



} // namespace Maze
//////////////////////////////////////////
