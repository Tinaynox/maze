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
#if (!defined(_MazeCSharpEvents_hpp_))
#define _MazeCSharpEvents_hpp_


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
    MAZE_USING_SHARED_PTR(ScriptClass);
    MAZE_USING_SHARED_PTR(ScriptProperty);
    MAZE_USING_SHARED_PTR(ScriptField);


    //////////////////////////////////////////
    // Class CSharpCoreAssemblyLoadedEvent
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API CSharpCoreAssemblyLoadedEvent
        : public GenericEvent<CSharpCoreAssemblyLoadedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(CSharpCoreAssemblyLoadedEvent, Event);
    };


    //////////////////////////////////////////
    // Class CSharpAppAssemblyLoadedEvent
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API CSharpAppAssemblyLoadedEvent
        : public GenericEvent<CSharpAppAssemblyLoadedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(CSharpAppAssemblyLoadedEvent, Event);
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCSharpEvents_hpp_
//////////////////////////////////////////