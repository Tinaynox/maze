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
#if (!defined(_MazePlane_hpp_))
#define _MazePlane_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeRay.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Plane
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Plane
    {
    public:

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Plane() 
            : m_point(Vec3DF::c_zero)
            , m_normal(Vec3DF::c_unitZ)
        { 
        }

        //////////////////////////////////////////
        inline Plane(Plane const& _plane) 
            : m_point(_plane.m_point)
            , m_normal(_plane.m_normal)
        {
        }

        //////////////////////////////////////////
        inline Plane(Plane&& _plane) noexcept
            : m_point(std::move(_plane.m_point))
            , m_normal(std::move(_plane.m_normal))
        {
        }

        //////////////////////////////////////////
        inline explicit Plane(Vec3DF const& _point, Vec3DF const& _normal)
            : m_point(_point)
            , m_normal(_normal)
        {
        }

        //////////////////////////////////////////
        inline void setPoint(Vec3DF const& _normal) { m_point = _normal; }

        //////////////////////////////////////////
        inline Vec3DF const& getPoint() const { return m_point; }

        //////////////////////////////////////////
        inline void setNormal(Vec3DF const& _normal) { m_normal = _normal; }

        //////////////////////////////////////////
        inline Vec3DF const& getNormal() const { return m_normal; }


        //////////////////////////////////////////
        bool raycast(Ray const& _ray, F32& _dist);


        //////////////////////////////////////////
        inline Plane& operator=(Plane const& _plane)
        {
            m_point = _plane.m_point;
            m_normal = _plane.m_normal;

            return *this;
        }

        //////////////////////////////////////////
        inline Plane& operator=(Plane&& _plane) noexcept
        {
            m_point = std::move(_plane.m_point);
            m_normal = std::move(_plane.m_normal);
            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Plane const& _plane) const
        {
            return (m_point == _plane.m_point && m_normal == _plane.m_normal);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Plane const& _plane) const
        {
            return (m_point != _plane.m_point || m_normal != _plane.m_normal);
        }

        //////////////////////////////////////////
        inline String toString() const { return m_point.toString() + ":" + m_normal.toString(); }

        //////////////////////////////////////////
        inline static Plane FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ':');
            if (words.size() == 2)
            {
                return Plane(
                    Vec3DF::FromString(words[0]),
                    Vec3DF::FromString(words[1]));
            }

            return Plane();
        }

    protected:
        Vec3DF m_point;
        Vec3DF m_normal;
    };

    
    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Plane const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Plane& _value, CString _data, Size _count)
    {
        _value = Plane::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Plane const& _value)
    {
        return sizeof(Plane);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Plane const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Plane));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Plane& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Plane));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePlane_hpp_
//////////////////////////////////////////
