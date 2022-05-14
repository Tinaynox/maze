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
#include "maze-core/math/MazeVec3D.hpp"
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
            : m_point(Vec3DF::c_zero)
            , m_direction(Vec3DF::c_unitZ)
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
        inline explicit Ray(Vec3DF const& _point, Vec3DF const& _direction)
            : m_point(_point)
            , m_direction(_direction)
        {
        }

        //////////////////////////////////////////
        inline void setPoint(Vec3DF const& _direction) { m_point = _direction; }

        //////////////////////////////////////////
        inline Vec3DF const& getPoint() const { return m_point; }

        //////////////////////////////////////////
        inline void setDirection(Vec3DF const& _direction) { m_direction = _direction; }

        //////////////////////////////////////////
        inline Vec3DF const& getDirection() const { return m_direction; }

        //////////////////////////////////////////
        inline Vec3DF getPoint(F32 _length) const { return m_point + m_direction * _length; }


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
        inline String toString() const { return m_point.toString() + ":" + m_direction.toString(); }

        //////////////////////////////////////////
        inline static Ray FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ':');
            if (words.size() == 2)
            {
                return Ray(
                    Vec3DF::FromString(words[0]),
                    Vec3DF::FromString(words[1]));
            }

            return Ray();
        }

    protected:
        Vec3DF m_point;
        Vec3DF m_direction;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Ray const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Ray& _value, CString _data, Size _count)
    {
        _value = Ray::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Ray const& _value)
    {
        return sizeof(Ray);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Ray const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Ray));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Ray& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Ray));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRay_hpp_
//////////////////////////////////////////
