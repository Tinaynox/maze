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
#include "MazeCoreHeader.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TransformEventsSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TransformEventsSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TransformEventsSystem);

    //////////////////////////////////////////
    TransformEventsSystem::TransformEventsSystem()
    {
    }

    //////////////////////////////////////////
    TransformEventsSystem::~TransformEventsSystem()
    {
    }

    //////////////////////////////////////////
    TransformEventsSystemPtr TransformEventsSystem::Create()
    {
        TransformEventsSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TransformEventsSystem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool TransformEventsSystem::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void TransformEventsSystem::processSystemAdded()
    {
        m_transform2Ds = m_worldRaw->requestInclusiveSample<Transform2D>();
        m_transform3Ds = m_worldRaw->requestInclusiveSample<Transform3D>();
    }
        
    //////////////////////////////////////////
    void TransformEventsSystem::processUpdate(UpdateEvent const* _event)
    {
        MAZE_PROFILE_EVENT("TransformEventsSystem::processUpdate");

        m_transform2Ds->process(
            [](Entity* entity, Transform2D* _transform2D)
            {
                _transform2D->processEndFrame();
            });

        m_transform3Ds->process(
            [](Entity* entity, Transform3D* _transform3D)
            {
                _transform3D->processEndFrame();
            });
    }
    
    
} // namespace Maze
//////////////////////////////////////////
