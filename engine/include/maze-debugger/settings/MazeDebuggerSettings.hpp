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
#if (!defined(_MazeDebuggerSettings_hpp_))
#define _MazeDebuggerSettings_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(DebuggerSettings);

    //////////////////////////////////////////
    // Class DebuggerSettings
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API DebuggerSettings
        : public Settings
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(DebuggerSettings, Settings);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(DebuggerSettings);

    public:

        //////////////////////////////////////////
        DebuggerSettings();

        //////////////////////////////////////////
        virtual ~DebuggerSettings();


        //////////////////////////////////////////
        void setActive(bool _active);

        //////////////////////////////////////////
        inline bool getActive() const { return m_active; }

        //////////////////////////////////////////
        inline void switchActive() { setActive(!getActive()); }


    public:

        //////////////////////////////////////////
        MultiDelegate<bool> eventActiveChanged;

    protected:

        bool m_active;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDebuggerSettings_hpp_
//////////////////////////////////////////
