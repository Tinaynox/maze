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
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/components/MazeName.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ECSSceneState);

    //////////////////////////////////////////
    // Class ECSScene
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ECSScene);

    //////////////////////////////////////////
    ECSScene::ECSScene()
        : m_state(ECSSceneState::None)
        , m_flags(0)
        , m_world(nullptr)
    {

    }

    //////////////////////////////////////////
    ECSScene::~ECSScene()
    {
        destroyAllEntities();
        removeAllSystems();

        ECSWorld* world = m_world;
        if (world)
            world->update(0.0f);
    }

    //////////////////////////////////////////
    bool ECSScene::init()
    {
        m_world = assignWorld();
        MAZE_ERROR_RETURN_VALUE_IF(!m_world, false, "World is null!");

        return true;
    }

    //////////////////////////////////////////
    void ECSScene::setState(ECSSceneState _state)
    {
        if (m_state == _state)
            return;

        ECSSceneState prevState = m_state;

        m_state = _state;

        switch (m_state)
        {
            case ECSSceneState::Created:
            {
                break;
            }
            case ECSSceneState::Active:
            {
                load();
                break;
            }
            case ECSSceneState::Paused:
            {
                break;
            }
            case ECSSceneState::Destroy:
            {
                break;
            }
            default:
            {
                break;
            }
        }

        processSceneStateChanged(m_state);
    }

    //////////////////////////////////////////
    void ECSScene::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void ECSScene::processSceneWillBeDestroyed()
    {
        destroyAllEntities();
        removeAllSystems();

        ECSWorld* world = m_world;
        if (world)
            world->update(0.0f);
    }

    //////////////////////////////////////////
    void ECSScene::destroyAllEntities()
    {
        while (!m_entities.empty())
        {
            Entity* entity =(*m_entities.begin());
            entity->setECSScene(nullptr);
            entity->removeFromECSWorld();
        }
    }

    //////////////////////////////////////////
    void ECSScene::destroyAllEntitiesExcept(EntityPtr const& _value)
    {
        while (m_entities.size() > 1)
        {
            Set<Entity*>::iterator it = m_entities.begin();
            if ((*it) == _value.get())
                ++it;

            if (it != m_entities.end())
            {
                Entity* entity = (*it);
                entity->setECSScene(nullptr);
                entity->removeFromECSWorld();
            }
        }
    }

    //////////////////////////////////////////
    void ECSScene::addSystem(ComponentSystemPtr const& _system)
    {
        ECSWorld* world = m_world;

        m_systems.push_back(_system);
        world->addSystem(_system);
    }

    //////////////////////////////////////////
    void ECSScene::removeSystem(ComponentSystemPtr const& _system)
    {
        ECSWorld* world = m_world;

        auto systemIt = std::find(m_systems.begin(), m_systems.end(), _system);
        if (systemIt != m_systems.end())
            m_systems.erase(systemIt);

        world->removeSystem(_system);
    }

    //////////////////////////////////////////
    void ECSScene::removeAllSystems()
    {
        ECSWorld* world = m_world;

        for (ComponentSystemPtr const& system : m_systems)
            world->removeSystem(system);

        m_systems.clear();
    }

    //////////////////////////////////////////
    void ECSScene::load()
    {
        
    }

    //////////////////////////////////////////
    EntityPtr ECSScene::createEntity()
    {
        EntityPtr entity = Entity::Create();
        entity->setECSScene(this);

        m_world->addEntity(entity);

        return entity;
    }

    //////////////////////////////////////////
    EntityPtr ECSScene::createEntity(String const& _name)
    {
        EntityPtr entity = createEntity();
        entity->createComponent<Name>(_name);
        return entity;
    }

    //////////////////////////////////////////
    EntityPtr ECSScene::createEntity(CString _name)
    {
        EntityPtr entity = createEntity();

        if (_name)
            entity->createComponent<Name>(_name);

        return entity;
    }

    //////////////////////////////////////////
    EntityPtr ECSScene::ensureEntity(String const& _name)
    {
        EntityPtr entity = findEntity(_name.c_str());
        if (entity)
            return entity;
        return createEntity(_name);
    }

    //////////////////////////////////////////
    EntityPtr ECSScene::ensureEntity(CString _name)
    {
        EntityPtr entity = findEntity(_name);
        if (entity)
            return entity;
        return createEntity(_name);
    }

    //////////////////////////////////////////
    EntityPtr ECSScene::findEntity(CString _name)
    {
        if (!_name)
            return nullptr;

        for (auto it = m_entities.begin(), end = m_entities.end(); it != end; ++it)
        {
            Name* component = (*it)->getComponentRaw<Name>();
            if (component && component->getName() == _name)
                return (*it)->getSharedPtr();
        }

        return nullptr;
    }

    //////////////////////////////////////////
    void ECSScene::processEntityAdded(Entity* _entity)
    {
        m_entities.emplace(_entity);
    }

    //////////////////////////////////////////
    void ECSScene::processEntityRemoved(Entity* _entity)
    {
        m_entities.erase(_entity);
    }

    //////////////////////////////////////////
    void ECSScene::makeMainScene()
    {
        SceneManager::GetInstancePtr()->setMainScene(getSharedPtr());
    }

    //////////////////////////////////////////
    ECSWorld* ECSScene::assignWorld()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorldRaw();
    }

} // namespace Maze
//////////////////////////////////////////
