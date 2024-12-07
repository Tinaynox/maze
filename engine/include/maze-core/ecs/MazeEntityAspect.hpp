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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(EntityAspect);


    //////////////////////////////////////////
    enum class EntityAspectType
    {
        None = 0,
        HaveAllOfComponents,
        HaveAnyOfComponents,
        ExcludeOfComponents,
        MAX
    };


    //////////////////////////////////////////
    class MAZE_CORE_API EntityAspect
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EntityAspect)

    public:

        //////////////////////////////////////////
        EntityAspect(
            EntityAspectType _type = EntityAspectType::None,
            Vector<ComponentId> const& _componentIds = Vector<ComponentId>());

        //////////////////////////////////////////
        virtual ~EntityAspect();

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect Construct(EntityAspectType _type)
        {
            EntityAspect aspect(_type, Vector<ComponentId>());
            ExpandComponentIds<TComponents...>(aspect.m_componentIds);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect HaveAllOfComponents()
        {
            EntityAspect aspect(EntityAspectType::HaveAllOfComponents, Vector<ComponentId>());
            ExpandComponentIds<TComponents...>(aspect.m_componentIds);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect HaveAnyOfComponents()
        {
            EntityAspect aspect(EntityAspectType::HaveAnyOfComponents, Vector<ComponentId>());
            ExpandComponentIds<TComponents...>(aspect.m_componentIds);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect ExcludeOfComponents()
        {
            EntityAspect aspect(EntityAspectType::ExcludeOfComponents, Vector<ComponentId>());
            ExpandComponentIds< TComponents... >(aspect.m_componentIds);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        inline bool isAspect(EntityAspectType _type) const
        {
            if (m_type != _type)
                return false;

            if (sizeof...(TComponents) != m_componentIds.size())
                return false;

            Vector<ComponentId> const& componentIds = GetComponentIds<TComponents...>();

            return equal(componentIds.begin(), componentIds.end(), m_componentIds.begin());
        }

        //////////////////////////////////////////
        inline bool operator==(EntityAspect const& _otherAspect) const
        {
            if (m_type != _otherAspect.m_type)
                return false;

            if (m_componentIds != _otherAspect.m_componentIds)
                return false;

            return true;
        }

        //////////////////////////////////////////
        EntityAspectType const& getType() const { return m_type; }

        //////////////////////////////////////////
        Vector<ComponentId> const& getComponentIds() const { return m_componentIds; }

        //////////////////////////////////////////
        bool hasIntersection(Entity* _entity) const;

        //////////////////////////////////////////
        inline bool hasIntersection(EntityPtr const& _entity) const
        {
            return hasIntersection(_entity.get());
        }

        //////////////////////////////////////////
        inline S64 getComponentsMask() const { return m_componentsMask; }

    protected:

        //////////////////////////////////////////
        template <typename... TComponents>
        inline static void ExpandComponentIds(Vector<ComponentId>& _result)
        {
#if 1
            ComponentId ids[] = { GetStaticComponentId<TComponents>()... };
            for (S32 i = 0, n = sizeof(ids) / sizeof(ids[0]); i < n; i++)
                _result.push_back(ids[i]);
#elif 0     
            auto expand = [&_result](ComponentId id) { _result.push_back(id); };
            (void)std::initializer_list<int>{(expand(GetStaticComponentId<TComponents>()), 0)...};
#else
            MAZE_NOT_IMPLEMENTED
#endif
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
        inline void updateComponentsMask()
        {
            m_componentsMask = 0;
            for (ComponentId componentId : m_componentIds)
                m_componentsMask |= (S64)1 << U32(componentId % 64);
        }

    protected:
        EntityAspectType m_type;
        Vector<ComponentId> m_componentIds;

        S64 m_componentsMask;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntityAspect_hpp_
//////////////////////////////////////////
