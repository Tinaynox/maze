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
#if (!defined(_MazeAABB2D_hpp_))
#define _MazeAABB2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class AABB2D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AABB2D
    {
    public:

        //////////////////////////////////////////
        static AABB2D const c_zero;
        
    public:
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR AABB2D()
            : m_min(Vec2DF::c_zero)
            , m_max(Vec2DF::c_zero)
        {
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR AABB2D(F32 _minX, F32 _minY, F32 _maxX, F32 _maxY)
            : m_min(_minX, _minY)
            , m_max(_maxX, _maxY)
        {
        }
        
        
        //////////////////////////////////////////
        inline AABB2D(Vec2DF const& _point0, Vec2DF const& _point1)
        {
            set(_point0, _point1);
        }

        //////////////////////////////////////////
        inline AABB2D(Vec2DF const& _point)
        {
            set(_point, _point);
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR AABB2D(AABB2D const& _aabb)
            : m_min(_aabb.m_min)
            , m_max(_aabb.m_max)
        {
            
        }
        
        //////////////////////////////////////////
        inline AABB2D(AABB2D&& _aabb) noexcept
            : m_min(std::move(_aabb.m_min))
            , m_max(std::move(_aabb.m_max)) 
        {
            
        }

        //////////////////////////////////////////
        template <class UValue = F32>
        static AABB2D FromRect2D(Rect2D<UValue> const& _rect)
        {
            AABB2D result;
            result.m_min.x = static_cast<F32>(_rect.position.x);
            result.m_min.y = static_cast<F32>(_rect.position.y);
            result.m_max.x = static_cast<F32>(_rect.position.x + _rect.size.x);
            result.m_max.y = static_cast<F32>(_rect.position.y + _rect.size.y);
            return result;
        }

        //////////////////////////////////////////
        template <class UValue = F32>
        Rect2D<UValue> toRect() const
        {
            Rect2D<UValue> result;
            result.position.x = static_cast<UValue>(m_min.x);
            result.position.y = static_cast<UValue>(m_min.y);
            result.size.x = static_cast<UValue>(m_max.x - m_min.x);
            result.size.y = static_cast<UValue>(m_max.y - m_min.y);
            return result;
        }
        
        //////////////////////////////////////////
        inline void setMin(Vec2DF const& _min) { m_min = _min; }
        
        //////////////////////////////////////////
        inline void setMin(F32 _x, F32 _y) { m_min.x = _x; m_min.y = _y; }
        
        //////////////////////////////////////////
        inline void setMinX(F32 _x) { m_min.x = _x; }
        
        //////////////////////////////////////////
        inline void setMinY(F32 _y) { m_min.y = _y; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF const& getMin() const { return m_min; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMinX() const { return m_min.x; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMinY() const { return m_min.y; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF const& getLeftTop() const { return m_min; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getRightTop() const { return Vec2DF(m_max.x, m_min.y); }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getLeftBottom() const { return Vec2DF(m_min.x, m_max.y); }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF const& getRightBottom() const { return m_max; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getLeft() const { return Vec2DF(m_min.x, m_min.y + getHeight() * 0.5f); }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getRight() const { return Vec2DF(m_max.x, m_min.y + getHeight() * 0.5f); }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getBottom() const { return Vec2DF(m_min.x + getWidth() * 0.5f, m_min.y); }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getTop() const { return Vec2DF(m_min.x + getWidth() * 0.5f, m_max.y); }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 getWidth() const { return m_max.x - m_min.x; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 getHeight() const { return m_max.y - m_min.y; }
        
        //////////////////////////////////////////
        inline void setMax(Vec2DF const& _max) { m_max = _max; }
        
        //////////////////////////////////////////
        inline void setMax(F32 _x, F32 _y) { m_max.x = _x; m_max.y = _y; }
        
        //////////////////////////////////////////
        inline void setMaxX(F32 _x) { m_max.x = _x; }
        
        //////////////////////////////////////////
        inline void setMaxY(F32 _y) { m_max.y = _y; }
        
        //////////////////////////////////////////
        inline void set(F32 _x, F32 _y)
        {
            m_min.x = m_max.x = _x;
            m_min.y = m_max.y = _y;
        }
        
        //////////////////////////////////////////
        inline void set(Vec2DF const& _point0, Vec2DF const& _point1)
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
            
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF const& getMax() const { return m_max; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMaxX() const { return m_max.x; }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 const getMaxY() const { return m_max.y; }
        
        //////////////////////////////////////////
        inline AABB2D& operator=(AABB2D const& _aabb)
        {
            m_min = _aabb.m_min;
            m_max = _aabb.m_max;
            
            return *this;
        }
        
        //////////////////////////////////////////
        inline AABB2D& operator=(AABB2D&& _aabb) noexcept
        {
            m_min = std::move(_aabb.m_min);
            m_max = std::move(_aabb.m_max);
            
            return *this;
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(AABB2D const& _aabb) const
        {
            return m_min == _aabb.m_min && m_max == _aabb.m_max;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(AABB2D const& _aabb) const
        {
            return !operator==(_aabb);
        }
        
        //////////////////////////////////////////
        inline bool intersects(AABB2D const& _aabb) const
        {
            if (m_min.x <= _aabb.m_max.x && m_max.x >= _aabb.m_min.x && m_min.y <= _aabb.m_max.y && m_max.y >= _aabb.m_min.y)
                return true;
            
            return false;
        }
        
        //////////////////////////////////////////
        inline bool contains(Vec2DF const& _p) const
        {
            if (m_min.x <= _p.x && m_max.x >= _p.x && m_min.y <= _p.y && m_max.y >= _p.y)
                return true;
            
            return false;
        }
        
        //////////////////////////////////////////
        inline F32 closestDistanceTo(AABB2D const& _aabb) const
        {            
            if (intersects(_aabb))
                return 0.0f;
            
            return Math::Min(Math::Min(Math::Abs(m_min.x - _aabb.m_max.x), Math::Abs(_aabb.m_min.x - m_max.x)), Math::Min(Math::Abs(m_min.y - _aabb.m_max.y), Math::Abs(_aabb.m_min.y - m_max.y)));
        }
        
        //////////////////////////////////////////
        inline void applyIntersection(AABB2D const& _aabb)
        {
            m_min.x = Math::Max(m_min.x, _aabb.m_min.x);
            m_max.x = Math::Min(m_max.x, _aabb.m_max.x);
            m_min.y = Math::Max(m_min.y, _aabb.m_min.y);
            m_max.y = Math::Min(m_max.y, _aabb.m_max.y);
        }
        
        //////////////////////////////////////////
        inline void applyUnion(AABB2D const& _aabb)
        {
            m_min.x = Math::Min(m_min.x, _aabb.m_min.x);
            m_max.x = Math::Max(m_max.x, _aabb.m_max.x);
            m_min.y = Math::Min(m_min.y, _aabb.m_min.y);
            m_max.y = Math::Max(m_max.y, _aabb.m_max.y);
        }
        
        //////////////////////////////////////////
        inline void applyUnion(F32 _minX, F32 _minY, F32 _maxX, F32 _maxY)
        {
            m_min.x = Math::Min(m_min.x, _minX);
            m_max.x = Math::Max(m_max.x, _maxX);
            m_min.y = Math::Min(m_min.y, _minY);
            m_max.y = Math::Max(m_max.y, _maxY);
        }
        
        //////////////////////////////////////////
        inline void applyUnion(Vec2DF const& _point)
        {
            m_min.x = Math::Min(m_min.x, _point.x);
            m_max.x = Math::Max(m_max.x, _point.x);
            m_min.y = Math::Min(m_min.y, _point.y);
            m_max.y = Math::Max(m_max.y, _point.y);
        }
        
        //////////////////////////////////////////
        inline AABB2D unionCopy(AABB2D const& _aabb) const
        {
            AABB2D result;
            
            result.m_min.x = Math::Min(m_min.x, _aabb.m_min.x);
            result.m_max.x = Math::Max(m_max.x, _aabb.m_max.x);
            result.m_min.y = Math::Min(m_min.y, _aabb.m_min.y);
            result.m_max.y = Math::Max(m_max.y, _aabb.m_max.y);
            
            return result;
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2DF getSize() const
        {
            return Vec2DF(m_max.x - m_min.x, m_max.y - m_min.y);
        }
        
        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isValid() const
        {
            return (m_min.x < m_max.x) && (m_min.y < m_max.y);
        }
        
        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(m_min.x) + ";" + StringHelper::ToString(m_min.y) + ";" + StringHelper::ToString(m_max.x) + ";" + StringHelper::ToString(m_max.y); }

        //////////////////////////////////////////
        inline static AABB2D FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 4)
            {
                return AABB2D(
                    StringHelper::StringToF32(words[0]),
                    StringHelper::StringToF32(words[1]),
                    StringHelper::StringToF32(words[2]),
                    StringHelper::StringToF32(words[3]));
            }

            return AABB2D::c_zero;
        }
        
    protected:
        Vec2DF m_min;
        Vec2DF m_max;
    };
    
    
    //////////////////////////////////////////
    inline std::ostream& operator<<(std::ostream& _o, AABB2D const& _aabb)
    {
        _o << "AABB2D(" << _aabb.getMin() << ", " << _aabb.getMax() <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(AABB2D const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(AABB2D& _value, CString _data, Size _count)
    {
        _value = AABB2D::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(AABB2D const& _value)
    {
        return sizeof(AABB2D);
    }

    //////////////////////////////////////////
    inline void SerializeValue(AABB2D const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(AABB2D));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(AABB2D& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(AABB2D));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAABB2D_hpp_
//////////////////////////////////////////
