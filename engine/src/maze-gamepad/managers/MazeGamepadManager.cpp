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
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GamepadManager
    //
    //////////////////////////////////////////
    GamepadManager* GamepadManager::s_instance = nullptr;
    U32 const GamepadManager::s_virtualGamepadDeviceId = -1;

    //////////////////////////////////////////
    GamepadManager::GamepadManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    GamepadManager::~GamepadManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void GamepadManager::Initialize(GamepadManagerPtr& _gamepadManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(GamepadManager, _gamepadManager, init());
    }

    //////////////////////////////////////////
    bool GamepadManager::init()
    {
        DeviceCategory deviceCategory = SystemManager::GetInstancePtr()->getDeviceCategory();
        if (   deviceCategory == DeviceCategory::Phone 
            || deviceCategory == DeviceCategory::Pad
            )
        {
            m_virtualGamepad = attachGamepad(s_virtualGamepadDeviceId, "Virtual Gamepad", 0, 0, 8, 8);
        }

        return true;
    }

    //////////////////////////////////////////
    void GamepadManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void GamepadManager::detectGamepads()
    {
    }

    //////////////////////////////////////////
    GamepadPtr GamepadManager::attachGamepad(
        U32 _deviceId,
        String const& _desc,
        U32 _vendorId,
        U32 _productId,
        U32 _axesCount,
        U32 _buttonsCount)
    {
        GamepadPtr gamepad;

        for (GamepadListByDeviceId::iterator    it = m_gamepads.begin(),
                                                end = m_gamepads.end();
                                                it != end;
                                                ++it)
        {
            if (!it->second)
                continue;
            
            if (false == it->second->getConnected()
                && it->second->getDescription() == _desc
                && it->second->getVendorId() == _vendorId
                && it->second->getProductId() == _productId
                && it->second->getAxesCount() == _axesCount
                && it->second->getButtonsCount() == _buttonsCount)
            {
                // Gamepad reconnected with new deviceId
                gamepad = it->second;
                gamepad->setDeviceId(_deviceId);
                m_gamepads.erase(it);
                break;
            }
        }

        if (!gamepad)
            gamepad = Gamepad::Create(_deviceId, _desc, _vendorId, _productId, _axesCount, _buttonsCount);

        m_gamepads.insert(GamepadListByDeviceId::value_type(_deviceId, gamepad));
        gamepad->setConnected(true);

        return gamepad;
    }

    //////////////////////////////////////////
    void GamepadManager::detachGamepad(U32 _deviceId)
    {
        GamepadListByDeviceId::iterator it = m_gamepads.find(_deviceId);
        if (it == m_gamepads.end())
            return;

        it->second->setConnected(false);
    }

    //////////////////////////////////////////
    GamepadPtr const& GamepadManager::getGamepadById(U32 _id) const
    {
        static GamepadPtr nullGamepad;

        for (GamepadListByDeviceId::const_iterator    it = m_gamepads.begin(),
                                                    end = m_gamepads.end();
                                                    it != end;
                                                    ++it)
        {
            if (!it->second)
                continue;
            
            if (it->second->getId() == _id)
                return it->second;
        }

        return nullGamepad;
    }

    //////////////////////////////////////////
    Vector<GamepadPtr> GamepadManager::getGamepadsByProductId(U32 _vid, U32 _pid) const
    {
        Vector<GamepadPtr> result;
        for( GamepadListByDeviceId::const_iterator    it = m_gamepads.begin(),
                                                    end = m_gamepads.end();
                                                    it != end;
                                                    ++it )
        {
            if (!it->second)
                continue;
            
            if (it->second->getVendorId() == _vid && it->second->getProductId() == _pid)
                result.push_back(it->second);
        }

        return result;
    }

    
} // namespace Maze
//////////////////////////////////////////
