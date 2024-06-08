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
#if (!defined(_MazeMathRaytracing_hpp_))
#define _MazeMathRaytracing_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeTMat.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Math
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastPlane(
            Vec3F const& _rayPoint,
            Vec3F const& _rayDirection,
            Vec3F const& _planePoint,
            Vec3F const& _planeNormal,
            F32& _dist);

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastCube(
            Vec3F const& _rayPoint,
            Vec3F const& _rayDirection,
            Vec3F const& _cubeCenter,
            Vec3F const& _cubeForward,
            Vec3F const& _cubeUp,
            Vec3F const& _cubeScale,
            F32& _dist);

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastSphere(
            Vec3F const& _rayPoint,
            Vec3F const& _rayDirection,
            Vec3F const& _sphereCenter,
            F32 _sphereRadius,
            F32& _dist);

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastCylinder(
            Vec3F const& _rayPoint,
            Vec3F const& _rayDirection,
            Vec3F const& _cylinderCenter,
            Vec3F const& _cylinderForward,
            F32 _cylinderRadius,
            F32 _cylinderHeight,
            F32& _dist);

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastCone(
            Vec3F const& _rayPoint,
            Vec3F const& _rayDirection,
            Vec3F const& _coneOrigin,
            Vec3F const& _coneForward,
            F32 _coneRadius,
            F32 _coneHeight,
            F32& _dist);

        //////////////////////////////////////////
        MAZE_CORE_API bool RaycastTorus(
            Vec3F const& _rayPoint,
            Vec3F const& _rayDirection,
            Vec3F const& _torusOrigin,
            Vec3F const& _torusForward,
            F32 _torusRadius,
            F32 _torusCsRadius,
            F32& _dist);


    } // namespace Math
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMathRaytracing_hpp_
//////////////////////////////////////////
