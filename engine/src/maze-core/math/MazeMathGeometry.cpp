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
#include "MazeCoreHeader.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastPlane(
            Vec3DF const& _rayPoint,
            Vec3DF const& _rayDirection,
            Vec3DF const& _planePoint,
            Vec3DF const& _planeNormal,
            F32& _dist)
        {
            F32 denom = _planeNormal.dotProduct(_rayDirection);
            if (Math::Abs(denom) > 0.0001f)
            {
                F32 t = (_planePoint - _rayPoint).dotProduct(_planeNormal) / denom;
                if (t >= 0.0f)
                {
                    Vec3DF p = _rayPoint + t * _rayDirection;
                    _dist = (p - _rayPoint).length();
                    return true;
                }
            }
            return false;
        };

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastSphere(
            Vec3DF const& _rayPoint,
            Vec3DF const& _rayDirection,
            Vec3DF const& _sphereCenter,
            F32 _sphereRadius,
            F32& _dist)
        {
            Vec3DF m = _rayPoint - _sphereCenter;
            F32 b = m.dotProduct(_rayDirection);
            F32 c = m.dotProduct(m) - _sphereRadius * _sphereRadius;

            // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0) 
            if (c > 0.0f && b > 0.0f)
                return false;
            F32 discr = b * b - c;

            // A negative discriminant corresponds to ray missing sphere 
            if (discr < 0.0f)
                return false;

            // Ray now found to intersect sphere, compute smallest t value of intersection
            _dist = -b - Sqrt(discr);

            // If t is negative, ray started inside sphere so clamp t to zero 
            if (_dist < 0.0f)
                _dist = 0.0f;

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastCylinder(
            Vec3DF const& _rayPoint,
            Vec3DF const& _rayDirection,
            Vec3DF const& _cylinderCenter,
            Vec3DF const& _cylinderForward,
            F32 _cylinderRadius,
            F32 _cylinderHeight,
            F32& _dist)
        {
            F32 cylinderHalfHeight = _cylinderHeight * 0.5f;

            Vec3DF _cylinderUp = _cylinderForward.perpendicular();
            Vec3DF _cylinderRight = _cylinderUp.crossProduct(_cylinderForward).normalizedCopy();
            Mat4DF cylinderTransform = Mat4DF::CreateChangeOfBasisMatrix(_cylinderRight, _cylinderUp, _cylinderForward).inversedAffineCopy();

            Ray localRay(
                cylinderTransform.transformAffine(_rayPoint - _cylinderCenter),
                cylinderTransform.transformAffine(_rayDirection));

            F32 a = localRay.getDirection().x * localRay.getDirection().x + localRay.getDirection().y * localRay.getDirection().y;
            F32 b = 2.0f * (localRay.getPoint().x * localRay.getDirection().x + localRay.getPoint().y * localRay.getDirection().y);
            F32 c = localRay.getPoint().x * localRay.getPoint().x + localRay.getPoint().y * localRay.getPoint().y - _cylinderRadius * _cylinderRadius;

            _dist = 100e6;
            bool pointFound = false;

            F32 delta = b * b - 4 * a * c;
            if (delta > 0.0f)
            {
                F32 deltaSqrt = Sqrt(delta);
                F32 t0 = (-b + deltaSqrt) / (2 * a);
                F32 t1 = (-b - deltaSqrt) / (2 * a);

                if (t0 > 0.0f)
                {
                    Vec3DF point0 = localRay.getPoint(t0);
                    if (Abs(point0.z) <= cylinderHalfHeight)
                    {
                        _dist = t0;
                        pointFound = true;
                    }
                }

                if (t1 > 0.0f && t1 < _dist)
                {
                    Vec3DF point1 = localRay.getPoint(t1);
                    if (Abs(point1.z) <= cylinderHalfHeight)
                    {
                        _dist = t1;
                        pointFound = true;
                    }
                }

            }

            if (Abs(localRay.getDirection().z) > 0.00001f)
            {
                F32 t2 = (localRay.getPoint().z - cylinderHalfHeight) / -localRay.getDirection().z;
                if (t2 > 0.0f && t2 < _dist)
                {
                    Vec3DF point2 = localRay.getPoint(t2);
                    if (point2.x * point2.x + point2.y * point2.y < _cylinderRadius)
                    {
                        _dist = t2;
                        pointFound = true;
                    }
                }

                F32 t3 = (localRay.getPoint().z + cylinderHalfHeight) / -localRay.getDirection().z;
                if (t3 > 0.0f && t3 < _dist)
                {
                    Vec3DF point3 = localRay.getPoint(t3);
                    if (point3.x * point3.x + point3.y * point3.y < _cylinderRadius)
                    {
                        _dist = t3;
                        pointFound = true;
                    }
                }
            }
            

            return pointFound;
            /*
            Vec3DF cylinderOrigin = _cylinderCenter;
            cylinderOrigin.y -= _cylinderHeight * 0.5f;

            F32 a = (_rayDirection.x * _rayDirection.x) + (_rayDirection.z * _rayDirection.z);
            F32 b = 2.0f * (_rayDirection.x * (_rayPoint.x - cylinderOrigin.x) + _rayDirection.z * (_rayPoint.z - cylinderOrigin.z));
            F32 c = (_rayPoint.x - cylinderOrigin.x) * (_rayPoint.x - cylinderOrigin.x) +
                    (_rayPoint.z - cylinderOrigin.z) * (_rayPoint.z - cylinderOrigin.z) -
                    (_cylinderRadius * _cylinderRadius);

            F32 delta = b * b - 4 * (a * c);
            if (delta < 0.001f)
                return false;

            F32 t1 = (-b - Sqrt(delta)) / (2.0f * a);
            F32 t2 = (-b + Sqrt(delta)) / (2.0f * a);

            if (t1 > t2)
                _dist = t2;
            else
                _dist = t1;

            F32 r = _rayPoint.y + _dist * _rayDirection.y;

            if ((r >= cylinderOrigin.y) && (r <= cylinderOrigin.y + _cylinderHeight))
                return true;
            else
                return false;
            */
        }

    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
