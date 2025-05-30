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
#pragma warning(push)
#pragma warning(disable: 26495)

    //////////////////////////////////////////
    // Class Vec2
    //
    //////////////////////////////////////////
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_zero = Vec2<TValue>(0, 0);
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_one = Vec2<TValue>(1, 1);
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_negativeOne = Vec2<TValue>(-1, -1);
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_unitX = Vec2<TValue>(1, 0);
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_unitY = Vec2<TValue>(0, 1);
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_negativeUnitX = Vec2<TValue>(-1, 0);
    template <class TValue>
    const Vec2<TValue> Vec2<TValue>::c_negativeUnitY = Vec2<TValue>(0, -1);

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>::Vec2()
    {

    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue>::Vec2(TValue const _x, TValue const _y)
        : x(_x)
        , y(_y)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue>::Vec2(TValue const _x)
        : x(_x)
        , y(_x)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue>::Vec2(TValue const _arr[2])
        : x(_arr[0])
        , y(_arr[1])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue>::Vec2(TValue* const _arr)
        : x(_arr[0])
        , y(_arr[1])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue>::Vec2(Vec2 const& _vec)
        : x(_vec.x)
        , y(_vec.y)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue>::Vec2(Vec2&& _vec) noexcept
        : x(std::move(_vec.x))
        , y(std::move(_vec.y))
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::set(TValue const _x, TValue const _y)
    {
        x = _x;
        y = _y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::set(TValue const _x)
    {
        x = _x;
        y = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::setX(TValue const _x)
    {
        x = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::setY(TValue const _y)
    {
        y = _y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec2<TValue>::getX() const
    {
        return x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec2<TValue>::getY() const
    {
        return y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::swap(Vec2& _other)
    {
        std::swap(x, _other.x);
        std::swap(y, _other.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    MAZE_FORCEINLINE TValue Vec2<TValue>::operator[](Size const _i) const
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    MAZE_FORCEINLINE TValue& Vec2<TValue>::operator[](Size const _i)
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Vec2<TValue>::ptr()
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline const TValue* Vec2<TValue>::ptr() const
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator=(Vec2<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator=(Vec2<TValue>&& _vec) noexcept
    {
        x = std::move(_vec.x);
        y = std::move(_vec.y);
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator=(TValue const _value)
    {
        x = _value;
        y = _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::operator==(Vec2<TValue> const& _vec) const
    {
        return (x == _vec.x && y == _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::operator!=(Vec2<TValue> const& _vec) const
    {
        return (x != _vec.x || y != _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::operator+(Vec2<TValue> const& _vec) const
    {
        return Vec2<TValue>(
            x + _vec.x,
            y + _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::operator-(Vec2<TValue> const& _vec) const
    {
        return Vec2<TValue>(
            x - _vec.x,
            y - _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::operator*(Vec2<TValue> const& _vec) const
    {
        return Vec2<TValue>(
            x * _vec.x,
            y * _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::operator*(TValue const _value) const
    {
        return Vec2<TValue>(
            x * _value,
            y * _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec2<TValue>::operator/(TValue const _value) const
    {
        return Vec2<TValue>(
            x / _value,
            y / _value);
    }

    //////////////////////////////////////////
    template <>
    inline Vec2<F32> Vec2<F32>::operator/(F32 const _value) const
    {
        F32 inv = 1.0f / _value;

        return Vec2(
            x * inv,
            y * inv);
    }

    //////////////////////////////////////////
    template <>
    inline Vec2<F64> Vec2<F64>::operator/(F64 const _value) const
    {
        F64 inv = 1.0 / _value;

        return Vec2(
            x * inv,
            y * inv);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::operator/(Vec2<TValue> const& _vec) const
    {
        return Vec2<TValue>(
            x / _vec.x,
            y / _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> const& Vec2<TValue>::operator+() const
    {
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::operator-() const
    {
        #pragma warning(push)
        #pragma warning(disable : 4146)

        return Vec2<TValue>(-x, -y);

        #pragma warning(pop)
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator+=(Vec2<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator+=(TValue const _value)
    {
        x += _value;
        y += _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator-=(Vec2<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator-=(TValue const _value)
    {
        x -= _value;
        y -= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator*=(TValue const _value)
    {
        x *= _value;
        y *= _value;

        return *this;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator*=(Vec2<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator/=(TValue const _value)
    {
        x /= _value;
        y /= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec2<F32>& Vec2<F32>::operator/=(F32 const _value)
    {
        F32 inv = 1.0f / _value;

        x *= inv;
        y *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec2<F64>& Vec2<F64>::operator/=(F64 const _value)
    {
        F64 inv = 1.0 / _value;

        x *= inv;
        y *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue>& Vec2<TValue>::operator/=(Vec2<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec2<TValue>::length() const
    {
        return Math::Sqrt(x * x + y * y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec2<TValue>::squaredLength() const
    {
        return x * x + y * y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec2<TValue>::distance(Vec2<TValue> const& _vec) const
    {
        return (*this - _vec).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec2<TValue>::squaredDistance(Vec2<TValue> const& _vec) const
    {
        return (*this - _vec).squaredLength();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec2<TValue>::dotProduct(Vec2<TValue> const& _vec) const
    {
        return x * _vec.x + y * _vec.y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::midPoint(Vec2<TValue> const& _vec) const
    {
        return Vec2<TValue>(
            (x + _vec.x) * static_cast<TValue>(0.5),
            (y + _vec.y) * static_cast<TValue>(0.5));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::operator<(Vec2 const& _vec) const
    {
        return (x < _vec.x && y < _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::operator>(Vec2 const& _vec) const
    {
        return (x > _vec.x && y > _vec.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::makeFloor(Vec2 const& _vec)
    {
        if (_vec.x < x) x = _vec.x;
        if (_vec.y < y) y = _vec.y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec2<TValue>::makeCeil(Vec2 const& _vec)
    {
        if (_vec.x > x) x = _vec.x;
        if (_vec.y > y) y = _vec.y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::perpendicular() const
    {
        #pragma warning(push)
        #pragma warning(disable : 4146)

        return Vec2<TValue>(-y, x);

        #pragma warning(pop)
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec2<TValue>::crossProduct(Vec2 const& _vec) const
    {
        return x * _vec.y - y * _vec.x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::isZeroLength() const
    {
        return (((x * x) + (y * y)) < (1e-06 * 1e-06));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Vec2<TValue>::isEqual(Vec2<TValue> const& _vec, TValue _eps) const
    {
        if (   !Math::Equal<TValue>(x, _vec.x, _eps)
            || !Math::Equal<TValue>(y, _vec.y, _eps))
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec2<TValue>::normalizedCopy() const
    {
        Vec2<TValue> ret = *this;
        ret.normalize();
        return ret;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::reflect(Vec2 const& _normal) const
    {
        return Vec2<TValue>(*this - (2 * this->dotProduct(_normal) * _normal));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::isNaN() const
    {
        return Math::IsNaN(x) || Math::IsNaN(y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec2<TValue>::isInfinity() const
    {
        return Math::IsInfinity(x) || Math::IsInfinity(y);
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::FromAngle(F32 _angle)
    {
        return Vec2<TValue>(
            static_cast<TValue>(cosf(_angle)),
            static_cast<TValue>(sinf(_angle)));
    }

    //////////////////////////////////////////
    template <class TValue> 
    inline F32 Vec2<TValue>::toAngle() const
    {
        return (y >= 0    ? acosf((F32)Vec2<TValue>(1, 0).dotProduct(normalizedCopy())) 
                        : -acosf((F32)Vec2<TValue>(1, 0).dotProduct(normalizedCopy())));
    }

    //////////////////////////////////////////
    template <class TValue>
    TValue Vec2<TValue>::normalize()
    {
        TValue length = Sqrt(x * x + y * y);
        
        if (length > 1e-08)
        {
            x /= length;
            y /= length;
        }
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F32 Vec2<F32>::normalize()
    {
        F32 length = Math::Sqrt(x * x + y * y);
        
        if (length > 1e-08)
        {
            F32 invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
        }
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F64 Vec2<F64>::normalize()
    {
        F64 length = Math::Sqrt(x * x + y * y);
        
        if (length > 1e-08)
        {
            F64 invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
        }
        
        return length;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    TValue Vec2<TValue>::normalizeTo(TValue _requiredLength)
    {
        TValue length = Sqrt(x * x + y * y);
        
        if (length > 1e-08)
        {
            x /= length;
            y /= length;                
        }
        x *= _requiredLength;
        y *= _requiredLength;
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F32 Vec2<F32>::normalizeTo(F32 _requiredLength)
    {
        F32 length = Math::Sqrt(x * x + y * y);
        
        F32 invLength = _requiredLength / length;
        x *= invLength;
        y *= invLength;
        
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F64 Vec2<F64>::normalizeTo(F64 _requiredLength)
    {
        F64 length = Math::Sqrt(x * x + y * y);
        
        F64 invLength = _requiredLength / length;
        x *= invLength;
        y *= invLength;
        
        
        return length;
    }
    
    
    //////////////////////////////////////////
    template <class TValue>
    Vec2<TValue> Vec2<TValue>::randomDeviant(F32 _angle) const
    {
        _angle *= Random::UnitRandom() * Math::c_twoPi;
        F32 cosa = cosf(_angle);
        F32 sina = sinf(_angle);
        return  Vec2<TValue>(
        static_cast<TValue>(cosa * x - sina * y),
        static_cast<TValue>(sina * x + cosa * y));
    }
    

    //////////////////////////////////////////
    template <class TValue> 
    Vec2<TValue> Vec2<TValue>::rotatedCopy(F32 _angle) const
    {
        F32 s = sinf(_angle);
        F32 c = cosf(_angle);
        
        return Vec2<TValue>(
        static_cast<TValue>(c * x - s * y),
        static_cast<TValue>(s * x + c * y));
    }
    
    //////////////////////////////////////////
    template <class TValue> 
    MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::rotatedCopy(F32 _s, F32 _c) const
    {
        return Vec2<TValue>(
        static_cast<TValue>(_c * x - _s * y),
        static_cast<TValue>(_s * x + _c * y));
    }
    
    //////////////////////////////////////////
    template <class TValue> 
    void Vec2<TValue>::rotate(F32 _angle)
    {
        (*this) = rotatedCopy(_angle);
    }
    
    //////////////////////////////////////////
    template <class TValue> 
    void Vec2<TValue>::rotate(F32 _s, F32 _c)
    {
        (*this) = rotatedCopy(_s, _c);
    }
    
    
    //////////////////////////////////////////
    template <class TValue> 
    MAZE_CONSTEXPR Vec2<TValue> Vec2<TValue>::RandomDirection()
    {
        return FromAngle(Random::UnitRandom() * Math::c_twoPi);
    }
    
    //////////////////////////////////////////
    template <class TValue> 
    Vec2<TValue> Vec2<TValue>::RandomSemicircleDirection(Vec2<TValue> const& _normal)
    {
        Vec2<TValue> dir = RandomDirection();
        
        if (dir.dotProduct(_normal) < 0)
        dir = -dir;
        
        return dir;
    }
    
    //////////////////////////////////////////
    template <class TValue> 
    inline MAZE_CONSTEXPR F32 Vec2<TValue>::angleBetween(Vec2<TValue> const& _vec) const
    {
        return NormalizedAngle(acosf((F32)dotProduct(_vec) / length() * _vec.length()));
    }


    //////////////////////////////////////////
    template <class TValue>
    inline String Vec2<TValue>::toString(Char _separator) const
    {
        return StringHelper::ToString(x) + _separator + StringHelper::ToString(y);
    }

    //////////////////////////////////////////
    template <class TValue>
    CString Vec2<TValue>::ParseString(CString _string, Size _size, Vec2& _result, Char _separator)
    {
        CString end = _string + _size;

        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.x);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.y);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec2<TValue> Vec2<TValue>::FromString(CString _string, Size _size, Char _separator)
    {
        Vec2 result = Vec2::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec2<TValue>::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }


#pragma warning(pop)

} // namespace Maze
//////////////////////////////////////////
