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
#if (!defined(_MazeBox2DHelper_hpp_))
#define _MazeBox2DHelper_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include <box2d/box2d.h>
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(Collider2D);


    //////////////////////////////////////////
    namespace Box2DHelper
    {
        //////////////////////////////////////////
        using ReportFixturePredicate = std::function<bool(b2Fixture*)>;


        //////////////////////////////////////////
        MAZE_PHYSICS2D_API inline b2Vec2 ToVec2(Vec2F const& _vec)
        {
            return b2Vec2(_vec.x, _vec.y);
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API inline Vec2F ToVec2F32(b2Vec2 const& _vec)
        {
            return Vec2F(_vec.x, _vec.y);
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API inline AABB2D ToAABB2D(b2AABB const& _aabb)
        {
            AABB2D aabb;
            aabb.setMin(ToVec2F32(_aabb.lowerBound));
            aabb.setMax(ToVec2F32(_aabb.upperBound));
            return aabb;
        }

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API inline ColorF128 ToColor128F(b2Color const& _color)
        {
            return ColorF128(_color.r, _color.g, _color.b, _color.a);
        }


        //////////////////////////////////////////
        // Struct QueryCallback 
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API QueryCallback
            : b2QueryCallback
        {
        public:

            //////////////////////////////////////////
            QueryCallback(ReportFixturePredicate const& _pred);

            //////////////////////////////////////////
            bool ReportFixture(b2Fixture* _fixture);


        public:
            ReportFixturePredicate pred;
        };


        //////////////////////////////////////////
        // Struct CheckBodyOverlapCallback 
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API CheckBodyOverlapCallback
            : b2QueryCallback
        {
            //////////////////////////////////////////
            CheckBodyOverlapCallback(
                const b2Body* _body,
                const ReportFixturePredicate& _pred
            );

            //////////////////////////////////////////
            bool ReportFixture(b2Fixture* _fixture);


        public:
            const b2Body* body;
            bool isOverlap;
            ReportFixturePredicate pred;
        };


        //////////////////////////////////////////
        // Struct CheckOverlapCallback 
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API CheckOverlapCallback
            : b2QueryCallback
        {
            //////////////////////////////////////////
            CheckOverlapCallback(
                b2Vec2 const& _pos,
                ReportFixturePredicate const& _pred);

            //////////////////////////////////////////
            bool ReportFixture(b2Fixture* _fixture);


        public:
            b2Vec2 pos;
            bool isOverlap;
            ReportFixturePredicate pred;
        };


        //////////////////////////////////////////
        // Struct CustomOverlapCallback 
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API CustomOverlapCallback
            : b2QueryCallback
        {
            //////////////////////////////////////////
            CustomOverlapCallback(ReportFixturePredicate const& _pred);

            //////////////////////////////////////////
            bool ReportFixture(b2Fixture* _fixture);


        public:
            bool isOverlap;
            ReportFixturePredicate pred;
        };


        //////////////////////////////////////////
        // Struct QueryEntitiesOverlapCallback 
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API QueryEntitiesOverlapCallback
            : b2QueryCallback
        {
            //////////////////////////////////////////
            QueryEntitiesOverlapCallback(ReportFixturePredicate const& _pred);

            //////////////////////////////////////////
            bool ReportFixture(b2Fixture* _fixture);


        public:
            Set<Entity*> entities;
            ReportFixturePredicate pred;
        };


        //////////////////////////////////////////
        // Struct RayCastWrapper 
        //
        //////////////////////////////////////////
        class MAZE_PHYSICS2D_API RayCastWrapper
            : public b2RayCastCallback
        {
        public:

            //////////////////////////////////////////
            using Callback = std::function<
                F32(
                    b2Fixture* _fixture,
                    b2Vec2 const& _point,
                    b2Vec2 const& _normal,
                    F32 _fraction)>;

            //////////////////////////////////////////
            RayCastWrapper(Callback _callback) :
                m_callback(_callback) {}

        public:

            //////////////////////////////////////////
            virtual F32 ReportFixture(
                b2Fixture* _fixture,
                b2Vec2 const& _point,
                b2Vec2 const& _normal,
                F32 _fraction) MAZE_OVERRIDE
            {
                return m_callback(_fixture, _point, _normal, _fraction);
            }

        private:
            Callback m_callback;
        };


        //////////////////////////////////////////
        MAZE_PHYSICS2D_API F32 RayCast(
            PhysicsWorld2DPtr const& _world,
            Vec2F const& _from,
            Vec2F const& _to,
            std::function<
            bool(
                Rigidbody2D* _rigidbody2D,
                Collider2D* _collider2D,
                Vec2F const& _point,
                Vec2F const& _normal)> _filter = nullptr);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API b2AABB GetBodyAABB(b2Body const* _body);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API AABB2D GetBodyAABB2D(
            PhysicsWorld2D const* _world,
            b2Body const* _body);

        ////////////////////////////////////
        MAZE_PHYSICS2D_API bool IsOverlap(b2World const* _world, b2Body const* _body, ReportFixturePredicate _pred);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API bool IsOverlap(b2World const* _world, b2Vec2 const& _pos, ReportFixturePredicate _pred);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API bool IsOverlap(b2World const* _world, b2AABB const& _aabb, ReportFixturePredicate _pred);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Set<Entity*> GetEntitiesOverlap(b2World const* _world, b2AABB const& _aabb, ReportFixturePredicate _pred);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API bool IsShapeValid(b2Vec2 const* _vertices, int32 _count);


    } // namespace Box2DHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeBox2DHelper_hpp_
//////////////////////////////////////////
