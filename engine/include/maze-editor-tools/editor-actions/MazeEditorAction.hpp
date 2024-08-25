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
#if (!defined(_MazeEditorAction_hpp_))
#define _MazeEditorAction_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorAction);


    //////////////////////////////////////////
    // Class EditorAction
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorAction
        : public std::enable_shared_from_this<EditorAction>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EditorAction);

        //////////////////////////////////////////
        static S32 GetCurrentTimestamp();

    public:

        //////////////////////////////////////////
        virtual ~EditorAction();

        

        //////////////////////////////////////////
        bool apply();

        //////////////////////////////////////////
        bool revert();


        //////////////////////////////////////////
        inline bool getApplied() const { return m_applied; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline S32 getTimestamp() const { return m_timestamp; }


        //////////////////////////////////////////
        template <typename TEditorAction>
        SharedPtr<TEditorAction> castSafe()
        {
            if (getClassUID() == ClassInfo<TEditorAction>::UID())
                return std::static_pointer_cast<TEditorAction>(shared_from_this());

            return nullptr;
        }

    public:

        //////////////////////////////////////////
        MultiDelegate<bool> eventAppliedChanged;

    protected:

        //////////////////////////////////////////
        EditorAction();

        //////////////////////////////////////////
        void setApplied(bool _applied);

        ////////////////////////////////////
        void resetTimestamp();


        ////////////////////////////////////
        virtual void applyImpl() MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual void revertImpl() MAZE_ABSTRACT;

    protected:
        bool m_applied = false;

        String m_name;
        
        S32 m_timestamp = 0;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorAction_hpp_
//////////////////////////////////////////
