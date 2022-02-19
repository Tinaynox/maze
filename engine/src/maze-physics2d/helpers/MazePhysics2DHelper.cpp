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
#include "maze-physics2d/helpers/MazePhysics2DHelper.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/ecs/components/MazeCollider2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Physics2DHelper
    {
        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Size Raycast(
            Vec2DF const& _srcPosition,
            Vec2DF const& _destPosition,
            Vector<RaycastHit2D>& _result)
        {
            _result.clear();

            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            Box2DHelper::RayCast(
                world,
                _srcPosition,
                _destPosition,
                [&](
                    Rigidbody2D* _rigidbody2D,
                    Collider2D* _collider2D,
                    Vec2DF const& _point,
                    Vec2DF const& _normal) -> bool
                {
                    _result.emplace_back(
                        RaycastHit2D
                        {
                            _point,
                            _normal,
                            _collider2D,
                            _rigidbody2D 
                        });

                    return false;
                });

            return _result.size();
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Size Raycast(
            Vec2DF const& _origin,
            Vec2DF const& _direction,
            F32 _distance,
            Vector<RaycastHit2D>& _result)
        {
            return Raycast(_origin, _origin + _direction * _distance, _result);
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API RaycastHit2DPtr Raycast(
            Vec2DF const& _srcPosition,
            Vec2DF const& _destPosition)
        {
            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            RaycastHit2DPtr result;

            Box2DHelper::RayCast(
                world,
                _srcPosition,
                _destPosition,
                [&](
                    Rigidbody2D* _rigidbody2D,
                    Collider2D* _collider2D,
                    Vec2DF const& _point,
                    Vec2DF const& _normal) -> bool
                {
                    result = std::make_shared<RaycastHit2D>(
                        _point,
                        _normal,
                        _collider2D,
                        _rigidbody2D);

                    return true;
                });

            return result;
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API RaycastHit2DPtr Raycast(
            Vec2DF const& _origin,
            Vec2DF const& _direction,
            F32 _distance)
        {
            return Raycast(
                _origin,
                _origin + _direction * _distance);
        }
        
        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<RaycastHit2D> RaycastAll(
            Vec2DF const& _origin,
            Vec2DF const& _direction,
            F32 _distance)
        {
            Vector<RaycastHit2D> result;
            Raycast(_origin, _direction, _distance, result);

            return result;
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<RaycastHit2D> RaycastAll(
            Vec2DF const& _srcPosition,
            Vec2DF const& _destPosition)
        {
            Vector<RaycastHit2D> result;
            Raycast(_srcPosition, _destPosition, result);

            return result;
        }


        //////////////////////////////////////////
        MAZE_PHYSICS2D_API OverlapHit2DPtr OverlapZone(
            Vec2DF const& _from,
            Vec2DF const& _to)
        {
            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            b2Vec2 posFrom = Box2DHelper::ToVec2(world->convertUnitsToMeters(_from));
            b2Vec2 posTo = Box2DHelper::ToVec2(world->convertUnitsToMeters(_to));

            b2AABB aabb;
            aabb.lowerBound.Set(posFrom.x, posFrom.y);
            aabb.upperBound.Set(posTo.x, posTo.y);

            OverlapHit2DPtr result;

            Box2DHelper::CustomOverlapCallback callback(
                [&](b2Fixture* _fixture) -> bool
            {
                b2Body* body = _fixture->GetBody();

                Collider2D* collider = static_cast<Collider2D*>((void*)_fixture->GetUserData().pointer);
                Rigidbody2D* rigidbody = reinterpret_cast<Rigidbody2D*>(body->GetUserData().pointer);

                result = std::make_shared<OverlapHit2D>(
                    collider,
                    rigidbody);

                return true;
            });

            world->getBox2DWorld()->QueryAABB(&callback, aabb);

            return result;
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API OverlapHit2DPtr OverlapRect(
            Vec2DF const& _position,
            Vec2DF const& _size)
        {
            Vec2DF const halfSize = _size * 0.5f;

            return OverlapZone(_position - halfSize, _position + halfSize);
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapZoneAll(
            Vec2DF const& _from,
            Vec2DF const& _to,
            std::function<bool(b2Fixture* _fixture)> const& _predicate)
        {
            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            b2Vec2 posFrom = Box2DHelper::ToVec2(world->convertUnitsToMeters(_from));
            b2Vec2 posTo = Box2DHelper::ToVec2(world->convertUnitsToMeters(_to));

            b2AABB aabb;
            aabb.lowerBound.Set(posFrom.x, posFrom.y);
            aabb.upperBound.Set(posTo.x, posTo.y);

            Vector<OverlapHit2DPtr> result;

            Box2DHelper::CustomOverlapCallback callback(
                [&](b2Fixture* _fixture) -> bool
            {
                if (_predicate && !_predicate(_fixture))
                    return false;

                b2Body* body = _fixture->GetBody();

                Collider2D* collider = static_cast<Collider2D*>((void*)_fixture->GetUserData().pointer);
                Rigidbody2D* rigidbody = reinterpret_cast<Rigidbody2D*>(body->GetUserData().pointer);

                result.emplace_back(
                    std::make_shared<OverlapHit2D>(
                    collider,
                    rigidbody));

                return false;
            });

            world->getBox2DWorld()->QueryAABB(&callback, aabb);

            return result;
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapZoneAll(
            Vec2DF const& _from,
            Vec2DF const& _to)
        {
            return OverlapZoneAll(_from, _to, nullptr);
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapSegmentAll(
            Vec2DF const& _position,
            Vec2DF const& _direction,
            F32 _distance,
            F32 _width)
        {
            Vector<OverlapHit2DPtr> result;
            Vec2DF pos0 = _position;
            Vec2DF pos1 = _position + _direction * _distance;
            Vec2DF min = { Math::Min(pos0.x, pos1.x) - _width * 0.5f, Math::Min(pos0.y, pos1.y) - _width * 0.5f };
            Vec2DF max = { Math::Max(pos0.x, pos1.x) + _width * 0.5f, Math::Max(pos0.y, pos1.y) + _width * 0.5f };

            Vec2DF p = _direction.perpendicular();

            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            b2PolygonShape polyShape;
            b2Vec2 points[4] =
            {
                Box2DHelper::ToVec2(world->convertUnitsToMeters({ pos0 - p * _width * 0.5f })),
                Box2DHelper::ToVec2(world->convertUnitsToMeters({ pos0 + p * _width * 0.5f })),
                Box2DHelper::ToVec2(world->convertUnitsToMeters({ pos1 - p * _width * 0.5f })),
                Box2DHelper::ToVec2(world->convertUnitsToMeters({ pos1 + p * _width * 0.5f }))
            };
            polyShape.Set(points, 4);

            static const b2Transform nullTransform(b2Vec2(0, 0), b2Rot(0));

            return OverlapZoneAll(
                min,
                max,
                [&](b2Fixture* _fixture) -> bool
                {
                    if (b2TestOverlap(
                        _fixture->GetShape(),
                        0,
                        &polyShape,
                        0,
                        _fixture->GetBody()->GetTransform(),
                        nullTransform))
                    {
                        return true;
                    }
                    
                    return false;
                });
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapRectAll(
            Vec2DF const& _position,
            Vec2DF const& _size)
        {
            Vec2DF const halfSize = _size * 0.5f;
            return OverlapZoneAll(_position - halfSize, _position + halfSize);
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API OverlapHit2DPtr OverlapPoint(
            Vec2DF const& _position)
        {
            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            return OverlapRect(_position, world->convertMetersToUnits({ 0.00002f, 0.00002f }));
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapCircleAll(
            Vec2DF const& _position,
            F32 _radius)
        {
            PhysicsWorld2DPtr const& world = Physics2DManager::GetInstancePtr()->getWorld();

            b2CircleShape shape;
            shape.m_p = Box2DHelper::ToVec2(world->convertUnitsToMeters(_position));
            shape.m_radius = world->convertUnitsToMeters(_radius);

            Vector<OverlapHit2DPtr> result;

            static const b2Transform nullTransform(b2Vec2(0, 0), b2Rot(0));

            Box2DHelper::CustomOverlapCallback callback(
                [&](b2Fixture* _fixture) -> bool
            {
                if (!b2TestOverlap(
                    _fixture->GetShape(),
                    0,
                    &shape,
                    0,
                    _fixture->GetBody()->GetTransform(),
                    nullTransform))
                {
                    return false;
                }

                b2Body* body = _fixture->GetBody();

                Collider2D* collider = static_cast<Collider2D*>((void*)_fixture->GetUserData().pointer);
                Rigidbody2D* rigidbody = reinterpret_cast<Rigidbody2D*>(body->GetUserData().pointer);

                result.emplace_back(
                    std::make_shared<OverlapHit2D>(
                        collider,
                        rigidbody));

                return false;
            });

            b2AABB aabb;
            shape.ComputeAABB(&aabb, nullTransform, 0);

            world->getBox2DWorld()->QueryAABB(&callback, aabb);

            return result;
        }

    } // namespace Physics2DHelper
    /////////////////////////////////////////
    

} // namespace Maze
//////////////////////////////////////////
