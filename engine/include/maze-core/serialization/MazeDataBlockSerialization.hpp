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
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeTMat.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/math/MazePlane.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/math/MazeRay.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/serialization/MazeStringSerialization.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/utils/MazeManagedSharedPtr.hpp"
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
    template <typename UValue>
    MAZE_FORCEINLINE void ValueToDataBlock(ManagedSharedPtr<UValue> const& _value, DataBlock& _data);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(ManagedSharedPtr<UValue>& _value, DataBlock const& _data);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(ManagedSharedPtr<UValue>& _value, DataBlock const& _data);



    //////////////////////////////////////////
    // Type: DataBlock
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsDataBlock<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        _data = _value;
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsDataBlock<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        _value = _data;
    }


    //////////////////////////////////////////
    // Type: Any base type (non-class, non-enum)
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(
        !std::is_class<TValue>::value && !std::is_enum<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        _data.clearData();
        AddDataToDataBlock<TValue>(_data, MAZE_HCS("value"), _value);
    }

    ////////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(
        !std::is_class<TValue>::value && !std::is_enum<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        DataBlock::ParamIndex paramIndex = _data.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
            _value = GetDataBlockParam<TValue>(_data, paramIndex);
    }


    //////////////////////////////////////////
    // Type: VecN and MatN types
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(
        IsVec2<TValue>::value || IsVec3<TValue>::value || IsVec4<TValue>::value ||
        IsMat3<TValue>::value || IsMat4<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        _data.clearData();
        AddDataToDataBlock<TValue>(_data, MAZE_HCS("value"), _value);
    }

    ////////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(
        IsVec2<TValue>::value || IsVec3<TValue>::value || IsVec4<TValue>::value ||
        IsMat3<TValue>::value || IsMat4<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        DataBlock::ParamIndex paramIndex = _data.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
            _value = GetDataBlockParam<TValue>(_data, paramIndex);
    }


    //////////////////////////////////////////
    // Type: TMat
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsTMat<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        _data.clearData();
        AddDataToDataBlock<TValue>(_data, MAZE_HCS("value"), _value);
    }

    ////////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsTMat<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        DataBlock::ParamIndex paramIndex = _data.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
            _value = GetDataBlockParam<TValue>(_data, paramIndex);
    }


    //////////////////////////////////////////
    // Type: String
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        _data.clearData();
        AddDataToDataBlock<TValue>(_data, MAZE_HCS("value"), _value);
    }

    ////////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        DataBlock::ParamIndex paramIndex = _data.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
            _value = GetDataBlockParam<TValue>(_data, paramIndex);
    }


    //////////////////////////////////////////
    // Type: C++ enum
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(
        !std::is_class<TValue>::value && std::is_enum<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        _data.clearData();
        AddDataToDataBlock<S32>(_data, MAZE_HCS("value"), static_cast<S32>(_value));
    }

    ////////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(
        !std::is_class<TValue>::value && std::is_enum<TValue>::value), void>::type
        ValueFromDataBlock(TValue& _value, DataBlock const& _data)
    {
        DataBlock::ParamIndex paramIndex = _data.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
            _value = static_cast<TValue>(GetDataBlockParam<S32>(_data, paramIndex));
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
    // Type: EntityId
    //
    //////////////////////////////////////////
    inline void ValueToDataBlock(EntityId const& _value, DataBlock& _data)
    {
        ValueToDataBlock((S32)_value, _data);
    }

    //////////////////////////////////////////
    inline void ValueFromDataBlock(EntityId& _value, DataBlock const& _data)
    {
        S32 eid;
        ValueFromDataBlock(eid, _data);
        _value = EntityId(eid);
    }


    //////////////////////////////////////////
    // Type: Rotation2D
    //
    //////////////////////////////////////////
    inline void ValueToDataBlock(Rotation2D const& _value, DataBlock& _data)
    {
        _data.setF32(MAZE_HCS("s"), _value.getSin());
        _data.setF32(MAZE_HCS("c"), _value.getCos());
    }

    //////////////////////////////////////////
    inline void ValueFromDataBlock(Rotation2D& _value, DataBlock const& _data)
    {
        _value = Rotation2D(_data.getF32(MAZE_HCS("s")), _data.getF32(MAZE_HCS("c")));
    }


    //////////////////////////////////////////
    // Type: Plane
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Plane const& _value, DataBlock& _data)
    {
        _data.setVec3F(MAZE_HCS("point"), _value.getPoint());
        _data.setVec3F(MAZE_HCS("normal"), _value.getNormal());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Plane& _value, DataBlock const& _data)
    {
        _value.setPoint(_data.getVec3F(MAZE_HCS("point")));
        _value.setNormal(_data.getVec3F(MAZE_HCS("normal")));
    }


    //////////////////////////////////////////
    // Type: Quaternion
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Quaternion const& _value, DataBlock& _data)
    {
        _data.setVec4F(MAZE_HCS("value"), _value.toVec4F());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Quaternion& _value, DataBlock const& _data)
    {
        _value.setVec4F(_data.getVec4F(MAZE_HCS("value")));
    }


    //////////////////////////////////////////
    // Type: Ray
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Ray const& _value, DataBlock& _data)
    {
        _data.setVec3F(MAZE_HCS("point"), _value.getPoint());
        _data.setVec3F(MAZE_HCS("direction"), _value.getDirection());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Ray& _value, DataBlock const& _data)
    {
        _value.setPoint(_data.getVec3F(MAZE_HCS("point")));
        _value.setDirection(_data.getVec3F(MAZE_HCS("direction")));
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
    // Type: ManagedSharedPtr
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsManagedSharedPtr<TValue>::value), void>::type
        ValueToDataBlock(TValue const& _value, DataBlock& _data)
    {
        ValueToDataBlock(_value, _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsManagedSharedPtr<TValue>::value), void>::type
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
        _data.clearData();
        for (TIterator it = _first; it != _last; ++it)
        {
            AddDataToDataBlock(_data, MAZE_HCS("item"), (*it));
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
            _value = MakeShared<UValue>();

        if (!TryValueFromDataBlock<UValue>(*_value.get(), _data))
            _value.reset();
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
    // Type: ManagedSharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void ValueToDataBlock(ManagedSharedPtr<UValue> const& _value, DataBlock& _data)
    {
        if (_value)
            TryValueToDataBlock<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(ManagedSharedPtr<UValue>& _value, DataBlock const& _data)
    {
        if (!_value)
            _value = MakeShared<UValue>();

        TryValueFromDataBlock<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromDataBlock(ManagedSharedPtr<UValue>& _value, DataBlock const& _data)
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
        if MAZE_CONSTEXPR17 (dataBlockParamType != DataBlockParamType::None)
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
