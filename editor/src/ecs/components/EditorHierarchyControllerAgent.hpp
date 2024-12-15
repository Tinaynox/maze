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
#if (!defined(_EditorHierarchyControllerAgent_hpp_))
#define _EditorHierarchyControllerAgent_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorHierarchyControllerAgent);
    MAZE_USING_SHARED_PTR(EditorHierarchyController);


    //////////////////////////////////////////
    // Class EditorHierarchyControllerAgent
    //
    //////////////////////////////////////////
    class EditorHierarchyControllerAgent
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EditorHierarchyControllerAgent, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EditorHierarchyControllerAgent);

        //////////////////////////////////////////
        friend class Entity;


    public:

        //////////////////////////////////////////
        virtual ~EditorHierarchyControllerAgent();

        //////////////////////////////////////////
        static EditorHierarchyControllerAgentPtr Create(EditorHierarchyController* _controller);


        //////////////////////////////////////////
        void processEntityParentChanged(
            EntityId _entityId,
            EntityId _prevParentEntityId,
            EntityId _newParentEntityId);

    protected:

        //////////////////////////////////////////
        EditorHierarchyControllerAgent();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(EditorHierarchyController* _controller);

    protected:
        EditorHierarchyController* m_controller = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorHierarchyControllerAgent_hpp_
//////////////////////////////////////////
