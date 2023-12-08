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
#if (!defined(_MazeVec4DB_hpp_))
#define _MazeVec4DB_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2DB.hpp"
#include "maze-core/math/MazeVec3DB.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Vec4DB
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Vec4DB
    {
    public:

        //////////////////////////////////////////
        static Vec4DB const c_false;
        static Vec4DB const c_true;

    public:

        //////////////////////////////////////////
        inline Vec4DB()
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4DB(
            bool _x,
            bool _y,
            bool _z,
            bool _w)
            : x(_x)
            , y(_y)
            , z(_z)
            , w(_w)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4DB(bool _x)
            : x(_x)
            , y(_x)
            , z(_x)
            , w(_x)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4DB(bool* const _arr)
            : x(_arr[0])
            , y(_arr[1])
            , z(_arr[2])
            , w(_arr[3])
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4DB(Vec4DB const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_vec.w)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4DB(Vec4DB&& _vec) noexcept
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_vec.w)
        {}

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4DB(Vec4DB const& _vec)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_vec.z))
            , w(static_cast<bool>(_vec.w))
        {
        }


        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4DB(
            Vec2D<UValue> const& _vec,
            UValue _z = 0,
            UValue _w = 1)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_z))
            , w(static_cast<bool>(_w))
        {
        }

        //////////////////////////////////////////
        template <class UValue, class DValue, class FValue>
        inline MAZE_CONSTEXPR Vec4DB(Vec2D<UValue> const& _vec, DValue _z, FValue _w = 1)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_z))
            , w(static_cast<bool>(_w))
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4DB(
            Vec2D<UValue> const& _vecXY,
            Vec2D<UValue> const& _vecZW)
            : x(static_cast<bool>(_vecXY.x))
            , y(static_cast<bool>(_vecXY.y))
            , z(static_cast<bool>(_vecZW.x))
            , w(static_cast<bool>(_vecZW.y))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4DB(Vec3DB const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(1)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4DB(Vec3D<UValue> const& _vec)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_vec.z))
            , w(1)
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4DB(Vec3DB const& _vec, bool _w)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_w)
        {
        }

        //////////////////////////////////////////
        template <class UValue, class DValue>
        inline MAZE_CONSTEXPR Vec4DB(Vec3D<UValue> const& _vec, DValue _w)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_vec.z))
            , w(static_cast<bool>(_w))
        {
        }

        //////////////////////////////////////////
        inline void set(
            bool const _x,
            bool const _y,
            bool const _z,
            bool const _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        //////////////////////////////////////////
        inline void set(bool _x)
        {
            x = _x;
            y = _x;
            z = _x;
            w = _x;
        }

        //////////////////////////////////////////
        inline void setX(bool _x) { x = _x; }

        //////////////////////////////////////////
        inline void setY(bool _y) { y = _y; }

        //////////////////////////////////////////
        inline void setZ(bool _z) { z = _z; }

        //////////////////////////////////////////
        inline void setW(bool _w) { w = _w; }

        //////////////////////////////////////////
        inline bool getX() const { return x; }

        //////////////////////////////////////////
        inline bool getY() const { return y; }

        //////////////////////////////////////////
        inline bool getZ() const { return z; }

        //////////////////////////////////////////
        inline bool getW() const { return w; }

        //////////////////////////////////////////
        inline Vec2DB xy() const { return { x, y }; }

        //////////////////////////////////////////
        inline Vec3DB xyz() const { return { x, y, z }; }

        //////////////////////////////////////////
        inline Vec2DB zw() const { return { z, w }; }

        //////////////////////////////////////////
        inline void swap(Vec4DB& _other)
        {
            std::swap(x, _other.x);
            std::swap(y, _other.y);
            std::swap(z, _other.z);
            std::swap(w, _other.w);
        }

        //////////////////////////////////////////
        inline bool operator[](Size const _i) const { return *(&x + _i); }

        //////////////////////////////////////////
        inline bool& operator[](Size const _i) { return *(&x + _i); }

        //////////////////////////////////////////
        inline bool* ptr() { return &x; }

        //////////////////////////////////////////
        inline const bool* ptr() const { return &x; }

        //////////////////////////////////////////
        inline Vec4DB& operator=(Vec4DB const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;
            w = _vec.w;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4DB& operator=(Vec4DB&& _vec) noexcept
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;
            w = _vec.w;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4DB& operator=(bool _value)
        {
            x = _value;
            y = _value;
            z = _value;
            w = _value;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4DB& operator=(Vec2DB const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = false;
            w = false;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4DB& operator=(Vec3DB const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;
            w = false;

            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec4DB const& _vec) const
        {
            return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec4DB const& _vec) const
        {
            return (x != _vec.x || y != _vec.y || z != _vec.z || w != _vec.w);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec4DB const& _vec) const
        {
            return (x < _vec.x&& y < _vec.y&& z < _vec.z&& w < _vec.w);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec4DB const& _vec) const
        {
            return (x > _vec.x && y > _vec.y && z > _vec.z && w > _vec.w);
        }


        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const { return StringHelper::ToString(x) + _separator + StringHelper::ToString(y) + _separator + StringHelper::ToString(z) + _separator + StringHelper::ToString(w); }

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec4DB& _result, Char _separator = ';')
        {
            CString end = _string + _size;

            S8 temp = 0;

            _string = StringHelper::ParseInteger<S8>(_string, end, temp);
            _result.x = (temp == 1);

            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::SkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');

            _string = StringHelper::ParseInteger<S8>(_string, end, temp);
            _result.y = (temp == 1);

            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::SkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');

            _string = StringHelper::ParseInteger<S8>(_string, end, temp);
            _result.z = (temp == 1);

            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::SkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');

            _string = StringHelper::ParseInteger<S8>(_string, end, temp);
            _result.w = (temp == 1);

            return _string;
        }

        //////////////////////////////////////////
        static Vec4DB FromString(CString _string, Size _size, Char _separator = ';')
        {
            Vec4DB result = Vec4DB(false);
            ParseString(_string, _size, result, _separator);
            return result;
        }

        //////////////////////////////////////////
        static Vec4DB FromString(String const& _string, Char _separator = ';')
        {
            return FromString(&_string[0], _string.size(), _separator);
        }


    public:
        bool x = false;
        bool y = false;
        bool z = false;
        bool w = false;
    }; 


    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec4DB const& _v)
    {
        _o << "Vec4DB(" << _v.x << ", " << _v.y << ", " << _v.z << ", " << _v.w <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <>
    struct IsVec4D<Vec4DB> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Vec4DB const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Vec4DB& _value, CString _data, Size _count)
    {
        _value = Vec4DB::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Vec4DB const& _value)
    {
        return sizeof(Vec4DB);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Vec4DB const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Vec4DB));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Vec4DB& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Vec4DB));
    }



} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVec4DB_hpp_
//////////////////////////////////////////
