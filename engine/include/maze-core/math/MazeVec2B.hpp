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
#if (!defined(_MazeVec2B_hpp_))
#define _MazeVec2B_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec2B
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Vec2B
    {
    public:

        //////////////////////////////////////////
        static Vec2B const c_false;
        static Vec2B const c_true;

    public:

        //////////////////////////////////////////
        inline Vec2B()
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2B(bool _x, bool _y)
            : x(_x)
            , y(_y)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2B(bool _x)
            : x(_x)
            , y(_x)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2B(bool* const _arr)
            : x(_arr[0])
            , y(_arr[1])
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2B(Vec2B const& _vec)
            : x(_vec.x)
            , y(_vec.y)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2B(Vec2B&& _vec) noexcept
            : x(_vec.x)
            , y(_vec.y)
        {}

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2B(Vec2<UValue> const& _vec)
            : x(static_cast<bool>(_vec.x))
            , y(static_cast<bool>(_vec.y))
        {
        }

        //////////////////////////////////////////
        inline void set(bool _x, bool _y)
        {
            x = _x;
            y = _y;
        }

        //////////////////////////////////////////
        inline void set(bool _x)
        {
            x = _x;
            y = _x;
        }

        //////////////////////////////////////////
        inline void setX(bool _x) { x = _x; }

        //////////////////////////////////////////
        inline void setY(bool _y) { y = _y; }

        //////////////////////////////////////////
        inline bool getX() const { return x; }

        //////////////////////////////////////////
        inline bool getY() const { return y; }

        //////////////////////////////////////////
        inline void swap(Vec2B& _other)
        {
            std::swap(x, _other.x);
            std::swap(y, _other.y);
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
        inline Vec2B& operator=(Vec2B const& _vec)
        {
            x = _vec.x;
            y = _vec.y;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec2B& operator=(Vec2B&& _vec) noexcept
        {
            x = _vec.x;
            y = _vec.y;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec2B& operator=(bool _value)
        {
            x = _value;
            y = _value;

            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec2B const& _vec) const
        {
            return (x == _vec.x && y == _vec.y);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec2B const& _vec) const
        {
            return (x != _vec.x || y != _vec.y);
        }


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec2B const& _vec) const
        {
            return (x < _vec.x&& y < _vec.y);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec2B const& _vec) const
        {
            return (x > _vec.x && y > _vec.y);
        }


        //////////////////////////////////////////
        String toString(Char _separator = ',') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, Vec2B& _result, Char _separator = ',');

        //////////////////////////////////////////
        static CString ParseStringPretty(CString _string, Size _size, Vec2B& _result, Char _separator = ',');

        //////////////////////////////////////////
        static Vec2B FromString(CString _string, Size _size, Char _separator = ',');

        //////////////////////////////////////////
        static Vec2B FromString(String const& _string, Char _separator = ',');

    public:
        bool x = false;
        bool y = false;
    }; 


    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec2B const& _v)
    {
        _o << "Vec2B(" << _v.x << ", " << _v.y <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <>
    struct IsVec2<Vec2B> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Vec2B const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Vec2B& _value, CString _data, Size _count)
    {
        _value = Vec2B::FromString(_data, _count);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void GetValueSerializationSize(Vec2B const& _value, U32& _outSize)
    {
        _outSize = sizeof(Vec2B);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void SerializeValue(Vec2B const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Vec2B));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void DeserializeValue(Vec2B& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Vec2B));
    }
   

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVec2B_hpp_
//////////////////////////////////////////
