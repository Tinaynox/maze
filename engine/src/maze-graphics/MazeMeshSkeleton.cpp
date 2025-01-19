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
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimation.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class MeshSkeleton
    //
    //////////////////////////////////////////
    MeshSkeleton::MeshSkeleton()
    {
    }

    //////////////////////////////////////////
    MeshSkeleton::~MeshSkeleton()
    {
    }

    //////////////////////////////////////////
    MeshSkeletonPtr MeshSkeleton::Create()
    {
        MeshSkeletonPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshSkeleton, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MeshSkeleton::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void MeshSkeleton::clear()
    {
        m_bones.clear();
    }

    //////////////////////////////////////////
    MeshSkeleton::BoneIndex MeshSkeleton::addBone(
        String const& _name,
        TMat const& _inverseBindPoseTransform,
        BoneIndex _parentBoneIndex)
    {
        BoneIndex index = (BoneIndex)m_bones.size();
        m_bones.emplace_back(
            _name,
            _inverseBindPoseTransform,
            _parentBoneIndex);
        m_boneIndicesByName.insert(_name, index);
        return index;
    }

    //////////////////////////////////////////
    MeshSkeleton::BoneIndex MeshSkeleton::findBoneIndex(HashedCString _name)
    {
        auto it = m_boneIndicesByName.find(_name);
        if (it == m_boneIndicesByName.end())
            return -1;

        return it->second;
    }

    //////////////////////////////////////////
    MeshSkeleton::Bone* MeshSkeleton::findBone(HashedCString _name)
    {
        BoneIndex boneIndex = findBoneIndex(_name);
        if (boneIndex < 0)
            return nullptr;

        return &m_bones[boneIndex];
    }

    //////////////////////////////////////////
    MeshSkeleton::BoneIndex MeshSkeleton::ensureBoneIndex(HashedCString _name)
    {
        BoneIndex boneIndex = findBoneIndex(_name);
        if (boneIndex >= 0)
            return boneIndex;

        return addBone(_name.str);
    }

    //////////////////////////////////////////
    MeshSkeleton::Bone& MeshSkeleton::ensureBone(HashedCString _name)
    {
        MeshSkeleton::Bone* bone = findBone(_name);
        if (bone)
            return *bone;

        BoneIndex boneIndex = addBone(_name.str);
        return getBone(boneIndex);
    }

    //////////////////////////////////////////
    MeshSkeletonAnimationPtr const& MeshSkeleton::ensureAnimation(String const& _name)
    {
        auto it = m_animations.find(_name);
        if (it == m_animations.end())
        {
            MeshSkeletonAnimationPtr animation = MeshSkeletonAnimation::Create();
            animation->setName(_name);
            return *m_animations.insert(_name, animation);
        }

        return it->second;
    }

    //////////////////////////////////////////
    MeshSkeletonAnimationPtr const& MeshSkeleton::getAnimation(HashedCString _name)
    {
        static MeshSkeletonAnimationPtr nullPointer;

        auto it = m_animations.find(_name);
        if (it == m_animations.end())
            return nullPointer;

        return it->second;
    }

} // namespace Maze
//////////////////////////////////////////
