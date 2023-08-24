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
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class EventManager
    //
    //////////////////////////////////////////
    EventManager* EventManager::s_instance = nullptr;

    //////////////////////////////////////////
    EventManager::EventManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EventManager::~EventManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EventManager::Initialize(EventManagerPtr& _eventManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EventManager, _eventManager, init());
    }

    //////////////////////////////////////////
    bool EventManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void EventManager::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("EventManager::update");

        auto& events = m_events.switchContainer();

        for (auto& eventsMap : events)
        {
            if (eventsMap.second.size() > 0u)
            {
                MultiDelegate<ClassUID, Event*>& eventCallbacks = m_eventCallbacks[eventsMap.first];
                for (auto& event : eventsMap.second)
                    eventCallbacks(eventsMap.first, event.get());
            }

            eventsMap.second.clear();
        }
    }

} // namespace Maze
//////////////////////////////////////////
