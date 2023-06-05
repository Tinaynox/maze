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
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/ecs/components/MazeCollider2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Box2DHelper
    {

        //////////////////////////////////////////
        // Struct QueryCallback 
        //
        //////////////////////////////////////////
        QueryCallback::QueryCallback(
            ReportFixturePredicate const& _pred)
            : pred(_pred)
        {}

        //////////////////////////////////////////
        bool QueryCallback::ReportFixture(b2Fixture* _fixture)
        {
            // Skip by pred
            if (pred && !pred(_fixture))
                return true;

            return true;
        }


        //////////////////////////////////////////
        // Struct CheckBodyOverlapCallback 
        //
        //////////////////////////////////////////
        CheckBodyOverlapCallback::CheckBodyOverlapCallback(
            b2Body const* _body,
            ReportFixturePredicate const& _pred)
            : body(_body)
            , isOverlap(false)
            , pred(_pred)
        {}

        //////////////////////////////////////////
        bool CheckBodyOverlapCallback::ReportFixture(b2Fixture* _fixture)
        {
            // Skip self.
            if (_fixture->GetBody() == body)
                return true;

            // Skip by pred
            if (pred && !pred(_fixture))
                return true;

            for (
                const b2Fixture* bodyFixture = body->GetFixtureList();
                bodyFixture;
                bodyFixture = bodyFixture->GetNext())
            {
                if (bodyFixture->IsSensor())
                    continue;

                if (b2TestOverlap(
                    _fixture->GetShape(),
                    0,
                    bodyFixture->GetShape(),
                    0,
                    _fixture->GetBody()->GetTransform(),
                    body->GetTransform()))
                {
                    isOverlap = true;
                    return false;
                }
            }

            return true;
        }


        //////////////////////////////////////////
        // Struct CheckOverlapCallback 
        //
        //////////////////////////////////////////
        CheckOverlapCallback::CheckOverlapCallback(
            b2Vec2 const& _pos,
            ReportFixturePredicate const& _pred)
            : pos(_pos)
            , isOverlap(false)
            , pred(_pred)
        {}

        //////////////////////////////////////////
        bool CheckOverlapCallback::ReportFixture(b2Fixture* _fixture)
        {
            if (pred && !pred(_fixture))
                return true;

            b2EdgeShape fakeShape;
            fakeShape.SetTwoSided(pos, pos);
            fakeShape.m_radius = 0.000001f;

            static const b2Transform nullTransform(b2Vec2(0, 0), b2Rot(0));

            if (b2TestOverlap(
                _fixture->GetShape(),
                0,
                &fakeShape,
                0,
                _fixture->GetBody()->GetTransform(),
                nullTransform))
            {
                isOverlap = true;
                return false;
            }

            return true;
        }


        //////////////////////////////////////////
        // Struct CustomOverlapCallback 
        //
        //////////////////////////////////////////
        CustomOverlapCallback::CustomOverlapCallback(
            const ReportFixturePredicate& _pred)
            : isOverlap(false)
            , pred(_pred)
        {}

        //////////////////////////////////////////
        bool CustomOverlapCallback::ReportFixture(b2Fixture* fixture)
        {
            // Skip by pred
            if (pred && !pred(fixture))
                return true;

            isOverlap = true;
            return false;
        }


        //////////////////////////////////////////
        // Struct QueryOverlapCallback 
        //
        //////////////////////////////////////////
        QueryEntitiesOverlapCallback::QueryEntitiesOverlapCallback(
            const ReportFixturePredicate& _pred)
            : pred(_pred)
        {}

        //////////////////////////////////////////
        bool QueryEntitiesOverlapCallback::ReportFixture(b2Fixture* _fixture)
        {
            Rigidbody2D* rigidbody2D = reinterpret_cast<Rigidbody2D*>(_fixture->GetBody()->GetUserData().pointer);
            if (!rigidbody2D)
                return true;

            Entity* entity = rigidbody2D->getEntityRaw();

            if (entities.count(entity))
                return true;

            if (pred && !pred(_fixture))
                return true;

            entities.insert(entity);
            return true;
        }


        //////////////////////////////////////////
        F32 RayCast(
            PhysicsWorld2DPtr const& _world,
            Vec2DF const& _fromUnits,
            Vec2DF const& _toUnits,
            std::function<
            bool(
                Rigidbody2D* _rigidbody2D,
                Collider2D* _collider2D,
                Vec2DF const& _point,
                Vec2DF const& _normal)> _filter)
        {
            Vec2DF positionMeters = _world->convertUnitsToMeters(_fromUnits);
            Vec2DF targetMeters = _world->convertUnitsToMeters(_toUnits);

            // Box2D assert prevention
            if (targetMeters == positionMeters)
                return 0;

            F32 result = 0;

            RayCastWrapper callback(
                [&](
                    b2Fixture* _fixture,
                    const b2Vec2& _point,
                    const b2Vec2& _normal,
                    F32 _fraction) -> F32
                {
                    b2Body* body = _fixture->GetBody();

                    if (!body)
                        return 0;

                    void* userData = (void*)_fixture->GetBody()->GetUserData().pointer;
                    Rigidbody2D* rigidbody2D = nullptr;
                    Collider2D* collider2D = nullptr;

                    if (userData)
                    {
                        rigidbody2D = static_cast<Rigidbody2D*>(userData);
                        collider2D = static_cast<Collider2D*>((void*)_fixture->GetUserData().pointer);
                    }

                    if (_filter &&
                        !_filter(
                            rigidbody2D,
                            collider2D,
                            _world->convertMetersToUnits(Box2DHelper::ToVec2DF(_point)),
                            Box2DHelper::ToVec2DF(_normal)))
                    {
                        return -1;
                    }

                    Vec2DF pt = ToVec2DF(_point);

                    result = (pt - positionMeters).length();

                    return 0;
                });


            b2World* world = _world->getBox2DWorld();

            world->RayCast(&callback,
                Box2DHelper::ToVec2(positionMeters),
                Box2DHelper::ToVec2(targetMeters));

            return result;
        }


        //////////////////////////////////////////
        b2AABB GetBodyAABB(b2Body const* _body)
        {
            b2AABB result;
            b2Transform trans = _body->GetTransform();
            b2Fixture const* first = _body->GetFixtureList();

            for (b2Fixture const* fixture = first; fixture; fixture = fixture->GetNext())
            {
                b2AABB aabb;
                fixture->GetShape()->ComputeAABB(&aabb, trans, 0);
                if (fixture == first)
                    result = aabb;
                else
                    result.Combine(aabb);
            }

            return result;
        }

        //////////////////////////////////////////
        AABB2D GetBodyAABB2D(
            PhysicsWorld2D const* _world,
            b2Body const* _body)
        {
            b2AABB aabb = GetBodyAABB(_body);
            return _world->convertMetersToUnits(ToAABB2D(aabb));
        }

        //////////////////////////////////////////
        bool IsOverlap(b2World const* _world, b2Body const* _body, ReportFixturePredicate _pred)
        {
            CheckBodyOverlapCallback callback(_body, _pred);
            _world->QueryAABB(&callback, GetBodyAABB(_body));
            return callback.isOverlap;
        }

        //////////////////////////////////////////
        bool IsOverlap(b2World const* _world, b2Vec2 const& _pos, ReportFixturePredicate _pred)
        {
            b2AABB aabb;
            aabb.lowerBound.Set(_pos.x - 0.00001f, _pos.y - 0.00001f);
            aabb.upperBound.Set(_pos.x + 0.00001f, _pos.y + 0.00001f);

            CheckOverlapCallback callback(_pos, _pred);
            _world->QueryAABB(&callback, aabb);
            return callback.isOverlap;
        }


        //////////////////////////////////////////
        bool IsOverlap(b2World const* _world, b2AABB const& _aabb, ReportFixturePredicate _pred)
        {
            CustomOverlapCallback callback(_pred);
            _world->QueryAABB(&callback, _aabb);
            return callback.isOverlap;
        }

        //////////////////////////////////////////
        Set<Entity*> GetEntitiesOverlap(b2World const* _world, b2AABB const& _aabb, ReportFixturePredicate _pred)
        {
            QueryEntitiesOverlapCallback callback(_pred);
            _world->QueryAABB(&callback, _aabb);
            return callback.entities;
        }


        //////////////////////////////////////////
        bool IsShapeValid(b2Vec2 const* _vertices, int32 _count)
        {
            if (_count < 3 || _count > b2_maxPolygonVertices)
                return false;

            int32 n = b2Min(_count, b2_maxPolygonVertices);

            b2Vec2 ps[b2_maxPolygonVertices];
            int32 tempCount = 0;
            for (int32 i = 0; i < n; ++i)
            {
                b2Vec2 v = _vertices[i];

                bool unique = true;
                for (int32 j = 0; j < tempCount; ++j)
                {
                    if (b2DistanceSquared(v, ps[j]) < 0.5f * b2_linearSlop)
                    {
                        unique = false;
                        break;
                    }
                }

                if (unique)
                {
                    ps[tempCount++] = v;
                }
            }

            n = tempCount;
            if (n < 3)
                return false;

            // Find the right most point on the hull
            int32 i0 = 0;
            F32 x0 = ps[0].x;
            for (int32 i = 1; i < n; ++i)
            {
                F32 x = ps[i].x;
                if (x > x0 || (x == x0 && ps[i].y < ps[i0].y))
                {
                    i0 = i;
                    x0 = x;
                }
            }

            int32 hull[b2_maxPolygonVertices];
            int32 m = 0;
            int32 ih = i0;

            for (;;)
            {
                hull[m] = ih;

                int32 ie = 0;
                for (int32 j = 1; j < n; ++j)
                {
                    if (ie == ih)
                    {
                        ie = j;
                        continue;
                    }

                    b2Vec2 r = ps[ie] - ps[hull[m]];
                    b2Vec2 v = ps[j] - ps[hull[m]];
                    F32 c = b2Cross(r, v);
                    if (c < 0.0f)
                    {
                        ie = j;
                    }

                    // Collinearity check
                    if (c == 0.0f && v.LengthSquared() > r.LengthSquared())
                    {
                        ie = j;
                    }
                }

                ++m;
                ih = ie;

                if (ie == i0)
                {
                    break;
                }
            }

            if (m < 3)
                return false;

            return true;
        }


    } // namespace Box2DHelper
    /////////////////////////////////////////
    

} // namespace Maze
//////////////////////////////////////////
