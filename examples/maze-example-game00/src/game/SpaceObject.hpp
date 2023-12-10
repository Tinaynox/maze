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
#pragma once
#if (!defined(_SpaceObject_hpp_))
#define _SpaceObject_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "game/SpaceObject.hpp"
#include "game/DamageData.hpp"
#include "game/ProjectileAvatarType.hpp"
#include "game/SpaceObjectAvatar.hpp"
#include "game/SpecialEffectType.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SpaceObject);
    MAZE_USING_SHARED_PTR(LevelAdapter);
    MAZE_USING_SHARED_PTR(PlayerOwner);
    MAZE_USING_SHARED_PTR(Health);
    MAZE_USING_SHARED_PTR(UnitMeshPartRenderer);
    MAZE_USING_SHARED_PTR(Material);
    MAZE_USING_SHARED_PTR(ShaderUniformVariant);
    MAZE_USING_SHARED_PTR(Projectile);
    MAZE_USING_SHARED_PTR(UnitPartRenderer);
    MAZE_USING_SHARED_PTR(SpaceObjectAvatar);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2(SpaceObjectAvatarDirection,
        Right,
        Left);


    //////////////////////////////////////////
    // Class SpaceObject
    //
    //////////////////////////////////////////
    class SpaceObject
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SpaceObject, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SpaceObject);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SpaceObject();

        //////////////////////////////////////////
        static SpaceObjectPtr Create(LevelAdapterPtr const& _levelAdapter = nullptr);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        virtual void processEvent(Event* _event) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline void setLevelAdapter(LevelAdapterPtr const& _levelAdapter) { m_levelAdapter = _levelAdapter; }


        //////////////////////////////////////////
        inline F32 getDamage() const { return m_damage; }

        //////////////////////////////////////////
        inline void setDamage(F32 _damage) { m_damage = _damage; }


        //////////////////////////////////////////
        inline Vec2F32 const& getEngineForce() const { return m_engineForce; }

        //////////////////////////////////////////
        inline void setEngineForce(Vec2F32 const& _engineForce) { m_engineForce = _engineForce; }

        //////////////////////////////////////////
        inline Vec2F32 const& getThrottle() const { return m_throttle; }

        //////////////////////////////////////////
        void setThrottle(Vec2F32 const& _throttle) { m_throttle = _throttle; }


        //////////////////////////////////////////
        inline Vec2F32 const& getWeaponPoint() const { return m_weaponPoint; }

        //////////////////////////////////////////
        void setWeaponPoint(Vec2F32 const& _weaponPoint) { m_weaponPoint = _weaponPoint; }

        //////////////////////////////////////////
        inline Vec2F32 getWeaponPointDirected()
        {
            switch (m_direction)
            {
                case SpaceObjectAvatarDirection::Right:
                {
                    return m_weaponPoint;
                }

                case SpaceObjectAvatarDirection::Left:
                {
                    return Vec2F32(-m_weaponPoint.x, m_weaponPoint.y);
                }

                default:
                    return Vec2F32::c_zero;
            }
        }


        //////////////////////////////////////////
        inline void setFire(bool _fire) { m_fire = _fire; }

        //////////////////////////////////////////
        inline bool getFire() const { return m_fire; }


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        inline PlayerOwnerPtr const& getPlayerOwner() const { return m_playerOwner; }

        //////////////////////////////////////////
        inline Rigidbody2DPtr const& getRigidbody2D() const { return m_rigibody2D; }

        //////////////////////////////////////////
        inline HealthPtr const& getHealth() const { return m_health; }


        //////////////////////////////////////////
        inline F32 getFireTimer() const { return m_fireTimer; }


        //////////////////////////////////////////
        inline void setFireCooldown(F32 _fireCooldown) { m_fireCooldown = _fireCooldown; }

        //////////////////////////////////////////
        inline F32 getFireCooldown() const { return m_fireCooldown; }



        //////////////////////////////////////////
        inline F32 getCollisionDamage() const { return m_collisionDamage; }

        //////////////////////////////////////////
        inline void setCollisionDamage(F32 _collisionDamage) { m_collisionDamage = _collisionDamage; }


        //////////////////////////////////////////
        inline void setProjectileAvatar(ProjectileAvatarType _projectileAvatar) { m_projectileAvatar = _projectileAvatar; }

        //////////////////////////////////////////
        inline ProjectileAvatarType getProjectileAvatar() const { return m_projectileAvatar; }



        //////////////////////////////////////////
        inline void setAvatar(SpaceObjectAvatarPtr const& _avatarTransform) 
        {
            if (m_avatar == _avatarTransform)
                return;

            m_avatar = _avatarTransform;

            updateDirection();
        }

        //////////////////////////////////////////
        inline SpaceObjectAvatarPtr const& getAvatar() const { return m_avatar; }

        //////////////////////////////////////////
        inline void setAvatar(ComponentPtr _avatar)
        {
            if (_avatar)
            {
                MAZE_DEBUG_BP_RETURN_IF(_avatar->getClassUID() != ClassInfo<SpaceObjectAvatar>::UID());
                setAvatar(_avatar->cast<SpaceObjectAvatar>());
            }
            else
            {
                setAvatar(SpaceObjectAvatarPtr());
            }
        }

        //////////////////////////////////////////
        inline ComponentPtr getAvatarComponent() const { return m_avatar; }



        //////////////////////////////////////////
        void setDirection(SpaceObjectAvatarDirection direction);

        //////////////////////////////////////////
        inline SpaceObjectAvatarDirection getDirection() const { return m_direction; }


        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        void prepare();

        //////////////////////////////////////////
        void release();

    protected:

        //////////////////////////////////////////
        SpaceObject();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(LevelAdapterPtr const& _levelAdapter);

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void createProjectile();

        //////////////////////////////////////////
        void notifyDeath();

        //////////////////////////////////////////
        void notifyDamage(DamageData const& _damageData);


        //////////////////////////////////////////
        void updateDirection();

    public:

        //////////////////////////////////////////
        MultiDelegate<ProjectilePtr const&> eventProjectileSpawned;

    protected:
        SharedPtr<ComponentPoolObject<SpaceObject>> m_poolObject;

        Transform3DPtr m_transform;
        PlayerOwnerPtr m_playerOwner;
        Rigidbody2DPtr m_rigibody2D;
        HealthPtr m_health;

        LevelAdapterPtr m_levelAdapter;

        SpaceObjectAvatarPtr m_avatar;

        F32 m_damage;

        Vec2F32 m_engineForce;
        Vec2F32 m_throttle;

        Vec2F32 m_weaponPoint;

        bool m_fire;
        F32 m_fireTimer;
        F32 m_fireCooldown;

        F32 m_collisionDamage;        
        F32 m_collisionDamageProtectionTimer;

        ProjectileAvatarType m_projectileAvatar;

        SpaceObjectAvatarDirection m_direction;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SpaceObject_hpp_
//////////////////////////////////////////
