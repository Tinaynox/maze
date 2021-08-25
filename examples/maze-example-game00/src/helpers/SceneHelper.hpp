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
#if (!defined(_SceneHelper_hpp_))
#define _SceneHelper_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "scenes/SceneFadePreloader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SceneHelper
    {
        //////////////////////////////////////////
        template <typename TScene>
        bool LoadSceneWithPreloader(
            std::function<void(SharedPtr<TScene> const&)> _onComplete = nullptr)
        {
            if (SceneManager::GetInstancePtr()->getScene<SceneFadePreloader>())
                return false;

            SceneFadePreloaderPtr sceneFadePreloader = SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
            SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
            sceneFadePreloader->eventFade.subscribe(
                [sceneFadePreloaderWeak, _onComplete]()
                {
                    SharedPtr<TScene> scene = SceneManager::GetInstancePtr()->loadScene<TScene>(false);

                    sceneFadePreloaderWeak.lock()->fadeOut();

                    if (_onComplete)
                    {
                        _onComplete(scene);
                    }
                });

            reutrn true;
        }

        //////////////////////////////////////////
        template <typename TScene>
        bool LoadSceneWithPreloader(
            ECSScenePtr const& _sceneToUnload,
            std::function<void(SharedPtr<TScene> const&)> _onComplete = nullptr)
        {
            if (SceneManager::GetInstancePtr()->getScene<SceneFadePreloader>())
                return false;

            ECSSceneWPtr sceneToUnloadWeak = _sceneToUnload;

            SceneFadePreloaderPtr sceneFadePreloader = SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
            SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
            sceneFadePreloader->eventFade.subscribe(
                [sceneFadePreloaderWeak, sceneToUnloadWeak, _onComplete]()
                {
                    SharedPtr<TScene> scene = SceneManager::GetInstancePtr()->loadScene<TScene>(true);

                    ECSScenePtr sceneToUnload = sceneToUnloadWeak.lock();
                    if (sceneToUnload)
                    {
                        SceneManager::GetInstancePtr()->destroyScene(sceneToUnload);
                    }

                    sceneFadePreloaderWeak.lock()->fadeOut();

                    if (_onComplete)
                    {
                        _onComplete(scene);
                    }
                });

            return true;
        }


    } // namespace SceneHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneHelper_hpp_
//////////////////////////////////////////
