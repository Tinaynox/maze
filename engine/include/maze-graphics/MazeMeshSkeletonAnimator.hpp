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
        inline TMat const* getBonesGlobalTransforms() const { return &m_bonesGlobalTransforms[0]; }

        //////////////////////////////////////////
        inline TMat const* getBonesSkinningTransforms() const { return &m_bonesSkinningTransforms[0]; }

        //////////////////////////////////////////
        inline Size getBonesCount() const { return m_bonesSkinningTransforms.size(); }


        //////////////////////////////////////////
        bool playAnimation(HashedCString _name);

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimator();

        //////////////////////////////////////////
        bool init();    
    
        //////////////////////////////////////////
        TMat const& calculateBoneGlobalTransform(MeshSkeleton::BoneIndex _i);

    protected:
        MeshSkeletonPtr m_skeleton;

        Vector<TMat> m_bonesGlobalTransforms;
        Vector<TMat> m_bonesSkinningTransforms;
        Vector<bool> m_bonesTransformsDirty;

        MeshSkeletonAnimatorPlayerPtr m_player;
    };


    //////////////////////////////////////////
    // Class MeshSkeletonAnimatorPlayer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonAnimatorPlayer
    {
    public:

        //////////////////////////////////////////
        static MeshSkeletonAnimatorPlayerPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void rewindToEnd();


        //////////////////////////////////////////
        void play(MeshSkeletonAnimationPtr const& _animation);

        //////////////////////////////////////////
        void evaluateBoneTransform(
            MeshSkeleton::BoneIndex _i,
            Vec3F& _outTranslation,
            Quaternion& _outRotation,
            Vec3F& _outScale);

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimatorPlayer();

        //////////////////////////////////////////
        bool init();


    protected:
        MeshSkeletonAnimationPtr m_animation;
        F32 m_currentTime = 0.0f;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeletonAnimator_hpp_
//////////////////////////////////////////
