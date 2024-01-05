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
namespace Maze
{
    /*
    //////////////////////////////////////////
    // Type: bool
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<bool>::value), bool>::type
        ValueToString(bool const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<bool>::value), bool>::type
        ValueFromString(bool& _value, CString _data, Size _count)
    {
        StringHelper::ParseBool(_data, _data + _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: S8
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S8>::value), bool>::type
        ValueToString(S8 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S8>::value), bool>::type
        ValueFromString(S8& _value, CString _data, Size _count)
    {
        StringHelper::ParseS8(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: U8
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U8>::value), bool>::type
        ValueToString(U8 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U8>::value), bool>::type
        ValueFromString(U8& _value, CString _data, Size _count)
    {
        StringHelper::ParseU8(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: S16
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S16>::value), bool>::type
        ValueToString(S16 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S16>::value), bool>::type
        ValueFromString(S16& _value, CString _data, Size _count)
    {
        StringHelper::ParseS16(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: U16
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U16>::value), bool>::type
        ValueToString(U16 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U16>::value), bool>::type
        ValueFromString(U16& _value, CString _data, Size _count)
    {
        StringHelper::ParseU16(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: S32
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S32>::value), bool>::type
        ValueToString(S32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S32>::value), bool>::type
        ValueFromString(S32& _value, CString _data, Size _count)
    {
        StringHelper::ParseS32(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: U32
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U32>::value), bool>::type
        ValueToString(U32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U32>::value), bool>::type
        ValueFromString(U32& _value, CString _data, Size _count)
    {
        StringHelper::ParseU32(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: S64
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S64>::value), bool>::type
        ValueToString(S64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<S64>::value), bool>::type
        ValueFromString(S64& _value, CString _data, Size _count)
    {
        StringHelper::ParseS64(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: U64
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U64>::value), bool>::type
        ValueToString(U64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<U64>::value), bool>::type
        ValueFromString(U64& _value, CString _data, Size _count)
    {
        StringHelper::ParseU64(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: F32
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<F32>::value), bool>::type
        ValueToString(F32 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<F32>::value), bool>::type
        ValueFromString(F32& _value, CString _data, Size _count)
    {
        StringHelper::ParseF32(_data, _count, _value);
        return true;
    }


    //////////////////////////////////////////
    // Type: F64
    //
    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<F64>::value), bool>::type
        ValueToString(F64 const& _value, String& _data)
    {
        _data = StringHelper::ToString(_value);
        return true;
    }

    //////////////////////////////////////////
    template <>
    MAZE_FORCEINLINE typename ::std::enable_if<(!std::is_class<F64>::value), bool>::type
        ValueFromString(F64& _value, CString _data, Size _count)
    {
        StringHelper::ParseF64(_data, _count, _value);
        return true;
    }
    */

    ///////////////////////////////////////////////////////////////////////////////


    /*
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
        StringHelper::ParseBool(_data, _data + _count, _value);
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
        StringHelper::ParseS8(_data, _count, _value);
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
        StringHelper::ParseU8(_data, _count, _value);
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
        StringHelper::ParseS16(_data, _count, _value);
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
        StringHelper::ParseU16(_data, _count, _value);
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
        StringHelper::ParseS32(_data, _count, _value);
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
        StringHelper::ParseU32(_data, _count, _value);
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
        StringHelper::ParseS64(_data, _count, _value);
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
        StringHelper::ParseU64(_data, _count, _value);
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
        StringHelper::ParseF32(_data, _count, _value);
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
        StringHelper::ParseF64(_data, _count, _value);
    }
    */
    
} // namespace Maze
//////////////////////////////////////////
