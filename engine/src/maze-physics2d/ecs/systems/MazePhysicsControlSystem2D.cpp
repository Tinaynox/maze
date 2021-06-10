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
#include "maze-physics2d/ecs/systems/MazePhysicsControlSystem2D.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsControlSystem2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PhysicsControlSystem2D, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PhysicsControlSystem2D);

    //////////////////////////////////////////
    PhysicsControlSystem2D::PhysicsControlSystem2D()
    {
    }

    //////////////////////////////////////////
    PhysicsControlSystem2D::~PhysicsControlSystem2D()
    {
        setPhysicsWorld2D(nullptr);
    }

    //////////////////////////////////////////
    PhysicsControlSystem2DPtr PhysicsControlSystem2D::Create()
    {
        PhysicsControlSystem2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsControlSystem2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool PhysicsControlSystem2D::init()
    {
        setPhysicsWorld2D(Physics2DManager::GetInstancePtr()->getWorld());

        return true;
    }

    //////////////////////////////////////////
    void PhysicsControlSystem2D::processSystemAdded()
    {
        m_rigidbodies = m_worldRaw->requestInclusiveSample<Rigidbody2D, Transform3D>();
    }

    //////////////////////////////////////////
    void PhysicsControlSystem2D::processUpdate(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void PhysicsControlSystem2D::setPhysicsWorld2D(PhysicsWorld2DPtr const& _physicsWorld2D)
    {
        if (m_physicsWorld2D == _physicsWorld2D)
            return;

        if (m_physicsWorld2D)
        {
            m_physicsWorld2D->eventFixedUpdateStart.unsubscribe(this);
            m_physicsWorld2D->eventUpdateFinished.unsubscribe(this);
        }

        m_physicsWorld2D = _physicsWorld2D;

        if (m_physicsWorld2D)
        {
            m_physicsWorld2D->eventFixedUpdateStart.subscribe(this, &PhysicsControlSystem2D::notifyFixedUpdateStart);
            m_physicsWorld2D->eventUpdateFinished.subscribe(this, &PhysicsControlSystem2D::notifyUpdateFinished);
        }
    }

    //////////////////////////////////////////
    void PhysicsControlSystem2D::notifyFixedUpdateStart(F32 _dt)
    {
        m_rigidbodies->process(
            [&](Entity* _entity, Rigidbody2D* _rigidbody, Transform3D* _transform)
            {
                _rigidbody->fixedUpdateStart(_dt);
            });
    }

    //////////////////////////////////////////
    void PhysicsControlSystem2D::notifyUpdateFinished(F32 _dt)
    {
        F32 accumulatorRatio = m_physicsWorld2D->getAccumulatorRatio();

        m_rigidbodies->process(
            [&](Entity* _entity, Rigidbody2D* _rigidbody, Transform3D* _transform)
            {
                if (!_rigidbody->getBody())
                    return;

                _transform->setLocalPosition(
                    Math::Lerp(
                        _rigidbody->getFixedUpdateStartPosition(),
                        _rigidbody->getPosition(),
                        accumulatorRatio));

                _transform->setLocalRotation(
                    Quaternion(
                        Math::Lerp(
                            _rigidbody->getFixedUpdateStartAngle(),
                            _rigidbody->getAngle(),
                            accumulatorRatio),
                        Vec3DF::c_unitZ));
            });
    }
    
} // namespace Maze
//////////////////////////////////////////
