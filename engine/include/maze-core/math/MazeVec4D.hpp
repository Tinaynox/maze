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
#if (!defined(_MazeVec4D_hpp_))
#define _MazeVec4D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Vec4D
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Vec4D
    {
    public:

        //////////////////////////////////////////
        static Vec4D const c_zero;
        static Vec4D const c_one;
        static Vec4D const c_negativeOne;
        static Vec4D const c_unitX;
        static Vec4D const c_unitY;
        static Vec4D const c_unitZ;
        static Vec4D const c_negativeUnitX;
        static Vec4D const c_negativeUnitY;
        static Vec4D const c_negativeUnitZ;

    public:

        //////////////////////////////////////////
        inline Vec4D();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(
            TValue const _x,
            TValue const _y,
            TValue const _z,
            TValue const _w);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4D(TValue const _x);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4D(TValue const _arr[4]);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4D(TValue* const _arr);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(Vec4D const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(Vec4D&& _vec);

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4D(Vec4D<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
            , w(static_cast<TValue>(_vec.w))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(
            Vec2D<TValue> const& _vec,
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
        inline MAZE_CONSTEXPR Vec4D(
            Vec2D<UValue> const& _vec,
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
        inline MAZE_CONSTEXPR Vec4D(Vec2D<UValue> const& _vec, DValue _z, FValue _w = 1)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_z))
            , w(static_cast<TValue>(_w))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(
            Vec2D<TValue> const& _vecXY,
            Vec2D<TValue> const& _vecZW)
            : x(_vecXY.x)
            , y(_vecXY.y)
            , z(_vecZW.x)
            , w(_vecZW.y)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4D(
            Vec2D<UValue> const& _vecXY,
            Vec2D<UValue> const& _vecZW)
            : x(static_cast<TValue>(_vecXY.x))
            , y(static_cast<TValue>(_vecXY.y))
            , z(static_cast<TValue>(_vecZW.x))
            , w(static_cast<TValue>(_vecZW.y))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(Vec3D<TValue> const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(1)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4D(Vec3D<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
            , w(1)
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D(Vec3D<TValue> const& _vec, TValue _w)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_w)
        {
        }

        //////////////////////////////////////////
        template <class UValue, class DValue>
        inline MAZE_CONSTEXPR Vec4D(Vec3D<UValue> const& _vec, DValue _w)
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
        inline Vec2D<TValue> xy() const;

        //////////////////////////////////////////
        inline Vec3D<TValue> xyz() const;

        //////////////////////////////////////////
        inline Vec2D<TValue> zw() const;

        //////////////////////////////////////////
        inline void swap(Vec4D& _other);

        //////////////////////////////////////////
        inline TValue operator[](Size const _i) const;

        //////////////////////////////////////////
        inline TValue& operator[](Size const _i);

        //////////////////////////////////////////
        inline TValue* ptr();

        //////////////////////////////////////////
        inline const TValue* ptr() const;

        //////////////////////////////////////////
        inline Vec4D& operator=(Vec4D const& _vec);

        //////////////////////////////////////////
        inline Vec4D& operator=(Vec4D&& _vec) noexcept;

        //////////////////////////////////////////
        inline Vec4D& operator=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4D& operator=(Vec2D<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec4D& operator=(Vec3D<TValue> const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D operator+(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D operator-(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D operator*(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4D operator*(UValue const _value) const
        {
            return Vec4D(
                static_cast<TValue>(x * _value),
                static_cast<TValue>(y * _value),
                static_cast<TValue>(z * _value),
                static_cast<TValue>(w * _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D operator*(Vec4D const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4D operator*=(Vec4D<UValue> const& _vec) const
        {
            return Vec4D<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y),
                static_cast<TValue>(z * _vec.z),
                static_cast<TValue>(w * _vec.w));
        }

        //////////////////////////////////////////
        inline Vec4D operator/(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline Vec4D operator/(UValue const _value) const
        {
            return Vec4D<TValue>(
                static_cast<TValue>(x / _value),
                static_cast<TValue>(y / _value),
                static_cast<TValue>(z / _value),
                static_cast<TValue>(w / _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D operator/(Vec4D const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4D operator/(Vec4D<UValue> const& _vec) const
        {
            return Vec4D(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y),
                static_cast<TValue>(z / _vec.z),
                static_cast<TValue>(w / _vec.w));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D const& operator+() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D operator-() const;


        //////////////////////////////////////////
        inline Vec4D& operator+=(Vec4D const& _vec);

        //////////////////////////////////////////
        inline Vec4D& operator+=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4D& operator-=(Vec4D const& _vec);

        //////////////////////////////////////////
        inline Vec4D& operator-=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4D& operator*=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4D& operator*=(Vec4D const& _vec);

        //////////////////////////////////////////
        inline Vec4D& operator/=(TValue const _value);

        //////////////////////////////////////////
        inline Vec4D& operator/=(Vec4D const& _vec);


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue length() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredLength() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue distance(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredDistance(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue dotProduct(Vec4D const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4D midPoint(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec4D const& _vec) const;

        //////////////////////////////////////////
        inline void makeFloor(Vec4D const& _vec);

        //////////////////////////////////////////
        inline void makeCeil(Vec4D const& _vec);


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isZeroLength() const;

        //////////////////////////////////////////
        inline bool isEqual(Vec4D const& _vec, TValue _eps = static_cast<TValue>(1e-04)) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isNaN() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isInfinity() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isFinite() const { return !isNaN() && !isInfinity(); }


        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec4D& _result, Char _separator = ';');

        //////////////////////////////////////////
        static Vec4D FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Vec4D FromString(String const& _string, Char _separator = ';');


    public:
        TValue x;
        TValue y;
        TValue z;
        TValue w;
    }; 


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> operator*(TValue const _value, Vec4D<TValue> const& _vec)
    {
        return Vec4D<TValue>(
            _value * _vec.x,
            _value * _vec.y,
            _value * _vec.z,
            _value * _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> operator/(TValue const _value, Vec4D<TValue> const& _vec)
    {
        return Vec4D<TValue>(
            _value / _vec.x,
            _value / _vec.y,
            _value / _vec.z,
            _value / _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> operator+(Vec4D<TValue> const& _vec, TValue const _value)
    {
        return Vec4D<TValue>(
            _vec.x + _value,
            _vec.y + _value,
            _vec.z + _value,
            _vec.w + _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> operator+(TValue const _value, Vec4D<TValue> const& _vec)
    {
        return Vec4D<TValue>(
            _value + _vec.x,
            _value + _vec.y,
            _value + _vec.z,
            _value + _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> operator-(Vec4D<TValue> const& _vec, TValue const _value)
    {
        return Vec4D<TValue>(
            _vec.x - _value,
            _vec.y - _value,
            _vec.z - _value,
            _vec.w - _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> operator-(TValue const _value, Vec4D<TValue> const& _vec)
    {
        return Vec4D<TValue>(
            _value - _vec.x,
            _value - _vec.y,
            _value - _vec.z,
            _value - _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec4D<TValue> const& _v)
    {
        _o << "Vec4D(" << _v.x << ", " << _v.y << ", " << _v.z << ", " << _v.w <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsVec4D : std::false_type {};
    template <class TValue>
    struct IsVec4D<Maze::Vec4D<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4D<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4D<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4D<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4D<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec4D<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Vec4DS = Vec4D<S32>;
    using Vec4DU = Vec4D<U32>;
    using Vec4DF = Vec4D<F32>;


    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec4DF Math::Floor<Vec4DF>(Vec4DF const& _value)
    { 
        return Vec4DF(Floor(_value.x), Floor(_value.y), Floor(_value.z), Floor(_value.w)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec4DF Math::Ceil<Vec4DF>(Vec4DF const& _value)
    { 
        return Vec4DF(Ceil(_value.x), Ceil(_value.y), Ceil(_value.z), Ceil(_value.w)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec4DF Math::Round<Vec4DF>(Vec4DF const& _value)
    { 
        return Vec4DF(Round(_value.x), Round(_value.y), Round(_value.z), Round(_value.w)); 
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeVec4D.inl"


#endif // _MazeVec4D_hpp_
//////////////////////////////////////////
