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
#include "maze-editor-tools/ecs/components/MazeGizmosController.hpp"
#include "maze-editor-tools/ecs/components/MazeGizmosDrawer.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GizmosController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(GizmosController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GizmosController);

    //////////////////////////////////////////
    GizmosController::GizmosController()
        : m_renderTarget(nullptr)
    {
        
    }

    //////////////////////////////////////////
    GizmosController::~GizmosController()
    {
        if (m_renderTarget)
            m_renderTarget->eventRenderTargetDestroyed.unsubscribe(this);

        if (GizmosManager::GetInstancePtr())
            GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.unsubscribe(this);

        if (EditorToolsManager::GetInstancePtr())
            EditorToolsManager::GetInstancePtr()->setGizmosController(nullptr);
    }

    //////////////////////////////////////////
    GizmosControllerPtr GizmosController::Create(RenderTarget* _renderTarget)
    {
        GizmosControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GizmosController, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool GizmosController::init(RenderTarget* _renderTarget)
    {
        m_renderTarget = _renderTarget;
        m_renderTarget->eventRenderTargetDestroyed.subscribe(this, &GizmosController::notifyRenderTargetDestroyed);

        if (GizmosManager::GetInstancePtr())
            GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.subscribe(this, &GizmosController::notifyGizmosPerComponentClass);        

        if (EditorToolsManager::GetInstancePtr())
            EditorToolsManager::GetInstancePtr()->setGizmosController(this);

        return true;
    }

    //////////////////////////////////////////
    void GizmosController::processEntityAwakened()
    {
        m_drawer = GizmosDrawer::Create(getEntityRaw()->getEcsWorld(), m_renderTarget);
        m_canvasesSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<Canvas>();
        m_cameras3DSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<Camera3D>();

        updateGizmosSamples();
    }

    //////////////////////////////////////////
    void GizmosController::notifyGizmosPerComponentClass()
    {
        updateGizmosSamples();
    }

    //////////////////////////////////////////
    void GizmosController::updateGizmosSamples()
    {
        m_samples.clear();

        if (GizmosManager::GetInstancePtr())
        {
            auto const& gizmosPerComponentClass = GizmosManager::GetInstancePtr()->getGizmosPerComponentClass();
            for (auto const& gizmoPerComponentClass : gizmosPerComponentClass)
            {
                GizmosSample gizmosSample;
                gizmosSample.componentId = gizmoPerComponentClass.first;
                gizmosSample.sample = getEntityRaw()->getEcsWorld()->requestCommonSample(
                    EntityAspect(
                        EntityAspectType::HaveAllOfComponents,
                        { gizmosSample.componentId }));
                gizmosSample.gizmos = gizmoPerComponentClass.second;

                m_samples.push_back(gizmosSample);
            }
        }
    }

    //////////////////////////////////////////
    void GizmosController::drawGizmos(
        F32 _dt,
        bool _draw)
    {
        if (!m_drawer)
            return;

        // m_drawer->clear();

        if (_draw)
        {
            for (GizmosSample const& gizmosSample : m_samples)
            {
                for (Entity* entity : gizmosSample.sample->getEntities())
                {
                    if (!entity)
                        continue;

                    ComponentPtr const& component = entity->getComponentById(gizmosSample.componentId);
                    gizmosSample.gizmos->drawGizmos(entity, component, m_drawer.get());

                    eventDrawGizmosEvent(gizmosSample.gizmos, entity, component, m_drawer.get());
                }
            }
        }

        m_drawer->update(_dt);
    }

    //////////////////////////////////////////
    void GizmosController::clear()
    {
        m_drawer->clear();
    }

    //////////////////////////////////////////
    void GizmosController::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        if (m_drawer)
            m_drawer->destroy();

        m_samples.clear();
        m_drawer.reset();

        if (_renderTarget)
            _renderTarget->eventRenderTargetDestroyed.unsubscribe(this);

        m_renderTarget = nullptr;
    }

    //////////////////////////////////////////
    void GizmosController::processEcsWorldWillBeDestroyed()
    {
        if (m_drawer)
            m_drawer->destroy();

        m_samples.clear();
        m_drawer.reset();
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(GizmosSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        MAZE_ECS_ORDER(
            MAZE_ECS_ORDER_AFTER(),
            MAZE_ECS_ORDER_BEFORE(MAZE_HS("RenderControllerSystem"))),
        PostUpdateEvent const& _event,
        Entity* _entity,
        GizmosController* _gizmosController)
    {
        bool haveGizmosMask = false;
        _gizmosController->getCameras3DSample()->query(
            [&haveGizmosMask](Entity* _entity, Camera3D* _camera)
            {
                haveGizmosMask |= (bool)(_camera->getRenderMask() & S32(DefaultRenderMask::Gizmos));
            });

        _gizmosController->drawGizmos(_event.getDt(), haveGizmosMask);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(GizmosSystemPostRender,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        MAZE_ECS_ORDER(
            MAZE_ECS_ORDER_AFTER(MAZE_HS("RenderControllerSystem")),
            MAZE_ECS_ORDER_BEFORE()),
        PostUpdateEvent const& _event,
        Entity* _entity,
        GizmosController* _gizmosController)
    {
        _gizmosController->clear();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(GizmosControllerOnEcsWorldWillBeDestroyed,
        {},
        {},
        EcsWorldWillBeDestroyedEvent const& _event,
        Entity* _entity,
        GizmosController* _gizmosController)
    {
        _gizmosController->processEcsWorldWillBeDestroyed();
    }

    
} // namespace Maze
//////////////////////////////////////////
