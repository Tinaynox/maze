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
#if (!defined(_MazeVec3D_hpp_))
#define _MazeVec3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec3D
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Vec3D
    {
    public:

        ////////////////////////////////////
        static Vec3D const c_zero;
        static Vec3D const c_one;
        static Vec3D const c_negativeOne;
        static Vec3D const c_unitX;
        static Vec3D const c_unitY;
        static Vec3D const c_unitZ;
        static Vec3D const c_negativeUnitX;
        static Vec3D const c_negativeUnitY;
        static Vec3D const c_negativeUnitZ;

    public:

        //////////////////////////////////////////
        inline Vec3D();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D(
            TValue const _x,
            TValue const _y,
            TValue const _z);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3D(TValue const _x);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3D(TValue const _arr[3]);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3D(TValue* const _arr);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D(Vec3D const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D(Vec3D&& _vec) noexcept;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D(Vec3D<UValue> const& _vec)
            : x(static_cast<TValue>(_vec.x))
            , y(static_cast<TValue>(_vec.y))
            , z(static_cast<TValue>(_vec.z))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D(Vec2D<TValue> const& _vec, TValue _z = 0)
            : x(_vec.x)
            , y(_vec.y)
            , z(_z)
        {
        }

        ////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D(Vec2D<UValue> const& _vec, UValue _z = 0)
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
        inline Vec2D<TValue> xy() const;

        //////////////////////////////////////////
        inline void swap(Vec3D& _other);

        //////////////////////////////////////////
        inline TValue operator[](Size const _i) const;

        //////////////////////////////////////////
        inline TValue& operator[](Size const _i);

        //////////////////////////////////////////
        inline TValue* ptr();

        //////////////////////////////////////////
        inline const TValue* ptr() const;

        //////////////////////////////////////////
        inline Vec3D& operator=(Vec3D const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator=(Vec3D&& _vec) noexcept;

        //////////////////////////////////////////
        inline Vec3D& operator=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3D& operator=(Vec2D<TValue> const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator+(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator-(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator*(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D operator*(UValue const _value) const
        {
            return Vec3D(
                static_cast<TValue>(x * _value),
                static_cast<TValue>(y * _value),
                static_cast<TValue>(z * _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator*(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator*(Vec2D<TValue> const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D operator*=(Vec3D<UValue> const& _vec) const
        {
            return Vec3D<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y),
                static_cast<TValue>(z * _vec.z));
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D operator*=(Vec2D<UValue> const& _vec) const
        {
            return Vec3D<TValue>(
                static_cast<TValue>(x * _vec.x),
                static_cast<TValue>(y * _vec.y),
                static_cast<TValue>(z));
        }

        //////////////////////////////////////////
        inline Vec3D operator/(TValue const _value) const;

        //////////////////////////////////////////
        template <class UValue>
        inline Vec3D operator/(UValue const _value) const
        {
            return Vec3D<TValue>(
                static_cast<TValue>(x / _value),
                static_cast<TValue>(y / _value),
                static_cast<TValue>(z / _value));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator/(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator/(Vec2D<TValue> const& _vec) const;

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D operator/(Vec3D<UValue> const& _vec) const
        {
            return Vec3D(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y),
                static_cast<TValue>(z / _vec.z));
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3D operator/(Vec2D<UValue> const& _vec) const
        {
            return Vec3D(
                static_cast<TValue>(x / _vec.x),
                static_cast<TValue>(y / _vec.y),
                static_cast<TValue>(z));
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D const& operator+() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D operator-() const;


        //////////////////////////////////////////
        inline Vec3D& operator+=(Vec3D const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator+=(Vec2D<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator+=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3D& operator-=(Vec3D const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator-=(Vec2D<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator-=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3D& operator*=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3D& operator*=(Vec3D const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator*=(Vec2D<TValue> const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator/=(TValue const _value);

        //////////////////////////////////////////
        inline Vec3D& operator/=(Vec3D const& _vec);

        //////////////////////////////////////////
        inline Vec3D& operator/=(Vec2D<TValue> const& _vec);

        //////////////////////////////////////////
        inline operator Vec2D<TValue>() const { return Vec2D<TValue>(x, y); }


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue length() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredLength() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue distance(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue squaredDistance(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue dotProduct(Vec3D const& _vec) const;
        

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D midPoint(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec3D const& _vec) const;

        //////////////////////////////////////////
        inline void makeFloor(Vec3D const& _vec);

        //////////////////////////////////////////
        inline void makeCeil(Vec3D const& _vec);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D<TValue> crossProduct(Vec3D const& _vec) const;


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isZeroLength() const;

        //////////////////////////////////////////
        inline bool isEqual(Vec3D const& _vec, TValue _eps = static_cast<TValue>(1e-04)) const;

        //////////////////////////////////////////
        inline Vec3D normalizedCopy() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D reflect(Vec3D const& _normal) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isNaN() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isInfinity() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isFinite() const { return !isNaN() && !isInfinity(); }


        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(x) + ";" + StringHelper::ToString(y) + ";" + StringHelper::ToString(z); }

        //////////////////////////////////////////
        static Vec3D FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 3)
            {
                return Vec3D(
                    static_cast<TValue>(StringHelper::StringToF32(words[0])),
                    static_cast<TValue>(StringHelper::StringToF32(words[1])),
                    static_cast<TValue>(StringHelper::StringToF32(words[2])));
            }

            return Vec3D::c_zero;
        }

        //////////////////////////////////////////
        static inline Vec3D SLerp(Vec3D const& _start, Vec3D const& _end, F32 _percent)
        {
            TValue cosAlpha = _start.dotProduct(_end);
            TValue alpha = Math::ACos(cosAlpha);
            TValue sinAlpha = Math::Sin(alpha);
            TValue t1 = Math::Sin((static_cast<TValue>(1) - static_cast<TValue>(_percent)) * alpha) / sinAlpha;
            TValue t2 = Math::Sin(_percent * alpha) / sinAlpha;

            return _start * t1 + _end * t2;
        }

        //////////////////////////////////////////
        inline Vec3D slerp(Vec3D const& _end, F32 _percent)
        {
            return SLerp(*this, _end, _percent);
        }


        //////////////////////////////////////////
        Vec3D perpendicular() const;

        //////////////////////////////////////////
        TValue normalize();

        //////////////////////////////////////////
        TValue normalizeTo(TValue _requiredLength);

        //////////////////////////////////////////
        static Vec3D<TValue> RandomDirection();

        //////////////////////////////////////////
        static Vec3D<TValue> RandomHemisphereDirection(Vec3D<TValue> const& _normal);


        //////////////////////////////////////////
        inline F32 getAngleBetween(Vec3D const& _vector) const
        {
            F32 angle = dotProduct(_vector);
            angle /= (length() * _vector.length());
            return angle = Math::ACos(angle);
        }

        //////////////////////////////////////////
        static inline F32 GetAngleBetween(
            Vec3D const& _vectorA,
            Vec3D const& _vectorB)
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
    inline MAZE_CONSTEXPR Vec3D<TValue> operator*(TValue const _value, Vec3D<TValue> const& _vec)
    {
        return Vec3D<TValue>(
            _value * _vec.x,
            _value * _vec.y,
            _value * _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> operator/(TValue const _value, Vec3D<TValue> const& _vec)
    {
        return Vec3D<TValue>(
            _value / _vec.x,
            _value / _vec.y,
            _value / _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> operator+(Vec3D<TValue> const& _vec, TValue const _value)
    {
        return Vec3D<TValue>(
            _vec.x + _value,
            _vec.y + _value,
            _vec.z + _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> operator+(TValue const _value, Vec3D<TValue> const& _vec)
    {
        return Vec3D<TValue>(
            _value + _vec.x,
            _value + _vec.y,
            _value + _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> operator-(Vec3D<TValue> const& _vec, TValue const _value)
    {
        return Vec3D<TValue>(
            _vec.x - _value,
            _vec.y - _value,
            _vec.z - _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> operator-(TValue const _value, Vec3D<TValue> const& _vec)
    {
        return Vec3D<TValue>(
            _value - _vec.x,
            _value - _vec.y,
            _value - _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec3D<TValue> const& _v)
    {
        _o << "Vec3D(" << _v.x << ", " << _v.y << ", " << _v.z <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsVec3D : std::false_type {};
    template <class TValue>
    struct IsVec3D<Maze::Vec3D<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3D<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3D<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3D<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3D<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsVec3D<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Vec3DS = Vec3D<S32>;
    using Vec3DU = Vec3D<U32>;
    using Vec3DF = Vec3D<F32>;


    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec3DF Math::Floor<Vec3DF>(Vec3DF const& _value)
    { 
        return Vec3DF(Floor(_value.x), Floor(_value.y), Floor(_value.z)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec3DF Math::Ceil<Vec3DF>(Vec3DF const& _value)
    { 
        return Vec3DF(Ceil(_value.x), Ceil(_value.y), Ceil(_value.z)); 
    }

    //////////////////////////////////////////
    template <>
    inline MAZE_CONSTEXPR Vec3DF Math::Round<Vec3DF>(Vec3DF const& _value)
    { 
        return Vec3DF(Round(_value.x), Round(_value.y), Round(_value.z)); 
    }
    

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeVec3D.inl"


#endif // _MazeVec3D_hpp_
//////////////////////////////////////////
