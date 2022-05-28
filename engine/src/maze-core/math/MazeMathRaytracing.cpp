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
#include "maze-core/math/MazeMathRaytracing.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeRay.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


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
        MAZE_CORE_API bool RaycastCube(
            Vec3DF const& _rayPoint,
            Vec3DF const& _rayDirection,
            Vec3DF const& _cubeCenter,
            Vec3DF const& _cubeForward,
            Vec3DF const& _cubeUp,
            Vec3DF const& _cubeScale,
            F32& _dist)
        {
            Vec3DF cubeRight = _cubeUp.crossProduct(_cubeForward).normalizedCopy();
            Mat4DF cubeTransform =
                Mat4DF::CreateChangeOfBasisMatrix(cubeRight, _cubeUp, _cubeForward) *
                Mat4DF::CreateScaleMatrix(_cubeScale * 0.5f);
            cubeTransform = cubeTransform.inversedAffineCopy();

            Ray localRay(
                cubeTransform.transformAffine(_rayPoint - _cubeCenter),
                cubeTransform.transformAffine(_rayDirection));

            Vec3DF const& a = localRay.getPoint();
            Vec3DF const& k = localRay.getDirection();

            F32 t[6];
            F32 u[6];
            F32 v[6];

            // Front and back
            if (Abs(k.z) > 0.000001f)
            {
                t[0] = (a.z - 1.0f) / -k.z;
                t[1] = (a.z + 1.0f) / -k.z;
                u[0] = a.x + k.x * t[0];
                v[0] = a.y + k.y * t[0];
                u[1] = a.x + k.x * t[1];
                v[1] = a.y + k.y * t[1];
            }
            else
            {
                t[0] = t[1] = 100e6;
                u[0] = v[0] = u[1] = v[1] = 0.0f;
            }

            // Left and Right
            if (Abs(k.x) > 0.000001f)
            {
                t[2] = (a.x - 1.0f) / -k.x;
                t[3] = (a.x + 1.0f) / -k.x;
                u[2] = a.z + k.z * t[2];
                v[2] = a.y + k.y * t[2];
                u[3] = a.z + k.z * t[3];
                v[3] = a.y + k.y * t[3];
            }
            else
            {
                t[2] = t[3] = 100e6;
                u[2] = v[2] = u[3] = v[3] = 0.0f;
            }

            // Top and Bottom
            if (Abs(k.y) > 0.000001f)
            {
                t[4] = (a.y - 1.0f) / -k.y;
                t[5] = (a.y + 1.0f) / -k.y;
                u[4] = a.x + k.x * t[4];
                v[4] = a.z + k.z * t[4];
                u[5] = a.x + k.x * t[5];
                v[5] = a.z + k.z * t[5];
            }
            else
            {
                t[4] = t[5] = 100e6;
                u[4] = v[4] = u[5] = v[5] = 0.0f;
            }

            F32 finalU = 0.0f;
            F32 finalV = 0.0f;
            F32 finalT = 100e6;
            S32 finalIndex = -1;
            for (S32 i = 0; i < 6; ++i)
            {
                if ((t[i] < finalT) && (t[i] > 0.0f) && (Math::Abs(u[i]) <= 1.0f) && (Math::Abs(v[i]) <= 1.0))
                {
                    finalT = t[i];
                    finalIndex = i;
                    finalU = u[i];
                    finalV = v[i];
                }
            }

            if (finalIndex >= 0)
            {
                _dist = finalT;
                return true;
            }

            return false;
        }

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
            Vec3DF cylinderRight = _cylinderUp.crossProduct(_cylinderForward).normalizedCopy();
            Mat4DF cylinderTransform = Mat4DF::CreateChangeOfBasisMatrix(cylinderRight, _cylinderUp, _cylinderForward).inversedAffineCopy();

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
                    if (point2.x * point2.x + point2.y * point2.y < _cylinderRadius * _cylinderRadius)
                    {
                        _dist = t2;
                        pointFound = true;
                    }
                }

                F32 t3 = (localRay.getPoint().z + cylinderHalfHeight) / -localRay.getDirection().z;
                if (t3 > 0.0f && t3 < _dist)
                {
                    Vec3DF point3 = localRay.getPoint(t3);
                    if (point3.x * point3.x + point3.y * point3.y < _cylinderRadius * _cylinderRadius)
                    {
                        _dist = t3;
                        pointFound = true;
                    }
                }
            }

            return pointFound;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastCone(
            Vec3DF const& _rayPoint,
            Vec3DF const& _rayDirection,
            Vec3DF const& _coneOrigin,
            Vec3DF const& _coneForward,
            F32 _coneRadius,
            F32 _coneHeight,
            F32& _dist)
        {
            Vec3DF coneUp = _coneForward.perpendicular();
            Vec3DF coneRight = coneUp.crossProduct(_coneForward).normalizedCopy();
            Mat4DF coneTransform =
                Mat4DF::CreateChangeOfBasisMatrix(coneRight, coneUp, _coneForward) *
                Mat4DF::CreateScaleMatrix(_coneRadius, _coneRadius, _coneHeight);
            coneTransform = coneTransform.inversedAffineCopy();

            Ray localRay(
                coneTransform.transformAffine(_rayPoint - _coneOrigin) - Vec3DF::c_unitZ,
                coneTransform.transformAffine(_rayDirection));

            F32 a = localRay.getDirection().x * localRay.getDirection().x + localRay.getDirection().y * localRay.getDirection().y - localRay.getDirection().z * localRay.getDirection().z;
            F32 b = 2.0f * (localRay.getPoint().x * localRay.getDirection().x + localRay.getPoint().y * localRay.getDirection().y - localRay.getPoint().z * localRay.getDirection().z);
            F32 c = localRay.getPoint().x * localRay.getPoint().x + localRay.getPoint().y * localRay.getPoint().y - localRay.getPoint().z * localRay.getPoint().z;

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
                    if (point0.z > -1.0f && point0.z < 0.0f)
                    {
                        _dist = t0;
                        pointFound = true;
                    }
                }

                if (t1 > 0.0f && t1 < _dist)
                {
                    Vec3DF point1 = localRay.getPoint(t1);
                    if (point1.z > -1.0f && point1.z < 0.0f)
                    {
                        _dist = t1;
                        pointFound = true;
                    }
                }

            }

            if (Abs(localRay.getDirection().z) > 0.00001f)
            {
                F32 t2 = (localRay.getPoint().z + 1) / -localRay.getDirection().z;
                if (t2 > 0.0f && t2 < _dist)
                {
                    Vec3DF point2 = localRay.getPoint(t2);
                    if (point2.x * point2.x + point2.y * point2.y < 1.0f)
                    {
                        _dist = t2;
                        pointFound = true;
                    }
                }
            }

            return pointFound;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastTorus(
            Vec3DF const& _rayPoint,
            Vec3DF const& _rayDirection,
            Vec3DF const& _torusOrigin,
            Vec3DF const& _torusForward,
            F32 _torusRadius,
            F32 _torusCsRadius,
            F32& _dist)
        {
            Vec3DF torusUp = _torusForward.perpendicular();
            Vec3DF torusRight = torusUp.crossProduct(_torusForward).normalizedCopy();
            Mat4DF torusTransform = Mat4DF::CreateChangeOfBasisMatrix(torusRight, torusUp, _torusForward);
            torusTransform = torusTransform.inversedAffineCopy();

            Ray localRay(
                torusTransform.transformAffine(_rayPoint - _torusOrigin),
                torusTransform.transformAffine(_rayDirection));

            F32 ox = localRay.getPoint().x;
            F32 oy = localRay.getPoint().y;
            F32 oz = localRay.getPoint().z;

            F32 dx = localRay.getDirection().x;
            F32 dy = localRay.getDirection().y;
            F32 dz = localRay.getDirection().z;

            F32 sumDSqrd = dx * dx + dz * dz + dy * dy;
            F32 e = ox * ox + oz * oz + oy * oy -
                _torusRadius * _torusRadius - _torusCsRadius * _torusCsRadius;
            F32 f = ox * dx + oz * dz + oy * dy;
            F32 fourASqrd = 4.0f * _torusRadius * _torusRadius;

            F32 c0 = e * e - fourASqrd * (_torusCsRadius * _torusCsRadius - oz * oz);
            F32 c1 = 4.0f * f * e + 2.0f * fourASqrd * oz * dz;
            F32 c2 = 2.0f * sumDSqrd * e + 4.0f * f * f + fourASqrd * dz * dz;
            F32 c3 = 4.0f * sumDSqrd * f;
            F32 c4 = sumDSqrd * sumDSqrd;

            F32 solution[4];
            S32 solutionsCount = Math::SolveQuartic(c0, c1, c2, c3, c4, solution);

            // Ray misses the torus
            if (solutionsCount == 0)
                return false;

            // Find the smallest root greater than epsilon, if any
            // the roots array is not sorted
            _dist = 1e6f;
            bool found = false;
            for (S32 i = 0; i < solutionsCount; ++i)
            {
                F32 t = solution[i];
                if ((t > 0.0001f) && (t < _dist))
                {
                    _dist = t;
                    found = true;
                }
            }

            return found && Math::IsFinite(_dist);
        }
        
    } // namespace Math
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
