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
    // Class Vec3D
    //
    //////////////////////////////////////////
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_zero = Vec3D<TValue>(0, 0, 0);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_one = Vec3D<TValue>(1, 1, 1);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_negativeOne = Vec3D<TValue>(-1, -1, -1);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_unitX = Vec3D<TValue>(1, 0, 0);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_unitY = Vec3D<TValue>(0, 1, 0);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_unitZ = Vec3D<TValue>(0, 0, 1);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_negativeUnitX = Vec3D<TValue>(-1, 0, 0);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_negativeUnitY = Vec3D<TValue>(0, -1, 0);
    template <class TValue>
    const Vec3D<TValue> Vec3D<TValue>::c_negativeUnitZ = Vec3D<TValue>(0, 0, -1);

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>::Vec3D()
    {

    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue>::Vec3D(
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
    inline MAZE_CONSTEXPR Vec3D<TValue>::Vec3D(TValue const _x)
        : x(_x)
        , y(_x)
        , z(_x)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue>::Vec3D(TValue const _arr[3])
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue>::Vec3D(TValue* const _arr)
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue>::Vec3D(Vec3D const& _vec)
        : x(_vec.x)
        , y(_vec.y)
        , z(_vec.z)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue>::Vec3D(Vec3D&& _vec) noexcept
        : x(std::move(_vec.x))
        , y(std::move(_vec.y))
        , z(std::move(_vec.z))
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::set(
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
    inline void Vec3D<TValue>::set(TValue const _x)
    {
        x = _x;
        y = _x;
        z = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::setX(TValue const _x)
    {
        x = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::setY(TValue const _y)
    {
        y = _y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::setZ(TValue const _z)
    {
        z = _z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3D<TValue>::getX() const
    {
        return x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3D<TValue>::getY() const
    {
        return y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3D<TValue>::getZ() const
    {
        return z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2D<TValue> Vec3D<TValue>::xy() const
    {
        return Vec2D<TValue>(x, y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2D<TValue> Vec3D<TValue>::xz() const
    {
        return Vec2D<TValue>(x, z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::swap(Vec3D& _other)
    {
        std::swap(x, _other.x);
        std::swap(y, _other.y);
        std::swap(z, _other.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec3D<TValue>::operator[](Size const _i) const
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue& Vec3D<TValue>::operator[](Size const _i)
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Vec3D<TValue>::ptr()
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline const TValue* Vec3D<TValue>::ptr() const
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator=(Vec3D<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator=(Vec3D<TValue>&& _vec) noexcept
    {
        x = std::move(_vec.x);
        y = std::move(_vec.y);
        z = std::move(_vec.z);
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator=(TValue const _value)
    {
        x = _value;
        y = _value;
        z = _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator=(Vec2D<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = static_cast<TValue>(0);

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::operator==(Vec3D<TValue> const& _vec) const
    {
        return (x == _vec.x && y == _vec.y && z == _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::operator!=(Vec3D<TValue> const& _vec) const
    {
        return (x != _vec.x || y != _vec.y || z != _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator+(Vec3D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            x + _vec.x,
            y + _vec.y,
            z + _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator-(Vec3D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            x - _vec.x,
            y - _vec.y,
            z - _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator*(Vec3D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            x * _vec.x,
            y * _vec.y,
            z * _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator*(Vec2D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            x * _vec.x,
            y * _vec.y,
            z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator*(TValue const _value) const
    {
        return Vec3D<TValue>(
            x * _value,
            y * _value,
            z * _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue> Vec3D<TValue>::operator/(TValue const _value) const
    {
        return Vec3D<TValue>(
            x / _value,
            y / _value,
            z / _value);
    }

    //////////////////////////////////////////
    template <>
    inline Vec3D<F32> Vec3D<F32>::operator/(F32 const _value) const
    {
        F32 inv = 1.0f / _value;

        return Vec3D(
            x * inv,
            y * inv,
            z * inv);
    }

    //////////////////////////////////////////
    template <>
    inline Vec3D<F64> Vec3D<F64>::operator/(F64 const _value) const
    {
        F64 inv = 1.0 / _value;

        return Vec3D(
            x * inv,
            y * inv,
            z * inv);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator/(Vec3D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            x / _vec.x,
            y / _vec.y,
            z / _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator/(Vec2D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            x / _vec.x,
            y / _vec.y,
            z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> const& Vec3D<TValue>::operator+() const
    {
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::operator-() const
    {
    #pragma warning(push)
    #pragma warning(disable : 4146)

        return Vec3D<TValue>(-x, -y, -z);

    #pragma warning(pop)
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator+=(Vec3D<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;
        z += _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator+=(Vec2D<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator+=(TValue const _value)
    {
        x += _value;
        y += _value;
        z += _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator-=(Vec3D<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;
        z -= _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator-=(Vec2D<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;
        
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator-=(TValue const _value)
    {
        x -= _value;
        y -= _value;
        z -= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator*=(TValue const _value)
    {
        x *= _value;
        y *= _value;
        z *= _value;

        return *this;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator*=(Vec3D<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;
        z *= _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator*=(Vec2D<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator/=(TValue const _value)
    {
        x /= _value;
        y /= _value;
        z /= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec3D<F32>& Vec3D<F32>::operator/=(F32 const _value)
    {
        F32 inv = 1.0f / _value;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec3D<F64>& Vec3D<F64>::operator/=(F64 const _value)
    {
        F64 inv = 1.0 / _value;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator/=(Vec3D<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;
        z /= _vec.z;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue>& Vec3D<TValue>::operator/=(Vec2D<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;
        
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3D<TValue>::length() const
    {
        return Math::Sqrt(x * x + y * y + z * z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3D<TValue>::squaredLength() const
    {
        return x * x + y * y + z * z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3D<TValue>::distance(Vec3D<TValue> const& _vec) const
    {
        return (*this - _vec).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3D<TValue>::squaredDistance(Vec3D<TValue> const& _vec) const
    {
        return (*this - _vec).squaredLength();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec3D<TValue>::dotProduct(Vec3D<TValue> const& _vec) const
    {
        return x * _vec.x + y * _vec.y + z * _vec.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::midPoint(Vec3D<TValue> const& _vec) const
    {
        return Vec3D<TValue>(
            (x + _vec.x) * static_cast<TValue>(0.5),
            (y + _vec.y) * static_cast<TValue>(0.5),
            (z + _vec.z) * static_cast<TValue>(0.5));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::operator<(Vec3D const& _vec) const
    {
        return (x < _vec.x && y < _vec.y && z < _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::operator>(Vec3D const& _vec) const
    {
        return (x > _vec.x && y > _vec.y && z > _vec.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::makeFloor(Vec3D const& _vec)
    {
        if (_vec.x < x) x = _vec.x;
        if (_vec.y < y) y = _vec.y;
        if (_vec.z < z) z = _vec.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec3D<TValue>::makeCeil(Vec3D const& _vec)
    {
        if (_vec.x > x) x = _vec.x;
        if (_vec.y > y) y = _vec.y;
        if (_vec.z > z) z = _vec.z;
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::crossProduct(Vec3D const& _vec) const
    {
        return Vec3D<TValue>(
            y * _vec.z - z * _vec.y,
            z * _vec.x - x * _vec.z,
            x * _vec.y - y * _vec.x);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::isZeroLength() const
    {
        return (((x * x) + (y * y) + (z * z)) < (1e-06 * 1e-06 * 1e-06));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Vec3D<TValue>::isEqual(Vec3D<TValue> const& _vec, TValue _eps) const
    {
        if (   !Math::Equal<TValue>(x, _vec.x, _eps)
            || !Math::Equal<TValue>(y, _vec.y, _eps)
            || !Math::Equal<TValue>(z, _vec.z, _eps))
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue> Vec3D<TValue>::normalizedCopy() const
    {
        Vec3D<TValue> ret = *this;
        ret.normalize();
        return ret;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3D<TValue> Vec3D<TValue>::reflect(Vec3D const& _normal) const
    {
        return Vec3D<TValue>(*this - (2 * this->dotProduct(_normal) * _normal));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::isNaN() const
    {
        return IsNaN(x) || IsNaN(y) || IsNaN(z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec3D<TValue>::isInfinity() const
    {
        return IsInfinity(x) || IsInfinity(y) || IsInfinity(z);
    }

    
    //////////////////////////////////////////
    template <class TValue>
    Vec3D<TValue> Vec3D<TValue>::perpendicular() const
    {
        TValue length = Math::Sqrt(x * x + y * y + z * z);
        Vec3D<TValue> norm = (length > 1e-08) ? *this / length : *this;

        TValue const epsilon = 0.5f;
        if (Math::Abs(norm.z) < epsilon)
            return Vec3D(-y, x, z).crossProduct(*this).normalizedCopy() * length;
        else
        if (Math::Abs(norm.y) < epsilon)
            return Vec3D(-z, y, x).crossProduct(*this).normalizedCopy() * length;
        else
            return Vec3D(x, -z, y).crossProduct(*this).normalizedCopy() * length;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    TValue Vec3D<TValue>::normalize()
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
    Vec3D<TValue>& Vec3D<TValue>::rotate(Vec3D<TValue> const& _axis, F32 _theta)
    {
        F32 cosTheta = cos(_theta);
        F32 sinTheta = sin(_theta);

        return this->operator=((*this * cosTheta) + (_axis.crossProduct(*this) * sinTheta) + (_axis * _axis.dotProduct(*this)) * (1.0f - cosTheta));
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec3D<TValue> Vec3D<TValue>::rotatedCopy(Vec3D<TValue> const& _axis, F32 _theta)
    {
        Vec3D<TValue> copy = *this;
        copy.rotate(_axis, _theta);
        return copy;
    }
    
    //////////////////////////////////////////
    template <>
    inline F32 Vec3D<F32>::normalize()
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
    inline F64 Vec3D<F64>::normalize()
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
    TValue Vec3D<TValue>::normalizeTo(TValue _requiredLength)
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
    inline F32 Vec3D<F32>::normalizeTo(F32 _requiredLength)
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
    inline F64 Vec3D<F64>::normalizeTo(F64 _requiredLength)
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
    Vec3D<TValue> Vec3D<TValue>::RandomDirection()
    {
        F32 phi = Random::RangeRandom<F32>(0.0f, Math::c_twoPi);
        F32 cosTheta = Random::RangeRandom<F32>(-1.0f, 1.0f);
        F32 theta = Math::ACos(cosTheta);
        
        Vec3D<TValue> result;
        result.x = static_cast<TValue>(sinf(theta) * cosf(phi));
        result.y = static_cast<TValue>(sinf(theta) * sinf(phi));
        result.z = static_cast<TValue>(cosf(theta));
        return result;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Vec3D<TValue> Vec3D<TValue>::RandomHemisphereDirection(Vec3D<TValue> const& _normal)
    {
        Vec3D<TValue> dir = RandomDirection();
        
        if (dir.dotProduct(_normal) < 0)
            dir = -dir;
        
        return dir;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline String Vec3D<TValue>::toString(Char _separator) const
    {
        return StringHelper::ToString(x) + _separator + StringHelper::ToString(y) + _separator + StringHelper::ToString(z);
    }

    //////////////////////////////////////////
    template <class TValue>
    CString Vec3D<TValue>::ParseString(CString _string, Size _size, Vec3D& _result, Char _separator)
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
    Vec3D<TValue> Vec3D<TValue>::FromString(CString _string, Size _size, Char _separator)
    {
        Vec3D result = Vec3D::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec3D<TValue> Vec3D<TValue>::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }

#pragma warning(pop)

} // namespace Maze
//////////////////////////////////////////
