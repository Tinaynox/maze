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
#include "maze-core/ecs/MazeECSWorld.hpp"
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

        GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.subscribe(this, &GizmosController::notifyGizmosPerComponentClass);        

        EditorToolsManager::GetInstancePtr()->setGizmosController(this);

        return true;
    }

    //////////////////////////////////////////
    void GizmosController::processEntityAwakened()
    {
        m_drawer = GizmosDrawer::Create(getEntityRaw()->getECSScene()->getWorld(), m_renderTarget);
        m_canvasesSample = getEntityRaw()->getECSScene()->getWorld()->requestInclusiveSample<Canvas>();
        m_cameras3DSample = getEntityRaw()->getECSScene()->getWorld()->requestInclusiveSample<Camera3D>();

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

        auto const& gizmosPerComponentClass = GizmosManager::GetInstancePtr()->getGizmosPerComponentClass();
        for (auto const& gizmoPerComponentClass : gizmosPerComponentClass)
        {
            GizmosSample gizmosSample;
            gizmosSample.componentClassUID = gizmoPerComponentClass.first;
            gizmosSample.sample = getEntityRaw()->getECSScene()->getWorld()->requestCommonSample(
                EntityAspect(
                    EntityAspectType::HaveAllOfComponents,
                    { gizmosSample.componentClassUID }));
            gizmosSample.gizmos = gizmoPerComponentClass.second;

            m_samples.push_back(gizmosSample);
        }
    }

    //////////////////////////////////////////
    void GizmosController::drawGizmos(F32 _dt)
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

        m_drawer->update(_dt);
    }

    //////////////////////////////////////////
    void GizmosController::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        m_samples.clear();
        m_drawer.reset();
        getEntityRaw()->getECSScene()->getWorld()->update(0.0f);
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM(GizmosSystem, 40000,
        UpdateEvent const& _event,
        Entity* _entity,
        GizmosController* _gizmosController)
    {
        bool haveGizmosMask = false;
        _gizmosController->getCameras3DSample()->process(
            [&haveGizmosMask](Entity* _entity, Camera3D* _camera)
        {
            haveGizmosMask |= (bool)(_camera->getRenderMask() & S32(DefaultRenderMask::Gizmos));
        });

        if (haveGizmosMask)
            _gizmosController->drawGizmos(_event.getDt());
    }

    
} // namespace Maze
//////////////////////////////////////////
