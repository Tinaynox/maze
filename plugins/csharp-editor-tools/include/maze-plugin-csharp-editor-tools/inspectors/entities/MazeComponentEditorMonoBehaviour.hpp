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
#if (!defined(_MazeComponentEditorMonoBehaviour_hpp_))
#define _MazeComponentEditorMonoBehaviour_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ComponentEditorMonoBehaviour);


    //////////////////////////////////////////
    // Class ComponentEditorMonoBehaviour
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API ComponentEditorMonoBehaviour
        : public GenericComponentEditor<MonoBehaviour>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ComponentEditorMonoBehaviour, ComponentEditor);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ComponentEditorMonoBehaviour);

    public:

        //////////////////////////////////////////
        virtual ~ComponentEditorMonoBehaviour();

        //////////////////////////////////////////
        static ComponentEditorMonoBehaviourPtr Create(ComponentId _componentId, MetaClass* _metaClass);


    protected:

        //////////////////////////////////////////
        ComponentEditorMonoBehaviour();

        //////////////////////////////////////////
        virtual bool init(ComponentId _componentId, MetaClass* _metaClass) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual String buildEditorTitle() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    protected:
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeComponentEditorMonoBehaviour_hpp_
//////////////////////////////////////////
