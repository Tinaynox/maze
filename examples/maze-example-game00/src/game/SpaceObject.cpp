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
#include "SpaceObject.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "PhysicsBody.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-physics2d/events/MazePhysicsEvents.hpp"
#include "managers/GameManager.hpp"
#include "managers/ProjectileManager.hpp"
#include "game/Projectile.hpp"
#include "game/LevelAdapter.hpp"
#include "game/ProjectilePool.hpp"
#include "game/UnitBounds.hpp"
#include "game/PlayerOwner.hpp"
#include "game/Health.hpp"
#include "game/UnitMeshPartRenderer.hpp"
#include "game/TrailProjectile.hpp"
#include "game/UnitPartRenderer.hpp"
#include "game/SpaceObjectAvatar.hpp"
#include "game/ViewTriggers.hpp"
#include "game/SpecialEffectPool.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(SpaceObjectAvatarDirection);


    //////////////////////////////////////////
    // Class SpaceObject
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpaceObject, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, damage, 40.0f, getDamage, setDamage),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F32, engineForce, Vec2F32(80.0f, 60.0f), getEngineForce, setEngineForce),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F32, throttle, Vec2F32::c_zero, getThrottle, setThrottle),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F32, weaponPoint, Vec2F32::c_zero, getWeaponPoint, setWeaponPoint),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, fire, false, getFire, setFire),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, fireCooldown, 0.1f, getFireCooldown, setFireCooldown),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, collisionDamage, 10.0f, getCollisionDamage, setCollisionDamage),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, avatar, ComponentPtr(), getAvatarComponent, setAvatar),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ProjectileAvatarType, projectileAvatar, ProjectileAvatarType::RedFlare00, getProjectileAvatar, setProjectileAvatar),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpaceObjectAvatarDirection, direction, SpaceObjectAvatarDirection::Right, getDirection, setDirection)
    );

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpaceObject);

    //////////////////////////////////////////
    SpaceObject::SpaceObject()
        : m_damage(40.0f)
        , m_engineForce(Vec2F32(80.0f, 60.0f))
        , m_throttle(Vec2F32::c_zero)
        , m_weaponPoint(Vec2F32::c_zero)
        , m_fire(false)
        , m_fireTimer(0.0f)
        , m_fireCooldown(0.1f)
        , m_collisionDamage(10.0f)
        , m_collisionDamageProtectionTimer(0.0f)
        , m_projectileAvatar(ProjectileAvatarType::RedFlare00)
        , m_direction(SpaceObjectAvatarDirection::Right)
    {
    }

    //////////////////////////////////////////
    SpaceObject::~SpaceObject()
    {
        if (m_health)
        {
            m_health->eventDeath.unsubscribe(this);
            m_health->eventDamage.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SpaceObjectPtr SpaceObject::Create(LevelAdapterPtr const& _levelAdapter)
    {
        SpaceObjectPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpaceObject, object, init(_levelAdapter));
        return object;
    }

    //////////////////////////////////////////
    bool SpaceObject::init(LevelAdapterPtr const& _levelAdapter)
    {
        m_levelAdapter = _levelAdapter;

        return true;
    }

    //////////////////////////////////////////
    void SpaceObject::update(F32 _dt)
    {
        if (m_health->isAlive())
        {
            if (m_fireTimer > 0.0f)
            {
                m_fireTimer -= _dt;
                if (m_fireTimer < 0.0f)
                    m_fireTimer = 0.0f;
            }

            if (m_fireTimer <= 0.0f)
            {
                if (m_fire)
                {
                    m_fireTimer = m_fireCooldown;
                    createProjectile();
                }
            }

            if (m_collisionDamageProtectionTimer > 0.0f)
            {
                m_collisionDamageProtectionTimer -= _dt;
                if (m_collisionDamageProtectionTimer < 0.0f)
                    m_collisionDamageProtectionTimer = 0.0f;
            }
        }
        else
        {
            if (m_avatar->isDeathAnimationFinished())
            {
                release();
            }
        }
    }

    //////////////////////////////////////////
    void SpaceObject::processEvent(Event* _event)
    {
        ClassUID eventUID = _event->getClassUID();
        if (eventUID == ClassInfo<AllowContactEvent>::UID())
        {
            AllowContactEvent* evt = _event->castRaw<AllowContactEvent>();

            SpaceObject* contactSpaceObject = evt->contactRigidbody2D->getEntityRaw()->getComponentRaw<SpaceObject>();
            if (contactSpaceObject)
            {
                if (contactSpaceObject->getPlayerOwner()->getCurrentOwner() != getPlayerOwner()->getCurrentOwner())
                {
                    if (m_collisionDamageProtectionTimer <= 0.0f)
                    {
                        F32 collisionDamage = contactSpaceObject->getCollisionDamage();
                        if (collisionDamage > 0.0f)
                        {
                            m_health->doDamage(collisionDamage);

                            m_collisionDamageProtectionTimer = 0.25f;
                        }
                    }
                }
            }
        }
    }


    //////////////////////////////////////////
    void SpaceObject::setup()
    {
        if (!m_poolObject)
            m_poolObject = getEntityRaw()->getComponent<ComponentPoolObject<SpaceObject>>();

        m_avatar->setup(m_levelAdapter.get());
    }

    //////////////////////////////////////////
    void SpaceObject::prepare()
    {
        m_rigibody2D->setCollisionMask(0xFFFF);

        m_health->setHealth(m_health->getHealthMax());

        if (m_avatar)
            m_avatar->prepare();
    }

    //////////////////////////////////////////
    void SpaceObject::release()
    {
        m_poolObject->release(cast<SpaceObject>());
    }
    
    //////////////////////////////////////////
    void SpaceObject::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_playerOwner = getEntityRaw()->ensureComponent<PlayerOwner>();
        m_rigibody2D = getEntityRaw()->ensureComponent<Rigidbody2D>();
        m_rigibody2D->setFixedRotation(true);
        m_rigibody2D->setType(Rigidbody2DType::Dynamic);
        m_health = getEntityRaw()->ensureComponent<Health>();
        m_health->eventDeath.subscribe(this, &SpaceObject::notifyDeath);
        m_health->eventDamage.subscribe(this, &SpaceObject::notifyDamage);

        updateDirection();
    }

    //////////////////////////////////////////
    void SpaceObject::notifyDeath()
    {
        m_rigibody2D->setCollisionMask(0x0);
        m_transform->setLocalPosition(m_rigibody2D->getPosition());
        m_avatar->processDeath();
    }

    //////////////////////////////////////////
    void SpaceObject::notifyDamage(DamageData const& _damageData)
    {
        m_avatar->processDamage();
    }

    //////////////////////////////////////////
    void SpaceObject::createProjectile()
    {
        if (m_projectileAvatar == ProjectileAvatarType::None)
            return;

        Vec2F32 velocity = m_rigibody2D->getVelocity();

        ProjectilePtr projectile = m_levelAdapter->getProjectilePool()->createProjectile(m_projectileAvatar);

        switch (m_direction)
        {
            case SpaceObjectAvatarDirection::Right: projectile->setDirection(Vec2F32::c_unitX); break;
            case SpaceObjectAvatarDirection::Left: projectile->setDirection(Vec2F32::c_negativeUnitX); break;
            default:
                break;
        }
        projectile->getTransform()->setLocalPosition(m_transform->getLocalPosition() + getWeaponPointDirected() + velocity * (1.0f / 20.0f));
        
        projectile->getPlayerOwner()->setPlayer(getPlayerOwner()->getCurrentOwner());
        projectile->setDamage(m_damage);
        
        projectile->setExtraSpeed(
            {
                velocity.x,
                velocity.y * 0.35f,
            });
        
        m_avatar->invokeViewTrigger(ViewTriggers::g_fire);

        eventProjectileSpawned(projectile);
    }

    //////////////////////////////////////////
    void SpaceObject::setDirection(SpaceObjectAvatarDirection direction)
    {
        if (m_direction == direction)
            return;

        m_direction = direction;

        updateDirection();
    }

    //////////////////////////////////////////
    void SpaceObject::updateDirection()
    {
        switch (m_direction)
        {
            case SpaceObjectAvatarDirection::Right:
            {
                if (m_avatar)
                    m_avatar->getTransform()->setLocalRotation(Quaternion(0.0f, Math::c_halfPi, 0.0f));

                break;
            }
            case SpaceObjectAvatarDirection::Left:
            {
                if (m_avatar)
                    m_avatar->getTransform()->setLocalRotation(Quaternion(0.0f, -Math::c_halfPi, 0.0f));

                break;
            }
        default:
            break;
        }
    }


} // namespace Maze
//////////////////////////////////////////
