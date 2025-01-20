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
        for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            m_players[i] = MeshSkeletonAnimatorPlayer::Create();

        return true;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimator::update(F32 _dt)
    {
        _dt *= m_animationSpeed;

        F32 totalWeight = 0.0f;
        for (MeshSkeletonAnimatorPlayerPtr const& player : m_players)
        {
            player->update(_dt);

            if (player->getAnimation())
                totalWeight += player->getCurrentWeight();
        }

        if (totalWeight > 0.0f)
        {
            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
                m_playersBlendWeights[i] = m_players[i]->getCurrentWeight() / totalWeight;

            for (auto& boneTransformsDirty : m_bonesTransformsDirty)
                boneTransformsDirty = true;

            for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesGlobalTransforms.size(); i < in; ++i)
                m_bonesGlobalTransforms[i] = calculateBoneGlobalTransform(i);
        }
        else
        {
            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
                m_playersBlendWeights[i] = 0.0f;

            for (auto& boneTransformsDirty : m_bonesTransformsDirty)
                boneTransformsDirty = false;

            for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesGlobalTransforms.size(); i < in; ++i)
                m_bonesGlobalTransforms[i] = m_skeleton->getBone(i).inverseBindPoseTransform.inversed();
        }
       

        for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesSkinningTransforms.size(); i < in; ++i)
        {
            auto& bone = m_skeleton->getBone(i);
            m_bonesSkinningTransforms[i] = m_bonesGlobalTransforms[i].transform(bone.inverseBindPoseTransform);
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

        MeshSkeletonAnimatorPlayerPtr const& player = findPlayerForNewAnimation(animation);
        player->play(animation);

        for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            if (m_players[i] != player && m_players[i]->isActive())
                m_players[i]->stop();

        return true;
    }

    //////////////////////////////////////////
    MeshSkeletonAnimatorPlayerPtr const& MeshSkeletonAnimator::getCurrentAnimation()
    {
        static MeshSkeletonAnimatorPlayerPtr nullPointer;

        S32 playerIndex = 0;
        F32 bestWeight = m_players[playerIndex]->getCurrentWeight();
        if (m_players[playerIndex]->isActive() && bestWeight == 1.0f)
            return m_players[playerIndex];

        // Find best active player
        for (S32 i = 1; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
        {
            if ((!m_players[playerIndex]->isActive() && m_players[i]->isActive()) ||
                m_players[i]->getCurrentWeight() > bestWeight)
            {
                playerIndex = i;
                bestWeight = m_players[i]->getCurrentWeight();

                if (m_players[playerIndex]->isActive() && bestWeight == 1.0f)
                    return m_players[playerIndex];
            }
        }

        if (!m_players[playerIndex]->isActive())
            return nullPointer;

        return m_players[playerIndex];
    }

    //////////////////////////////////////////
    TMat const& MeshSkeletonAnimator::calculateBoneGlobalTransform(MeshSkeleton::BoneIndex _i)
    {
        if (m_bonesTransformsDirty[_i])
        {
            MeshSkeleton::Bone& bone = m_skeleton->getBone(_i);

            TMat boneRotationMatrix = TMat::c_identity;

            
            Vec3F boneTranslationFinal = Vec3F::c_zero;
            Quaternion boneRotationFinal(0.0f, 0.0f, 0.0f, 0.0f);
            Vec3F boneScaleFinal = Vec3F::c_zero;

            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            {
                if (m_players[i]->getCurrentWeight() == 0.0f)
                    continue;

                Vec3F boneTranslation;
                Quaternion boneRotation(0.0f, 0.0f, 0.0f, 0.0f);
                Vec3F boneScale;

                m_players[i]->evaluateBoneTransform(
                    _i,
                    boneTranslation,
                    boneRotation,
                    boneScale);

                boneTranslationFinal += boneTranslation * m_playersBlendWeights[i];
                boneRotationFinal = boneRotationFinal + boneRotation * m_playersBlendWeights[i];
                boneScaleFinal += boneScale * m_playersBlendWeights[i];
            }

            boneRotationFinal.toRotationMatrix(boneRotationMatrix);

            m_bonesGlobalTransforms[_i] = TMat::CreateTranslation(boneTranslationFinal).transform(
                boneRotationMatrix).transform(
                    TMat::CreateScale(boneScaleFinal));

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
    MeshSkeletonAnimatorPlayerPtr const& MeshSkeletonAnimator::findPlayerForNewAnimation(
        MeshSkeletonAnimationPtr const& _animation)
    {
        S32 playerIndex = 0;
        F32 bestWeight = m_players[playerIndex]->getCurrentWeight();
        if (_animation == m_players[playerIndex]->getAnimation() || (!m_players[playerIndex]->isActive() && bestWeight == 0.0f))
            return m_players[playerIndex];

        // Find best inactive player or player with lowest current weight
        for (S32 i = 1; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
        {
            if (_animation == m_players[playerIndex]->getAnimation() ||
                (m_players[playerIndex]->isActive() && !m_players[i]->isActive()) ||
                m_players[i]->getCurrentWeight() < bestWeight)
            {
                playerIndex = i;
                bestWeight = m_players[i]->getCurrentWeight();

                if (!m_players[playerIndex]->isActive() && bestWeight == 0.0f)
                    return m_players[playerIndex];
            }
        }

        return m_players[playerIndex];
    }



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

        switch (m_state)
        {
            case State::In:
            {
                m_currentWeight += m_weightSpeed * _dt;
                if (m_currentWeight >= 1.0f)
                {
                    m_currentWeight = 1.0f;
                    setState(State::Active);
                }

                break;
            }
            case State::Out:
            {
                m_currentWeight -= m_weightSpeed * _dt;
                if (m_currentWeight <= 0.0f)
                {
                    m_currentWeight = 0.0f;
                    setState(State::None);
                }

                break;
            }
            default:
                break;
        }

        if (m_state != State::None)
        {
            m_currentTime += _dt;
            if (m_currentTime >= m_animation->getAnimationTime())
            {
                if (m_looped)
                    m_currentTime = 0.0f;
                else
                    m_currentTime = m_animation->getAnimationTime();
            }
        }
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::rewindToEnd()
    {
        if (m_animation)
            m_currentTime = m_animation->getAnimationTime();
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::rewindToRandom()
    {
        if (m_animation)
            m_currentTime = Random::UnitRandom() * m_animation->getAnimationTime();
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::play(MeshSkeletonAnimationPtr const& _animation)
    {
        if (m_state != State::Active)
            setState(State::In);

        if (m_animation != _animation)
            m_currentTime = 0.0f;

        m_animation = _animation;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::stop()
    {
        if (m_state != State::None)
            setState(State::Out);
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

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::setState(State _state)
    {
        if (m_state == _state)
            return;

        m_state = _state;

        switch (m_state)
        {
            case State::None:
            {
                m_currentWeight = 0.0f;
                m_animation.reset();
                break;
            }

            default:
                break;
        }
    }



} // namespace Maze
//////////////////////////////////////////
