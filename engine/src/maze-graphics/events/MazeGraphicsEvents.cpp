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
#include "maze-graphics/events/MazeGraphicsEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TextureDestroyedEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderDestroyedEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderBufferDestroyedEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderWindowWillCloseEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, renderWindowResourceId, -1, getRenderWindowResourceId, setRenderWindowResourceId));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(GlobalShaderUniformAddedEvent, Event);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(GlobalShaderUniformChangedEvent, Event);

} // namespace Maze
//////////////////////////////////////////
