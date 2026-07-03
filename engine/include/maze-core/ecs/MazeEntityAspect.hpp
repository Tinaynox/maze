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
#pragma once
#if (!defined(_MazeEntityAspect_hpp_))
#define _MazeEntityAspect_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(EntityAspect);


    //////////////////////////////////////////
    // Class EntityAspect
    //
    // An entity matches the aspect when it has ALL required components
    // and NONE of the forbidden ones
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityAspect
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EntityAspect)

    public:

        //////////////////////////////////////////
        EntityAspect(
            Vector<ComponentId> const& _requiredComponentIds = Vector<ComponentId>(),
            Vector<ComponentId> const& _forbiddenComponentIds = Vector<ComponentId>());

        //////////////////////////////////////////
        virtual ~EntityAspect();

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect Require(
            Vector<ComponentId> const& _forbiddenComponentIds = Vector<ComponentId>())
        {
            EntityAspect aspect;
            ExpandComponentIds<TComponents...>(aspect.m_requiredComponentIds);
            aspect.m_forbiddenComponentIds = _forbiddenComponentIds;
            std::sort(aspect.m_forbiddenComponentIds.begin(), aspect.m_forbiddenComponentIds.end());
            aspect.updateComponentsMasks();
            return aspect;
        }

        //////////////////////////////////////////
        // True when the required list matches TComponents exactly (in template order).
        // Forbidden components are not considered
        template <typename ...TComponents>
        inline bool isRequiring() const
        {
            if (sizeof...(TComponents) != m_requiredComponentIds.size())
                return false;

            Vector<ComponentId> const& componentIds = GetComponentIds<TComponents...>();

            return equal(componentIds.begin(), componentIds.end(), m_requiredComponentIds.begin());
        }

        //////////////////////////////////////////
        inline bool operator==(EntityAspect const& _otherAspect) const
        {
            if (m_requiredComponentIds != _otherAspect.m_requiredComponentIds)
                return false;

            if (m_forbiddenComponentIds != _otherAspect.m_forbiddenComponentIds)
                return false;

            return true;
        }

        //////////////////////////////////////////
        Vector<ComponentId> const& getRequiredComponentIds() const { return m_requiredComponentIds; }

        //////////////////////////////////////////
        // Always sorted ascending
        Vector<ComponentId> const& getForbiddenComponentIds() const { return m_forbiddenComponentIds; }

        //////////////////////////////////////////
        bool hasIntersection(Entity* _entity) const;

        //////////////////////////////////////////
        inline bool hasIntersection(EntityPtr const& _entity) const
        {
            return hasIntersection(_entity.get());
        }

        //////////////////////////////////////////
        // Same test against a bare component combination (archetype signature).
        // _sortedComponentIds must be sorted ascending
        bool hasIntersection(
            Vector<ComponentId> const& _sortedComponentIds,
            S64 _componentsMask) const;

        //////////////////////////////////////////
        inline S64 getRequiredComponentsMask() const { return m_requiredComponentsMask; }

        //////////////////////////////////////////
        inline S64 getForbiddenComponentsMask() const { return m_forbiddenComponentsMask; }

    protected:

        //////////////////////////////////////////
        template <typename... TComponents>
        inline static void ExpandComponentIds(Vector<ComponentId>& _result)
        {
            ComponentId ids[] = { GetStaticComponentId<TComponents>()... };
            for (S32 i = 0, n = sizeof(ids) / sizeof(ids[0]); i < n; i++)
                _result.push_back(ids[i]);
        }


        //////////////////////////////////////////
        template <typename ...TComponents>
        inline static Vector<ComponentId> const& GetComponentIds()
        {
            static Vector<ComponentId> s_componentIds;
            if (s_componentIds.empty())
                ExpandComponentIds<TComponents...>(s_componentIds);

            return s_componentIds;
        }

        //////////////////////////////////////////
        inline void updateComponentsMasks()
        {
            m_requiredComponentsMask = 0;
            for (ComponentId componentId : m_requiredComponentIds)
                m_requiredComponentsMask |= (S64)1 << U32(componentId % 64);

            m_forbiddenComponentsMask = 0;
            for (ComponentId componentId : m_forbiddenComponentIds)
                m_forbiddenComponentsMask |= (S64)1 << U32(componentId % 64);
        }

    protected:
        Vector<ComponentId> m_requiredComponentIds;
        Vector<ComponentId> m_forbiddenComponentIds;

        S64 m_requiredComponentsMask = 0;
        S64 m_forbiddenComponentsMask = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntityAspect_hpp_
//////////////////////////////////////////
