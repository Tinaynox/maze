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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorAction
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(EditorAction);
    
    //////////////////////////////////////////
    EditorAction::EditorAction()
    {

    }
     
    //////////////////////////////////////////
    EditorAction::~EditorAction()
    {

    }

    //////////////////////////////////////////
    S32 EditorAction::GetCurrentTimestamp()
    {
        return static_cast<S32>(time(0));
    }

    //////////////////////////////////////////
    bool EditorAction::apply()
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(m_applied, false, "Action is already applied!");

        setApplied(true);
        applyImpl();

        return true;
    }

    //////////////////////////////////////////
    bool EditorAction::revert()
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(!m_applied, false, "Action is not applied!");

        setApplied(false);
        revertImpl();

        return true;
    }

    //////////////////////////////////////////
    void EditorAction::setApplied(bool _applied)
    {
        if (m_applied == _applied)
            return;

        m_applied = _applied;

        resetTimestamp();
        eventAppliedChanged(m_applied);
    }

    //////////////////////////////////////////
    void EditorAction::resetTimestamp()
    {
        m_timestamp = GetCurrentTimestamp();
    }

} // namespace Maze
//////////////////////////////////////////
