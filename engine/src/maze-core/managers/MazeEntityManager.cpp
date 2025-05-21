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
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeEventRetranslator.hpp"
#include "maze-core/ecs/components/MazeSerializationIndex.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeLinearMovement3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeAwakeRandomRotation3D.hpp"
#include "maze-core/ecs/components/MazePrefabInstance.hpp"


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
        m_libraryWorld.reset();

        m_defaultWorld.reset();
    }

    //////////////////////////////////////////
    void EntityManager::Initialize(
        EntityManagerPtr& _entityManager,
        DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EntityManager, _entityManager, init(_config));
    }

    //////////////////////////////////////////
    bool EntityManager::init(DataBlock const& _config)
    {
        m_defaultWorld = EcsWorld::Create(
            MAZE_HS("Default"),
            true,
            MAZE_ECS_TAGS(MAZE_HS("default"), MAZE_HS("render")));
        m_defaultWorld->reserveEntityIndices(
            _config.getS32(MAZE_HCS("defaultReserveIndices"), 3000));

        m_libraryWorld = EcsWorld::Create(
            MAZE_HS("Library"),
            false);
        m_libraryWorld->reserveEntityIndices(
            _config.getS32(MAZE_HCS("libraryReserveIndices"), 0));
        
        m_componentFactory = ComponentFactory::Create();

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        // Common
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Name>("Common");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<EventRetranslator>("Common");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SerializationIndex>("Common");

        // 2D
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Transform2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Bounds2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SizePolicy2D>("2D");

        // 3D
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Transform3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Rotor3D>("3D/Utils");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<LinearMovement3D>("3D/Utils");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SinMovement3D>("3D/Utils");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<AwakeRandomRotation3D>("3D/Utils");

        EntitySerializationManager::Initialize(m_entitySerializationManager);
        if (!m_entitySerializationManager)
            return false;

        EntityPrefabManager::Initialize(m_entityPrefabManager);
        if (!m_entityPrefabManager)
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
        MAZE_PROFILE_EVENT("EntityManager::update");
        m_defaultWorld->update(_dt);
    }

} // namespace Maze
//////////////////////////////////////////
