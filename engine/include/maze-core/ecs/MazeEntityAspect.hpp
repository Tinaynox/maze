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
#include "maze-core/ecs/MazeECSTypes.hpp"
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
            Set<ClassUID> const& _componentUIDs = Set<ClassUID>());

        //////////////////////////////////////////
        virtual ~EntityAspect();

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect Construct(EntityAspectType _type)
        {
            EntityAspect aspect(_type, Set<ClassUID>());
            ExpandComponentUIDs<TComponents...>(aspect.m_componentUIDs);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect HaveAllOfComponents()
        {
            EntityAspect aspect(EntityAspectType::HaveAllOfComponents, Set<ClassUID>());
            ExpandComponentUIDs<TComponents...>(aspect.m_componentUIDs);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect HaveAnyOfComponents()
        {
            EntityAspect aspect(EntityAspectType::HaveAnyOfComponents, Set<ClassUID>());
            ExpandComponentUIDs<TComponents...>(aspect.m_componentUIDs);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        static inline EntityAspect ExcludeOfComponents()
        {
            EntityAspect aspect(EntityAspectType::ExcludeOfComponents, Set<ClassUID>());
            ExpandComponentUIDs< TComponents... >(aspect.m_componentUIDs);
            aspect.updateComponentsMask();
            return aspect;
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        inline bool isAspect(EntityAspectType _type) const
        {
            if (m_type != _type)
                return false;

            if (sizeof...(TComponents) != m_componentUIDs.size())
                return false;

            Set<ClassUID> const& componentUIDs = GetComponentUIDs<TComponents...>();

            return equal(componentUIDs.begin(), componentUIDs.end(), m_componentUIDs.begin());
        }

        //////////////////////////////////////////
        inline bool operator==(EntityAspect const& _otherAspect) const
        {
            if (m_type != _otherAspect.m_type)
                return false;

            if (m_componentUIDs != _otherAspect.m_componentUIDs)
                return false;

            return true;
        }

        //////////////////////////////////////////
        EntityAspectType const& getType() const { return m_type; }

        //////////////////////////////////////////
        Set<ClassUID> const& getComponentUIDs() const { return m_componentUIDs; }

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
        template <typename ...TComponents>
        inline static void ExpandComponentUIDs(Set<ClassUID>& _result)
        {
            ClassUID uids[] = { ClassInfo<TComponents>::UID()... };
            for (S32 i = 0, n = sizeof(uids) / sizeof(uids[0]); i < n; i++)
                _result.insert(uids[i]);
        }

        //////////////////////////////////////////
        template <typename ...TComponents>
        inline static Set<ClassUID> const& GetComponentUIDs()
        {
            static Set<ClassUID> s_componentUIDs;
            if (s_componentUIDs.empty())                
                ExpandComponentUIDs<TComponents...>(s_componentUIDs);
                
            return s_componentUIDs;
        }

        //////////////////////////////////////////
        inline void updateComponentsMask()
        {
            m_componentsMask = 0;
            for (ClassUID componentUID : m_componentUIDs)
                m_componentsMask |= (S64)1 << U32(componentUID % 64);
        }

    protected:
        EntityAspectType m_type;
        Set<ClassUID> m_componentUIDs;

        S64 m_componentsMask;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntityAspect_hpp_
//////////////////////////////////////////
