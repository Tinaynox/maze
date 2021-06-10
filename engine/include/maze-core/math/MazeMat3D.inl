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
    // Class Mat3D
    //
    //////////////////////////////////////////
    template <class TValue>
    const TValue Mat3D<TValue>::c_epsilon = (TValue)1e-05;
    template <class TValue>
    const Mat3D<TValue> Mat3D<TValue>::c_zero(
        0,0,0,
        0,0,0,
        0,0,0);
    template <class TValue>
    const Mat3D<TValue> Mat3D<TValue>::c_identity(
        1,0,0,
        0,1,0,
        0,0,1);

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>::Mat3D()
        : planeMatrix{ 0 }
    {
    }
    
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>::Mat3D(TValue const _arr[3][3])
    {
        memcpy(m, _arr, 9 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>::Mat3D(Mat3D const& _matrix)
    {
        memcpy(m, _matrix.m, 9 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>::Mat3D(Mat3D&& _matrix)
    {
        memcpy(m, _matrix.m, 9 * sizeof(TValue));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>::Mat3D(    
        TValue _value00, TValue _value01, TValue _value02,
        TValue _value10, TValue _value11, TValue _value12,
        TValue _value20, TValue _value21, TValue _value22)
    {
        set(_value00, _value01, _value02,
            _value10, _value11, _value12,
            _value20, _value21, _value22);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::set(    
        TValue _value00, TValue _value01, TValue _value02,
        TValue _value10, TValue _value11, TValue _value12,
        TValue _value20, TValue _value21, TValue _value22)
    {
        m[0][0] = _value00;
        m[0][1] = _value01;
        m[0][2] = _value02;
        m[1][0] = _value10;
        m[1][1] = _value11;
        m[1][2] = _value12;
        m[2][0] = _value20;
        m[2][1] = _value21;
        m[2][2] = _value22;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::setAffine(    
        TValue _value00, TValue _value01, TValue _value02,
        TValue _value10, TValue _value11, TValue _value12)
    {
        m[0][0] = _value00;
        m[0][1] = _value01;
        m[0][2] = _value02;
        m[1][0] = _value10;
        m[1][1] = _value11;
        m[1][2] = _value12;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateTranslationMatrix(TValue _x, TValue _y)
    {
        return Mat3D<TValue>(
            1, 0, _x,
            0, 1, _y,
            0, 0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateTranslationMatrix(
        Vec2D<TValue> const& _vector)
    {
        return CreateTranslationMatrix(_vector.x, _vector.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateRotationMatrix(F32 _s, F32 _c)
    {
        return Mat3D<TValue>(
                (TValue)_c,    (TValue)-_s,(TValue)0,
                (TValue)_s,    (TValue)_c,    (TValue)0,
                (TValue)0,    (TValue)0,    (TValue)1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateRotationMatrix(F32 _angle)
    {
        return CreateRotationMatrix(sinf(_angle), cosf(_angle));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateScaleMatrix(TValue _x, TValue _y)
    {
        return Mat3D<TValue>(
            _x, 0, 0,
            0, _y, 0,
            0, 0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateScaleMatrix(
        Vec2D<TValue> const& _vector)
    {
        return CreateScaleMatrix(_vector.x, _vector.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateAffineTransformMatrix(
        const Vec2DF& _pos,
        const Rotation2D& _rotation,
        const Vec2DF& _scale)
    {
        return Mat3D<TValue>(
            (TValue)(_rotation.getCos() * _scale.x),    (TValue)(-_rotation.getSin() * _scale.y),        (TValue)(_pos.x),
            (TValue)(_rotation.getSin() * _scale.x),    (TValue)(_rotation.getCos() * _scale.y),        (TValue)(_pos.y),
            (TValue)(0),                                (TValue)(0),                                    (TValue)(1));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Mat3D<TValue> Mat3D<TValue>::CreateAffineTransformMatrix(
        const Vec2DF& _pos,
        F32 _angle,
        const Vec2DF& _scale)
    {
        return CreateAffineTransformMatrix(_pos, Rotation2D(_angle), _scale);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::CreateAffineTransformMatrix(
        const Vec2DF& _pos,
        const Rotation2D& _rotation,
        const Vec2DF& _scale,
        const Vec2DF& _pivot,
        const Vec2DF& _size)
    {
        F32 sax = _size.x * _pivot.x;
        F32 say = _size.y * _pivot.y;

        return Mat3D<TValue>(
            (TValue)(_rotation.getCos() * _scale.x),    (TValue)(-_rotation.getSin() * _scale.y),    (TValue)(-_rotation.getCos() * _scale.x * sax + _rotation.getSin() * _scale.y * say + _pos.x),
            (TValue)(_rotation.getSin() * _scale.x),    (TValue)(_rotation.getCos() * _scale.y),    (TValue)(-_rotation.getSin() * _scale.x * sax - _rotation.getCos() * _scale.y * say + _pos.y),
            (TValue)(0),                                (TValue)(0),                                (TValue)(1));
    }

    //////////////////////////////////////////
    template <class TValue>
    inline const TValue* Mat3D<TValue>::getPlaneMatrix() const { return planeMatrix; }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::swap(Mat3D<TValue>& _other)
    {
        std::swap(m[0][0], _other.m[0][0]);
        std::swap(m[0][1], _other.m[0][1]);
        std::swap(m[0][2], _other.m[0][2]);
        std::swap(m[1][0], _other.m[1][0]);
        std::swap(m[1][1], _other.m[1][1]);
        std::swap(m[1][2], _other.m[1][2]);
        std::swap(m[2][0], _other.m[2][0]);
        std::swap(m[2][1], _other.m[2][1]);
        std::swap(m[2][2], _other.m[2][2]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline TValue* Mat3D<TValue>::operator[](Size _row) const
    {
        return (TValue*)m[_row];
    }
   
    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue> Mat3D<TValue>::getColumn(Size _column) const
    {
        return Vec3D<TValue>(m[0][_column], m[1][_column], m[2][_column]);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::setColumn(Size _column, Vec3D<TValue> const& _vec)
    {
        m[0][_column] = _vec.x;
        m[1][_column] = _vec.y;
        m[2][_column] = _vec.z;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::setAxes(
        const Vec3D<TValue>& _xAxis,
        const Vec3D<TValue>& _yAxis,
        const Vec3D<TValue>& _zAxis)
    {
        setColumn(0, _xAxis);
        setColumn(1, _yAxis);
        setColumn(2, _zAxis);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>& Mat3D<TValue>::operator=(Mat3D<TValue> const& _matrix)
    {
        memcpy(m, _matrix.m, 9 * sizeof(TValue));
        return *this;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue>& Mat3D<TValue>::operator=(Mat3D<TValue>&& _matrix)
    {
        memcpy(m, _matrix.m, 9 * sizeof(TValue));
        return *this;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat3D<TValue>::operator==(Mat3D<TValue> const& _m2) const
    {
        if (
           m[0][0] != _m2.m[0][0] || m[0][1] != _m2.m[0][1] || m[0][2] != _m2.m[0][2] ||
           m[1][0] != _m2.m[1][0] || m[1][1] != _m2.m[1][1] || m[1][2] != _m2.m[1][2] ||
           m[2][0] != _m2.m[2][0] || m[2][1] != _m2.m[2][1] || m[2][2] != _m2.m[2][2])
            return false;

        return true;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat3D<TValue>::operator!=(Mat3D<TValue> const& _m2) const
    {
        if (
            m[0][0] != _m2.m[0][0] || m[0][1] != _m2.m[0][1] || m[0][2] != _m2.m[0][2] ||
            m[1][0] != _m2.m[1][0] || m[1][1] != _m2.m[1][1] || m[1][2] != _m2.m[1][2] ||
            m[2][0] != _m2.m[2][0] || m[2][1] != _m2.m[2][1] || m[2][2] != _m2.m[2][2])
            return true;
        return false;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::operator+(Mat3D<TValue> const& _matrix) const
    {
        Mat3D<TValue> sum;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
            {
                sum.m[r][c] = m[r][c] + _matrix.m[r][c];
            }
        }
        return sum;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::operator-(Mat3D<TValue> const& _matrix) const
    {
        Mat3D<TValue> diff;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
            {
                diff.m[r][c] = m[r][c] - _matrix.m[r][c];
            }
        }
        return diff;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::operator*(Mat3D<TValue> const& _matrix) const
    {
        Mat3D<TValue> prod;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
            {
                prod.m[r][c] =
                    m[r][0] * _matrix.m[0][c] +
                    m[r][1] * _matrix.m[1][c] +
                    m[r][2] * _matrix.m[2][c];
            }
        }
        return prod;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::operator-() const
    {
        #pragma warning(push)
        #pragma warning(disable : 4146)

        Mat3D<TValue> neg;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
                neg[r][c] = -m[r][c];
        }
        return neg;

        #pragma warning(pop)
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2D<TValue> Mat3D<TValue>::operator*(Vec2D<TValue> const& _vec) const
    {
        Vec2D<TValue> prod;
        for (Size r = 0; r < 2; r++)
        {
            prod[r] =
                m[r][0] * _vec[0] +
                m[r][1] * _vec[1];
        }
        return prod;
    }

    

    //////////////////////////////////////////
    template <class TValue>
    inline Vec3D<TValue> Mat3D<TValue>::operator*(Vec3D<TValue> const& _vec) const
    {
        Vec3D<TValue> prod;
        for (Size r = 0; r < 3; r++)
        {
            prod[r] =
                m[r][0] * _vec[0] +
                m[r][1] * _vec[1] +
                m[r][2] * _vec[2];
        }
        return prod;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::operator*(TValue _value) const
    {
        Mat3D prod;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
                prod[r][c] = _value * m[r][c];
        }
        return prod;
    }
    

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR bool Mat3D<TValue>::isAffine() const
    {
        return m[2][0] == 0 && m[2][1] == 0 && m[2][2] == 1;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::concatenateAffine(
        Mat3D<TValue> const& _m2,
        Mat3D<TValue>& _out) const
    {
        _out[0][0] = m[0][0] * _m2.m[0][0] + m[0][1] * _m2.m[1][0];
        _out[0][1] = m[0][0] * _m2.m[0][1] + m[0][1] * _m2.m[1][1];
        _out[0][2] = m[0][0] * _m2.m[0][2] + m[0][1] * _m2.m[1][2]  + m[0][2];

        _out[1][0] = m[1][0] * _m2.m[0][0] + m[1][1] * _m2.m[1][0];
        _out[1][1] = m[1][0] * _m2.m[0][1] + m[1][1] * _m2.m[1][1];
        _out[1][2] = m[1][0] * _m2.m[0][2] + m[1][1] * _m2.m[1][2] + m[1][2];

        _out[2][0] = 0;
        _out[2][1] = 0;
        _out[2][2] = 1;
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::concatenateAffine(
        TValue _value00, TValue _value01, TValue _value02,
        TValue _value10, TValue _value11, TValue _value12,
        Mat3D<TValue>& _out) const
    {
        _out[0][0] = m[0][0] * _value00 + m[0][1] * _value10;
        _out[0][1] = m[0][0] * _value01 + m[0][1] * _value11;
        _out[0][2] = m[0][0] * _value02 + m[0][1] * _value12  + m[0][2];

        _out[1][0] = m[1][0] * _value00 + m[1][1] * _value10;
        _out[1][1] = m[1][0] * _value01 + m[1][1] * _value11;
        _out[1][2] = m[1][0] * _value02 + m[1][1] * _value12 + m[1][2];

        _out[2][0] = 0;
        _out[2][1] = 0;
        _out[2][2] = 1;
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Mat3D<TValue> Mat3D<TValue>::concatenatedAffineCopy(Mat3D<TValue> const& _m2) const
    {
        return Mat3D(
            m[0][0] * _m2.m[0][0] + m[0][1] * _m2.m[1][0],
            m[0][0] * _m2.m[0][1] + m[0][1] * _m2.m[1][1],
            m[0][0] * _m2.m[0][2] + m[0][1] * _m2.m[1][2]  + m[0][2],

            m[1][0] * _m2.m[0][0] + m[1][1] * _m2.m[1][0],
            m[1][0] * _m2.m[0][1] + m[1][1] * _m2.m[1][1],
            m[1][0] * _m2.m[0][2] + m[1][1] * _m2.m[1][2] + m[1][2],

            0, 0, 1);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3D<TValue>::getAffineTranslationX() const 
    { 
        return m[0][2]; 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3D<TValue>::getAffineTranslationY() const 
    { 
        return m[1][2]; 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> Mat3D<TValue>::getAffineTranslation() const 
    { 
        return Vec2D<TValue>(getAffineTranslationX(), getAffineTranslationY()); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3D<TValue>::getAffineScaleX() const 
    { 
        return Vec2D<TValue>(m[0][0], m[1][0]).length(); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR TValue Mat3D<TValue>::getAffineScaleY() const 
    { 
        return Vec2D<TValue>(m[0][1], m[1][1]).length(); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Vec2D<TValue> Mat3D<TValue>::getAffineScale() const
    { 
        return Vec2D<TValue>(getAffineScaleX(), getAffineScaleY()); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline MAZE_CONSTEXPR Rotation2D Mat3D<TValue>::getAffineRotation() const 
    { 
        return Rotation2D((F32)m[1][0] / getAffineScaleX(), (F32)m[0][0] / getAffineScaleX()); 
    }

    //////////////////////////////////////////
    template <class TValue>
    inline Vec2D<TValue> Mat3D<TValue>::transformAffine(Vec2D<TValue> const& _v) const
    {
        return Vec2D<TValue>(
                m[0][0] * _v.x + m[0][1] * _v.y + m[0][2], 
                m[1][0] * _v.x + m[1][1] * _v.y + m[1][2]);
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::transformAffine(
        TValue const& _inX,
        TValue const& _inY,
        TValue& _outX,
        TValue& _outY) const
    {
        _outX = m[0][0] * _inX + m[0][1] * _inY + m[0][2];
        _outY = m[1][0] * _inX + m[1][1] * _inY + m[1][2];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::transformAffineX(
        TValue const& _inX, 
        TValue& _outX,
        TValue& _outY) const
    {
        _outX = m[0][0] * _inX + m[0][2];
        _outY = m[1][0] * _inX + m[1][2];
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::transformAffineY(
        TValue const& _inY,
        TValue& _outX,
        TValue& _outY) const
    {
        _outX = m[0][1] * _inY + m[0][2];
        _outY = m[1][1] * _inY + m[1][2];
    }
        
    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::transformAffine(
        Vec2D<TValue> const& _v,
        TValue& _outX,
        TValue& _outY) const
    {
        transformAffine(_v.x, _v.y, _outX, _outY);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline void Mat3D<TValue>::transformAffine(
        Vec2D<TValue> const& _v,
        Vec2D<TValue>& _out) const
    {
        transformAffine(_v, _out.x, _out.y);
    }

    //////////////////////////////////////////
    template <class TValue>
    inline bool Mat3D<TValue>::compareAffine(Mat3D const& _m2) const
    {
        if (
            !Math::Equal<TValue>(m[0][0], _m2.m[0][0], c_epsilon) || !Math::Equal<TValue>(m[0][1], _m2.m[0][1], c_epsilon) || !Math::Equal<TValue>(m[0][2], _m2.m[0][2], c_epsilon) ||
            !Math::Equal<TValue>(m[1][0], _m2.m[1][0], c_epsilon) || !Math::Equal<TValue>(m[1][1], _m2.m[1][1], c_epsilon) || !Math::Equal<TValue>(m[1][2], _m2.m[1][2], c_epsilon))
            return false;
        return true;
    }
    
    
    //////////////////////////////////////////
    template <class TValue>
    Mat3D<TValue> Mat3D<TValue>::transpose() const
    {
        Mat3D<TValue> transpose;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
                transpose[r][c] = m[c][r];
        }
        return transpose;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    bool Mat3D<TValue>::inverse(Mat3D<TValue>& _matrix, TValue _tolerance) const
    {
        _matrix[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];
        _matrix[0][1] = m[0][2]*m[2][1] - m[0][1]*m[2][2];
        _matrix[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
        _matrix[1][0] = m[1][2]*m[2][0] - m[1][0]*m[2][2];
        _matrix[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];
        _matrix[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
        _matrix[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];
        _matrix[2][1] = m[0][1]*m[2][0] - m[0][0]*m[2][1];
        _matrix[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0];
        
        TValue det =
        m[0][0]*_matrix[0][0] +
        m[0][1]*_matrix[1][0] +
        m[0][2]*_matrix[2][0];
        
        if (Abs(det) <= _tolerance)
            return false;
        
        TValue invDet = 1 / det;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
                _matrix[r][c] *= invDet;
        }
        
        return true;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Mat3D<TValue> Mat3D<TValue>::inverse(TValue _tolerance) const
    {
        Mat3D matrix;
        inverse(matrix, _tolerance);
        return matrix;
    }
    
    //////////////////////////////////////////
    template <class TValue>
    Mat3D<TValue> Mat3D<TValue>::inverseAffine() const
    {
        #pragma warning(push)
        #pragma warning(disable : 4146)
        
        Mat3D<TValue> matrix;
        
        matrix[0][0] = m[1][1];
        matrix[0][1] = -m[0][1];
        matrix[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
        matrix[1][0] = -m[1][0];
        matrix[1][1] = m[0][0];
        matrix[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
        matrix[2][0] = 0;
        matrix[2][1] = 0;
        matrix[2][2] = 1;
        
        TValue det = m[0][0] * matrix[0][0] + m[0][1] * matrix[1][0];
        
        TValue invDet = 1 / det;

        for (Size r = 0; r < 2; r++)
            for (Size c = 0; c < 3; c++)
                matrix[r][c] *= invDet;
        
        return matrix;
        
        #pragma warning(pop)
    }
    
    //////////////////////////////////////////
    template <class TValue>
    TValue Mat3D<TValue>::determinant() const
    {
        TValue cofactor00 = m[1][1]*m[2][2] - m[1][2]*m[2][1];
        TValue cofactor10 = m[1][2]*m[2][0] - m[1][0]*m[2][2];
        TValue cofactor20 = m[1][0]*m[2][1] - m[1][1]*m[2][0];
        
        TValue det =
        m[0][0] * cofactor00 +
        m[0][1] * cofactor10 +
        m[0][2] * cofactor20;
        
        return det;
        
    }
    
    //////////////////////////////////////////
    // Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
    //
    //////////////////////////////////////////
    template <class TValue>
    void Mat3D<TValue>::orthonormalize()
    {
        // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
        // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
        //
        //   q0 = m0/|m0|
        //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
        //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
        //
        // where |V| indicates length of vector V and A*B indicates dot
        // product of vectors A and B.
        
        // compute q0
        TValue invLength = InvSqrt(m[0][0]*m[0][0] + m[1][0]*m[1][0] + m[2][0]*m[2][0]);
        
        m[0][0] *= invLength;
        m[1][0] *= invLength;
        m[2][0] *= invLength;
        
        // compute q1
        TValue dot0 =
        m[0][0]*m[0][1] +
        m[1][0]*m[1][1] +
        m[2][0]*m[2][1];
        
        m[0][1] -= dot0*m[0][0];
        m[1][1] -= dot0*m[1][0];
        m[2][1] -= dot0*m[2][0];
        
        invLength = InvSqrt(m[0][1]*m[0][1] + m[1][1]*m[1][1] +    m[2][1]*m[2][1]);
        
        m[0][1] *= invLength;
        m[1][1] *= invLength;
        m[2][1] *= invLength;
        
        // compute q2
        TValue dot1 =
        m[0][1]*m[0][2] +
        m[1][1]*m[1][2] +
        m[2][1]*m[2][2];
        
        dot0 =
        m[0][0]*m[0][2] +
        m[1][0]*m[1][2] +
        m[2][0]*m[2][2];
        
        m[0][2] -= dot0*m[0][0] + dot1*m[0][1];
        m[1][2] -= dot0*m[1][0] + dot1*m[1][1];
        m[2][2] -= dot0*m[2][0] + dot1*m[2][1];
        
        invLength = InvSqrt(m[0][2]*m[0][2] + m[1][2]*m[1][2] +    m[2][2]*m[2][2]);
        
        m[0][2] *= invLength;
        m[1][2] *= invLength;
        m[2][2] *= invLength;
        
    }

    

} // namespace Maze
//////////////////////////////////////////
