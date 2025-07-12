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
#include "MazeCSharpHeader.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/system/MazeSystemCursor.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsEngine.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include "maze-editor-tools/helpers/MazeGizmosHelper.hpp"
#include "maze-engine/MazeEngine.hpp"



//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline S32 LoadAssetScene(
        MonoString* _sceneName,
        S32 _renderTargetId,
        bool _additive,
        S8 _ecsWorldId)
    {
        EcsAssetScenePtr scene;

        Char* sceneName = mono_string_to_utf8(_sceneName);

        Debug::Log("Loading asset scene: %s", sceneName);

        if (_renderTargetId == -1)
        {
            scene = Engine::GetInstancePtr()->loadAssetScene(
                sceneName,
                _additive,
                EcsWorldId(_ecsWorldId));
        }
        else
        {
            if (RenderTarget* renderTarget = RenderWindow::GetResource(_renderTargetId))
            {
                scene = Engine::GetInstancePtr()->loadAssetScene(
                    sceneName,
                    renderTarget->getSharedPtr(),
                    _additive,
                    EcsWorldId(_ecsWorldId));
            }
            else
            {
                MAZE_ERROR("Render target with id=%d is not found! (sceneName='%s')", _renderTargetId, sceneName);
            }
        }
        mono_free(sceneName);

        if (scene)
            return (S32)scene->getId();

        return -1;
    }

    //////////////////////////////////////////
    inline void DestroyScene(S32 _ecsSceneId)
    {
         SceneManager::GetInstancePtr()->destroyScene(EcsSceneId(_ecsSceneId));
    }

    //////////////////////////////////////////
    inline bool IsEngineMainRenderWindowFocused()
    {
        RenderWindowPtr const& renderWindow = Engine::GetInstancePtr()->getMainRenderWindow();
        if (!renderWindow)
            return false;

        return renderWindow->getFocused();
    }

    //////////////////////////////////////////
    inline void SetSystemCursor(S32 _systemCursorId)
    {
        RenderWindowPtr const& renderWindow = Engine::GetInstancePtr()->getMainRenderWindow();
        if (!renderWindow)
            return;

        if (SystemCursor* systemCursor = SystemCursor::GetResource(_systemCursorId))
            renderWindow->getWindow()->setCursor(systemCursor->getSharedPtr());
    }

    //////////////////////////////////////////
    inline S32 GetEngineFrame()
    {
        if (!Engine::GetInstancePtr())
            return -1;

        return Engine::GetInstancePtr()->getFrame();
    }

    


    //////////////////////////////////////////
    void MAZE_PLUGIN_CSHARP_API BindCppFunctionsEngine()
    {
        // Scene
        MAZE_ENGINE_MONO_BIND_FUNC(LoadAssetScene);
        MAZE_ENGINE_MONO_BIND_FUNC(DestroyScene);

        // Main RenderWindow
        MAZE_ENGINE_MONO_BIND_FUNC(IsEngineMainRenderWindowFocused);
        MAZE_ENGINE_MONO_BIND_FUNC(SetSystemCursor);

        // Engine
        MAZE_ENGINE_MONO_BIND_FUNC(GetEngineFrame);
    }

} // namespace Maze
//////////////////////////////////////////
