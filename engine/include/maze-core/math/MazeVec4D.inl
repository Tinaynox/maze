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
    // Class Vec4D
    //
    //////////////////////////////////////////
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_zero = Vec4D<TValue>(0, 0, 0, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_one = Vec4D<TValue>(1, 1, 1, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_negativeOne = Vec4D<TValue>(-1, -1, -1, -1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_unitX = Vec4D<TValue>(1, 0, 0, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_unitY = Vec4D<TValue>(0, 1, 0, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_unitZ = Vec4D<TValue>(0, 0, 1, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_negativeUnitX = Vec4D<TValue>(-1, 0, 0, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_negativeUnitY = Vec4D<TValue>(0, -1, 0, 1);
    template <class TValue>
    const Vec4D<TValue> Vec4D<TValue>::c_negativeUnitZ = Vec4D<TValue>(0, 0, -1, 1);

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>::Vec4D()
    {

    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue>::Vec4D(
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
    inline MAZE_CONSTEXPR Vec4D<TValue>::Vec4D(TValue const _x)
        : x(_x)
        , y(_x)
        , z(_x)
        , w(_x)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue>::Vec4D(TValue const _arr[4])
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
        , w(_arr[3])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue>::Vec4D(TValue* const _arr)
        : x(_arr[0])
        , y(_arr[1])
        , z(_arr[2])
        , w(_arr[3])
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue>::Vec4D(Vec4D const& _vec)
        : x(_vec.x)
        , y(_vec.y)
        , z(_vec.z)
        , w(_vec.w)
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue>::Vec4D(Vec4D&& _vec)
        : x(std::move(_vec.x))
        , y(std::move(_vec.y))
        , z(std::move(_vec.z))
        , w(std::move(_vec.w))
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::set(
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
    inline void Vec4D<TValue>::set(TValue const _x)
    {
        x = _x;
        y = _x;
        z = _x;
        w = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::setX(TValue const _x)
    {
        x = _x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::setY(TValue const _y)
    {
        y = _y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::setZ(TValue const _z)
    {
        z = _z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::setW(TValue const _w)
    {
        w = _w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4D<TValue>::getX() const
    {
        return x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4D<TValue>::getY() const
    {
        return y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4D<TValue>::getZ() const
    {
        return z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4D<TValue>::getW() const
    {
        return w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2D<TValue> Vec4D<TValue>::xy() const
    {
        return Vec2D<TValue>(x, y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue> Vec4D<TValue>::xyz() const
    {
        return Vec3D<TValue>(x, y, z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2D<TValue> Vec4D<TValue>::zw() const
    {
        return Vec2D<TValue>(z, w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::swap(Vec4D& _other)
    {
        std::swap(x, _other.x);
        std::swap(y, _other.y);
        std::swap(z, _other.z);
        std::swap(w, _other.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue Vec4D<TValue>::operator[](Size const _i) const
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue& Vec4D<TValue>::operator[](Size const _i)
    {
        return *(&x + _i);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Vec4D<TValue>::ptr()
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline const TValue* Vec4D<TValue>::ptr() const
    {
        return &x;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator=(Vec4D<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;
        w = _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator=(Vec4D<TValue>&& _vec) noexcept
    {
        x = std::move(_vec.x);
        y = std::move(_vec.y);
        z = std::move(_vec.z);
        w = std::move(_vec.w);
            
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator=(TValue const _value)
    {
        x = _value;
        y = _value;
        z = _value;
        w = _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator=(Vec2D<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = static_cast<TValue>(0);
        w = static_cast<TValue>(1);

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator=(Vec3D<TValue> const& _vec)
    {
        x = _vec.x;
        y = _vec.y;
        z = _vec.z;
        w = static_cast<TValue>(1);

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4D<TValue>::operator==(Vec4D<TValue> const& _vec) const
    {
        return (x == _vec.x && y == _vec.y && z == _vec.z && w == _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4D<TValue>::operator!=(Vec4D<TValue> const& _vec) const
    {
        return (x != _vec.x || y != _vec.y || z != _vec.z || w != _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::operator+(Vec4D<TValue> const& _vec) const
    {
        return Vec4D<TValue>(
            x + _vec.x,
            y + _vec.y,
            z + _vec.z,
            w + _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::operator-(Vec4D<TValue> const& _vec) const
    {
        return Vec4D<TValue>(
            x - _vec.x,
            y - _vec.y,
            z - _vec.z,
            w - _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::operator*(Vec4D<TValue> const& _vec) const
    {
        return Vec4D<TValue>(
            x * _vec.x,
            y * _vec.y,
            z * _vec.z,
            w * _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::operator*(TValue const _value) const
    {
        return Vec4D<TValue>(
            x * _value,
            y * _value,
            z * _value,
            w * _value);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue> Vec4D<TValue>::operator/(TValue const _value) const
    {
        return Vec4D<TValue>(
            x / _value,
            y / _value,
            z / _value,
            w / _value);
    }

    //////////////////////////////////////////
    template <>
    inline Vec4D<F32> Vec4D<F32>::operator/(F32 const _value) const
    {
        F32 inv = 1.0f / _value;

        return Vec4D(
            x * inv,
            y * inv,
            z * inv,
            w * inv);
    }

    //////////////////////////////////////////
    template <>
    inline Vec4D<F64> Vec4D<F64>::operator/(F64 const _value) const
    {
        F64 inv = 1.0 / _value;

        return Vec4D(
            x * inv,
            y * inv,
            z * inv,
            w * inv);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::operator/(Vec4D<TValue> const& _vec) const
    {
        return Vec4D<TValue>(
            x / _vec.x,
            y / _vec.y,
            z / _vec.z,
            w / _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> const& Vec4D<TValue>::operator+() const
    {
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::operator-() const
    {
        #pragma warning(push)
        #pragma warning(disable : 4146)

        return Vec4D<TValue>(-x, -y, -z, -w);

        #pragma warning(pop)
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator+=(Vec4D<TValue> const& _vec)
    {
        x += _vec.x;
        y += _vec.y;
        z += _vec.z;
        w += _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator+=(TValue const _value)
    {
        x += _value;
        y += _value;
        z += _value;
        w += _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator-=(Vec4D<TValue> const& _vec)
    {
        x -= _vec.x;
        y -= _vec.y;
        z -= _vec.z;
        w -= _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator-=(TValue const _value)
    {
        x -= _value;
        y -= _value;
        z -= _value;
        w -= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator*=(TValue const _value)
    {
        x *= _value;
        y *= _value;
        z *= _value;
        w *= _value;

        return *this;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator*=(Vec4D<TValue> const& _vec)
    {
        x *= _vec.x;
        y *= _vec.y;
        z *= _vec.z;
        w *= _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4D<TValue>& Vec4D<TValue>::operator/=(TValue const _value)
    {
        x /= _value;
        y /= _value;
        z /= _value;
        w /= _value;

        return *this;
    }

    //////////////////////////////////////////
    template <>
    inline Vec4D<F32>& Vec4D<F32>::operator/=(F32 const _value)
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
    inline Vec4D<F64>& Vec4D<F64>::operator/=(F64 const _value)
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
    inline Vec4D<TValue>& Vec4D<TValue>::operator/=(Vec4D<TValue> const& _vec)
    {
        x /= _vec.x;
        y /= _vec.y;
        z /= _vec.z;
        w /= _vec.w;

        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4D<TValue>::length() const
    {
        return Sqrt(x * x + y * y + z * z + w * w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4D<TValue>::squaredLength() const
    {
        return x * x + y * y + z * z + w * w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4D<TValue>::distance(Vec4D<TValue> const& _vec) const
    {
        return (*this - _vec).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4D<TValue>::squaredDistance(Vec4D<TValue> const& _vec) const
    {
        return (*this - _vec).squaredLength();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Vec4D<TValue>::dotProduct(Vec4D<TValue> const& _vec) const
    {
        return x * _vec.x + y * _vec.y + z * _vec.z + w * _vec.w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec4D<TValue> Vec4D<TValue>::midPoint(Vec4D<TValue> const& _vec) const
    {
        return Vec4D<TValue>(
            (x + _vec.x) * static_cast<TValue>(0.5),
            (y + _vec.y) * static_cast<TValue>(0.5),
            (z + _vec.z) * static_cast<TValue>(0.5),
            (w + _vec.w) * static_cast<TValue>(0.5));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4D<TValue>::operator<(Vec4D const& _vec) const
    {
        return (x < _vec.x && y < _vec.y && z < _vec.z && w < _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4D<TValue>::operator>(Vec4D const& _vec) const
    {
        return (x > _vec.x && y > _vec.y && z > _vec.z && w > _vec.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::makeFloor(Vec4D const& _vec)
    {
        if (_vec.x < x) x = _vec.x;
        if (_vec.y < y) y = _vec.y;
        if (_vec.z < z) z = _vec.z;
        if (_vec.w < w) w = _vec.w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Vec4D<TValue>::makeCeil(Vec4D const& _vec)
    {
        if (_vec.x > x) x = _vec.x;
        if (_vec.y > y) y = _vec.y;
        if (_vec.z > z) z = _vec.z;
        if (_vec.w > w) w = _vec.w;
    }


    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Vec4D<TValue>::isZeroLength() const
    {
        return (((x * x) + (y * y) + (z * z) + (w * w)) < (1e-06 * 1e-06 * 1e-06 * 1e-06));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Vec4D<TValue>::isEqual(Vec4D<TValue> const& _vec, TValue _eps) const
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
    inline MAZE_CONSTEXPR bool Vec4D<TValue>::isNaN() const
    {
        return IsNaN(x) || IsNaN(y) || IsNaN(z) || IsNaN(w);
    }

#pragma warning(pop)

} // namespace Maze
//////////////////////////////////////////
