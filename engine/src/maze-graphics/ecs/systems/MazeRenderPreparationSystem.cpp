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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/systems/MazeRenderPreparationSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderPreparationSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderPreparationSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(RenderPreparationSystem);

    //////////////////////////////////////////
    RenderPreparationSystem::RenderPreparationSystem()
    {
    }

    //////////////////////////////////////////
    RenderPreparationSystem::~RenderPreparationSystem()
    {
        
    }

    //////////////////////////////////////////
    RenderPreparationSystemPtr RenderPreparationSystem::Create(RenderSystemPtr const& _renderSystem)
    {
        RenderPreparationSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderPreparationSystem, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool RenderPreparationSystem::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    void RenderPreparationSystem::processSystemAdded()
    {
        m_canvasesSample = m_worldRaw->requestInclusiveSample<Canvas>();
        m_cameras3DSample = m_worldRaw->requestInclusiveSample<Camera3D>();
    }

    //////////////////////////////////////////
    void RenderPreparationSystem::processUpdate(UpdateEvent const* _event)
    {
        MAZE_PROFILE_EVENT("RenderPreparationSystem::processUpdate");

        F32 dt = _event->getDt();

        m_canvasesSample->process(
            [dt](Entity* _entity, Canvas* _canvas)
            {
                _canvas->prepareFrame(dt);
            });
    }

    
} // namespace Maze
//////////////////////////////////////////
