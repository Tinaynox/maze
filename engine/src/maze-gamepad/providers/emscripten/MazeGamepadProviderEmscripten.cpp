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
#include "maze-gamepad/providers/emscripten/MazeGamepadProviderEmscripten.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeProfiler.hpp"


//////////////////////////////////////////
EM_BOOL OnGamepadEvent(Maze::S32 _eventType, EmscriptenGamepadEvent const* _event, void* _userData)
{
    return ((Maze::GamepadProviderEmscripten*)_userData)->processGamepadEvent(_eventType, _event);
}


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline GamepadPtr AttachGamepad(EmscriptenGamepadEvent const* _event)
    {
        String gamepadName;
        U32 vendorId = 0u;
        U32 productId = 0u;
        
        MAZE_CONSTEXPR Char const vendorStr[] = "(Vendor: ";
        MAZE_CONSTEXPR Char const productStr[] = " Product: ";
        MAZE_CONSTEXPR std::integral_constant<Size, sizeof(vendorStr) - 1> vendorStrLen{};
        MAZE_CONSTEXPR std::integral_constant<Size, sizeof(productStr) - 1> productStrLen{};
        
        auto vendorProductPtr = strstr(_event->id, vendorStr);
        if (vendorProductPtr != nullptr)
        {
            auto productPtr = strstr(vendorProductPtr, productStr);
            if (productPtr != nullptr)
            {
                auto endPtr = strstr(productPtr, ")");
                if (endPtr != nullptr)
                {
                    CString vendorValuePtr = vendorProductPtr + vendorStrLen;
                    CString productValuePtr = productPtr + productStrLen;
                    
                    S32 vendorValueLen = S32(std::ptrdiff_t(productPtr) - std::ptrdiff_t(vendorValuePtr));
                    S32 productValueLen = S32(std::ptrdiff_t(endPtr) - std::ptrdiff_t(productValuePtr));
                    
                    String vendorValueStr(vendorValuePtr, vendorValuePtr + vendorValueLen);
                    String productValueStr(productValuePtr, productValuePtr + productValueLen);
                    
                    vendorId = (U32)std::stoi(vendorValueStr.c_str(), nullptr, 16);
                    productId = (U32)std::stoi(productValueStr.c_str(), nullptr, 16);
                    
                    S32 gamepadNameLen = S32(std::ptrdiff_t(vendorProductPtr) - std::ptrdiff_t(_event->id));
                    gamepadName = String(_event->id, _event->id + gamepadNameLen);
                }
                else
                    gamepadName = _event->id;
            }
            else
                gamepadName = _event->id;
        }
        else
        {
            gamepadName = _event->id;
            
            if (strstr(_event->id, "XInput") != nullptr)
            {
                vendorId = 0x45E;
                productId = 0x28E;
            }
        }
        
        StringHelper::TrimString(gamepadName);
        
        GamepadPtr newGamepad = GamepadManager::GetInstancePtr()->attachGamepad(
            (U32)_event->index,
            gamepadName,
            vendorId,
            productId,
            _event->numAxes,
            _event->numButtons);

        if (!newGamepad)
            return nullptr;

        for (Size i = 0; i < (Size)_event->numAxes; ++i)
            newGamepad->setAxisState((S32)i, _event->axis[i]);

        for (Size i = 0; i < (Size)_event->numButtons; ++i)
            newGamepad->setButtonState((S32)i, _event->digitalButton[i] != 0);

        return newGamepad;
    }
    
    
    
    //////////////////////////////////////////
    // Class GamepadProviderEmscripten
    //
    //////////////////////////////////////////
    GamepadProviderEmscripten::GamepadProviderEmscripten()
    {
    }

    //////////////////////////////////////////
    GamepadProviderEmscripten::~GamepadProviderEmscripten()
    {
        
    }

    //////////////////////////////////////////
    GamepadProviderEmscriptenPtr GamepadProviderEmscripten::Create()
    {
        GamepadProviderEmscriptenPtr gamepad;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            GamepadProviderEmscripten,
            gamepad,
            init());
        return gamepad;
    }

    //////////////////////////////////////////
    bool GamepadProviderEmscripten::init()
    {
        if (!GamepadProvider::init())
            return false;
        
        if (emscripten_sample_gamepad_data() != EMSCRIPTEN_RESULT_SUCCESS)
        {
            Debug::Log("Gamepads API is not supported");
            return false;
        }

        emscripten_set_gamepadconnected_callback(this, 0, OnGamepadEvent);
        emscripten_set_gamepaddisconnected_callback(this, 0, OnGamepadEvent);


        return true;
    }

    //////////////////////////////////////////
    void GamepadProviderEmscripten::update(F32 _dt)
    {
        MAZE_PROFILER_SCOPED_LOCK(GAMEPADS);
        MAZE_PROFILE_EVENT("GamepadProviderEmscripten::update");
        
        if (emscripten_sample_gamepad_data() == EMSCRIPTEN_RESULT_SUCCESS)
        {
            S32 numGamepads = emscripten_get_num_gamepads();
            
            for(S32 i = 0; i < numGamepads && i < MAZE_EMSCRIPTEN_MAX_GAMEPADS_COUNT; ++i)
            {
                EmscriptenGamepadEvent gamepadEvent;
                S32 ret = emscripten_get_gamepad_status(i, &gamepadEvent);
                
                if (ret == EMSCRIPTEN_RESULT_SUCCESS)
                {
                    U32 gamepadIndex = (U32)gamepadEvent.index;
                    for(S32 j = 0; j < gamepadEvent.numAxes; ++j)
                    {
                        if (m_cachedGamepadStates[i].axis[j] != gamepadEvent.axis[j])
                        {
                            m_cachedGamepadStates[i].axis[j] = gamepadEvent.axis[j];
                            
                            GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadByDeviceId(gamepadIndex);
                            if (gamepad)
                            {
                                gamepad->setAxisState(j, gamepadEvent.axis[j]);
                            }
                            else
                            {
                                GamepadPtr newGamepad = AttachGamepad(&gamepadEvent);
                                if (newGamepad)
                                    newGamepad->setAxisState(j, gamepadEvent.axis[j]);
                            }
                        }
                    }
                    
                    for(S32 j = 0; j < gamepadEvent.numButtons; ++j)
                    {
                        if (m_cachedGamepadStates[i].analogButton[j] != gamepadEvent.analogButton[j] || m_cachedGamepadStates[i].digitalButton[j] != gamepadEvent.digitalButton[j])
                        {
                            m_cachedGamepadStates[i].analogButton[j] = gamepadEvent.analogButton[j];
                            m_cachedGamepadStates[i].digitalButton[j] = gamepadEvent.digitalButton[j];
                            
                            bool buttonState = (gamepadEvent.digitalButton[j] != 0);
                            
                            GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadByDeviceId(gamepadIndex);
                            if (gamepad)
                            {
                                gamepad->setButtonState(j, buttonState);
                            }
                            else
                            {
                                GamepadPtr newGamepad = AttachGamepad(&gamepadEvent);
                                if (newGamepad)
                                    newGamepad->setButtonState(j, buttonState);
                            }                            
                        }
                    }
                }
            }
                
        }
    }

    //////////////////////////////////////////
    void GamepadProviderEmscripten::detectGamepads()
    {
        // We detect them already in update
    }
    
    //////////////////////////////////////////
    EM_BOOL GamepadProviderEmscripten::processGamepadEvent(S32 _eventType, EmscriptenGamepadEvent const* _event)
    {
        
        switch (_eventType)
        {
            case EMSCRIPTEN_EVENT_GAMEPADCONNECTED:
            {
                AttachGamepad(_event);
                break;
            }
            case EMSCRIPTEN_EVENT_GAMEPADDISCONNECTED:
            {
                GamepadManager::GetInstancePtr()->detachGamepad((U32)_event->index);
                break;
            }
            default:
                break;
        }
        
        return 0;
    }
    

    
} // namespace Maze
//////////////////////////////////////////
