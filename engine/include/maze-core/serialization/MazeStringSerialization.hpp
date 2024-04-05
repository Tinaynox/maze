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
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/math/MazePlane.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/math/MazeRay.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include <json/json.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Forward declaration
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data);

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count);

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void ValueToString(SharedPtr<UValue> const& _value, String& _data);
    
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromString(SharedPtr<UValue>& _value, CString _data, Size _count);
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromString(SharedPtr<UValue>& _value, CString _data, Size _count);

    //////////////////////////////////////////
    namespace JSONHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern String ToString(Json::Value const& _value);

        //////////////////////////////////////////
        MAZE_CORE_API extern Json::Value FromString(String const& _value);
    }


    //////////////////////////////////////////
    // Type: Bool
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Bool const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Bool& _value, CString _data, Size _count)
    {
        StringHelper::ParseBool(_data, _data + _count, _value);
    }


    //////////////////////////////////////////
    // Type: S8
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(S8 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(S8& _value, CString _data, Size _count)
    {
        StringHelper::ParseS8(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: U8
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(U8 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(U8& _value, CString _data, Size _count)
    {
        StringHelper::ParseU8(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: S16
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(S16 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(S16& _value, CString _data, Size _count)
    {
        StringHelper::ParseS16(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: U16
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(U16 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(U16& _value, CString _data, Size _count)
    {
        StringHelper::ParseU16(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: S32
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(S32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(S32& _value, CString _data, Size _count)
    {
        StringHelper::ParseS32(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: U32
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(U32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(U32& _value, CString _data, Size _count)
    {
        StringHelper::ParseU32(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: S64
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(S64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(S64& _value, CString _data, Size _count)
    {
        StringHelper::ParseS64(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: U64
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(U64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(U64& _value, CString _data, Size _count)
    {
        StringHelper::ParseU64(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: F32
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(F32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(F32& _value, CString _data, Size _count)
    {
        StringHelper::ParseF32(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: F64
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(F64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(F64& _value, CString _data, Size _count)
    {
        StringHelper::ParseF64(_data, _count, _value);
    }


    //////////////////////////////////////////
    // Type: Rotation2D
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Rotation2D const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Rotation2D& _value, CString _data, Size _count)
    {
        _value = Rotation2D::FromString(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: Plane
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Plane const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Plane& _value, CString _data, Size _count)
    {
        _value = Plane::FromString(_data, _count);
    }


    //////////////////////////////////////////
    // Type: Quaternion
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Quaternion const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Quaternion& _value, CString _data, Size _count)
    {
        _value = Quaternion::FromString(_data, _count);
    }


    //////////////////////////////////////////
    // Type: Ray
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Ray const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Ray& _value, CString _data, Size _count)
    {
        _value = Ray::FromString(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: IStringSerializable
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(IStringSerializable const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(IStringSerializable& _value, CString _data, Size _count)
    {
        _value.setString(_data, _count);
    }


    //////////////////////////////////////////
    // Type: String
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value;
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsString<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = String(_data, _count);
    }


    //////////////////////////////////////////
    // Type: HashedCString
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(HashedCString const& _value, String& _data)
    {
        _data = !_value.empty() ? _value.str : String();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(HashedCString& _value, CString _data, Size _count)
    {
        _value = HashedCString(_data);
    }


    //////////////////////////////////////////
    // Type: HashedString
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(HashedString const& _value, String& _data)
    {
        _data = _value.getString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(HashedString& _value, CString _data, Size _count)
    {
        _value = HashedString(_data, _count);
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        ValueToString(_value, _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsSharedPtr<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        ValueFromString(_value, _data, _count);
    }


    //////////////////////////////////////////
    // Type: Container
    //
    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void ContainerMetaPropertyToString(
        TIterator _first,
        TIterator const _last,
        String& _data);

    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void ContainerMetaPropertyFromString(
        TIterator _it,
        String const& _data);


    //////////////////////////////////////////
    // Vector redirect functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        ContainerMetaPropertyToString<
            typename TValue::const_iterator,
            typename TValue::value_type>(
                _value.begin(),
                _value.end(),
                _data);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVector<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        ContainerMetaPropertyFromString<
            std::back_insert_iterator<TValue>,
            typename TValue::value_type>(
                std::back_inserter(_value),
                String(_data, _count));
    }



    //////////////////////////////////////////
    // Utils
    //
    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueFromString : std::false_type { };
    template <typename T>
    struct HasValueFromString <T, decltype(Maze::ValueFromString(std::declval<T&>(), std::declval<CString>(), std::declval<Size>()), 0)> : std::true_type { };

    //////////////////////////////////////////
    template <typename T, typename = int>
    struct HasValueToString : std::false_type { };
    template <typename T>
    struct HasValueToString <T, decltype(Maze::ValueToString(std::declval<T>(), std::declval<String&>()), 0)> : std::true_type { };


    //////////////////////////////////////////
    // Try functions
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE bool TryValueFromString(typename ::std::enable_if<(
        HasValueFromString<TValue>::value), TValue>::type& _value, CString _data, Size _count)
    {
        ValueFromString(_value, _data, _count);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE  bool TryValueFromString(typename ::std::enable_if<(
        !HasValueFromString<TValue>::value), TValue>::type& _value, CString _data, Size _count)
    {
        return false;
    }


    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE bool TryValueToString(typename ::std::enable_if<(
        HasValueToString<TValue>::value), TValue>::type const& _value, String& _data)
    {
        ValueToString(_value, _data);
        return true;
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE bool TryValueToString(typename ::std::enable_if<(
        !HasValueToString<TValue>::value), TValue>::type const& _value, String& _data)
    {
        return false;
    }


    //////////////////////////////////////////
    // Type: SharedPtr
    //
    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE void ValueToString(SharedPtr<UValue> const& _value, String& _data)
    {
        if (_value)
            TryValueToString<UValue>(*_value.get(), _data);
        else
            _data.clear();
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromString(SharedPtr<UValue>& _value, CString _data, Size _count)
    {
        if (!_value)
            _value = std::make_shared<UValue>();

        TryValueFromString<UValue>(*_value.get(), _data, _count);
    }

    //////////////////////////////////////////
    template <typename UValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(!StdHelper::HasDefaultConstructor<UValue>::value), void>::type
        ValueFromString(SharedPtr<UValue>& _value, CString _data, Size _count)
    {
        MAZE_ERROR_RETURN_IF(!_value, "%s class has no default constructor to make shared ptr", static_cast<CString>(ClassInfo<UValue>::Name()));
        TryValueFromString<UValue>(*_value.get(), _data, _count);
    }


    //////////////////////////////////////////
    template <typename TIterator, typename TValue>
    MAZE_FORCEINLINE void ContainerMetaPropertyToString(
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
    MAZE_FORCEINLINE void ContainerMetaPropertyFromString(
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

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeStringSerialization.inl"


//////////////////////////////////////////
#endif // _MazeStringSerialization_hpp_
