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
#include "maze-engine/ecs/scenes/MazeSceneEnginePlayer.hpp"
#include "maze-graphics/ecs/components/MazeRenderController.hpp"
#include "maze-particles/ecs/components/MazeParticlesDrawerController.hpp"
#include "maze-ui/ecs/components/MazeInputSystem2D.hpp"
#include "maze-editor-tools/ecs/components/MazeGizmosController.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-engine/ecs/components/MazeEnginePlayer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneEnginePlayer
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneEnginePlayer, EcsRenderScene);

    //////////////////////////////////////////
    SceneEnginePlayer::SceneEnginePlayer()
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneEnginePlayer::~SceneEnginePlayer()
    {
    }

    //////////////////////////////////////////
    SceneEnginePlayerPtr SceneEnginePlayer::Create(SceneEnginePlayerInitConfig const& _config)
    {
        SceneEnginePlayerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneEnginePlayer, object, init(_config));
        return object;
    }

    //////////////////////////////////////////
    bool SceneEnginePlayer::init(SceneEnginePlayerInitConfig const& _config)
    {
        if (!EcsRenderScene::init(_config.renderTarget))
            return false;

        EntityPtr enginePlayerEntity = createEntity("EnginePlayer");
        m_enginePlayer = enginePlayerEntity->createComponent<EnginePlayer>();
        m_enginePlayer->setCurrentCamera3D(_config.currentCamera3D);

        return true;
    }

    //////////////////////////////////////////
    void SceneEnginePlayer::update(F32 _dt)
    {
    }

    


} // namespace Maze
//////////////////////////////////////////
