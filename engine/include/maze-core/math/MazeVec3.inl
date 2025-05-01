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
    // Class Vec3
    //
    //////////////////////////////////////////
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_zero = Vec3<TValue>(0, 0, 0);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_one = Vec3<TValue>(1, 1, 1);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_negativeOne = Vec3<TValue>(-1, -1, -1);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_unitX = Vec3<TValue>(1, 0, 0);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_unitY = Vec3<TValue>(0, 1, 0);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_unitZ = Vec3<TValue>(0, 0, 1);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_negativeUnitX = Vec3<TValue>(-1, 0, 0);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_negativeUnitY = Vec3<TValue>(0, -1, 0);
    template <class TValue>
    const Vec3<TValue> Vec3<TValue>::c_negativeUnitZ = Vec3<TValue>(0, 0, -1);

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>::Vec3()
    {

    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue>::Vec3(
        TValue const _x,
        TValue const _y,
        TValue const _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue>::Vec3(TValue const _x)
        : x(_x)
        , y(_x)
        , z(_x)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue>::Vec3(TValue const _arr[3])
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue>::Vec3(TValue* const _arr)
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue>::Vec3(Vec3 const& _vec)
        : x(_vec.x)
        , y(_vec.y)
        , z(_vec.z)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue>::Vec3(Vec3&& _vec) noexcept
        : x(std::move(_vec.x))
        , y(std::move(_vec.y))
        , z(std::move(_vec.z))
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::set(
        TValue const _x,
        TValue const _y,
        TValue const _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::set(TValue const _x)
    {
        x = _x;
        y = _x;
        z = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::setX(TValue const _x)
    {
        x = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::setY(TValue const _y)
    {
        y = _y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::setZ(TValue const _z)
    {
        z = _z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3<TValue>::getX() const
    {
        return x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3<TValue>::getY() const
    {
        return y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3<TValue>::getZ() const
    {
        return z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec3<TValue>::xy() const
    {
        return Vec2<TValue>(x, y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec3<TValue>::xz() const
    {
        return Vec2<TValue>(x, z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::swap(Vec3& _other)
    {
        std::swap(x, _other.x);
        std::swap(y, _other.y);
        std::swap(z, _other.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    MAZE_FORCEINLINE TValue Vec3<TValue>::operator[](Size const _i) const
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    MAZE_FORCEINLINE TValue& Vec3<TValue>::operator[](Size const _i)
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Vec3<TValue>::ptr()
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline const TValue* Vec3<TValue>::ptr() const
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator=(Vec3<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator=(Vec3<TValue>&& _vec) noexcept
    {
        x = std::move(_vec.x);
        y = std::move(_vec.y);
        z = std::move(_vec.z);
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator=(TValue const _value)
    {
        x = _value;
        y = _value;
        z = _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator=(Vec2<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = static_cast<TValue>(0);

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::operator==(Vec3<TValue> const& _vec) const
    {
        return (x == _vec.x && y == _vec.y && z == _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::operator!=(Vec3<TValue> const& _vec) const
    {
        return (x != _vec.x || y != _vec.y || z != _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator+(Vec3<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            x + _vec.x,
            y + _vec.y,
            z + _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator-(Vec3<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            x - _vec.x,
            y - _vec.y,
            z - _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator*(Vec3<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            x * _vec.x,
            y * _vec.y,
            z * _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator*(Vec2<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            x * _vec.x,
            y * _vec.y,
            z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator*(TValue const _value) const
    {
        return Vec3<TValue>(
            x * _value,
            y * _value,
            z * _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Vec3<TValue>::operator/(TValue const _value) const
    {
        return Vec3<TValue>(
            x / _value,
            y / _value,
            z / _value);
    }

    //////////////////////////////////////////
    template <>
    inline Vec3<F32> Vec3<F32>::operator/(F32 const _value) const
    {
        F32 inv = 1.0f / _value;

        return Vec3(
            x * inv,
            y * inv,
            z * inv);
    }

    //////////////////////////////////////////
    template <>
    inline Vec3<F64> Vec3<F64>::operator/(F64 const _value) const
    {
        F64 inv = 1.0 / _value;

        return Vec3(
            x * inv,
            y * inv,
            z * inv);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator/(Vec3<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            x / _vec.x,
            y / _vec.y,
            z / _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator/(Vec2<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            x / _vec.x,
            y / _vec.y,
            z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> const& Vec3<TValue>::operator+() const
    {
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::operator-() const
    {
    #pragma warning(push)
    #pragma warning(disable : 4146)

        return Vec3<TValue>(-x, -y, -z);

    #pragma warning(pop)
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator+=(Vec3<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;
        z += _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator+=(Vec2<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator+=(TValue const _value)
    {
        x += _value;
        y += _value;
        z += _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator-=(Vec3<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;
        z -= _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator-=(Vec2<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;
        
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator-=(TValue const _value)
    {
        x -= _value;
        y -= _value;
        z -= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator*=(TValue const _value)
    {
        x *= _value;
        y *= _value;
        z *= _value;

        return *this;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator*=(Vec3<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;
        z *= _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator*=(Vec2<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator/=(TValue const _value)
    {
        x /= _value;
        y /= _value;
        z /= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec3<F32>& Vec3<F32>::operator/=(F32 const _value)
    {
        F32 inv = 1.0f / _value;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec3<F64>& Vec3<F64>::operator/=(F64 const _value)
    {
        F64 inv = 1.0 / _value;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator/=(Vec3<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;
        z /= _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue>& Vec3<TValue>::operator/=(Vec2<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;
        
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3<TValue>::length() const
    {
        return Math::Sqrt(x * x + y * y + z * z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3<TValue>::squaredLength() const
    {
        return x * x + y * y + z * z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3<TValue>::distance(Vec3<TValue> const& _vec) const
    {
        return (*this - _vec).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3<TValue>::squaredDistance(Vec3<TValue> const& _vec) const
    {
        return (*this - _vec).squaredLength();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3<TValue>::dotProduct(Vec3<TValue> const& _vec) const
    {
        return x * _vec.x + y * _vec.y + z * _vec.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::midPoint(Vec3<TValue> const& _vec) const
    {
        return Vec3<TValue>(
            (x + _vec.x) * static_cast<TValue>(0.5),
            (y + _vec.y) * static_cast<TValue>(0.5),
            (z + _vec.z) * static_cast<TValue>(0.5));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::operator<(Vec3 const& _vec) const
    {
        return (x < _vec.x && y < _vec.y && z < _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::operator>(Vec3 const& _vec) const
    {
        return (x > _vec.x && y > _vec.y && z > _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::makeFloor(Vec3 const& _vec)
    {
        if (_vec.x < x) x = _vec.x;
        if (_vec.y < y) y = _vec.y;
        if (_vec.z < z) z = _vec.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3<TValue>::makeCeil(Vec3 const& _vec)
    {
        if (_vec.x > x) x = _vec.x;
        if (_vec.y > y) y = _vec.y;
        if (_vec.z > z) z = _vec.z;
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::crossProduct(Vec3 const& _vec) const
    {
        return Vec3<TValue>(
            y * _vec.z - z * _vec.y,
            z * _vec.x - x * _vec.z,
            x * _vec.y - y * _vec.x);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::isZeroLength() const
    {
        return (((x * x) + (y * y) + (z * z)) < (1e-06 * 1e-06 * 1e-06));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Vec3<TValue>::isEqual(Vec3<TValue> const& _vec, TValue _eps) const
    {
        if (   !Math::Equal<TValue>(x, _vec.x, _eps)
            || !Math::Equal<TValue>(y, _vec.y, _eps)
            || !Math::Equal<TValue>(z, _vec.z, _eps))
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Vec3<TValue>::normalizedCopy() const
    {
        Vec3<TValue> ret = *this;
        ret.normalize();
        return ret;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Vec3<TValue>::reflect(Vec3 const& _normal) const
    {
        return Vec3<TValue>(*this - (2 * this->dotProduct(_normal) * _normal));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::isNaN() const
    {
        return IsNaN(x) || IsNaN(y) || IsNaN(z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3<TValue>::isInfinity() const
    {
        return IsInfinity(x) || IsInfinity(y) || IsInfinity(z);
    }

    
    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue> Vec3<TValue>::perpendicular() const
    {
        TValue length = Math::Sqrt(x * x + y * y + z * z);
        Vec3<TValue> norm = (length > 1e-08) ? *this / length : *this;

        TValue const epsilon = 0.5f;
        if (Math::Abs(norm.z) < epsilon)
            return Vec3(-y, x, z).crossProduct(*this).normalizedCopy() * length;
        else
        if (Math::Abs(norm.y) < epsilon)
            return Vec3(-z, y, x).crossProduct(*this).normalizedCopy() * length;
        else
            return Vec3(x, -z, y).crossProduct(*this).normalizedCopy() * length;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    TValue Vec3<TValue>::normalize()
    {
        TValue length = Sqrt(x * x + y * y + z * z);
        
        if (length > 1e-08)
        {
            x /= length;
            y /= length;
            z /= length;
        }
        
        return length;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue>& Vec3<TValue>::rotate(Vec3<TValue> const& _axis, F32 _theta)
    {
        F32 cosTheta = cos(_theta);
        F32 sinTheta = sin(_theta);

        return this->operator=((*this * cosTheta) + (_axis.crossProduct(*this) * sinTheta) + (_axis * _axis.dotProduct(*this)) * (1.0f - cosTheta));
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue> Vec3<TValue>::rotatedCopy(Vec3<TValue> const& _axis, F32 _theta)
    {
        Vec3<TValue> copy = *this;
        copy.rotate(_axis, _theta);
        return copy;
    }
    
    //////////////////////////////////////////
    template <>
    inline F32 Vec3<F32>::normalize()
    {
        F32 length = Math::Sqrt(x * x + y * y + z * z);
        
        if (length > 1e-08)
        {
            F32 invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F64 Vec3<F64>::normalize()
    {
        F64 length = Math::Sqrt(x * x + y * y + z * z);
        
        if (length > 1e-08)
        {
            F64 invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
        
        return length;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    TValue Vec3<TValue>::normalizeTo(TValue _requiredLength)
    {
        TValue length = Sqrt(x * x + y * y + z * z);
        
        if (length > 1e-08)
        {
            x /= length;
            y /= length;
            z /= length;
        }
        x *= _requiredLength;
        y *= _requiredLength;
        z *= _requiredLength;
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F32 Vec3<F32>::normalizeTo(F32 _requiredLength)
    {
        F32 length = Math::Sqrt(x * x + y * y + z * z);
        
        F32 invLength = _requiredLength / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        
        return length;
    }
    
    //////////////////////////////////////////
    template <>
    inline F64 Vec3<F64>::normalizeTo(F64 _requiredLength)
    {
        F64 length = Math::Sqrt(x * x + y * y + z * z);
        
        F64 invLength = _requiredLength / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        
        
        return length;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue> Vec3<TValue>::RandomDirection()
    {
        F32 phi = Random::RangeRandom<F32>(0.0f, Math::c_twoPi);
        F32 cosTheta = Random::RangeRandom<F32>(-1.0f, 1.0f);
        F32 theta = Math::ACos(cosTheta);
        
        Vec3<TValue> result;
        result.x = static_cast<TValue>(sinf(theta) * cosf(phi));
        result.y = static_cast<TValue>(sinf(theta) * sinf(phi));
        result.z = static_cast<TValue>(cosf(theta));
        return result;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue> Vec3<TValue>::RandomHemisphereDirection(Vec3<TValue> const& _normal)
    {
        Vec3<TValue> dir = RandomDirection();
        
        if (dir.dotProduct(_normal) < 0)
            dir = -dir;
        
        return dir;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline String Vec3<TValue>::toString(Char _separator) const
    {
        return StringHelper::ToString(x) + _separator + StringHelper::ToString(y) + _separator + StringHelper::ToString(z);
    }

    //////////////////////////////////////////
    template <class TValue>
    CString Vec3<TValue>::ParseString(CString _string, Size _size, Vec3& _result, Char _separator)
    {
        CString end = _string + _size;

        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.x);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.y);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.z);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue> Vec3<TValue>::FromString(CString _string, Size _size, Char _separator)
    {
        Vec3 result = Vec3::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec3<TValue> Vec3<TValue>::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }

#pragma warning(pop)

} // namespace Maze
//////////////////////////////////////////
