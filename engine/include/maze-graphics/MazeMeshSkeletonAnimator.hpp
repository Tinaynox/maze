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
        bool playAnimation(HashedCString _name);

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayerPtr const& getCurrentAnimation();


        //////////////////////////////////////////
        inline void setAnimationSpeed(F32 _animationSpeed) { m_animationSpeed = _animationSpeed; }

        //////////////////////////////////////////
        inline F32 getAnimationSpeed() const { return m_animationSpeed; }

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimator();

        //////////////////////////////////////////
        bool init();    
    
        //////////////////////////////////////////
        TMat const& calculateBoneGlobalTransform(MeshSkeleton::BoneIndex _i);

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayerPtr const& findPlayerForNewAnimation(
            MeshSkeletonAnimationPtr const& _animation);

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayerPtr const& findPlayerWith();

    protected:
        MeshSkeletonPtr m_skeleton;
        F32 m_animationSpeed = 1.0f;

        Vector<TMat> m_bonesGlobalTransforms;
        Vector<TMat> m_bonesSkinningTransforms;
        FastVector<bool> m_bonesTransformsDirty;

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
        void play(MeshSkeletonAnimationPtr const& _animation);

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
        inline F32 getCurrentWeight() const { return m_currentWeight; }

        //////////////////////////////////////////
        inline void setCurrentWeight(F32 _value) { m_currentWeight = _value; }


        //////////////////////////////////////////
        inline bool isActive() const { return m_state != State::None && m_state != State::Out; }

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

        F32 m_currentWeight = 0.0f;
        F32 m_weightSpeed = 5.0f;
        State m_state = State::None;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeletonAnimator_hpp_
//////////////////////////////////////////
