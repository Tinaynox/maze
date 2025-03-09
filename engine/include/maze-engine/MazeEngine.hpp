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
#if (!defined(_MazeEngine_hpp_))
#define _MazeEngine_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngineHeader.hpp"
#include "maze-engine/MazeEngineConfig.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-engine/ecs/scenes/MazeSceneEngine.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-graphics/ecs/MazeEcsAssetScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Engine);
    MAZE_USING_SHARED_PTR(SystemManager);
    MAZE_USING_SHARED_PTR(TaskManager);
    MAZE_USING_SHARED_PTR(EventManager);
    MAZE_USING_SHARED_PTR(InputManager);
    MAZE_USING_SHARED_PTR(WindowManager);
    MAZE_USING_SHARED_PTR(DynLibManager);
    MAZE_USING_SHARED_PTR(PluginManager);
    MAZE_USING_SHARED_PTR(GraphicsManager);
    MAZE_USING_SHARED_PTR(SceneManager);
    MAZE_USING_SHARED_PTR(AssetManager);
    MAZE_USING_SHARED_PTR(EntityManager);
    MAZE_USING_SHARED_PTR(SettingsManager);
    MAZE_USING_SHARED_PTR(GamepadManager);
    MAZE_USING_SHARED_PTR(UIManager);
    MAZE_USING_SHARED_PTR(ParticlesManager);
    MAZE_USING_SHARED_PTR(EditorToolsManager);
    MAZE_USING_SHARED_PTR(Physics2DManager);
    MAZE_USING_MANAGED_SHARED_PTR(RenderWindow);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_SHARED_PTR(SoundManager);
    MAZE_USING_SHARED_PTR(SceneEngine);


    //////////////////////////////////////////
    // Class Engine
    //
    //////////////////////////////////////////
    class MAZE_ENGINE_API Engine
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        virtual ~Engine();

        //////////////////////////////////////////
        static EnginePtr Create(EngineConfig const& _config);

        //////////////////////////////////////////
        bool frame();

        //////////////////////////////////////////
        void run();

        //////////////////////////////////////////
        void shutdown();

        //////////////////////////////////////////
        inline EngineConfig const& getConfig() const { return m_config; }


        //////////////////////////////////////////
        inline SystemManagerPtr const& getSystemManager() const { return m_systemManager; }

        //////////////////////////////////////////
        inline TaskManagerPtr const& getTaskManager() const { return m_taskManager; }

        //////////////////////////////////////////
        inline EventManagerPtr const& getEventManager() const { return m_eventManager; }

        //////////////////////////////////////////
        inline InputManagerPtr const& getInputManager() const { return m_inputManager; }

        //////////////////////////////////////////
        inline WindowManagerPtr const& getWindowManager() const { return m_windowManager; }

        //////////////////////////////////////////
        inline DynLibManagerPtr const getDynLibManager() const { return m_dynLibManager; }

        //////////////////////////////////////////
        inline PluginManagerPtr const& getPluginManager() const { return m_pluginManager; }

        //////////////////////////////////////////
        inline GraphicsManagerPtr const& getGraphicsManager() const { return m_graphicsManager; }

        //////////////////////////////////////////
        inline SceneManagerPtr const& getSceneManager() const { return m_sceneManager; }

        //////////////////////////////////////////
        inline AssetManagerPtr const& getAssetManager() const { return m_assetManager; }

        //////////////////////////////////////////
        inline EntityManagerPtr const& getEntity() const { return m_entityManager; }

        //////////////////////////////////////////
        inline SettingsManagerPtr const& getSettingsManager() const { return m_settingsManager; }

        //////////////////////////////////////////
        inline GamepadManagerPtr const& getGamepadManager() const { return m_gamepadManager; }

        //////////////////////////////////////////
        inline Physics2DManagerPtr const& getPhysics2DManager() const { return m_physics2DManager; }

        //////////////////////////////////////////
        inline ParticlesManagerPtr const& getParticlesManager() const { return m_particlesManager; }

        //////////////////////////////////////////
        inline SoundManagerPtr const& getSoundManager() const { return m_soundManager; }

        //////////////////////////////////////////
        inline EditorToolsManagerPtr const& getEditorToolsManager() const { return m_editorToolsManager; }

        //////////////////////////////////////////
        inline bool getRunning() const { return m_running; }


        //////////////////////////////////////////
        void setEngineRenderTarget(RenderTargetPtr const& _engineRenderTarget);

        //////////////////////////////////////////
        inline RenderTargetPtr const& getEngineRenderTarget() const { return m_engineRenderTarget; }


        //////////////////////////////////////////
        EcsAssetScenePtr loadAssetScene(
            Path const& _sceneName,
            RenderTargetPtr const& _renderTarget,
            bool _additive = true,
            EcsWorldId _ecsWorldId = c_invalidWorldId);

        //////////////////////////////////////////
        EcsAssetScenePtr loadAssetScene(
            Path const& _sceneName,
            bool _additive = true,
            EcsWorldId _ecsWorldId = c_invalidWorldId);


        //////////////////////////////////////////
        static inline Engine* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline Engine& GetInstance() { return *s_instance; }

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventInit;
        MultiDelegate<> eventFrame;
        MultiDelegate<> eventShutdown;

    protected:

        //////////////////////////////////////////
        Engine();

        //////////////////////////////////////////
        virtual bool init(EngineConfig const& _config);

        //////////////////////////////////////////
        virtual void update(F32 dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyApplicationInit();

        //////////////////////////////////////////
        void notifyApplicationFrame();

        //////////////////////////////////////////
        virtual bool initMainManagers();

        //////////////////////////////////////////
        virtual void createPrimaryEcsWorldSystems(
            EcsWorldPtr const& _world,
            RenderWindowPtr const& _renderWindow,
            RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        virtual SceneEnginePtr createEngineScene(SceneEngineInitConfig const& _config);

        //////////////////////////////////////////
        virtual S32 getUpdatableOrder() const MAZE_OVERRIDE { return 10000; }

    protected:
        static Engine* s_instance;

        EngineConfig m_config;

        bool m_running;

        RenderTargetPtr m_engineRenderTarget;
                
        SystemManagerPtr m_systemManager;
        TaskManagerPtr m_taskManager;
        EventManagerPtr m_eventManager;
        InputManagerPtr m_inputManager;
        WindowManagerPtr m_windowManager;
        DynLibManagerPtr m_dynLibManager;
        PluginManagerPtr m_pluginManager;
        GraphicsManagerPtr m_graphicsManager;
        SceneManagerPtr m_sceneManager;
        AssetManagerPtr m_assetManager;
        EntityManagerPtr m_entityManager;
        SettingsManagerPtr m_settingsManager;
        GamepadManagerPtr m_gamepadManager;
        Physics2DManagerPtr m_physics2DManager;
        UIManagerPtr m_uiManager;
        ParticlesManagerPtr m_particlesManager;
        SoundManagerPtr m_soundManager;
        EditorToolsManagerPtr m_editorToolsManager;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEngine_hpp_
//////////////////////////////////////////
