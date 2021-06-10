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
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule2D.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule3D.hpp"
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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderControlSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderControlSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(RenderControlSystem);

    //////////////////////////////////////////
    RenderControlSystem::RenderControlSystem()
        : m_renderTargetsDirty(true)
    {
    }

    //////////////////////////////////////////
    RenderControlSystem::~RenderControlSystem()
    {
        if (m_canvasesSample)
        {
            m_canvasesSample->eventEntityAdded.unsubscribe(this);
            m_canvasesSample->eventEntityRemoved.unsubscribe(this);
        }

        if (m_cameras3DSample)
        {
            m_cameras3DSample->eventEntityAdded.unsubscribe(this);
            m_cameras3DSample->eventEntityRemoved.unsubscribe(this);
        }

        clearRenderTargets();
    }

    //////////////////////////////////////////
    RenderControlSystemPtr RenderControlSystem::Create(RenderSystemPtr const& _renderSystem)
    {
        RenderControlSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderControlSystem, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool RenderControlSystem::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;

        m_renderTargetsDirty = true;

        return true;
    }

    //////////////////////////////////////////
    void RenderControlSystem::processSystemAdded()
    {
        m_canvasesSample = m_worldRaw->requestInclusiveSample<Canvas>();
        m_canvasesSample->eventEntityAdded.subscribe(this, &RenderControlSystem::processCanvasEntityAdded);
        m_canvasesSample->eventEntityRemoved.subscribe(this, &RenderControlSystem::processCanvasEntityRemoved);

        m_cameras3DSample = m_worldRaw->requestInclusiveSample<Camera3D>();
        m_cameras3DSample->eventEntityAdded.subscribe(this, &RenderControlSystem::processCameraEntityAdded);
        m_cameras3DSample->eventEntityRemoved.subscribe(this, &RenderControlSystem::processCameraEntityRemoved);

        m_module3D = RenderControlSystemModule3D::Create(getWorld());
        m_module2D = RenderControlSystemModule2D::Create(getWorld());
    }

    //////////////////////////////////////////
    void RenderControlSystem::updateRenderTargets()
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
    void RenderControlSystem::processCanvasEntityAdded(Entity* _entity, Canvas* _canvas)
    {
        m_renderTargetsDirty = true;
    }

    //////////////////////////////////////////
    void RenderControlSystem::processCanvasEntityRemoved(Entity* _entity, Canvas* _canvas)
    {
        m_renderTargetsDirty = true;
    }

    //////////////////////////////////////////
    void RenderControlSystem::processCameraEntityAdded(Entity* _entity, Camera3D* _camera3D)
    {
        m_renderTargetsDirty = true;
    }

    //////////////////////////////////////////
    void RenderControlSystem::processCameraEntityRemoved(Entity* _entity, Camera3D* _camera3D)
    {
        m_renderTargetsDirty = true;
    }

    //////////////////////////////////////////
    void RenderControlSystem::processUpdate(F32 _dt)
    {
        m_module3D->processUpdate(_dt);
        m_module2D->processUpdate(_dt);

        if (m_renderTargetsDirty)
            updateRenderTargets();

        for (RenderTarget* renderTarget : m_renderTargets)
        {            
            m_module3D->draw(renderTarget);
            m_module2D->draw(renderTarget);

            renderTarget->swapBuffers();
        }

        m_module3D->processPostUpdate(_dt);
        m_module2D->processPostUpdate(_dt);

    }

    //////////////////////////////////////////
    void RenderControlSystem::addRenderTarget(RenderTarget* _renderTarget)
    {
        auto it = std::find(
            m_renderTargets.begin(),
            m_renderTargets.end(),
            _renderTarget);

        if (it != m_renderTargets.end())
            return;

        m_renderTargets.push_back(_renderTarget);

        _renderTarget->eventRenderTargetOrderChanged.subscribe(this, &RenderControlSystem::notifyRenderTargetOrderChanged);
        _renderTarget->eventRenderTargetDestroyed.subscribe(this, &RenderControlSystem::notifyRenderTargetDestroyed);
    }

    //////////////////////////////////////////
    void RenderControlSystem::removeRenderTarget(RenderTarget* _renderTarget)
    {
        auto it = std::find(
            m_renderTargets.begin(),
            m_renderTargets.end(),
            _renderTarget);

        if (it == m_renderTargets.end())
            return;

        (*it)->eventRenderTargetOrderChanged.unsubscribe(this);
        (*it)->eventRenderTargetDestroyed.unsubscribe(this);

        m_renderTargets.erase(it);
    }

    //////////////////////////////////////////
    void RenderControlSystem::clearRenderTargets()
    {
        while (!m_renderTargets.empty())
        {
            RenderTarget* renderTarget = m_renderTargets.back();
            renderTarget->eventRenderTargetOrderChanged.unsubscribe(this);
            renderTarget->eventRenderTargetDestroyed.unsubscribe(this);

            m_renderTargets.pop_back();
        }
    }

    //////////////////////////////////////////
    void RenderControlSystem::sortRenderTargets()
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
    void RenderControlSystem::notifyRenderTargetOrderChanged(RenderTarget* _renderTarget, S32 _order)
    {
        sortRenderTargets();
    }

    //////////////////////////////////////////
    void RenderControlSystem::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        removeRenderTarget(_renderTarget);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
