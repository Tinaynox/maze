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
#include "maze-graphics/ecs/components/MazeSkinnedMeshAttach3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimator.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeRenderCommands.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SkinnedMeshAttach3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SkinnedMeshAttach3D, Component,
        MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(skeleton, Skeleton),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, boneName, String(), getBoneName, setBoneName));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SkinnedMeshAttach3D);


    //////////////////////////////////////////
    SkinnedMeshAttach3D::SkinnedMeshAttach3D()
    {
        
    }

    //////////////////////////////////////////
    SkinnedMeshAttach3D::~SkinnedMeshAttach3D()
    {
        
    }

    //////////////////////////////////////////
    SkinnedMeshAttach3DPtr SkinnedMeshAttach3D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        SkinnedMeshAttach3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SkinnedMeshAttach3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SkinnedMeshAttach3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool SkinnedMeshAttach3D::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        if (!Component::init(
            _component,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SkinnedMeshAttach3D::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void SkinnedMeshAttach3D::assignSkeletonIfRequired()
    {
        if (!m_skeleton)
        {
            if (SkinnedMeshSkeleton* skeletonRaw = EcsHelper::GetFirstTrunkComponent<SkinnedMeshSkeleton>(getEntityRaw()))
                m_skeleton = skeletonRaw->cast<SkinnedMeshSkeleton>();
        }

        updateBoneIndex();
    }

    //////////////////////////////////////////
    void SkinnedMeshAttach3D::processEntityAwakened()
    {
        assignSkeletonIfRequired();
    }

    //////////////////////////////////////////
    void SkinnedMeshAttach3D::updateBoneIndex()
    {
        m_boneIndex = -1;
        if (m_boneName.empty())
            return;

        if (!m_skeleton ||
            !m_skeleton->getRenderMesh() ||
            !m_skeleton->getRenderMesh()->getMesh())
            return;

        MeshSkeletonPtr const& skeleton = m_skeleton->getRenderMesh()->getMesh()->getSkeleton();
        if (!skeleton)
            return;

        m_boneIndex = skeleton->findBoneIndex(HashedCString(m_boneName.c_str()));
        if (m_boneIndex == -1)
        {
            Debug::LogError("Undefined bone with name: %s! Available bones", m_boneName.c_str());
            for (S32 i = 0; i < skeleton->getBonesCount(); ++i)
                Debug::LogError("  %d - %s", i, skeleton->getBone(i).name.c_str());

        }
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SkinnedMeshAttach3DAdded,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        SkinnedMeshAttach3D* _attach)
    {
        _attach->assignSkeletonIfRequired();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SkinnedMeshAttach3DUpdateSystem,
        {}, //MAZE_ECS_TAGS(MAZE_HS("default")),
        MAZE_ECS_ORDER(
            MAZE_ECS_ORDER_AFTER(MAZE_HS("SkinnedMeshSkeletonUpdateSystem")),
            MAZE_ECS_ORDER_BEFORE()),
        UpdateEvent const& _event,
        Entity* _entity,
        SkinnedMeshAttach3D* _attach,
        Transform3D* _transform)
    {
        if (!_transform->getParent())
            return;
        
        S32 boneIndex = _attach->getBoneIndex();
        if (boneIndex < 0)
            return;

        SkinnedMeshSkeletonPtr const& skeletonController = _attach->getSkeleton();
        if (!skeletonController->getAnimator())
            return;

        Vector<TMat> const& boneGlobalTms = skeletonController->getAnimator()->getBonesGlobalTransforms();
        if (boneIndex >= (S32)boneGlobalTms.size())
            return;

        TMat const& rootTm = skeletonController->getRenderMesh()->getMesh()->getSkeleton()->getRootTransform();

        TMat tm = rootTm.transform(boneGlobalTms[boneIndex]);
        tm.resetScale();
        _transform->setLocalTransform(tm);
    }

    
} // namespace Maze
//////////////////////////////////////////
