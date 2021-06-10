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
#if (!defined(_SceneGame_hpp_))
#define _SceneGame_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "game/level/LevelSpawnData.hpp"
#include "game/LevelSpawnController.hpp"
#include "game/LevelEnvironmentController.hpp"
#include "game/LevelPlayerController.hpp"
#include "game/LevelBloomController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneGame);
    MAZE_USING_SHARED_PTR(VirtualJoystickController);
    MAZE_USING_SHARED_PTR(SpaceObject);
    MAZE_USING_SHARED_PTR(GameplaySystem);
    MAZE_USING_SHARED_PTR(Projectile);
    MAZE_USING_SHARED_PTR(LevelAdapter);
    MAZE_USING_SHARED_PTR(ProjectilePool);
    MAZE_USING_SHARED_PTR(SpaceObjectPool);
    MAZE_USING_SHARED_PTR(RenderBuffer);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);


    //////////////////////////////////////////
    // Class SceneGame
    //
    //////////////////////////////////////////
    class SceneGame
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneGame, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneGamePtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneGame();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        ProjectilePtr createProjectile(Vec2DF const& _position);


        //////////////////////////////////////////
        Camera3DPtr const& getCamera3D() const { return m_camera3D; }


        //////////////////////////////////////////
        Rect2DF const& getGameplayBounds() const { return m_gameplayBounds; }


        //////////////////////////////////////////
        LevelAdapterPtr const& getLevelAdapter() const { return m_levelAdapter; }

        //////////////////////////////////////////
        inline F32 getGameTimer() const { return m_gameTimer; }


        //////////////////////////////////////////
        RenderBufferPtr const& getRenderBuffer() const { return m_renderBuffer; }

        //////////////////////////////////////////
        GameplaySystemPtr const& getGameplaySystem() const { return m_gameplaySystem; }


        //////////////////////////////////////////
        LevelSpawnControllerPtr const& getSpawnController() const { return m_spawnController; }

        //////////////////////////////////////////
        LevelEnvironmentControllerPtr const& getEnvironmentController() const { return m_environmentController; }

        //////////////////////////////////////////
        LevelPlayerControllerPtr const& getPlayerController() const { return m_playerController; }

    public:

        //////////////////////////////////////////
        MultiDelegate<F32> eventPostGameplayUpdate;
        MultiDelegate<F32> eventFixedUpdate;

    protected:

        //////////////////////////////////////////
        SceneGame();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMouse(InputEventMouseData const& _data);

        //////////////////////////////////////////
        void createControllers();

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void createSystems();

        //////////////////////////////////////////
        void notifyGameplaySystemUpdateFinished(F32 _dt);

        //////////////////////////////////////////
        void updateGameplayBounds();

        //////////////////////////////////////////
        void notifyFixedUpdateFinished(F32 _dt);

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void loadLevel(String const& _levelFileName);
        

    protected:
        F32 m_gameTimer;

        RenderBufferPtr m_renderBuffer;
        SpriteRenderer2DPtr m_renderColorSprite;
        SpriteRenderer2DPtr m_renderDepthSprite;

        CanvasPtr m_canvas;
        Camera3DPtr m_camera3D;

        VirtualJoystickControllerPtr m_virtualJoystickController;

        Rect2DF m_gameplayBounds;

        LevelAdapterPtr m_levelAdapter;
        
        ProjectilePoolPtr m_projectilePool;
        SpaceObjectPoolPtr m_spaceObjectPool;

        GameplaySystemPtr m_gameplaySystem;

        LevelSpawnControllerPtr m_spawnController;
        LevelEnvironmentControllerPtr m_environmentController;
        LevelPlayerControllerPtr m_playerController;
        LevelBloomControllerPtr m_bloomController;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneGame_hpp_
//////////////////////////////////////////
