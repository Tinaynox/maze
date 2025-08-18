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
#if (!defined(_MazeMeshSkeletonAnimator_hpp_))
#define _MazeMeshSkeletonAnimator_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MeshSkeletonAnimator);
    MAZE_USING_SHARED_PTR(MeshSkeletonAnimatorPlayer);
    MAZE_USING_SHARED_PTR(MeshSkeleton);
    MAZE_USING_SHARED_PTR(MeshSkeletonAnimation);


    //////////////////////////////////////////
    #define MESH_SKELETON_ANIMATOR_PLAYERS_COUNT (3)
    

    //////////////////////////////////////////
    enum class MAZE_GRAPHICS_API MeshSkeletonAnimationStartFlags : U8
    {
        None = 0,

        Looped                      = MAZE_BIT(0),
        Additive                    = MAZE_BIT(1),
        StopCurrentAnimations       = MAZE_BIT(2),
        Important                   = MAZE_BIT(3)
    };

    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API MeshSkeletonAnimationStartParams
    {
        F32 blendTime = 0.2f;
        F32 weight = 1.0f;
        F32 speed = 1.0f;
        U8 flags = U8(MeshSkeletonAnimationStartFlags::Looped) |
                   U8(MeshSkeletonAnimationStartFlags::StopCurrentAnimations) |
                   U8(MeshSkeletonAnimationStartFlags::Important);
    };


    //////////////////////////////////////////
    // Class MeshSkeletonAnimator
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonAnimator
    {
    public:

    public:

        //////////////////////////////////////////
        static MeshSkeletonAnimatorPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        void setSkeleton(MeshSkeletonPtr const& _skeleton);

        //////////////////////////////////////////
        inline MeshSkeletonPtr const& getSkeleton() const { return m_skeleton; }


        //////////////////////////////////////////
        inline Vector<TMat> const& getBonesGlobalTransforms() const { return m_bonesGlobalTransforms; }

        //////////////////////////////////////////
        inline Vector<TMat> const& getBonesSkinningTransforms() const { return m_bonesSkinningTransforms; }

        //////////////////////////////////////////
        inline Size getBonesCount() const { return m_bonesSkinningTransforms.size(); }


        //////////////////////////////////////////
        S32 playAnimation(
            HashedCString _name,
            MeshSkeletonAnimationStartParams _startParams = MeshSkeletonAnimationStartParams());

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayerPtr const& getCurrentAnimation();


        //////////////////////////////////////////
        inline void setAnimationSpeed(F32 _animationSpeed) { m_animationSpeed = _animationSpeed; }

        //////////////////////////////////////////
        inline F32 getAnimationSpeed() const { return m_animationSpeed; }


        //////////////////////////////////////////
        inline MeshSkeletonAnimatorPlayerPtr const& getPlayer(Size _i) const { return m_players[_i]; }

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimator();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void calculateBoneTransforms(MeshSkeleton::BoneIndex _i);

        //////////////////////////////////////////
        S32 findPlayerIndexForNewAnimation(
            MeshSkeletonAnimationPtr const& _animation,
            bool _important);

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayerPtr const& findPlayerForNewAnimation(
            MeshSkeletonAnimationPtr const& _animation,
            bool _important = true);

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayerPtr const& findPlayerWith();

    protected:
        MeshSkeletonPtr m_skeleton;
        F32 m_animationSpeed = 1.0f;

        Vector<TMat> m_bonesGlobalTransforms; // Mesh space
        FastVector<bool> m_bonesTransformsDirty;

        Vector<TMat> m_bonesSkinningTransforms; // Animation delta, Mesh space

        MeshSkeletonAnimatorPlayerPtr m_players[MESH_SKELETON_ANIMATOR_PLAYERS_COUNT];
        F32 m_playersBlendWeights[MESH_SKELETON_ANIMATOR_PLAYERS_COUNT];
    };


    //////////////////////////////////////////
    // Class MeshSkeletonAnimatorPlayer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonAnimatorPlayer
    {
    public:

        //////////////////////////////////////////
        enum class State
        {
            None = 0,
            In,
            Active,
            Out
        };


    public:

        //////////////////////////////////////////
        static MeshSkeletonAnimatorPlayerPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void rewindToEnd();

        //////////////////////////////////////////
        void rewindToRandom();

        //////////////////////////////////////////
        void rewindTo(F32 _time);


        //////////////////////////////////////////
        inline bool getAdditive() const { return m_additive; }


        //////////////////////////////////////////
        void play(
            MeshSkeletonAnimationPtr const& _animation,
            bool _loop = true,
            bool _additive = false);

        //////////////////////////////////////////
        void stop();

        //////////////////////////////////////////
        inline MeshSkeletonAnimationPtr const& getAnimation() const { return m_animation; }

        //////////////////////////////////////////
        void evaluateBoneTransform(
            MeshSkeleton::BoneIndex _i,
            Vec3F& _outTranslation,
            Quaternion& _outRotation,
            Vec3F& _outScale);


        //////////////////////////////////////////
        inline F32 getSpeedMult() const { return m_speedMult; }

        //////////////////////////////////////////
        inline void setSpeedMult(F32 _value) { m_speedMult = _value; }


        //////////////////////////////////////////
        inline F32 getWeightMult() const { return m_weightMult; }

        //////////////////////////////////////////
        inline void setWeightMult(F32 _value) { m_weightMult = _value; }


        //////////////////////////////////////////
        inline F32 getCurrentWeight() const { return m_currentWeight; }

        //////////////////////////////////////////
        inline void setCurrentWeight(F32 _value) { m_currentWeight = _value; }


        //////////////////////////////////////////
        inline F32 getTotalWeight() const { return m_currentWeight * m_weightMult; }


        //////////////////////////////////////////
        inline F32 getWeightSpeed() const { return m_weightSpeed; }

        //////////////////////////////////////////
        inline void setWeightSpeed(F32 _value) { m_weightSpeed = _value; }


        //////////////////////////////////////////
        inline bool isActive() const { return m_state != State::None && m_state != State::Out; }


        //////////////////////////////////////////
        inline F32 getCurrentTime() const { return m_currentTime; }

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayer();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void setState(State _state);

    protected:
        MeshSkeletonAnimationPtr m_animation;
        F32 m_currentTime = 0.0f;
        
        bool m_looped = true;
        bool m_additive = false;

        F32 m_speedMult = 1.0f;

        F32 m_weightMult = 1.0f;
        F32 m_currentWeight = 0.0f;
        F32 m_weightSpeed = 5.0f;
        State m_state = State::None;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeletonAnimator_hpp_
//////////////////////////////////////////
