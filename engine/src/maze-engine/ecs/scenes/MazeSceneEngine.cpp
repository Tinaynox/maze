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
#include "maze-ui/ecs/components/MazeInputSystem2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneEngine
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneEngine, EcsRenderScene);

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
    SceneEnginePtr SceneEngine::Create(SceneEngineInitConfig const& _config)
    {
        SceneEnginePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneEngine, object, init(_config));
        return object;
    }

    //////////////////////////////////////////
    bool SceneEngine::init(SceneEngineInitConfig const& _config)
    {
        MAZE_PROFILE_EVENT("SceneEngine::init");

        if (!EcsRenderScene::init(_config.renderTarget))
            return false;

        // Render
        if (_config.createRenderController)
            m_renderController = createAndAddEntityWithComponent<RenderController>("RenderController", _config.renderSystem);

        // Particles
        if (_config.createParticlesDrawerController)
            m_particlesDrawerController = createAndAddEntityWithComponent<ParticlesDrawerController>("ParticlesDrawerController", _config.renderSystem);

        // 2D Input
        if (_config.createInputSystem2D)
            m_inputSystem2D = createAndAddEntityWithComponent<InputSystem2D>("InputSystem2D");
        
        return true;
    }

    //////////////////////////////////////////
    void SceneEngine::update(F32 _dt)
    {
    }


} // namespace Maze
//////////////////////////////////////////
