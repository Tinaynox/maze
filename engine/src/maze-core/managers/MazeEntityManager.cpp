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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeAwakeRandomRotation3D.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class EntityManager
    //
    //////////////////////////////////////////
    EntityManager* EntityManager::s_instance = nullptr;

    //////////////////////////////////////////
    EntityManager::EntityManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EntityManager::~EntityManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EntityManager::Initialize(EntityManagerPtr& _entityManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EntityManager, _entityManager, init());
    }

    //////////////////////////////////////////
    bool EntityManager::init()
    {
        m_defaultWorld = ECSWorld::Create(0);
        m_libraryWorld = ECSWorld::Create(100000000);
        
        m_componentFactory = ComponentFactory::Create();

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        // Common
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Name>("Common");

        // 2D
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Transform2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Bounds2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SizePolicy2D>("2D");

        // 3D
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Transform3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Rotor3D>("3D/Utils");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SinMovement3D>("3D/Utils");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<AwakeRandomRotation3D>("3D/Utils");


        EntitySerializationManager::Initialize(m_entitySerializationManager);
        if (!m_entitySerializationManager)
            return false;

        return true;
    }

    //////////////////////////////////////////
    S32 EntityManager::getComponentPriority(ClassUID _componentUID)
    {
        return static_cast<S32>(_componentUID);
    }

    //////////////////////////////////////////
    void EntityManager::update(F32 _dt)
    {
        m_defaultWorld->update(_dt);
    }

} // namespace Maze
//////////////////////////////////////////
