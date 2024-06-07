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
    // Class Mat4
    //
    // [ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]
    // | m[1][0]  m[1][1]  m[1][2]  m[1][3] |
    // | m[2][0]  m[2][1]  m[2][2]  m[2][3] |
    // [ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]
    //
    // Affine scheme:
    // [ RX RY RZ RW ]
    // | UX UY UZ UW |
    // | FX FY FZ FW |
    // [ TX TY TZ TW ]
    // R - right, U - up, F - front, T - translation
    //
    //////////////////////////////////////////
    template <class TValue>
    const TValue Mat4<TValue>::c_epsilon = (TValue)1e-05;
    template <class TValue>
    const Mat4<TValue> Mat4<TValue>::c_zero(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);
    template <class TValue>
    const Mat4<TValue> Mat4<TValue>::c_identity(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue>::Mat4()
        : planeMatrix{ 0 }
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue>::Mat4(TValue const _arr[4][4])
    {
        memcpy(m, _arr, 16 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue>::Mat4(Mat4 const& _matrix)
    {
        memcpy(m, _matrix.m, 16 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue>::Mat4(Mat3<TValue> const& _mat3)
        : planeMatrix{    
            _mat3.m[0][0],    _mat3.m[0][1],    _mat3.m[0][2],    0,
            _mat3.m[1][0],    _mat3.m[1][1],    _mat3.m[1][2],    0,
            _mat3.m[2][0],    _mat3.m[2][1],    _mat3.m[2][2],    0,
            0,                0,                0,                1 }
    {
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue>::Mat4(    
        TValue _value00, TValue _value01, TValue _value02, TValue _value03,
        TValue _value10, TValue _value11, TValue _value12, TValue _value13,
        TValue _value20, TValue _value21, TValue _value22, TValue _value23,
        TValue _value30, TValue _value31, TValue _value32, TValue _value33)
    {
        set(_value00,  _value01,  _value02,  _value03,
            _value10,  _value11,  _value12,  _value13,
            _value20,  _value21,  _value22,  _value23,
            _value30,  _value31,  _value32,  _value33);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::set(    
        TValue _value00, TValue _value01, TValue _value02, TValue _value03,
        TValue _value10, TValue _value11, TValue _value12, TValue _value13,
        TValue _value20, TValue _value21, TValue _value22, TValue _value23,
        TValue _value30, TValue _value31, TValue _value32, TValue _value33)
    {
        m[0][0] = _value00; m[0][1] = _value01; m[0][2] = _value02; m[0][3] = _value03;
        m[1][0] = _value10; m[1][1] = _value11; m[1][2] = _value12; m[1][3] = _value13;
        m[2][0] = _value20; m[2][1] = _value21; m[2][2] = _value22; m[2][3] = _value23;
        m[3][0] = _value30; m[3][1] = _value31; m[3][2] = _value32; m[3][3] = _value33;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateTranslationMatrix(TValue _x, TValue _y, TValue _z)
    {
        return Mat4(
            1,  0,  0,  0,
            0,  1,  0,  0,
            0,  0,  1,  0,
            _x, _y, _z, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateBasisMatrix(
        Vec3<TValue> const& _xBasis,
        Vec3<TValue> const& _yBasis,
        Vec3<TValue> const& _zBasis)
    {
        return Mat4(
            _xBasis.x, _xBasis.y, _xBasis.z, 0,
            _yBasis.x, _yBasis.y, _yBasis.z, 0,
            _zBasis.x, _zBasis.y, _zBasis.z, 0,
            0,         0,         0,         1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateTranslationMatrix(Vec2<TValue> const& _vector)
    {
        return CreateTranslationMatrix(_vector.x, _vector.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateTranslationMatrix(Vec3<TValue> const& _vector)
    {
        return CreateTranslationMatrix(_vector.x, _vector.y, _vector.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateRotationZMatrix(Rotation2D const& _rotation)
    {
        return Mat4(
            (TValue)(_rotation.getCos()),  (TValue)(_rotation.getSin()), (TValue)0, (TValue)0,
            (TValue)(-_rotation.getSin()), (TValue)(_rotation.getCos()), (TValue)0, (TValue)0,
            (TValue)0,                     (TValue)0,                    (TValue)1, (TValue)0,
            (TValue)0,                     (TValue)0,                    (TValue)0, (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateRotationXMatrix(F32 _angle)
    {
        // CCW rotation around X axis 
        F32 c = cosf(_angle);
        F32 s = sinf(_angle);
        return Mat4(
            (TValue)1, (TValue)0,  (TValue)0, (TValue)0,
            (TValue)0, (TValue)c,  (TValue)s, (TValue)0,
            (TValue)0, (TValue)-s, (TValue)c, (TValue)0,
            (TValue)0, (TValue)0,  (TValue)0, (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateRotationYMatrix(F32 _angle)
    {
        // CCW rotation around Y axis 
        F32 c = cosf(_angle);
        F32 s = sinf(_angle);
        return Mat4(
            (TValue)c, (TValue)0, (TValue)-s, (TValue)0,
            (TValue)0, (TValue)1, (TValue)0,  (TValue)0,
            (TValue)s, (TValue)0, (TValue)c,  (TValue)0,
            (TValue)0, (TValue)0, (TValue)0,  (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateRotationZMatrix(F32 _angle)
    {
        // CCW rotation around Z axis 
        F32 c = cosf(_angle);
        F32 s = sinf(_angle);
        return Mat4(
            (TValue)c, (TValue)s, (TValue)0, (TValue)0,
            (TValue)-s, (TValue)c,  (TValue)0, (TValue)0,
            (TValue)0, (TValue)0,  (TValue)1, (TValue)0,
            (TValue)0, (TValue)0,  (TValue)0, (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateRotationMatrix(F32 _angleX, F32 _angleY, F32 _angleZ)
    {
        // CCW rotation
        F32 cx = cosf(_angleX);
        F32 sx = sinf(_angleX);

        F32 cy = cosf(_angleY);
        F32 sy = sinf(_angleY);

        F32 cz = cosf(_angleZ);
        F32 sz = sinf(_angleZ);

        return 
            Mat4(
                (TValue)cy, (TValue)0, (TValue)-sy, (TValue)0,
                (TValue)0, (TValue)1, (TValue)0, (TValue)0,
                (TValue)sy, (TValue)0, (TValue)cy, (TValue)0,
                (TValue)0, (TValue)0, (TValue)0, (TValue)1).concatenatedAffineCopy(
                Mat4(
                    (TValue)1, (TValue)0, (TValue)0, (TValue)0,
                    (TValue)0, (TValue)cx, (TValue)sx, (TValue)0,
                    (TValue)0, (TValue)-sx, (TValue)cx, (TValue)0,
                    (TValue)0, (TValue)0, (TValue)0, (TValue)1)).concatenatedAffineCopy(
                    Mat4(
                        (TValue)cz, (TValue)sz, (TValue)0, (TValue)0,
                        (TValue)-sz, (TValue)cz, (TValue)0, (TValue)0,
                        (TValue)0, (TValue)0, (TValue)1, (TValue)0,
                        (TValue)0, (TValue)0, (TValue)0, (TValue)1));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateScaleMatrix(TValue _x, TValue _y, TValue _z)
    {
        return Mat4(
            _x, 0, 0, 0,
            0, _y, 0, 0,
            0, 0, _z, 0,
            0, 0, 0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateScaleMatrix(Vec2<TValue> const& _vector)
    {
        return CreateScaleMatrix(_vector.x, _vector.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateScaleMatrix(Vec3<TValue> const& _vector)
    {
        return CreateScaleMatrix(_vector.x, _vector.y, _vector.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::CreateAffineTansformMatrix(
        Vec2<TValue> const& _pos,
        Rotation2D& _rotation,
        Vec2<TValue> const& _scale)
    {
        return Mat4(
            (TValue)(_rotation.getCos() * _scale.x),  (TValue)(_rotation.getSin() * _scale.y), 0, 0,
            (TValue)(-_rotation.getSin() * _scale.x), (TValue)(_rotation.getCos() * _scale.y), 0, 0,
            0,                                        0,                                       1, 0,
            _pos.x,                                   _pos.y,                                  0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateAffineTransformMatrix(
        Vec2<TValue> const& _pos,
        Rotation2D& _rotation,
        Vec2<TValue> const& _scale,
        Vec2<TValue> const& _pivot,
        Vec2<TValue> const& _size)
    {
        TValue sax = _size.x * _pivot.x;
        TValue say = _size.y * _pivot.y;

        TValue x = (TValue)(-_rotation.getCos() * _scale.x * sax + _rotation.getSin() * _scale.y * say + _pos.x);
        TValue y = (TValue)(-_rotation.getSin() * _scale.x * sax - _rotation.getCos() * _scale.y * say + _pos.y);

        return Mat4(
            (TValue)(_rotation.getCos() * _scale.x),  (TValue)(_rotation.getSin() * _scale.x), 0, 0,
            (TValue)(-_rotation.getSin() * _scale.y), (TValue)(_rotation.getCos() * _scale.y), 0, 0,
            0,                                        0,                                       1, 0,
            x,                                        y,                                       0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateProjectionPerspectiveRHMatrix(
        F32 _fieldOfViewY,
        F32 _aspectRatio,
        F32 _nearZ,
        F32 _farZ)
    {
        F32 tanHalfFieldOfView = tanf(_fieldOfViewY * 0.5f);
        F32 invTanHalfFieldOfView = 1.0f / tanHalfFieldOfView;

        F32 invFD = 1.0f / (_farZ - _nearZ);

        TValue s0 = invTanHalfFieldOfView / _aspectRatio;
        TValue s1 = invTanHalfFieldOfView;
        TValue z0 = -(_farZ + _nearZ) * invFD;
        TValue z1 = -(2.0f * _farZ * _nearZ) * invFD;

        return Mat4(
            s0, 0, 0, 0,
            0, s1, 0, 0,
            0, 0, z0, -1,
            0, 0, z1, 0);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateProjectionPerspectiveLHMatrix(
        F32 _fieldOfViewY,
        F32 _aspectRatio,
        F32 _nearZ,
        F32 _farZ)
    {
        F32 tanHalfFieldOfView = tanf(_fieldOfViewY * 0.5f);
        F32 invTanHalfFieldOfView = 1.0f / tanHalfFieldOfView;

        F32 invFD = 1.0f / (_farZ - _nearZ);

        TValue s0 = invTanHalfFieldOfView / _aspectRatio;
        TValue s1 = invTanHalfFieldOfView;
        TValue z0 = -(_farZ + _nearZ) * invFD;
        TValue z1 = (2.0f * _farZ * _nearZ) * invFD;

        return Mat4(
            s0, 0, 0, 0,
            0, s1, 0, 0,
            0, 0, z0, 1,
            0, 0, z1, 0);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateProjection2DMatrix(
        F32 _x,
        F32 _y,
        F32 _w,
        F32 _h)
    {
        F32 left = _x;
        F32 bottom = _y;

        F32 right = left + _w;
        F32 top = bottom + _h;

        F32 s0 = 2.0f / _w;
        F32 s1 = 2.0f / _h;

        F32 tx = (left + right) / (left - right);
        F32 ty = (bottom + top) / (bottom - top);

        return Mat4(
            s0, 0, 0, 0,
            0, s1, 0, 0,
            0,  0, -1.0f, 0.0f,
            tx, ty, 0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::CreateLookAtMatrix(
        Vec3<TValue> const& _from,
        Vec3<TValue> const& _to,
        Vec3<TValue> const& _up)
    {
        Vec3<TValue> forward = (_from - _to).normalizedCopy();
        Vec3<TValue> right = _up.crossProduct(forward);
        Vec3<TValue> up = forward.crossProduct(right);

        Mat4 m;
        m.setRow(0, Vec4<TValue>(right, 0));
        m.setRow(1, Vec4<TValue>(up, 0));
        m.setRow(2, Vec4<TValue>(forward, 0));
        m.setRow(3, Vec4<TValue>(_from, 1));

        return m;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::swap(Mat4<TValue>& _other)
    {
        std::swap(m[0][0], _other.m[0][0]); std::swap(m[0][1], _other.m[0][1]); std::swap(m[0][2], _other.m[0][2]); std::swap(m[0][3], _other.m[0][3]);
        std::swap(m[1][0], _other.m[1][0]); std::swap(m[1][1], _other.m[1][1]); std::swap(m[1][2], _other.m[1][2]); std::swap(m[1][3], _other.m[1][3]);
        std::swap(m[2][0], _other.m[2][0]); std::swap(m[2][1], _other.m[2][1]); std::swap(m[2][2], _other.m[2][2]); std::swap(m[2][3], _other.m[2][3]);
        std::swap(m[3][0], _other.m[3][0]); std::swap(m[3][1], _other.m[3][1]); std::swap(m[3][2], _other.m[3][2]); std::swap(m[3][3], _other.m[3][3]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue> Mat4<TValue>::getRow(Size _row) const
    {
        return *(Vec4<TValue>*)m[_row];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::setRow(Size _row, Vec4<TValue> const& _vec)
    {
        *(Vec4<TValue>*)m[_row] = _vec;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Mat4<TValue>::operator[](Size _row) const
    {
        return (TValue*)m[_row];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue> Mat4<TValue>::getColumn(Size _column) const
    {
        return Vec4<TValue>(
            m[0][_column],
            m[1][_column],
            m[2][_column],
            m[3][_column]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::setColumn(Size _column, Vec4<TValue> const& _vec)
    {
        m[0][_column] = _vec.x;
        m[1][_column] = _vec.y;
        m[2][_column] = _vec.z;
        m[3][_column] = _vec.w;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue>& Mat4<TValue>::operator=(Mat4 const& _matrix)
    {
        memcpy(m, _matrix.m, 16 * sizeof(TValue));
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat4<TValue>::operator==(Mat4 const& _m2) const
    {
        if (
            m[0][0] != _m2.m[0][0] || m[0][1] != _m2.m[0][1] || m[0][2] != _m2.m[0][2] || m[0][3] != _m2.m[0][3] ||
            m[1][0] != _m2.m[1][0] || m[1][1] != _m2.m[1][1] || m[1][2] != _m2.m[1][2] || m[1][3] != _m2.m[1][3] ||
            m[2][0] != _m2.m[2][0] || m[2][1] != _m2.m[2][1] || m[2][2] != _m2.m[2][2] || m[2][3] != _m2.m[2][3] ||
            m[3][0] != _m2.m[3][0] || m[3][1] != _m2.m[3][1] || m[3][2] != _m2.m[3][2] || m[3][3] != _m2.m[3][3])
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat4<TValue>::operator!=(Mat4 const& _m2) const
    {
        if (
            m[0][0] != _m2.m[0][0] || m[0][1] != _m2.m[0][1] || m[0][2] != _m2.m[0][2] || m[0][3] != _m2.m[0][3] ||
            m[1][0] != _m2.m[1][0] || m[1][1] != _m2.m[1][1] || m[1][2] != _m2.m[1][2] || m[1][3] != _m2.m[1][3] ||
            m[2][0] != _m2.m[2][0] || m[2][1] != _m2.m[2][1] || m[2][2] != _m2.m[2][2] || m[2][3] != _m2.m[2][3] ||
            m[3][0] != _m2.m[3][0] || m[3][1] != _m2.m[3][1] || m[3][2] != _m2.m[3][2] || m[3][3] != _m2.m[3][3])
            return true;
        return false;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::concatenate(Mat4 const& m2)
    {
        (*this) = concatenatedCopy(m2);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::concatenatedCopy(Mat4 const& _m2) const
    {
        Mat4 r;
        r.m[0][0] = m[0][0] * _m2.m[0][0] + m[0][1] * _m2.m[1][0] + m[0][2] * _m2.m[2][0] + m[0][3] * _m2.m[3][0];
        r.m[0][1] = m[0][0] * _m2.m[0][1] + m[0][1] * _m2.m[1][1] + m[0][2] * _m2.m[2][1] + m[0][3] * _m2.m[3][1];
        r.m[0][2] = m[0][0] * _m2.m[0][2] + m[0][1] * _m2.m[1][2] + m[0][2] * _m2.m[2][2] + m[0][3] * _m2.m[3][2];
        r.m[0][3] = m[0][0] * _m2.m[0][3] + m[0][1] * _m2.m[1][3] + m[0][2] * _m2.m[2][3] + m[0][3] * _m2.m[3][3];

        r.m[1][0] = m[1][0] * _m2.m[0][0] + m[1][1] * _m2.m[1][0] + m[1][2] * _m2.m[2][0] + m[1][3] * _m2.m[3][0];
        r.m[1][1] = m[1][0] * _m2.m[0][1] + m[1][1] * _m2.m[1][1] + m[1][2] * _m2.m[2][1] + m[1][3] * _m2.m[3][1];
        r.m[1][2] = m[1][0] * _m2.m[0][2] + m[1][1] * _m2.m[1][2] + m[1][2] * _m2.m[2][2] + m[1][3] * _m2.m[3][2];
        r.m[1][3] = m[1][0] * _m2.m[0][3] + m[1][1] * _m2.m[1][3] + m[1][2] * _m2.m[2][3] + m[1][3] * _m2.m[3][3];

        r.m[2][0] = m[2][0] * _m2.m[0][0] + m[2][1] * _m2.m[1][0] + m[2][2] * _m2.m[2][0] + m[2][3] * _m2.m[3][0];
        r.m[2][1] = m[2][0] * _m2.m[0][1] + m[2][1] * _m2.m[1][1] + m[2][2] * _m2.m[2][1] + m[2][3] * _m2.m[3][1];
        r.m[2][2] = m[2][0] * _m2.m[0][2] + m[2][1] * _m2.m[1][2] + m[2][2] * _m2.m[2][2] + m[2][3] * _m2.m[3][2];
        r.m[2][3] = m[2][0] * _m2.m[0][3] + m[2][1] * _m2.m[1][3] + m[2][2] * _m2.m[2][3] + m[2][3] * _m2.m[3][3];

        r.m[3][0] = m[3][0] * _m2.m[0][0] + m[3][1] * _m2.m[1][0] + m[3][2] * _m2.m[2][0] + m[3][3] * _m2.m[3][0];
        r.m[3][1] = m[3][0] * _m2.m[0][1] + m[3][1] * _m2.m[1][1] + m[3][2] * _m2.m[2][1] + m[3][3] * _m2.m[3][1];
        r.m[3][2] = m[3][0] * _m2.m[0][2] + m[3][1] * _m2.m[1][2] + m[3][2] * _m2.m[2][2] + m[3][3] * _m2.m[3][2];
        r.m[3][3] = m[3][0] * _m2.m[0][3] + m[3][1] * _m2.m[1][3] + m[3][2] * _m2.m[2][3] + m[3][3] * _m2.m[3][3];

        return r;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::operator*(Mat4 const& _m2) const
    {
        return concatenatedCopy(_m2);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Mat4<TValue>::operator*(Vec3<TValue> const& _v) const
    {
        Vec3<TValue> r;

        F32 invW = 1.0f / (m[0][3] * _v.x + m[1][3] * _v.y + m[2][3] * _v.z + m[3][3]);

        r.x = TValue((m[0][0] * _v.x + m[1][0] * _v.y + m[2][0] * _v.z + m[3][0]) * invW);
        r.y = TValue((m[0][1] * _v.x + m[1][1] * _v.y + m[2][1] * _v.z + m[3][1]) * invW);
        r.z = TValue((m[0][2] * _v.x + m[1][2] * _v.y + m[2][2] * _v.z + m[3][2]) * invW);

        return r;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue> Mat4<TValue>::operator*(Vec4<TValue> const& _v) const
    {
        return Vec4<TValue>(
            m[0][0] * _v.x + m[0][1] * _v.y + m[0][2] * _v.z + m[0][3] * _v.w, 
            m[1][0] * _v.x + m[1][1] * _v.y + m[1][2] * _v.z + m[1][3] * _v.w,
            m[2][0] * _v.x + m[2][1] * _v.y + m[2][2] * _v.z + m[2][3] * _v.w,
            m[3][0] * _v.x + m[3][1] * _v.y + m[3][2] * _v.z + m[3][3] * _v.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::operator+(Mat4 const& _m2) const
    {
        Mat4 r;

        r.m[0][0] = m[0][0] + _m2.m[0][0];
        r.m[0][1] = m[0][1] + _m2.m[0][1];
        r.m[0][2] = m[0][2] + _m2.m[0][2];
        r.m[0][3] = m[0][3] + _m2.m[0][3];

        r.m[1][0] = m[1][0] + _m2.m[1][0];
        r.m[1][1] = m[1][1] + _m2.m[1][1];
        r.m[1][2] = m[1][2] + _m2.m[1][2];
        r.m[1][3] = m[1][3] + _m2.m[1][3];

        r.m[2][0] = m[2][0] + _m2.m[2][0];
        r.m[2][1] = m[2][1] + _m2.m[2][1];
        r.m[2][2] = m[2][2] + _m2.m[2][2];
        r.m[2][3] = m[2][3] + _m2.m[2][3];

        r.m[3][0] = m[3][0] + _m2.m[3][0];
        r.m[3][1] = m[3][1] + _m2.m[3][1];
        r.m[3][2] = m[3][2] + _m2.m[3][2];
        r.m[3][3] = m[3][3] + _m2.m[3][3];

        return r;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::operator-(Mat4 const& _m2) const
    {
        Mat4 r;

        r.m[0][0] = m[0][0] - _m2.m[0][0];
        r.m[0][1] = m[0][1] - _m2.m[0][1];
        r.m[0][2] = m[0][2] - _m2.m[0][2];
        r.m[0][3] = m[0][3] - _m2.m[0][3];

        r.m[1][0] = m[1][0] - _m2.m[1][0];
        r.m[1][1] = m[1][1] - _m2.m[1][1];
        r.m[1][2] = m[1][2] - _m2.m[1][2];
        r.m[1][3] = m[1][3] - _m2.m[1][3];

        r.m[2][0] = m[2][0] - _m2.m[2][0];
        r.m[2][1] = m[2][1] - _m2.m[2][1];
        r.m[2][2] = m[2][2] - _m2.m[2][2];
        r.m[2][3] = m[2][3] - _m2.m[2][3];

        r.m[3][0] = m[3][0] - _m2.m[3][0];
        r.m[3][1] = m[3][1] - _m2.m[3][1];
        r.m[3][2] = m[3][2] - _m2.m[3][2];
        r.m[3][3] = m[3][3] - _m2.m[3][3];

        return r;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::operator=(Mat3<TValue> const& _mat3)
    {
        m[0][0] = _mat3.m[0][0]; m[0][1] = _mat3.m[0][1]; m[0][2] = _mat3.m[0][2];
        m[1][0] = _mat3.m[1][0]; m[1][1] = _mat3.m[1][1]; m[1][2] = _mat3.m[1][2];
        m[2][0] = _mat3.m[2][0]; m[2][1] = _mat3.m[2][1]; m[2][2] = _mat3.m[2][2];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::transpose() const
    {
        return Mat4(
            m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::setTranslation(Vec2<TValue> const& _v)
    {
        m[3][0] = _v.x;
        m[3][1] = _v.y;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::setTranslation(Vec3<TValue> const& _v)
    {
        m[3][0] = _v.x;
        m[3][1] = _v.y;
        m[3][2] = _v.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Mat4<TValue>::getAffineTranslation() const
    {
        return Vec3<TValue>(
            m[3][0],
            m[3][1],
            m[3][2]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Mat4<TValue>::getAffineTranslation2D() const
    {
        return Vec2<TValue>(
            m[3][0],
            m[3][1]);
    }

    //////////////////////////////////////////
    /*
    template <class TValue>
    inline MAZE_CONSTEXPR14 Vec3<TValue> Mat4<TValue>::getAffineRotationEulerAngles() const
    {
        Vec3<TValue> invAffineScale = 1.0f / getAffineScale();

        Vec3<TValue> euler;
        TValue m11 = this->m[0][0] * invAffineScale.x;
        TValue m12 = this->m[0][1] * invAffineScale.y;
        TValue m13 = this->m[0][2] * invAffineScale.z;
        TValue m21 = this->m[1][0] * invAffineScale.x;
        TValue m31 = this->m[2][0] * invAffineScale.x;
        TValue m32 = this->m[2][1] * invAffineScale.y;
        TValue m33 = this->m[2][2] * invAffineScale.z;

        if (Math::Abs(m31) >= (TValue)1.0)
        {
            euler.z = (TValue)0.0;

            if (m31 < 0.0)
            {
                euler.y = Math::c_pi / (TValue)2.0;
                euler.x = atan2(m12, m13);
            }
            else
            {
                euler.y = (-Math::c_pi / (TValue)2.0);
                euler.x = atan2(-m12, -m13);
            }
        }
        else
        {
            euler.y = -asin(m31);    
            euler.x = atan2(m32 / cos(euler.y), m33 / cos(euler.y));
            euler.z = atan2(m21 / cos(euler.y), m11 / cos(euler.y));
        }
        
        return euler;
    }
    */

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat4<TValue> Mat4<TValue>::operator*(F32 _scalar) const
    {
        return Mat4(
            (TValue)(_scalar * m[0][0]), (TValue)(_scalar * m[0][1]), (TValue)(_scalar * m[0][2]), (TValue)(_scalar * m[0][3]),
            (TValue)(_scalar * m[1][0]), (TValue)(_scalar * m[1][1]), (TValue)(_scalar * m[1][2]), (TValue)(_scalar * m[1][3]),
            (TValue)(_scalar * m[2][0]), (TValue)(_scalar * m[2][1]), (TValue)(_scalar * m[2][2]), (TValue)(_scalar * m[2][3]),
            (TValue)(_scalar * m[3][0]), (TValue)(_scalar * m[3][1]), (TValue)(_scalar * m[3][2]), (TValue)(_scalar * m[3][3]));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Mat4<TValue>::isAffine() const
    {
        return m[0][3] == 0 && m[1][3] == 0 && m[2][3] == 0 && m[3][3] == 1;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::concatenatedAffineCopy(Mat4 const& _m2) const
    {
        Mat4<TValue> m;
        concatenateAffine(_m2, m);
        return m;
    }

    //////////////////////////////////////////
    //
    // [ _m2[0][0] _m2[0][1] _m2[0][2] 0 ]   [ m[0][0] m[0][1] m[0][2] 0 ]
    // | _m2[1][0] _m2[1][1] _m2[1][2] 0 | * | m[1][0] m[1][1] m[1][2] 0 |
    // | _m2[2][0] _m2[2][1] _m2[2][2] 0 |   | m[2][0] m[2][1] m[2][2] 0 |
    // [ _m2[3][0] _m2[3][1] _m2[3][2] 1 ]   [ m[3][0] m[3][1] m[3][2] 1 ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::concatenateAffine(Mat4 const& _m2, Mat4& _out) const
    {
        _out[0][0] = _m2.m[0][0] * m[0][0] + _m2.m[0][1] * m[1][0] + _m2.m[0][2] * m[2][0];
        _out[0][1] = _m2.m[0][0] * m[0][1] + _m2.m[0][1] * m[1][1] + _m2.m[0][2] * m[2][1];
        _out[0][2] = _m2.m[0][0] * m[0][2] + _m2.m[0][1] * m[1][2] + _m2.m[0][2] * m[2][2];
        _out[0][3] = 0;

        _out[1][0] = _m2.m[1][0] * m[0][0] + _m2.m[1][1] * m[1][0] + _m2.m[1][2] * m[2][0];
        _out[1][1] = _m2.m[1][0] * m[0][1] + _m2.m[1][1] * m[1][1] + _m2.m[1][2] * m[2][1];
        _out[1][2] = _m2.m[1][0] * m[0][2] + _m2.m[1][1] * m[1][2] + _m2.m[1][2] * m[2][2];
        _out[1][3] = 0;

        _out[2][0] = _m2.m[2][0] * m[0][0] + _m2.m[2][1] * m[1][0] + _m2.m[2][2] * m[2][0];
        _out[2][1] = _m2.m[2][0] * m[0][1] + _m2.m[2][1] * m[1][1] + _m2.m[2][2] * m[2][1];
        _out[2][2] = _m2.m[2][0] * m[0][2] + _m2.m[2][1] * m[1][2] + _m2.m[2][2] * m[2][2];
        _out[2][3] = 0;

        _out[3][0] = _m2.m[3][0] * m[0][0] + _m2.m[3][1] * m[1][0] + _m2.m[3][2] * m[2][0] + m[3][0];
        _out[3][1] = _m2.m[3][0] * m[0][1] + _m2.m[3][1] * m[1][1] + _m2.m[3][2] * m[2][1] + m[3][1];
        _out[3][2] = _m2.m[3][0] * m[0][2] + _m2.m[3][1] * m[1][2] + _m2.m[3][2] * m[2][2] + m[3][2];
        _out[3][3] = 1;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Mat4<TValue>::transformAffine(Vec2<TValue> const& _v) const
    {
        return Vec2<TValue>(
            m[0][0] * _v.x + m[1][0] * _v.y + m[3][0], 
            m[0][1] * _v.x + m[1][1] * _v.y + m[3][1]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::transformAffine(Vec2<TValue> const& _v, TValue& _outX, TValue& _outY) const
    {
        _outX = m[0][0] * _v.x + m[1][0] * _v.y + m[3][0];
        _outY = m[0][1] * _v.x + m[1][1] * _v.y + m[3][1];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat4<TValue>::transformAffine(Vec2<TValue> const& _v, Vec2<TValue>& _out) const
    {
        transformAffine(_v, _out.x, _out.y);
    }

    //////////////////////////////////////////
    //
    //                           [ m[0][0] m[0][1] m[0][2] m[0][3] ]
    // [_v.x, _v.y, _v.z, 1.0] * | m[1][0] m[1][1] m[1][2] m[1][3] |
    //                           | m[2][0] m[2][1] m[2][2] m[2][3] |
    //                           [ m[3][0] m[3][1] m[3][2] m[3][3] ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Mat4<TValue>::transformAffine(Vec3<TValue> const& _v) const
    {
        return Vec3<TValue>(
            _v.x * m[0][0] + _v.y * m[1][0] + _v.z * m[2][0] + m[3][0],
            _v.x * m[0][1] + _v.y * m[1][1] + _v.z * m[2][1] + m[3][1],
            _v.x * m[0][2] + _v.y * m[1][2] + _v.z * m[2][2] + m[3][2]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec4<TValue> Mat4<TValue>::transformAffine(Vec4<TValue> const& _v) const
    {
        return Vec4<TValue>(
            _v.x * m[0][0] + _v.y * m[1][0] + _v.z * m[2][0] + m[3][0] * _v.w,
            _v.x * m[0][1] + _v.y * m[1][1] + _v.z * m[2][1] + m[3][1] * _v.w,
            _v.x * m[0][2] + _v.y * m[1][2] + _v.z * m[2][2] + m[3][2] * _v.w,
            _v.w);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat4<TValue>::compareAffine(Mat3<TValue> const& _m2) const
    {
        if (!Math::Equal<TValue>(m[0][0], _m2.m[0][0], c_epsilon) || !Math::Equal<TValue>(m[0][1], _m2.m[0][1], c_epsilon) ||
            !Math::Equal<TValue>(m[1][0], _m2.m[1][0], c_epsilon) || !Math::Equal<TValue>(m[1][1], _m2.m[1][1], c_epsilon) ||
            !Math::Equal<TValue>(m[2][0], _m2.m[2][0], c_epsilon) || !Math::Equal<TValue>(m[2][1], _m2.m[2][1], c_epsilon))
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat4<TValue>::compareAffine(Mat4 const& _m2) const
    {
        if (
            m[0][0] != _m2.m[0][0] || m[0][1] != _m2.m[0][1] || m[0][2] != _m2.m[0][2] ||
            m[1][0] != _m2.m[1][0] || m[1][1] != _m2.m[1][1] || m[1][2] != _m2.m[1][2] ||
            m[2][0] != _m2.m[2][0] || m[2][1] != _m2.m[2][1] || m[2][2] != _m2.m[2][2] ||
            m[3][0] != _m2.m[3][0] || m[3][1] != _m2.m[3][1] || m[3][2] != _m2.m[3][2])
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat4<TValue>::getAffineScaleXSignless() const
    {
        return Vec3<TValue>(m[0][0], m[0][1], m[0][2]).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat4<TValue>::getAffineScaleYSignless() const
    {
        return Vec3<TValue>(m[1][0], m[1][1], m[1][2]).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat4<TValue>::getAffineScaleZSignless() const
    {
        return Vec3<TValue>(m[2][0], m[2][1], m[2][2]).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Mat4<TValue>::getAffineScaleSignless() const
    {
        return Vec3<TValue>(getAffineScaleXSignless(), getAffineScaleYSignless(), getAffineScaleZSignless());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Mat4<TValue>::getAffineScale2DSignless() const
    {
        return Vec2<TValue>(getAffineScaleXSignless(), getAffineScaleYSignless());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rotation2D Mat4<TValue>::getAffineRotation2D() const 
    { 
        return Rotation2D((F32)m[0][1] / getAffineScaleXSignless(), (F32)m[0][0] / getAffineScaleXSignless());
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Mat4<TValue> Mat4<TValue>::inversedCopy() const
    {
        TValue m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
        TValue m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
        TValue m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
        TValue m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

        TValue v0 = m20 * m31 - m21 * m30;
        TValue v1 = m20 * m32 - m22 * m30;
        TValue v2 = m20 * m33 - m23 * m30;
        TValue v3 = m21 * m32 - m22 * m31;
        TValue v4 = m21 * m33 - m23 * m31;
        TValue v5 = m22 * m33 - m23 * m32;

        TValue t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
        TValue t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
        TValue t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
        TValue t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

        TValue invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        TValue d00 = t00 * invDet;
        TValue d10 = t10 * invDet;
        TValue d20 = t20 * invDet;
        TValue d30 = t30 * invDet;

        TValue d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        TValue d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        TValue d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        TValue d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        TValue d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        TValue d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        TValue d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        TValue d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        TValue d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        TValue d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        TValue d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        TValue d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        return Mat4<TValue>(
            d00, d01, d02, d03,
            d10, d11, d12, d13,
            d20, d21, d22, d23,
            d30, d31, d32, d33);
    }

    //////////////////////////////////////////
    template <class TValue>
    Mat4<TValue> Mat4<TValue>::inversedAffineCopy() const
    {
        F32 m01 = (F32)m[0][1], m11 = (F32)m[1][1], m21 = (F32)m[2][1];
        F32 m02 = (F32)m[0][2], m12 = (F32)m[1][2], m22 = (F32)m[2][2];
        
        F32 t00 = m22 * m11 - m12 * m21;
        F32 t01 = m02 * m21 - m22 * m01;
        F32 t02 = m12 * m01 - m02 * m11;
        
        F32 m00 = (F32)m[0][0], m10 = (F32)m[1][0], m20 = (F32)m[2][0];
        
        F32 invDet = 1 / (m00 * t00 + m10 * t01 + m20 * t02);
        
        t00 *= invDet; t01 *= invDet; t02 *= invDet;
        
        m00 *= invDet; m10 *= invDet; m20 *= invDet;
        
        F32 r00 = t00;
        F32 r10 = m20 * m12 - m10 * m22;
        F32 r20 = m10 * m21 - m20 * m11;
        
        F32 r01 = t01;
        F32 r11 = m00 * m22 - m20 * m02;
        F32 r21 = m20 * m01 - m00 * m21;
        
        F32 r02 = t02;
        F32 r12 = m10 * m02 - m00 * m12;
        F32 r22 = m00 * m11 - m10 * m01;
        
        F32 m30 = (F32)m[3][0], m31 = (F32)m[3][1], m32 = (F32)m[3][2];
        
        F32 r30 = - (r00 * m30 + r10 * m31 + r20 * m32);
        F32 r31 = - (r01 * m30 + r11 * m31 + r21 * m32);
        F32 r32 = - (r02 * m30 + r12 * m31 + r22 * m32);
        
        return Mat4<TValue>(
        (TValue)r00, (TValue)r01, (TValue)r02, 0,
        (TValue)r10, (TValue)r11, (TValue)r12, 0,
        (TValue)r20, (TValue)r21, (TValue)r22, 0,
        (TValue)r30, (TValue)r31, (TValue)r32, 1);
    }


    //////////////////////////////////////////
    template <class TValue>
    inline String Mat4<TValue>::toString(Char _separator) const
    {
        String result;

        for (S32 i = 0; i < 16; ++i)
        {
            if (!result.empty())
                result += _separator;

            result += StringHelper::ToString(planeMatrix[i]);
        }

        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline CString Mat4<TValue>::ParseString(CString _string, Size _size, Mat4& _result, Char _separator)
    {
        CString end = _string + _size;

        S32 i = 0;
        for (; i < 15; ++i)
        {
            _string = StringHelper::ParseNumber<TValue>(_string, end, _result.planeMatrix[i]);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::SkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
        }
        _string = StringHelper::ParseNumber<TValue>(_string, end, _result.planeMatrix[i]);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline CString Mat4<TValue>::ParseStringPretty(CString _string, Size _size, Mat4& _result, Char _separator)
    {
        TValue v;

        CString end = _string + _size;
        _string = StringHelper::ExpectSkipChar(_string, end, '[', 1);
        for (S32 r = 0; r < 4; ++r)
        {
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, '[', 1);
            _string = StringHelper::SkipChar(_string, end, ' ');

            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _result[r][0] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _result[r][1] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _result[r][2] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _result[r][3] = v;

            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, ']', 1);
        }
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, ']', 1);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat4<TValue> Mat4<TValue>::FromString(CString _string, Size _size, Char _separator)
    {
        Mat4 result = Mat4::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    Mat4<TValue> Mat4<TValue>::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }

} // namespace Maze
//////////////////////////////////////////
