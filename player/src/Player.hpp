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
#if (!defined(_Player_hpp_))
#define _Player_hpp_


//////////////////////////////////////////
#include "maze-engine/MazeEngine.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Player);
    MAZE_USING_SHARED_PTR(UIManager);
    MAZE_USING_SHARED_PTR(PlayerManager);
    MAZE_USING_SHARED_PTR(PlayerConfigManager);
    

    //////////////////////////////////////////
    enum class PlayerMainRenderWindowState
    {
        None = 0,
        ReadyToCreate,
        Created,
        Error
    };


    //////////////////////////////////////////
    // Class Player
    //
    //////////////////////////////////////////
    class Player 
        : public Engine
    {
    public:

        //////////////////////////////////////////
        virtual ~Player();

        //////////////////////////////////////////
        static PlayerPtr Create(EngineConfig const& _config);

    

        //////////////////////////////////////////
        static inline Player* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline Player& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline PlayerManagerPtr const& getPlayerManager() const { return m_playerManager; }

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventMainRenderWindowCreated;
        MultiDelegate<> eventCorePlayerResourcesLoaded;

    protected:

        //////////////////////////////////////////
        Player();

        //////////////////////////////////////////
        bool init(EngineConfig const& _config);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyFrame();

        //////////////////////////////////////////
        bool createMainRenderWindow();

        //////////////////////////////////////////
        bool createMainRenderWindowAndGoToFirstSceneNow();

        //////////////////////////////////////////
        void processReadyToCreateWindowAndGoToSplash();

        //////////////////////////////////////////
        virtual bool initMainManagers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool initPlayerManagers();

        //////////////////////////////////////////
        bool loadPlugins();


        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

        //////////////////////////////////////////
        void notifyCorePlayerResourcesLoaded();

    protected:
        static Player* s_instance;

        PlayerMainRenderWindowState m_mainRenderWindowState;
        
        PlayerManagerPtr m_playerManager;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _Player_hpp_
//////////////////////////////////////////
