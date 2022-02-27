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
#include "MazeCoreHeader.hpp"
#include "maze-core/math/MazeQuaternion.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Quaternion
    //
    //////////////////////////////////////////
    F32 const Quaternion::c_epsilon = (F32)1e-03;
    Quaternion const Quaternion::c_zero(0, 0, 0, 0);
    Quaternion const Quaternion::c_identity(1, 0, 0, 0);

    //////////////////////////////////////////
    void Quaternion::setRotationMatrix(Mat3DF const& _rotationMatrix)
    {
        F32 trace = _rotationMatrix[0][0] + _rotationMatrix[1][1] + _rotationMatrix[2][2];
        F32 root;

        // |w| > 1/2, may as well choose w > 1/2
        if (trace > 0.0)
        {
            // 2w
            root = Math::Sqrt(trace + 1.0f);

            w = 0.5f * root;

            // 1/(4w)
            root = 0.5f / root;  

            x = (_rotationMatrix[2][1] - _rotationMatrix[1][2]) * root;
            y = (_rotationMatrix[0][2] - _rotationMatrix[2][0]) * root;
            z = (_rotationMatrix[1][0] - _rotationMatrix[0][1]) * root;
        }
        // |w| <= 1/2
        else
        {
            static Size const c_next[3] = { 1, 2, 0 };
            size_t i = 0;

            if (_rotationMatrix[1][1] > _rotationMatrix[0][0])
                i = 1;

            if (_rotationMatrix[2][2] > _rotationMatrix[i][i])
                i = 2;

            size_t j = c_next[i];
            size_t k = c_next[j];

            root = Math::Sqrt(_rotationMatrix[i][i] - _rotationMatrix[j][j] - _rotationMatrix[k][k] + 1.0f);
            F32* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = 0.5f * root;
            root = 0.5f / root;
            w = (_rotationMatrix[k][j] - _rotationMatrix[j][k]) * root;
            *apkQuat[j] = (_rotationMatrix[j][i] + _rotationMatrix[i][j]) * root;
            *apkQuat[k] = (_rotationMatrix[k][i] + _rotationMatrix[i][k]) * root;
        }
    }

    //////////////////////////////////////////
    void Quaternion::setRotationMatrix(Mat3DF const& _rotationMatrix, bool _multInvScale)
    {
        if (_multInvScale)
        {
            Vec3DF scale = _rotationMatrix.getAffineScale();
            Mat3DF unscaledMat = _rotationMatrix * Mat3DF::CreateScaleMatrix(1.0f / scale);
            setRotationMatrix(unscaledMat);
        }
        else
        {
            setRotationMatrix(_rotationMatrix);
        }
    }
    
    //////////////////////////////////////////
    void Quaternion::toRotationMatrix(Mat3DF& _rotationMatrix) const
    {
        F32 tx = x + x;
        F32 ty = y + y;
        F32 tz = z + z;
        F32 twx = tx * w;
        F32 twy = ty * w;
        F32 twz = tz * w;
        F32 txx = tx * x;
        F32 txy = ty * x;
        F32 txz = tz * x;
        F32 tyy = ty * y;
        F32 tyz = tz * y;
        F32 tzz = tz * z;

        _rotationMatrix[0][0] = 1.0f - (tyy + tzz);
        _rotationMatrix[0][1] = txy - twz;
        _rotationMatrix[0][2] = txz + twy;
        _rotationMatrix[1][0] = txy + twz;
        _rotationMatrix[1][1] = 1.0f - (txx + tzz);
        _rotationMatrix[1][2] = tyz - twx;
        _rotationMatrix[2][0] = txz - twy;
        _rotationMatrix[2][1] = tyz + twx;
        _rotationMatrix[2][2] = 1.0f - (txx + tyy);
    }

    //////////////////////////////////////////
    void Quaternion::toRotationMatrix(Mat4DF& _rotationMatrix) const
    {
        Mat3DF m;
        toRotationMatrix(m);
        _rotationMatrix = m;
        _rotationMatrix[0][3] = 0;
        _rotationMatrix[1][3] = 0;
        _rotationMatrix[2][3] = 0;
        _rotationMatrix[3][0] = 0;
        _rotationMatrix[3][1] = 0;
        _rotationMatrix[3][2] = 0;
        _rotationMatrix[3][3] = 1;
    }
    
    //////////////////////////////////////////
    void Quaternion::setAngleAxis(F32 const& _angle, Vec3DF const& _axis)
    {
        F32 halangle(0.5f * _angle);
        F32 s = Math::Sin(halangle);
        w = Math::Cos(halangle);
        x = s * _axis.x;
        y = s * _axis.y;
        z = s * _axis.z;
    }

    //////////////////////////////////////////
    void Quaternion::toAngleAxis(F32& _angle, Vec3DF& _axis) const
    {
        // The quaternion representing the rotation is
        // q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        F32 sqrLength = x * x + y * y + z * z;
        if (sqrLength > 0.0)
        {
            _angle = 2.0f * Math::ACos(w);
            F32 invLength = Math::InvSqrt(sqrLength);
            _axis.x = x * invLength;
            _axis.y = y * invLength;
            _axis.z = z * invLength;
        }
        else
        {
            // Angle is 0 (mod 2*pi), so any axis will do
            _angle = 0.0;
            _axis.x = 1.0;
            _axis.y = 0.0;
            _axis.z = 0.0;
        }
    }

    //////////////////////////////////////////
    void Quaternion::setAxes(Vec3DF const* _axis)
    {
        Mat3DF rotationMatrix;

        for (Size c = 0; c < 3; c++)
        {
            rotationMatrix[0][c] = _axis[c].x;
            rotationMatrix[1][c] = _axis[c].y;
            rotationMatrix[2][c] = _axis[c].z;
        }

        setRotationMatrix(rotationMatrix);
    }

    //////////////////////////////////////////
    void Quaternion::setAxes(
        Vec3DF const& _xAxis,
        Vec3DF const& _yAxis,
        Vec3DF const& _zAxis)
    {
        Mat3DF rotationMatrix;

        rotationMatrix.setAxes(_xAxis, _yAxis, _zAxis);

        setRotationMatrix(rotationMatrix);
    }

    //////////////////////////////////////////
    void Quaternion::setEulerAngles(
        F32 _xAngle,
        F32 _yAngle,
        F32 _zAngle)
    {
        F32 pitch = _xAngle;
        F32 yaw = _yAngle;
        F32 roll = _zAngle;

        F32 rollOver2 = roll * 0.5f;
        F32 sinRollOver2 = Math::Sin(rollOver2);
        F32 cosRollOver2 = Math::Cos(rollOver2);
        F32 pitchOver2 = pitch * 0.5f;
        F32 sinPitchOver2 = Math::Sin(pitchOver2);
        F32 cosPitchOver2 = Math::Cos(pitchOver2);
        F32 yawOver2 = yaw * 0.5f;
        F32 sinYawOver2 = Math::Sin(yawOver2);
        F32 cosYawOver2 = Math::Cos(yawOver2);
        w = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
        x = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
        y = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
        z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
    }

    //////////////////////////////////////////
    void Quaternion::toAxes(Vec3DF* _axes) const
    {
        Mat3DF rotationMatrix;

        toRotationMatrix(rotationMatrix);

        for (Size c = 0; c < 3; c++)
        {
            _axes[c].x = rotationMatrix[0][c];
            _axes[c].y = rotationMatrix[1][c];
            _axes[c].z = rotationMatrix[2][c];
        }
    }

    //////////////////////////////////////////
    Vec3DF Quaternion::getXAxis() const
    {
        F32 ty = 2.0f * y;
        F32 tz = 2.0f * z;
        F32 twy = ty * w;
        F32 twz = tz * w;
        F32 txy = ty * x;
        F32 txz = tz * x;
        F32 tyy = ty * y;
        F32 tzz = tz * z;

        return Vec3DF(1.0f - (tyy + tzz), txy + twz, txz - twy);
    }
    
    //////////////////////////////////////////
    Vec3DF Quaternion::getYAxis() const
    {
        F32 tx = 2.0f * x;
        F32 ty = 2.0f * y;
        F32 tz = 2.0f * z;
        F32 twx = tx * w;
        F32 twz = tz * w;
        F32 txx = tx * x;
        F32 txy = ty * x;
        F32 tyz = tz * y;
        F32 tzz = tz * z;

        return Vec3DF(txy - twz, 1.0f - (txx + tzz), tyz + twx);
    }
    
    //////////////////////////////////////////
    Vec3DF Quaternion::getZAxis() const
    {
        F32 tx = 2.0f * x;
        F32 ty = 2.0f * y;
        F32 tz = 2.0f * z;
        F32 twx = tx * w;
        F32 twy = ty * w;
        F32 txx = tx * x;
        F32 txz = tz * x;
        F32 tyy = ty * y;
        F32 tyz = tz * y;

        return Vec3DF(txz + twy, tyz - twx, 1.0f - (txx + tyy));
    }

    //////////////////////////////////////////
    void Quaternion::toAxes(
        Vec3DF& _xAxis,
        Vec3DF& _yAxis,
        Vec3DF& _zAxis) const
    {
        Mat3DF rotationMatrix;

        toRotationMatrix(rotationMatrix);

        _xAxis.x = rotationMatrix[0][0];
        _xAxis.y = rotationMatrix[1][0];
        _xAxis.z = rotationMatrix[2][0];

        _yAxis.x = rotationMatrix[0][1];
        _yAxis.y = rotationMatrix[1][1];
        _yAxis.z = rotationMatrix[2][1];

        _zAxis.x = rotationMatrix[0][2];
        _zAxis.y = rotationMatrix[1][2];
        _zAxis.z = rotationMatrix[2][2];
    }

    //////////////////////////////////////////
    Quaternion Quaternion::operator+(Quaternion const& _q) const
    {
        return Quaternion(w + _q.w, x + _q.x, y + _q.y, z + _q.z);
    }

    //////////////////////////////////////////
    Quaternion Quaternion::operator-(Quaternion const& _q) const
    {
        return Quaternion(w - _q.w, x - _q.x, y - _q.y, z - _q.z);
    }
    
    //////////////////////////////////////////
    Quaternion Quaternion::operator*(Quaternion const& _q) const
    {
        // Multiplication is not generally commutative, so in most
        // cases p*q != q*p.

        return Quaternion
        (
            w * _q.w - x * _q.x - y * _q.y - z * _q.z,
            w * _q.x + x * _q.w + y * _q.z - z * _q.y,
            w * _q.y + y * _q.w + z * _q.x - x * _q.z,
            w * _q.z + z * _q.w + x * _q.y - y * _q.x
        );
    }
    
    //////////////////////////////////////////
    Quaternion Quaternion::inversedCopy() const
    {
        F32 norm = w * w + x * x + y * y + z * z;
        if (norm > 0.0)
        {
            F32 invNorm = 1.0f / norm;
            return Quaternion(w * invNorm, -x * invNorm, -y * invNorm, -z * invNorm);
        }
        else
        {
            return c_zero;
        }
    }
    
    //////////////////////////////////////////
    Quaternion Quaternion::inversedUnitCopy() const
    {
        return Quaternion(w, -x, -y, -z);
    }
    
    //////////////////////////////////////////
    Quaternion Quaternion::exp() const
    {
        // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // exp(q) = e^w(cos(A)+sin(A)*(x*i+y*j+z*k)).  If sin(A) is near zero,
        // use exp(q) = e^w(cos(A)+(x*i+y*j+z*k)) since sin(A)/A has limit 1.

        F32 angle(Math::Sqrt(x * x + y * y + z * z));
        F32 s = Math::Sin(angle);
        F32 expW = std::exp(w);

        Quaternion result;
        result.w = expW * Math::Cos(angle);

        if (Math::Abs(angle) >= c_epsilon)
        {
            F32 coeff = expW * (s / (angle));
            result.x = coeff * x;
            result.y = coeff * y;
            result.z = coeff * z;
        }
        else
        {
            result.x = expW * x;
            result.y = expW * y;
            result.z = expW * z;
        }

        return result;
    }

    //////////////////////////////////////////
    Quaternion Quaternion::log() const
    {
        // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // log(q) = (A/sin(A))*(x*i+y*j+z*k).  If sin(A) is near zero, use
        // log(q) = (x*i+y*j+z*k) since A/sin(A) has limit 1.

        Quaternion result;
        result.w = 0.0;

        if (Math::Abs(w) < 1.0)
        {
            F32 normV = Math::Sqrt(x * x + y * y + z * z);
            F32 angle(Math::ATan2(normV, w));

            F32 s = Math::Sin(angle);
            if (Math::Abs(s) >= c_epsilon)
            {
                F32 coeff = angle / s;
                result.x = coeff * x;
                result.y = coeff * y;
                result.z = coeff * z;
                return result;
            }
        }

        result.x = x;
        result.y = y;
        result.z = z;

        return result;
    }

    //////////////////////////////////////////
    Vec3DF Quaternion::operator*(Vec3DF const& _v) const
    {
        Vec3DF uv;
        Vec3DF uuv;
        Vec3DF qvec(x, y, z);
        uv = qvec.crossProduct(_v);
        uuv = qvec.crossProduct(uv);
        uv *= (2.0f * w);
        uuv *= 2.0f;

        return _v + uv + uuv;

    }

    //////////////////////////////////////////
    Quaternion Quaternion::Slerp(
        F32 _t,
        Quaternion const& _p,
        Quaternion const& _q,
        bool _shortestPath)
    {
        F32 c = _p.dot(_q);
        Quaternion r;

        if (c < 0.0f && _shortestPath)
        {
            c = -c;
            r = -_q;
        }
        else
        {
            r = _q;
        }

        if (Math::Abs(c) < 1 - c_epsilon)
        {
            F32 s = Math::Sqrt(1 - Math::Sqr(c));
            F32 angle = Math::ATan2(s, c);
            F32 invSin = 1.0f / s;
            F32 coeff0 = Math::Sin((1.0f - _t) * angle) * invSin;
            F32 coeff1 = Math::Sin(_t * angle) * invSin;
            return coeff0 * _p + coeff1 * r;
        }
        else
        {
            Quaternion t = (1.0f - _t) * _p + _t * r;

            t.normalize();

            return t;
        }
    }
    
    //////////////////////////////////////////
    Quaternion Quaternion::SlerpExtraSpins(
        F32 _t,
        Quaternion const& _p,
        Quaternion const& _q,
        S32 _extraSpins)
    {
        F32 c = _p.dot(_q);
        F32 angle(Math::ACos(c));

        if (Math::Abs(angle) < c_epsilon)
            return _p;

        F32 fSin = Math::Sin(angle);
        F32 fPhase(Math::c_pi * _extraSpins * _t);
        F32 fInvSin = 1.0f / fSin;
        F32 coeff0 = Math::Sin((1.0f - _t) * angle - fPhase) * fInvSin;
        F32 coeff1 = Math::Sin(_t * angle + fPhase) * fInvSin;
        return coeff0 * _p + coeff1 * _q;
    }

    //////////////////////////////////////////
    void Quaternion::Intermediate(
        Quaternion const& _q0,
        Quaternion const& _q1,
        Quaternion const& _q2,
        Quaternion& _a,
        Quaternion& _b)
    {
        Quaternion kQ0inv = _q0.inversedUnitCopy();
        Quaternion kQ1inv = _q1.inversedUnitCopy();
        Quaternion p0 = kQ0inv * _q1;
        Quaternion p1 = kQ1inv * _q2;
        Quaternion arg = 0.25 * (p0.log() - p1.log());
        Quaternion minusArg = -arg;

        _a = _q1 * arg.exp();
        _b = _q1 * minusArg.exp();
    }

    //////////////////////////////////////////
    Quaternion Quaternion::Squad(
        F32 _t,
        Quaternion const& _p,
        Quaternion const& _a,
        Quaternion const& _b,
        Quaternion const& _q,
        bool _shortestPath)
    {
        F32 slerpT = 2.0f * _t * (1.0f - _t);
        Quaternion slerpP = Slerp(_t, _p, _q, _shortestPath);
        Quaternion slerpQ = Slerp(_t, _a, _b);
        return Slerp(slerpT, slerpP, slerpQ);
    }

    //////////////////////////////////////////
    Quaternion Quaternion::Nlerp(
        F32 _t,
        Quaternion const& _p,
        Quaternion const& _q,
        bool _shortestPath)
    {
        Quaternion result;

        F32 c = _p.dot(_q);

        if (c < 0.0f && _shortestPath)
        {
            result = _p + _t * ((-_q) - _p);
        }
        else
        {
            result = _p + _t * (_q - _p);
        }

        result.normalize();

        return result;
    }

    //////////////////////////////////////////
    Vec3DF Quaternion::getEuler() const
    {
        // If the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion not-normalizedness
        F32 unit = (x * x) + (y * y) + (z * z) + (w * w);

        // This will have a magnitude of 0.5 or greater if and only if this is a singularity case
        F32 test = x * w - y * z;

        Vec3DF euler;

        // Singularity at north pole
        if (test > 0.49995f * unit)
        {
            euler.x = Math::c_halfPi;
            euler.y = 2.0f * Math::ATan2(y, x);
            euler.z = 0;
        }
        else
        // Singularity at south pole
        if (test < -0.49995f * unit)
        {
            euler.x = -Math::c_halfPi;
            euler.y = -2.0f * Math::ATan2(y, x);
            euler.z = 0;
        }
        // No singularity - this is the majority of cases
        else
        {
            Quaternion q(y, w, z, x);
            euler.x = Math::ASin(2.0f * (q.x * q.z - q.w * q.y));
            euler.y = Math::ATan2(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1.0f - 2.0f * (q.z * q.z + q.w * q.w));
            euler.z = Math::ATan2(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1.0f - 2.0f * (q.y * q.y + q.z * q.z));
        }
        Math::NormalizeAngle(euler.x);
        Math::NormalizeAngle(euler.y);
        Math::NormalizeAngle(euler.z);

        return euler;
    }

    //////////////////////////////////////////
    Quaternion Quaternion::LookRotation(Vec3DF const& _forward, Vec3DF const& _up)
    {
        Vec3DF vector = _forward.normalizedCopy();
        Vec3DF vector2 = _up.crossProduct(vector).normalizedCopy();
        Vec3DF vector3 = vector.crossProduct(vector2);

        F32 m00 = vector2.x;
        F32 m01 = vector2.y;
        F32 m02 = vector2.z;
        F32 m10 = vector3.x;
        F32 m11 = vector3.y;
        F32 m12 = vector3.z;
        F32 m20 = vector.x;
        F32 m21 = vector.y;
        F32 m22 = vector.z;

        F32 num8 = (m00 + m11) + m22;
        Quaternion quaternion;

        if (num8 > 0.0f)
        {
            F32 num = (F32)Math::Sqrt(num8 + 1.0f);
            quaternion.w = num * 0.5f;
            num = 0.5f / num;
            quaternion.x = (m12 - m21) * num;
            quaternion.y = (m20 - m02) * num;
            quaternion.z = (m01 - m10) * num;

            return quaternion;
        }

        if ((m00 >= m11) && (m00 >= m22))
        {
            F32 num7 = (F32)Math::Sqrt(((1.0f + m00) - m11) - m22);
            F32 num4 = 0.5f / num7;
            quaternion.x = 0.5f * num7;
            quaternion.y = (m01 + m10) * num4;
            quaternion.z = (m02 + m20) * num4;
            quaternion.w = (m12 - m21) * num4;

            return quaternion;
        }

        if (m11 > m22)
        {
            F32 num6 = (F32)Math::Sqrt(((1.0f + m11) - m00) - m22);
            F32 num3 = 0.5f / num6;
            quaternion.x = (m10 + m01) * num3;
            quaternion.y = 0.5f * num6;
            quaternion.z = (m21 + m12) * num3;
            quaternion.w = (m20 - m02) * num3;

            return quaternion;
        }

        F32 num5 = (F32)Math::Sqrt(((1.0f + m22) - m00) - m11);
        F32 num2 = 0.5f / num5;
        quaternion.x = (m20 + m02) * num2;
        quaternion.y = (m21 + m12) * num2;
        quaternion.z = 0.5f * num5;
        quaternion.w = (m01 - m10) * num2;

        return quaternion;
    }


} // namespace Maze
//////////////////////////////////////////
