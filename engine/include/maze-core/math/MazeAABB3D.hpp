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
#if (!defined(_MazeAABB3D_hpp_))
#define _MazeAABB3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class AABB3D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AABB3D
    {
    public:

        //////////////////////////////////////////
        static AABB3D const c_zero;
        
    public:
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR AABB3D()
            : m_min(Vec3F::c_zero)
            , m_max(Vec3F::c_zero)
        {
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR AABB3D(
            F32 _minX,
            F32 _minY,
            F32 _minZ,
            F32 _maxX,
            F32 _maxY,
            F32 _maxZ)
            : m_min(_minX, _minY, _minZ)
            , m_max(_maxX, _maxY, _maxZ)
        {
        }
        
        
        //////////////////////////////////////////
        inline AABB3D(Vec3F const& _point0, Vec3F const& _point1)
        {
            set(_point0, _point1);
        }

        //////////////////////////////////////////
        inline AABB3D(Vec3F const& _point)
            : AABB3D(
                _point.x,
                _point.y,
                _point.z,
                _point.x,
                _point.y,
                _point.z)
        {
            
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR AABB3D(AABB3D const& _aabb)
            : m_min(_aabb.m_min)
            , m_max(_aabb.m_max)
        {
            
        }
        
        //////////////////////////////////////////
        inline AABB3D(AABB3D&& _aabb) noexcept
            : m_min(std::move(_aabb.m_min))
            , m_max(std::move(_aabb.m_max)) 
        {
            
        }

        //////////////////////////////////////////
        inline void setMin(Vec3F const& _min) { m_min = _min; }
        
        //////////////////////////////////////////
        inline void setMin(F32 _x, F32 _y, F32 _z) { m_min.x = _x; m_min.y = _y; m_min.z = _z; }
        
        //////////////////////////////////////////
        inline void setMinX(F32 _x) { m_min.x = _x; }
        
        //////////////////////////////////////////
        inline void setMinY(F32 _y) { m_min.y = _y; }

        //////////////////////////////////////////
        inline void setMinZ(F32 _z) { m_min.z = _z; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F const& getMin() const { return m_min; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMinX() const { return m_min.x; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMinY() const { return m_min.y; }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMinZ() const { return m_min.z; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F const& getLeftTop() const { return m_min; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F getRightTop() const { return Vec3F(m_max.x, m_min.y, m_min.z); }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F getLeftBottom() const { return Vec3F(m_min.x, m_max.y, m_max.z); }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F const& getRightBottom() const { return m_max; }
        
        
        //////////////////////////////////////////
        inline void setMax(Vec3F const& _max) { m_max = _max; }
        
        //////////////////////////////////////////
        inline void setMax(F32 _x, F32 _y, F32 _z) { m_max.x = _x; m_max.y = _y; m_max.z = _z; }
        
        //////////////////////////////////////////
        inline void setMaxX(F32 _x) { m_max.x = _x; }
        
        //////////////////////////////////////////
        inline void setMaxY(F32 _y) { m_max.y = _y; }

        //////////////////////////////////////////
        inline void setMaxZ(F32 _z) { m_max.z = _z; }
        
        //////////////////////////////////////////
        inline void set(F32 _x, F32 _y, F32 _z)
        {
            m_min.x = m_max.x = _x;
            m_min.y = m_max.y = _y;
            m_min.z = m_max.z = _z;
        }
        
        //////////////////////////////////////////
        inline void set(Vec3F const& _point0, Vec3F const& _point1)
        {
            if (_point0.x < _point1.x)
            {
                setMinX(_point0.x);
                setMaxX(_point1.x);
            }
            else
            {
                setMinX(_point1.x);
                setMaxX(_point0.x);
            }
            
            if (_point0.y < _point1.y)
            {
                setMinY(_point0.y);
                setMaxY(_point1.y);
            }
            else
            {
                setMinY(_point1.y);
                setMaxY(_point0.y);
            }
            
            if (_point0.z < _point1.z)
            {
                setMinZ(_point0.z);
                setMaxZ(_point1.z);
            }
            else
            {
                setMinZ(_point1.z);
                setMaxZ(_point0.z);
            }
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F const& getMax() const { return m_max; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMaxX() const { return m_max.x; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMaxY() const { return m_max.y; }
        
        //////////////////////////////////////////
        inline AABB3D& operator=(AABB3D const& _aabb)
        {
            m_min = _aabb.m_min;
            m_max = _aabb.m_max;
            
            return *this;
        }
        
        //////////////////////////////////////////
        inline AABB3D& operator=(AABB3D&& _aabb) noexcept
        {
            m_min = std::move(_aabb.m_min);
            m_max = std::move(_aabb.m_max);
            
            return *this;
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(AABB3D const& _aabb) const
        {
            return m_min == _aabb.m_min && m_max == _aabb.m_max;
        }
        
        //////////////////////////////////////////
        inline bool intersects(AABB3D const& _aabb) const
        {
            if (    m_min.x <= _aabb.m_max.x && m_max.x >= _aabb.m_min.x 
                &&  m_min.y <= _aabb.m_max.y && m_max.y >= _aabb.m_min.y
                &&  m_min.z <= _aabb.m_max.z && m_max.z >= _aabb.m_min.z)
                return true;
            
            return false;
        }
        
        //////////////////////////////////////////
        inline bool contains(Vec3F const& _p) const
        {
            if (    m_min.x <= _p.x && m_max.x >= _p.x 
                &&  m_min.y <= _p.y && m_max.y >= _p.y
                &&  m_min.z <= _p.z && m_max.z >= _p.z)
                return true;
            
            return false;
        }
        
        //////////////////////////////////////////
        inline F32 closestDistanceTo(AABB3D const& _aabb) const
        {            
            if (intersects(_aabb))
                return 0.0f;
            
            return Math::Min(
                Math::Min(Math::Abs(m_min.x - _aabb.m_max.x), Math::Abs(_aabb.m_min.x - m_max.x)),
                Math::Min(Math::Abs(m_min.y - _aabb.m_max.y), Math::Abs(_aabb.m_min.y - m_max.y)),
                Math::Min(Math::Abs(m_min.z - _aabb.m_max.z), Math::Abs(_aabb.m_min.z - m_max.z)));
        }
        
        //////////////////////////////////////////
        inline void applyIntersection(AABB3D const& _aabb)
        {
            m_min.x = Math::Max(m_min.x, _aabb.m_min.x);
            m_max.x = Math::Min(m_max.x, _aabb.m_max.x);
            m_min.y = Math::Max(m_min.y, _aabb.m_min.y);
            m_max.y = Math::Min(m_max.y, _aabb.m_max.y);
            m_min.z = Math::Max(m_min.z, _aabb.m_min.z);
            m_max.z = Math::Min(m_max.z, _aabb.m_max.z);
        }
        
        //////////////////////////////////////////
        inline void applyUnion(AABB3D const& _aabb)
        {
            m_min.x = Math::Min(m_min.x, _aabb.m_min.x);
            m_max.x = Math::Max(m_max.x, _aabb.m_max.x);
            m_min.y = Math::Min(m_min.y, _aabb.m_min.y);
            m_max.y = Math::Max(m_max.y, _aabb.m_max.y);
            m_min.z = Math::Min(m_min.z, _aabb.m_min.z);
            m_max.z = Math::Max(m_max.z, _aabb.m_max.z);
        }
        
        //////////////////////////////////////////
        inline void applyUnion(
            F32 _minX,
            F32 _minY,
            F32 _minZ,
            F32 _maxX,
            F32 _maxY,
            F32 _maxZ)
        {
            m_min.x = Math::Min(m_min.x, _minX);
            m_max.x = Math::Max(m_max.x, _maxX);
            m_min.y = Math::Min(m_min.y, _minY);
            m_max.y = Math::Max(m_max.y, _maxY);
            m_min.z = Math::Min(m_min.z, _minZ);
            m_max.z = Math::Max(m_max.z, _maxZ);
        }
        
        //////////////////////////////////////////
        inline void applyUnion(Vec3F const& _point)
        {
            m_min.x = Math::Min(m_min.x, _point.x);
            m_max.x = Math::Max(m_max.x, _point.x);
            m_min.y = Math::Min(m_min.y, _point.y);
            m_max.y = Math::Max(m_max.y, _point.y);
            m_min.z = Math::Min(m_min.z, _point.z);
            m_max.z = Math::Max(m_max.z, _point.z);
        }
        
        //////////////////////////////////////////
        inline AABB3D unionCopy(AABB3D const& _aabb) const
        {
            AABB3D result;
            
            result.m_min.x = Math::Min(m_min.x, _aabb.m_min.x);
            result.m_max.x = Math::Max(m_max.x, _aabb.m_max.x);
            result.m_min.y = Math::Min(m_min.y, _aabb.m_min.y);
            result.m_max.y = Math::Max(m_max.y, _aabb.m_max.y);
            result.m_min.z = Math::Min(m_min.z, _aabb.m_min.z);
            result.m_max.z = Math::Max(m_max.z, _aabb.m_max.z);
            
            return result;
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3F getSize() const
        {
            return Vec3F(m_max.x - m_min.x, m_max.y - m_min.y, m_max.z - m_min.z);
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isValid() const
        {
            return (m_min.x < m_max.x) && (m_min.y < m_max.y) && (m_min.z < m_max.z);
        }
        
        //////////////////////////////////////////
        String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static CString ParseString(CString _string, Size _size, AABB3D& _result, Char _separator = ';');

        //////////////////////////////////////////
        static AABB3D FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static AABB3D FromString(String const& _string, Char _separator = ';');
        
    protected:
        Vec3F m_min;
        Vec3F m_max;
    };
    
    
    //////////////////////////////////////////
    inline std::ostream& operator<<(std::ostream& _o, AABB3D const& _aabb)
    {
        _o << "AABB3D(" << _aabb.getMin() << ", " << _aabb.getMax() <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(AABB3D const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(AABB3D& _value, CString _data, Size _count)
    {
        _value = AABB3D::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(AABB3D const& _value)
    {
        return sizeof(AABB3D);
    }

    //////////////////////////////////////////
    inline void SerializeValue(AABB3D const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(AABB3D));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(AABB3D& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(AABB3D));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAABB3D_hpp_
//////////////////////////////////////////
