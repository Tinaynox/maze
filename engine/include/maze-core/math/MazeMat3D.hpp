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
#pragma once
#if (!defined(_MazeMat3D_hpp_))
#define _MazeMat3D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Mat3D
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Mat3D
    {
    public:
        
        //////////////////////////////////////////
        template <class UValue>
        friend class Mat4D;

        //////////////////////////////////////////
        static const TValue c_epsilon;
        static const Mat3D<TValue> c_zero;
        static const Mat3D<TValue> c_identity;

    public:

        //////////////////////////////////////////
        inline Mat3D();
        
        //////////////////////////////////////////
        inline explicit Mat3D(TValue const _arr[3][3]);

        //////////////////////////////////////////
        inline Mat3D(Mat3D const& _matrix);
        
        //////////////////////////////////////////
        inline Mat3D(Mat3D&& _matrix);

        //////////////////////////////////////////
        inline Mat3D(
            TValue _value00, TValue _value01, TValue _value02,
            TValue _value10, TValue _value11, TValue _value12,
            TValue _value20, TValue _value21, TValue _value22);

        //////////////////////////////////////////
        inline void set(
            TValue _value00, TValue _value01, TValue _value02,
            TValue _value10, TValue _value11, TValue _value12,
            TValue _value20, TValue _value21, TValue _value22);

        //////////////////////////////////////////
        inline void setAffine(
            TValue _value00, TValue _value01, TValue _value02,
            TValue _value10, TValue _value11, TValue _value12);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateTranslationMatrix(TValue _x, TValue _y);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateTranslationMatrix(Vec2D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateRotationMatrix(F32 _s, F32 _c);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateRotationMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateScaleMatrix(TValue _x, TValue _y);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateScaleMatrix(TValue _x, TValue _y, TValue _z);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateScaleMatrix(Vec2D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateScaleMatrix(Vec3D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateAffineTransformMatrix(
            const Vec2DF& _pos,
            const Rotation2D& _rotation,
            const Vec2DF& _scale);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3D CreateAffineTransformMatrix(
            const Vec2DF& _pos,
            F32 _angle,
            const Vec2DF& _scale);

        //////////////////////////////////////////
        static inline Mat3D CreateAffineTransformMatrix(
            const Vec2DF& _pos,
            const Rotation2D& _rotation,
            const Vec2DF& _scale,
            const Vec2DF& _pivot,
            const Vec2DF& _size);

        //////////////////////////////////////////
        inline const TValue* getPlaneMatrix() const;

        //////////////////////////////////////////
        inline void swap(Mat3D& _other);

        //////////////////////////////////////////
        inline TValue* operator[](Size _row) const;

        //////////////////////////////////////////
        inline Vec3D<TValue> getColumn(Size _column) const;

        //////////////////////////////////////////
        inline void setColumn(Size column, Vec3D<TValue> const& _vec);

        //////////////////////////////////////////
        inline void setAxes(
            Vec3D<TValue> const& _yAxis,
            Vec3D<TValue> const& _xAxis,
            Vec3D<TValue> const& _zAxis);

        //////////////////////////////////////////
        inline Mat3D& operator=(Mat3D const& _matrix);

        //////////////////////////////////////////
        inline Mat3D<TValue>& operator=(Mat3D<TValue>&& _matrix);

        //////////////////////////////////////////
        inline bool operator==(Mat3D<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline bool operator!=(Mat3D<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline Mat3D<TValue> operator+(Mat3D<TValue> const& _matrix) const;

        //////////////////////////////////////////
        inline Mat3D<TValue> operator-(Mat3D<TValue> const& _matrix) const;

        //////////////////////////////////////////
        inline Mat3D<TValue> operator*(Mat3D<TValue> const& _matrix) const;

        //////////////////////////////////////////
        inline Mat3D<TValue> operator-() const;

        //////////////////////////////////////////
        inline Vec2D<TValue> operator*(Vec2D<TValue> const& _vec) const;

        //////////////////////////////////////////
        inline Vec3D<TValue> operator*(Vec3D<TValue> const& _vec) const;

        //////////////////////////////////////////
        inline Mat3D<TValue> operator*(TValue _value) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isAffine() const;

        //////////////////////////////////////////
        inline void concatenateAffine(Mat3D<TValue> const& _m2, Mat3D<TValue>& _out) const;

        //////////////////////////////////////////
        inline void concatenateAffine(
            TValue _value00, TValue _value01, TValue _value02,
            TValue _value10, TValue _value11, TValue _value12,
            Mat3D<TValue>& _out) const;

        //////////////////////////////////////////
        inline Mat3D<TValue> concatenatedAffineCopy(Mat3D<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineTranslation2DX() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineTranslation2DY() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D<TValue> getAffineTranslation2D() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScale2DXSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScale2DYSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D<TValue> getAffineScale2DSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleXSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleYSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleZSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D<TValue> getAffineScaleSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rotation2D getAffineRotation2D() const;

        //////////////////////////////////////////
        inline Vec2D<TValue> transformAffine(Vec2D<TValue> const& _v) const;

        //////////////////////////////////////////
        inline void transformAffine(
            TValue const& _inX,
            TValue const& _inY,
            TValue& _outX,
            TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffineX(
            TValue const& _inX,
            TValue& _outX,
            TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffineY(
            TValue const& _inY,
            TValue& _outX,
            TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffine(
            Vec2D<TValue> const& _v,
            TValue& _outX,
            TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffine(
            Vec2D<TValue> const& _v,
            Vec2D<TValue>& _out) const;
        
        //////////////////////////////////////////
        inline bool compareAffine(Mat3D const& _m2) const;



        //////////////////////////////////////////
        Mat3D<TValue> transpose() const;

        //////////////////////////////////////////
        bool inverse(Mat3D<TValue>& _matrix, TValue _tolerance = 1e-06) const;

        //////////////////////////////////////////
        Mat3D<TValue> inverse(TValue _tolerance = 1e-06) const;

        //////////////////////////////////////////
        Mat3D inverseAffine() const;

        //////////////////////////////////////////
        TValue determinant() const;

        //////////////////////////////////////////
        void orthonormalize();


        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, Mat3D& _result, Char _separator = ';');

        //////////////////////////////////////////
        static inline CString ParseStringPretty(CString _string, Size _size, Mat3D& _result, Char _separator = ',');

        //////////////////////////////////////////
        static inline Mat3D FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Mat3D FromString(String const& _string, Char _separator = ';');

    public:

        union 
        {
            TValue m[3][3];
            TValue planeMatrix[9];
        };
    }; 


    //////////////////////////////////////////
    template <typename>
    struct IsMat3D : std::false_type {};
    template <class TValue>
    struct IsMat3D<Maze::Mat3D<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat3D<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat3D<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat3D<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat3D<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat3D<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Mat3DS = Mat3D<S32>;
    using Mat3DU = Mat3D<U32>;
    using Mat3DF = Mat3D<F32>;


    //////////////////////////////////////////
    template <class TValue = F32>
    inline Vec2D<TValue> operator*(
        Vec2D<TValue> const& _vec,
        Mat3D<TValue> const& _matrix)
    {
        Vec2D<TValue> prod;
        for (Size r = 0; r < 2; r++)
        {
            prod[r] =
                _vec[0] * _matrix.m[0][r] +
                _vec[1] * _matrix.m[1][r];
        }
        return prod;
    }

    //////////////////////////////////////////
    template <class TValue = F32>
    inline Vec3D<TValue> operator*(
        Vec3D<TValue> const& _vec,
        Mat3D<TValue> const& _matrix)
    {
        Vec3D<TValue> prod;
        for (Size r = 0; r < 3; r++)
        {
            prod[r] =
                _vec[0] * _matrix.m[0][r] +
                _vec[1] * _matrix.m[1][r] +
                _vec[2] * _matrix.m[2][r];
        }
        return prod;
    }

    //////////////////////////////////////////
    template <class TValue = F32>
    inline Mat3D<TValue> operator*(
        TValue _value,
        Mat3D<TValue> const& _matrix)
    {
        Mat3D<TValue> prod;
        for (Size r = 0; r < 3; r++)
        {
            for (Size c = 0; c < 3; c++)
                prod[r][c] = _value * _matrix.m[r][c];
        }
        return prod;
    }

    //////////////////////////////////////////
    template <class TValue = F32>
    inline std::ostream& operator<<(
        std::ostream& _o,
        Mat3D<TValue> const& _mat)
    {
        _o << "Mat3D(" << _mat[0][0] << ", " << _mat[0][1] << ", " << _mat[0][2] << ", " 
                       << _mat[1][0] << ", " << _mat[1][1] << ", " << _mat[1][2] << ", " 
                       << _mat[2][0] << ", " << _mat[2][1] << ", " << _mat[2][2] << ")";
        return _o;
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeMat3D.inl"


#endif // _MazeMat3D_hpp_
//////////////////////////////////////////
