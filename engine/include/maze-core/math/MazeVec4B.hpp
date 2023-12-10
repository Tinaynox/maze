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
#if (!defined(_MazeVec4B_hpp_))
#define _MazeVec4B_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2B.hpp"
#include "maze-core/math/MazeVec3B.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Vec4B
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Vec4B
    {
    public:

        //////////////////////////////////////////
        static Vec4B const c_false;
        static Vec4B const c_true;

    public:

        //////////////////////////////////////////
        inline Vec4B()
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4B(
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
        inline MAZE_CONSTEXPR explicit Vec4B(bool _x)
            : x(_x)
            , y(_x)
            , z(_x)
            , w(_x)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec4B(bool* const _arr)
            : x(_arr[0])
            , y(_arr[1])
            , z(_arr[2])
            , w(_arr[3])
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4B(Vec4B const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_vec.w)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4B(Vec4B&& _vec) noexcept
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_vec.w)
        {}

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4B(Vec4B const& _vec)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_vec.z))
            , w(static_cast<bool>(_vec.w))
        {
        }


        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4B(
            Vec2<UValue> const& _vec,
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
        inline MAZE_CONSTEXPR Vec4B(Vec2<UValue> const& _vec, DValue _z, FValue _w = 1)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_z))
            , w(static_cast<bool>(_w))
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4B(
            Vec2<UValue> const& _vecXY,
            Vec2<UValue> const& _vecZW)
            : x(static_cast<bool>(_vecXY.x))
            , y(static_cast<bool>(_vecXY.y))
            , z(static_cast<bool>(_vecZW.x))
            , w(static_cast<bool>(_vecZW.y))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4B(Vec3B const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(1)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec4B(Vec3<UValue> const& _vec)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_vec.z))
            , w(1)
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec4B(Vec3B const& _vec, bool _w)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
            , w(_w)
        {
        }

        //////////////////////////////////////////
        template <class UValue, class DValue>
        inline MAZE_CONSTEXPR Vec4B(Vec3<UValue> const& _vec, DValue _w)
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
        inline Vec2B xy() const { return { x, y }; }

        //////////////////////////////////////////
        inline Vec3B xyz() const { return { x, y, z }; }

        //////////////////////////////////////////
        inline Vec2B zw() const { return { z, w }; }

        //////////////////////////////////////////
        inline void swap(Vec4B& _other)
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
        inline Vec4B& operator=(Vec4B const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;
            w = _vec.w;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4B& operator=(Vec4B&& _vec) noexcept
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;
            w = _vec.w;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4B& operator=(bool _value)
        {
            x = _value;
            y = _value;
            z = _value;
            w = _value;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4B& operator=(Vec2B const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = false;
            w = false;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec4B& operator=(Vec3B const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;
            w = false;

            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec4B const& _vec) const
        {
            return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec4B const& _vec) const
        {
            return (x != _vec.x || y != _vec.y || z != _vec.z || w != _vec.w);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec4B const& _vec) const
        {
            return (x < _vec.x&& y < _vec.y&& z < _vec.z&& w < _vec.w);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec4B const& _vec) const
        {
            return (x > _vec.x && y > _vec.y && z > _vec.z && w > _vec.w);
        }


        //////////////////////////////////////////
        String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec4B& _result, Char _separator = ';');

        //////////////////////////////////////////
        static CString ParseStringPretty(CString _string, Size _size, Vec4B& _result, Char _separator = ';');

        //////////////////////////////////////////
        static Vec4B FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Vec4B FromString(String const& _string, Char _separator = ';');


    public:
        bool x = false;
        bool y = false;
        bool z = false;
        bool w = false;
    }; 


    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec4B const& _v)
    {
        _o << "Vec4B(" << _v.x << ", " << _v.y << ", " << _v.z << ", " << _v.w <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <>
    struct IsVec4<Vec4B> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Vec4B const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Vec4B& _value, CString _data, Size _count)
    {
        _value = Vec4B::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Vec4B const& _value)
    {
        return sizeof(Vec4B);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Vec4B const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Vec4B));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Vec4B& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Vec4B));
    }



} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVec4B_hpp_
//////////////////////////////////////////
