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
#include "EditorActionManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "managers/EditorManager.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionManager
    //
    //////////////////////////////////////////
    EditorActionManager* EditorActionManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorActionManager::EditorActionManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorActionManager::~EditorActionManager()
    {
        if (EditorManager::GetInstancePtr())
        {
            EditorManager::GetInstancePtr()->eventSceneModeChanged.unsubscribe(this);
            EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.unsubscribe(this);
        }

        if (EditorPrefabManager::GetInstancePtr())
            EditorPrefabManager::GetInstancePtr()->eventPrefabEntityChanged.unsubscribe(this);

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorActionManager::Initialize(EditorActionManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorActionManager::init()
    {
        EditorManager::GetInstancePtr()->eventSceneModeChanged.subscribe(this, &EditorActionManager::notifyEditorSceneModeChanged);
        EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.subscribe(this, &EditorActionManager::notifyPlaytestModeEnabledChanged);
        EditorPrefabManager::GetInstancePtr()->eventPrefabEntityChanged.subscribe(this, &EditorActionManager::notifyPrefabEntityChanged);

        return true;
    }

    //////////////////////////////////////////
    void EditorActionManager::notifyEditorSceneModeChanged(EditorSceneMode _mode)
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->clearHistory();
    }

    //////////////////////////////////////////
    void EditorActionManager::notifyPrefabEntityChanged(EntityPtr const& _entity)
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->clearHistory();
    }

    //////////////////////////////////////////
    void EditorActionManager::notifyPlaytestModeEnabledChanged(bool _value)
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->clearHistory();
    }



} // namespace Maze
//////////////////////////////////////////
