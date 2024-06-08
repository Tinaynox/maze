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
    // Class TMat
    //
    // [ m[0][0]  m[0][1]  m[0][2] ]
    // | m[1][0]  m[1][1]  m[1][2] |
    // | m[2][0]  m[2][1]  m[2][2] |
    // [ m[3][0]  m[3][1]  m[3][2] ]
    //
    // Affine scheme:
    // [ RX RY RZ RW ]
    // | UX UY UZ UW |
    // | FX FY FZ FW |
    // [ TX TY TZ TW ]
    // R - right, U - up, F - front, T - translation
    //
    //////////////////////////////////////////
    inline TMat::TMat()
        : planeMatrix{ 0 }
    {
    }

    //////////////////////////////////////////
    inline TMat::TMat(ValueType const _arr[4][3])
    {
        memcpy(m, _arr, 12 * sizeof(ValueType));
    }

    //////////////////////////////////////////
    inline TMat::TMat(TMat const& _m)
    {
        memcpy(m, _m.m, 12 * sizeof(ValueType));
    }

    //////////////////////////////////////////
    inline TMat::TMat(Mat3<ValueType> const& _m)
        : planeMatrix{    
            _m[0][0],    _m[0][1],    _m[0][2],
            _m[1][0],    _m[1][1],    _m[1][2],
            _m[2][0],    _m[2][1],    _m[2][2],
            0,           0,           0}
    {
    }

    //////////////////////////////////////////
    inline TMat::TMat(    
        ValueType _m00, ValueType _m01, ValueType _m02,
        ValueType _m10, ValueType _m11, ValueType _m12,
        ValueType _m20, ValueType _m21, ValueType _m22,
        ValueType _m30, ValueType _m31, ValueType _m32)
    {
        set(_m00,  _m01,  _m02,
            _m10,  _m11,  _m12,
            _m20,  _m21,  _m22,
            _m30,  _m31,  _m32);
    }

    //////////////////////////////////////////
    inline void TMat::set(    
        ValueType _m00, ValueType _m01, ValueType _m02,
        ValueType _m10, ValueType _m11, ValueType _m12,
        ValueType _m20, ValueType _m21, ValueType _m22,
        ValueType _m30, ValueType _m31, ValueType _m32)
    {
        m[0][0] = _m00; m[0][1] = _m01; m[0][2] = _m02;
        m[1][0] = _m10; m[1][1] = _m11; m[1][2] = _m12;
        m[2][0] = _m20; m[2][1] = _m21; m[2][2] = _m22;
        m[3][0] = _m30; m[3][1] = _m31; m[3][2] = _m32;
    }
    
    //////////////////////////////////////////
    inline TMat TMat::CreateTranslation(ValueType _x, ValueType _y, ValueType _z)
    {
        return TMat(
            1,  0,  0,
            0,  1,  0,
            0,  0,  1,
            _x, _y, _z);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateBasis(
        Vec3<ValueType> const& _right,
        Vec3<ValueType> const& _up,
        Vec3<ValueType> const& _forward)
    {
        return TMat(
            _right.x,   _right.y,   _right.z,
            _up.x,      _up.y,      _up.z,
            _forward.x, _forward.y, _forward.z,
            0,          0,          0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateTranslation(Vec2<ValueType> const& _v)
    {
        return CreateTranslation(_v.x, _v.y);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateTranslation(Vec3<ValueType> const& _v)
    {
        return CreateTranslation(_v.x, _v.y, _v.z);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateRotationZ(Rotation2D const& _rotation)
    {
        return TMat(
            (ValueType)(_rotation.getCos()),  (ValueType)(_rotation.getSin()), (ValueType)0,
            (ValueType)(-_rotation.getSin()), (ValueType)(_rotation.getCos()), (ValueType)0,
            (ValueType)0,                     (ValueType)0,                    (ValueType)1,
            (ValueType)0,                     (ValueType)0,                    (ValueType)0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateRotationX(F32 _angle)
    {
        // CCW rotation around X axis 
        F32 c = cosf(_angle);
        F32 s = sinf(_angle);
        return TMat(
            (ValueType)1, (ValueType)0,  (ValueType)0,
            (ValueType)0, (ValueType)c,  (ValueType)s,
            (ValueType)0, (ValueType)-s, (ValueType)c,
            (ValueType)0, (ValueType)0,  (ValueType)0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateRotationY(F32 _angle)
    {
        // CCW rotation around Y axis 
        F32 c = cosf(_angle);
        F32 s = sinf(_angle);
        return TMat(
            (ValueType)c, (ValueType)0, (ValueType)-s,
            (ValueType)0, (ValueType)1, (ValueType)0,
            (ValueType)s, (ValueType)0, (ValueType)c,
            (ValueType)0, (ValueType)0, (ValueType)0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateRotationZ(F32 _angle)
    {
        // CCW rotation around Z axis 
        F32 c = cosf(_angle);
        F32 s = sinf(_angle);
        return TMat(
            (ValueType)c, (ValueType)s, (ValueType)0,
            (ValueType)-s, (ValueType)c,  (ValueType)0,
            (ValueType)0, (ValueType)0,  (ValueType)1,
            (ValueType)0, (ValueType)0,  (ValueType)0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateRotation(F32 _angleX, F32 _angleY, F32 _angleZ)
    {
        // CCW rotation
        F32 cx = cosf(_angleX);
        F32 sx = sinf(_angleX);

        F32 cy = cosf(_angleY);
        F32 sy = sinf(_angleY);

        F32 cz = cosf(_angleZ);
        F32 sz = sinf(_angleZ);

        return 
            TMat(
                (ValueType)cy, (ValueType)0, (ValueType)-sy,
                (ValueType)0, (ValueType)1, (ValueType)0,
                (ValueType)sy, (ValueType)0, (ValueType)cy,
                (ValueType)0, (ValueType)0, (ValueType)0).transform(
                TMat(
                    (ValueType)1, (ValueType)0, (ValueType)0,
                    (ValueType)0, (ValueType)cx, (ValueType)sx,
                    (ValueType)0, (ValueType)-sx, (ValueType)cx,
                    (ValueType)0, (ValueType)0, (ValueType)0)).transform(
                    TMat(
                        (ValueType)cz, (ValueType)sz, (ValueType)0,
                        (ValueType)-sz, (ValueType)cz, (ValueType)0,
                        (ValueType)0, (ValueType)0, (ValueType)1,
                        (ValueType)0, (ValueType)0, (ValueType)0));
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateScale(ValueType _x, ValueType _y, ValueType _z)
    {
        return TMat(
            _x, 0,  0,
            0,  _y, 0,
            0,  0,  _z,
            0,  0,  0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateScale(Vec2<ValueType> const& _v)
    {
        return CreateScale(_v.x, _v.y);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateScale(Vec3<ValueType> const& _v)
    {
        return CreateScale(_v.x, _v.y, _v.z);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateTransform(
        Vec2<ValueType> const& _pos,
        Rotation2D& _rotation,
        Vec2<ValueType> const& _scale)
    {
        return TMat(
            (ValueType)(_rotation.getCos() * _scale.x),  (ValueType)(_rotation.getSin() * _scale.y), 0,
            (ValueType)(-_rotation.getSin() * _scale.x), (ValueType)(_rotation.getCos() * _scale.y), 0,
            0,                                           0,                                          1,
            _pos.x,                                      _pos.y,                                     0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateTransform(
        Vec2<ValueType> const& _pos,
        Rotation2D& _rotation,
        Vec2<ValueType> const& _scale,
        Vec2<ValueType> const& _pivot,
        Vec2<ValueType> const& _size)
    {
        ValueType sax = _size.x * _pivot.x;
        ValueType say = _size.y * _pivot.y;

        ValueType x = (ValueType)(-_rotation.getCos() * _scale.x * sax + _rotation.getSin() * _scale.y * say + _pos.x);
        ValueType y = (ValueType)(-_rotation.getSin() * _scale.x * sax - _rotation.getCos() * _scale.y * say + _pos.y);

        return TMat(
            (ValueType)(_rotation.getCos() * _scale.x),  (ValueType)(_rotation.getSin() * _scale.x), 0,
            (ValueType)(-_rotation.getSin() * _scale.y), (ValueType)(_rotation.getCos() * _scale.y), 0,
            0,                                        0,                                             1,
            x,                                        y,                                             0);
    }

    //////////////////////////////////////////
    inline TMat TMat::CreateLookAt(
        Vec3<ValueType> const& _from,
        Vec3<ValueType> const& _to,
        Vec3<ValueType> const& _up)
    {
        Vec3<ValueType> forward = (_from - _to).normalizedCopy();
        Vec3<ValueType> right = _up.crossProduct(forward);
        Vec3<ValueType> up = forward.crossProduct(right);

        TMat m;
        m.setRow(0, right);
        m.setRow(1, up);
        m.setRow(2, forward);
        m.setRow(3, _from);

        return m;
    }

    //////////////////////////////////////////
    inline void TMat::swap(TMat& _m)
    {
        std::swap(m[0][0], _m[0][0]); std::swap(m[0][1], _m[0][1]); std::swap(m[0][2], _m[0][2]);
        std::swap(m[1][0], _m[1][0]); std::swap(m[1][1], _m[1][1]); std::swap(m[1][2], _m[1][2]);
        std::swap(m[2][0], _m[2][0]); std::swap(m[2][1], _m[2][1]); std::swap(m[2][2], _m[2][2]);
        std::swap(m[3][0], _m[3][0]); std::swap(m[3][1], _m[3][1]); std::swap(m[3][2], _m[3][2]);
    }

    //////////////////////////////////////////
    inline Vec3<TMat::ValueType> TMat::getRow(Size _row) const
    {
        return *(Vec3<ValueType>*)m[_row];
    }

    //////////////////////////////////////////
    inline void TMat::setRow(Size _row, Vec3<ValueType> const& _v)
    {
        *(Vec3<ValueType>*)m[_row] = _v;
    }

    //////////////////////////////////////////
    inline TMat::ValueType* TMat::operator[](Size _row) const
    {
        return (ValueType*)m[_row];
    }

    //////////////////////////////////////////
    inline Vec4<TMat::ValueType> TMat::getColumn(Size _column) const
    {
        return Vec4<ValueType>(
            m[0][_column],
            m[1][_column],
            m[2][_column],
            m[3][_column]);
    }

    //////////////////////////////////////////
    inline void TMat::setColumn(Size _column, Vec4<ValueType> const& _v)
    {
        m[0][_column] = _v.x;
        m[1][_column] = _v.y;
        m[2][_column] = _v.z;
        m[3][_column] = _v.w;
    }

    //////////////////////////////////////////
    inline TMat& TMat::operator=(TMat const& _m)
    {
        memcpy(m, _m.m, 12 * sizeof(ValueType));
        return *this;
    }

    //////////////////////////////////////////
    inline bool TMat::operator==(TMat const& _m) const
    {
        return compare(_m);
    }

    //////////////////////////////////////////
    inline bool TMat::operator!=(TMat const& _m) const
    {
        return !compare(_m);
    }

    //////////////////////////////////////////
    inline void TMat::multiply(TMat const& m2)
    {
        (*this) = multiplied(m2);
    }

    //////////////////////////////////////////
    inline TMat TMat::multiplied(TMat const& _m) const
    {
        TMat res;
        _m.transform(*this, res);
        return res;
    }

    //////////////////////////////////////////
    inline TMat TMat::operator*(TMat const& _m) const
    {
        return multiplied(_m);
    }

    //////////////////////////////////////////
    inline Vec2<TMat::ValueType> operator*(Vec2<TMat::ValueType> const& _v, TMat const& _m)
    {
        return _m.transform(_v);
    }

    //////////////////////////////////////////
    inline Vec3<TMat::ValueType> operator*(Vec3<TMat::ValueType> const& _v, TMat const& _m)
    {
        return _m.transform(_v);
    }

    //////////////////////////////////////////
    inline Vec4<TMat::ValueType> operator*(Vec4<TMat::ValueType> const& _v, TMat const& _m)
    {
        return _m.transform(_v);
    }

    //////////////////////////////////////////
    inline TMat TMat::operator+(TMat const& _m) const
    {
        TMat res;

        res[0][0] = m[0][0] + _m[0][0];
        res[0][1] = m[0][1] + _m[0][1];
        res[0][2] = m[0][2] + _m[0][2];

        res[1][0] = m[1][0] + _m[1][0];
        res[1][1] = m[1][1] + _m[1][1];
        res[1][2] = m[1][2] + _m[1][2];

        res[2][0] = m[2][0] + _m[2][0];
        res[2][1] = m[2][1] + _m[2][1];
        res[2][2] = m[2][2] + _m[2][2];

        res[3][0] = m[3][0] + _m[3][0];
        res[3][1] = m[3][1] + _m[3][1];
        res[3][2] = m[3][2] + _m[3][2];

        return res;
    }

    //////////////////////////////////////////
    inline TMat TMat::operator-(TMat const& _m) const
    {
        TMat res;

        res[0][0] = m[0][0] - _m[0][0];
        res[0][1] = m[0][1] - _m[0][1];
        res[0][2] = m[0][2] - _m[0][2];

        res[1][0] = m[1][0] - _m[1][0];
        res[1][1] = m[1][1] - _m[1][1];
        res[1][2] = m[1][2] - _m[1][2];

        res[2][0] = m[2][0] - _m[2][0];
        res[2][1] = m[2][1] - _m[2][1];
        res[2][2] = m[2][2] - _m[2][2];

        res[3][0] = m[3][0] - _m[3][0];
        res[3][1] = m[3][1] - _m[3][1];
        res[3][2] = m[3][2] - _m[3][2];

        return res;
    }

    //////////////////////////////////////////
    inline void TMat::operator=(Mat3<ValueType> const& _m)
    {
        memcpy(m, _m.m, 9 * sizeof(ValueType));
        setTranslation(Vec3F::c_zero);
    }

    //////////////////////////////////////////
    inline void TMat::setTranslation(Vec2<ValueType> const& _v)
    {
        m[3][0] = _v.x;
        m[3][1] = _v.y;
    }

    //////////////////////////////////////////
    inline void TMat::setTranslation(Vec3<ValueType> const& _v)
    {
        m[3][0] = _v.x;
        m[3][1] = _v.y;
        m[3][2] = _v.z;
    }

    //////////////////////////////////////////
    inline Vec3<TMat::ValueType> TMat::getTranslation() const
    {
        return Vec3<ValueType>(
            m[3][0],
            m[3][1],
            m[3][2]);
    }

    //////////////////////////////////////////
    inline Vec2<TMat::ValueType> TMat::getTranslation2D() const
    {
        return Vec2<ValueType>(
            m[3][0],
            m[3][1]);
    }

    //////////////////////////////////////////
    inline TMat TMat::transform(TMat const& _m) const
    {
        TMat m;
        transform(_m, m);
        return m;
    }

    //////////////////////////////////////////
    //
    // [ _m[0][0] _m[0][1] _m[0][2] 0 ]   [ m[0][0] m[0][1] m[0][2] 0 ]
    // | _m[1][0] _m[1][1] _m[1][2] 0 | * | m[1][0] m[1][1] m[1][2] 0 |
    // | _m[2][0] _m[2][1] _m[2][2] 0 |   | m[2][0] m[2][1] m[2][2] 0 |
    // [ _m[3][0] _m[3][1] _m[3][2] 1 ]   [ m[3][0] m[3][1] m[3][2] 1 ]
    // 
    //////////////////////////////////////////
    inline void TMat::transform(TMat const& _m, TMat& _out) const
    {
        _out[0][0] = _m[0][0] * m[0][0] + _m[0][1] * m[1][0] + _m[0][2] * m[2][0];
        _out[0][1] = _m[0][0] * m[0][1] + _m[0][1] * m[1][1] + _m[0][2] * m[2][1];
        _out[0][2] = _m[0][0] * m[0][2] + _m[0][1] * m[1][2] + _m[0][2] * m[2][2];

        _out[1][0] = _m[1][0] * m[0][0] + _m[1][1] * m[1][0] + _m[1][2] * m[2][0];
        _out[1][1] = _m[1][0] * m[0][1] + _m[1][1] * m[1][1] + _m[1][2] * m[2][1];
        _out[1][2] = _m[1][0] * m[0][2] + _m[1][1] * m[1][2] + _m[1][2] * m[2][2];

        _out[2][0] = _m[2][0] * m[0][0] + _m[2][1] * m[1][0] + _m[2][2] * m[2][0];
        _out[2][1] = _m[2][0] * m[0][1] + _m[2][1] * m[1][1] + _m[2][2] * m[2][1];
        _out[2][2] = _m[2][0] * m[0][2] + _m[2][1] * m[1][2] + _m[2][2] * m[2][2];

        _out[3][0] = _m[3][0] * m[0][0] + _m[3][1] * m[1][0] + _m[3][2] * m[2][0] + m[3][0];
        _out[3][1] = _m[3][0] * m[0][1] + _m[3][1] * m[1][1] + _m[3][2] * m[2][1] + m[3][1];
        _out[3][2] = _m[3][0] * m[0][2] + _m[3][1] * m[1][2] + _m[3][2] * m[2][2] + m[3][2];
    }

    //////////////////////////////////////////
    inline Vec2<TMat::ValueType> TMat::transform(Vec2<ValueType> const& _v) const
    {
        Vec2<ValueType> res;
        transform(_v, res.x, res.y);
        return res;
    }

    /////////////////////////////////////////
    //
    //                    [ m[0][0] m[0][1] m[0][2] 0 ]
    // [ _v.x _v.y 0 1] * | m[1][0] m[1][1] m[1][2] 0 |
    //                    | m[2][0] m[2][1] m[2][2] 0 |
    //                    [ m[3][0] m[3][1] m[3][2] 1 ]
    // 
    //////////////////////////////////////////
    inline void TMat::transform(Vec2<ValueType> const& _v, ValueType& _outX, ValueType& _outY) const
    {
        _outX = m[0][0] * _v.x + m[1][0] * _v.y + m[3][0];
        _outY = m[0][1] * _v.x + m[1][1] * _v.y + m[3][1];
    }

    //////////////////////////////////////////
    inline void TMat::transform(Vec2<ValueType> const& _v, Vec2<ValueType>& _out) const
    {
        transform(_v, _out.x, _out.y);
    }

    //////////////////////////////////////////
    //
    //                           [ m[0][0] m[0][1] m[0][2] 0 ]
    // [_v.x, _v.y, _v.z, 1.0] * | m[1][0] m[1][1] m[1][2] 0 |
    //                           | m[2][0] m[2][1] m[2][2] 0 |
    //                           [ m[3][0] m[3][1] m[3][2] 1 ]
    // 
    //////////////////////////////////////////
    inline Vec3<TMat::ValueType> TMat::transform(Vec3<ValueType> const& _v) const
    {
        return Vec3<ValueType>(
            _v.x * m[0][0] + _v.y * m[1][0] + _v.z * m[2][0] + m[3][0],
            _v.x * m[0][1] + _v.y * m[1][1] + _v.z * m[2][1] + m[3][1],
            _v.x * m[0][2] + _v.y * m[1][2] + _v.z * m[2][2] + m[3][2]);
    }

    //////////////////////////////////////////
    //
    //                            [ m[0][0] m[0][1] m[0][2] 0 ]
    // [_v.x, _v.y, _v.z, _v.w] * | m[1][0] m[1][1] m[1][2] 0 |
    //                            | m[2][0] m[2][1] m[2][2] 0 |
    //                            [ m[3][0] m[3][1] m[3][2] 1 ]
    // 
    //////////////////////////////////////////
    inline Vec4<TMat::ValueType> TMat::transform(Vec4<ValueType> const& _v) const
    {
        return Vec4<ValueType>(
            _v.x * m[0][0] + _v.y * m[1][0] + _v.z * m[2][0] + m[3][0] * _v.w,
            _v.x * m[0][1] + _v.y * m[1][1] + _v.z * m[2][1] + m[3][1] * _v.w,
            _v.x * m[0][2] + _v.y * m[1][2] + _v.z * m[2][2] + m[3][2] * _v.w,
            _v.w);
    }

    //////////////////////////////////////////
    inline bool TMat::compare(Mat3<ValueType> const& _m) const
    {
        if (!Math::Equal<ValueType>(m[0][0], _m[0][0], c_epsilon) || !Math::Equal<ValueType>(m[0][1], _m[0][1], c_epsilon) ||
            !Math::Equal<ValueType>(m[1][0], _m[1][0], c_epsilon) || !Math::Equal<ValueType>(m[1][1], _m[1][1], c_epsilon) ||
            !Math::Equal<ValueType>(m[2][0], _m[2][0], c_epsilon) || !Math::Equal<ValueType>(m[2][1], _m[2][1], c_epsilon))
            return false;
        return true;
    }

    //////////////////////////////////////////
    inline bool TMat::compare(TMat const& _m) const
    {
        if (m[0][0] != _m[0][0] || m[0][1] != _m[0][1] || m[0][2] != _m[0][2] ||
            m[1][0] != _m[1][0] || m[1][1] != _m[1][1] || m[1][2] != _m[1][2] ||
            m[2][0] != _m[2][0] || m[2][1] != _m[2][1] || m[2][2] != _m[2][2] ||
            m[3][0] != _m[3][0] || m[3][1] != _m[3][1] || m[3][2] != _m[3][2])
            return false;
        return true;
    }

    //////////////////////////////////////////
    inline TMat::ValueType TMat::getScaleXSignless() const
    {
        return getRow(0).length();
    }

    //////////////////////////////////////////
    inline TMat::ValueType TMat::getScaleYSignless() const
    {
        return getRow(1).length();
    }

    //////////////////////////////////////////
    inline TMat::ValueType TMat::getScaleZSignless() const
    {
        return getRow(2).length();
    }

    //////////////////////////////////////////
    inline Vec3<TMat::ValueType> TMat::getScaleSignless() const
    {
        return Vec3<ValueType>(getScaleXSignless(), getScaleYSignless(), getScaleZSignless());
    }

    //////////////////////////////////////////
    inline Vec2<TMat::ValueType> TMat::getScale2DSignless() const
    {
        return Vec2<ValueType>(getScaleXSignless(), getScaleYSignless());
    }

    //////////////////////////////////////////
    inline Rotation2D TMat::getRotation2D() const 
    { 
        return Rotation2D((F32)m[0][1] / getScaleXSignless(), (F32)m[0][0] / getScaleXSignless());
    }

    //////////////////////////////////////////
    inline void TMat::getMat3(Mat3<ValueType>& _m) const
    {
        memcpy(_m.m, m, 9 * sizeof(ValueType));
    }
    
    //////////////////////////////////////////
    inline TMat TMat::inversed() const
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
        
        return TMat(
            (ValueType)r00, (ValueType)r01, (ValueType)r02,
            (ValueType)r10, (ValueType)r11, (ValueType)r12,
            (ValueType)r20, (ValueType)r21, (ValueType)r22,
            (ValueType)r30, (ValueType)r31, (ValueType)r32);
    }


    //////////////////////////////////////////
    inline String TMat::toString(Char _separator) const
    {
        String result;

        for (S32 i = 0; i < 12; ++i)
        {
            if (!result.empty())
                result += _separator;

            result += StringHelper::ToString(planeMatrix[i]);
        }

        return result;
    }

    //////////////////////////////////////////
    inline CString TMat::ParseString(CString _string, Size _size, TMat& _out, Char _separator)
    {
        CString end = _string + _size;

        S32 i = 0;
        for (; i < 11; ++i)
        {
            _string = StringHelper::ParseNumber<ValueType>(_string, end, _out.planeMatrix[i]);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::SkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
        }
        _string = StringHelper::ParseNumber<ValueType>(_string, end, _out.planeMatrix[i]);
        return _string;
    }

    //////////////////////////////////////////
    inline CString TMat::ParseStringPretty(CString _string, Size _size, TMat& _out, Char _separator)
    {
        ValueType v;

        CString end = _string + _size;
        _string = StringHelper::ExpectSkipChar(_string, end, '[', 1);
        for (S32 r = 0; r < 4; ++r)
        {
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, '[', 1);
            _string = StringHelper::SkipChar(_string, end, ' ');

            _string = StringHelper::ParseNumber<ValueType>(_string, end, v);
            _out[r][0] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<ValueType>(_string, end, v);
            _out[r][1] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<ValueType>(_string, end, v);
            _out[r][2] = v;

            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, ']', 1);
        }
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, ']', 1);
        return _string;
    }

    //////////////////////////////////////////
    inline TMat TMat::FromString(CString _string, Size _size, Char _separator)
    {
        TMat result = TMat::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    inline TMat TMat::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }

} // namespace Maze
//////////////////////////////////////////
