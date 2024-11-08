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
#if (!defined(_MazeComponentSystemHolder_hpp_))
#define _MazeComponentSystemHolder_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ComponentSystemHolder
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ComponentSystemHolder
    {
    protected:

        //////////////////////////////////////////
        static Set<ComponentSystemHolder*>& GetSystemHolders()
        {
            //////////////////////////////////////////
            static Set<ComponentSystemHolder*> s_systemHolders;
            return s_systemHolders;
        }

        //////////////////////////////////////////
        enum class Type
        {
            Default,
            Global
        };

    public:

        //////////////////////////////////////////
        static inline void Attach(EcsWorld* _world)
        {
            for (ComponentSystemHolder* holder : GetSystemHolders())
                holder->attach(_world);
        }

        //////////////////////////////////////////
        static inline void Detach(EcsWorld* _world)
        {
            for (ComponentSystemHolder* holder : GetSystemHolders())
                holder->detach(_world);
        }

        //////////////////////////////////////////
        using AddSystemFunc = SharedPtr<ComponentSystemEventHandler>(EcsWorld::*)(HashedCString, typename ComponentSystemEventHandler::Func, Set<HashedString> const&, ComponentSystemOrder const&, U8);
        using AddSystemGlobalFunc = SharedPtr<ComponentSystemEventHandler>(EcsWorld::*)(HashedCString, typename ComponentSystemEventHandler::Func, Set<HashedString> const&, ComponentSystemOrder const&);


        //////////////////////////////////////////
        template<typename TEventType, typename ...TComponents>
        inline ComponentSystemHolder(
            HashedCString _name,
            void(*_func)(TEventType&, Entity*, TComponents* ...),
            Set<HashedString> _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder(),
            U8 _sampleFlags = 0)
            : m_name(_name)
            , m_func((ComponentSystemEventHandler::Func)_func)
            , m_tags(_tags)
            , m_order(_order)
            , m_sampleFlags(_sampleFlags)
        {
            m_type = Type::Default;

            auto address = &EcsWorld::addSystemEventHandler<TEventType, TComponents...>;
            m_addSystemFunc = (AddSystemFunc)(address);

            GetSystemHolders().insert(this);
        }

        //////////////////////////////////////////
        template<typename TEventType>
        inline ComponentSystemHolder(
            HashedCString _name,
            void(*_func)(TEventType&),
            Set<HashedString> _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
            : m_name(_name)
            , m_func((ComponentSystemEventHandler::Func)_func)
            , m_tags(_tags)
            , m_order(_order)
        {
            m_type = Type::Global;

            auto address = &EcsWorld::addSystemEventHandlerGlobal<TEventType>;
            m_addSystemGlobalFunc = (AddSystemGlobalFunc)(address);

            GetSystemHolders().insert(this);
        }

        //////////////////////////////////////////
        inline ~ComponentSystemHolder()
        {
            GetSystemHolders().erase(this);
        }
        
        //////////////////////////////////////////
        inline void attach(EcsWorld* _world)
        {
            switch (m_type)
            {
                case Type::Default:
                {
                    m_systems[_world] = (_world->*m_addSystemFunc)(m_name, m_func, m_tags, m_order, m_sampleFlags);
                    break;
                }
                case Type::Global:
                {
                    m_systems[_world] = (_world->*m_addSystemGlobalFunc)(m_name, m_func, m_tags, m_order);
                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                }
            }
            
        }

        //////////////////////////////////////////
        inline void detach(EcsWorld* _world)
        {
            _world->removeSystemEventHandler(m_systems[_world].lock());
        }

    protected:
        Type m_type = Type::Default;
        HashedCString m_name;
        Set<HashedString> m_tags;
        typename ComponentSystemEventHandler::Func m_func;
        ComponentSystemOrder m_order;
        U8 m_sampleFlags = 0;
        AddSystemFunc m_addSystemFunc;
        AddSystemGlobalFunc m_addSystemGlobalFunc;
        Map<EcsWorld*, WeakPtr<ComponentSystemEventHandler>> m_systems;
    };


    //////////////////////////////////////////
    #define COMPONENT_SYSTEM_EVENT_HANDLER(DName, DTags, DOrder, ...) \
        void DName(__VA_ARGS__); \
        static ComponentSystemHolder DName##_holder(MAZE_HCS(#DName), DName, DTags, DOrder); \
        void DName(__VA_ARGS__)

    //////////////////////////////////////////
    #define COMPONENT_SYSTEM_EVENT_HANDLER_EX(DName, DTags, DOrder, DSampleFlags, ...) \
        void DName(__VA_ARGS__); \
        static ComponentSystemHolder DName##_holder(MAZE_HCS(#DName), DName, DTags, DOrder, DSampleFlags); \
        void DName(__VA_ARGS__)
        

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentSystemHolder_hpp_
//////////////////////////////////////////
