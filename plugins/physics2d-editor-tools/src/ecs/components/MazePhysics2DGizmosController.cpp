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
#include "MazePhysics2DEditorToolsHeader.hpp"
#include "maze-plugin-physics2d-editor-tools/ecs/components/MazePhysics2DGizmosController.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Physics2DGizmosController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Physics2DGizmosController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Physics2DGizmosController);

    //////////////////////////////////////////
    Physics2DGizmosController::Physics2DGizmosController()
    {
        
    }

    //////////////////////////////////////////
    Physics2DGizmosController::~Physics2DGizmosController()
    {
        if (GizmosManager::GetInstancePtr())
            GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.unsubscribe(this);

        if (m_renderTarget)
            m_renderTarget->eventRenderTargetDestroyed.unsubscribe(this);
    }

    //////////////////////////////////////////
    Physics2DGizmosControllerPtr Physics2DGizmosController::Create(PhysicsWorld2D* _physicsWorld, RenderTarget* _renderTarget)
    {
        Physics2DGizmosControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Physics2DGizmosController, object, init(_physicsWorld, _renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool Physics2DGizmosController::init(PhysicsWorld2D* _physicsWorld, RenderTarget* _renderTarget)
    {
        m_physicsWorld = _physicsWorld;
        m_physicsWorld->getBox2DWorld()->SetDebugDraw(this);
        SetFlags(e_shapeBit);

        m_renderTarget = _renderTarget;
        m_renderTarget->eventRenderTargetDestroyed.subscribe(this, &Physics2DGizmosController::notifyRenderTargetDestroyed);

        return true;
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::processEntityAwakened()
    {
        m_drawer = GizmosDrawer::Create(getEntityRaw()->getEcsWorld(), m_renderTarget);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::drawGizmos()
    {
        if (!m_drawer)
            return;

        m_drawer->clear();

        m_physicsWorld->getBox2DWorld()->DebugDraw();

        m_drawer->rebuildMeshes();
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        m_drawer.reset();
        getEntityRaw()->getEcsWorld()->update(0.0f);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawPolygon(b2Vec2 const* _vertices, int32 _vertexCount, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);

        m_drawer->setColor(color);
        for (S32 i = 0; i < _vertexCount - 1; ++i)
        {
            Vec2F p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[i]));
            Vec2F p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[i + 1]));
            m_drawer->drawLine(p0, p1);
        }

        Vec2F p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[_vertexCount - 1]));
        Vec2F p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[0]));
        m_drawer->drawLine(p0, p1);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawSolidPolygon(b2Vec2 const* _vertices, int32 _vertexCount, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);

        m_drawer->setColor(color);
        for (S32 i = 0; i < _vertexCount - 1; ++i)
        {
            Vec2F p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[i]));
            Vec2F p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[i + 1]));
            m_drawer->drawLine(p0, p1);
        }

        Vec2F p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[_vertexCount - 1]));
        Vec2F p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_vertices[0]));
        m_drawer->drawLine(p0, p1);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawCircle(b2Vec2 const& _center, F32 _radius, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2F p = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_center));
        F32 r = m_physicsWorld->convertMetersToUnits(_radius);

        m_drawer->setColor(color);
        m_drawer->drawWireCircle(p, Vec3F::c_negativeUnitZ, r);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawSolidCircle(b2Vec2 const& _center, F32 _radius, b2Vec2 const& _axis, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2F p = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_center));
        F32 r = m_physicsWorld->convertMetersToUnits(_radius);

        m_drawer->setColor(color);
        m_drawer->drawWireCircle(p, Vec3F::c_negativeUnitZ, r);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawSegment(b2Vec2 const& _p1, b2Vec2 const& _p2, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2F p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_p1));
        Vec2F p2 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_p2));

        m_drawer->setColor(color);
        m_drawer->drawLine(p1, p2);
    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawTransform(b2Transform const& _xf)
    {

    }

    //////////////////////////////////////////
    void Physics2DGizmosController::DrawPoint(b2Vec2 const& _p, F32 _size, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2F p = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2F32(_p));
        F32 r = 0.1f;

        m_drawer->setColor(color);
        m_drawer->drawWireCircle(p, Vec3F::c_negativeUnitZ, r);
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(Physics2DGizmosSystem,
        MAZE_ECS_ORDER(
            MAZE_ECS_ORDER_AFTER(MAZE_HS("GizmosSystem")),
            MAZE_ECS_ORDER_BEFORE(MAZE_HS("RenderControllerSystem"))),
        UpdateEvent const& _event,
        Entity* _entity,
        Physics2DGizmosController* _controller)
    {
        _controller->drawGizmos();
    }
    
} // namespace Maze
//////////////////////////////////////////
