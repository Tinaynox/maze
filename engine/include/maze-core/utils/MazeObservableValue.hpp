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
#if (!defined(_MazeObservableValue_hpp_))
#define _MazeObservableValue_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    template<typename TValue>
    class ObservableValue
    {
    public:

        //////////////////////////////////////////
        ObservableValue()
        {}

        //////////////////////////////////////////
        ObservableValue(TValue const& _value)
            : m_value(_value)
        {}

        //////////////////////////////////////////
        ObservableValue(TValue&& _value)
            : m_value(std::move(_value))
        {}

        //////////////////////////////////////////
        inline ObservableValue<TValue>& operator=(TValue const& _value)
        {
            setValue(_value);
            return *this;
        }

        //////////////////////////////////////////
        inline operator TValue() const { return m_value; }

        //////////////////////////////////////////
        inline TValue const& getValue() const { return m_value; }

        //////////////////////////////////////////
        inline void setValue(TValue const& _value)
        {
            if (m_value == _value)
                return;

            setValueWithoutNotify(_value);
            eventValueChanged(_value);
        }

        //////////////////////////////////////////
        inline void setValueWithoutNotify(TValue const& _value)
        {
            m_value = _value;
        }

        //////////////////////////////////////////
        inline void removeAllListeners()
        {
            eventValueChanged.clear();
        }

    public:
        MultiDelegate<TValue const&> eventValueChanged;

    private:
        TValue m_value;
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeObservableValue_hpp_
