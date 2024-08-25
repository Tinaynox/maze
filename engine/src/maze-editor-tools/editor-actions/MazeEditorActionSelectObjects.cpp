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
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectObjects.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionSelectObjects
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionSelectObjects, EditorAction);
 
    //////////////////////////////////////////
    EditorActionSelectObjects::EditorActionSelectObjects()
    {

    }
     
    //////////////////////////////////////////
    EditorActionSelectObjects::~EditorActionSelectObjects()
    {

    }

    //////////////////////////////////////////
    EditorActionSelectObjectsPtr EditorActionSelectObjects::Create(
        bool _select,
        Vector<ObjectPtr> const& _entities,
        bool _clearSelectionList)
    {
        EditorActionSelectObjectsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionSelectObjects, object, init(_select, _entities, _clearSelectionList));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionSelectObjects::init(
        bool _select,
        Vector<ObjectPtr> const& _entities,
        bool _clearSelectionList)
    {
        m_select = _select;
        m_entities = _entities;
        m_clearSelectionList = _clearSelectionList;

        return true;
    }


    //////////////////////////////////////////
    void EditorActionSelectObjects::applyImpl()
    {
        m_prevSelectedObjects = SelectionManager::GetInstancePtr()->getSelectedObjects();

        if (m_select)
            SelectionManager::GetInstancePtr()->selectObjects(m_entities, m_clearSelectionList);
        else
            SelectionManager::GetInstancePtr()->unselectObjects(m_entities);
    }

    //////////////////////////////////////////
    void EditorActionSelectObjects::revertImpl()
    {
        if (m_select)
        {
            if (m_clearSelectionList)
                SelectionManager::GetInstancePtr()->selectObjects(
                    Vector<ObjectPtr>(m_prevSelectedObjects.begin(), m_prevSelectedObjects.end()),
                    true);
            else
                SelectionManager::GetInstancePtr()->unselectObjects(m_entities);
        }
        else
        {
            SelectionManager::GetInstancePtr()->selectObjects(
                Vector<ObjectPtr>(m_prevSelectedObjects.begin(), m_prevSelectedObjects.end()),
                true);
        }
    }


} // namespace Maze
//////////////////////////////////////////
