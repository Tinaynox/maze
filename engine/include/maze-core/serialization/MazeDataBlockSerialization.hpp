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
#if (!defined(_MazeDataBlockSerialization_hpp_))
#define _MazeDataBlockSerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/serialization/MazeStringSerialization.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include <utility>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Forward declaration
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data);

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data);

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE bool AddDataToDataBlock(DataBlock& _dataBlock, HashedCString _name, TValue const& _value);

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE TValue GetDataBlockParam(DataBlock const& _data, DataBlock::ParamIndex _i);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void ValueToDataBlock(SharedPtr<UValue> const& _value, DataBlock& _data);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(SharedPtr<UValue>& _value, DataBlock const& _data);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(SharedPtr<UValue>& _value, DataBlock const& _data);



    //////////////////////////////////////////
    // Type: Any base type (non-class)
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        AddDataToDataBlock<TValue>(_data, MAZE_HS("value"), _value);
    }

    ////////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        DataBlock::ParamIndex paramIndex = _data.findParamIndex(MAZE_HS("value"));
        if (paramIndex >= 0)
            _value = GetDataBlockParam<TValue>(_data, paramIndex);
    }


    //////////////////////////////////////////
    // Type: IDataBlockSerializable
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(IDataBlockSerializable const& _value, DataBlock& _data)
    {
        _value.toDataBlock(_data);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(IDataBlockSerializable& _value, DataBlock const& _data)
    {
        _value.loadFromDataBlock(_data);
    }


    //////////////////////////////////////////
    // Type: Rotation2D
    //
    //////////////////////////////////////////
    inline void ValueToDataBlock(Rotation2D const& _value, DataBlock& _data)
    {
        _data.setF32(MAZE_HS("s"), _value.getSin());
        _data.setF32(MAZE_HS("c"), _value.getCos());
    }

    //////////////////////////////////////////
    inline void ValueFromDataBlock(Rotation2D& _value, DataBlock const& _data)
    {
        _value = Rotation2D(_data.getF32(MAZE_HS("s")), _data.getF32(MAZE_HS("c")));
    }


    //////////////////////////////////////////
    // Type: DataBlock
    //
    //////////////////////////////////////////
    inline void ValueToDataBlock(DataBlock const& _value, DataBlock& _data)
    {
        _data.copyFrom(&_value);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline void ValueFromDataBlock(DataBlock& _value, DataBlock const& _data)
    {
        _value.copyFrom(&_data);
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        ValueToDataBlock(_value, _data);
    }
    
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        ValueFromDataBlock(_value, _data);
    }


    //////////////////////////////////////////
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void ContainerMetaPropertyToDataBlock(
        TIterator _first,
        TIterator const _last,
        DataBlock& _data)
    {
        for (TIterator it = _first; it != _last; ++it)
        {
            AddDataToDataBlock(_data, MAZE_HS("item"), (*it));
        }
    }


    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void ContainerMetaPropertyFromDataBlock(
        TIterator _it,
        DataBlock const& _data);


    //////////////////////////////////////////
    // Vector redirect functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        ContainerMetaPropertyToDataBlock<
            typename TValue::const_iterator,
            typename TValue::value_type>(
                _value.begin(),
                _value.end(),
                _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        ContainerMetaPropertyFromDataBlock<
            std::back_insert_iterator<TValue>,
            typename TValue::value_type>(
                std::back_inserter(_value),
                _data);
    }


    //////////////////////////////////////////
    // Utils
    //
    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueFromDataBlock : std::false_type { };
    template <typename T>
    struct HasValueFromDataBlock <T, decltype(Maze::ValueFromDataBlock(std::declval<T&>(), std::declval<DataBlock>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueToDataBlock : std::false_type { };
    template <typename T>
    struct HasValueToDataBlock <T, decltype(Maze::ValueToDataBlock(std::declval<T>(), std::declval<DataBlock&>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    // Try functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueFromDataBlock(typename ::std::enable_if<(
        HasValueFromDataBlock<TValue>::value), TValue>::type& _value, DataBlock const& _data)
    {
        ValueFromDataBlock(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromDataBlock(typename ::std::enable_if<(
        !HasValueFromDataBlock<TValue>::value), TValue>::type& _value, DataBlock const& _data)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToDataBlock(typename ::std::enable_if<(
        HasValueToDataBlock<TValue>::value), TValue>::type const& _value, DataBlock& _data)
    {
        ValueToDataBlock(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToDataBlock(typename ::std::enable_if<(
        !HasValueToDataBlock<TValue>::value), TValue>::type const& _value, DataBlock& _data)
    {
        return false;
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void ValueToDataBlock(SharedPtr<UValue> const& _value, DataBlock& _data)
    {
        if (_value)
            TryValueToDataBlock<UValue>(*_value.get(), _data);
    }
    
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(SharedPtr<UValue>& _value, DataBlock const& _data)
    {
        if (!_value)
            _value = std::make_shared<UValue>();

        TryValueFromDataBlock<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(SharedPtr<UValue>& _value, DataBlock const& _data)
    {
        MAZE_ERROR_RETURN_IF(!_value, "%s class has no default constructor to make shared ptr", static_cast<CString>(ClassInfo<UValue>::Name()));
        TryValueFromDataBlock<UValue>(*_value.get(), _data);
    }


    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void ContainerMetaPropertyFromDataBlock(
        TIterator _it,
        DataBlock const& _data)
    {
        MAZE_CONSTEXPR DataBlockParamType dataBlockParamType = DataBlock::TypeOf<TValue>::type;
        if MAZE_CONSTEXPR14(dataBlockParamType != DataBlockParamType::None)
        {
            for (DataBlock::ParamIndex i = 0; i < _data.getParamsCount(); ++i)
            {
                *_it++ = GetDataBlockParam<TValue>(_data, i);
            }
        }
        else
        {
            if (_data.getDataBlocksCount() > 0)
            {
                for (DataBlock const* dataBlock : _data)
                {
                    TValue value;
                    if (TryValueFromDataBlock<TValue>(value, *dataBlock))
                        *_it++ = value;
                }
            }
            else
            {
                for (DataBlock::ParamIndex i = 0; i < (DataBlock::ParamIndex)_data.getParamsCount(); ++i)
                {
                    TValue value;
                    String const& string = _data.getString(i);
                    if (TryValueFromString<TValue>(value, string.c_str(), string.size()))
                        *_it++ = value;
                }
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeDataBlockSerialization.inl"


//////////////////////////////////////////
#endif // _MazeDataBlockSerialization_hpp_
