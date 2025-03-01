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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/ecs/events/MazeEcsCSharpEvents.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MonoEvent
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(MonoEvent);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MonoEvent);
    
    //////////////////////////////////////////
    MonoEvent::MonoEvent(MonoObject* _monoEvent)
        : m_monoEvent(_monoEvent)
        , m_gcHandle(_monoEvent ? mono_gchandle_new(m_monoEvent, true) : 0u)
    {
        if (_monoEvent)
        {
            MonoClass* klass = mono_object_get_class(_monoEvent);
            CString className = mono_class_get_name(klass);
            m_eventUID = CalculateClassUID(className);
        }
    }

    //////////////////////////////////////////
    MonoEvent::~MonoEvent()
    {
        if (m_gcHandle != 0u)
        {
            mono_gchandle_free(m_gcHandle);
            m_gcHandle = 0u;
        }
    }


} // namespace Maze
//////////////////////////////////////////
