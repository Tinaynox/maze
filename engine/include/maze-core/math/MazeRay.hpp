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
#if (!defined(_MazeRay_hpp_))
#define _MazeRay_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Ray
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Ray
    {
    public:

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Ray()
            : m_point(Vec3F::c_zero)
            , m_direction(Vec3F::c_unitZ)
        {
        }

        //////////////////////////////////////////
        inline Ray(Ray const& _plane)
            : m_point(_plane.m_point)
            , m_direction(_plane.m_direction)
        {
        }

        //////////////////////////////////////////
        inline Ray(Ray&& _plane) noexcept
            : m_point(std::move(_plane.m_point))
            , m_direction(std::move(_plane.m_direction))
        {
        }

        //////////////////////////////////////////
        inline explicit Ray(Vec3F const& _point, Vec3F const& _direction)
            : m_point(_point)
            , m_direction(_direction)
        {
        }

        //////////////////////////////////////////
        inline void setPoint(Vec3F const& _direction) { m_point = _direction; }

        //////////////////////////////////////////
        inline Vec3F const& getPoint() const { return m_point; }

        //////////////////////////////////////////
        inline void setDirection(Vec3F const& _direction) { m_direction = _direction; }

        //////////////////////////////////////////
        inline Vec3F const& getDirection() const { return m_direction; }

        //////////////////////////////////////////
        inline Vec3F getPoint(F32 _length) const { return m_point + m_direction * _length; }


        //////////////////////////////////////////
        inline Ray& operator=(Ray const& _plane)
        {
            m_point = _plane.m_point;
            m_direction = _plane.m_direction;

            return *this;
        }

        //////////////////////////////////////////
        inline Ray& operator=(Ray&& _plane) noexcept
        {
            m_point = std::move(_plane.m_point);
            m_direction = std::move(_plane.m_direction);
            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Ray const& _plane) const
        {
            return (m_point == _plane.m_point && m_direction == _plane.m_direction);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Ray const& _plane) const
        {
            return (m_point != _plane.m_point || m_direction != _plane.m_direction);
        }

        //////////////////////////////////////////
        inline String toString(Char _separator = ':') const { return m_point.toString() + _separator + m_direction.toString(); }

        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, Ray& _result, Char _separator = ':')
        {
            CString end = _string + _size;

            _string = Vec3F::ParseString(_string, _size, _result.m_point);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = Vec3F::ParseString(_string, end - _string, _result.m_direction);
            return _string;
        }

        //////////////////////////////////////////
        static inline Ray FromString(CString _string, Size _size, Char _separator = ':')
        {
            Ray result;
            ParseString(_string, _size, result, _separator);
            return result;
        }

        //////////////////////////////////////////
        static inline Ray FromString(String const& _string, Char _separator = ':')
        {
            return FromString(&_string[0], _string.size(), _separator);
        }

    protected:
        Vec3F m_point;
        Vec3F m_direction;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(Ray const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(Ray& _value, CString _data, Size _count)
    {
        _value = Ray::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void GetValueSerializationSize(Ray const& _value, U32& _outSize)
    {
        _outSize = sizeof(Ray);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void SerializeValue(Ray const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Ray));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void DeserializeValue(Ray& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Ray));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Ray const& _value, DataBlock& _data)
    {
        _data.setVec3F(MAZE_HS("point"), _value.getPoint());
        _data.setVec3F(MAZE_HS("direction"), _value.getDirection());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Ray& _value, DataBlock const& _data)
    {
        _value.setPoint(_data.getVec3F(MAZE_HS("point")));
        _value.setDirection(_data.getVec3F(MAZE_HS("direction")));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRay_hpp_
//////////////////////////////////////////
