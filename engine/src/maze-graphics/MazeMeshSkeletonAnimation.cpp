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
#include "maze-graphics/MazeMeshSkeletonAnimation.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Struct MeshSkeletonAnimationBone
    //
    //////////////////////////////////////////
    void MeshSkeletonAnimationBone::evaluateBoneTransform(
        F32 _time,
        Vec3F& _outTranslation,
        Quaternion& _outRotation,
        Vec3F& _outScale) const
    {
        _outTranslation.x = translation[0].evaluate(_time);
        _outTranslation.y = translation[1].evaluate(_time);
        _outTranslation.z = translation[2].evaluate(_time);

        _outRotation = rotation.evaluate(_time);

        _outScale.x = scale[0].evaluate(_time);
        _outScale.y = scale[1].evaluate(_time);
        _outScale.z = scale[2].evaluate(_time);
    }


    //////////////////////////////////////////
    // Class MeshSkeletonAnimation
    //
    //////////////////////////////////////////
    MeshSkeletonAnimation::MeshSkeletonAnimation()
    {
    }

    //////////////////////////////////////////
    MeshSkeletonAnimation::~MeshSkeletonAnimation()
    {
    }

    //////////////////////////////////////////
    MeshSkeletonAnimationPtr MeshSkeletonAnimation::Create()
    {
        MeshSkeletonAnimationPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshSkeletonAnimation, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MeshSkeletonAnimation::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimation::clear()
    {
        m_boneAnimations.clear();
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimation::evaluateBoneTransform(
        MeshSkeleton::BoneIndex _i,
        F32 _time,
        Vec3F& _outTranslation,
        Quaternion& _outRotation,
        Vec3F& _outScale) const
    {
        MAZE_DEBUG_ASSERT(_i >= 0 && _i < (MeshSkeleton::BoneIndex)m_boneAnimations.size());

        MeshSkeletonAnimationBone const& animationBone = m_boneAnimations[_i];

        animationBone.evaluateBoneTransform(
            _time,
            _outTranslation,
            _outRotation,
            _outScale);
    }


} // namespace Maze
//////////////////////////////////////////
