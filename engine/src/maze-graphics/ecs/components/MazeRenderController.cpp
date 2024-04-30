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
#include "maze-graphics/ecs/components/MazeRenderController.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderControllerModule2D.hpp"
#include "maze-graphics/ecs/components/MazeRenderControllerModule3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(RenderController);

    //////////////////////////////////////////
    RenderController::RenderController()
        : m_renderTargetsDirty(true)
    {
    }

    //////////////////////////////////////////
    RenderController::~RenderController()
    {
        for (Camera3D* camera : m_cameras3D)
            camera->eventRenderTargetChanged.unsubscribe(this);
        m_cameras3D.clear();

        for (Canvas* canvas : m_canvases)
            canvas->eventRenderTargetChanged.unsubscribe(this);
        m_canvases.clear();
        
        if (m_canvasesSample)
        {
            m_canvasesSample->eventEntityAdded.unsubscribe(this, &RenderController::processCanvasEntityAdded);
            m_canvasesSample->eventEntityRemoved.unsubscribe(this, &RenderController::processCanvasEntityRemoved);
        }

        if (m_cameras3DSample)
        {
            m_cameras3DSample->eventEntityAdded.unsubscribe(this, &RenderController::processCameraEntityAdded);
            m_cameras3DSample->eventEntityRemoved.unsubscribe(this, &RenderController::processCameraEntityRemoved);
        }

        clearRenderTargets();
    }

    //////////////////////////////////////////
    RenderControllerPtr RenderController::Create(RenderSystemPtr const& _renderSystem)
    {
        RenderControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderController, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool RenderController::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;

        m_renderTargetsDirty = true;

        return true;
    }

    //////////////////////////////////////////
    void RenderController::processEntityAwakened()
    {
        m_canvasesSample = getEntityRaw()->getECSWorld()->requestInclusiveSample<Canvas>();
        m_canvasesSample->eventEntityAdded.subscribe(this, &RenderController::processCanvasEntityAdded);
        m_canvasesSample->eventEntityRemoved.subscribe(this, &RenderController::processCanvasEntityRemoved);

        m_cameras3DSample = getEntityRaw()->getECSWorld()->requestInclusiveSample<Camera3D>();
        m_cameras3DSample->eventEntityAdded.subscribe(this, &RenderController::processCameraEntityAdded);
        m_cameras3DSample->eventEntityRemoved.subscribe(this, &RenderController::processCameraEntityRemoved);

        m_module3D = RenderControllerModule3D::Create(getEntityRaw()->getECSWorld()->cast<ECSWorld>(), m_renderSystem);
        m_module2D = RenderControllerModule2D::Create(getEntityRaw()->getECSWorld()->cast<ECSWorld>(), m_renderSystem);
    }

    //////////////////////////////////////////
    void RenderController::updateRenderTargets()
    {
        clearRenderTargets();

        m_canvasesSample->process(
            [&](Entity* _entity, Canvas* _canvas)
            {
                if (_canvas->getRenderTarget())
                    addRenderTarget(_canvas->getRenderTarget().get());
            });

        m_cameras3DSample->process(
            [&](Entity* _entity, Camera3D* _camera)
            {
                if (_camera->getRenderTarget())
                    addRenderTarget(_camera->getRenderTarget().get());
            });

        sortRenderTargets();

        m_renderTargetsDirty = false;
    }

    //////////////////////////////////////////
    void RenderController::processCanvasEntityAdded(Entity* _entity, Canvas* _canvas)
    {
        m_renderTargetsDirty = true;
        m_canvases.insert(_canvas);
        _canvas->eventRenderTargetChanged.subscribe(this, &RenderController::processCanvasRenderTargetChanged);
    }

    //////////////////////////////////////////
    void RenderController::processCanvasEntityRemoved(Entity* _entity, Canvas* _canvas)
    {
        m_renderTargetsDirty = true;
        _canvas->eventRenderTargetChanged.unsubscribe(this);
        m_canvases.erase(_canvas);
    }

    //////////////////////////////////////////
    void RenderController::processCanvasRenderTargetChanged(Canvas* _canvas, RenderTargetPtr const& _renderTarget)
    {
        m_renderTargetsDirty = true;
    }

    //////////////////////////////////////////
    void RenderController::processCameraEntityAdded(Entity* _entity, Camera3D* _camera3D)
    {
        m_renderTargetsDirty = true;
        m_cameras3D.insert(_camera3D);
        _camera3D->eventRenderTargetChanged.subscribe(this, &RenderController::processCameraRenderTargetChanged);
    }

    //////////////////////////////////////////
    void RenderController::processCameraEntityRemoved(Entity* _entity, Camera3D* _camera3D)
    {
        m_renderTargetsDirty = true;
        _camera3D->eventRenderTargetChanged.unsubscribe(this);
        m_cameras3D.erase(_camera3D);
    }

    //////////////////////////////////////////
    void RenderController::processCameraRenderTargetChanged(Camera3D* _camera3D, RenderTargetPtr const& _renderTarget)
    {
        m_renderTargetsDirty = true;
    }


    //////////////////////////////////////////
    void RenderController::update(UpdateEvent const& _event)
    {
        m_module3D->processUpdate(_event);
        m_module2D->processUpdate(_event);

        render();

        m_module3D->processPostUpdate(_event);
        m_module2D->processPostUpdate(_event);
    }

    //////////////////////////////////////////
    void RenderController::render()
    {
        if (m_renderTargetsDirty)
            updateRenderTargets();

        for (RenderTarget* renderTarget : m_renderTargets)
        {
            m_module3D->draw(renderTarget);
            m_module2D->draw(renderTarget);

            renderTarget->swapBuffers();
        }
    }

    //////////////////////////////////////////
    void RenderController::addRenderTarget(RenderTarget* _renderTarget)
    {
        auto it = std::find(
            m_renderTargets.begin(),
            m_renderTargets.end(),
            _renderTarget);

        if (it != m_renderTargets.end())
            return;

        m_renderTargets.push_back(_renderTarget);

        _renderTarget->eventRenderTargetOrderChanged.subscribe(this, &RenderController::notifyRenderTargetOrderChanged);
        _renderTarget->eventRenderTargetDestroyed.subscribe(this, &RenderController::notifyRenderTargetDestroyed);
    }

    //////////////////////////////////////////
    void RenderController::removeRenderTarget(RenderTarget* _renderTarget)
    {
        auto it = std::find(
            m_renderTargets.begin(),
            m_renderTargets.end(),
            _renderTarget);

        if (it == m_renderTargets.end())
            return;

        (*it)->eventRenderTargetOrderChanged.unsubscribe(this, &RenderController::notifyRenderTargetOrderChanged);
        (*it)->eventRenderTargetDestroyed.unsubscribe(this, &RenderController::notifyRenderTargetDestroyed);

        m_renderTargets.erase(it);
    }

    //////////////////////////////////////////
    void RenderController::clearRenderTargets()
    {
        while (!m_renderTargets.empty())
        {
            RenderTarget* renderTarget = m_renderTargets.back();
            renderTarget->eventRenderTargetOrderChanged.unsubscribe(this, &RenderController::notifyRenderTargetOrderChanged);
            renderTarget->eventRenderTargetDestroyed.unsubscribe(this, &RenderController::notifyRenderTargetDestroyed);

            m_renderTargets.pop_back();
        }
    }

    //////////////////////////////////////////
    void RenderController::sortRenderTargets()
    {
        std::sort(
            m_renderTargets.begin(),
            m_renderTargets.end(),
            [](RenderTarget* _renderTarget0, RenderTarget* _renderTarget1) -> bool
            {
                return _renderTarget0->getOrder() < _renderTarget1->getOrder();
            });
    }

    //////////////////////////////////////////
    void RenderController::notifyRenderTargetOrderChanged(RenderTarget* _renderTarget, S32 _order)
    {
        sortRenderTargets();
    }

    //////////////////////////////////////////
    void RenderController::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        removeRenderTarget(_renderTarget);
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM(RenderControllerSystem, 50000,
        UpdateEvent const& _event,
        Entity* _entity,
        RenderController* _renderController)
    {
        _renderController->update(_event);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
