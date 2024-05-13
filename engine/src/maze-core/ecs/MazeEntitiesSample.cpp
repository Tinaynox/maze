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
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class IEntitiesSample
    //
    //////////////////////////////////////////
    IEntitiesSample::IEntitiesSample()
    {
    }

    //////////////////////////////////////////
    IEntitiesSample::~IEntitiesSample()
    {
    }

    //////////////////////////////////////////
    bool IEntitiesSample::init(
        EcsWorldPtr const& _ecs,
        EntityAspect const& _aspect)
    {
        if (!_ecs)
            return false;

        m_world = _ecs;
        m_worldRaw = _ecs.get();
        m_aspect = _aspect;

        return true;
    }


    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EntitiesSample);

    //////////////////////////////////////////
    // Class EntitiesSample
    //
    //////////////////////////////////////////
    EntitiesSample::EntitiesSample()
    {

    }

    //////////////////////////////////////////
    EntitiesSample::~EntitiesSample()
    {

    }

    //////////////////////////////////////////
    bool EntitiesSample::init(
        EcsWorldPtr const& _ecs,
        EntityAspect const& _aspect)
    {
        if (!IEntitiesSample::init(_ecs, _aspect))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void EntitiesSample::clear()
    {
        for (Entity* entity : m_entities)
            eventEntityRemoved(entity);

        m_entities.clear();
    }

    //////////////////////////////////////////
    void EntitiesSample::processEntity(Entity* _entity)
    {
        Vector<Entity*>::iterator it = std::find(
            m_entities.begin(),
            m_entities.end(),
            _entity);

        bool intersects;
        if (!_entity->getActiveInHierarchy() || !_entity->getEcsWorld())
            intersects = false;
        else
            intersects = m_aspect.hasIntersection(_entity);            

        if (it == m_entities.end() && intersects)
        {
            m_entities.push_back(_entity);
            eventEntityAdded(_entity);
        }
        else
        if (it != m_entities.end() && !intersects)
        {
            eventEntityRemoved(_entity);
            m_entities.erase(it);
        }
    }

    //////////////////////////////////////////
    Vector<EntityId> EntitiesSample::getEntityIds() const
    {
        Vector<EntityId> result;
        for (Entity* entity : m_entities)
            result.push_back(entity->getId());

        return result;
    }


} // namespace Maze
//////////////////////////////////////////
