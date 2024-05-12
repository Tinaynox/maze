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
#include "MazePhysics2DHeader.hpp"
#include "maze-physics2d/ecs/components/MazePhysicsController2D.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/ecs/components/MazePhysicsRotor2D.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/ecs/events/MazeEcsPhysics2DEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsController2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PhysicsController2D, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PhysicsController2D);

    //////////////////////////////////////////
    PhysicsController2D::PhysicsController2D()
    {
    }

    //////////////////////////////////////////
    PhysicsController2D::~PhysicsController2D()
    {
        setPhysicsWorld2D(nullptr);
    }

    //////////////////////////////////////////
    PhysicsController2DPtr PhysicsController2D::Create()
    {
        PhysicsController2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsController2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool PhysicsController2D::init()
    {
        setPhysicsWorld2D(Physics2DManager::GetInstancePtr()->getWorld());

        return true;
    }

    //////////////////////////////////////////
    void PhysicsController2D::processEntityAwakened()
    {
    }

    //////////////////////////////////////////
    void PhysicsController2D::setPhysicsWorld2D(PhysicsWorld2DPtr const& _physicsWorld2D)
    {
        if (m_physicsWorld2D == _physicsWorld2D)
            return;

        if (m_physicsWorld2D)
        {
            m_physicsWorld2D->eventFixedUpdateStart.unsubscribe(this);
            m_physicsWorld2D->eventFixedUpdateFinished.unsubscribe(this);
            m_physicsWorld2D->eventUpdateFinished.unsubscribe(this);
        }

        m_physicsWorld2D = _physicsWorld2D;

        if (m_physicsWorld2D)
        {
            m_physicsWorld2D->eventFixedUpdateStart.subscribe(this, &PhysicsController2D::notifyFixedUpdateStart);
            m_physicsWorld2D->eventFixedUpdateFinished.subscribe(this, &PhysicsController2D::notifyFixedUpdateFinished);
            m_physicsWorld2D->eventUpdateFinished.subscribe(this, &PhysicsController2D::notifyUpdateFinished);
        }
    }

    //////////////////////////////////////////
    void PhysicsController2D::notifyFixedUpdateStart(F32 _dt)
    {
        getEntityRaw()->getEcsWorld()->broadcastEventImmediate<Physics2DFixedUpdateStartEvent>(_dt);
    }

    //////////////////////////////////////////
    void PhysicsController2D::notifyFixedUpdateFinished(F32 _dt)
    {
        getEntityRaw()->getEcsWorld()->broadcastEventImmediate<Physics2DFixedUpdateFinishedEvent>(_dt);
    }

    //////////////////////////////////////////
    void PhysicsController2D::notifyUpdateFinished(F32 _dt)
    {
        F32 accumulatorRatio = m_physicsWorld2D->getAccumulatorRatio();
        getEntityRaw()->getEcsWorld()->broadcastEventImmediate<Physics2DUpdateFinishedEvent>(_dt, accumulatorRatio);
    }
    
} // namespace Maze
//////////////////////////////////////////
