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
#include "EditorUIManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "events/EditorEvents.hpp"
#include "helpers/EditorProjectHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorUIManager
    //
    //////////////////////////////////////////
    EditorUIManager* EditorUIManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorUIManager::EditorUIManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorUIManager::~EditorUIManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorUIManager::Initialize(EditorUIManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorUIManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorUIManager::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void EditorUIManager::addTopBarOption(
        String const& _menuName,
        String const& _option,
        MenuListTree2D::ItemCallback _callback,
        MenuListTree2D::ItemValidateCallback _validate)
    {
        m_topBarMenuData.emplace_back(_menuName, _option, _callback, _validate);
    }


} // namespace Maze
//////////////////////////////////////////
