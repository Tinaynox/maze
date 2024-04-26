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
#if (!defined(_MazeMathGeometry_hpp_))
#define _MazeMathGeometry_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRay.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {

        //////////////////////////////////////////
        // A test to see if p0 is on the same side as p1 of a line segment ab
        //
        //////////////////////////////////////////
        MAZE_CORE_API inline bool IsSameSide(
            Vec3F _p0,
            Vec3F _p1,
            Vec3F _a,
            Vec3F _b)
        {
            Vec3F cp1 = (_b - _a).crossProduct(_p0 - _a);
            Vec3F cp2 = (_b - _a).crossProduct(_p1 - _a);

            if (cp1.dotProduct(cp2) >= 0)
                return true;
            else
                return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline bool IsPointInTriangle(
            Vec2F const& _point,
            Vec2F const& _a,
            Vec2F const& _b,
            Vec2F const& _c)
        {
            Vec2F vert[] = {_a, _b, _c};

            S32 c = 0;

            for (S32 i = 0, j = 2; i < 3; j = i++)
            {
                if (((vert[i].y > _point.y) != (vert[j].y>_point.y)) &&
                    (_point.x < (vert[j].x - vert[i].x) * (_point.y - vert[i].y) / (vert[j].y - vert[i].y) + vert[i].x))
                    c = !c;
            }

            return c > 0;
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline Vec3F GenTriangleNormal(
            Vec3F _t0,
            Vec3F _t1,
            Vec3F _t2)
        {
            Vec3F u = _t1 - _t0;
            Vec3F v = _t2 - _t0;

            Vec3F normal = u.crossProduct(v);

            return normal;
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline Vec3F ProjectionPointOnVector(Vec3F const& _a, Vec3F const& _b)
        {
            Vec3F bn = _b / _b.length();
            return bn * _a.dotProduct(bn);
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline Vec3F ProjectionPointOnPlane(
            Vec3F const& _point,
            Vec3F const& _planeOrigin,
            Vec3F const& _planeNormal)
        {
            Vec3F v = _point - _planeOrigin;
            F32 dist = v.dotProduct(_planeNormal);
            return _point - dist * _planeNormal;
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline bool IsPointInTriangle(
            Vec3F const& _point,
            Vec3F const& _a,
            Vec3F const& _b,
            Vec3F const& _c)
        {
            bool withinTriPrisim = 
                IsSameSide(_point, _a, _b, _c) && 
                IsSameSide(_point, _b, _a, _c) &&
                IsSameSide(_point, _c, _a, _b);

            // If it isn't it will never be on the triangle
            if (!withinTriPrisim)
                return false;

            // Calulate Triangle's Normal
            Vec3F n = GenTriangleNormal(_a, _b, _c);

            // Project the point onto this normal
            Vec3F projection = ProjectionPointOnVector(_point, n);

            // If the distance from the triangle to the point is 0
            //    it lies on the triangle
            if (projection.length() <= std::numeric_limits<F32>::epsilon())
                return true;
            else
                return false;
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline F32 DistanceFromPointToLine(
            Vec2F const& _point,
            Vec2F const& _linePoint0,
            Vec2F const& _linePoint1)
        {
            F32 normalLength = hypotf(_linePoint1.x - _linePoint0.x, _linePoint1.y - _linePoint0.y);
            F32 d = ((_point.x - _linePoint0.x) * (_linePoint1.y - _linePoint0.y) - (_point.y - _linePoint0.y) * (_linePoint1.x - _linePoint0.x)) / normalLength;
            return Abs(d);
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline bool CrossPoint(
            Vec2F const& _line0Point0,
            Vec2F const& _line0Point1,
            Vec2F const& _line1Point0,
            Vec2F const& _line1Point1,
            Vec2F* _result)
        {
            Vec3F segment0 = _line0Point1 - _line0Point0;
            Vec3F segment1 = _line1Point1 - _line1Point0;

            Vec3F prod0 = segment0.crossProduct(_line1Point0 - _line0Point0);
            Vec3F prod1 = segment0.crossProduct(_line1Point1 - _line0Point0);

            if (Sign(prod0.z) == Sign(prod1.z) || prod0.z == 0 || prod1.z == 0)
                return false;

            prod0 = segment1.crossProduct(_line0Point0 - _line1Point0);
            prod1 = segment1.crossProduct(_line0Point1 - _line1Point0);

            if (Sign(prod0.z) == Sign(prod1.z) || prod0.z == 0 || prod1.z == 0)
                return false;

            if (_result)
            {
                F32 a = fabs(prod0.z) / fabs(prod1.z - prod0.z);

                _result->x = _line0Point0.x + segment0.x * a;
                _result->y = _line0Point0.y + segment0.y * a;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline bool IsPointOnSegment(
            Vec3F const& _segmentPointA,
            Vec3F const& _segmentPointB,
            Vec3F const& _point,
            F32 _epsilon = 0.00001f)
        {
            F32 ab = (_segmentPointB - _segmentPointA).length();
            F32 ac = (_point - _segmentPointA).length();
            F32 cb = (_segmentPointB - _point).length();
            return (fabs(ab - ac - cb) <= _epsilon);
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline Vec3F ClosestPointOnLine(
            Vec3F const& _lineA,
            Vec3F const& _lineB,
            Vec3F const& _point)
        {
            Vec3F c = _point - _lineA;
            Vec3F v = _lineB - _lineA;
            
            v.normalize();
            F32 t = v.dotProduct(c);

            v *= t;

            return (_lineA + v);
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline Vec3F ClosestPointOnSegment(
            Vec3F const& _segmentPointA,
            Vec3F const& _segmentPointB,
            Vec3F const& _point)
        {
            Vec3F c = _point - _segmentPointA;
            Vec3F v = _segmentPointB - _segmentPointA;
            F32 d = v.length();
            v.normalize();
            F32 t = v.dotProduct(c);
 
            if (t < 0.0f)
                return _segmentPointA;

            if (t > d)
                return _segmentPointB;
 
            v *= t;

            return (_segmentPointA + v);
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline Vec3F Ortogonalize(Vec3F const& _v1, Vec3F const& _v2)
        {
            Vec3F v2ProjV1 = ClosestPointOnSegment(_v1, -_v1, _v2);
            Vec3F res = _v2 - v2ProjV1;
            res.normalize();
            return res;
        }

    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMathGeometry_hpp_
//////////////////////////////////////////
