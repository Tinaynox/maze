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
        EntityAspectType _type, Vector<ClassUID> const& _componentUIDs)
        : m_type(_type)
        , m_componentUIDs(_componentUIDs)
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
                    _entity->getComponentUIDs().begin(),
                    _entity->getComponentUIDs().end(),
                    getComponentUIDs().begin(),
                    getComponentUIDs().end());
                */

                // Strict order
                for (Vector<ClassUID>::const_iterator it = getComponentUIDs().begin(),
                                                           end = getComponentUIDs().end();
                                                           it != end;
                                                           ++it)
                {                    if (_entity->getComponentUIDs().find((*it)) == _entity->getComponentUIDs().end())
                        return false;
                }
            }

            case EntityAspectType::HaveAnyOfComponents:
            {
                for (Vector<ClassUID>::const_iterator it = getComponentUIDs().begin(),
                                                      end = getComponentUIDs().end();
                                                      it != end;
                                                      ++it)
                {
                    if (_entity->getComponentUIDs().find((*it)) != _entity->getComponentUIDs().end())
                        return true;
                }

                return false;
            }

            case EntityAspectType::ExcludeOfComponents:
            {
                return !std::includes(
                    _entity->getComponentUIDs().begin(),
                    _entity->getComponentUIDs().end(),
                    getComponentUIDs().begin(),
                    getComponentUIDs().end());
            }

            default:
                break;
        }

        return false;
    }

    
} // namespace Maze
//////////////////////////////////////////
