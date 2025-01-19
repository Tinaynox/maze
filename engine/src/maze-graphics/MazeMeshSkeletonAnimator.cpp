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
#include "maze-graphics/MazeMeshSkeletonAnimator.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimation.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class MeshSkeletonAnimator
    //
    //////////////////////////////////////////
    MeshSkeletonAnimator::MeshSkeletonAnimator()
    {
    }

    //////////////////////////////////////////
    MeshSkeletonAnimatorPtr MeshSkeletonAnimator::Create()
    {
        MeshSkeletonAnimatorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshSkeletonAnimator, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MeshSkeletonAnimator::init()
    {
        m_player = MeshSkeletonAnimatorPlayer::Create();

        return true;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimator::update(F32 _dt)
    {
        m_player->update(_dt);

        // Test
        // m_player->rewindToEnd();


        for (auto& boneTransformsDirty : m_bonesTransformsDirty)
            boneTransformsDirty = true;
            
        for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesGlobalTransforms.size(); i < in; ++i)
            m_bonesGlobalTransforms[i] = calculateBoneGlobalTransform(i);

        for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesSkinningTransforms.size(); i < in; ++i)
        {
            auto& bone = m_skeleton->getBone(i);
            TMat bindPose = bone.inverseBindPoseTransform.inversed();
            m_bonesSkinningTransforms[i] = m_bonesGlobalTransforms[i].transform(bone.inverseBindPoseTransform);
            int a = 0;
        }
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimator::setSkeleton(MeshSkeletonPtr const& _skeleton)
    {
        m_skeleton = _skeleton;

        if (m_skeleton)
        {
            m_bonesGlobalTransforms.resize(m_skeleton->getBonesCount());
            memset(&m_bonesGlobalTransforms[0], 0, sizeof(TMat) * m_bonesGlobalTransforms.size());

            m_bonesSkinningTransforms.resize(m_skeleton->getBonesCount());
            memset(&m_bonesSkinningTransforms[0], 0, sizeof(TMat) * m_bonesSkinningTransforms.size());

            m_bonesTransformsDirty.resize(m_skeleton->getBonesCount());
        }
        else
        {
            m_bonesGlobalTransforms.clear();
            m_bonesSkinningTransforms.clear();
            m_bonesTransformsDirty.clear();
        }
    }

    //////////////////////////////////////////
    bool MeshSkeletonAnimator::playAnimation(HashedCString _name)
    {
        if (!m_skeleton)
            return false;

        MeshSkeletonAnimationPtr const& animation = m_skeleton->getAnimation(_name);
        MAZE_ERROR_RETURN_VALUE_IF(!animation, false, "Undefined animation - %s!", _name.str);

        m_player->play(animation);
        return true;
    }

    //////////////////////////////////////////
    TMat const& MeshSkeletonAnimator::calculateBoneGlobalTransform(MeshSkeleton::BoneIndex _i)
    {
        if (m_bonesTransformsDirty[_i])
        {
            MeshSkeleton::Bone& bone = m_skeleton->getBone(_i);

            TMat boneRotationMatrix = TMat::c_identity;
            Vec3F boneTranslation;
            Quaternion boneRotation;
            Vec3F boneScale;

            m_player->evaluateBoneTransform(
                _i,
                boneTranslation,
                boneRotation,
                boneScale);

            boneRotation.toRotationMatrix(boneRotationMatrix);

            m_bonesGlobalTransforms[_i] = TMat::CreateTranslation(boneTranslation).transform(
                boneRotationMatrix).transform(
                    TMat::CreateScale(boneScale));

            if (bone.parentBoneIndex != -1)
            {
                m_bonesGlobalTransforms[_i] = calculateBoneGlobalTransform(bone.parentBoneIndex).transform(m_bonesGlobalTransforms[_i]);
            }
            else
                m_bonesGlobalTransforms[_i] = m_skeleton->getRootTransform().transform(m_bonesGlobalTransforms[_i]);

            m_bonesTransformsDirty[_i] = false;
        }

        return m_bonesGlobalTransforms[_i];
    };


    //////////////////////////////////////////
    // Class MeshSkeletonAnimatorPlayer
    //
    //////////////////////////////////////////
    MeshSkeletonAnimatorPlayer::MeshSkeletonAnimatorPlayer()
    {
        
    }

    //////////////////////////////////////////
    MeshSkeletonAnimatorPlayerPtr MeshSkeletonAnimatorPlayer::Create()
    {
        MeshSkeletonAnimatorPlayerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshSkeletonAnimatorPlayer, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MeshSkeletonAnimatorPlayer::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::update(F32 _dt)
    {
        if (!m_animation)
            return;

        m_currentTime += _dt;
        if (m_currentTime >= m_animation->getAnimationTime())
            m_currentTime = 0.0f;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::rewindToEnd()
    {
        if (m_animation)
            m_currentTime = m_animation->getAnimationTime();
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::play(MeshSkeletonAnimationPtr const& _animation)
    {
        if (m_animation == _animation)
            return;

        m_animation = _animation;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::evaluateBoneTransform(
        MeshSkeleton::BoneIndex _i,
        Vec3F& _outTranslation,
        Quaternion& _outRotation,
        Vec3F& _outScale)
    {
        if (!m_animation)
            return;

        m_animation->evaluateBoneTransform(
            _i,
            m_currentTime,
            _outTranslation,
            _outRotation,
            _outScale);
    }


} // namespace Maze
//////////////////////////////////////////
