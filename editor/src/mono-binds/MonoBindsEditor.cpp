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
#include "mono-binds/MonoBindsEditor.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-ui/ecs/components/MazeMenuBar2D.hpp"
#include "scenes/SceneEditor.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorSceneManager.hpp"
#include "ecs/events/EcsEditorEvents.hpp"
#include "Editor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline void MenuBarAddOption(MonoString* _menuName, MonoString* _option)
    {
        SceneEditor* sceneEditor = SceneEditor::GetInstancePtr();
        if (!sceneEditor)
            return;

        Char* strMenuName = mono_string_to_utf8(_menuName);
        Char* strOption = mono_string_to_utf8(_option);
        
        MenuBar2DPtr const& menuBar = sceneEditor->getMenuBar();
        menuBar->addOption(
            strMenuName,
            strOption,
            [](String const& _text)
            {
                SceneEditor* sceneEditor = SceneEditor::GetInstancePtr();
                if (sceneEditor)
                {
                    sceneEditor->getWorld()->broadcastEvent<OpenEditorEvent>(_text);
                }
            },
            nullptr,
            MAZE_HS("mono-editor"));

        mono_free(strMenuName);
        mono_free(strOption);
    }

    //////////////////////////////////////////
    inline void MenuBarClearOptions()
    {
        SceneEditor* sceneEditor = SceneEditor::GetInstancePtr();
        if (sceneEditor)
        {
            MenuBar2DPtr const& menuBar = sceneEditor->getMenuBar();
            menuBar->removeOptionsWithTag(MAZE_HS("mono-editor"));
        }
    }

    //////////////////////////////////////////
    inline S8 GetEditorMainSceneEcsWorld()
    {
        return EditorManager::GetInstancePtr()->getMainEcsWorld()->getId();
    }

    //////////////////////////////////////////
    inline S32 GetEditorOpenedScene()
    {
        EcsAssetScenePtr const& ecsScene = EditorSceneManager::GetInstancePtr()->getScene();
        if (ecsScene)
            return ecsScene->getId();

        return c_invalidEcsSceneId;
    }

    //////////////////////////////////////////
    inline S32 GetEditorMainRenderWindow()
    {
        RenderWindowPtr const& renderWindow = Editor::GetInstancePtr()->getMainRenderWindow();
        if (!renderWindow)
            return -1;

        return renderWindow->getResourceId();
    }

    //////////////////////////////////////////
    inline bool GetEditorPlaytestModeEnabled()
    {
        return EditorManager::GetInstancePtr()->getPlaytestModeEnabled();
    }


    //////////////////////////////////////////
    void BindCppFunctionsEditor()
    {
        MAZE_EDITOR_MONO_BIND_FUNC(MenuBarAddOption);
        MAZE_EDITOR_MONO_BIND_FUNC(MenuBarClearOptions);
        MAZE_EDITOR_MONO_BIND_FUNC(GetEditorMainSceneEcsWorld);
        MAZE_EDITOR_MONO_BIND_FUNC(GetEditorOpenedScene);
        MAZE_EDITOR_MONO_BIND_FUNC(GetEditorMainRenderWindow);
        MAZE_EDITOR_MONO_BIND_FUNC(GetEditorPlaytestModeEnabled);
    }


} // namespace Maze
//////////////////////////////////////////
