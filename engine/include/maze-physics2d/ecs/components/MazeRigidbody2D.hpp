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
#if (!defined(_MazeRigidbody2D_hpp_))
#define _MazeRigidbody2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include <box2d/box2d.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(BoxCollider2D);
    MAZE_USING_SHARED_PTR(CircleCollider2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(PhysicsMaterial2D);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(
        MAZE_PHYSICS2D_API,
        Rigidbody2DType,
        Static,
        Dynamic,
        Kinematic);


    //////////////////////////////////////////
    MAZE_PHYSICS2D_API b2BodyType GetBox2DBodyType(Rigidbody2DType _type);


    //////////////////////////////////////////
    // Class Rigidbody2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API Rigidbody2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Rigidbody2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Rigidbody2D);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        enum Rigidbody2DFlag
        {
            IsBullet = MAZE_BIT(0),
            FixedRotation = MAZE_BIT(1),
            TransformDirty = MAZE_BIT(5),
            EnabledDirty = MAZE_BIT(6),
            BodyDirty = MAZE_BIT(7),
        };

    public:

        //////////////////////////////////////////
        virtual ~Rigidbody2D();

        //////////////////////////////////////////
        static Rigidbody2DPtr Create(PhysicsWorld2D* _world = nullptr);


        //////////////////////////////////////////
        void fixedUpdateStart(F32 _dt);



        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }

        //////////////////////////////////////////
        inline void setEnabled(bool _enabled)
        {
            if (m_enabled == _enabled)
                return;

            m_enabled = _enabled;

            updateBodyEnabled();
        }

        //////////////////////////////////////////
        inline Rigidbody2DType const& getType() const { return m_type; }

        //////////////////////////////////////////
        inline void setType(Rigidbody2DType const& _type)
        {
            if (m_type == _type)
                return;

            m_type = _type;

            if (m_body)
            {
                m_body->SetType(GetBox2DBodyType(m_type));
                m_body->SetAwake(true);
            }
        }


        //////////////////////////////////////////
        F32 getLinearDamping() const { return m_linearDamping; }

        //////////////////////////////////////////
        void setLinearDamping(F32 _linearDamping)
        {
            m_linearDamping = _linearDamping;

            if (m_body)
                m_body->SetLinearDamping(getLinearDamping());
        }


        //////////////////////////////////////////
        F32 getAngularDamping() const { return m_angularDamping; }

        //////////////////////////////////////////
        void setAngularDamping(F32 _angularDamping)
        {
            m_angularDamping = _angularDamping;

            if (m_body)
                m_body->SetAngularDamping(getAngularDamping());
        }


        //////////////////////////////////////////
        F32 getGravityScale() const { return m_gravityScale; }

        //////////////////////////////////////////
        void setGravityScale(F32 _gravityScale)
        {
            if (m_gravityScale == _gravityScale)
                return;

            m_gravityScale = _gravityScale;

            if (m_body)
                m_body->SetGravityScale(getGravityScale());
        }


        //////////////////////////////////////////
        U16 getCollisionMask() const { return m_collisionMask; }

        //////////////////////////////////////////
        void setCollisionMask(U16 _collisionMask)
        {
            if (m_collisionMask == _collisionMask)
                return;

            m_collisionMask = _collisionMask;

            dirtyBody();
        }


        //////////////////////////////////////////
        U16 getCategoryMask() const { return m_categoryMask; }

        //////////////////////////////////////////
        void setCategoryMask(U16 _categoryMask)
        {
            if (m_categoryMask == _categoryMask)
                return;

            m_categoryMask = _categoryMask;

            dirtyBody();
        }


        //////////////////////////////////////////
        void setIsBullet(bool _isBullet)
        {
            if (_isBullet)
                setFlags(m_flags | Rigidbody2DFlag::IsBullet);
            else
                setFlags(m_flags & ~Rigidbody2DFlag::IsBullet);
        }

        //////////////////////////////////////////
        bool getIsBullet() const { return (m_flags & Rigidbody2DFlag::IsBullet) != 0; }


        //////////////////////////////////////////
        void setFixedRotation(bool _isFixedRotation)
        {
            if (_isFixedRotation)
                setFlags(m_flags | Rigidbody2DFlag::FixedRotation);
            else
                setFlags(m_flags & ~Rigidbody2DFlag::FixedRotation);
        }

        //////////////////////////////////////////
        bool getFixedRotation() const { return (m_flags & Rigidbody2DFlag::FixedRotation) != 0; }

        //////////////////////////////////////////
        bool getBodyDirty() const { return (m_flags & Rigidbody2DFlag::BodyDirty) != 0; }

        //////////////////////////////////////////
        bool getEnabledDirty() const { return (m_flags & Rigidbody2DFlag::EnabledDirty) != 0; }

        //////////////////////////////////////////
        bool getTransformDirty() const { return (m_flags & Rigidbody2DFlag::TransformDirty) != 0; }


        //////////////////////////////////////////
        U8 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        void setFlags(U8 _flags)
        {
            if (m_flags == _flags)
                return;

            m_flags = _flags;

            dirtyBody();
        }


        //////////////////////////////////////////
        inline b2Body* getBody() const { return m_body; }


        //////////////////////////////////////////
        inline Vec2F getPosition() const
        {
            if (!m_body)
                return Vec2F::c_zero;

            return m_world->convertMetersToUnits(Box2DHelper::ToVec2F32(m_body->GetPosition()));
        }

        //////////////////////////////////////////
        inline void setPosition(Vec2F const& _position, bool _resetStartPosition = true)
        {
            if (!m_body)
                return;

            if (_resetStartPosition)
                m_fixedUpdateStartPosition = _position;

            if (m_world->getBox2DWorld()->IsLocked())
                m_flags |= Rigidbody2DFlag::TransformDirty;
            else
                m_body->SetTransform(Box2DHelper::ToVec2(m_world->convertUnitsToMeters(_position)), m_body->GetAngle());
        }

        //////////////////////////////////////////
        inline void setPositionX(F32 _x, bool _resetStartPosition = true)
        { setPosition({ _x, getPosition().y }, _resetStartPosition); }

        //////////////////////////////////////////
        inline void setPositionY(F32 _y, bool _resetStartPosition = true)
        { setPosition({ getPosition().x, _y}, _resetStartPosition); }

        //////////////////////////////////////////
        inline F32 getAngle() const
        {
            if (!m_body)
                return 0.0f;

            return m_body->GetAngle();
        }

        //////////////////////////////////////////
        inline void setAngle(F32 _angle)
        {
            if (!m_body)
                return;

            m_fixedUpdateStartAngle = _angle;
            m_body->SetTransform(m_body->GetPosition(), _angle);
        }
         

        //////////////////////////////////////////
        Vec2F const& getFixedUpdateStartPosition() const { return m_fixedUpdateStartPosition; }

        //////////////////////////////////////////
        F32 getFixedUpdateStartAngle() const { return m_fixedUpdateStartAngle; }


        //////////////////////////////////////////
        void rebuildBody();

        //////////////////////////////////////////
        inline void dirtyBody()
        {
            m_flags |= Rigidbody2DFlag::BodyDirty;
        }


        //////////////////////////////////////////
        void applyImpulse(Vec2F const& _impulse, bool _multiplyMass = true)
        {
            if (!m_body)
            {
                rebuildBody();

                if (!m_body)
                    return;
            }

            b2Vec2 i = Box2DHelper::ToVec2(m_world->convertUnitsToMeters(_impulse));
            if (_multiplyMass)
                i *= m_body->GetMass();

            m_body->ApplyLinearImpulseToCenter(
                i,
                true);
        }

        //////////////////////////////////////////
        void applyForce(Vec2F const& _force, bool _multiplyMass = true)
        {
            if (!m_body)
            {
                rebuildBody();

                if (!m_body)
                    return;
            }

            b2Vec2 f = Box2DHelper::ToVec2(m_world->convertUnitsToMeters(_force));
            if (_multiplyMass)
                f *= m_body->GetMass();

            m_body->ApplyForceToCenter(
                f,
                true);
        }

        //////////////////////////////////////////
        inline void setVelocity(Vec2F const& _velocity)
        {
            if (!m_body)
            {
                rebuildBody();

                if (!m_body)
                    return;
            }

            m_body->SetLinearVelocity(Box2DHelper::ToVec2(m_world->convertUnitsToMeters(_velocity)));
        }

        //////////////////////////////////////////
        inline Vec2F getVelocity()
        {
            if (!m_body)
                return Vec2F::c_zero;

            return m_world->convertMetersToUnits(
                Box2DHelper::ToVec2F32(
                    m_body->GetLinearVelocity()));
        }

        //////////////////////////////////////////
        void destroyBody();


        //////////////////////////////////////////
        AABB2D getAABB();

    protected:

        //////////////////////////////////////////
        Rigidbody2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(PhysicsWorld2D* _world = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EcsWorld* _world,
            EntityCopyData _copyData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processComponentRemoved() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityEnabled() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityDisabled() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void setWorld(PhysicsWorld2D* _world);


        //////////////////////////////////////////
        void notifyWorldDestroyed();

        //////////////////////////////////////////
        void notifyWorldClear();


        //////////////////////////////////////////
        void collectColliders(
            Vector<BoxCollider2D*>& _boxColliders,
            Vector<CircleCollider2D*>& _circleColliders);

        //////////////////////////////////////////
        void updateBodyEnabled();

    protected:
        PhysicsWorld2D* m_world;

        bool m_enabled;
        Rigidbody2DType m_type;
        F32 m_linearDamping;
        F32 m_angularDamping;
        F32 m_gravityScale;
        U16 m_collisionMask;
        U16 m_categoryMask;
        U8 m_flags;

        b2Body* m_body;

        Transform2DPtr m_transform2D;
        Transform3DPtr m_transform3D;

        Vec2F m_fixedUpdateStartPosition;
        F32 m_fixedUpdateStartAngle;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRigidbody2D_hpp_
//////////////////////////////////////////
