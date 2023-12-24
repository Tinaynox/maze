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
#if (!defined(_MazeVec4_hpp_))
#define _MazeVec4_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Vec4
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Vec4
    {
    public:

        //////////////////////////////////////////
        static Vec4 const c_zero;
        static Vec4 const c_one;
        static Vec4 const c_negativeOne;
        static Vec4 const c_unitX;
        static Vec4 const c_unitY;
        static Vec4 const c_unitZ;
        static Vec4 const c_negativeUnitX;
        static Vec4 const c_negativeUnitY;
        static Vec4 const c_negativeUnitZ;

    public:

        //////////////////////////////////////////
        inline Vec4();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(
            TValue const _x,
            TValue const _y,
            TValue const _z,
            TValue const _w);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4(TValue const _x);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4(TValue const _arr[4]);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4(TValue* const _arr);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(Vec4 const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(Vec4&& _vec);

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4(Vec4<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
            , w(static_cast<TValue>(_vec.w))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(
            Vec2<TValue> const& _vec,
            TValue _z = 0,
            TValue _w = 1)
            : x(_vec.x)
            , y(_vec.y)
            , z(_z)
            , w(_w)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4(
            Vec2<UValue> const& _vec,
            UValue _z = 0,
            UValue _w = 1)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_z))
            , w(static_cast<TValue>(_w))
        {
        }

        //////////////////////////////////////////
        template <class UValue, class DValue, class FValue>
        inline MAZE_CONSTEXPR Vec4(Vec2<UValue> const& _vec, DValue _z, FValue _w = 1)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_z))
            , w(static_cast<TValue>(_w))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(
            Vec2<TValue> const& _vecXY,
            Vec2<TValue> const& _vecZW)
            : x(_vecXY.x)
            , y(_vecXY.y)
            , z(_vecZW.x)
            , w(_vecZW.y)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4(
            Vec2<UValue> const& _vecXY,
            Vec2<UValue> const& _vecZW)
            : x(static_cast<TValue>(_vecXY.x))
            , y(static_cast<TValue>(_vecXY.y))
            , z(static_cast<TValue>(_vecZW.x))
            , w(static_cast<TValue>(_vecZW.y))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(Vec3<TValue> const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(1)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4(Vec3<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
            , w(1)
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4(Vec3<TValue> const& _vec, TValue _w)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_w)
        {
        }

        //////////////////////////////////////////
        template <class UValue, class DValue>
        inline MAZE_CONSTEXPR Vec4(Vec3<UValue> const& _vec, DValue _w)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
            , w(static_cast<TValue>(_w))
        {
        }

        //////////////////////////////////////////
        inline void set(
            TValue const _x,
            TValue const _y,
            TValue const _z,
            TValue const _w);

        //////////////////////////////////////////
        inline void set(TValue const _x);

        //////////////////////////////////////////
        inline void setX(TValue const _x);

        //////////////////////////////////////////
        inline void setY(TValue const _y);

        //////////////////////////////////////////
        inline void setZ(TValue const _z);

        //////////////////////////////////////////
        inline void setW(TValue const _w);

        //////////////////////////////////////////
        inline TValue getX() const;

        //////////////////////////////////////////
        inline TValue getY() const;

        //////////////////////////////////////////
        inline TValue getZ() const;

        //////////////////////////////////////////
        inline TValue getW() const;

        //////////////////////////////////////////
        inline Vec2<TValue> xy() const;

        //////////////////////////////////////////
        inline Vec3<TValue> xyz() const;

        //////////////////////////////////////////
        inline Vec2<TValue> zw() const;

        //////////////////////////////////////////
        inline void swap(Vec4& _other);

        //////////////////////////////////////////
        inline TValue operator[](Size const _i) const;

        //////////////////////////////////////////
        inline TValue& operator[](Size const _i);

        //////////////////////////////////////////
        inline TValue* ptr();

        //////////////////////////////////////////
        inline const TValue* ptr() const;

        //////////////////////////////////////////
        inline Vec4& operator=(Vec4 const& _vec);

        //////////////////////////////////////////
        inline Vec4& operator=(Vec4&& _vec) noexcept;

        //////////////////////////////////////////
        inline Vec4& operator=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4& operator=(Vec2<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec4& operator=(Vec3<TValue> const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 operator+(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 operator-(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 operator*(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4 operator*(UValue const _value) const
        {
            return Vec4(
                static_cast<TValue>(x * _value),
                static_cast<TValue>(y * _value),
                static_cast<TValue>(z * _value),
                static_cast<TValue>(w * _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 operator*(Vec4 const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4 operator*=(Vec4<UValue> const& _vec) const
        {
            return Vec4<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y),
                static_cast<TValue>(z * _vec.z),
                static_cast<TValue>(w * _vec.w));
        }

        //////////////////////////////////////////
        inline Vec4 operator/(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline Vec4 operator/(UValue const _value) const
        {
            return Vec4<TValue>(
                static_cast<TValue>(x / _value),
                static_cast<TValue>(y / _value),
                static_cast<TValue>(z / _value),
                static_cast<TValue>(w / _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 operator/(Vec4 const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4 operator/(Vec4<UValue> const& _vec) const
        {
            return Vec4(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y),
                static_cast<TValue>(z / _vec.z),
                static_cast<TValue>(w / _vec.w));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 const& operator+() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 operator-() const;


        //////////////////////////////////////////
        inline Vec4& operator+=(Vec4 const& _vec);

        //////////////////////////////////////////
        inline Vec4& operator+=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4& operator-=(Vec4 const& _vec);

        //////////////////////////////////////////
        inline Vec4& operator-=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4& operator*=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4& operator*=(Vec4 const& _vec);

        //////////////////////////////////////////
        inline Vec4& operator/=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4& operator/=(Vec4 const& _vec);


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue length() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredLength() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue distance(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredDistance(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue dotProduct(Vec4 const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4 midPoint(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec4 const& _vec) const;

        //////////////////////////////////////////
        inline void makeFloor(Vec4 const& _vec);

        //////////////////////////////////////////
        inline void makeCeil(Vec4 const& _vec);


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isZeroLength() const;

        //////////////////////////////////////////
        inline bool isEqual(Vec4 const& _vec, TValue _eps = static_cast<TValue>(1e-04)) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isNaN() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isInfinity() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isFinite() const { return !isNaN() && !isInfinity(); }


        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec4& _result, Char _separator = ';');

        //////////////////////////////////////////
        static Vec4 FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Vec4 FromString(String const& _string, Char _separator = ';');


    public:
        TValue x;
        TValue y;
        TValue z;
        TValue w;
    }; 


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> operator*(TValue const _value, Vec4<TValue> const& _vec)
    {
        return Vec4<TValue>(
            _value * _vec.x,
            _value * _vec.y,
            _value * _vec.z,
            _value * _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> operator/(TValue const _value, Vec4<TValue> const& _vec)
    {
        return Vec4<TValue>(
            _value / _vec.x,
            _value / _vec.y,
            _value / _vec.z,
            _value / _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> operator+(Vec4<TValue> const& _vec, TValue const _value)
    {
        return Vec4<TValue>(
            _vec.x + _value,
            _vec.y + _value,
            _vec.z + _value,
            _vec.w + _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> operator+(TValue const _value, Vec4<TValue> const& _vec)
    {
        return Vec4<TValue>(
            _value + _vec.x,
            _value + _vec.y,
            _value + _vec.z,
            _value + _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> operator-(Vec4<TValue> const& _vec, TValue const _value)
    {
        return Vec4<TValue>(
            _vec.x - _value,
            _vec.y - _value,
            _vec.z - _value,
            _vec.w - _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> operator-(TValue const _value, Vec4<TValue> const& _vec)
    {
        return Vec4<TValue>(
            _value - _vec.x,
            _value - _vec.y,
            _value - _vec.z,
            _value - _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec4<TValue> const& _v)
    {
        _o << "Vec4(" << _v.x << ", " << _v.y << ", " << _v.z << ", " << _v.w <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsVec4 : std::false_type {};
    template <class TValue>
    struct IsVec4<Maze::Vec4<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(_data, _count);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Vec4S32 = Vec4<S32>;
    using Vec4U32 = Vec4<U32>;
    using Vec4F32 = Vec4<F32>;


    //////////////////////////////////////////
    // Aliases
    using Vec4S = Vec4S32;
    using Vec4U = Vec4U32;
    using Vec4F = Vec4F32;


    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec4F Math::Floor<Vec4F>(Vec4F const& _value)
    { 
        return Vec4F(Floor(_value.x), Floor(_value.y), Floor(_value.z), Floor(_value.w)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec4F Math::Ceil<Vec4F>(Vec4F const& _value)
    { 
        return Vec4F(Ceil(_value.x), Ceil(_value.y), Ceil(_value.z), Ceil(_value.w)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec4F Math::Round<Vec4F>(Vec4F const& _value)
    { 
        return Vec4F(Round(_value.x), Round(_value.y), Round(_value.z), Round(_value.w)); 
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeVec4.inl"


#endif // _MazeVec4_hpp_
//////////////////////////////////////////
