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
#include "MazeGamepadHeader.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Gamepad
    //
    //////////////////////////////////////////
    Gamepad::Gamepad()
        : m_id(0)
        , m_deviceId(0)
        , m_connected(false)
        , m_vendorId(0)
        , m_productId(0)
        , m_axesCount(0)
        , m_buttonsCount(0)
    {
    }

    //////////////////////////////////////////
    Gamepad::~Gamepad()
    {
    }

    //////////////////////////////////////////
    GamepadPtr Gamepad::Create(
        U32 _deviceId,
        String const& _name,
        S32 _vendorId,
        S32 _productId,
        U32 _axesCount,
        U32 _buttonsCount)
    {
        GamepadPtr gamepad;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            Gamepad,
            gamepad,
            init(
                _deviceId,
                _name,
                _vendorId,
                _productId,
                _axesCount,
                _buttonsCount));
        return gamepad;
    }

    //////////////////////////////////////////
    bool Gamepad::init(
        U32 _deviceId,
        String const& _name,
        S32 _vendorId,
        S32 _productId,
        Size _axesCount,
        Size _buttonsCount)
    {
        static U32 s_id = 0;
        m_id = s_id++;

        m_deviceId = _deviceId;
        m_name = _name;
        m_vendorId = _vendorId;
        m_productId = _productId;
        setAxesAndButtonsCount(_axesCount, _buttonsCount);

        return true;
    }

    //////////////////////////////////////////
    void Gamepad::setAxesAndButtonsCount(
        Size _axesCount,
        Size _buttonsCount)
    {
        m_axesCount = _axesCount;
        m_axisStates.resize(m_axesCount);
        for (Size i = 0; i < m_axesCount; ++i)
            m_axisStates[i] = 0.0f;

        m_buttonsCount = _buttonsCount;
        m_buttonStates.resize(m_buttonsCount);
        for (Size i = 0; i < m_buttonsCount; ++i)
            m_buttonStates[i] = false;
    }

    //////////////////////////////////////////
    void Gamepad::setConnected(bool _connected)
    {
        m_connected = _connected;

        eventConnectedChanged(this, m_connected);

        if (m_connected)
        {
            Debug::Log("Gamepad%u: %s - Attached with deviceId=%u (0x%d 0x%d)", m_id, m_name.c_str(), m_deviceId, m_vendorId, m_productId);
            Debug::Log("\tButtons count: %u", m_buttonsCount);
            Debug::Log("\tAxes count: %u", m_axesCount);
        }
        else
        {
            Debug::Log("Gamepad%u: %s - Removed", m_id, m_name.c_str());
        }
    }

    //////////////////////////////////////////
    void Gamepad::setAxisState(S32 _axisId, F32 _axisState)
    {
        MAZE_DEBUG_BP_RETURN_IF(_axisId >= (S32)m_axesCount);

        if (m_axisStates[_axisId] == _axisState)
            return;

        F32 lastState = m_axisStates[_axisId];
        m_axisStates[_axisId] = _axisState;

        eventAxisValueChanged(this, _axisId, _axisState);
    }

    //////////////////////////////////////////
    void Gamepad::setButtonState(S32 _buttonId, bool _buttonState)
    {
        MAZE_DEBUG_BP_RETURN_IF(_buttonId >= (S32)m_buttonsCount);

        if (m_buttonStates[_buttonId] == _buttonState)
            return;

        m_buttonStates[_buttonId] = _buttonState;

        eventButtonValueChanged(this, _buttonId, _buttonState);
    }
    
} // namespace Maze
//////////////////////////////////////////
