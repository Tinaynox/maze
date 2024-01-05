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
#if (!defined(_MazeBinarySerialization_hpp_))
#define _MazeBinarySerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Forward declaration
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize);

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data);

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void GetValueSerializationSize(SharedPtr<UValue> const& _value, U32& _outSize);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void SerializeValue(SharedPtr<UValue> const& _value, U8* _data);
    
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        DeserializeValue(SharedPtr<UValue>& _value, U8 const* _data);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        DeserializeValue(SharedPtr<UValue>& _value, U8 const* _data);


    //////////////////////////////////////////
    // Type: Any base type (non-class)
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        _outSize = sizeof(_value);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(_value));
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(_value));
    }


    //////////////////////////////////////////
    // Type: IBinarySerializable
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void GetValueSerializationSize(IBinarySerializable const& _value, U32& _outSize)
    {
        _outSize = _value.getValueSerializationSize();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void SerializeValue(IBinarySerializable const& _value, U8* _data)
    {
        _value.serialize(_data);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void DeserializeValue(IBinarySerializable& _value, U8 const* _data)
    {
        _value.deserialize(_data);
    }


    //////////////////////////////////////////
    // Type: String
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        _outSize = 0;
        _outSize += sizeof(U32);
        _outSize += (U32)_value.size();
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        U32 containerSize = (U32)_value.size();
        SerializeValue(containerSize, _data);
        _data += sizeof(containerSize);

        memcpy(_data, (U8 const*)&_value[0], containerSize);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        U32 containerSize;
        DeserializeValue(containerSize, _data);
        _data += sizeof(containerSize);

        _value.resize(containerSize);
        memcpy((U8*)&_value[0], _data, containerSize);
    }


    //////////////////////////////////////////
    // Type: HashedString
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void GetValueSerializationSize(HashedString const& _value, U32& _outSize)
    {
        GetValueSerializationSize(_value.getString(), _outSize);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void SerializeValue(HashedString const& _value, U8* _data)
    {
        SerializeValue(_value.getString(), _data);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void DeserializeValue(HashedString& _value, U8 const* _data)
    {
        String str;
        DeserializeValue(str, _data);
        _value = HashedString(str);
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        GetValueSerializationSize(_value, _outSize);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        SerializeValue(_value, _data);
    }
    
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        DeserializeValue(_value, _data);
    }


    //////////////////////////////////////////
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void GetContainerMetaPropertySerializationSize(
        TIterator _first,
        TIterator const _last,
        U32& _outSize);

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void SerializeContainerMetaProperty(
        TIterator _first,
        TIterator const _last,
        U32 _containerSize,
        U8* _data);

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void DeserializeContainerMetaProperty(
        TIterator _it,
        U8 const* _data);


    //////////////////////////////////////////
    // Type: Vector types
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        GetContainerMetaPropertySerializationSize<
            typename TValue::const_iterator,
            typename TValue::value_type>(
                _value.begin(),
                _value.end(),
                _outSize);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        SerializeContainerMetaProperty<
            typename TValue::const_iterator,
            typename TValue::value_type>(
                _value.begin(),
                _value.end(),
                (U32)_value.size(),
                _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        DeserializeContainerMetaProperty<
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
    struct HasGetValueSerializationSize : std::false_type { };
    template <typename T>
    struct HasGetValueSerializationSize <T, decltype(Maze::GetValueSerializationSize(std::declval<T>(), std::declval<U32&>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasSerializeValue : std::false_type { };
    template <typename T>
    struct HasSerializeValue <T, decltype(Maze::SerializeValue(std::declval<T>(), std::declval<U8*>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasDeserializeValue : std::false_type { };
    template <typename T>
    struct HasDeserializeValue <T, decltype(Maze::DeserializeValue(std::declval<T&>(), std::declval<U8 const*>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    // Try functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryGetValueSerializationSize(typename ::std::enable_if<(
        HasGetValueSerializationSize<TValue>::value), TValue>::type const& _value, U32& _outSize)
    {
        GetValueSerializationSize(_value, _outSize);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryGetValueSerializationSize(typename ::std::enable_if<(
        !HasGetValueSerializationSize<TValue>::value), TValue>::type const& _value, U32& _outSize)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline bool TrySerializeValue(typename ::std::enable_if<(
        HasSerializeValue<TValue>::value), TValue>::type const& _value, U8* _data)
    {
        SerializeValue(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TrySerializeValue(typename ::std::enable_if<(
        !HasSerializeValue<TValue>::value), TValue>::type const& _value, U8* _data)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryDeserializeValue(typename ::std::enable_if<(
        HasDeserializeValue<TValue>::value), TValue>::type& _value, U8 const* _data)
    {
        DeserializeValue(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryDeserializeValue(typename ::std::enable_if<(
        !HasDeserializeValue<TValue>::value), TValue>::type& _value, U8 const* _data)
    {
        return false;
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void GetValueSerializationSize(SharedPtr<UValue> const& _value, U32& _outSize)
    {
        TryGetValueSerializationSize<UValue>(*_value.get(), _outSize);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void SerializeValue(SharedPtr<UValue> const& _value, U8* _data)
    {
        TrySerializeValue<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        DeserializeValue(SharedPtr<UValue>& _value, U8 const* _data)
    {
        if (!_value)
            _value = std::make_shared<UValue>();

        TryDeserializeValue<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        DeserializeValue(SharedPtr<UValue>& _value, U8 const* _data)
    {
        MAZE_ERROR_RETURN_IF(!_value, "%s class has no default constructor to make shared ptr", static_cast<CString>(ClassInfo<UValue>::Name()));
        TryDeserializeValue<UValue>(*_value.get(), _data);
    }


    //////////////////////////////////////////
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void GetContainerMetaPropertySerializationSize(
        TIterator _first,
        TIterator const _last,
        U32& _outSize)
    {
        _outSize = 0;

        // Container elements count
        _outSize += sizeof(U32);

        for (TIterator it = _first; it != _last; ++it)
        {
            // Element size
            _outSize += sizeof(U32);

            // Element
            TValue const& childValue = (*it);
            U32 valueSize;
            TryGetValueSerializationSize<TValue>(childValue, valueSize);
            _outSize += valueSize;
        }
    }

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void SerializeContainerMetaProperty(
        TIterator _first,
        TIterator const _last,
        U32 _containerSize,
        U8* _data)
    {
        SerializeValue(_containerSize, _data);
        _data += sizeof(_containerSize);

        for (TIterator it = _first; it != _last; ++it)
        {
            TValue const& childValue = (*it);

            U32 sizeBytes = 0u;
            TryGetValueSerializationSize<TValue>(childValue, sizeBytes);
            SerializeValue(sizeBytes, _data);
            _data += sizeof(sizeBytes);

            TrySerializeValue<TValue>(childValue, _data);
            _data += sizeBytes;
        }

    }

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void DeserializeContainerMetaProperty(
        TIterator _it,
        U8 const* _data)
    {
        U32 containerSize;

        DeserializeValue(containerSize, _data);
        _data += sizeof(containerSize);

        for (U32 i = 0; i < containerSize; ++i)
        {
            U32 sizeBytes;
            DeserializeValue(sizeBytes, _data);
            _data += sizeof(sizeBytes);

            TValue value;
            TryDeserializeValue<TValue>(value, _data);
            _data += sizeBytes;

            *_it++ = value;
        }
    }
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeBinarySerialization_hpp_
