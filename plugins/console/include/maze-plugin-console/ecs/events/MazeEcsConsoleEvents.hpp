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
#if (!defined(_MazeEcsConsoleEvents_hpp_))
#define _MazeEcsConsoleEvents_hpp_


//////////////////////////////////////////
#include "maze-plugin-console/MazeConsoleHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EcsConsoleCommandEvent
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CONSOLE_API EcsConsoleCommandEvent
        : public GenericEvent<EcsConsoleCommandEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EcsConsoleCommandEvent, Event);

    public:

        //////////////////////////////////////////
        inline EcsConsoleCommandEvent(
            HashedCString const _command,
            String const* _argv,
            S32 const _argc)
            : command(_command)
            , argv(_argv)
            , argc(_argc)
        {}


    public:
        HashedCString const command;
        String const* argv = nullptr;
        S32 const argc = 0;
        bool processed = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsConsoleEvents_hpp_
//////////////////////////////////////////
