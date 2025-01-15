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
#if (!defined(_MazeMeshSkeleton_hpp_))
#define _MazeMeshSkeleton_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
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
    MAZE_USING_SHARED_PTR(MeshSkeleton);
    MAZE_USING_SHARED_PTR(MeshSkeletonAnimation);
    

    //////////////////////////////////////////
    // Class MeshSkeleton
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API MeshSkeleton
        : public SharedObject<MeshSkeleton>
    {
    public:

        //////////////////////////////////////////
        using BoneIndex = S32;

        //////////////////////////////////////////
        struct Bone
        {
            //////////////////////////////////////////
            Bone(
                String const& _name = String(),
                TMat const& _transform = TMat::c_identity,
                BoneIndex _parentBoneIndex = -1)
                : name(_name)
                , transform(_transform)
                , parentBoneIndex(_parentBoneIndex)
            {}

            String name;
            TMat transform;
            BoneIndex parentBoneIndex = -1;
        };

    public:

        //////////////////////////////////////////
        static MeshSkeletonPtr Create();

        //////////////////////////////////////////
        virtual ~MeshSkeleton();


        //////////////////////////////////////////
        void clear();


        //////////////////////////////////////////
        BoneIndex addBone(
            String const& _name,
            TMat const& _transform = TMat::c_identity,
            BoneIndex _parentBoneIndex = -1);

        //////////////////////////////////////////
        Bone& getBone(BoneIndex _index) { return m_bones[_index]; }

        //////////////////////////////////////////
        BoneIndex findBoneIndex(HashedCString _name);

        //////////////////////////////////////////
        Bone* findBone(HashedCString _name);

        //////////////////////////////////////////
        BoneIndex ensureBoneIndex(HashedCString _name);

        //////////////////////////////////////////
        Bone& ensureBone(HashedCString _name);


        //////////////////////////////////////////
        MeshSkeletonAnimationPtr const& ensureAnimation(String const& _name);

    protected:

        //////////////////////////////////////////
        MeshSkeleton();

        //////////////////////////////////////////
        virtual bool init();    
    

    protected:
        FastVector<Bone> m_bones;
        StringKeyMap<BoneIndex> m_boneIndicesByName;

        StringKeyMap<MeshSkeletonAnimationPtr> m_animations;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMeshSkeleton_hpp_
//////////////////////////////////////////
