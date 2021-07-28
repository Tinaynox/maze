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
#if (!defined(_MazeValueSerialization_hpp_))
#define _MazeValueSerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
// #include <json/reader.h>
// #include <json/writer.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Type: Any base type (non-class)
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(!std::is_class<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count);

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
    // Type: IStringSerializable / IBinarySerializable
    //
    //////////////////////////////////////////
    inline void ValueToString(IStringSerializable const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(IStringSerializable& _value, CString _data, Size _count)
    {
        _value.setString(String(_data, _count));
    }

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
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        ValueToString(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        ValueFromString(*_value.get(), _data, _count);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return GetValueSerializationSize(*_value.get());
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        SerializeValue(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        DeserializeValue(*_value.get(), _data);
    }


    //////////////////////////////////////////
    // Type: std::function
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {}

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {}

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsStdFunction<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return 0;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {}

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {}


    //////////////////////////////////////////
    // Type: String
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsString<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsString<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = String(_data, _count);
    }

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
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline void ContainerMetaPropertyToString(
        TIterator _first,
        TIterator const _last,
        String& _data)
    {
        _data.clear();

        Json::Value json;
        
        for (TIterator it = _first; it != _last; ++it)
        {
            TValue const& childValue = (*it);

            String childValueString;
            ValueToString(childValue, childValueString);
            json.append(childValueString.c_str());
        }

        _data = JSONHelper::ToString(json);
    }

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    inline void ContainerMetaPropertyFromString(
        TIterator _it,
        String const& _data)
    {
        Json::Value value = JSONHelper::FromString(_data);

        if (value.isNull())
            return;

        for (Json::Value const& child : value)
        {
            TValue value;
            Json::String str = child.asString();
            ValueFromString(value, str.c_str(), str.size());

            *_it++ = value;
        }
    }

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
            result += GetValueSerializationSize(childValue);
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

            U32 sizeBytes = GetValueSerializationSize(childValue);
            SerializeValue(sizeBytes, _data);
            _data += sizeof(sizeBytes);

            SerializeValue(childValue, _data);
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
            DeserializeValue(value, _data);
            _data += sizeBytes;

            *_it++ = value;
        }
    }

    //////////////////////////////////////////
    // Type: Vector
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueToString(Vector<UValue> const& _value, String& _data)
    {
        ContainerMetaPropertyToString<
            typename Vector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end(),
                _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        ValueToString(_value, _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueFromString(Vector<UValue>& _value, CString _data, Size _count)
    {
        ContainerMetaPropertyFromString<
            std::back_insert_iterator<Vector<UValue>>,
            UValue>(
                std::back_inserter(_value),
                String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        ValueFromString(_value, _data, _count);
    }

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
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return GetValueSerializationSize(_value);
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
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        SerializeValue(_value, _data);
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
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        DeserializeValue(_value, _data);
    }


    //////////////////////////////////////////
    // Enum class
    //
    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_CPP_ENUMCLASS_SERIALIZATION(DEnumClass) \
        inline void ValueToString(DEnumClass const& _value, String& _data) \
        { \
            _data = StringHelper::ToString((S32)_value); \
        } \
        inline void ValueFromString(DEnumClass& _value, CString _data, Size _count) \
        { \
            _value = (DEnumClass)StringHelper::StringToS32(String(_data, _count)); \
        } \
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
    #define MAZE_NOT_IMPLEMENTED_SERIALIZATION(DClass) \
        inline void ValueToString(DClass const& _value, String& _data) \
        { \
            MAZE_TODO; \
        } \
        inline void ValueFromString(DClass & _value, CString _data, Size _count) \
        { \
            MAZE_TODO; \
        } \
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

    //////////////////////////////////////////
    #define MAZE_STRING_ONLY_SERIALIZATION(DClass) \
        inline void ValueToString(DClass const& _value, String& _data) \
        { \
            _data = _value.toString(); \
        } \
        inline void ValueFromString(DClass& _value, CString _data, Size _count) \
        { \
            _value = DClass::FromString(String(_data, _count)); \
        } \
        inline U32 GetValueSerializationSize(DClass const& _value) \
        { \
            MAZE_TODO; \
            return 0; \
        } \
        inline void SerializeValue(DClass const& _value, U8* _data) \
        { \
            MAZE_TODO; \
        } \
        inline void DeserializeValue(DClass & _value, U8 const* _data) \
        { \
            MAZE_TODO; \
        }
    
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeValueSerialization.inl"


//////////////////////////////////////////
#endif // _MazeValueSerialization_hpp_
