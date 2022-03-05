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
#if (!defined(_MazeVec2DB_hpp_))
#define _MazeVec2DB_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Vec2DB
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Vec2DB
    {
    public:

        ////////////////////////////////////
        static Vec2DB const c_false;
        static Vec2DB const c_true;

    public:

        //////////////////////////////////////////
        inline Vec2DB()
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DB(bool _x, bool _y)
            : x(_x)
            , y(_y)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2DB(bool _x)
            : x(_x)
            , y(_x)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR explicit Vec2DB(bool* const _arr)
            : x(_arr[0])
            , y(_arr[1])
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DB(Vec2DB const& _vec)
            : x(_vec.x)
            , y(_vec.y)
        {}

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DB(Vec2DB&& _vec) noexcept
            : x(_vec.x)
            , y(_vec.y)
        {}

        //////////////////////////////////////////
        template <class UValue>
        inline MAZE_CONSTEXPR Vec2DB(Vec2D<UValue> const& _vec)
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
        inline void swap(Vec2DB& _other)
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
        inline Vec2DB& operator=(Vec2DB const& _vec)
        {
            x = _vec.x;
            y = _vec.y;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec2DB& operator=(Vec2DB&& _vec) noexcept
        {
            x = _vec.x;
            y = _vec.y;

            return *this;
        }

        //////////////////////////////////////////
        inline Vec2DB& operator=(bool _value)
        {
            x = _value;
            y = _value;

            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Vec2DB const& _vec) const
        {
            return (x == _vec.x && y == _vec.y);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Vec2DB const& _vec) const
        {
            return (x != _vec.x || y != _vec.y);
        }


        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator<(Vec2DB const& _vec) const
        {
            return (x < _vec.x&& y < _vec.y);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator>(Vec2DB const& _vec) const
        {
            return (x > _vec.x && y > _vec.y);
        }


        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(x) + ";" + StringHelper::ToString(y); }

        //////////////////////////////////////////
        static Vec2DB FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 2)
            {
                return Vec2DB(
                    static_cast<bool>(StringHelper::StringToBool(words[0])),
                    static_cast<bool>(StringHelper::StringToBool(words[1])));
            }

            return Vec2DB::c_false;
        }

    public:
        bool x = false;
        bool y = false;
    }; 


    //////////////////////////////////////////
    template <class TValue> 
    inline std::ostream& operator<<(std::ostream& _o, Vec2DB const& _v)
    {
        _o << "Vec2DB(" << _v.x << ", " << _v.y <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <>
    struct IsVec2D<Vec2DB> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Vec2DB const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Vec2DB& _value, CString _data, Size _count)
    {
        _value = Vec2DB::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Vec2DB const& _value)
    {
        return sizeof(Vec2DB);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Vec2DB const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Vec2DB));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Vec2DB& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Vec2DB));
    }
   

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVec2DB_hpp_
//////////////////////////////////////////
