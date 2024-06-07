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
#if (!defined(_MazeMat3_hpp_))
#define _MazeMat3_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include <ostream>


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
    template <class TValue = F32>
    class Mat3
    {
    public:
        
        //////////////////////////////////////////
        template <class UValue>
        friend class Mat4;

        //////////////////////////////////////////
        static const TValue c_epsilon;
        static const Mat3<TValue> c_zero;
        static const Mat3<TValue> c_identity;

    public:

        //////////////////////////////////////////
        inline Mat3();
        
        //////////////////////////////////////////
        inline explicit Mat3(TValue const _arr[3][3]);

        //////////////////////////////////////////
        inline Mat3(Mat3 const& _matrix);
        
        //////////////////////////////////////////
        inline Mat3(Mat3&& _matrix);

        //////////////////////////////////////////
        inline Mat3(
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
            TValue _value00, TValue _value01,
            TValue _value10, TValue _value11,
            TValue _x, TValue _y);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateTranslationMatrix(TValue _x, TValue _y);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateTranslationMatrix(Vec2<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateRotationMatrix(F32 _s, F32 _c);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateRotationMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateScaleMatrix(TValue _x, TValue _y);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateScaleMatrix(TValue _x, TValue _y, TValue _z);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateScaleMatrix(Vec2<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateScaleMatrix(Vec3<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateAffineTransformMatrix(
            const Vec2F& _pos,
            const Rotation2D& _rotation,
            const Vec2F& _scale);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat3 CreateAffineTransformMatrix(
            const Vec2F& _pos,
            F32 _angle,
            const Vec2F& _scale);

        //////////////////////////////////////////
        static inline Mat3 CreateAffineTransformMatrix(
            const Vec2F& _pos,
            const Rotation2D& _rotation,
            const Vec2F& _scale,
            const Vec2F& _pivot,
            const Vec2F& _size);

        //////////////////////////////////////////
        inline const TValue* getPlaneMatrix() const;

        //////////////////////////////////////////
        inline void swap(Mat3& _other);

        //////////////////////////////////////////
        inline Vec3<TValue> getRow(Size _row) const;

        //////////////////////////////////////////
        inline void setRow(Size _row, Vec3<TValue> const& _vec);

        //////////////////////////////////////////
        inline TValue* operator[](Size _row) const;

        //////////////////////////////////////////
        inline Vec3<TValue> getColumn(Size _column) const;

        //////////////////////////////////////////
        inline void setColumn(Size column, Vec3<TValue> const& _vec);

        //////////////////////////////////////////
        inline void setAxes(
            Vec3<TValue> const& _yAxis,
            Vec3<TValue> const& _xAxis,
            Vec3<TValue> const& _zAxis);

        //////////////////////////////////////////
        inline Mat3& operator=(Mat3 const& _matrix);

        //////////////////////////////////////////
        inline Mat3<TValue>& operator=(Mat3<TValue>&& _matrix);

        //////////////////////////////////////////
        inline bool operator==(Mat3<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline bool operator!=(Mat3<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline Mat3<TValue> operator+(Mat3<TValue> const& _matrix) const;

        //////////////////////////////////////////
        inline Mat3<TValue> operator-(Mat3<TValue> const& _matrix) const;

        //////////////////////////////////////////
        inline Mat3<TValue> operator*(Mat3<TValue> const& _matrix) const;

        //////////////////////////////////////////
        inline Mat3<TValue> operator-() const;

        //////////////////////////////////////////
        inline Vec2<TValue> operator*(Vec2<TValue> const& _vec) const;

        //////////////////////////////////////////
        inline Vec3<TValue> operator*(Vec3<TValue> const& _vec) const;

        //////////////////////////////////////////
        inline Mat3<TValue> operator*(TValue _value) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isAffine() const;

        //////////////////////////////////////////
        inline void concatenateAffine(Mat3<TValue> const& _m2, Mat3<TValue>& _out) const;

        //////////////////////////////////////////
        inline void concatenateAffine(
            TValue _value00, TValue _value01,
            TValue _value10, TValue _value11,
            TValue _x, TValue _y,
            Mat3<TValue>& _out) const;

        //////////////////////////////////////////
        inline Mat3<TValue> concatenatedAffineCopy(Mat3<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineTranslation2DX() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineTranslation2DY() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2<TValue> getAffineTranslation2D() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScale2DXSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScale2DYSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2<TValue> getAffineScale2DSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleXSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleYSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleZSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3<TValue> getAffineScaleSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rotation2D getAffineRotation2D() const;

        //////////////////////////////////////////
        inline Vec2<TValue> transformAffine(Vec2<TValue> const& _v) const;

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
            Vec2<TValue> const& _v,
            TValue& _outX,
            TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffine(
            Vec2<TValue> const& _v,
            Vec2<TValue>& _out) const;
        
        //////////////////////////////////////////
        inline bool compareAffine(Mat3 const& _m2) const;



        //////////////////////////////////////////
        Mat3<TValue> transpose() const;

        //////////////////////////////////////////
        bool inverse(Mat3<TValue>& _matrix, TValue _tolerance = 1e-06) const;

        //////////////////////////////////////////
        Mat3<TValue> inverse(TValue _tolerance = 1e-06) const;

        //////////////////////////////////////////
        Mat3 inverseAffine() const;

        //////////////////////////////////////////
        TValue determinant() const;


        //////////////////////////////////////////
        inline String toString(Char _separator = ',') const;

        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, Mat3& _result, Char _separator = ',');

        //////////////////////////////////////////
        static inline CString ParseStringPretty(CString _string, Size _size, Mat3& _result, Char _separator = ',');

        //////////////////////////////////////////
        static inline Mat3 FromString(CString _string, Size _size, Char _separator = ',');

        //////////////////////////////////////////
        static Mat3 FromString(String const& _string, Char _separator = ',');

    public:

        union 
        {
            TValue m[3][3];
            TValue planeMatrix[9];
        };
    }; 


    //////////////////////////////////////////
    template <typename>
    struct IsMat3 : std::false_type {};
    template <class TValue>
    struct IsMat3<Maze::Mat3<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsMat3<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsMat3<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(_data, _count);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsMat3<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        _outSize = sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsMat3<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsMat3<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Mat3S32 = Mat3<S32>;
    using Mat3U32 = Mat3<U32>;
    using Mat3F32 = Mat3<F32>;


    //////////////////////////////////////////
    // Aliases
    using Mat3S = Mat3S32;
    using Mat3U = Mat3U32;
    using Mat3F = Mat3F32;


    //////////////////////////////////////////
    template <class TValue = F32>
    inline std::ostream& operator<<(
        std::ostream& _o,
        Mat3<TValue> const& _mat)
    {
        _o << "Mat3(" << _mat[0][0] << ", " << _mat[0][1] << ", " << _mat[0][2] << ", " 
                      << _mat[1][0] << ", " << _mat[1][1] << ", " << _mat[1][2] << ", " 
                      << _mat[2][0] << ", " << _mat[2][1] << ", " << _mat[2][2] << ")";
        return _o;
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeMat3.inl"


#endif // _MazeMat3_hpp_
//////////////////////////////////////////
