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
#if (!defined(_PlayerGamepad_hpp_))
#define _PlayerGamepad_hpp_


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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_5(PlayerGamepadDeviceCommand,
        MoveLeft,
        MoveRight,
        MoveDown,
        MoveUp,
        Fire);

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2(PlayerGamepadDeviceType,
        KeyboardMouse,
        Gamepad);

    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_7(PlayerGamepadInput,
        AxisPositive,
        AxisNegative,
        Button,
        Mouse,
        MouseButton,
        MouseWheelPositive,
        MouseWheelNegative);


    //////////////////////////////////////////
    // Struct PlayerGamepadBind
    //
    //////////////////////////////////////////
    struct PlayerGamepadBind
    {
        //////////////////////////////////////////
        PlayerGamepadBind()
            : input(PlayerGamepadInput::None)
            , number(0)
        {
        }

        //////////////////////////////////////////
        PlayerGamepadBind(
            PlayerGamepadInput::Enum _input,
            U32 _number,
            String const& _desc = "")
            : input(_input)
            , number(_number)
        {
        }

        //////////////////////////////////////////
        bool operator==(PlayerGamepadBind const& _value) const 
        {
            return     input == _value.input
                    && number == _value.number;
        }

        PlayerGamepadInput input;
        U32 number;
    };


    //////////////////////////////////////////
    using PlayerGamepadBindMap = Map<PlayerGamepadDeviceCommand, PlayerGamepadBind>;


    //////////////////////////////////////////
    // Struct PlayerGamepadBindData
    //
    //////////////////////////////////////////
    struct PlayerGamepadBindData
    {
        ////////////////////////////////////
        PlayerGamepadBindData()
        {}

        PlayerGamepadBindMap bindMap;
    };


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PlayerGamepad);


    //////////////////////////////////////////
    // Class PlayerGamepad
    //
    //////////////////////////////////////////
    class PlayerGamepad
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        using GetBindStringFunction = std::function<bool(PlayerGamepad*, PlayerGamepadDeviceCommand, String&)>;

    public:

        //////////////////////////////////////////
        ~PlayerGamepad();

        //////////////////////////////////////////
        static PlayerGamepadPtr Create(S32 _playerIndex);

        //////////////////////////////////////////
        PlayerGamepadBindMap const& getBindMap() const { return m_bindMap; }

        //////////////////////////////////////////
        PlayerGamepadBind const& getBind(PlayerGamepadDeviceCommand const& _command ){ return m_bindMap[_command]; }

        //////////////////////////////////////////
        F32 getBindValue(PlayerGamepadDeviceCommand _value){ return getBindValue(m_bindMap[_value]); }

        //////////////////////////////////////////
        Vec2DF getMoveAxis();

        //////////////////////////////////////////
        bool getFireButton();

        //////////////////////////////////////////
        String getDeviceName();

        //////////////////////////////////////////
        void resetBinds();

        //////////////////////////////////////////
        bool getConnected();

        //////////////////////////////////////////
        PlayerGamepadBindData* getCurrentPlayerBindData();

        //////////////////////////////////////////
        void setDevice(
            PlayerGamepadDeviceType _deviceType,
            U32 _deviceNumber);

        //////////////////////////////////////////
        inline S32 getPlayerIndex() const { return m_playerIndex; }

        //////////////////////////////////////////
        inline PlayerGamepadDeviceType getDeviceType() const { return m_deviceType; }

        //////////////////////////////////////////
        inline U32 getDeviceNumber() const { return m_deviceNumber; }

    protected:

        //////////////////////////////////////////
        PlayerGamepad();

        //////////////////////////////////////////
        bool init(S32 _playerIndex);

        ////////////////////////////////////
        F32 getBindValue(PlayerGamepadBind const& _bind);

        //////////////////////////////////////////
        void setBind(
            PlayerGamepadDeviceCommand _command,
            PlayerGamepadBind const& _bind);

        //////////////////////////////////////////
        bool autobindWithFile(String const& _fileName);

    protected:
        S32 m_playerIndex;
        PlayerGamepadDeviceType m_deviceType;
        U32 m_deviceNumber;

        PlayerGamepadBindMap m_bindMap;

        F32 m_mouseWheelState;

        GetBindStringFunction m_getBindStringFunction;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _PlayerGamepad_hpp_
//////////////////////////////////////////
