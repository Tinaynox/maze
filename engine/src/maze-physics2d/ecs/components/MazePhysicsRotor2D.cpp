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
#include "maze-physics2d/ecs/components/MazePhysicsRotor2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/ecs/events/MazeECSPhysics2DEvents.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
// PhysicsRotor2Dspace Maze
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsRotor2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PhysicsRotor2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, speed, 5.0f, getSpeed, setSpeed));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(PhysicsRotor2D);

    //////////////////////////////////////////
    PhysicsRotor2D::PhysicsRotor2D()
        : m_speed(5.0f)
    {
    }

    //////////////////////////////////////////
    PhysicsRotor2D::~PhysicsRotor2D()
    {
    }

    //////////////////////////////////////////
    PhysicsRotor2DPtr PhysicsRotor2D::Create(F32 _speed)
    {
        PhysicsRotor2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsRotor2D, object, init(_speed));
        return object;
    }

    //////////////////////////////////////////
    bool PhysicsRotor2D::init(F32 _speed)
    {
        setSpeed(_speed);

        return true;
    }

    //////////////////////////////////////////
    void PhysicsRotor2D::update(F32 _dt)
    {
        m_rigidbody->setAngle(m_rigidbody->getAngle() + m_speed * _dt);
    }

    //////////////////////////////////////////
    void PhysicsRotor2D::processEntityAwakened()
    {
        m_rigidbody = getEntityRaw()->getComponent<Rigidbody2D>();
    }


    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM_EVENT_HANDLER(PhysicsRotor2DOnFixedUpdateStart, {},
        Physics2DFixedUpdateStartEvent const& _event,
        Entity* _entity,
        PhysicsRotor2D* _rotor)
    {
        _rotor->update(_event.getDt());
    }
    
} // namespace Maze
//////////////////////////////////////////
