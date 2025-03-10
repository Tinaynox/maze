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
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PreUpdateEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UpdateEvent, Event);    
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(PostUpdateEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityAddedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityRemovedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityAddedToSampleEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityRemovedFromSampleEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityActiveChangedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EcsEntityActiveChangedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EcsSceneStateChangedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EcsChildRemovedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EcsChildAddedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EcsWorldWillBeDestroyedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntityParentChangedEvent, Event);

    
} // namespace Maze
//////////////////////////////////////////
