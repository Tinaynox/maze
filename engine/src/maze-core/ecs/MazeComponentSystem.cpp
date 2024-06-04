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
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ComponentSystemEntityAddedToSampleEventHandler
    //
    //////////////////////////////////////////
    ComponentSystemEntityAddedToSampleEventHandler::ComponentSystemEntityAddedToSampleEventHandler()
    {

    }

    //////////////////////////////////////////
    ComponentSystemEntityAddedToSampleEventHandler::~ComponentSystemEntityAddedToSampleEventHandler()
    {
        m_eventHandler->getSample()->eventEntityAdded.unsubscribe(this);
    }

    //////////////////////////////////////////
    bool ComponentSystemEntityAddedToSampleEventHandler::init(
        EcsWorld* _world,
        ComponentSystemEventHandlerPtr const& _eventHandler)
    {
        m_world = _world;
        m_eventHandler = _eventHandler;

        m_eventHandler->getSample()->eventEntityAdded.subscribe(this, &ComponentSystemEntityAddedToSampleEventHandler::notifyEntityAdded);

        return true;
    }

    //////////////////////////////////////////
    void ComponentSystemEntityAddedToSampleEventHandler::notifyEntityAdded(Entity* _entity)
    {
        m_world->processEntityAddedToSample(m_eventHandler, _entity->getId());
    }
    

    //////////////////////////////////////////
    // Class ComponentSystemEntityRemovedFromSampleEventHandler
    //
    //////////////////////////////////////////
    ComponentSystemEntityRemovedFromSampleEventHandler::ComponentSystemEntityRemovedFromSampleEventHandler()
    {

    }

    //////////////////////////////////////////
    ComponentSystemEntityRemovedFromSampleEventHandler::~ComponentSystemEntityRemovedFromSampleEventHandler()
    {
        m_eventHandler->getSample()->eventEntityWillBeRemoved.unsubscribe(this);
    }

    //////////////////////////////////////////
    bool ComponentSystemEntityRemovedFromSampleEventHandler::init(
        EcsWorld* _world,
        ComponentSystemEventHandlerPtr const& _eventHandler)
    {
        m_world = _world;
        m_eventHandler = _eventHandler;

        m_eventHandler->getSample()->eventEntityWillBeRemoved.subscribe(this, &ComponentSystemEntityRemovedFromSampleEventHandler::notifyEntityWillBeRemoved);

        return true;
    }

    //////////////////////////////////////////
    void ComponentSystemEntityRemovedFromSampleEventHandler::notifyEntityWillBeRemoved(Entity* _entity)
    {
        m_world->processEntityRemovedFromSample(m_eventHandler, _entity->getId());
    }


} // namespace Maze
//////////////////////////////////////////
