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
#if (!defined(_EditorEvents_hpp_))
#define _EditorEvents_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorProjectWillBeClosedEvent
    //
    //////////////////////////////////////////
    class EditorProjectWillBeClosedEvent
        : public GenericEvent<EditorProjectWillBeClosedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorProjectWillBeClosedEvent, Event);
    };


    //////////////////////////////////////////
    // Class EditorProjectOpenedEvent
    //
    //////////////////////////////////////////
    class EditorProjectOpenedEvent
        : public GenericEvent<EditorProjectOpenedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorProjectOpenedEvent, Event);
    };


    //////////////////////////////////////////
    // Class PlaytestModePrepareEvent
    //
    //////////////////////////////////////////
        class PlaytestModePrepareEvent
        : public GenericEvent<PlaytestModePrepareEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PlaytestModePrepareEvent, Event);

        //////////////////////////////////////////
        PlaytestModePrepareEvent(bool _playtestModeEnabled)
            : m_playtestModeEnabled(_playtestModeEnabled)
        {}

        //////////////////////////////////////////
        inline void setPlaytestModeEnabled(bool _value) { m_playtestModeEnabled = _value; }

        //////////////////////////////////////////
        inline bool getPlaytestModeEnabled() const { return m_playtestModeEnabled; }

    private:
        bool m_playtestModeEnabled = false;
    };


    //////////////////////////////////////////
    // Class PlaytestModeEnabledChangedEvent
    //
    //////////////////////////////////////////
    class PlaytestModeEnabledChangedEvent
        : public GenericEvent<PlaytestModeEnabledChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PlaytestModeEnabledChangedEvent, Event);

        //////////////////////////////////////////
        PlaytestModeEnabledChangedEvent(bool _playtestModeEnabled)
            : m_playtestModeEnabled(_playtestModeEnabled)
        {}

        //////////////////////////////////////////
        inline void setPlaytestModeEnabled(bool _value) { m_playtestModeEnabled = _value; }

        //////////////////////////////////////////
        inline bool getPlaytestModeEnabled() const { return m_playtestModeEnabled; }

    private:
        bool m_playtestModeEnabled = false;
    };
 

} // namespace Maze
//////////////////////////////////////////


#endif // _EditorEvents_hpp_
//////////////////////////////////////////
