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
        , m_iconSymbol(0xE307)
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
        String const& _description,
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
                _description,
                _vendorId,
                _productId,
                _axesCount,
                _buttonsCount));
        return gamepad;
    }

    //////////////////////////////////////////
    bool Gamepad::init(
        U32 _deviceId,
        String const& _description,
        S32 _vendorId,
        S32 _productId,
        Size _axesCount,
        Size _buttonsCount)
    {
        static U32 s_id = 0;
        m_id = s_id++;

        m_deviceId = _deviceId;
        m_description = _description;
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
    String Gamepad::getDescriptionWithIcons()
    {
        String result;
        
        if (!getConnected())
            result.append(" #{255,100,100}");
        result.append(m_description);

        if (!getConnected())
        {
            result.append(" #{255,255,255}");
            result.append(TextHelper::UTF8CodePointToBFString(0xE30F));
        }

        return result;
    }

    //////////////////////////////////////////
    void Gamepad::setConnected(bool _connected)
    {
        m_connected = _connected;

        InputEventGamepadData eventData;
        eventData.type = m_connected ? InputEventGamepadType::GamepadAttached : InputEventGamepadType::GamepadDetached;
        eventData.id = m_id;
        eventData.deviceId = m_deviceId;
        InputManager::GetInstancePtr()->generateInputEvent(eventData);

        if (m_connected)
        {
            Debug::Log("Gamepad%u: %s - Attached with deviceId=%u (0x%X 0x%X)", m_id, m_description.c_str(), m_deviceId, m_vendorId, m_productId);
            Debug::Log("\tButtons count: %u", m_buttonsCount);
            Debug::Log("\tAxes count: %u", m_axesCount);
        }
        else
        {
            Debug::Log("Gamepad%u: %s - Removed", m_id, m_description.c_str());
        }
    }

    //////////////////////////////////////////
    void Gamepad::setAxisState(S32 _axisId, F32 _axisState)
    {
        MAZE_DEBUG_BP_IF(_axisId >= (S32)m_axesCount);

        if (m_axisStates[_axisId] == _axisState)
            return;

        F32 lastState = m_axisStates[_axisId];
        m_axisStates[_axisId] = _axisState;

        InputEventGamepadData eventData;
        eventData.type = InputEventGamepadType::AxisMove;
        eventData.id = m_id;
        eventData.deviceId = m_deviceId;
        eventData.axisData.axisId = _axisId;
        eventData.axisData.value = _axisState;
        eventData.axisData.lastValue = lastState;
        InputManager::GetInstancePtr()->generateInputEvent(eventData);
    
    }

    //////////////////////////////////////////
    void Gamepad::setButtonState(S32 _buttonId, bool _buttonState)
    {
        MAZE_DEBUG_BP_IF(_buttonId >= (S32)m_buttonsCount);

        if (m_buttonStates[_buttonId] == _buttonState)
            return;

        m_buttonStates[_buttonId] = _buttonState;

        InputEventGamepadData eventData;
        eventData.id = m_id;
        eventData.deviceId = m_deviceId;
        eventData.type = _buttonState ? InputEventGamepadType::ButtonDown : InputEventGamepadType::ButtonUp;
        eventData.buttonId = _buttonId;
        InputManager::GetInstancePtr()->generateInputEvent(eventData);
    
    }
    
} // namespace Maze
//////////////////////////////////////////
