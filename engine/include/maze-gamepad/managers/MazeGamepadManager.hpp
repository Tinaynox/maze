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
#if (!defined(_MazeGamepadManager_hpp_))
#define _MazeGamepadManager_hpp_


//////////////////////////////////////////
#include "maze-gamepad/MazeGamepadHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GamepadManager);
    MAZE_USING_SHARED_PTR(Gamepad);
    MAZE_USING_SHARED_PTR(GamepadProvider);


    //////////////////////////////////////////
    // Class GamepadManager
    //
    //////////////////////////////////////////
    class MAZE_GAMEPAD_API GamepadManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        static U32 const s_virtualGamepadDeviceId;

    public:

        //////////////////////////////////////////
        using GamepadListByDeviceId = Map<U32, GamepadPtr>;


    public:

        //////////////////////////////////////////
        virtual ~GamepadManager();

        //////////////////////////////////////////
        static void Initialize(GamepadManagerPtr& _gamepadManager);


        //////////////////////////////////////////
        static inline GamepadManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline GamepadManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void detectGamepads();

        //////////////////////////////////////////
        GamepadPtr attachGamepad(
            U32 _deviceId,
            String const& _name,
            U32 _vendorId,
            U32 _productId,
            U32 _axesCount,
            U32 _buttonsCount);

        //////////////////////////////////////////
        void detachGamepad(U32 _deviceId);

        //////////////////////////////////////////
        inline GamepadPtr const& getGamepadByDeviceId(U32 _deviceId) const
        {
            static GamepadPtr nullPointer;
            GamepadListByDeviceId::const_iterator it = m_gamepads.find(_deviceId);
            if (it == m_gamepads.end())
                return nullPointer;
            
            return (*it).second;
        }

        //////////////////////////////////////////
        GamepadPtr getGamepadByVendorAndProduct(U32 _vendorId, U32 _productId) const
        {
            Vector<GamepadPtr> results;
            for (auto& gamepadData : m_gamepads)
                if (gamepadData.second->getVendorId() == _vendorId && gamepadData.second->getProductId() == _productId)
                    results.emplace_back(gamepadData.second);

            if (results.empty())
                return nullptr;

            std::sort(
                results.begin(),
                results.end(),
                [](GamepadPtr const& _gamepad0, GamepadPtr const& _gamepad1)
                {
                    if (_gamepad0->getConnected() && !_gamepad1->getConnected())
                        return true;
                    return false;
                });

            return results.front();
        }

        //////////////////////////////////////////
        inline GamepadPtr const& getFirstActiveGamepad() const
        {
            static GamepadPtr nullPointer;
            for (auto& gamepadData : m_gamepads)
                if (gamepadData.second->getConnected())
                    return gamepadData.second;

            return nullPointer;
        }

        //////////////////////////////////////////
        GamepadPtr const& getGamepadById(U32 _id) const;

        //////////////////////////////////////////
        Vector<GamepadPtr> getGamepadsByProductId(U32 _vid, U32 _pid) const;

        //////////////////////////////////////////
        GamepadListByDeviceId const& getGamepads() const { return m_gamepads; }

        //////////////////////////////////////////
        Size getGamepadsCount() const { return m_gamepads.size(); }

        //////////////////////////////////////////
        GamepadPtr const& getVirtualGamepad() const { return m_virtualGamepad; }

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventGamepadsChanged;
        MultiDelegate<GamepadPtr const&> eventGamepadAdded;
        MultiDelegate<GamepadPtr const&> eventGamepadWillBeRemoved;

    protected:

        //////////////////////////////////////////
        GamepadManager();

        //////////////////////////////////////////
        bool init();

    protected:
        static GamepadManager* s_instance;

        GamepadListByDeviceId m_gamepads;
        GamepadPtr m_virtualGamepad;

        Vector<GamepadProviderPtr> m_gamepadProviders;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGamepadManager_hpp_
//////////////////////////////////////////
