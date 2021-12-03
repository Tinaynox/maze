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
#if (!defined(_MazeVec2D_hpp_))
#define _MazeVec2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec2D
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Vec2D
    {
    public:

        ////////////////////////////////////
        static Vec2D const c_zero;
        static Vec2D const c_one;
        static Vec2D const c_negativeOne;
        static Vec2D const c_unitX;
        static Vec2D const c_unitY;
        static Vec2D const c_negativeUnitX;
        static Vec2D const c_negativeUnitY;

    public:

        //////////////////////////////////////////
        inline Vec2D();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D(TValue const _x, TValue const _y);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2D(TValue const _x);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2D(TValue const _arr[2]);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2D(TValue* const _arr);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D(Vec2D const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D(Vec2D&& _vec) noexcept;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2D(Vec2D<UValue> const& _vec)
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
        inline void swap(Vec2D& _other);

        //////////////////////////////////////////
        inline TValue operator[](Size const _i) const;

        //////////////////////////////////////////
        inline TValue& operator[](Size const _i);

        //////////////////////////////////////////
        inline TValue* ptr();

        //////////////////////////////////////////
        inline const TValue* ptr() const;

        //////////////////////////////////////////
        inline Vec2D& operator=(Vec2D const& _vec);

        //////////////////////////////////////////
        inline Vec2D& operator=(Vec2D&& _vec) noexcept;

        //////////////////////////////////////////
        inline Vec2D& operator=(TValue const _value);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D operator+(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D operator-(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D operator*(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2D operator*(UValue _value) const
        {
            return Vec2D(
                x * static_cast<TValue>(_value),
                y * static_cast<TValue>(_value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D operator*(Vec2D const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2D operator*=(Vec2D<UValue> const& _vec) const
        {
            return Vec2D<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y));
        }

        //////////////////////////////////////////
        inline Vec2D operator/(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline Vec2D operator/(UValue const _value) const
        {
            return Vec2D<TValue>(
                static_cast<TValue>(x / _value),
                static_cast<TValue>(y / _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D operator/(Vec2D const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2D operator/(Vec2D<UValue> const& _vec) const
        {
            return Vec2D(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D const& operator+() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D operator-() const;


        //////////////////////////////////////////
        inline Vec2D& operator+=(Vec2D const& _vec);

        //////////////////////////////////////////
        inline Vec2D& operator+=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2D& operator-=(Vec2D const& _vec);

        //////////////////////////////////////////
        inline Vec2D& operator-=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2D& operator*=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2D& operator*=(Vec2D const& _vec);

        //////////////////////////////////////////
        inline Vec2D& operator/=(TValue const _value);

        //////////////////////////////////////////
        inline Vec2D& operator/=(Vec2D const& _vec);


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue length() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredLength() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue distance(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredDistance(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue dotProduct(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D midPoint(Vec2D const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec2D const& _vec) const;

        //////////////////////////////////////////
        inline void makeFloor(Vec2D const& _vec);

        //////////////////////////////////////////
        inline void makeCeil(Vec2D const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D perpendicular() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue crossProduct(Vec2D const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isZeroLength() const;

        //////////////////////////////////////////
        inline bool isEqual(Vec2D const& _vec, TValue _eps = static_cast<TValue>(1e-04)) const;

        //////////////////////////////////////////
        inline Vec2D normalizedCopy() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D reflect(Vec2D const& _normal) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isNaN() const;

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Vec2D FromAngle(F32 _angle);

        //////////////////////////////////////////
        inline F32 toAngle() const;

        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(x) + ";" + StringHelper::ToString(y); }

        //////////////////////////////////////////
        static Vec2D FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 2)
            {
                return Vec2D(
                    static_cast<TValue>(StringHelper::StringToF32(words[0])),
                    static_cast<TValue>(StringHelper::StringToF32(words[1])));
            }

            return Vec2D::c_zero;
        }


        //////////////////////////////////////////
        TValue normalize();

        //////////////////////////////////////////
        TValue normalizeTo(TValue _requiredLength);

        //////////////////////////////////////////
        Vec2D randomDeviant(F32 _angle) const;

        //////////////////////////////////////////
        Vec2D rotatedCopy(F32 _angle) const;

        //////////////////////////////////////////
        MAZE_CONSTEXPR Vec2D rotatedCopy(F32 _s, F32 _c) const;

        //////////////////////////////////////////
        void rotate(F32 _angle);

        //////////////////////////////////////////
        void rotate(F32 _s, F32 _c);


        //////////////////////////////////////////
        static MAZE_CONSTEXPR Vec2D RandomDirection();

        //////////////////////////////////////////
        static Vec2D RandomSemicircleDirection(Vec2D const& _normal);

        //////////////////////////////////////////
        MAZE_CONSTEXPR F32 angleBetween(Vec2D const& _vec) const;


    public:
        TValue x;
        TValue y;
    }; 


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> operator*(TValue const _value, Vec2D<TValue> const& _vec)
    {
        return Vec2D<TValue>(
            _value * _vec.x,
            _value * _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> operator/(TValue const _value, Vec2D<TValue> const& _vec)
    {
        return Vec2D<TValue>(
            _value / _vec.x,
            _value / _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> operator+(Vec2D<TValue> const& _vec, TValue const _value)
    {
        return Vec2D<TValue>(
            _vec.x + _value,
            _vec.y + _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> operator+(TValue const _value, Vec2D<TValue> const& _vec)
    {
        return Vec2D<TValue>(
            _value + _vec.x,
            _value + _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> operator-(Vec2D<TValue> const& _vec, TValue const _value)
    {
        return Vec2D<TValue>(
            _vec.x - _value,
            _vec.y - _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> operator-(TValue const _value, Vec2D<TValue> const& _vec)
    {
        return Vec2D<TValue>(
            _value - _vec.x,
            _value - _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec2D<TValue> const& _v)
    {
        _o << "Vec2D(" << _v.x << ", " << _v.y <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsVec2D : std::false_type {};
    template <class TValue>
    struct IsVec2D<Maze::Vec2D<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec2D<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec2D<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec2D<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec2D<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec2D<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Vec2DS = Vec2D<S32>;
    using Vec2DU = Vec2D<U32>;
    using Vec2DF = Vec2D<F32>;


    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec2DF Math::Floor<Vec2DF>(Vec2DF const& _value)
    { 
        return Vec2DF(Floor(_value.x), Floor(_value.y)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec2DF Math::Ceil<Vec2DF>(Vec2DF const& _value)
    { 
        return Vec2DF(Ceil(_value.x), Ceil(_value.y)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec2DF Math::Round<Vec2DF>(Vec2DF const& _value)
    { 
        return Vec2DF(Round(_value.x), Round(_value.y)); 
    }
    

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeVec2D.inl"


#endif // _MazeVec2D_hpp_
//////////////////////////////////////////
