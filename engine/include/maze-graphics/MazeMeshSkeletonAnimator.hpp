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
    MAZE_USING_SHARED_PTR(MeshSkeleton);
    

    //////////////////////////////////////////
    // Class MeshSkeletonAnimator
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeletonAnimator
    {
    public:

        //////////////////////////////////////////
        struct BoneTRS
        {
            Vec3F translation;
            Vec3F rotation;
            Vec3F scale;
        };

    public:

        //////////////////////////////////////////
        static MeshSkeletonAnimatorPtr Create();

        //////////////////////////////////////////
        virtual ~MeshSkeletonAnimator();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        void setSkeleton(MeshSkeletonPtr const& _skeleton);

        //////////////////////////////////////////
        inline MeshSkeletonPtr const& getSkeleton() const { return m_skeleton; }


        //////////////////////////////////////////
        inline BoneTRS const* getBonesTRS() const { return &m_bonesTRS[0]; }

        //////////////////////////////////////////
        inline Size getBonesCount() const { return m_bonesCount; }

    protected:

        //////////////////////////////////////////
        MeshSkeletonAnimator();

        //////////////////////////////////////////
        virtual bool init();    
    

    protected:
        MeshSkeletonPtr m_skeleton;

        BoneTRS m_bonesTRS[MAZE_SKELETON_BONES_MAX];
        Size m_bonesCount = 0u;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeletonAnimator_hpp_
//////////////////////////////////////////
