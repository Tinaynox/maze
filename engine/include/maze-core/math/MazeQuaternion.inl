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
#include "MazeQuaternion.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Quaternion
    //
    //////////////////////////////////////////
    inline Quaternion::Quaternion()
        : w(1)
        , x(0)
        , y(0)
        , z(0)
    {
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(Quaternion const& rhs)
        : w(rhs.w)
        , x(rhs.x)
        , y(rhs.y)
        , z(rhs.z)
    {}

    //////////////////////////////////////////
    inline Quaternion::Quaternion(F32 _w, F32 _x, F32 _y, F32 _z)
        : w(_w)
        , x(_x)
        , y(_y)
        , z(_z)
    {
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(Vec4F const& _vec)
        : w(_vec.w)
        , x(_vec.x)
        , y(_vec.y)
        , z(_vec.z)
    {     
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(Mat3F const& _rotationMatrix)
    {
        setRotationMatrix(_rotationMatrix);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(Mat4F const& _rotationMatrix)
    {
        setRotationMatrix(_rotationMatrix);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(TMat const& _rotationMatrix)
    {
        setRotationMatrix(_rotationMatrix);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(F32 const& _angle, Vec3F const& _axis)
    {
        setAngleAxis(_angle, _axis);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(
        Vec3F const& _xAxis,
        Vec3F const& _yAxis,
        Vec3F const& _zAxis)
    {
        setAxes(_xAxis, _yAxis, _zAxis);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(Vec3F const* _axes)
    {
        setAxes(_axes);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(F32* _value)
        : Quaternion()
    {
        memcpy(&w, _value, sizeof(F32) * 4);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(
        F32 _xAngle,
        F32 _yAngle,
        F32 _zAngle)
    {
        setEulerAngles(_xAngle, _yAngle, _zAngle);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(Vec3F const& _eulerAngles)
    {
        setEulerAngles(_eulerAngles.x, _eulerAngles.y, _eulerAngles.z);
    }

    //////////////////////////////////////////
    inline Quaternion::Quaternion(
        Vec3F const& _fromVector,
        Vec3F const& _toVector)
    {
        F32 d = _fromVector.dotProduct(_toVector);

        if (d > 0.999999 || d < -0.999999)
        {
            *this = c_identity;
        }
        else
        {
            Vec3F a = _fromVector.crossProduct(_toVector);
            x = a.x;
            y = a.y;
            z = a.z;
            w = sqrt((_fromVector.squaredLength()) * (_toVector.squaredLength())) + d;
            normalize();
        }
    }

    //////////////////////////////////////////
    inline void Quaternion::swap(Quaternion& _q)
    {
        std::swap(w, _q.w);
        std::swap(x, _q.x);
        std::swap(y, _q.y);
        std::swap(z, _q.z);
    }

    //////////////////////////////////////////
    inline F32 Quaternion::operator[](Size const _i) const
    {
        MAZE_DEBUG_ERROR_IF(_i >= 4, "Index is out of bounds!");

        return *(&w + _i);
    }

    //////////////////////////////////////////
    inline F32& Quaternion::operator[](Size const _i)
    {
        MAZE_DEBUG_ERROR_IF(_i >= 4, "Index is out of bounds!");

        return *(&w + _i);
    }

    //////////////////////////////////////////
    inline F32* Quaternion::ptr()
    {
        return &w;
    }

    //////////////////////////////////////////
    inline F32 const* Quaternion::ptr() const
    {
        return &w;
    }

    //////////////////////////////////////////
    inline Quaternion& Quaternion::operator=(Quaternion const& _q)
    {
        w = _q.w;
        x = _q.x;
        y = _q.y;
        z = _q.z;
        return *this;
    }

    //////////////////////////////////////////
    inline Quaternion Quaternion::operator*(F32 _s) const
    {
        return Quaternion(_s * w, _s * x, _s * y, _s * z);
    }

    //////////////////////////////////////////
    inline Quaternion Quaternion::operator-() const
    { 
        return Quaternion(-w, -x, -y, -z); 
    }

    //////////////////////////////////////////
    inline bool Quaternion::operator==(Quaternion const& _q) const
    {
        return (_q.x == x) && (_q.y == y) && (_q.z == z) && (_q.w == w);
    }

    //////////////////////////////////////////
    inline bool Quaternion::operator!=(Quaternion const& _q) const
    {
        return !operator==(_q);
    }

    //////////////////////////////////////////
    inline F32 Quaternion::dot(Quaternion const& _q) const
    {
        return w * _q.w + x * _q.x + y * _q.y + z * _q.z;
    }

    //////////////////////////////////////////
    inline F32 Quaternion::norm() const
    {
        return Math::Sqrt(w * w + x * x + y * y + z * z);
    }

    //////////////////////////////////////////
    inline F32 Quaternion::normalize()
    {
        F32 l = norm();
        (*this) = 1.0f / l * (*this);
        return l;
    }

    //////////////////////////////////////////
    inline bool Quaternion::equals(Quaternion const& _q, F32 const& _tolerance) const
    {
        F32 d = dot(_q);
        F32 angle = Math::ACos(2.0f * d * d - 1.0f);

        return Math::Abs(angle) <= _tolerance;
    }

    //////////////////////////////////////////
    inline bool Quaternion::orientationEquals(Quaternion const& _q, F32 _tolerance) const
    {
        F32 d = dot(_q);
        return 1 - d * d < _tolerance;
    }

    //////////////////////////////////////////
    inline Quaternion Quaternion::FromEuler(F32 _x, F32 _y, F32 _z)
    {
        return Quaternion(Vec3F(_x, _y, _z));
    }


} // namespace Maze
//////////////////////////////////////////
