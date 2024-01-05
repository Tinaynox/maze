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
#if (!defined(_MazeVec3B_hpp_))
#define _MazeVec3B_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2B.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec3B
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Vec3B
    {
    public:

        //////////////////////////////////////////
        static Vec3B const c_false;
        static Vec3B const c_true;

    public:

        //////////////////////////////////////////
        inline Vec3B()
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3B(
            bool _x,
            bool _y,
            bool _z)
            : x(_x)
            , y(_y)
            , z(_z)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3B(bool _x)
            : x(_x)
            , y(_x)
            , z(_x)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec3B(bool* const _arr)
            : x(_arr[0])
            , y(_arr[1])
            , z(_arr[2])
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3B(Vec3B const& _vec)
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3B(Vec3B&& _vec) noexcept
            : x(_vec.x)
            , y(_vec.y)
            , z(_vec.z)
        {}

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3B(Vec3<UValue> const& _vec)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_vec.z))
        {
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3B(Vec2B const& _vec, bool _z = 0)
            : x(_vec.x)
            , y(_vec.y)
            , z(_z)
        {
        }

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec3B(Vec2<UValue> const& _vec, UValue _z = 0)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
            , z(static_cast<bool>(_z))
        {
        }

        //////////////////////////////////////////
        inline void set(
            bool _x,
            bool _y,
            bool _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        //////////////////////////////////////////
        inline void set(bool _x)
        {
            x = _x;
            y = _x;
            z = _x;
        }

        //////////////////////////////////////////
        inline void setX(bool _x) { x = _x; }

        //////////////////////////////////////////
        inline void setY(bool _y) { y = _y; }

        //////////////////////////////////////////
        inline void setZ(bool _z) { z = _z; }

        //////////////////////////////////////////
        inline bool getX() const { return x; }

        //////////////////////////////////////////
        inline bool getY() const { return y; }

        //////////////////////////////////////////
        inline bool getZ() const { return z; }

        //////////////////////////////////////////
        inline Vec2B xy() const { return { x, y }; }

        //////////////////////////////////////////
        inline void swap(Vec3B& _other)
        {
            std::swap(x, _other.x);
            std::swap(y, _other.y);
            std::swap(z, _other.z);
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
        inline Vec3B& operator=(Vec3B const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec3B& operator=(Vec3B&& _vec) noexcept
        {
            x = _vec.x;
            y = _vec.y;
            z = _vec.z;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec3B& operator=(bool _value)
        {
            x = _value;
            y = _value;
            z = _value;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec3B& operator=(Vec2B const& _vec)
        {
            x = _vec.x;
            y = _vec.y;
            z = false;

            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec3B const& _vec) const
        {
            return (x == _vec.x && y == _vec.y && z == _vec.z);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec3B const& _vec) const
        {
            return (x != _vec.x || y != _vec.y || z != _vec.z);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec3B const& _vec) const
        {
            return (x < _vec.x&& y < _vec.y&& z < _vec.z);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec3B const& _vec) const
        {
            return (x > _vec.x && y > _vec.y && z > _vec.z);
        }

        //////////////////////////////////////////
        String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec3B& _result, Char _separator = ';');

        //////////////////////////////////////////
        static CString ParseStringPretty(CString _string, Size _size, Vec3B& _result, Char _separator = ';');

        //////////////////////////////////////////
        static Vec3B FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Vec3B FromString(String const& _string, Char _separator = ';');

    public:
        bool x = false;
        bool y = false;
        bool z = false;
    }; 


    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec3B const& _v)
    {
        _o << "Vec3B(" << _v.x << ", " << _v.y << ", " << _v.z <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <>
    struct IsVec3<Vec3B> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Vec3B const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Vec3B& _value, CString _data, Size _count)
    {
        _value = Vec3B::FromString(_data, _count);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void GetValueSerializationSize(Vec3B const& _value, U32& _outSize)
    {
        _outSize = sizeof(Vec3B);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void SerializeValue(Vec3B const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Vec3B));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void DeserializeValue(Vec3B& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Vec3B));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVec3B_hpp_
//////////////////////////////////////////
