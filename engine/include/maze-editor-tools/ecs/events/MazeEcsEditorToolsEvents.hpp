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
#if (!defined(_MazeEcsEditorToolsEvents_hpp_))
#define _MazeEcsEditorToolsEvents_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class SceneDebugEditor;


    //////////////////////////////////////////
    // Class SceneDebugEditorInitEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API SceneDebugEditorInitEvent
        : public GenericEvent<SceneDebugEditorInitEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDebugEditorInitEvent, Event);

    public:

        //////////////////////////////////////////
        inline SceneDebugEditorInitEvent(SceneDebugEditor* _scene = nullptr)
            : m_scene(_scene)
        {}

        //////////////////////////////////////////
        inline SceneDebugEditor* getScene() const { return m_scene; }

    private:
        SceneDebugEditor* m_scene = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsEditorToolsEvents_hpp_
//////////////////////////////////////////
