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
#if (!defined(_MazeEditorActionMetaInstancesSetString_hpp_))
#define _MazeEditorActionMetaInstancesSetString_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorActionMetaInstancesSetString);


    //////////////////////////////////////////
    // Class EditorActionMetaInstancesSetString
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionMetaInstancesSetString
        : public EditorAction
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionMetaInstancesSetString, EditorAction);

    public:

        //////////////////////////////////////////
        virtual ~EditorActionMetaInstancesSetString();

        //////////////////////////////////////////
        static EditorActionMetaInstancesSetStringPtr Create(
            MetaProperty const* _metaProperty,
            MetaInstance const& _metaInstance,
            String const& _newValue);

        //////////////////////////////////////////
        static EditorActionMetaInstancesSetStringPtr Create(
            MetaProperty const* _metaProperty,
            Vector<MetaInstance> const& _metaInstances,
            String const& _newValue);


    protected:

        //////////////////////////////////////////
        EditorActionMetaInstancesSetString();

        //////////////////////////////////////////
        bool init(
            MetaProperty const* _metaProperty,
            MetaInstance const& _metaInstance,
            String const& _newValue);

        //////////////////////////////////////////
        bool init(
            MetaProperty const* _metaProperty,
            Vector<MetaInstance> const& _metaInstances,
            String const& _newValue);


        ////////////////////////////////////
        virtual void applyImpl() MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_OVERRIDE;

    protected:
        MetaProperty const* m_metaProperty = nullptr;
        Vector<MetaInstance> m_metaInstances;
        Vector<String> m_prevValues;
        String m_newValue;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionMetaInstancesSetString_hpp_
//////////////////////////////////////////
