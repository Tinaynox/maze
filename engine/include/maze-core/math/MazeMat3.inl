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
    // Class Mat3
    // 
    // [ m[0][0]  m[0][1]  m[0][2] ]
    // | m[1][0]  m[1][1]  m[1][2] |
    // [ m[2][0]  m[2][1]  m[2][2] ]
    // 
    // Affine scheme:
    // [ RX RY RZ ]
    // | UX UY UZ |
    // [ FX FY FZ ]
    // R - right, U - up, F - front
    // 
    //////////////////////////////////////////
    template <class TValue>
    const TValue Mat3<TValue>::c_epsilon = (TValue)1e-05;
    template <class TValue>
    const Mat3<TValue> Mat3<TValue>::c_zero(
        0, 0, 0,
        0, 0, 0,
        0, 0, 0);
    template <class TValue>
    const Mat3<TValue> Mat3<TValue>::c_identity(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1);

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>::Mat3()
        : planeMatrix{ 0 }
    {
    }
    
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>::Mat3(TValue const _arr[3][3])
    {
        memcpy(m, _arr, 9 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>::Mat3(Mat3 const& _m)
    {
        memcpy(m, _m.m, 9 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>::Mat3(Mat3&& _m)
    {
        memcpy(m, _m.m, 9 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>::Mat3(    
        TValue _m00, TValue _m01, TValue _m02,
        TValue _m10, TValue _m11, TValue _m12,
        TValue _m20, TValue _m21, TValue _m22)
    {
        set(_m00, _m01, _m02,
            _m10, _m11, _m12,
            _m20, _m21, _m22);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::set(    
        TValue _m00, TValue _m01, TValue _m02,
        TValue _m10, TValue _m11, TValue _m12,
        TValue _m20, TValue _m21, TValue _m22)
    {
        m[0][0] = _m00; m[0][1] = _m01; m[0][2] = _m02;
        m[1][0] = _m10; m[1][1] = _m11; m[1][2] = _m12;
        m[2][0] = _m20; m[2][1] = _m21; m[2][2] = _m22;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::setAffine(    
        TValue _m00, TValue _m01,
        TValue _m10, TValue _m11,
        TValue _x, TValue _y)
    {
        m[0][0] = _m00; m[0][1] = _m01; m[0][2] = 0;
        m[1][0] = _m10; m[1][1] = _m11; m[1][2] = 0;
        m[2][0] = _x;   m[2][1] = _y;   m[2][2] = 1;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineTranslation(TValue _x, TValue _y)
    {
        return Mat3<TValue>(
            1,  0,  0,
            0,  1,  0,
            _x, _y, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineTranslation(
        Vec2<TValue> const& _v)
    {
        return CreateAffineTranslation(_v.x, _v.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineRotation(F32 _s, F32 _c)
    {
        return Mat3<TValue>(
                (TValue)_c,  (TValue)_s, (TValue)0,
                (TValue)-_s, (TValue)_c, (TValue)0,
                (TValue)0,   (TValue)0,  (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineRotation(F32 _angle)
    {
        return CreateAffineRotation(sinf(_angle), cosf(_angle));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineScale(TValue _x, TValue _y)
    {
        return Mat3<TValue>(
            _x, 0,  0,
            0,  _y, 0,
            0,  0,  (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineScale(TValue _x, TValue _y, TValue _z)
    {
        return Mat3<TValue>(
            _x, 0, 0,
            0, _y, 0,
            0, 0, _z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineScale(
        Vec2<TValue> const& _v)
    {
        return CreateAffineScale(_v.x, _v.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineScale(
        Vec3<TValue> const& _v)
    {
        return CreateAffineScale(_v.x, _v.y, _v.z);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineTransform(
        const Vec2F& _pos,
        const Rotation2D& _rotation,
        const Vec2F& _scale)
    {
        return Mat3<TValue>(
            (TValue)(_rotation.getCos() * _scale.x),  (TValue)(_rotation.getSin() * _scale.x), (TValue)(0),
            (TValue)(-_rotation.getSin() * _scale.y), (TValue)(_rotation.getCos() * _scale.y), (TValue)(0),
            (TValue)(_pos.x),                         (TValue)(_pos.y),                        (TValue)(1));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3<TValue> Mat3<TValue>::CreateAffineTransform(
        const Vec2F& _pos,
        F32 _angle,
        const Vec2F& _scale)
    {
        return CreateAffineTransform(_pos, Rotation2D(_angle), _scale);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::CreateAffineTransform(
        const Vec2F& _pos,
        const Rotation2D& _rotation,
        const Vec2F& _scale,
        const Vec2F& _pivot,
        const Vec2F& _size)
    {
        F32 sax = _size.x * _pivot.x;
        F32 say = _size.y * _pivot.y;

        F32 x = (TValue)(-_rotation.getCos() * _scale.x * sax + _rotation.getSin() * _scale.y * say + _pos.x);
        F32 y = (TValue)(-_rotation.getSin() * _scale.x * sax - _rotation.getCos() * _scale.y * say + _pos.y);

        return Mat3<TValue>(
            (TValue)(_rotation.getCos() * _scale.x),  (TValue)(_rotation.getSin() * _scale.x), (TValue)(0),
            (TValue)(-_rotation.getSin() * _scale.y), (TValue)(_rotation.getCos() * _scale.y), (TValue)(0),
            x,                                        y,                                       (TValue)(1));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::swap(Mat3<TValue>& _m)
    {
        std::swap(m[0][0], _m[0][0]); std::swap(m[0][1], _m[0][1]); std::swap(m[0][2], _m[0][2]);
        std::swap(m[1][0], _m[1][0]); std::swap(m[1][1], _m[1][1]); std::swap(m[1][2], _m[1][2]);
        std::swap(m[2][0], _m[2][0]); std::swap(m[2][1], _m[2][1]); std::swap(m[2][2], _m[2][2]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Mat3<TValue>::getRow(Size _row) const
    {
        return *(Vec3<TValue>*)m[_row];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::setRow(Size _row, Vec3<TValue> const& _v)
    {
        *(Vec3<TValue>*)m[_row] = _v;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Mat3<TValue>::operator[](Size _row) const
    {
        return (TValue*)m[_row];
    }
   
    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Mat3<TValue>::getColumn(Size _column) const
    {
        return Vec3<TValue>(m[0][_column], m[1][_column], m[2][_column]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::setColumn(Size _column, Vec3<TValue> const& _v)
    {
        m[0][_column] = _v.x;
        m[1][_column] = _v.y;
        m[2][_column] = _v.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>& Mat3<TValue>::operator=(Mat3<TValue> const& _m)
    {
        memcpy(m, _m.m, 9 * sizeof(TValue));
        return *this;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue>& Mat3<TValue>::operator=(Mat3<TValue>&& _m)
    {
        memcpy(m, _m.m, 9 * sizeof(TValue));
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat3<TValue>::operator==(Mat3<TValue> const& _m) const
    {
        if (m[0][0] != _m[0][0] || m[0][1] != _m[0][1] || m[0][2] != _m[0][2] ||
            m[1][0] != _m[1][0] || m[1][1] != _m[1][1] || m[1][2] != _m[1][2] ||
            m[2][0] != _m[2][0] || m[2][1] != _m[2][1] || m[2][2] != _m[2][2])
            return false;
        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat3<TValue>::operator!=(Mat3<TValue> const& _m) const
    {
        if (m[0][0] != _m[0][0] || m[0][1] != _m[0][1] || m[0][2] != _m[0][2] ||
            m[1][0] != _m[1][0] || m[1][1] != _m[1][1] || m[1][2] != _m[1][2] ||
            m[2][0] != _m[2][0] || m[2][1] != _m[2][1] || m[2][2] != _m[2][2])
            return true;
        return false;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::operator+(Mat3<TValue> const& _m) const
    {
        Mat3<TValue> res;
        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 3; c++)
                res[r][c] = m[r][c] + _m[r][c];
        return res;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::operator-(Mat3<TValue> const& _m) const
    {
        Mat3<TValue> res;
        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 3; c++)
                res[r][c] = m[r][c] - _m[r][c];
        return res;
    }
        
    //////////////////////////////////////////
    //
    // [ m[0][0] m[0][1] m[0][2] ]   [ _m[0][0] _m[0][1] _m[0][2] ]
    // | m[1][0] m[1][1] m[1][2] | * | _m[1][0] _m[1][1] _m[1][2] |
    // [ m[2][0] m[2][1] m[2][2] ]   [ _m[2][0] _m[2][1] _m[2][2] ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::operator*(Mat3<TValue> const& _m) const
    {
        Mat3<TValue> res;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
            {
                res[r][c] =
                    m[r][0] * _m[0][c] +
                    m[r][1] * _m[1][c] +
                    m[r][2] * _m[2][c];
            }
        }
        return res;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::operator-() const
    {
#pragma warning(push)
#pragma warning(disable : 4146)

        Mat3<TValue> res;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
                res[r][c] = -m[r][c];
        }
        return res;

#pragma warning(pop)
    }

    //////////////////////////////////////////
    //
    // [ m[0][0] m[0][1] 0 ]   [ _v.x ]
    // | m[1][0] m[1][1] 0 | * | _v.y |
    // [ m[2][0] m[2][1] 1 ]   [   0  ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Mat3<TValue>::operator*(Vec2<TValue> const& _v) const
    {
        Vec2<TValue> res;
        for (Size r = 0; r < 2; r++)
        {
            res[r] =
                m[r][0] * _v.x +
                m[r][1] * _v.y;
        }
        return res;
    }

    //////////////////////////////////////////
    //
    //                   [ m[0][0] m[0][1] m[0][2] ]
    // [ _v.x _v.y 0 ] * | m[1][0] m[1][1] m[1][2] |
    //                   [ m[2][0] m[2][1] m[2][2] ]
    // 
    //////////////////////////////////////////
    template <class TValue = F32>
    inline Vec2<TValue> operator*(
        Vec2<TValue> const& _v,
        Mat3<TValue> const& _m)
    {
        Vec2<TValue> res;
        for (Size c = 0; c < 2; c++)
        {
            res[c] =
                _v[0] * _m[0][c] +
                _v[1] * _m[1][c];
        }
        return res;
    }
    

    //////////////////////////////////////////
    //
    // [ m[0][0] m[0][1] m[0][2] ]   [ _v.x ]
    // | m[1][0] m[1][1] m[1][2] | * | _v.y |
    // [ m[2][0] m[2][1] m[2][2] ]   [ _v.z ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline Vec3<TValue> Mat3<TValue>::operator*(Vec3<TValue> const& _v) const
    {
        Vec3<TValue> res;
        for (Size r = 0; r < 3; r++)
        {
            res[r] =
                m[r][0] * _v[0] +
                m[r][1] * _v[1] +
                m[r][2] * _v[2];
        }
        return res;
    }

    //////////////////////////////////////////
    //
    //                      [ m[0][0] m[0][1] m[0][2] ]
    // [ _v.x _v.y _v.z ] * | m[1][0] m[1][1] m[1][2] |
    //                      [ m[2][0] m[2][1] m[2][2] ]
    // 
    //////////////////////////////////////////
    template <class TValue = F32>
    inline Vec3<TValue> operator*(
        Vec3<TValue> const& _v,
        Mat3<TValue> const& _m)
    {
        Vec3<TValue> res;
        for (Size c = 0; c < 3; c++)
        {
            res[c] =
                _v[0] * _m[0][c] +
                _v[1] * _m[1][c] +
                _v[2] * _m[2][c];
        }
        return res;
    }

    //////////////////////////////////////////
    //
    // [ m[0][0] m[0][1] m[0][2] ]   
    // | m[1][0] m[1][1] m[1][2] | * _value
    // [ m[2][0] m[2][1] m[2][2] ]   
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::operator*(TValue _value) const
    {
        Mat3 res;
        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 3; c++)
                res[r][c] = m[r][c] * _value;
        return res;
    }
    
    //////////////////////////////////////////
    //
    //          [ m[0][0] m[0][1] m[0][2] ]
    // _value * | m[1][0] m[1][1] m[1][2] |
    //          [ m[2][0] m[2][1] m[2][2] ]
    // 
    //////////////////////////////////////////
    template <class TValue = F32>
    inline Mat3<TValue> operator*(
        TValue _value,
        Mat3<TValue> const& _m)
    {
        Mat3<TValue> res;
        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 3; c++)
                res[r][c] = _value * _m[r][c];
        return res;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Mat3<TValue>::isAffine() const
    {
        return m[0][2] == 0 && m[1][2] == 0 && m[2][2] == 1;
    }
        
    //////////////////////////////////////////
    //
    // [ m[0][0] m[0][1] 0 ]   [ _m[0][0] _m[0][1] 0 ]
    // | m[1][0] m[1][1] 0 | * | _m[1][0] _m[1][1] 0 |
    // [ m[2][0] m[2][1] 1 ]   [ _m[2][0] _m[2][1] 1 ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffine(
        Mat3<TValue> const& _m,
        Mat3<TValue>& _out) const
    {
        _out[0][0] = m[0][0] * _m[0][0] + m[1][0] * _m[0][1];
        _out[0][1] = m[0][1] * _m[0][0] + m[1][1] * _m[0][1];
        _out[0][2] = 0;

        _out[1][0] = m[0][0] * _m[1][0] + m[1][0] * _m[1][1];
        _out[1][1] = m[0][1] * _m[1][0] + m[1][1] * _m[1][1];
        _out[1][2] = 0;

        _out[2][0] = m[0][0] * _m[2][0] + m[1][0] * _m[2][1] + m[2][0];     
        _out[2][1] = m[0][1] * _m[2][0] + m[1][1] * _m[2][1] + m[2][1];
        _out[2][2] = 1;
    }
        
    //////////////////////////////////////////
    //
    // [ m[0][0] m[0][1] 0 ]   [ _m00 _m01 0 ]
    // | m[1][0] m[1][1] 0 | * | _m10 _m11 0 |
    // [ m[2][0] m[2][1] 1 ]   [ _x   _y   1 ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffine(
        TValue _m00, TValue _m01,
        TValue _m10, TValue _m11,
        TValue _x, TValue _y,
        Mat3<TValue>& _out) const
    {
        _out[0][0] = m[0][0] * _m00 + m[1][0] * _m01;
        _out[0][1] = m[0][1] * _m00 + m[1][1] * _m01;
        _out[0][2] = 0;

        _out[1][0] = m[0][0] * _m10 + m[1][0] * _m11;
        _out[1][1] = m[0][1] * _m10 + m[1][1] * _m11;
        _out[1][2] = 0;

        _out[2][0] = m[0][0] * _x + m[1][0] * _y + m[2][0];
        _out[2][1] = m[0][1] * _x + m[1][1] * _y + m[2][1];      
        _out[2][2] = 1;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::transformAffine(Mat3<TValue> const& _m) const
    {
        Mat3<TValue> res;
        transformAffine(_m, res);
        return res;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineTranslation2DX() const 
    { 
        return m[2][0]; 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineTranslation2DY() const 
    { 
        return m[2][1]; 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Mat3<TValue>::getAffineTranslation2D() const 
    { 
        return Vec2<TValue>(getAffineTranslation2DX(), getAffineTranslation2DY()); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineScale2DXSignless() const
    { 
        return Vec2<TValue>(m[0][0], m[0][1]).length(); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineScale2DYSignless() const
    { 
        return Vec2<TValue>(m[1][0], m[1][1]).length(); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2<TValue> Mat3<TValue>::getAffineScale2DSignless() const
    { 
        return Vec2<TValue>(getAffineScaleXSignless(), getAffineScaleYSignless());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineScaleXSignless() const
    {
        return getRow(0).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineScaleYSignless() const
    {
        return getRow(1).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3<TValue>::getAffineScaleZSignless() const
    {
        return getRow(2).length();
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec3<TValue> Mat3<TValue>::getAffineScaleSignless() const
    {
        return Vec3<TValue>(getAffineScaleXSignless(), getAffineScaleYSignless(), getAffineScaleZSignless());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rotation2D Mat3<TValue>::getAffineRotation2D() const 
    { 
        return Rotation2D((F32)m[0][1] / getAffineScaleXSignless(), (F32)m[0][0] / this->getAffineScaleXSignless());
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2<TValue> Mat3<TValue>::transformAffine(Vec2<TValue> const& _v) const
    {
        Vec2<TValue> res;
        transformAffine(_v.x, _v.y, res.x, res.y);
        return res;
    }
        
    //////////////////////////////////////////
    //
    //                   [ m[0][0] m[0][1] 0 ]
    // [_inX, _inY, 0] * | m[1][0] m[1][1] 0 |
    //                   [ m[2][0] m[2][1] 1 ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffine(
        TValue const& _inX,
        TValue const& _inY,
        TValue& _outX,
        TValue& _outY) const
    {
        _outX = _inX * m[0][0] + _inY * m[1][0] + m[2][0];
        _outY = _inX * m[0][1] + _inY * m[1][1] + m[2][1];
    }

    //////////////////////////////////////////
    //
    //                [ m[0][0] m[0][1] 0 ]
    // [_inX, 0, 0] * | m[1][0] m[1][1] 0 |
    //                [ m[2][0] m[2][1] 1 ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffineX(
        TValue const& _inX, 
        TValue& _outX,
        TValue& _outY) const
    {
        _outX = _inX * m[0][0] + m[2][0];
        _outY = _inX * m[0][1] + m[2][1];
    }

    //////////////////////////////////////////
    //
    //                [ m[0][0] m[0][1] 0 ]
    // [0, _inY, 0] * | m[1][0] m[1][1] 0 |
    //                [ m[2][0] m[2][1] 1 ]
    // 
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffineY(
        TValue const& _inY,
        TValue& _outX,
        TValue& _outY) const
    {
        _outX = _inY * m[1][0] + m[2][0];
        _outY = _inY * m[1][1] + m[2][1];
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffine(
        Vec2<TValue> const& _v,
        TValue& _outX,
        TValue& _outY) const
    {
        transformAffine(_v.x, _v.y, _outX, _outY);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3<TValue>::transformAffine(
        Vec2<TValue> const& _v,
        Vec2<TValue>& _out) const
    {
        transformAffine(_v, _out.x, _out.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat3<TValue>::compareAffine(Mat3 const& _m) const
    {
        if (!Math::Equal<TValue>(m[0][0], _m[0][0], c_epsilon) || !Math::Equal<TValue>(m[0][1], _m[0][1], c_epsilon) ||
            !Math::Equal<TValue>(m[1][0], _m[1][0], c_epsilon) || !Math::Equal<TValue>(m[1][1], _m[1][1], c_epsilon) ||
            !Math::Equal<TValue>(m[2][0], _m[2][0], c_epsilon) || !Math::Equal<TValue>(m[2][1], _m[2][1], c_epsilon))
            return false;
        return true;
    }
    
    
    //////////////////////////////////////////
    template <class TValue>
    Mat3<TValue> Mat3<TValue>::transpose() const
    {
        Mat3<TValue> res;
        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 3; c++)
                res[r][c] = m[c][r];
        return res;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    bool Mat3<TValue>::inverse(Mat3<TValue>& _out, TValue _tolerance) const
    {
        _out[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
        _out[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
        _out[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
        _out[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
        _out[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
        _out[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
        _out[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
        _out[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
        _out[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        
        TValue det =
            m[0][0] * _out[0][0] +
            m[0][1] * _out[1][0] +
            m[0][2] * _out[2][0];
        
        if (Math::Abs(det) <= _tolerance)
            return false;
        
        TValue invDet = 1 / det;
        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 3; c++)
                _out[r][c] *= invDet;
        
        return true;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Mat3<TValue> Mat3<TValue>::inverse(TValue _tolerance) const
    {
        Mat3 res;
        inverse(res, _tolerance);
        return res;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Mat3<TValue> Mat3<TValue>::inverseAffine() const
    {
#pragma warning(push)
#pragma warning(disable : 4146)
        
        Mat3<TValue> matrix;
        
        matrix[0][0] = m[1][1];
        matrix[0][1] = -m[0][1];
        matrix[0][2] = 0;

        matrix[1][0] = -m[1][0];
        matrix[1][1] = m[0][0];
        matrix[1][2] = 0;

        matrix[2][0] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
        matrix[2][1] = m[2][0] * m[0][1] - m[0][0] * m[2][1];
        matrix[2][2] = 1;
        
        TValue det = m[0][0] * matrix[0][0] + m[1][0] * matrix[0][1];
        
        TValue invDet = 1 / det;

        for (Size r = 0; r < 3; r++)
            for (Size c = 0; c < 2; c++)
                matrix[r][c] *= invDet;
        
        return matrix;
        
#pragma warning(pop)
    }
    
    //////////////////////////////////////////
    template <class TValue>
    TValue Mat3<TValue>::determinant() const
    {
        TValue cofactor00 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
        TValue cofactor01 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
        TValue cofactor02 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
        
        TValue det =
            m[0][0] * cofactor00 +
            m[0][1] * cofactor01 +
            m[0][2] * cofactor02;
        
        return det;
        
    }
    
    //////////////////////////////////////////
    template <class TValue>
    inline String Mat3<TValue>::toString(Char _separator) const
    {
        String result;

        for (S32 i = 0; i < 9; ++i)
        {
            if (!result.empty())
                result += _separator;

            result += StringHelper::ToString(planeMatrix[i]);
        }

        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline CString Mat3<TValue>::ParseString(CString _string, Size _size, Mat3& _out, Char _separator)
    {
        CString end = _string + _size;

        S32 i = 0;
        for (; i < 8; ++i)
        {
            _string = StringHelper::ParseNumber<TValue>(_string, end, _out.planeMatrix[i]);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::SkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
        }
        _string = StringHelper::ParseNumber<TValue>(_string, end, _out.planeMatrix[i]);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline CString Mat3<TValue>::ParseStringPretty(CString _string, Size _size, Mat3& _out, Char _separator)
    {
        TValue v;

        CString end = _string + _size;
        _string = StringHelper::ExpectSkipChar(_string, end, '[', 1);
        for (S32 i = 0; i < 3; ++i)
        {
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, '[', 1);
            _string = StringHelper::SkipChar(_string, end, ' ');

            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _out[i][0] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _out[i][1] = v;
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseNumber<TValue>(_string, end, v);
            _out[i][2] = v;

            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, ']', 1);
        }
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, ']', 1);
        return _string;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3<TValue> Mat3<TValue>::FromString(CString _string, Size _size, Char _separator)
    {
        Mat3 result = Mat3::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    template <class TValue>
    Mat3<TValue> Mat3<TValue>::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }

} // namespace Maze
//////////////////////////////////////////
