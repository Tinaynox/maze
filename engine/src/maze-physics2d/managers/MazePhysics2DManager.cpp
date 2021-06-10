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
#include "MazePhysics2DHeader.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-physics2d/ecs/components/gizmos/MazeBoxCollider2DGizmos.hpp"
#include "maze-physics2d/ecs/components/gizmos/MazeCircleCollider2DGizmos.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/managers/MazePhysicsMaterial2DManager.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Physics2DManager
    //
    //////////////////////////////////////////
    Physics2DManager* Physics2DManager::s_instance = nullptr;
    
    //////////////////////////////////////////
    Physics2DManager::Physics2DManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    Physics2DManager::~Physics2DManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void Physics2DManager::Initialize(
        Physics2DManagerPtr& _physics2DManager,
        PhysicsWorld2DConfig const& _defaultWorldConfig)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(Physics2DManager, _physics2DManager, init(_defaultWorldConfig));
    }

    //////////////////////////////////////////
    bool Physics2DManager::init(PhysicsWorld2DConfig const& _defaultWorldConfig)
    {
        m_world = PhysicsWorld2D::Create();

        GizmosManager::GetInstancePtr()->registerGizmos<BoxCollider2D, BoxCollider2DGizmos>();
        GizmosManager::GetInstancePtr()->registerGizmos<CircleCollider2D, CircleCollider2DGizmos>();

        PhysicsMaterial2DManager::Initialize(m_physicsMaterial2DManager);
        if (!m_physicsMaterial2DManager)
            return false;

        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Rigidbody2D>("2D/Physics");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<BoxCollider2D>("2D/Physics");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<CircleCollider2D>("2D/Physics");

        return true;
    }

    //////////////////////////////////////////
    void Physics2DManager::update(F32 _dt)
    {
        if (m_world)
            m_world->update(_dt);
    }

    
} // namespace Maze
//////////////////////////////////////////
