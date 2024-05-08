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
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SimpleComponentSystemHolder
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SimpleComponentSystemHolder
    {
    protected:

        //////////////////////////////////////////
        static Set<SimpleComponentSystemHolder*>& GetSimpleSystemHolders()
        {
            //////////////////////////////////////////
            static Set<SimpleComponentSystemHolder*> s_simpleSystemHolders;
            return s_simpleSystemHolders;
        }

    public:

        //////////////////////////////////////////
        static inline void Attach(ECSWorld* _world)
        {
            for (SimpleComponentSystemHolder* holder : GetSimpleSystemHolders())
                holder->attach(_world);
        }

        //////////////////////////////////////////
        static inline void Detach(ECSWorld* _world)
        {
            for (SimpleComponentSystemHolder* holder : GetSimpleSystemHolders())
                holder->detach(_world);
        }

        //////////////////////////////////////////
        using AddSystemFunc = SharedPtr<SimpleComponentSystemEventHandler>(ECSWorld::*)(HashedCString, typename SimpleComponentSystemEventHandler::Func, ComponentSystemOrder const&);


        //////////////////////////////////////////
        template<typename TEventType, typename ...TComponents>
        inline SimpleComponentSystemHolder(
            HashedCString _name,
            void(*_func)(TEventType&, Entity*, TComponents* ...),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
            : m_name(_name)
            , m_func((SimpleComponentSystemEventHandler::Func)_func)
            , m_order(_order)
        {
            auto address = &ECSWorld::addSystemEventHandler<TEventType, TComponents...>;
            m_addSystemFunc = (AddSystemFunc)(address);

            GetSimpleSystemHolders().insert(this);
        }

        //////////////////////////////////////////
        inline ~SimpleComponentSystemHolder()
        {
            GetSimpleSystemHolders().erase(this);
        }
        
        //////////////////////////////////////////
        inline void attach(ECSWorld* _world)
        {
            m_systems[_world] = (_world->*m_addSystemFunc)(m_name, m_func, m_order);
        }

        //////////////////////////////////////////
        inline void detach(ECSWorld* _world)
        {
            _world->removeSystemEventHandler(m_systems[_world].lock());
        }

    protected:
        HashedCString m_name;
        typename SimpleComponentSystemEventHandler::Func m_func;
        ComponentSystemOrder m_order;
        AddSystemFunc m_addSystemFunc;
        Map<ECSWorld*, WeakPtr<SimpleComponentSystemEventHandler>> m_systems;
    };


    //////////////////////////////////////////
    #define SIMPLE_COMPONENT_SYSTEM_EVENT_HANDLER(TName, TOrder, ...) \
        void TName(__VA_ARGS__); \
        static SimpleComponentSystemHolder TName##_holder(MAZE_HCS(#TName), TName, TOrder); \
        void TName(__VA_ARGS__)
        

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentSystemHolder_hpp_
//////////////////////////////////////////
