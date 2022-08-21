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
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/events/MazePhysicsEvents.hpp"
#include "maze-core/managers/MazeEventManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PhysicsWorld2D
    //    
    //////////////////////////////////////////
    PhysicsWorld2D::PhysicsWorld2D()
        : m_world(nullptr)
        , m_accumulator(0.0f)
        , m_accumulatorRatio(0.0f)
        , m_unitsPerMeter(200)
        , m_worldMaxSteps(10)
        , m_worldStep(1.0f / 50.0f)
        , m_velocityIterations(6)
        , m_positionIterations(8)
        , m_size(Vec2DF::c_zero)
    {
        
    }

    //////////////////////////////////////////
    PhysicsWorld2D::~PhysicsWorld2D()
    {
        destroyWorld();

        eventDestroyed();
    }

    //////////////////////////////////////////
    PhysicsWorld2DPtr PhysicsWorld2D::Create(PhysicsWorld2DConfig const& _config)
    {
        PhysicsWorld2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PhysicsWorld2D, object, init(_config));
        return object;
    }

    //////////////////////////////////////////
    bool PhysicsWorld2D::init(PhysicsWorld2DConfig const& _config)
    {
        m_unitsPerMeter = _config.unitsPerMeter;
        m_worldMaxSteps = _config.worldMaxSteps;
        m_worldStep = _config.worldStep;
        m_velocityIterations = _config.velocityIterations;
        m_positionIterations = _config.positionIterations;

        createWorld();

        setGravity(_config.gravityUnits);

        return true;
    }

    ////////////////////////////////////
    void PhysicsWorld2D::setGravity(Vec2DF const& _gravityUnits)
    {
        if (!m_world)
            return;

        m_world->SetGravity(Box2DHelper::ToVec2(convertUnitsToMeters(_gravityUnits)));
    }

    //////////////////////////////////////////
    void PhysicsWorld2D::update(F32 _dt)
    {
        if (!m_active)
            return;

        MAZE_MUTEX_SCOPED_LOCK(m_worldMutex);

        if (m_world)
        {
            m_accumulator += _dt;

            S32 steps = S32(Math::Floor(m_accumulator / m_worldStep));

            if (steps > 0)
                m_accumulator -= steps * m_worldStep;

            MAZE_DEBUG_BP_IF(m_accumulator >= m_worldStep + FLT_EPSILON);

            m_accumulatorRatio = m_accumulator / m_worldStep;

            S32 stepsClamped = Math::Min(steps, m_worldMaxSteps);

            U32 time = UpdateManager::GetInstancePtr()->getMilliseconds();
            if (stepsClamped)
            {
                for (S32 i = 0; i < stepsClamped; ++i)
                {
                    singleStep(m_worldStep);

                    if ((UpdateManager::GetInstancePtr()->getMilliseconds() - time) >= 5)
                        break;
                }
            }

            eventUpdateFinished(_dt);
        }
    }

    //////////////////////////////////////////
    void PhysicsWorld2D::singleStep(F32 _dt)
    {
        if (!m_world)
            return;

        resetSmoothStates();
        eventFixedUpdateStart(_dt);
        m_world->Step(_dt, m_velocityIterations, m_positionIterations);
        eventFixedUpdateFinished(_dt);
    }

    //////////////////////////////////////////
    void PhysicsWorld2D::resetSmoothStates()
    {
        if (!m_world)
            return;
    }

    //////////////////////////////////////////
    void PhysicsWorld2D::createWorld()
    {
        destroyWorld();

        Debug::log << "Creating Box2D world..." << endl;

        MAZE_MUTEX_SCOPED_LOCK(m_worldMutex);
        m_world = MAZE_NEW_WITH_ARGS(b2World, b2Vec2(0.0f, 0.0f));
        m_world->SetContactListener(this);
        m_world->SetAutoClearForces(true);

        Debug::log << "Box2D world created." << endl;
    }

    //////////////////////////////////////////
    void PhysicsWorld2D::destroyWorld()
    {
        if (!m_world)
            return;

        Debug::log << "Destroying Box2D world..." << endl;

        MAZE_MUTEX_SCOPED_LOCK(m_worldMutex);
        clear();
        MAZE_SAFE_DELETE(m_world);

        Debug::log << "Box2D world destroyed." << endl;
    }

    ////////////////////////////////////
    void PhysicsWorld2D::clear()
    {
        m_size = Vec2DF::c_zero;

        destroyDefaultBodies();

        while (b2Body* b = m_world->GetBodyList())
        {
            m_world->DestroyBody(b);
        }

        eventClear();
    }

    ////////////////////////////////////
    void PhysicsWorld2D::destroyDefaultBodies()
    {
        if (!m_world)
            return;

        if (m_defaultBodies.innerWalls)
        {
            m_world->DestroyBody(m_defaultBodies.innerWalls);
            m_defaultBodies.innerWalls = nullptr;
        }

        if (m_defaultBodies.ground)
        {
            m_world->DestroyBody(m_defaultBodies.ground);
            m_defaultBodies.ground = nullptr;
        }
    }

    ////////////////////////////////////
    void PhysicsWorld2D::createDefaultBodies()
    {
        if (!m_world)
            return;

        destroyDefaultBodies();

        if (m_defaultBodies.ground == nullptr)
        {
            b2BodyDef bd;
            m_defaultBodies.ground = m_world->CreateBody(&bd);
        }

        b2BodyDef bd;
        m_defaultBodies.innerWalls = m_world->CreateBody(&bd);

        b2EdgeShape shape;
        Vec2DF sizeMeters = m_size / m_unitsPerMeter;

        shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(sizeMeters.x, 0.0f));
        b2Fixture* topWall = m_defaultBodies.innerWalls->CreateFixture(&shape, 0.0f);
        topWall->SetFriction(1.0f);

        shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, sizeMeters.y));
        b2Fixture* leftWall = m_defaultBodies.innerWalls->CreateFixture(&shape, 0.0f);
        leftWall->SetFriction(1.0f);

        shape.SetTwoSided(b2Vec2(sizeMeters.x, 0.0f), b2Vec2(sizeMeters.x, sizeMeters.y));
        b2Fixture* rightWall = m_defaultBodies.innerWalls->CreateFixture(&shape, 0.0f);
        rightWall->SetFriction(1.0f);

        shape.SetTwoSided(b2Vec2(0.0f, sizeMeters.y), b2Vec2(sizeMeters.x, sizeMeters.y));
        b2Fixture* bottomWall = m_defaultBodies.innerWalls->CreateFixture(&shape, 0.0f);
        bottomWall->SetFriction(1.0f);
    }

    ////////////////////////////////////
    void PhysicsWorld2D::BeginContact(b2Contact* _contact)
    {
        b2Fixture* fixtureA = _contact->GetFixtureA();
        b2Fixture* fixtureB = _contact->GetFixtureB();

        Collider2D* colliderA = static_cast<Collider2D*>((void*)fixtureA->GetUserData().pointer);
        Collider2D* colliderB = static_cast<Collider2D*>((void*)fixtureB->GetUserData().pointer);

        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        Rigidbody2D* rigidbodyA = reinterpret_cast<Rigidbody2D*>(bodyA->GetUserData().pointer);
        Rigidbody2D* rigidbodyB = reinterpret_cast<Rigidbody2D*>(bodyB->GetUserData().pointer);

        if (rigidbodyA && rigidbodyA->getEntityRaw())
        {
            SharedPtr<AllowContactEvent> evt =
                EventManager::GetInstancePtr()->generateAndSendEventImmediate<AllowContactEvent>(
                    rigidbodyA->getEntityRaw(),
                    rigidbodyA,
                    rigidbodyB,
                    colliderA,
                    colliderB);

            if (evt->ignore)
            {
                _contact->SetEnabled(false);
                return;
            }
        }

        if (rigidbodyB && rigidbodyB->getEntityRaw())
        {
            SharedPtr<AllowContactEvent> evt =
                EventManager::GetInstancePtr()->generateAndSendEventImmediate<AllowContactEvent>(
                    rigidbodyB->getEntityRaw(),
                    rigidbodyB,
                    rigidbodyA,
                    colliderB,
                    colliderA);

            if (evt->ignore)
            {
                _contact->SetEnabled(false);
                return;
            }
        }

        if (rigidbodyA && rigidbodyA->getEntityRaw())
        {
            EventManager::GetInstancePtr()->generateAndSendEventImmediate<BeginContactEvent>(
                rigidbodyA->getEntityRaw(),
                rigidbodyA,
                rigidbodyB,
                colliderA,
                colliderB);
        }

        if (rigidbodyB && rigidbodyB->getEntityRaw())
        {
            EventManager::GetInstancePtr()->generateAndSendEventImmediate<BeginContactEvent>(
                rigidbodyB->getEntityRaw(),
                rigidbodyB,
                rigidbodyA,
                colliderB,
                colliderA);
        }
    }

    ////////////////////////////////////
    void PhysicsWorld2D::PreSolve(b2Contact* _contact, const b2Manifold* _oldManifold)
    {
        b2Fixture* fixtureA = _contact->GetFixtureA();
        b2Fixture* fixtureB = _contact->GetFixtureB();

        Collider2D* colliderA = static_cast<Collider2D*>((void*)fixtureA->GetUserData().pointer);
        Collider2D* colliderB = static_cast<Collider2D*>((void*)fixtureB->GetUserData().pointer);

        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        Rigidbody2D* rigidbodyA = reinterpret_cast<Rigidbody2D*>(bodyA->GetUserData().pointer);
        Rigidbody2D* rigidbodyB = reinterpret_cast<Rigidbody2D*>(bodyB->GetUserData().pointer);

        if (rigidbodyA && rigidbodyA->getEntityRaw())
        {
            SharedPtr<AllowContactEvent> evt =
                EventManager::GetInstancePtr()->generateAndSendEventImmediate<AllowContactEvent>(
                    rigidbodyA->getEntityRaw(),
                    rigidbodyA,
                    rigidbodyB,
                    colliderA,
                    colliderB);

            if (evt->ignore)
            {
                _contact->SetEnabled(false);
                return;
            }
        }

        if (rigidbodyB && rigidbodyB->getEntityRaw())
        {
            SharedPtr<AllowContactEvent> evt =
                EventManager::GetInstancePtr()->generateAndSendEventImmediate<AllowContactEvent>(
                    rigidbodyB->getEntityRaw(),
                    rigidbodyB,
                    rigidbodyA,
                    colliderB,
                    colliderA);

            if (evt->ignore)
            {
                _contact->SetEnabled(false);
                return;
            }
        }

        if (rigidbodyA && rigidbodyA->getEntityRaw())
        {
            EventManager::GetInstancePtr()->generateAndSendEventImmediate<PreSolveContactEvent>(
                rigidbodyA->getEntityRaw(),
                rigidbodyA,
                rigidbodyB,
                colliderA,
                colliderB);
        }

        if (rigidbodyB && rigidbodyB->getEntityRaw())
        {
            EventManager::GetInstancePtr()->generateAndSendEventImmediate<PreSolveContactEvent>(
                rigidbodyB->getEntityRaw(),
                rigidbodyB,
                rigidbodyA,
                colliderB,
                colliderA);
        }
    }

    ////////////////////////////////////
    void PhysicsWorld2D::PostSolve(b2Contact* _contact, const b2ContactImpulse* _impulse)
    {
        b2Fixture* fixtureA = _contact->GetFixtureA();
        b2Fixture* fixtureB = _contact->GetFixtureB();

        Collider2D* colliderA = static_cast<Collider2D*>((void*)fixtureA->GetUserData().pointer);
        Collider2D* colliderB = static_cast<Collider2D*>((void*)fixtureB->GetUserData().pointer);

        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        Rigidbody2D* rigidbodyA = reinterpret_cast<Rigidbody2D*>(bodyA->GetUserData().pointer);
        Rigidbody2D* rigidbodyB = reinterpret_cast<Rigidbody2D*>(bodyB->GetUserData().pointer);

        if (rigidbodyA || rigidbodyB)
        {
            F32 impulseValue = 0.0f;
            for (Size i = 0, in = _impulse->count; i < in; ++i)
                impulseValue += fabs(_impulse->normalImpulses[i]);

            if (rigidbodyA && rigidbodyA->getEntityRaw())
            {
                EventManager::GetInstancePtr()->generateAndSendEventImmediate<PostSolveContactEvent>(
                    rigidbodyA->getEntityRaw(),
                    rigidbodyA,
                    rigidbodyB,
                    colliderA,
                    colliderB,
                    impulseValue);
            }

            if (rigidbodyB && rigidbodyB->getEntityRaw())
            {
                EventManager::GetInstancePtr()->generateAndSendEventImmediate<PostSolveContactEvent>(
                    rigidbodyB->getEntityRaw(),
                    rigidbodyB,
                    rigidbodyA,
                    colliderB,
                    colliderA,
                    impulseValue);
            }
        }
    }

    ////////////////////////////////////
    void PhysicsWorld2D::EndContact(b2Contact* _contact)
    {
        b2Fixture* fixtureA = _contact->GetFixtureA();
        b2Fixture* fixtureB = _contact->GetFixtureB();

        Collider2D* colliderA = static_cast<Collider2D*>((void*)fixtureA->GetUserData().pointer);
        Collider2D* colliderB = static_cast<Collider2D*>((void*)fixtureB->GetUserData().pointer);

        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        Rigidbody2D* rigidbodyA = reinterpret_cast<Rigidbody2D*>(bodyA->GetUserData().pointer);
        Rigidbody2D* rigidbodyB = reinterpret_cast<Rigidbody2D*>(bodyB->GetUserData().pointer);

        if (rigidbodyA && rigidbodyA->getEntityRaw())
        {
            EventManager::GetInstancePtr()->generateAndSendEventImmediate<EndContactEvent>(
                rigidbodyA->getEntityRaw(),
                rigidbodyA,
                rigidbodyB,
                colliderA,
                colliderB);
        }

        if (rigidbodyB && rigidbodyB->getEntityRaw())
        {
            EventManager::GetInstancePtr()->generateAndSendEventImmediate<EndContactEvent>(
                rigidbodyB->getEntityRaw(),
                rigidbodyB,
                rigidbodyA,
                colliderB,
                colliderA);
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
