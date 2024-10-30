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
#if (!defined(_MazeCustomComponentSystemHolder_hpp_))
#define _MazeCustomComponentSystemHolder_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class CustomComponentSystemHolder
    //
    //////////////////////////////////////////
    class MAZE_CORE_API CustomComponentSystemHolder
    {
    protected:

        //////////////////////////////////////////
        static Set<CustomComponentSystemHolder*>& GetSystemHolders()
        {
            //////////////////////////////////////////
            static Set<CustomComponentSystemHolder*> s_systemHolders;
            return s_systemHolders;
        }

    public:

        //////////////////////////////////////////
        static inline void Attach(EcsWorld* _world)
        {
            for (CustomComponentSystemHolder* holder : GetSystemHolders())
                holder->attach(_world);
        }

        //////////////////////////////////////////
        static inline void Detach(EcsWorld* _world)
        {
            for (CustomComponentSystemHolder* holder : GetSystemHolders())
                holder->detach(_world);
        }


        //////////////////////////////////////////
        inline CustomComponentSystemHolder(
            HashedCString _name,
            ClassUID _eventUID,
            std::function<IEntitiesSamplePtr(EcsWorld*)> const& _createSampleFunc,
            ComponentSystemEventHandler::Func _func,
            Set<HashedString> _tags = Set<HashedString>(),
            ComponentSystemOrder const& _order = ComponentSystemOrder())
            : m_name(_name)
            , m_eventUID(_eventUID)
            , m_createSampleFunc(_createSampleFunc)
            , m_func(_func)
            , m_tags(_tags)
            , m_order(_order)
        {
            GetSystemHolders().insert(this);
        }


        //////////////////////////////////////////
        inline ~CustomComponentSystemHolder()
        {
            GetSystemHolders().erase(this);
        }
        
        //////////////////////////////////////////
        inline void attach(EcsWorld* _world)
        {
            ComponentSystemEventHandlerPtr system = ComponentSystemEventHandler::Create(
                _world,
                m_name,
                m_eventUID,
                m_createSampleFunc(_world),
                m_func,
                m_tags,
                m_order);

            _world->addSystemEventHandler(system);
            m_systems[_world] = system;
        }

        //////////////////////////////////////////
        inline void detach(EcsWorld* _world)
        {
            _world->removeSystemEventHandler(m_systems[_world].lock());
        }

    protected:
        HashedCString m_name;
        ClassUID m_eventUID = 0;
        std::function<IEntitiesSamplePtr(EcsWorld*)> m_createSampleFunc;
        ComponentSystemEventHandler::Func m_func;
        Set<HashedString> m_tags;
        ComponentSystemOrder m_order;

        Map<EcsWorld*, WeakPtr<ComponentSystemEventHandler>> m_systems;
    };

    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCustomComponentSystemHolder_hpp_
//////////////////////////////////////////
