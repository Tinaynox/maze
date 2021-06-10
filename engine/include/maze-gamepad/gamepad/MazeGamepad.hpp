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
#if (!defined(_MazeGamepad_hpp_))
#define _MazeGamepad_hpp_


//////////////////////////////////////////
#include "maze-gamepad/MazeGamepadHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Gamepad);


    //////////////////////////////////////////
    // Class Gamepad
    //
    //////////////////////////////////////////
    class MAZE_GAMEPAD_API Gamepad
    {
    public:

        //////////////////////////////////////////
        ~Gamepad();

        //////////////////////////////////////////
        static GamepadPtr Create(
            U32 _deviceId,
            String const& _description,
            S32 _vendorId,
            S32 _productId,
            U32 _axesCount,
            U32 _buttonsCount);

        //////////////////////////////////////////
        void setAxesAndButtonsCount(
            Size _axesCount,
            Size _buttonsCount);

        //////////////////////////////////////////
        inline U32 getId() const { return m_id; }

        //////////////////////////////////////////
        inline void setDeviceId(U32 _deviceId) { m_deviceId = _deviceId; }

        //////////////////////////////////////////
        inline U32 getDeviceId() const { return m_deviceId; }

        //////////////////////////////////////////
        void setConnected(bool _connected);

        //////////////////////////////////////////
        inline bool getConnected() const { return m_connected; }

        //////////////////////////////////////////
        inline U32 getIconSymbol() const { return m_iconSymbol; }

        //////////////////////////////////////////
        inline String const& getDescription() const { return m_description; }

        //////////////////////////////////////////
        String getDescriptionWithIcons();

        //////////////////////////////////////////
        inline S32 getVendorId() const { return m_vendorId; }

        //////////////////////////////////////////
        inline S32 getProductId() const { return m_productId; }

        //////////////////////////////////////////
        inline Size getAxesCount() const { return m_axesCount; }

        //////////////////////////////////////////
        inline Size getButtonsCount() const { return m_buttonsCount; }

        //////////////////////////////////////////
        void setAxisState(S32 _axisId, F32 _axisState);

        //////////////////////////////////////////
        inline F32 getAxisState(S32 _axisId) { return m_axisStates[_axisId]; }


        //////////////////////////////////////////
        void setButtonState(S32 _buttonId, bool _buttonState);

        //////////////////////////////////////////
        inline bool getButtonState(S32 _buttonId) { return m_buttonStates[_buttonId]; }

    public:

        //////////////////////////////////////////
        MultiDelegate<S32> eventAxisEvent;
        MultiDelegate<S32> eventButtonEvent;

    protected:

        //////////////////////////////////////////
        Gamepad();

        //////////////////////////////////////////
        bool init(
            U32 _deviceId,
            String const& _description,
            S32 _vendorId,
            S32 _productId,
            Size _axesCount,
            Size _buttonsCount);

    protected:
        U32 m_id;
        U32 m_deviceId;
        bool m_connected;
        U32 m_iconSymbol;
        String m_description;

        S32 m_vendorId;
        S32 m_productId;

        Size m_axesCount;
        Size m_buttonsCount;

        Vector<F32> m_axisStates;
        Vector<bool> m_buttonStates;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGamepad_hpp_
//////////////////////////////////////////
