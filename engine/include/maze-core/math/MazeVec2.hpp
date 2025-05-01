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
#if (!defined(_MazeVec2_hpp_))
#define _MazeVec2_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeRandom.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec2
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Vec2
    {
    public:

        //////////////////////////////////////////
        static Vec2 const c_zero;
        static Vec2 const c_one;
        static Vec2 const c_negativeOne;
        static Vec2 const c_unitX;
        static Vec2 const c_unitY;
        static Vec2 const c_negativeUnitX;
        static Vec2 const c_negativeUnitY;

    public:

        //////////////////////////////////////////
        inline Vec2();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2(TValue const _x, TValue const _y);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2(TValue const _x);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2(TValue const _arr[2]);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2(TValue* const _arr);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2(Vec2 const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2(Vec2&& _vec) noexcept;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2(Vec2<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
        {
        }

        //////////////////////////////////////////
        inline void set(TValue const _x, TValue const _y);

        //////////////////////////////////////////
        inline void set(TValue const _x);

        //////////////////////////////////////////
        inline void setX(TValue const _x);

        //////////////////////////////////////////
        inline void setY(TValue const _y);

        //////////////////////////////////////////
        inline TValue getX() const;

        //////////////////////////////////////////
        inline TValue getY() const;

        //////////////////////////////////////////
        inline void swap(Vec2& _other);

        //////////////////////////////////////////
        MAZE_FORCEINLINE TValue operator[](Size const _i) const;

        //////////////////////////////////////////
        MAZE_FORCEINLINE TValue& operator[](Size const _i);

        //////////////////////////////////////////
        inline TValue* ptr();

        //////////////////////////////////////////
        inline const TValue* ptr() const;

        //////////////////////////////////////////
        inline Vec2& operator=(Vec2 const& _vec);

        //////////////////////////////////////////
        inline Vec2& operator=(Vec2&& _vec) noexcept;

        //////////////////////////////////////////
        inline Vec2& operator=(TValue const _value);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 operator+(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 operator-(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 operator*(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2 operator*(UValue _value) const
        {
            return Vec2(
                x * static_cast<TValue>(_value),
                y * static_cast<TValue>(_value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 operator*(Vec2 const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2 operator*=(Vec2<UValue> const& _vec) const
        {
            return Vec2<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y));
        }

        //////////////////////////////////////////
        inline Vec2 operator/(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline Vec2 operator/(UValue const _value) const
        {
            return Vec2<TValue>(
                static_cast<TValue>(x / _value),
                static_cast<TValue>(y / _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 operator/(Vec2 const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2 operator/(Vec2<UValue> const& _vec) const
        {
            return Vec2(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 const& operator+() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 operator-() const;


        //////////////////////////////////////////
        inline Vec2& operator+=(Vec2 const& _vec);

        //////////////////////////////////////////
        inline Vec2& operator+=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2& operator-=(Vec2 const& _vec);

        //////////////////////////////////////////
        inline Vec2& operator-=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2& operator*=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2& operator*=(Vec2 const& _vec);

        //////////////////////////////////////////
        inline Vec2& operator/=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2& operator/=(Vec2 const& _vec);


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue length() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredLength() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue distance(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredDistance(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue dotProduct(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 midPoint(Vec2 const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec2 const& _vec) const;

        //////////////////////////////////////////
        inline void makeFloor(Vec2 const& _vec);

        //////////////////////////////////////////
        inline void makeCeil(Vec2 const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 perpendicular() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue crossProduct(Vec2 const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isZeroLength() const;

        //////////////////////////////////////////
        inline bool isEqual(Vec2 const& _vec, TValue _eps = static_cast<TValue>(1e-04)) const;

        //////////////////////////////////////////
        inline Vec2 normalizedCopy() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2 reflect(Vec2 const& _normal) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isNaN() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isInfinity() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isFinite() const { return !isNaN() && !isInfinity(); }


        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Vec2 FromAngle(F32 _angle);

        //////////////////////////////////////////
        inline F32 toAngle() const;

        //////////////////////////////////////////
        inline String toString(Char _separator = ',') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec2& _result, Char _separator = ',');

        //////////////////////////////////////////
        static Vec2 FromString(CString _string, Size _size, Char _separator = ',');

        //////////////////////////////////////////
        static inline Vec2 FromString(String const& _string, Char _separator = ',');


        //////////////////////////////////////////
        TValue normalize();

        //////////////////////////////////////////
        TValue normalizeTo(TValue _requiredLength);

        //////////////////////////////////////////
        Vec2 randomDeviant(F32 _angle) const;

        //////////////////////////////////////////
        Vec2 rotatedCopy(F32 _angle) const;

        //////////////////////////////////////////
        MAZE_CONSTEXPR Vec2 rotatedCopy(F32 _s, F32 _c) const;

        //////////////////////////////////////////
        void rotate(F32 _angle);

        //////////////////////////////////////////
        void rotate(F32 _s, F32 _c);


        //////////////////////////////////////////
        static MAZE_CONSTEXPR Vec2 RandomDirection();

        //////////////////////////////////////////
        static Vec2 RandomSemicircleDirection(Vec2 const& _normal);

        //////////////////////////////////////////
        MAZE_CONSTEXPR F32 angleBetween(Vec2 const& _vec) const;


    public:
        TValue x;
        TValue y;
    }; 


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> operator*(TValue const _value, Vec2<TValue> const& _vec)
    {
        return Vec2<TValue>(
            _value * _vec.x,
            _value * _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> operator/(TValue const _value, Vec2<TValue> const& _vec)
    {
        return Vec2<TValue>(
            _value / _vec.x,
            _value / _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> operator+(Vec2<TValue> const& _vec, TValue const _value)
    {
        return Vec2<TValue>(
            _vec.x + _value,
            _vec.y + _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> operator+(TValue const _value, Vec2<TValue> const& _vec)
    {
        return Vec2<TValue>(
            _value + _vec.x,
            _value + _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> operator-(Vec2<TValue> const& _vec, TValue const _value)
    {
        return Vec2<TValue>(
            _vec.x - _value,
            _vec.y - _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> operator-(TValue const _value, Vec2<TValue> const& _vec)
    {
        return Vec2<TValue>(
            _value - _vec.x,
            _value - _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec2<TValue> const& _v)
    {
        _o << "Vec2(" << _v.x << ", " << _v.y <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsVec2 : std::false_type {};
    template <class TValue>
    struct IsVec2<Maze::Vec2<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVec2<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVec2<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(_data, _count);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVec2<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        _outSize = sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVec2<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsVec2<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Vec2S32 = Vec2<S32>;
    using Vec2U32 = Vec2<U32>;
    using Vec2F32 = Vec2<F32>;


    //////////////////////////////////////////
    // Aliases
    using Vec2S = Vec2S32;
    using Vec2U = Vec2U32;
    using Vec2F = Vec2F32;


    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec2F Math::Floor<Vec2F>(Vec2F const& _value)
    { 
        return Vec2F(Floor(_value.x), Floor(_value.y)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec2F Math::Ceil<Vec2F>(Vec2F const& _value)
    { 
        return Vec2F(Ceil(_value.x), Ceil(_value.y)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec2F Math::Round<Vec2F>(Vec2F const& _value)
    { 
        return Vec2F(Round(_value.x), Round(_value.y)); 
    }

    

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeVec2.inl"


#endif // _MazeVec2_hpp_
//////////////////////////////////////////
