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
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GizmosSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(GizmosSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GizmosSystem);

    //////////////////////////////////////////
    GizmosSystem::GizmosSystem()
        : m_renderTarget(nullptr)
    {
        
    }

    //////////////////////////////////////////
    GizmosSystem::~GizmosSystem()
    {
        GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    GizmosSystemPtr GizmosSystem::Create(RenderTarget* _renderTarget)
    {
        GizmosSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GizmosSystem, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool GizmosSystem::init(RenderTarget* _renderTarget)
    {
        m_renderTarget = _renderTarget;
        m_renderTarget->eventRenderTargetDestroyed.subscribe(this, &GizmosSystem::notifyRenderTargetDestroyed);

        GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.subscribe(this, &GizmosSystem::notifyGizmosPerComponentClass);
    

        return true;
    }

    //////////////////////////////////////////
    void GizmosSystem::processSystemAdded()
    {
        m_drawer = GizmosDrawer::Create(m_worldRaw, m_renderTarget);

        updateGizmosSamples();
    }

    //////////////////////////////////////////
    void GizmosSystem::processUpdate(F32 _dt)
    {
        drawGizmos();
    }

    //////////////////////////////////////////
    void GizmosSystem::notifyGizmosPerComponentClass()
    {
        updateGizmosSamples();
    }

    //////////////////////////////////////////
    void GizmosSystem::updateGizmosSamples()
    {
        m_samples.clear();

        auto const& gizmosPerComponentClass = GizmosManager::GetInstancePtr()->getGizmosPerComponentClass();
        for (auto const& gizmoPerComponentClass : gizmosPerComponentClass)
        {
            GizmosSample gizmosSample;
            gizmosSample.componentClassUID = gizmoPerComponentClass.first;
            gizmosSample.sample = m_worldRaw->requestCommonSample(
                EntityAspect(
                    EntityAspectType::HaveAllOfComponents,
                    { gizmosSample.componentClassUID }));
            gizmosSample.gizmos = gizmoPerComponentClass.second;

            m_samples.push_back(gizmosSample);
        }
    }

    //////////////////////////////////////////
    void GizmosSystem::drawGizmos()
    {
        if (!m_drawer)
            return;

        m_drawer->clear();

        for (GizmosSample const& gizmosSample : m_samples)
        {
            for (Entity* entity : gizmosSample.sample->getEntities())
            {
                if (!entity)
                    continue;

                ComponentPtr const& component = entity->getComponentByUID(gizmosSample.componentClassUID);
                gizmosSample.gizmos->drawGizmos(entity, component, m_drawer.get());

                eventDrawGizmosEvent(gizmosSample.gizmos, entity, component, m_drawer.get());
            }
        }

        m_drawer->rebuildMeshes();
    }

    //////////////////////////////////////////
    void GizmosSystem::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        m_samples.clear();
        m_drawer.reset();
        m_worldRaw->update(0.0f);
    }

    
} // namespace Maze
//////////////////////////////////////////
