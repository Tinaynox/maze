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
    // Class Vec4
    //
    //////////////////////////////////////////
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_zero = Vec4<TValue>(0, 0, 0, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_one = Vec4<TValue>(1, 1, 1, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_negativeOne = Vec4<TValue>(-1, -1, -1, -1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_unitX = Vec4<TValue>(1, 0, 0, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_unitY = Vec4<TValue>(0, 1, 0, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_unitZ = Vec4<TValue>(0, 0, 1, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_negativeUnitX = Vec4<TValue>(-1, 0, 0, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_negativeUnitY = Vec4<TValue>(0, -1, 0, 1);
    template <class TValue>
    const Vec4<TValue> Vec4<TValue>::c_negativeUnitZ = Vec4<TValue>(0, 0, -1, 1);

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>::Vec4()
    {

    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue>::Vec4(
        TValue const _x,
        TValue const _y,
        TValue const _z,
        TValue const _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue>::Vec4(TValue const _x)
        : x(_x)
        , y(_x)
        , z(_x)
        , w(_x)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue>::Vec4(TValue const _arr[4])
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
        , w(_arr[3])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue>::Vec4(TValue* const _arr)
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
        , w(_arr[3])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue>::Vec4(Vec4 const& _vec)
        : x(_vec.x)
        , y(_vec.y)
        , z(_vec.z)
        , w(_vec.w)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue>::Vec4(Vec4&& _vec)
        : x(std::move(_vec.x))
        , y(std::move(_vec.y))
        , z(std::move(_vec.z))
        , w(std::move(_vec.w))
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::set(
        TValue const _x,
        TValue const _y,
        TValue const _z,
        TValue const _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::set(TValue const _x)
    {
        x = _x;
        y = _x;
        z = _x;
        w = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::setX(TValue const _x)
    {
        x = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::setY(TValue const _y)
    {
        y = _y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::setZ(TValue const _z)
    {
        z = _z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::setW(TValue const _w)
    {
        w = _w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4<TValue>::getX() const
    {
        return x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4<TValue>::getY() const
    {
        return y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4<TValue>::getZ() const
    {
        return z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4<TValue>::getW() const
    {
        return w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec4<TValue>::xy() const
    {
        return Vec2<TValue>(x, y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Vec4<TValue>::xyz() const
    {
        return Vec3<TValue>(x, y, z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Vec4<TValue>::zw() const
    {
        return Vec2<TValue>(z, w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::swap(Vec4& _other)
    {
        std::swap(x, _other.x);
        std::swap(y, _other.y);
        std::swap(z, _other.z);
        std::swap(w, _other.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4<TValue>::operator[](Size const _i) const
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue& Vec4<TValue>::operator[](Size const _i)
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Vec4<TValue>::ptr()
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline const TValue* Vec4<TValue>::ptr() const
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator=(Vec4<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;
        w = _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator=(Vec4<TValue>&& _vec) noexcept
    {
        x = std::move(_vec.x);
        y = std::move(_vec.y);
        z = std::move(_vec.z);
        w = std::move(_vec.w);
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator=(TValue const _value)
    {
        x = _value;
        y = _value;
        z = _value;
        w = _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator=(Vec2<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = static_cast<TValue>(0);
        w = static_cast<TValue>(1);

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator=(Vec3<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;
        w = static_cast<TValue>(1);

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::operator==(Vec4<TValue> const& _vec) const
    {
        return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::operator!=(Vec4<TValue> const& _vec) const
    {
        return (x != _vec.x || y != _vec.y || z != _vec.z || w != _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::operator+(Vec4<TValue> const& _vec) const
    {
        return Vec4<TValue>(
            x + _vec.x,
            y + _vec.y,
            z + _vec.z,
            w + _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::operator-(Vec4<TValue> const& _vec) const
    {
        return Vec4<TValue>(
            x - _vec.x,
            y - _vec.y,
            z - _vec.z,
            w - _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::operator*(Vec4<TValue> const& _vec) const
    {
        return Vec4<TValue>(
            x * _vec.x,
            y * _vec.y,
            z * _vec.z,
            w * _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::operator*(TValue const _value) const
    {
        return Vec4<TValue>(
            x * _value,
            y * _value,
            z * _value,
            w * _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue> Vec4<TValue>::operator/(TValue const _value) const
    {
        return Vec4<TValue>(
            x / _value,
            y / _value,
            z / _value,
            w / _value);
    }

    //////////////////////////////////////////
    template <>
    inline Vec4<F32> Vec4<F32>::operator/(F32 const _value) const
    {
        F32 inv = 1.0f / _value;

        return Vec4(
            x * inv,
            y * inv,
            z * inv,
            w * inv);
    }

    //////////////////////////////////////////
    template <>
    inline Vec4<F64> Vec4<F64>::operator/(F64 const _value) const
    {
        F64 inv = 1.0 / _value;

        return Vec4(
            x * inv,
            y * inv,
            z * inv,
            w * inv);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::operator/(Vec4<TValue> const& _vec) const
    {
        return Vec4<TValue>(
            x / _vec.x,
            y / _vec.y,
            z / _vec.z,
            w / _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> const& Vec4<TValue>::operator+() const
    {
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::operator-() const
    {
        #pragma warning(push)
        #pragma warning(disable : 4146)

        return Vec4<TValue>(-x, -y, -z, -w);

        #pragma warning(pop)
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator+=(Vec4<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;
        z += _vec.z;
        w += _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator+=(TValue const _value)
    {
        x += _value;
        y += _value;
        z += _value;
        w += _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator-=(Vec4<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;
        z -= _vec.z;
        w -= _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator-=(TValue const _value)
    {
        x -= _value;
        y -= _value;
        z -= _value;
        w -= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator*=(TValue const _value)
    {
        x *= _value;
        y *= _value;
        z *= _value;
        w *= _value;

        return *this;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator*=(Vec4<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;
        z *= _vec.z;
        w *= _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator/=(TValue const _value)
    {
        x /= _value;
        y /= _value;
        z /= _value;
        w /= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec4<F32>& Vec4<F32>::operator/=(F32 const _value)
    {
        F32 inv = 1.0f / _value;

        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec4<F64>& Vec4<F64>::operator/=(F64 const _value)
    {
        F64 inv = 1.0 / _value;

        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue>& Vec4<TValue>::operator/=(Vec4<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;
        z /= _vec.z;
        w /= _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4<TValue>::length() const
    {
        return Math::Sqrt(x * x + y * y + z * z + w * w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4<TValue>::squaredLength() const
    {
        return x * x + y * y + z * z + w * w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4<TValue>::distance(Vec4<TValue> const& _vec) const
    {
        return (*this - _vec).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4<TValue>::squaredDistance(Vec4<TValue> const& _vec) const
    {
        return (*this - _vec).squaredLength();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4<TValue>::dotProduct(Vec4<TValue> const& _vec) const
    {
        return x * _vec.x + y * _vec.y + z * _vec.z + w * _vec.w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4<TValue> Vec4<TValue>::midPoint(Vec4<TValue> const& _vec) const
    {
        return Vec4<TValue>(
            (x + _vec.x) * static_cast<TValue>(0.5),
            (y + _vec.y) * static_cast<TValue>(0.5),
            (z + _vec.z) * static_cast<TValue>(0.5),
            (w + _vec.w) * static_cast<TValue>(0.5));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::operator<(Vec4 const& _vec) const
    {
        return (x < _vec.x && y < _vec.y && z < _vec.z && w < _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::operator>(Vec4 const& _vec) const
    {
        return (x > _vec.x && y > _vec.y && z > _vec.z && w > _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::makeFloor(Vec4 const& _vec)
    {
        if (_vec.x < x) x = _vec.x;
        if (_vec.y < y) y = _vec.y;
        if (_vec.z < z) z = _vec.z;
        if (_vec.w < w) w = _vec.w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4<TValue>::makeCeil(Vec4 const& _vec)
    {
        if (_vec.x > x) x = _vec.x;
        if (_vec.y > y) y = _vec.y;
        if (_vec.z > z) z = _vec.z;
        if (_vec.w > w) w = _vec.w;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::isZeroLength() const
    {
        return (((x * x) + (y * y) + (z * z) + (w * w)) < (1e-06 * 1e-06 * 1e-06 * 1e-06));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Vec4<TValue>::isEqual(Vec4<TValue> const& _vec, TValue _eps) const
    {
        if (   !Math::Equal<TValue>(x, _vec.x, _eps)
            || !Math::Equal<TValue>(y, _vec.y, _eps)
            || !Math::Equal<TValue>(z, _vec.z, _eps)
            || !Math::Equal<TValue>(w, _vec.w, _eps))
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::isNaN() const
    {
        return IsNaN(x) || IsNaN(y) || IsNaN(z) || IsNaN(w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4<TValue>::isInfinity() const
    {
        return IsInfinity(x) || IsInfinity(y) || IsInfinity(z) || IsInfinity(w);
    }


    //////////////////////////////////////////
    template <class TValue>
    inline String Vec4<TValue>::toString(Char _separator) const
    {
        return StringHelper::ToString(x) + _separator + StringHelper::ToString(y) + _separator + StringHelper::ToString(z) + _separator + StringHelper::ToString(w);
    }

    //////////////////////////////////////////
    template <class TValue>
    CString Vec4<TValue>::ParseString(CString _string, Size _size, Vec4& _result, Char _separator)
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
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.w);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec4<TValue> Vec4<TValue>::FromString(CString _string, Size _size, Char _separator)
    {
        Vec4 result = Vec3<TValue>::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    Vec4<TValue> Vec4<TValue>::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }

#pragma warning(pop)

} // namespace Maze
//////////////////////////////////////////
