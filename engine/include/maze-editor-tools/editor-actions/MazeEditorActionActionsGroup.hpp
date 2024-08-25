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
#if (!defined(_MazeEditorActionActionsGroup_hpp_))
#define _MazeEditorActionActionsGroup_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorActionActionsGroup);


    //////////////////////////////////////////
    // Class EditorActionActionsGroup
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionActionsGroup
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionActionsGroup, EditorAction);

    public:

        //////////////////////////////////////////
        virtual ~EditorActionActionsGroup();

        //////////////////////////////////////////
        static EditorActionActionsGroupPtr Create();


        //////////////////////////////////////////
        void addAction(EditorActionPtr const& _action);

        //////////////////////////////////////////
        inline Size getActionsCount() const { return m_actions.size(); }

    protected:

        //////////////////////////////////////////
        EditorActionActionsGroup();

        //////////////////////////////////////////
        bool init();


        ////////////////////////////////////
        virtual void applyImpl() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_OVERRIDE;

    protected:
        Vector<EditorActionPtr> m_actions;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionActionsGroup_hpp_
//////////////////////////////////////////
