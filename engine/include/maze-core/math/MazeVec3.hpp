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
#if (!defined(_MazeVec3_hpp_))
#define _MazeVec3_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeRandom.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec3
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Vec3
    {
    public:

        ////////////////////////////////////
        static Vec3 const c_zero;
        static Vec3 const c_one;
        static Vec3 const c_negativeOne;
        static Vec3 const c_unitX;
        static Vec3 const c_unitY;
        static Vec3 const c_unitZ;
        static Vec3 const c_negativeUnitX;
        static Vec3 const c_negativeUnitY;
        static Vec3 const c_negativeUnitZ;

    public:

        //////////////////////////////////////////
        inline Vec3();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3(
            TValue const _x,
            TValue const _y,
            TValue const _z);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3(TValue const _x);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3(TValue const _arr[3]);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3(TValue* const _arr);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3(Vec3 const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3(Vec3&& _vec) noexcept;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3(Vec3<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3(Vec2<TValue> const& _vec, TValue _z = 0)
            : x(_vec.x)
            , y(_vec.y)
            , z(_z)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3(Vec2<UValue> const& _vec, UValue _z = 0)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_z))
        {
        }

        //////////////////////////////////////////
        inline void set(
            TValue const _x,
            TValue const _y,
            TValue const _z);

        //////////////////////////////////////////
        inline void set(TValue const _x);

        //////////////////////////////////////////
        inline void setX(TValue const _x);

        //////////////////////////////////////////
        inline void setY(TValue const _y);

        //////////////////////////////////////////
        inline void setZ(TValue const _z);

        //////////////////////////////////////////
        inline TValue getX() const;

        //////////////////////////////////////////
        inline TValue getY() const;

        //////////////////////////////////////////
        inline TValue getZ() const;

        //////////////////////////////////////////
        inline Vec2<TValue> xy() const;

        //////////////////////////////////////////
        inline Vec2<TValue> xz() const;

        //////////////////////////////////////////
        inline void swap(Vec3& _other);

        //////////////////////////////////////////
        inline TValue operator[](Size const _i) const;

        //////////////////////////////////////////
        inline TValue& operator[](Size const _i);

        //////////////////////////////////////////
        inline TValue* ptr();

        //////////////////////////////////////////
        inline const TValue* ptr() const;

        //////////////////////////////////////////
        inline Vec3& operator=(Vec3 const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator=(Vec3&& _vec) noexcept;

        //////////////////////////////////////////
        inline Vec3& operator=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3& operator=(Vec2<TValue> const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator+(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator-(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator*(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3 operator*(UValue const _value) const
        {
            return Vec3(
                static_cast<TValue>(x * _value),
                static_cast<TValue>(y * _value),
                static_cast<TValue>(z * _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator*(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator*(Vec2<TValue> const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3 operator*=(Vec3<UValue> const& _vec) const
        {
            return Vec3<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y),
                static_cast<TValue>(z * _vec.z));
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3 operator*=(Vec2<UValue> const& _vec) const
        {
            return Vec3<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y),
                static_cast<TValue>(z));
        }

        //////////////////////////////////////////
        inline Vec3 operator/(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline Vec3 operator/(UValue const _value) const
        {
            return Vec3<TValue>(
                static_cast<TValue>(x / _value),
                static_cast<TValue>(y / _value),
                static_cast<TValue>(z / _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator/(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator/(Vec2<TValue> const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3 operator/(Vec3<UValue> const& _vec) const
        {
            return Vec3(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y),
                static_cast<TValue>(z / _vec.z));
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3 operator/(Vec2<UValue> const& _vec) const
        {
            return Vec3(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y),
                static_cast<TValue>(z));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 const& operator+() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 operator-() const;


        //////////////////////////////////////////
        inline Vec3& operator+=(Vec3 const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator+=(Vec2<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator+=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3& operator-=(Vec3 const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator-=(Vec2<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator-=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3& operator*=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3& operator*=(Vec3 const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator*=(Vec2<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator/=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3& operator/=(Vec3 const& _vec);

        //////////////////////////////////////////
        inline Vec3& operator/=(Vec2<TValue> const& _vec);

        //////////////////////////////////////////
        inline operator Vec2<TValue>() const { return Vec2<TValue>(x, y); }


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue length() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredLength() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue distance(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredDistance(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue dotProduct(Vec3 const& _vec) const;
        

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 midPoint(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec3 const& _vec) const;

        //////////////////////////////////////////
        inline void makeFloor(Vec3 const& _vec);

        //////////////////////////////////////////
        inline void makeCeil(Vec3 const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3<TValue> crossProduct(Vec3 const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isZeroLength() const;

        //////////////////////////////////////////
        inline bool isEqual(Vec3 const& _vec, TValue _eps = static_cast<TValue>(1e-04)) const;

        //////////////////////////////////////////
        inline Vec3 normalizedCopy() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3 reflect(Vec3 const& _normal) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isNaN() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isInfinity() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isFinite() const { return !isNaN() && !isInfinity(); }


        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec3& _result, Char _separator = ';');

        //////////////////////////////////////////
        static Vec3 FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Vec3 FromString(String const& _string, Char _separator = ';');

        //////////////////////////////////////////
        static inline Vec3 SLerp(Vec3 const& _start, Vec3 const& _end, F32 _percent)
        {
            TValue cosAlpha = _start.dotProduct(_end);
            TValue alpha = Math::ACos(cosAlpha);
            TValue sinAlpha = Math::Sin(alpha);
            TValue t1 = Math::Sin((static_cast<TValue>(1) - static_cast<TValue>(_percent)) * alpha) / sinAlpha;
            TValue t2 = Math::Sin(_percent * alpha) / sinAlpha;

            return _start * t1 + _end * t2;
        }

        //////////////////////////////////////////
        inline Vec3 slerp(Vec3 const& _end, F32 _percent)
        {
            return SLerp(*this, _end, _percent);
        }


        //////////////////////////////////////////
        Vec3 perpendicular() const;

        //////////////////////////////////////////
        TValue normalize();

        //////////////////////////////////////////
        TValue normalizeTo(TValue _requiredLength);

        //////////////////////////////////////////
        Vec3& rotate(Vec3<TValue> const& _axis, F32 _theta);

        //////////////////////////////////////////
        Vec3 rotatedCopy(Vec3<TValue> const& _axis, F32 _theta);

        //////////////////////////////////////////
        static Vec3<TValue> RandomDirection();

        //////////////////////////////////////////
        static Vec3<TValue> RandomHemisphereDirection(Vec3<TValue> const& _normal);


        //////////////////////////////////////////
        inline F32 getAngleBetween(Vec3 const& _vector) const
        {
            F32 angle = dotProduct(_vector);
            angle /= (length() * _vector.length());
            return angle = Math::ACos(angle);
        }

        //////////////////////////////////////////
        static inline F32 GetAngleBetween(
            Vec3 const& _vectorA,
            Vec3 const& _vectorB)
        {
            return _vectorA.getAngleBetween(_vectorB);
        }

    public:
        TValue x;
        TValue y;
        TValue z;
    }; 


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> operator*(TValue const _value, Vec3<TValue> const& _vec)
    {
        return Vec3<TValue>(
            _value * _vec.x,
            _value * _vec.y,
            _value * _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> operator/(TValue const _value, Vec3<TValue> const& _vec)
    {
        return Vec3<TValue>(
            _value / _vec.x,
            _value / _vec.y,
            _value / _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> operator+(Vec3<TValue> const& _vec, TValue const _value)
    {
        return Vec3<TValue>(
            _vec.x + _value,
            _vec.y + _value,
            _vec.z + _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> operator+(TValue const _value, Vec3<TValue> const& _vec)
    {
        return Vec3<TValue>(
            _value + _vec.x,
            _value + _vec.y,
            _value + _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> operator-(Vec3<TValue> const& _vec, TValue const _value)
    {
        return Vec3<TValue>(
            _vec.x - _value,
            _vec.y - _value,
            _vec.z - _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> operator-(TValue const _value, Vec3<TValue> const& _vec)
    {
        return Vec3<TValue>(
            _value - _vec.x,
            _value - _vec.y,
            _value - _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec3<TValue> const& _v)
    {
        _o << "Vec3(" << _v.x << ", " << _v.y << ", " << _v.z <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsVec3 : std::false_type {};
    template <class TValue>
    struct IsVec3<Maze::Vec3<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Vec3S32 = Vec3<S32>;
    using Vec3U32 = Vec3<U32>;
    using Vec3F32 = Vec3<F32>;


    //////////////////////////////////////////
    // Aliases
    using Vec3S = Vec3S32;
    using Vec3U = Vec3U32;
    using Vec3F = Vec3F32;


    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec3F Math::Floor<Vec3F>(Vec3F const& _value)
    { 
        return Vec3F(Floor(_value.x), Floor(_value.y), Floor(_value.z)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec3F Math::Ceil<Vec3F>(Vec3F const& _value)
    { 
        return Vec3F(Ceil(_value.x), Ceil(_value.y), Ceil(_value.z)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec3F Math::Round<Vec3F>(Vec3F const& _value)
    { 
        return Vec3F(Round(_value.x), Round(_value.y), Round(_value.z)); 
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeVec3.inl"


#endif // _MazeVec3_hpp_
//////////////////////////////////////////
