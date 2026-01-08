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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeSkinnedMeshSkeletonGizmos.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/ecs/components/MazeSkinnedMeshRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimator.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SkinnedMeshSkeletonGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SkinnedMeshSkeletonGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SkinnedMeshSkeletonGizmos);


    //////////////////////////////////////////
    SkinnedMeshSkeletonGizmos::SkinnedMeshSkeletonGizmos()
    {

    }

    //////////////////////////////////////////
    SkinnedMeshSkeletonGizmos::~SkinnedMeshSkeletonGizmos()
    {

    }

    //////////////////////////////////////////
    SkinnedMeshSkeletonGizmosPtr SkinnedMeshSkeletonGizmos::Create()
    {
        SkinnedMeshSkeletonGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SkinnedMeshSkeletonGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SkinnedMeshSkeletonGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeletonGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeletonGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        SkinnedMeshSkeleton* meshSkeleton = _component->castRaw<SkinnedMeshSkeleton>();
        MeshSkeletonAnimatorPtr const& animator = meshSkeleton->getAnimator();
        if (!animator)
            return;

        MeshSkeletonPtr const& skeleton = animator->getSkeleton();
        if (!skeleton)
            return;


        Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
        if (transform3D)
        {
            Vector<TMat> const& bonesGlobalTransform = animator->getBonesGlobalTransforms();

            for (MeshSkeleton::BoneIndex i = 0, in = (MeshSkeleton::BoneIndex)bonesGlobalTransform.size(); i < in; ++i)
            {
                MeshSkeleton::Bone& bone = skeleton->getBone(i);

                TMat const& boneGlobalTransform = bonesGlobalTransform[i];
                TMat boneWorldTm = transform3D->getWorldTransform().transform(
                    /*skeleton->getRootTransform().transform*/(boneGlobalTransform));
                
                F32 boneLen = 1.0f;

                if (bone.parentBoneIndex != -1)
                {
                    TMat parentBoneWorldTm = transform3D->getWorldTransform().transform(
                        /*skeleton->getRootTransform().transform*/(bonesGlobalTransform[bone.parentBoneIndex]));

                    Vec3F toChild = boneWorldTm[3] - parentBoneWorldTm[3];
                    F32 toChildLen = toChild.length();
                    if (toChildLen > 1e-3)
                    {
                        boneLen = toChildLen;
                        Vec3F toChildDir = toChild / toChildLen;

                        _drawer->drawWireCone(
                            parentBoneWorldTm[3],
                            toChildDir,
                            boneLen * 0.05f,
                            boneLen,
                            ColorF128(0.0f, 1.0f, 0.0f, 0.25f),
                            GizmosDrawer::GizmosMode::Debug,
                            GizmosDrawer::MeshRenderMode::TransparentTop);
                    }
                    
                    /*
                    if (!skeleton->isBoneHaveChildren(i))
                    {
                        boneLen = boneWorldTm.getUp().length();

                        _drawer->drawWireCone(
                            boneWorldTm.getTranslation(),
                            boneWorldTm.getForward(),
                            boneLen * 0.05f,
                            boneLen,
                            ColorF128(0.0f, 1.0f, 0.0f, 0.25f),
                            GizmosDrawer::GizmosMode::Debug,
                            GizmosDrawer::MeshRenderMode::TransparentTop);
                    }
                    */
                }

                _drawer->drawWireSphere(
                    boneWorldTm.getTranslation(),
                    boneLen * (bone.parentBoneIndex != -1 ? 0.1f : 0.15f),
                    bone.parentBoneIndex != -1 ? ColorF128(0.0f, 1.0f, 0.0f, 0.5f) : ColorF128(1.0f, 0.66f, 0.0f, 1.0f),
                    0.0f,
                    GizmosDrawer::GizmosMode::Debug,
                    GizmosDrawer::MeshRenderMode::TransparentTop);


                // Bind pose
                if (true)
                {
                    TMat boneBindPoseTransformMS = bone.inversedBindPoseTransformMS.inversed();
                    TMat boneBindPoseTransformWS = transform3D->getWorldTransform().transform(boneBindPoseTransformMS);
                    _drawer->drawWireSphere(
                        boneBindPoseTransformWS.getTranslation(),
                        boneLen * 0.2f,
                        ColorF128(1.0f, 0.0f, 0.0f, 0.5f),
                        0.0f,
                        GizmosDrawer::GizmosMode::Debug,
                        GizmosDrawer::MeshRenderMode::TransparentTop);
                }
            }
        }

    }

} // namespace Maze
//////////////////////////////////////////
