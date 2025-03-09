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
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(CSharpAssemblyLoadedEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(CSharpCoreAssemblyLoadedEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(CSharpAppAssemblyLoadedEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(CSharpEditorAssemblyLoadedEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(MonoPreShutdownEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(MonoInitializationEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(MonoShutdownEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(MonoPreReloadEvent);
    MAZE_IMPLEMENT_SIMPLE_GENERIC_EVENT(MonoReloadEvent);


} // namespace Maze
//////////////////////////////////////////
