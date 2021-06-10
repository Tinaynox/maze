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
#include "MazeValueSerialization.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Type: bool
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<bool>::value), void>::type
        ValueToString(bool const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<bool>::value), void>::type
        ValueFromString(bool& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToBool(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: S8
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S8>::value), void>::type
        ValueToString(S8 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S8>::value), void>::type
        ValueFromString(S8& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToS8(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: U8
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U8>::value), void>::type
        ValueToString(U8 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U8>::value), void>::type
        ValueFromString(U8& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToU8(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: S16
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S16>::value), void>::type
        ValueToString(S16 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S16>::value), void>::type
        ValueFromString(S16& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToS16(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: U16
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U16>::value), void>::type
        ValueToString(U16 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U16>::value), void>::type
        ValueFromString(U16& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToU16(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: S32
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S32>::value), void>::type
        ValueToString(S32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S32>::value), void>::type
        ValueFromString(S32& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToS32(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: U32
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U32>::value), void>::type
        ValueToString(U32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U32>::value), void>::type
        ValueFromString(U32& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToU32(String(_data, _count));
    }

    
    //////////////////////////////////////////
    // Type: S64
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S64>::value), void>::type
        ValueToString(S64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<S64>::value), void>::type
        ValueFromString(S64& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToS64(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: U64
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U64>::value), void>::type
        ValueToString(U64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<U64>::value), void>::type
        ValueFromString(U64& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToU64(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: F32
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<F32>::value), void>::type
        ValueToString(F32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<F32>::value), void>::type
        ValueFromString(F32& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToF32(String(_data, _count));
    }


    //////////////////////////////////////////
    // Type: F64
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<F64>::value), void>::type
        ValueToString(F64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(!std::is_class<F64>::value), void>::type
        ValueFromString(F64& _value, CString _data, Size _count)
    {
        _value = StringHelper::StringToF64(String(_data, _count));
    }
    
} // namespace Maze
//////////////////////////////////////////
