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

            if (player->getAnimation() && !player->getAdditive())
                totalWeight += player->getTotalWeight();
        }

        if (totalWeight > 0.0f)
        {
            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            {
                if (m_players[i]->getAdditive())
                    m_playersBlendWeights[i] = m_players[i]->getTotalWeight();
                else
                    m_playersBlendWeights[i] = m_players[i]->getTotalWeight() / totalWeight;
            }

            for (auto& boneTransformsDirty : m_bonesTransformsDirty)
                boneTransformsDirty = true;

            for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesGlobalTransforms.size(); i < in; ++i)
            {
                auto& bone = m_skeleton->getBone(i);

                calculateBoneTransforms(i);
                
                // Calculate animation delta
                m_bonesSkinningTransforms[i] =
                    (m_skeleton->getRootTransform().transform(m_bonesGlobalTransforms[i])).transform(bone.inversedBindPoseTransformMS);
            }
        }
        else
        {
            // Reset to Bind pose
            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            {
                if (m_players[i]->getAdditive())
                    m_playersBlendWeights[i] = m_players[i]->getTotalWeight();
                else
                    m_playersBlendWeights[i] = 0.0f;
            }

            for (auto& boneTransformsDirty : m_bonesTransformsDirty)
                boneTransformsDirty = false;

            for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)m_bonesGlobalTransforms.size(); i < in; ++i)
            {
                m_bonesGlobalTransforms[i] = m_skeleton->getBone(i).inversedBindPoseTransformMS.inversed();
                m_bonesSkinningTransforms[i] = TMat::c_identity;
            }
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
    S32 MeshSkeletonAnimator::playAnimation(
        HashedCString _name,
        MeshSkeletonAnimationStartParams _startParams)
    {
        if (!m_skeleton)
            return false;

        MeshSkeletonAnimationPtr const& animation = m_skeleton->getAnimation(_name);
        MAZE_WARNING_RETURN_VALUE_IF(!animation, -1, "Undefined animation - %s!", _name.str);

        F32 blendWeightSpeed = _startParams.blendTime > 0.0f ? 1.0f / _startParams.blendTime : F32_VERY_BIG_NUMBER;

        bool important = _startParams.flags & U8(MeshSkeletonAnimationStartFlags::Important);

        S32 playerIndex = findPlayerIndexForNewAnimation(animation, important);
        if (playerIndex < 0)
            return playerIndex;

        MeshSkeletonAnimatorPlayerPtr const& player = m_players[playerIndex];

        bool looped = _startParams.flags & U8(MeshSkeletonAnimationStartFlags::Looped);
        bool additive = _startParams.flags & U8(MeshSkeletonAnimationStartFlags::Additive);
        bool pauseEnding = _startParams.flags & U8(MeshSkeletonAnimationStartFlags::PauseEnding);
        bool stopCurrentAnimations = _startParams.flags & U8(MeshSkeletonAnimationStartFlags::StopCurrentAnimations);

        player->play(animation, looped, additive, pauseEnding);
        player->setWeightMult(_startParams.weight);
        player->setWeightSpeed(blendWeightSpeed);
        player->setSpeedMult(_startParams.speed);

        if (stopCurrentAnimations)
        {
            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            {
                if (m_players[i] != player && m_players[i]->isActive() && !m_players[i]->getAdditive())
                    m_players[i]->stop();
            }
        }

        return playerIndex;
    }

    //////////////////////////////////////////
    MeshSkeletonAnimatorPlayerPtr const& MeshSkeletonAnimator::getCurrentAnimation()
    {
        static MeshSkeletonAnimatorPlayerPtr nullPointer;

        S32 playerIndex = 0;
        F32 bestWeight = m_players[playerIndex]->getTotalWeight();
        if (m_players[playerIndex]->isActive() && bestWeight == 1.0f)
            return m_players[playerIndex];

        // Find best active player
        for (S32 i = 1; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
        {
            if ((!m_players[playerIndex]->isActive() && m_players[i]->isActive()) ||
                m_players[i]->getTotalWeight() > bestWeight)
            {
                playerIndex = i;
                bestWeight = m_players[i]->getTotalWeight();

                if (m_players[playerIndex]->isActive() && bestWeight == 1.0f)
                    return m_players[playerIndex];
            }
        }

        if (!m_players[playerIndex]->isActive())
            return nullPointer;

        return m_players[playerIndex];
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimator::calculateBoneTransforms(MeshSkeleton::BoneIndex _i)
    {
        if (m_bonesTransformsDirty[_i])
        {
            MeshSkeleton::Bone& bone = m_skeleton->getBone(_i);

            // Base layer
            Vec3F boneTranslationFinal = Vec3F::c_zero;
            Quaternion boneRotationFinal = Quaternion::c_identity;
            Vec3F boneScaleFinal = Vec3F::c_zero;
            bool firstRotation = true;
            F32 accumulatedRotationWeight = 0.0f;

            // Additive layer
            //Vec3F additiveBoneTranslationFinal = Vec3F::c_zero;
            //Quaternion additiveBoneRotationFinal = Quaternion::c_identity;
            //Vec3F additiveBoneScaleFinal = Vec3F::c_one;
            //bool additiveFirstRotation = true;
            //F32 additiveAccumulatedRotationWeight = 0.0f;


            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            {
                F32 weight = m_playersBlendWeights[i];
                if (weight == 0.0f)
                    continue;

                Vec3F boneTranslation;
                Quaternion boneRotation(0.0f, 0.0f, 0.0f, 0.0f);
                Vec3F boneScale;

                m_players[i]->evaluateBoneTransform(
                    _i,
                    boneTranslation,
                    boneRotation,
                    boneScale);


                if (!m_players[i]->getAdditive())
                {
                    boneTranslationFinal += boneTranslation * weight;
                    boneScaleFinal += boneScale * weight;

                    if (firstRotation)
                    {
                        boneRotationFinal = boneRotation;
                        firstRotation = false;
                        accumulatedRotationWeight = weight;
                    }
                    else
                    {
                        F32 t = weight / (weight + accumulatedRotationWeight);
                        boneRotationFinal = Quaternion::Slerp(t, boneRotationFinal, boneRotation);
                        accumulatedRotationWeight += weight;
                    }
                }
                else
                {
                    // #TODO:
                    /*
                    additiveBoneTranslationFinal += (boneTranslation + bone.inversedBindPoseTransformMS[3]) * weight;
                    // additiveBoneScaleFinal += boneScale * weight;

                    if (additiveFirstRotation)
                    {
                        additiveBoneRotationFinal = boneRotation;
                        additiveFirstRotation = false;
                        additiveAccumulatedRotationWeight = weight;
                    }
                    else
                    {
                        F32 t = weight / (weight + additiveAccumulatedRotationWeight);
                        additiveBoneRotationFinal = Quaternion::Slerp(t, additiveBoneRotationFinal, boneRotation);
                        additiveAccumulatedRotationWeight += weight;
                    }
                    */
                }
            }


            TMat boneRotationMatrix;

            //additiveBoneRotationFinal.toRotationMatrix(boneRotationMatrix);
            //TMat additiveMat = TMat::CreateTranslation(additiveBoneTranslationFinal).transform(
            //    boneRotationMatrix).transform(
            //        TMat::CreateScale(additiveBoneScaleFinal));

            (boneRotationFinal).toRotationMatrix(boneRotationMatrix);
            m_bonesGlobalTransforms[_i] = TMat::CreateTranslation(boneTranslationFinal).transform(
                boneRotationMatrix).transform(
                    TMat::CreateScale(boneScaleFinal));
            

            // Apply parent transform
            if (bone.parentBoneIndex != -1)
            {
                calculateBoneTransforms(bone.parentBoneIndex);
                m_bonesGlobalTransforms[_i] = m_bonesGlobalTransforms[bone.parentBoneIndex].transform(m_bonesGlobalTransforms[_i]);
            }

            m_bonesTransformsDirty[_i] = false;
        }
    };

    //////////////////////////////////////////
    S32 MeshSkeletonAnimator::findPlayerIndexForNewAnimation(
        MeshSkeletonAnimationPtr const& _animation,
        bool _important)
    {
        if (_important)
        {
            S32 playerIndex = 0;
            F32 bestWeight = m_players[playerIndex]->getTotalWeight();
            if (_animation == m_players[playerIndex]->getAnimation() || (!m_players[playerIndex]->isActive() && bestWeight == 0.0f))
                return playerIndex;

            // Find best inactive player or player with lowest current weight
            for (S32 i = 1; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
            {
                if (_animation == m_players[i]->getAnimation() ||
                    (m_players[playerIndex]->isActive() && !m_players[i]->isActive()) ||
                    m_players[i]->getTotalWeight() < bestWeight)
                    {
                        playerIndex = i;
                        bestWeight = m_players[i]->getTotalWeight();

                        if (!m_players[playerIndex]->isActive() && bestWeight == 0.0f)
                            return playerIndex;
                    }
            }

            return playerIndex;
        }
        else
        {
            for (S32 i = 0; i < MESH_SKELETON_ANIMATOR_PLAYERS_COUNT; ++i)
                if (_animation == m_players[i]->getAnimation() || !m_players[i]->isActive())
                    return i;

            return -1;
        }
    }

    //////////////////////////////////////////
    MeshSkeletonAnimatorPlayerPtr const& MeshSkeletonAnimator::findPlayerForNewAnimation(
        MeshSkeletonAnimationPtr const& _animation,
        bool _important)
    {
        static MeshSkeletonAnimatorPlayerPtr const nullPtr;

        S32 playerIndex = findPlayerIndexForNewAnimation(_animation, _important);
        if (playerIndex >= 0)
            return m_players[playerIndex];

        return nullPtr;
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

        _dt *= m_speedMult;

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

        if (m_state != State::None && m_state != State::Pause)
        {
            m_currentTime += _dt;
            if (m_currentTime >= m_animation->getAnimationTime())
            {
                if (m_looped)
                    m_currentTime = m_currentTime - m_animation->getAnimationTime();
                else
                {
                    m_currentTime = m_animation->getAnimationTime();

                    if (m_pauseEnding)
                        pause();
                    else
                        stop();
                }
            }
        }
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::rewindToEnd()
    {
        rewindTo(m_animation->getAnimationTime());
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::rewindToRandom()
    {
        rewindTo(Random::UnitRandom() * m_animation->getAnimationTime());
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::rewindTo(F32 _time)
    {
        if (m_animation)
            m_currentTime = _time;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::play(
        MeshSkeletonAnimationPtr const& _animation,
        bool _loop,
        bool _additive,
        bool _pauseEnding)
    {
        if (m_state != State::Active)
            setState(State::In);

        if (m_animation != _animation)
            m_currentTime = 0.0f;

        m_animation = _animation;
        m_looped = _loop;
        m_additive = _additive;
        m_pauseEnding = _pauseEnding;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimatorPlayer::pause()
    {
        if (m_state != State::None)
            setState(State::Pause);
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
