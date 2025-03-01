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
#if (!defined(_MazeEcsCSharpEvents_hpp_))
#define _MazeEcsCSharpEvents_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MonoEvent
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API MonoEvent
        : public Event
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MonoEvent, Event);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MonoEvent);

    public:

        //////////////////////////////////////////
        MonoEvent(MonoObject* _monoEvent = nullptr);

        //////////////////////////////////////////
        virtual ~MonoEvent();

        //////////////////////////////////////////
        virtual U32 getEventUID() const MAZE_OVERRIDE { return m_eventUID; }


        //////////////////////////////////////////
        inline MonoObject* getMonoEvent() const { return m_monoEvent; }

    private:
        MonoObject* m_monoEvent = nullptr;
        U32 m_gcHandle = 0u;
        U32 m_eventUID = 0u;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsCSharpEvents_hpp_
//////////////////////////////////////////
