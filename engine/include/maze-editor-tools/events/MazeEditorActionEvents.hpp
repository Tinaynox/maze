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
#if (!defined(_MazeEditorActionEvents_hpp_))
#define _MazeEditorActionEvents_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditoActionMetaInstancePropertyWillBeChanged
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorActionMetaInstancePropertyWillBeReverted
        : public GenericEvent<EditorActionMetaInstancePropertyWillBeReverted>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorActionMetaInstancePropertyWillBeReverted, Event);

    public:

        //////////////////////////////////////////
        inline EditorActionMetaInstancePropertyWillBeReverted(
            MetaInstance _changedMetaInstance = MetaInstance(),
            MetaProperty const* _changedMetaProperty = nullptr)
            : m_changedMetaInstance(_changedMetaInstance)
            , m_changedMetaProperty(_changedMetaProperty)
        {}

        //////////////////////////////////////////
        inline MetaInstance const& getChangedMetaInstance() const { return m_changedMetaInstance; }

        //////////////////////////////////////////
        inline MetaProperty const* getChangedMetaProperty() const { return m_changedMetaProperty; }


    private:
        MetaInstance m_changedMetaInstance;
        MetaProperty const* m_changedMetaProperty;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorActionEvents_hpp_
//////////////////////////////////////////
