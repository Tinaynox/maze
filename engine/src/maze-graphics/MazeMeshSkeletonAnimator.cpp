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


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class MeshSkeletonAnimator
    //
    //////////////////////////////////////////
    MeshSkeletonAnimator::MeshSkeletonAnimator()
    {
        memset(m_bonesTRS, 0, sizeof(BoneTRS) * MAZE_SKELETON_BONES_MAX);
    }

    //////////////////////////////////////////
    MeshSkeletonAnimator::~MeshSkeletonAnimator()
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
        return true;
    }

    //////////////////////////////////////////
    void MeshSkeletonAnimator::setSkeleton(MeshSkeletonPtr const& _skeleton)
    {
        m_skeleton = _skeleton;

        for (S32 i = 0; i < MAZE_SKELETON_BONES_MAX; ++i)
        {
            m_bonesTRS[i].translation.y = 2.0f;
            m_bonesTRS[i].scale = Vec3F(1.0f);
        }
    }


} // namespace Maze
//////////////////////////////////////////
