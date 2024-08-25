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
#include "maze-editor-tools/editor-actions/MazeEditorActionActionsGroup.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionActionsGroup
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionActionsGroup, EditorAction);
 
    //////////////////////////////////////////
    EditorActionActionsGroup::EditorActionActionsGroup()
    {

    }
     
    //////////////////////////////////////////
    EditorActionActionsGroup::~EditorActionActionsGroup()
    {

    }

    //////////////////////////////////////////
    EditorActionActionsGroupPtr EditorActionActionsGroup::Create()
    {
        EditorActionActionsGroupPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionActionsGroup, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionActionsGroup::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void EditorActionActionsGroup::addAction(EditorActionPtr const& _action)
    {
        MAZE_DEBUG_BP_RETURN_IF(std::find(m_actions.begin(), m_actions.end(), _action) != m_actions.end());

        m_actions.push_back(_action);
    }

    //////////////////////////////////////////
    void EditorActionActionsGroup::applyImpl()
    {
        for (Size i = 0, in = m_actions.size(); i < in; ++i)
            m_actions[i]->apply();
    }

    //////////////////////////////////////////
    void EditorActionActionsGroup::revertImpl()
    {
        for (S32 i = (S32)m_actions.size() - 1; i >= 0; --i)
            m_actions[i]->revert();
    }


} // namespace Maze
//////////////////////////////////////////
