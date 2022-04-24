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
#if (!defined(_MazeComponentEditorDefault_hpp_))
#define _MazeComponentEditorDefault_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ComponentEditorDefault);


    //////////////////////////////////////////
    // Class ComponentEditorDefault
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API ComponentEditorDefault
        : public ComponentEditor
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ComponentEditorDefault, ComponentEditor);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ComponentEditorDefault);

    public:

        //////////////////////////////////////////
        virtual ~ComponentEditorDefault();

        //////////////////////////////////////////
        static ComponentEditorDefaultPtr Create(MetaClass* _componentMetaClass);

        //////////////////////////////////////////
        virtual ClassUID getComponentUID() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual MetaClass* getComponentMetaClass() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ComponentEditorDefault();        

        //////////////////////////////////////////
        using ComponentEditor::init;
        
        //////////////////////////////////////////
        virtual bool init(MetaClass* _componentMetaClass);


    protected:
        MetaClass* m_metaClass;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentEditorDefault_hpp_
//////////////////////////////////////////
