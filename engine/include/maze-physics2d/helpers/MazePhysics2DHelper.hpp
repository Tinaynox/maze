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
#if (!defined(_MazePhysics2DHelper_hpp_))
#define _MazePhysics2DHelper_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Collider2D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);


    //////////////////////////////////////////
    namespace Physics2DHelper
    {
        //////////////////////////////////////////
        using OverlapHit2DPtr = Maze::SharedPtr<struct OverlapHit2D>;
        using RaycastHit2DPtr = Maze::SharedPtr<struct RaycastHit2D>;


        //////////////////////////////////////////
        // Struct OverlapHit2D
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API OverlapHit2D
        {
            //////////////////////////////////////////
            OverlapHit2D()
                : collider(nullptr)
                , rigidbody(nullptr)
            {}

            //////////////////////////////////////////
            OverlapHit2D(
                Collider2D* _collider,
                Rigidbody2D* _rigidbody)
                : collider(_collider)
                , rigidbody(_rigidbody)
            {}
            
            Collider2D* collider;
            Rigidbody2D* rigidbody;
        };


        //////////////////////////////////////////
        // Struct RaycastHit2D
        //
        //////////////////////////////////////////
        struct MAZE_PHYSICS2D_API RaycastHit2D
        {
            //////////////////////////////////////////
            RaycastHit2D()
                : point(Vec2F::c_zero)
                , normal(Vec2F::c_zero)
                , collider(nullptr)
                , rigidbody(nullptr)
            {}

            //////////////////////////////////////////
            RaycastHit2D(
                Vec2F const& _point,
                Vec2F const& _normal,
                Collider2D* _collider,
                Rigidbody2D* _rigidbody)
                : point(_point)
                , normal(_normal)
                , collider(_collider)
                , rigidbody(_rigidbody)
            {}


            Vec2F point;
            Vec2F normal;
            Collider2D* collider;
            Rigidbody2D* rigidbody;
        };

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Size Raycast(
            Vec2F const& _srcPosition,
            Vec2F const& _destPosition,
            Vector<RaycastHit2D>& _result);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Size Raycast(
            Vec2F const& _origin,
            Vec2F const& _direction,
            F32 _distance,
            Vector<RaycastHit2D>& _result);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API RaycastHit2DPtr Raycast(
            Vec2F const& _srcPosition,
            Vec2F const& _destPosition);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API RaycastHit2DPtr Raycast(
            Vec2F const& _origin,
            Vec2F const& _direction,
            F32 _distance);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<RaycastHit2D> RaycastAll(
            Vec2F const& _origin,
            Vec2F const& _direction,
            F32 _distance);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<RaycastHit2D> RaycastAll(
            Vec2F const& _srcPosition,
            Vec2F const& _destPosition);


        //////////////////////////////////////////
        MAZE_PHYSICS2D_API OverlapHit2DPtr OverlapZone(
            Vec2F const& _from,
            Vec2F const& _to);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API OverlapHit2DPtr OverlapRect(
            Vec2F const& _position,
            Vec2F const& _size);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapZoneAll(
            Vec2F const& _from,
            Vec2F const& _to);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapSegmentAll(
            Vec2F const& _position,
            Vec2F const& _direction,
            F32 _distance,
            F32 _width);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapRectAll(
            Vec2F const& _position,
            Vec2F const& _size);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API OverlapHit2DPtr OverlapPoint(
            Vec2F const& _position);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapPointAll(
            Vec2F const& _position);

        //////////////////////////////////////////
        MAZE_PHYSICS2D_API Vector<OverlapHit2DPtr> OverlapCircleAll(
            Vec2F const& _position,
            F32 _radius);
        

    } // namespace Physics2DHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysics2DHelper_hpp_
//////////////////////////////////////////
