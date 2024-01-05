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
    // Class GenericSimpleComponentSystemHolder
    //
    //////////////////////////////////////////
    template <typename TSimpleComponentSystem>
    class MAZE_CORE_API GenericSimpleComponentSystemHolder
    {
    protected:

        //////////////////////////////////////////
        static Set<GenericSimpleComponentSystemHolder*>& GetSimpleSystemHolders()
        {
            //////////////////////////////////////////
            static Set<GenericSimpleComponentSystemHolder*> s_simpleSystemHolders;
            return s_simpleSystemHolders;
        }

    public:

        //////////////////////////////////////////
        static inline void Attach(ECSWorld* _world)
        {
            for (GenericSimpleComponentSystemHolder* holder : GetSimpleSystemHolders())
                holder->attach(_world);
        }

        //////////////////////////////////////////
        static inline void Detach(ECSWorld* _world)
        {
            for (GenericSimpleComponentSystemHolder* holder : GetSimpleSystemHolders())
                holder->detach(_world);
        }

        //////////////////////////////////////////
        using AddSystemFunc = SharedPtr<TSimpleComponentSystem>(ECSWorld::*)(HashedCString, typename TSimpleComponentSystem::Func, S32);

        //////////////////////////////////////////
        template<typename ...TComponents>
        inline GenericSimpleComponentSystemHolder(
            HashedCString _name,
            SimpleComponentSystem::Func _func,
            S32 _order = 0)
            : m_name(_name)
            , m_func(_func)
            , m_order(_order)
        {

        }

        //////////////////////////////////////////
        inline ~GenericSimpleComponentSystemHolder()
        {
            GetSimpleSystemHolders().erase(this);
        }
        
        //////////////////////////////////////////
        inline void attach(ECSWorld* _world)
        {
            m_system = (_world->*m_addSystemFunc)(m_name, m_func, m_order);
        }

        //////////////////////////////////////////
        virtual void detach(ECSWorld* _world) MAZE_ABSTRACT;

    protected:
        HashedCString m_name;
        typename TSimpleComponentSystem::Func m_func;
        S32 m_order;
        AddSystemFunc m_addSystemFunc;
        WeakPtr<TSimpleComponentSystem> m_system;
    };


    //////////////////////////////////////////
    // Class SimpleComponentSystemHolder
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SimpleComponentSystemHolder
        : public GenericSimpleComponentSystemHolder<SimpleComponentSystem>
    {
    public:

        //////////////////////////////////////////
        using BaseClass = GenericSimpleComponentSystemHolder<SimpleComponentSystem>;


        //////////////////////////////////////////
        template<typename ...TComponents>
        inline SimpleComponentSystemHolder(
            HashedCString _name,
            void(*_func)(F32, Entity*, TComponents* ...),
            S32 _order = 0)
            : BaseClass(_name, (SimpleComponentSystem::Func)_func, _order)
        {
            auto address = &ECSWorld::addSystem<TComponents...>;
            m_addSystemFunc = (AddSystemFunc)(address);

            GetSimpleSystemHolders().insert(this);
        }

        //////////////////////////////////////////
        virtual void detach(ECSWorld* _world) MAZE_OVERRIDE
        {
            _world->removeSystem(m_system.lock());
        }
    };


    //////////////////////////////////////////
    // Class SimpleComponentSystemHolder
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SimpleComponentSystemEventHandlerHolder
        : public GenericSimpleComponentSystemHolder<SimpleComponentSystemEventHandler>
    {
    public:

        //////////////////////////////////////////
        using BaseClass = GenericSimpleComponentSystemHolder<SimpleComponentSystemEventHandler>;


        //////////////////////////////////////////
        template<typename TEventType, typename ...TComponents>
        inline SimpleComponentSystemEventHandlerHolder(
            HashedCString _name,
            void(*_func)(TEventType*, Entity*, TComponents* ...),
            S32 _order = 0)
            : BaseClass(_name, (SimpleComponentSystem::Func)_func, _order)
        {
            auto address = &ECSWorld::addSystemEventHandler<TEventType, TComponents...>;
            m_addSystemFunc = (AddSystemFunc)(address);

            GetSimpleSystemHolders().insert(this);
        }

        //////////////////////////////////////////
        virtual void detach(ECSWorld* _world) MAZE_OVERRIDE
        {
            _world->removeSystemEventHandler(m_system.lock());
        }
    };


    //////////////////////////////////////////
    #define SIMPLE_COMPONENT_SYSTEM(TName, TOrder, ...) \
        void TName(__VA_ARGS__); \
        static SimpleComponentSystemHolder TName##_holder(MAZE_HS(#TName), TName, TOrder); \
        void TName(__VA_ARGS__)


    //////////////////////////////////////////
    #define SIMPLE_COMPONENT_SYSTEM_EVENT_HANDLER(TName, TOrder, ...) \
        void TName(__VA_ARGS__); \
        static SimpleComponentSystemEventHandlerHolder TName##_holder(MAZE_HS(#TName), TName, TOrder); \
        void TName(__VA_ARGS__)
        

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentSystemHolder_hpp_
//////////////////////////////////////////
