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
#include "maze-core/math/MazeVec3.hpp"
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
        inline MAZE_CONSTEXPR Plane() = default;

        //////////////////////////////////////////
        inline Plane(Plane const& _plane) = default;

        //////////////////////////////////////////
        inline Plane(Plane&& _plane) noexcept = default;

        //////////////////////////////////////////
        inline Plane(Vec3F const& _normal, F32 _d)
            : m_normal(_normal)
            , m_d(_d)
        { }

        //////////////////////////////////////////
        inline explicit Plane(Vec3F const& _point, Vec3F const& _normal)
            : m_normal(_normal)
        {
            setPoint(_point);
        }

        //////////////////////////////////////////
        inline void setPoint(Vec3F const& _point) { m_d = -m_normal.dotProduct(_point); }

        //////////////////////////////////////////
        inline Vec3F getPoint() const { return -m_d * m_normal; }


        //////////////////////////////////////////
        inline F32 getD() const { return m_d; }

        //////////////////////////////////////////
        inline void setD(F32 _d) { m_d = _d; }

        //////////////////////////////////////////
        inline void setNormal(Vec3F const& _normal) { m_normal = _normal; }

        //////////////////////////////////////////
        inline Vec3F const& getNormal() const { return m_normal; }


        //////////////////////////////////////////
        inline void redefine(Vec3F const& _normal, Vec3F const& _point) { setNormal(_normal); setPoint(_point); }

        //////////////////////////////////////////
        bool raycast(Ray const& _ray, F32& _dist);

        //////////////////////////////////////////
        F32 normalize();


        //////////////////////////////////////////
        inline Plane& operator=(Plane const& _plane)
        {
            m_d = _plane.m_d;
            m_normal = _plane.m_normal;

            return *this;
        }

        //////////////////////////////////////////
        inline Plane& operator=(Plane&& _plane) noexcept = default;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Plane const& _plane) const
        {
            return (m_normal == _plane.m_normal && m_d == _plane.m_d);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Plane const& _plane) const
        {
            return (m_normal != _plane.m_normal || m_d != _plane.m_d);
        }

        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const { return m_normal.toString() + _separator + StringHelper::ToString(m_d); }

        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, Plane& _result, Char _separator = ';')
        {
            CString end = _string + _size;

            _string = Vec3F::ParseString(_string, end - _string, _result.m_normal);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<F32>(_string, end, _result.m_d);
            return _string;
        }

        //////////////////////////////////////////
        static inline Plane FromString(CString _string, Size _size, Char _separator = ';')
        {
            Plane result;
            ParseString(_string, _size, result, _separator);
            return result;
        }

        //////////////////////////////////////////
        inline static Plane FromString(String const& _string, Char _separator = ';')
        {
            return FromString(&_string[0], _string.size(), _separator);
        }

    protected:
        Vec3F m_normal = Vec3F::c_zero;
        F32 m_d = 0.0f;
    };



} // namespace Maze
//////////////////////////////////////////


#endif // _MazePlane_hpp_
//////////////////////////////////////////
