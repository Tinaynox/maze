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
#include "maze-core/ecs/MazeEntityAspect.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EntityAspect)


    //////////////////////////////////////////
    // EntityAspect
    //
    //////////////////////////////////////////
    EntityAspect::EntityAspect(
        Vector<ComponentId> const& _requiredComponentIds,
        Vector<ComponentId> const& _forbiddenComponentIds)
        : m_requiredComponentIds(_requiredComponentIds)
        , m_forbiddenComponentIds(_forbiddenComponentIds)
    {
        std::sort(m_forbiddenComponentIds.begin(), m_forbiddenComponentIds.end());
        updateComponentsMasks();
    }

    //////////////////////////////////////////
    EntityAspect::~EntityAspect()
    {

    }

    //////////////////////////////////////////
    bool EntityAspect::hasIntersection(Entity* _entity) const
    {
        S64 componentsMask = _entity->getComponentsMask();

        if (!m_requiredComponentIds.empty())
        {
            if (!(componentsMask & m_requiredComponentsMask))
                return false;

            for (ComponentId componentId : m_requiredComponentIds)
            {
                if (_entity->getComponents().find(componentId) == _entity->getComponents().end())
                    return false;
            }
        }

        if (componentsMask & m_forbiddenComponentsMask)
        {
            for (ComponentId componentId : m_forbiddenComponentIds)
            {
                if (_entity->getComponents().find(componentId) != _entity->getComponents().end())
                    return false;
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool EntityAspect::hasIntersection(
        Vector<ComponentId> const& _sortedComponentIds,
        S64 _componentsMask) const
    {
        if (!m_requiredComponentIds.empty())
        {
            if (!(_componentsMask & m_requiredComponentsMask))
                return false;

            for (ComponentId componentId : m_requiredComponentIds)
            {
                if (!std::binary_search(
                    _sortedComponentIds.begin(),
                    _sortedComponentIds.end(),
                    componentId))
                    return false;
            }
        }

        if (_componentsMask & m_forbiddenComponentsMask)
        {
            for (ComponentId componentId : m_forbiddenComponentIds)
            {
                if (std::binary_search(
                    _sortedComponentIds.begin(),
                    _sortedComponentIds.end(),
                    componentId))
                    return false;
            }
        }

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
