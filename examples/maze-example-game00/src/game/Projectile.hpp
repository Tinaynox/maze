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
#if (!defined(_Projectile_hpp_))
#define _Projectile_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "game/SpecialEffectType.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Projectile);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(PlayerOwner);
    MAZE_USING_SHARED_PTR(TrailRenderer3D);
    MAZE_USING_SHARED_PTR(LevelAdapter);


    //////////////////////////////////////////
    // Class Projectile
    //
    //////////////////////////////////////////
    class Projectile
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Projectile, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Projectile);

        //////////////////////////////////////////
        friend class Entity;


        //////////////////////////////////////////
        inline Transform3DPtr const& getTransform() const { return m_transform; }
        
        //////////////////////////////////////////
        inline PlayerOwnerPtr const& getPlayerOwner() const { return m_playerOwner; }

        //////////////////////////////////////////
        inline TrailRenderer3DPtr const& getTrailRenderer3D() const { return m_trailRenderer; }


        //////////////////////////////////////////
        inline F32 getSpeed() const { return m_speed; }

        //////////////////////////////////////////
        inline void setSpeed(F32 _speed) { m_speed = _speed; }


        //////////////////////////////////////////
        inline F32 getDamage() const { return m_damage; }

        //////////////////////////////////////////
        inline void setDamage(F32 _damage) { m_damage = _damage; }


        //////////////////////////////////////////
        inline Vec2F32 const& getExtraSpeed() const { return m_extraSpeed; }

        //////////////////////////////////////////
        inline void setExtraSpeed(Vec2F32 const& _extraSpeed) { m_extraSpeed = _extraSpeed; }


        //////////////////////////////////////////
        inline Vec2F32 const& getDirection() const { return m_direction; }

        //////////////////////////////////////////
        inline void setDirection(Vec2F32 const& _direction) { m_direction = _direction; }


        //////////////////////////////////////////
        inline SpecialEffectType getDestroyEffect() const { return m_destroyEffect; }

        //////////////////////////////////////////
        inline void setDestroyEffect(SpecialEffectType _direction) { m_destroyEffect = _direction; }


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void setup(LevelAdapter* _levelAdapater);

        //////////////////////////////////////////
        void prepare();

    public:

        //////////////////////////////////////////
        virtual ~Projectile();

        //////////////////////////////////////////
        static ProjectilePtr Create();


        //////////////////////////////////////////
        void release();

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventPrepare;

    protected:

        //////////////////////////////////////////
        Projectile();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

    protected:
        SharedPtr<ComponentPoolObject<Projectile>> m_poolObject;
        Transform3DPtr m_transform;     
        PlayerOwnerPtr m_playerOwner;
        TrailRenderer3DPtr m_trailRenderer;

        F32 m_speed;
        F32 m_damage;

        Vec2F32 m_extraSpeed;

        Vec2F32 m_direction;

        SpecialEffectType m_destroyEffect = SpecialEffectType::None;

        LevelAdapter* m_levelAdapter = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _Projectile_hpp_
//////////////////////////////////////////
