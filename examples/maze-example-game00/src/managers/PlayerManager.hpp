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
#if (!defined(_PlayerManager_hpp_))
#define _PlayerManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
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
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "input/PlayerGamepad.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PlayerManager);
    MAZE_USING_SHARED_PTR(Player);


    //////////////////////////////////////////
    // Struct PlayerDeviceBindKey
    //
    //////////////////////////////////////////
    struct PlayerDeviceBindKey
    {
        PlayerDeviceBindKey( 
            S32 _playerId = 0,
            PlayerGamepadDeviceType _deviceType = PlayerGamepadDeviceType::KeyboardMouse,
            S32 _vid = 0,
            S32 _pid = 0)
            : playerId(_playerId)
            , deviceType(_deviceType)
            , vid(_vid)
            , pid(_pid)
        {
        
        }
        
        //////////////////////////////////////////
        inline bool operator==(PlayerDeviceBindKey const& _value) const
        {
            return     playerId == _value.playerId
                    && deviceType == _value.deviceType
                    && vid == _value.vid
                    && pid == _value.pid;
        }

        //////////////////////////////////////////
        inline bool operator<(PlayerDeviceBindKey const& _value) const
        {
            return memcmp(&playerId, &_value.playerId, sizeof(PlayerDeviceBindKey)) < 0;
        }


        S32 playerId;
        PlayerGamepadDeviceType deviceType;
        S32 vid;
        S32 pid;
    };


    //////////////////////////////////////////
    enum class PlayerGamepadFamousBindType
    {
        None = 0,
        XInputLStickLeft,
        XInputLStickRight,
        XInputLStickUp,
        XInputLStickDown,
        XInputRStickLeft,
        XInputRStickRight,
        XInputRStickUp,
        XInputRLStickDown,
        XInputDPadLeft,
        XInputDPadRight,
        XInputDPadUp,
        XInputDPadDown,
        XInputButtonX,
        XInputButtonY,
        XInputButtonA,
        XInputButtonB,
        XInputButtonLB,
        XInputButtonLT,
        XInputButtonRB,
        XInputButtonRT,
        XInputButtonStart,
        XInputButtonBack,

        PSControllerLStickLeft,
        PSControllerLStickRight,
        PSControllerLStickUp,
        PSControllerLStickDown,
        PSControllerRStickLeft,
        PSControllerRStickRight,
        PSControllerRStickUp,
        PSControllerRLStickDown,
        PSControllerDPadLeft,
        PSControllerDPadRight,
        PSControllerDPadUp,
        PSControllerDPadDown,
        PSControllerButtonCross,
        PSControllerButtonTriangle,
        PSControllerButtonCircle,
        PSControllerButtonSquare,
        PSControllerButtonL1,
        PSControllerButtonL2,
        PSControllerButtonR1,
        PSControllerButtonR2,
        PSControllerButtonStart,
        PSControllerButtonBack
    };


    //////////////////////////////////////////
    // Class PlayerManager
    //
    //////////////////////////////////////////
    class PlayerManager
    {
    public:

        //////////////////////////////////////////
        ~PlayerManager();

        //////////////////////////////////////////
        static void Initialize(PlayerManagerPtr& _playerManager);
        

        //////////////////////////////////////////
        static inline PlayerManager* GetInstancePtr(){ return s_instance; }

        //////////////////////////////////////////
        static inline PlayerManager& GetInstance(){ return *s_instance; }


        //////////////////////////////////////////
        inline PlayerPtr const& getPlayer(Size _i) { return m_players[_i]; }

        //////////////////////////////////////////
        PlayerGamepadBindData* getPlayerBindData(
            S32 _playerIndex,
            PlayerGamepadDeviceType _deviceType,
            S32 _vid,
            S32 _pid);

    protected:

        //////////////////////////////////////////
        PlayerManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static PlayerManager* s_instance;

        PlayerPtr m_players[2];

        Map<PlayerDeviceBindKey, PlayerGamepadBindData> m_playerBindData;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _PlayerManager_hpp_
//////////////////////////////////////////
