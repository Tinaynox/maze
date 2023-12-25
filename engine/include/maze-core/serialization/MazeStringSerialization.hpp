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
#if (!defined(_MazeStringSerialization_hpp_))
#define _MazeStringSerialization_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
// #include "maze-core/helpers/MazeJSONHelper.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include <json/json.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace JSONHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern String ToString(Json::Value const& _value);

        //////////////////////////////////////////
        MAZE_CORE_API extern Json::Value FromString(String const& _value);
    }


    //////////////////////////////////////////
    // Utils
    //
    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueFromString : std::false_type { };
    template <typename T>
    struct HasValueFromString <T, decltype(ValueFromString(std::declval<T>(), std::declval<CString>(), std::declval<Size>()), 0)> : std::true_type { };

    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueToString : std::false_type { };
    template <typename T>
    struct HasValueToString <T, decltype(ValueToString(std::declval<T>(), std::declval<String>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    // Try functions (Forward declaration)
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromString(typename ::std::enable_if<(
        std::is_base_of<IStringSerializable, TValue>::value), TValue>::type& _value, CString _data, Size _count);

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueFromString(typename ::std::enable_if<(
        HasValueFromString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type& _value, CString _data, Size _count);

    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromString(typename ::std::enable_if<(
        !HasValueFromString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type& _value, CString _data, Size _count);

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToString(typename ::std::enable_if<(
        std::is_base_of<IStringSerializable, TValue>::value), TValue>::type const& _value, String& _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToString(typename ::std::enable_if<(
        HasValueToString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type const& _value, String& _data);

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToString(typename ::std::enable_if<(
        !HasValueToString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type const& _value, String& _data);


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
    // Type: IStringSerializable
    //
    //////////////////////////////////////////
    inline void ValueToString(IStringSerializable const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(IStringSerializable& _value, CString _data, Size _count)
    {
        _value.setString(_data, _count);
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueToString(SharedPtr<UValue> const& _value, String& _data)
    {
        TryValueToString<UValue>(*_value.get(), _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        ValueToString(_value, _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromString(SharedPtr<UValue>& _value, CString _data, Size _count)
    {
        if (!_value)
            _value = std::make_shared<UValue>();

        TryValueFromString<UValue>(*_value.get(), _data, _count);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromString(SharedPtr<UValue>& _value, CString _data, Size _count)
    {
        MAZE_ERROR_RETURN_IF(!_value, "%s class has no default constructor to make shared ptr", static_cast<CString>(ClassInfo<UValue>::Name()));
        TryValueFromString<UValue>(*_value.get(), _data, _count);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        ValueFromString(_value, _data, _count);
    }


    //////////////////////////////////////////
    // Type: std::function
    //
    //////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
    //    ValueToString(TValue const& _value, String& _data)
    //{}

    ////////////////////////////////////////////
    //template <typename TValue>
    //inline typename ::std::enable_if<(IsStdFunction<TValue>::value), void>::type
    //    ValueFromString(TValue& _value, CString _data, Size _count)
    //{}



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
    // Type: HashedString
    //
    //////////////////////////////////////////
    inline void ValueToString(HashedString const& _value, String& _data)
    {
        _data = _value.getString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(HashedString& _value, CString _data, Size _count)
    {
        _value = HashedString(_data, _count);
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
            TryValueToString<TValue>(childValue, childValueString);
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
            TryValueFromString<TValue>(value, str.c_str(), str.size());

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
    // Type: FastVector
    //
    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueToString(FastVector<UValue> const& _value, String& _data)
    {
        ContainerMetaPropertyToString<
            typename FastVector<UValue>::const_iterator,
            UValue>(
                _value.begin(),
                _value.end(),
                _data);
    }

    //////////////////////////////////////////
    template <typename UValue>
    inline void ValueFromString(FastVector<UValue>& _value, CString _data, Size _count)
    {
        ContainerMetaPropertyFromString<
            std::back_insert_iterator<FastVector<UValue>>,
            UValue>(
                std::back_inserter(_value),
                String(_data, _count));
    }


    //////////////////////////////////////////
    // Vector redirect functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        ValueToString(_value, _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        ValueFromString(_value, _data, _count);
    }
   

   


    //////////////////////////////////////////
    // Try functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromString(typename ::std::enable_if<(
        std::is_base_of<IStringSerializable, TValue>::value), TValue>::type& _value, CString _data, Size _count)
    {
        _value.setString(_data, _count);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueFromString(typename ::std::enable_if<(
        HasValueFromString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type& _value, CString _data, Size _count)
    {
        ValueFromString(_value, _data, _count);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline  bool TryValueFromString(typename ::std::enable_if<(
        !HasValueFromString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type& _value, CString _data, Size _count)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToString(typename ::std::enable_if<(
        std::is_base_of<IStringSerializable, TValue>::value), TValue>::type const& _value, String& _data)
    {
        _data = _value.toString();
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToString(typename ::std::enable_if<(
        HasValueToString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type const& _value, String& _data)
    {
        ValueToString(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline bool TryValueToString(typename ::std::enable_if<(
        !HasValueToString<TValue>::value &&
        !std::is_base_of<IStringSerializable, TValue>::value), TValue>::type const& _value, String& _data)
    {
        return false;
    }


    //////////////////////////////////////////
    // Enum class
    //
    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_CPP_ENUMCLASS_STRING_SERIALIZATION(DEnumClass) \
        inline void ValueToString(DEnumClass const& _value, String& _data) \
        { \
            _data = StringHelper::ToString((S32)_value); \
        } \
        inline void ValueFromString(DEnumClass& _value, CString _data, Size _count) \
        { \
            S32 value = 0; \
            StringHelper::ParseS32(_data, _count, value); \
            _value = (DEnumClass)value; \
        }


    //////////////////////////////////////////
    // Dummy
    //
    //////////////////////////////////////////
    #define MAZE_NOT_IMPLEMENTED_STRING_SERIALIZATION(DClass) \
        inline void ValueToString(DClass const& _value, String& _data) \
        { \
            MAZE_TODO; \
        } \
        inline void ValueFromString(DClass & _value, CString _data, Size _count) \
        { \
            MAZE_TODO; \
        }

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_STRING_SERIALIZATION(DClass) \
        inline void ValueToString(DClass const& _value, String& _data) \
        { \
            _data = _value.toString(); \
        } \
        inline void ValueFromString(DClass& _value, CString _data, Size _count) \
        { \
            _value = DClass::FromString(String(_data, _count)); \
        }
    
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeStringSerialization.inl"


//////////////////////////////////////////
#endif // _MazeStringSerialization_hpp_
