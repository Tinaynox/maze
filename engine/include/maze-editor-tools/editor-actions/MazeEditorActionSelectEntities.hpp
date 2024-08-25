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
#if (!defined(_MazeEditorActionSelectEntities_hpp_))
#define _MazeEditorActionSelectEntities_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorActionSelectEntities);
    MAZE_USING_SHARED_PTR(Entity);


    //////////////////////////////////////////
    // Class EditorActionSelectEntities
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionSelectEntities
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionSelectEntities, EditorAction);

    public:

        //////////////////////////////////////////
        virtual ~EditorActionSelectEntities();

        //////////////////////////////////////////
        static EditorActionSelectEntitiesPtr Create(
            bool _select,
            Vector<EntityPtr> const& _entities,
            bool _clearSelectionList = true);


    protected:

        //////////////////////////////////////////
        EditorActionSelectEntities();

        //////////////////////////////////////////
        bool init(
            bool _select,
            Vector<EntityPtr> const& _entities,
            bool _clearSelectionList);


        ////////////////////////////////////
        virtual void applyImpl() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_OVERRIDE;

    protected:
        Set<EntityPtr> m_prevSelectedEntities;

        bool m_select = true;
        Vector<EntityPtr> m_entities;
        bool m_clearSelectionList = false;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionSelectEntities_hpp_
//////////////////////////////////////////
