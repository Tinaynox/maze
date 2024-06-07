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
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/components/MazeName.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(EcsSceneState);

    //////////////////////////////////////////
    // Class EcsScene
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EcsScene);

    //////////////////////////////////////////
    EcsScene::EcsScene()
    {

    }

    //////////////////////////////////////////
    EcsScene::~EcsScene()
    {
        destroyAllEntities();

        EcsWorld* world = m_world;
        if (world)
        {
            world->update(0.0f);
            world->eventOnDestroy.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    bool EcsScene::init()
    {
        m_world = assignWorld();
        MAZE_ERROR_RETURN_VALUE_IF(!m_world, false, "World is null!");
        m_world->eventOnDestroy.subscribe(this, &EcsScene::notifyEcsWorldOnDestroy);

        return true;
    }

    //////////////////////////////////////////
    void EcsScene::setState(EcsSceneState _state)
    {
        if (m_state == _state)
            return;

        EcsSceneState prevState = m_state;

        m_state = _state;

        switch (m_state)
        {
            case EcsSceneState::Active:
            {
                load();
                break;
            }
            default:
                break;
        }

        m_world->broadcastEventImmediate<EcsSceneStateChangedEvent>(m_id, m_state);
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEvent<EcsSceneStateChangedEvent>(m_id, m_state);

        processSceneStateChanged(m_state);
    }

    //////////////////////////////////////////
    void EcsScene::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void EcsScene::processSceneWillBeDestroyed()
    {
        Debug::Log("%s::processSceneWillBeDestroyed started...", static_cast<CString>(getMetaClass()->getName()));

        destroyAllEntities();

        EcsWorld* world = m_world;
        if (world)
        {
            Debug::Log("%s::processSceneWillBeDestroyed world update started", static_cast<CString>(getMetaClass()->getName()));
            world->update(0.0f);
        }

        Debug::Log("%s::processSceneWillBeDestroyed finished.", static_cast<CString>(getMetaClass()->getName()));
    }

    //////////////////////////////////////////
    void EcsScene::destroyAllEntities()
    {
        while (!m_entities.empty())
        {
            Entity* entity =(*m_entities.begin());
            entity->setEcsScene(nullptr);
            entity->removeFromEcsWorld();
        }
    }

    //////////////////////////////////////////
    void EcsScene::destroyAllEntitiesExcept(EntityPtr const& _value)
    {
        while (m_entities.size() > 1)
        {
            Set<Entity*>::iterator it = m_entities.begin();
            if ((*it) == _value.get())
                ++it;

            if (it != m_entities.end())
            {
                Entity* entity = (*it);
                entity->setEcsScene(nullptr);
                entity->removeFromEcsWorld();
            }
        }
    }

    //////////////////////////////////////////
    void EcsScene::load()
    {
        
    }

    //////////////////////////////////////////
    EntityPtr EcsScene::createEntity()
    {
        EntityPtr entity = Entity::Create();
        entity->setEcsScene(this);
        m_world->addEntity(entity);
        
        return entity;
    }

    //////////////////////////////////////////
    EntityPtr EcsScene::createEntity(String const& _name)
    {
        EntityPtr entity = createEntity();
        entity->createComponent<Name>(_name);
        return entity;
    }

    //////////////////////////////////////////
    EntityPtr EcsScene::createEntity(CString _name)
    {
        EntityPtr entity = createEntity();

        if (_name)
            entity->createComponent<Name>(_name);

        return entity;
    }

    //////////////////////////////////////////
    EntityPtr EcsScene::ensureEntity(String const& _name)
    {
        EntityPtr entity = findEntity(_name.c_str());
        if (entity)
            return entity;
        return createEntity(_name);
    }

    //////////////////////////////////////////
    EntityPtr EcsScene::ensureEntity(CString _name)
    {
        EntityPtr entity = findEntity(_name);
        if (entity)
            return entity;
        return createEntity(_name);
    }

    //////////////////////////////////////////
    EntityPtr EcsScene::findEntity(CString _name)
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
    void EcsScene::processEntityAdded(Entity* _entity)
    {
        m_entities.emplace(_entity);
    }

    //////////////////////////////////////////
    void EcsScene::processEntityRemoved(Entity* _entity)
    {
        m_entities.erase(_entity);
    }

    //////////////////////////////////////////
    void EcsScene::makeMainScene()
    {
        SceneManager::GetInstancePtr()->setMainScene(getSharedPtr());
    }

    //////////////////////////////////////////
    EcsWorld* EcsScene::assignWorld()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorldRaw();
    }

    //////////////////////////////////////////
    void EcsScene::notifyEcsWorldOnDestroy(EcsWorld* _world)
    {
        if (_world && m_world == _world)
        {
            m_world->eventOnDestroy.unsubscribe(this);
            m_world = nullptr;
        }
    }

} // namespace Maze
//////////////////////////////////////////
