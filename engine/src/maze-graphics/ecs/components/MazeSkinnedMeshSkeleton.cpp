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
#include "maze-graphics/ecs/components/MazeSkinnedMeshSkeleton.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
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
    // Class SkinnedMeshSkeleton
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SkinnedMeshSkeleton, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(RenderMeshAssetRef, renderMesh, RenderMeshAssetRef(), getRenderMeshRef, setRenderMeshRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SkinnedMeshSkeleton);


    //////////////////////////////////////////
    SkinnedMeshSkeleton::SkinnedMeshSkeleton()
    {
        
    }

    //////////////////////////////////////////
    SkinnedMeshSkeleton::~SkinnedMeshSkeleton()
    {
        
    }

    //////////////////////////////////////////
    SkinnedMeshSkeletonPtr SkinnedMeshSkeleton::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        SkinnedMeshSkeletonPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SkinnedMeshSkeleton, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SkinnedMeshSkeleton::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;
        m_animator = MeshSkeletonAnimator::Create();

        return true;
    }

    //////////////////////////////////////////
    bool SkinnedMeshSkeleton::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<SkinnedMeshSkeleton>()->m_renderSystem;
        m_animator = MeshSkeletonAnimator::Create();

        if (!Component::init(
            _component,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::update(F32 _dt)
    {
        if (getEnabled())
            m_animator->update(_dt);
    }

    //////////////////////////////////////////
    S32 SkinnedMeshSkeleton::playAnimation(
        HashedCString _animationName,
        MeshSkeletonAnimationStartParams _startParams)
    {
        if (m_animator)
            return m_animator->playAnimation(_animationName, _startParams);

        return -1;
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::setMesh(MeshPtr const& _mesh)
    {
        MAZE_ERROR_RETURN_IF(!m_renderSystem, "Render system is null");

        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene()->getMetaClass()->isInheritedFrom<EcsRenderScene>(), "Entity Scene is not Render Scene");
#endif

        if (!_mesh)
        {
            clearMesh();
            return;
        }

        RenderTargetPtr const& renderTarget = getEntityRaw()->getEcsScene()->castRaw<EcsRenderScene>()->getRenderTarget();

        if (!m_renderMeshRef.getRenderMesh())
            m_renderMeshRef.setRenderMesh(renderTarget->createRenderMeshFromPool((S32)_mesh->getSubMeshesCount()));

        m_renderMeshRef.getRenderMesh()->loadFromMesh(
            _mesh,
            renderTarget.get());
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::setRenderMesh(String const& _renderMeshName)
    {
        RenderMeshPtr const& renderMesh = m_renderSystem->getRenderMeshManager()->getOrLoadRenderMesh(_renderMeshName);
        MAZE_ERROR_RETURN_IF(!renderMesh, "Udefined render mesh - %s!", _renderMeshName.c_str());
        setRenderMesh(renderMesh);
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::clearMesh()
    {
        if (!getRenderMesh())
            return;

        getRenderMesh()->clear();
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::processEntityAwakened()
    {
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::processEntityRemoved()
    {
        //m_renderMeshRef.setRenderMesh(nullptr);
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::setRenderMeshRef(RenderMeshAssetRef const& _renderMesh)
    {
        m_renderMeshRef.setRenderMesh(_renderMesh.getRenderMesh());

        if (m_animator)
        {
            RenderMeshPtr const& renderMesh = getRenderMesh();
            if (renderMesh && renderMesh->getMesh() && renderMesh->getMesh()->getSkeleton())
            {
                m_animator->setSkeleton(renderMesh->getMesh()->getSkeleton());
                m_animator->update(0.0f);
            }
            else
                m_animator->setSkeleton(nullptr);
        }
    }

    //////////////////////////////////////////
    void SkinnedMeshSkeleton::setRenderMesh(RenderMeshPtr const& _renderMesh)
    {
        setRenderMeshRef(_renderMesh);
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SkinnedMeshSkeletonUpdateSystem,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        SkinnedMeshSkeleton* _meshSkeleton)
    {
        _meshSkeleton->update(_event.getDt());
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SkinnedMeshSkeletonEntityRemoved,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        EntityRemovedEvent const& _event,
        Entity* _entity,
        SkinnedMeshSkeleton* _meshRenderer)
    {
        _meshRenderer->processEntityRemoved();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SkinnedMeshSkeletonOnEcsWorldWillBeDestroyed,
        {},
        {},
        EcsWorldWillBeDestroyedEvent const& _event,
        Entity* _entity,
        SkinnedMeshSkeleton* _meshRenderer)
    {
        // Release render mesh before RenderMeshPool will be destroyed
        _meshRenderer->setRenderMesh(RenderMeshPtr());
    }
    
    
} // namespace Maze
//////////////////////////////////////////
