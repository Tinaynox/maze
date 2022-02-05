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
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-physics2d/managers/MazePhysicsMaterial2DManager.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/physics/MazePhysicsMaterial2D.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(Rigidbody2DType);


    //////////////////////////////////////////
    MAZE_PHYSICS2D_API b2BodyType GetBox2DBodyType(Rigidbody2DType _type)
    {
        switch (_type)
        {
            case Rigidbody2DType::Static: return b2_staticBody; break;
            case Rigidbody2DType::Dynamic: return b2_dynamicBody; break;
            case Rigidbody2DType::Kinematic: return b2_kinematicBody; break;
            default: MAZE_NOT_IMPLEMENTED;
        }

        return b2_staticBody;
    }


    //////////////////////////////////////////
    // Class Rigidbody2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Rigidbody2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Rigidbody2DType, type, Rigidbody2DType::Dynamic, getType, setType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, linearDamping, 0.0f, getLinearDamping, setLinearDamping),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, angularDamping, 0.0f, getAngularDamping, setAngularDamping),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, gravityScale, 1.0f, getGravityScale, setGravityScale),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U16, collisionMask, 0xFFFF, getCollisionMask, setCollisionMask),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U16, categoryMask, 1, getCategoryMask, setCategoryMask),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, flags, 0, getFlags, setFlags));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Rigidbody2D);


    //////////////////////////////////////////
    Rigidbody2D::Rigidbody2D()
        : m_world(nullptr)
        , m_enabled(true)
        , m_type(Rigidbody2DType::Static)
        , m_linearDamping(0.0f)
        , m_angularDamping(0.0f)
        , m_gravityScale(1.0f)
        , m_collisionMask(0xFFFF)
        , m_categoryMask(1)
        , m_flags(0)
        , m_body(nullptr)
        , m_fixedUpdateStartPosition(Vec2DF::c_zero)
        , m_fixedUpdateStartAngle(0.0f)
    {
        
    }

    //////////////////////////////////////////
    Rigidbody2D::~Rigidbody2D()
    {
        setWorld(nullptr);
    }

    //////////////////////////////////////////
    Rigidbody2DPtr Rigidbody2D::Create(PhysicsWorld2D* _world)
    {
        Rigidbody2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Rigidbody2D, object, init(_world));
        return object;
    }

    //////////////////////////////////////////
    bool Rigidbody2D::init(PhysicsWorld2D* _world)
    {
        if (!_world)
            _world = Physics2DManager::GetInstancePtr()->getWorld().get();

        setWorld(_world);

        return true;
    }

    //////////////////////////////////////////
    bool Rigidbody2D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        if (!Component::init(_component, _world, _copyData))
            return false;

        setWorld(_component->castRaw<Rigidbody2D>()->m_world);

        return true;
    }

    //////////////////////////////////////////
    void Rigidbody2D::processEntityAwakened()
    {
        m_transform2D = getEntityRaw()->getComponent<Transform2D>();
        m_transform3D = getEntityRaw()->getComponent<Transform3D>();

        dirtyBody();
    }

    //////////////////////////////////////////
    void Rigidbody2D::processComponentRemoved()
    {
        destroyBody();
    }

    //////////////////////////////////////////
    void Rigidbody2D::destroyBody()
    {
        if (!m_world)
            return;

        b2World* world = m_world->getBox2DWorld();

        if (m_body)
        {
            world->DestroyBody(m_body);
            m_body = nullptr;
        }
    }

    //////////////////////////////////////////
    AABB2D Rigidbody2D::getAABB()
    {
        if (!m_body)
            return AABB2D::c_zero;

        return Box2DHelper::GetBodyAABB2D(m_world, m_body);
    }

    //////////////////////////////////////////
    void Rigidbody2D::processEntityEnabled()
    {
        updateBodyEnabled();
    }

    //////////////////////////////////////////
    void Rigidbody2D::processEntityDisabled()
    {
        updateBodyEnabled();
    }

    //////////////////////////////////////////
    void Rigidbody2D::updateBodyEnabled()
    {
        if (!m_body)
            return; 

        if (m_world->getBox2DWorld()->IsLocked())
            m_flags |= Rigidbody2DFlag::EnabledDirty;
        else
            m_body->SetEnabled(getEntityRaw()->getActiveInHierarchy() && m_enabled);
    }

    //////////////////////////////////////////
    void Rigidbody2D::fixedUpdateStart(F32 _dt)
    {
        if (getBodyDirty())
            rebuildBody();

        if (getEnabledDirty())
        {
            m_body->SetEnabled(getEntityRaw()->getActiveInHierarchy() && m_enabled);
            m_flags &= ~Rigidbody2DFlag::EnabledDirty;
        }

        if (getTransformDirty())
        {
            m_body->SetTransform(Box2DHelper::ToVec2(m_world->convertUnitsToMeters(m_fixedUpdateStartPosition)), m_body->GetAngle());
            m_flags &= ~Rigidbody2DFlag::TransformDirty;
        }

        m_fixedUpdateStartPosition = getPosition();
        m_fixedUpdateStartAngle = getAngle();
    }

    //////////////////////////////////////////
    void Rigidbody2D::setWorld(PhysicsWorld2D* _world)
    {
        if (m_world == _world)
            return;

        if (m_world)
        {
            m_world->eventDestroyed.unsubscribe(this);
            m_world->eventClear.unsubscribe(this);
        }

        m_world = _world;

        if (m_world)
        {
            m_world->eventDestroyed.subscribe(this, &Rigidbody2D::notifyWorldDestroyed);
            m_world->eventClear.subscribe(this, &Rigidbody2D::notifyWorldClear);
        }
    }

    //////////////////////////////////////////
    void Rigidbody2D::notifyWorldDestroyed()
    {
        m_body = nullptr;
    }

    //////////////////////////////////////////
    void Rigidbody2D::notifyWorldClear()
    {
        m_world = nullptr;
    }

    //////////////////////////////////////////
    void Rigidbody2D::rebuildBody()
    {
        b2World* world = m_world->getBox2DWorld();

        b2Vec2 linearVelocity = b2Vec2_zero;
        F32 angularVelocity = 0.0f;

        if (m_body)
        {
            linearVelocity = m_body->GetLinearVelocity();
            angularVelocity = m_body->GetAngularVelocity();
            world->DestroyBody(m_body);
            m_body = nullptr;
        }

        b2BodyDef bd;
        bd.type = GetBox2DBodyType(m_type);

        Vec2DF position = Vec2DF::c_zero;
        F32 angle = 0.0f;
        Vec2DF scale = Vec2DF::c_one;

        if (m_transform2D)
        {
            position = m_transform2D->getWorldPosition();
            angle = m_transform2D->getWorldTransform().transformAffine(Vec2DF::c_unitX).toAngle();
            scale = m_transform2D->getWorldTransform().getAffineScale2D();
        }
        else
        if (m_transform3D)
        {
            Vec3DF worldPosition = m_transform3D->getWorldPosition();
            position = { worldPosition.x, worldPosition.y };
            // angle = m_transform3D->getWorldTransform().getAffineRotationEulerAngles().z;
            angle = Quaternion::GetEuler(m_transform3D->getWorldTransform()).z;
            scale = m_transform3D->getWorldTransform().getAffineScale2D();
        }
        else
        {
            MAZE_NOT_IMPLEMENTED;
        }
        
        bd.position = Box2DHelper::ToVec2(m_world->convertUnitsToMeters(position));
        bd.angle = angle;
        bd.bullet = getIsBullet();
        bd.fixedRotation = getFixedRotation();

        m_body = world->CreateBody(&bd);
        m_body->GetUserData().pointer = (uintptr_t)this;
        m_body->SetAngularDamping(getAngularDamping());
        m_body->SetLinearDamping(getLinearDamping());
        m_body->SetGravityScale(getGravityScale());
        m_body->SetSleepingAllowed(true);
        m_body->SetLinearVelocity(linearVelocity);
        m_body->SetAngularVelocity(angularVelocity);

        Vector<BoxCollider2D*> boxColliders;
        Vector<CircleCollider2D*> circleColliders;
        collectColliders(
            boxColliders,
            circleColliders);

        for (BoxCollider2D* boxCollider : boxColliders)
        {
            b2Filter filter;
            filter.categoryBits = getCategoryMask();
            filter.maskBits = getCollisionMask();

            b2FixtureDef fixture;
            b2PolygonShape polygon;

            PhysicsMaterial2D* physicsMaterial = boxCollider->getPhysicsMaterial().get();
            if (!physicsMaterial)
                physicsMaterial = PhysicsMaterial2DManager::GetInstancePtr()->getDefaultMaterial().get();

            fixture.density = physicsMaterial->getDensity();
            fixture.friction = physicsMaterial->getFriction();
            fixture.restitution = physicsMaterial->getRestitution();
            fixture.filter = filter;
            fixture.isSensor = boxCollider->getIsSensor();

            if (fixture.isSensor)
                fixture.density = 0.0001f;

            polygon.SetAsBox(
                m_world->convertUnitsToMeters(boxCollider->getSize().x * scale.x * 0.5f),
                m_world->convertUnitsToMeters(boxCollider->getSize().y * scale.y * 0.5f),
                Box2DHelper::ToVec2(m_world->convertUnitsToMeters(
                    boxCollider->getOffset().x * scale.x, 
                    boxCollider->getOffset().y * scale.y)),
               0.0f);
            fixture.shape = &polygon;
            fixture.userData.pointer = (uintptr_t)(Collider2D*)boxCollider;

            m_body->CreateFixture(&fixture);
        }

        for (CircleCollider2D* circleCollider : circleColliders)
        {
            b2Filter filter;
            filter.categoryBits = getCategoryMask();
            filter.maskBits = getCollisionMask();

            b2FixtureDef fixture;
            b2CircleShape polygon;

            PhysicsMaterial2D* physicsMaterial = circleCollider->getPhysicsMaterial().get();
            if (!physicsMaterial)
                physicsMaterial = PhysicsMaterial2DManager::GetInstancePtr()->getDefaultMaterial().get();

            fixture.density = physicsMaterial->getDensity();
            fixture.friction = physicsMaterial->getFriction();
            fixture.restitution = physicsMaterial->getRestitution();
            fixture.filter = filter;
            fixture.isSensor = circleCollider->getIsSensor();

            if (fixture.isSensor)
                fixture.density = 0.0001f;

            F32 scaleMax = Math::Max(scale.x, scale.y);

            polygon.m_p = Box2DHelper::ToVec2(
                m_world->convertUnitsToMeters(
                    circleCollider->getOffset().x * scaleMax,
                    circleCollider->getOffset().y * scaleMax));
            polygon.m_radius = m_world->convertUnitsToMeters(circleCollider->getRadius() * scaleMax);
            fixture.shape = &polygon;
            fixture.userData.pointer = (uintptr_t)(Collider2D*)circleCollider;

            m_body->CreateFixture(&fixture);
        }

        updateBodyEnabled();

        m_flags &= ~Rigidbody2DFlag::BodyDirty;
    }

    //////////////////////////////////////////
    void Rigidbody2D::collectColliders(
        Vector<BoxCollider2D*>& _boxColliders,
        Vector<CircleCollider2D*>& _circleColliders)
    {
        if (m_transform2D)
        {
            _boxColliders = m_transform2D->getAllComponentsRaw<BoxCollider2D>(true);
            _circleColliders = m_transform2D->getAllComponentsRaw<CircleCollider2D>(true);
        }
        else
        if (m_transform3D)
        {
            _boxColliders = m_transform3D->getAllComponentsRaw<BoxCollider2D>(true);
            _circleColliders = m_transform3D->getAllComponentsRaw<CircleCollider2D>(true);
        }
    }
    
    
} // namespace Maze
//////////////////////////////////////////
