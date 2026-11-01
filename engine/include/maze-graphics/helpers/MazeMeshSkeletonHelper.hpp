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
#if (!defined(_MazeMeshSkeletonHelper_hpp_))
#define _MazeMeshSkeletonHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-core/math/MazeQuaternion.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MeshSkeletonHelper
    {
        //////////////////////////////////////////
        // Bind pose in the same space as animation curves:
        // root bones are relative to the skeleton root transform, others to their parent bone
        MAZE_GRAPHICS_API void CalculateBindPoseLocalTransforms(
            MeshSkeleton const& _skeleton,
            Vector<Vec3F>& _outTranslations,
            Vector<Quaternion>& _outRotations,
            Vector<Vec3F>& _outScales);

        //////////////////////////////////////////
        // Accumulates weighted (pose - bindPose) delta:
        // T += dT * w; R = R * slerp(I, dR, w); S *= lerp(1, dS, w)
        MAZE_GRAPHICS_API void AccumulateAdditiveDelta(
            F32 _weight,
            Vec3F const& _translation,
            Quaternion const& _rotation,
            Vec3F const& _scale,
            Vec3F const& _bindTranslation,
            Quaternion const& _bindRotation,
            Vec3F const& _bindScale,
            Vec3F& _inOutTranslation,
            Quaternion& _inOutRotation,
            Vec3F& _inOutScale);

    } // namespace MeshSkeletonHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeletonHelper_hpp_
//////////////////////////////////////////
