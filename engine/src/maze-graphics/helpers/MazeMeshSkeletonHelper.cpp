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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/helpers/MazeMeshSkeletonHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MeshSkeletonHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API void CalculateBindPoseLocalTransforms(
            MeshSkeleton const& _skeleton,
            Vector<Vec3F>& _outTranslations,
            Vector<Quaternion>& _outRotations,
            Vector<Vec3F>& _outScales)
        {
            Size bonesCount = _skeleton.getBonesCount();
            _outTranslations.resize(bonesCount);
            _outRotations.resize(bonesCount);
            _outScales.resize(bonesCount);

            TMat rootTransformInv = _skeleton.getRootTransform().inversed();
            for (MeshSkeleton::BoneIndex i = 0; i < (MeshSkeleton::BoneIndex)bonesCount; ++i)
            {
                MeshSkeleton::Bone const& bone = _skeleton.getBone(i);

                TMat parentInv = bone.parentBoneIndex != -1
                    ? _skeleton.getBone(bone.parentBoneIndex).inversedBindPoseTransformMS
                    : rootTransformInv;
                TMat bindLocal = parentInv.transform(bone.inversedBindPoseTransformMS.inversed());

                _outTranslations[i] = bindLocal.getTranslation();
                _outRotations[i] = Quaternion(bindLocal);
                _outScales[i] = bindLocal.getScaleSignless();
            }
        }

        //////////////////////////////////////////
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
            Vec3F& _inOutScale)
        {
            Vec3F deltaTranslation = _translation - _bindTranslation;

            Quaternion deltaRotation = _bindRotation.inversed() * _rotation;
            deltaRotation.normalize();

            Vec3F deltaScale;
            for (S32 c = 0; c < 3; ++c)
                deltaScale[c] = Math::Abs(_bindScale[c]) > 1e-6f ? _scale[c] / _bindScale[c] : 1.0f;

            _inOutTranslation += deltaTranslation * _weight;
            _inOutRotation = _inOutRotation * Quaternion::Slerp(_weight, Quaternion::c_identity, deltaRotation);
            _inOutScale *= Vec3F::c_one + (deltaScale - Vec3F::c_one) * _weight;
        }

    } // namespace MeshSkeletonHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
