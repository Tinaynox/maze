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
namespace Maze
{


    //////////////////////////////////////////
    // Class Rect2
    //
    //////////////////////////////////////////
    template <class TValue>
    const Rect2<TValue> Rect2<TValue>::c_zero = Rect2<TValue>(0, 0, 0, 0);

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rect2<TValue>::Rect2()
        : position(Vec2<TValue>::c_zero)
        , size(Vec2<TValue>::c_zero)
    {
    
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rect2<TValue>::Rect2(Rect2 const& _rect)
        : position(_rect.position)
        , size(_rect.size)
    {
    
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rect2<TValue>::Rect2(Rect2&& _rect)
        : position(std::move(_rect.position))
        , size(std::move(_rect.size))
    {
    
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rect2<TValue>::Rect2(
        Vec2<TValue> const& _p,
        Vec2<TValue> const& _s)
        : position(_p)
        , size(_s)
    {
    
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rect2<TValue>::Rect2(
        TValue _x,
        TValue _y,
        TValue _w,
        TValue _h)
        : position(Vec2<TValue>(_x, _y))
        , size(Vec2<TValue>(_w, _h))
    { 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Rect2<TValue>& Rect2<TValue>::operator=(Rect2 const& _rect)
    {
        position = _rect.position;
        size = _rect.size;
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Rect2<TValue>& Rect2<TValue>::operator=(Rect2&& _rect)
    {
        position = std::move(_rect.position);
        size = std::move(_rect.size);
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Rect2<TValue>::operator==(Rect2<TValue> const& _rect) const
    {
        return (position == _rect.position && size == _rect.size);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Rect2<TValue>::operator!=(Rect2<TValue> const& _rect) const
    {
        return (position != _rect.position || size != _rect.size);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Rect2<TValue>::applyUnion(Rect2 const& _rect)
    {
        if (_rect.position.x < position.x) position.x = _rect.position.x;
        if (_rect.position.y < position.y) position.y = _rect.position.y;
        if (_rect.position.x + _rect.size.x > position.x + size.x) size.x = _rect.position.x + _rect.size.x - position.x;
        if (_rect.position.y + _rect.size.y > position.y + size.y) size.y = _rect.position.y + _rect.size.y - position.y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Rect2<TValue>::applyIntersection(Rect2 const& _rect)
    {
        if (_rect.position.x > position.x)
        {
            size.x = Math::Max((TValue)0, size.x - (_rect.position.x - position.x));
            position.x = _rect.position.x;
        }

        if (_rect.position.y > position.y)
        {
            size.y = Math::Max((TValue)0, size.y - (_rect.position.y - position.y));
            position.y = _rect.position.y;
        }

        if (_rect.position.x + _rect.size.x < position.x + size.x)
        {
            size.x = Math::Max((TValue)0, _rect.position.x + _rect.size.x - position.x);
        }

        if (_rect.position.y + _rect.size.y < position.y + size.y)
        {
            size.y = Math::Max((TValue)0, _rect.position.y + _rect.size.y - position.y);
        }
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Rect2<TValue>::contains(Vec2<TValue> const& _p)
    {
        if ((_p.x < position.x) ||
            (_p.y < position.y) ||
            (_p.x >= position.x + size.x) ||
            (_p.y >= position.y + size.y))
            return false;

        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Rect2<TValue>::hasCircleIntersection(Vec2<TValue> const& _pos, TValue _radius)
    {
        TValue closestX = Math::Max(getLeft(), Math::Min(_pos.x, getRight()));
        TValue closestY = Math::Max(getTop(), Math::Min(_pos.y, getBottom()));

        TValue distanceX = _pos.x - closestX;
        TValue distanceY = _pos.y - closestY;
        TValue distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        return distanceSquared < (_radius * _radius);
    }


} // namespace Maze
//////////////////////////////////////////
