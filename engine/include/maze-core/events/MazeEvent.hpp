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
#if (!defined(_MazeEvent_hpp_))
#define _MazeEvent_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Event
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Event
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(Event);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Event);

    public:

        //////////////////////////////////////////
        virtual ~Event();

        //////////////////////////////////////////
        inline void setCaptured(bool _captured) { m_captured = _captured; }

        //////////////////////////////////////////
        inline bool getCaptured() const { return m_captured; }

        //////////////////////////////////////////
        inline void capture() { m_captured = true; }

        //////////////////////////////////////////
        template <typename UEvent>
        inline UEvent* castRaw()
        {
            return static_cast<UEvent*>(this);
        }

        //////////////////////////////////////////
        template <typename UEvent>
        inline UEvent* safeCastRaw()
        {
            MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
                !getMetaClass()->isInheritedFrom<UEvent>(),
                nullptr,
                "Incompatible cast '%s' to '%s'!",
                getClassName(),
                ClassInfo<UEvent>::Name());

            return static_cast<UEvent*>(this);
        }

    protected:

        //////////////////////////////////////////
        Event();

    protected:
        bool m_captured;
    };


    //////////////////////////////////////////
    // Class GenericEvent
    //
    //////////////////////////////////////////
    template <typename TEvent>
    class GenericEvent
        : public Event
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TEvent);

    public:

        //////////////////////////////////////////
        virtual ~GenericEvent() {}

    protected:

        //////////////////////////////////////////
        GenericEvent() {}
    };
    
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeEvent_hpp_
