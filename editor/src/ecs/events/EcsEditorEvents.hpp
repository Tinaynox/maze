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
#if (!defined(_EcsEditorEvents_hpp_))
#define _EcsEditorEvents_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class OpenEditorEvent
    //
    //////////////////////////////////////////
    class OpenEditorEvent
        : public GenericEvent<OpenEditorEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(OpenEditorEvent, Event);

    public:

        //////////////////////////////////////////
        inline OpenEditorEvent(
            String const& _editorName = String())
            : m_editorName(_editorName)
        {}

        //////////////////////////////////////////
        inline void setEditorName(String const& _editorName) { m_editorName = _editorName; }

        //////////////////////////////////////////
        inline String const& getEditorName() const { return m_editorName; }

    private:
        String m_editorName;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EcsEditorEvents_hpp_
//////////////////////////////////////////
