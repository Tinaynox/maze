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
    // Utils
    //
    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasGetValueSerializationSize : std::false_type { };
    template <typename T>
    struct HasGetValueSerializationSize <T, decltype(GetValueSerializationSize(std::declval<T>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasSerializeValue : std::false_type { };
    template <typename T>
    struct HasSerializeValue <T, decltype(SerializeValue(std::declval<T>(), std::declval<U8*>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasDeserializeValue : std::false_type { };
    template <typename T>
    struct HasDeserializeValue <T, decltype(DeserializeValue(std::declval<T>(), std::declval<U8 const*>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    // Try functions (Forward declaration)
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryGetValueSerializationSize(typename ::std::enable_if<(HasGetValueSerializationSize<TValue>::value), TValue>::type const& _value);

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryGetValueSerializationSize(typename ::std::enable_if<(std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value);

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryGetValueSerializationSize(typename ::std::enable_if<(
        !HasGetValueSerializationSize<TValue>::value &&
        !std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value);


    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TrySerializeValue(typename ::std::enable_if<(HasSerializeValue<TValue>::value), TValue>::type const& _value, U8* _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TrySerializeValue(typename ::std::enable_if<(std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value, U8* _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TrySerializeValue(typename ::std::enable_if<(
        !HasSerializeValue<TValue>::value &&
        !std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value, U8* _data);


    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryDeserializeValue(typename ::std::enable_if<(HasDeserializeValue<TValue>::value), TValue>::type& _value, U8 const* _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryDeserializeValue(typename ::std::enable_if<(std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type& _value, U8 const* _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryDeserializeValue(typename ::std::enable_if<(
        !HasDeserializeValue<TValue>::value &&
        !std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type& _value, U8 const* _data);



    //////////////////////////////////////////
    // Type: Any base type (non-class)
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!std::is_class<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(_value);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(_value));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(_value));
    }


    //////////////////////////////////////////
    // Type: IBinarySerializable
    //
    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(IBinarySerializable const& _value)
    {
        return _value.getValueSerializationSize();
    }

    //////////////////////////////////////////
    inline void SerializeValue(IBinarySerializable const& _value, U8* _data)
    {
        _value.serialize(_data);
    }

    //////////////////////////////////////////
    inline void DeserializeValue(IBinarySerializable& _value, U8 const* _data)
    {
        _value.deserialize(_data);
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline U32 GetValueSerializationSize(SharedPtr<UValue> const& _value)
    {
        return TryGetValueSerializationSize<UValue>(*_value.get());
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return GetValueSerializationSize(_value);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void SerializeValue(SharedPtr<UValue> const& _value, U8* _data)
    {
        TrySerializeValue<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        SerializeValue(_value, _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        DeserializeValue(SharedPtr<UValue>& _value, U8 const* _data)
    {
        if (!_value)
            _value = std::make_shared<UValue>();

        TryDeserializeValue<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        DeserializeValue(SharedPtr<UValue>& _value, U8 const* _data)
    {
        MAZE_ERROR_RETURN_IF(!_value, "%s class has no default constructor to make shared ptr", static_cast<CString>(ClassInfo<UValue>::Name()));
        TryDeserializeValue<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        DeserializeValue(_value, _data);
    }


    //////////////////////////////////////////
    // Type: std::function
    //
    //////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), U32>::type
    //    GetValueSerializationSize(TValue const& _value)
    //{
    //    return 0;
    //}

    ////////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
    //    SerializeValue(TValue const& _value, U8* _data)
    //{}

    ////////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
    //    DeserializeValue(TValue& _value, U8 const* _data)
    //{}


    //////////////////////////////////////////
    // Type: String
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsString<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        U32 result = 0;
        result += sizeof(U32);
        result += (U32)_value.size();
        return result;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsString<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        U32 containerSize = (U32)_value.size();
        SerializeValue(containerSize, _data);
        _data += sizeof(containerSize);

        memcpy(_data, (U8 const*)&_value[0], containerSize);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsString<TValue>::value), void>::type
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
    inline U32 GetValueSerializationSize(HashedString const& _value)
    {
        return GetValueSerializationSize(_value.getString());
    }

    //////////////////////////////////////////
    inline void SerializeValue(HashedString const& _value, U8* _data)
    {
        SerializeValue(_value.getString(), _data);
    }

    //////////////////////////////////////////
    inline void DeserializeValue(HashedString& _value, U8 const* _data)
    {
        String str;
        DeserializeValue(str, _data);
        _value = HashedString(str);
    }


    //////////////////////////////////////////
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline U32 GetContainerMetaPropertySerializationSize(
        TIterator _first,
        TIterator const _last)
    {
        U32 result = 0;
        
        // Container elements count
        result += sizeof(U32);

        for (TIterator it = _first; it != _last; ++it)
        {
            // Element size
            result += sizeof(U32);

            // Element
            TValue const& childValue = (*it);
            result += TryGetValueSerializationSize<TValue>(childValue);
        }

        return result;
    }

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline void SerializeContainerMetaProperty(
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

            U32 sizeBytes = TryGetValueSerializationSize<TValue>(childValue);
            SerializeValue(sizeBytes, _data);
            _data += sizeof(sizeBytes);

            TrySerializeValue<TValue>(childValue, _data);
            _data += sizeBytes;
        }
    }

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline void DeserializeContainerMetaProperty(
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

    //////////////////////////////////////////
    // Type: Vector
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline U32 GetValueSerializationSize(Vector<UValue> const& _value)
    {
        return GetContainerMetaPropertySerializationSize<
            typename Vector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end());
    }

    

    //////////////////////////////////////////
    template <typename UValue>
    inline void SerializeValue(
        Vector<UValue> const& _value,
        U8* _data)
    {
        return SerializeContainerMetaProperty<
            typename Vector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end(),
                (U32)_value.size(),
                _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void DeserializeValue(Vector<UValue>& _value, U8 const* _data)
    {
        DeserializeContainerMetaProperty<
            std::back_insert_iterator<Vector<UValue>>,
            UValue>(
                std::back_inserter(_value),
                _data);
    }


    //////////////////////////////////////////
    // Type: FastVector
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline U32 GetValueSerializationSize(FastVector<UValue> const& _value)
    {
        return GetContainerMetaPropertySerializationSize<
            typename FastVector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end());
    }



    //////////////////////////////////////////
    template <typename UValue>
    inline void SerializeValue(
        FastVector<UValue> const& _value,
        U8* _data)
    {
        return SerializeContainerMetaProperty<
            typename FastVector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end(),
                (U32)_value.size(),
                _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void DeserializeValue(FastVector<UValue>& _value, U8 const* _data)
    {
        DeserializeContainerMetaProperty<
            std::back_insert_iterator<FastVector<UValue>>,
            UValue>(
                std::back_inserter(_value),
                _data);
    }


    //////////////////////////////////////////
    // Vector redirect functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return GetValueSerializationSize(_value);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        SerializeValue(_value, _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        DeserializeValue(_value, _data);
    }


    //////////////////////////////////////////
    // Try functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryGetValueSerializationSize(typename ::std::enable_if<(HasGetValueSerializationSize<TValue>::value), TValue>::type const& _value)
    {
        return GetValueSerializationSize(_value);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryGetValueSerializationSize(typename ::std::enable_if<(std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value)
    {
        return _value.getValueSerializationSize();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryGetValueSerializationSize(typename ::std::enable_if<(
        !HasGetValueSerializationSize<TValue>::value &&
        !std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value)
    {
        return 0u;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TrySerializeValue(typename ::std::enable_if<(HasSerializeValue<TValue>::value), TValue>::type const& _value, U8* _data)
    {
        SerializeValue(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TrySerializeValue(typename ::std::enable_if<(std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value, U8* _data)
    {
        _value.serialize(_data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TrySerializeValue(typename ::std::enable_if<(
        !HasSerializeValue<TValue>::value &&
        !std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type const& _value, U8* _data)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryDeserializeValue(typename ::std::enable_if<(HasDeserializeValue<TValue>::value), TValue>::type& _value, U8 const* _data)
    {
        DeserializeValue(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryDeserializeValue(typename ::std::enable_if<(std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type& _value, U8 const* _data)
    {
        _value.deserialize(_data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline U32 TryDeserializeValue(typename ::std::enable_if<(
        !HasDeserializeValue<TValue>::value &&
        !std::is_base_of<IBinarySerializable, TValue>::value), TValue>::type& _value, U8 const* _data)
    {
        return false;
    }


    //////////////////////////////////////////
    // Enum class
    //
    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_CPP_ENUMCLASS_BINARY_SERIALIZATION(DEnumClass) \
        inline U32 GetValueSerializationSize(DEnumClass const& _value) \
        { \
            return sizeof(DEnumClass); \
        } \
        inline void SerializeValue(DEnumClass const& _value, U8* _data) \
        { \
            memcpy(_data, (U8 const*)(&_value), sizeof(DEnumClass)); \
        } \
        inline void DeserializeValue(DEnumClass& _value, U8 const* _data) \
        { \
            memcpy((U8*)&_value, _data, sizeof(DEnumClass)); \
        }


    //////////////////////////////////////////
    // Dummy
    //
    //////////////////////////////////////////
    #define MAZE_NOT_IMPLEMENTED_BINARY_SERIALIZATION(DClass) \
        inline U32 GetValueSerializationSize(DClass const& _value) \
        { \
            MAZE_TODO; \
            return 0; \
        } \
        inline void SerializeValue(DClass const& _value, U8* _data) \
        { \
            MAZE_TODO; \
        } \
        inline void DeserializeValue(DClass& _value, U8 const* _data) \
        { \
            MAZE_TODO; \
        }
    
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeBinarySerialization_hpp_
