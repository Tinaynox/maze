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
#include "Projectile.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-physics2d/helpers/MazePhysics2DHelper.hpp"
#include "game/UnitBounds.hpp"
#include "game/PlayerOwner.hpp"
#include "game/SpaceObject.hpp"
#include "game/Health.hpp"
#include "game/LevelAdapter.hpp"
#include "game/SpecialEffectPool.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-physics2d/helpers/MazePhysics2DHelper.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Projectile
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Projectile, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, speed, 15.0f, getSpeed, setSpeed),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, damage, 50.0f, getDamage, setDamage),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F32, extraSpeed, Vec2F32::c_zero, getExtraSpeed, setExtraSpeed),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F32, direction, Vec2F32::c_unitX, getDirection, setDirection),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpecialEffectType, destroyEffect, SpecialEffectType::None, getDestroyEffect, setDestroyEffect));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Projectile);

    //////////////////////////////////////////
    Projectile::Projectile()
        : m_speed(15.0f)
        , m_damage(50.0f)
        , m_direction(Vec2F32::c_unitX)
    {
    }

    //////////////////////////////////////////
    Projectile::~Projectile()
    {

    }

    //////////////////////////////////////////
    ProjectilePtr Projectile::Create()
    {
        ProjectilePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Projectile, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Projectile::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Projectile::update(F32 _dt)
    {
        Vec2F32 position = { m_transform->getLocalPosition().x, m_transform->getLocalPosition().y };
        Vec2F32 translation = (m_direction * m_speed + m_extraSpeed) * _dt;
        
        m_extraSpeed.x -= m_extraSpeed.x * Math::Min(32 * _dt, 1.0f);
        m_extraSpeed.y -= m_extraSpeed.y * Math::Min(32 * _dt, 1.0f);


        SpaceObjectPtr spaceObject;

        Physics2DHelper::OverlapHit2DPtr overlap =
            Physics2DHelper::OverlapPoint(
                position);

        if (overlap)
        {
            spaceObject = overlap->rigidbody->getEntityRaw()->getComponent<SpaceObject>();
        }
        else
        {
            Physics2DHelper::RaycastHit2DPtr raycast =
                Physics2DHelper::Raycast(
                    position,
                    position + translation);

            if (raycast)
                spaceObject = raycast->rigidbody->getEntityRaw()->getComponent<SpaceObject>();
        }

        if (spaceObject)
        {
            if (getPlayerOwner()->getCurrentOwner() != spaceObject->getPlayerOwner()->getCurrentOwner())
            {
                if (spaceObject->getHealth()->doDamage(getDamage()))
                {
                    if (m_destroyEffect != SpecialEffectType::None)
                    {
                        SpecialEffectPtr effect = m_levelAdapter->getSpecialEffectPool()->createSpecialEffect(m_destroyEffect);
                        effect->getTransform()->setLocalPosition(getTransform()->getLocalPosition());
                    }

                    release();
                    return;
                }
            }
        }
        
        m_transform->translate(translation);
        
    }

    //////////////////////////////////////////
    void Projectile::release()
    {
        m_poolObject->release(cast<Projectile>());
    }

    //////////////////////////////////////////
    void Projectile::setup(LevelAdapter* _levelAdapater)
    {
        m_levelAdapter = _levelAdapater;

        if (!m_poolObject)
            m_poolObject = getEntityRaw()->getComponent<ComponentPoolObject<Projectile>>();
    }

    //////////////////////////////////////////
    void Projectile::prepare()
    {
        eventPrepare();
    }

    //////////////////////////////////////////
    void Projectile::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_playerOwner = getEntityRaw()->ensureComponent<PlayerOwner>();
        m_trailRenderer = getEntityRaw()->getComponent<TrailRenderer3D>();
    }


} // namespace Maze
//////////////////////////////////////////
