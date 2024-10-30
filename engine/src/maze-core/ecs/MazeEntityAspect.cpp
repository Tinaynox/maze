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
        EntityAspectType _type, Vector<ComponentId> const& _componentIds)
        : m_type(_type)
        , m_componentIds(_componentIds)
    {
        updateComponentsMask();
    }

    //////////////////////////////////////////
    EntityAspect::~EntityAspect()
    {

    }

    //////////////////////////////////////////
    bool EntityAspect::hasIntersection(Entity* _entity) const
    {
        switch (m_type)
        {
            case EntityAspectType::HaveAllOfComponents:
            {
                if (!(_entity->getComponentsMask() & m_componentsMask))
                    return false;                

                /*
                // Unstrict order
                return std::includes(
                    _entity->getComponentIds().begin(),
                    _entity->getComponentIds().end(),
                    getComponentIds().begin(),
                    getComponentIds().end());
                */

                // Strict order
                for (Vector<ComponentId>::const_iterator it = getComponentIds().begin(),
                                                           end = getComponentIds().end();
                                                           it != end;
                                                           ++it)
                {                    if (_entity->getComponentIds().find((*it)) == _entity->getComponentIds().end())
                        return false;
                }
            }

            case EntityAspectType::HaveAnyOfComponents:
            {
                for (Vector<ComponentId>::const_iterator it = getComponentIds().begin(),
                                                      end = getComponentIds().end();
                                                      it != end;
                                                      ++it)
                {
                    if (_entity->getComponentIds().find((*it)) != _entity->getComponentIds().end())
                        return true;
                }

                return false;
            }

            case EntityAspectType::ExcludeOfComponents:
            {
                return !std::includes(
                    _entity->getComponentIds().begin(),
                    _entity->getComponentIds().end(),
                    getComponentIds().begin(),
                    getComponentIds().end());
            }

            default:
                break;
        }

        return false;
    }

    
} // namespace Maze
//////////////////////////////////////////
