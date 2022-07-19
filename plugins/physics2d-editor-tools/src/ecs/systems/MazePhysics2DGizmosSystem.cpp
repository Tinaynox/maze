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
#include "maze-plugin-physics2d-editor-tools/ecs/systems/MazePhysics2DGizmosSystem.hpp"
#include "maze-physics2d/helpers/MazeBox2DHelper.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Physics2DGizmosSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Physics2DGizmosSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Physics2DGizmosSystem);

    //////////////////////////////////////////
    Physics2DGizmosSystem::Physics2DGizmosSystem()
    {
        
    }

    //////////////////////////////////////////
    Physics2DGizmosSystem::~Physics2DGizmosSystem()
    {
        if (GizmosManager::GetInstancePtr())
            GizmosManager::GetInstancePtr()->eventGizmosPerComponentClassChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    Physics2DGizmosSystemPtr Physics2DGizmosSystem::Create(PhysicsWorld2D* _physicsWorld, RenderTarget* _renderTarget)
    {
        Physics2DGizmosSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Physics2DGizmosSystem, object, init(_physicsWorld, _renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool Physics2DGizmosSystem::init(PhysicsWorld2D* _physicsWorld, RenderTarget* _renderTarget)
    {
        m_physicsWorld = _physicsWorld;
        m_physicsWorld->getBox2DWorld()->SetDebugDraw(this);
        SetFlags(e_shapeBit);

        m_renderTarget = _renderTarget;
        m_renderTarget->eventRenderTargetDestroyed.subscribe(this, &Physics2DGizmosSystem::notifyRenderTargetDestroyed);

        return true;
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::processSystemAdded()
    {
        m_drawer = GizmosDrawer::Create(m_worldRaw, m_renderTarget);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::processUpdate(F32 _dt)
    {
        drawGizmos();
    }
    //////////////////////////////////////////
    void Physics2DGizmosSystem::drawGizmos()
    {
        if (!m_drawer)
            return;

        m_drawer->clear();

        m_physicsWorld->getBox2DWorld()->DebugDraw();

        m_drawer->rebuildMeshes();
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::notifyRenderTargetDestroyed(RenderTarget* _renderTarget)
    {
        m_drawer.reset();
        m_worldRaw->update(0.0f);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawPolygon(b2Vec2 const* _vertices, int32 _vertexCount, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);

        m_drawer->setColor(color);
        for (S32 i = 0; i < _vertexCount - 1; ++i)
        {
            Vec2DF p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[i]));
            Vec2DF p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[i + 1]));
            m_drawer->drawLine(p0, p1);
        }

        Vec2DF p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[_vertexCount - 1]));
        Vec2DF p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[0]));
        m_drawer->drawLine(p0, p1);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawSolidPolygon(b2Vec2 const* _vertices, int32 _vertexCount, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);

        m_drawer->setColor(color);
        for (S32 i = 0; i < _vertexCount - 1; ++i)
        {
            Vec2DF p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[i]));
            Vec2DF p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[i + 1]));
            m_drawer->drawLine(p0, p1);
        }

        Vec2DF p0 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[_vertexCount - 1]));
        Vec2DF p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_vertices[0]));
        m_drawer->drawLine(p0, p1);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawCircle(b2Vec2 const& _center, F32 _radius, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2DF p = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_center));
        F32 r = m_physicsWorld->convertMetersToUnits(_radius);

        m_drawer->setColor(color);
        m_drawer->drawWireCircle(p, Vec3DF::c_negativeUnitZ, r);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawSolidCircle(b2Vec2 const& _center, F32 _radius, b2Vec2 const& _axis, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2DF p = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_center));
        F32 r = m_physicsWorld->convertMetersToUnits(_radius);

        m_drawer->setColor(color);
        m_drawer->drawWireCircle(p, Vec3DF::c_negativeUnitZ, r);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawSegment(b2Vec2 const& _p1, b2Vec2 const& _p2, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2DF p1 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_p1));
        Vec2DF p2 = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_p2));

        m_drawer->setColor(color);
        m_drawer->drawLine(p1, p2);
    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawTransform(b2Transform const& _xf)
    {

    }

    //////////////////////////////////////////
    void Physics2DGizmosSystem::DrawPoint(b2Vec2 const& _p, F32 _size, b2Color const& _color)
    {
        ColorF128 color = Box2DHelper::ToColor128F(_color);
        Vec2DF p = m_physicsWorld->convertMetersToUnits(Box2DHelper::ToVec2DF(_p));
        F32 r = 0.1f;

        m_drawer->setColor(color);
        m_drawer->drawWireCircle(p, Vec3DF::c_negativeUnitZ, r);
    }

    
} // namespace Maze
//////////////////////////////////////////
