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

//////////////////////////////////////////
#if (MAZE_LIBSTEM_GAMEPAD_ENABLED)

//////////////////////////////////////////
#include "maze-gamepad/providers/MazeGamepadProviderLibstem.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include <gamepad/Gamepad.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline GamepadPtr AttachGamepad(Gamepad_device* _device)
    {
        GamepadPtr newGamepad = GamepadManager::GetInstancePtr()->attachGamepad(
            _device->deviceID,
            _device->description,
            _device->vendorID,
            _device->productID,
            _device->numAxes,
            _device->numButtons);

        if (!newGamepad)
            return nullptr;

        for (Size i = 0; i < (Size)_device->numAxes; ++i)
            newGamepad->setAxisState((S32)i, _device->axisStates[i]);

        for (Size i = 0; i < (Size)_device->numButtons; ++i)
            newGamepad->setButtonState((S32)i, _device->buttonStates[i] != 0);

        return newGamepad;
    }

    //////////////////////////////////////////
    void OnGamepadButtonDown(Gamepad_device* _device, U32 _buttonId, F64 _timestamp, void* _context)
    {
        GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadByDeviceId(_device->deviceID);
        if (!gamepad)
        {
            GamepadPtr newGamepad = AttachGamepad(_device);
            if (newGamepad)
                newGamepad->setButtonState(_buttonId, true);
            return;
        }

        gamepad->setButtonState(_buttonId, true);
    }

    //////////////////////////////////////////
    void OnGamepadButtonUp(Gamepad_device* _device, U32 _buttonId, F64 _timestamp, void* _context)
    {
        GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadByDeviceId(_device->deviceID);
        if (!gamepad)
        {
            GamepadPtr newGamepad = AttachGamepad(_device);
            if (newGamepad)
                newGamepad->setButtonState(_buttonId, false);
            return;
        }

        gamepad->setButtonState(_buttonId, false);
    }

    //////////////////////////////////////////
    void OnGamepadAxisMoved(Gamepad_device* _device, U32 _axisId, F32 _value, F32 _lastValue, F64 _timestamp, void* _context)
    {
        GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadByDeviceId(_device->deviceID);
        if (!gamepad)
        {
            GamepadPtr newGamepad = AttachGamepad(_device);
            if (newGamepad)
                newGamepad->setAxisState(_axisId, _value);
            return;
        }

        gamepad->setAxisState(_axisId, _value);
    }

    //////////////////////////////////////////
    void OnGamepadDeviceAttached(Gamepad_device* _device, void* _context)
    {
        AttachGamepad(_device);
    }

    //////////////////////////////////////////
    void OnGamepadDeviceRemoved(Gamepad_device* _device, void* _context)
    {
        GamepadManager::GetInstancePtr()->detachGamepad(_device->deviceID);
    }

    //////////////////////////////////////////
    void OnGamepadLog(S32 _gamepadLogPriority, CString _format, ...)
    {
        String newText;
        MAZE_FORMAT_VA_STRING(_format, newText);
        if (!newText.empty() && newText.back() == '\n')
            newText.pop_back();

        switch (_gamepadLogPriority)
        {
            case gamepad_log_warning:
                LogService::GetInstancePtr()->logFormatted(c_logPriority_Warning, "Libstem: %s", newText.c_str());
                break;
            case gamepad_log_error:
                LogService::GetInstancePtr()->logFormatted(c_logPriority_Error, "Libstem: %s", newText.c_str());
                break;
            default:
                LogService::GetInstancePtr()->logFormatted(c_logPriority_Default, "Libstem: %s", newText.c_str());
        }
    }


    //////////////////////////////////////////
    // Class GamepadProviderLibstem
    //
    //////////////////////////////////////////
    GamepadProviderLibstem::GamepadProviderLibstem()
    {
    }

    //////////////////////////////////////////
    GamepadProviderLibstem::~GamepadProviderLibstem()
    {
        shutdownLibstem();
    }

    //////////////////////////////////////////
    GamepadProviderLibstemPtr GamepadProviderLibstem::Create()
    {
        GamepadProviderLibstemPtr gamepad;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            GamepadProviderLibstem,
            gamepad,
            init());
        return gamepad;
    }

    //////////////////////////////////////////
    bool GamepadProviderLibstem::init()
    {
        if (!GamepadProvider::init())
            return false;

        initLibstem();

        return true;
    }

    //////////////////////////////////////////
    void GamepadProviderLibstem::update(F32 _dt)
    {
        Gamepad_processEvents();
    }

    //////////////////////////////////////////
    void GamepadProviderLibstem::detectGamepads()
    {
        Gamepad_detectDevices();
    }

    //////////////////////////////////////////
    void GamepadProviderLibstem::initLibstem()
    {
        Debug::Log("Init Libstem...");

        Gamepad_deviceAttachFunc(OnGamepadDeviceAttached, (void*)0x1);
        Gamepad_deviceRemoveFunc(OnGamepadDeviceRemoved, (void*)0x2);
        Gamepad_buttonDownFunc(OnGamepadButtonDown, (void*)0x3);
        Gamepad_buttonUpFunc(OnGamepadButtonUp, (void*)0x4);
        Gamepad_axisMoveFunc(OnGamepadAxisMoved, (void*)0x5);
        Gamepad_logFunc(OnGamepadLog);
        Gamepad_init();

        Debug::Log("Libstem inited.");
    }

    //////////////////////////////////////////
    void GamepadProviderLibstem::shutdownLibstem()
    {
        Debug::Log("Shutdown Libstem...");

        Gamepad_deviceAttachFunc(nullptr, (void*)0x0);
        Gamepad_deviceRemoveFunc(nullptr, (void*)0x0);
        Gamepad_buttonDownFunc(nullptr, (void*)0x0);
        Gamepad_buttonUpFunc(nullptr, (void*)0x0);
        Gamepad_axisMoveFunc(nullptr, (void*)0x0);
        Gamepad_shutdown();

        Debug::Log("Libstem shutdowned.");
    }
    
} // namespace Maze
//////////////////////////////////////////


#endif // MAZE_LIBSTEM_GAMEPAD_ENABLED
//////////////////////////////////////////