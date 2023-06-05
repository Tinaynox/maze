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
#if (!defined(_Game_hpp_))
#define _Game_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngine.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Game);
    MAZE_USING_SHARED_PTR(UIManager);
    MAZE_USING_SHARED_PTR(PlayerManager);
    MAZE_USING_SHARED_PTR(GameManager);


    //////////////////////////////////////////
    enum class GameMainRenderWindowState
    {
        None = 0,
        ReadyToCreate,
        Created,
        Error
    };


    //////////////////////////////////////////
    // Class Game
    //
    //////////////////////////////////////////
    class Game 
        : public Engine
    {
    public:

        //////////////////////////////////////////
        virtual ~Game();

        //////////////////////////////////////////
        static GamePtr Create(EngineConfig const& _config);

    
        //////////////////////////////////////////
        RenderWindowPtr const& getMainRenderWindow() const { return m_mainRenderWindow;}

        //////////////////////////////////////////
        Rect2DF const& getMainRenderWindowViewport() const { return m_mainRenderWindowViewport; }

        //////////////////////////////////////////
        inline Vec2DU getMainRenderWindowAbsoluteSize()
        {
            return Vec2DU(Vec2DF(getMainRenderWindow()->getRenderTargetSize()) * getMainRenderWindowViewport().size);
        }


        //////////////////////////////////////////
        PlayerManagerPtr const& getPlayerManager() const { return m_playerManager; }


        //////////////////////////////////////////
        static inline Game* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline Game& GetInstance() { return *s_instance; }

        //////////////////////////////////////////
        void openDebugEditor();

        //////////////////////////////////////////
        void closeDebugEditor();

        //////////////////////////////////////////
        void updateDebugEditor();

        //////////////////////////////////////////
        bool isMainWindowReadyToRender();

        //////////////////////////////////////////
        inline F32 getDebugEditorProgress() const { return m_debugEditorProgress; }

        //////////////////////////////////////////
        inline bool isDebugEditorProgress() const { return m_debugEditorProgress > 0.0f && m_debugEditorProgress < 1.0f; }


        //////////////////////////////////////////
        /*
        template <typename TScene>
        inline void goToSceneThroughPreloader(ECSScenePtr const& _sceneToUnload)
        {
            ECSSceneWPtr sceneWeak = getSharedPtr();

            SceneFadePreloaderPtr sceneFadePreloader = SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
            SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
            sceneFadePreloader->eventFade.subscribe(
                [sceneFadePreloaderWeak, sceneWeak, this]()
                {
                    SceneManager::GetInstancePtr()->loadScene<SceneMainMenu>();

                    ECSScenePtr scene = sceneWeak.lock();
                    if (scene)
                    {
                        SceneManager::GetInstancePtr()->destroyScene(scene);
                    }

                    sceneFadePreloaderWeak.lock()->fadeOut();
                });
        }
        */

    public:

        //////////////////////////////////////////
        MultiDelegate<Rect2DF const&> eventMainRenderWindowViewportChanged;
        MultiDelegate<> eventMainRenderWindowCreated;
        MultiDelegate<> eventCoreGameResourcesLoaded;

    protected:

        //////////////////////////////////////////
        Game();

        //////////////////////////////////////////
        bool init(EngineConfig const& _config);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyFrame();

        //////////////////////////////////////////
        bool createMainRenderWindow();

        //////////////////////////////////////////
        bool createMainRenderWindowAndGoToSplashNow();

        //////////////////////////////////////////
        void processReadyToCreateWindowAndGoToSplash();

        //////////////////////////////////////////
        virtual bool initMainManagers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool initGameManagers();

        //////////////////////////////////////////
        bool loadPlugins();

        //////////////////////////////////////////
        void setMainRenderWindowViewport(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void notifyDebuggerActiveChanged(bool const& _active);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

        //////////////////////////////////////////
        void updateDebugEditorViewport();

    protected:
        static Game* s_instance;

        GameMainRenderWindowState m_mainRenderWindowState;
        RenderWindowPtr m_mainRenderWindow;
        Rect2DF m_mainRenderWindowViewport;

        PlayerManagerPtr m_playerManager;
        GameManagerPtr m_gameManager;

        F32 m_debugEditorProgress;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _Game_hpp_
//////////////////////////////////////////
