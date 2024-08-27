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
#include "maze-editor-tools/editor-actions/MazeEditorActionCustom.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorActionCustom
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorActionCustom, EditorAction);
 
    //////////////////////////////////////////
    EditorActionCustom::EditorActionCustom()
    {

    }
     
    //////////////////////////////////////////
    EditorActionCustom::~EditorActionCustom()
    {

    }

    //////////////////////////////////////////
    EditorActionCustomPtr EditorActionCustom::Create(
        std::function<void()> const& _applyFunction,
        std::function<void()> const& _revertFunction)
    {
        EditorActionCustomPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorActionCustom, object, init(_applyFunction, _revertFunction));
        return object;
    }

    //////////////////////////////////////////
    bool EditorActionCustom::init(
        std::function<void()> const& _applyFunction,
        std::function<void()> const& _revertFunction)
    {
        m_applyFunction = _applyFunction;
        m_revertFunction = _revertFunction;

        return true;
    }


    //////////////////////////////////////////
    void EditorActionCustom::applyImpl()
    {
        m_applyFunction();
    }

    //////////////////////////////////////////
    void EditorActionCustom::revertImpl()
    {
        m_revertFunction();
    }


} // namespace Maze
//////////////////////////////////////////
