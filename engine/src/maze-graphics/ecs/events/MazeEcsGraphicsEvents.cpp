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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Render3DDefaultPrePassEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Render3DDefaultPassGatherRenderUnitsEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Render3DShadowPassGatherRenderUnitsEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Render3DDefaultPostPassEvent, Event);


    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Render2DPostUpdateEvent, Event);
    

    //////////////////////////////////////////
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(SpriteRenderer2DMaterialUpdatedEvent);

    
} // namespace Maze
//////////////////////////////////////////
