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
#include "MazeEngineHeader.hpp"
#include "maze-engine/ecs/scenes/MazeSceneEngine.hpp"
#include "maze-graphics/ecs/components/MazeRenderController.hpp"
#include "maze-particles/ecs/components/MazeParticlesDrawerController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneEngine
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneEngine, ECSRenderScene);

    //////////////////////////////////////////
    SceneEngine::SceneEngine()
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneEngine::~SceneEngine()
    {
    }

    //////////////////////////////////////////
    SceneEnginePtr SceneEngine::Create(
        RenderTargetPtr const& _renderTarget,
        RenderSystemPtr const& _renderSystem)
    {
        SceneEnginePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneEngine, object, init(_renderTarget, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SceneEngine::init(
        RenderTargetPtr const& _renderTarget,
        RenderSystemPtr const& _renderSystem)
    {
        MAZE_PROFILE_EVENT("SceneEngine::init");

        if (!ECSRenderScene::init(_renderTarget))
            return false;

        createAndAddEntityWithComponent<RenderController>("RenderController", _renderSystem);

        createAndAddEntityWithComponent<ParticlesDrawerController>("ParticlesDrawerController", _renderSystem);
        
        return true;
    }

    //////////////////////////////////////////
    void SceneEngine::update(F32 _dt)
    {
    }


} // namespace Maze
//////////////////////////////////////////
