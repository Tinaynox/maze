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
#if (!defined(_MazeMat4_hpp_))
#define _MazeMat4_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Mat4
    //
    //
    // [ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
    // | m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
    // | m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
    // [ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
    //
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Mat4
    {
    public:

        //////////////////////////////////////////
        static const TValue c_epsilon;
        static const Mat4<TValue> c_zero;
        static const Mat4<TValue> c_identity;

    public:

        //////////////////////////////////////////
        inline Mat4();

        //////////////////////////////////////////
        inline explicit Mat4(TValue const _arr[4][4]);

        //////////////////////////////////////////
        inline Mat4(Mat4 const& _matrix);

        //////////////////////////////////////////
        inline Mat4(Mat3<TValue> const& _mat3);

        //////////////////////////////////////////
        inline Mat4(    
            TValue _value00, TValue _value01, TValue _value02, TValue _value03,
            TValue _value10, TValue _value11, TValue _value12, TValue _value13,
            TValue _value20, TValue _value21, TValue _value22, TValue _value23,
            TValue _value30, TValue _value31, TValue _value32, TValue _value33);

        //////////////////////////////////////////
        inline void set(    
            TValue _value00, TValue _value01, TValue _value02, TValue _value03,
            TValue _value10, TValue _value11, TValue _value12, TValue _value13,
            TValue _value20, TValue _value21, TValue _value22, TValue _value23,
            TValue _value30, TValue _value31, TValue _value32, TValue _value33);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateChangeOfBasisMatrix(
            Vec3<TValue> const& _xBasis,
            Vec3<TValue> const& _yBasis,
            Vec3<TValue> const& _zBasis);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateTranslationMatrix(TValue _x, TValue _y, TValue _z = 0);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateTranslationMatrix(Vec2<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateTranslationMatrix(Vec3<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateRotationZMatrix(Rotation2D const& _rotation);

        //////////////////////////////////////////
        static inline Mat4 CreateRotationXMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline Mat4 CreateRotationYMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline Mat4 CreateRotationZMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline Mat4 CreateRotationMatrix(F32 _angleX, F32 _angleY, F32 _angleZ);

        //////////////////////////////////////////
        static inline Mat4 CreateRotationMatrix(Vec3F const& _angles)
        {
            return CreateRotationMatrix(_angles.x, _angles.y, _angles.z);
        }

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateScaleMatrix(TValue _x, TValue _y, TValue _z = 1);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateScaleMatrix(Vec2<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateScaleMatrix(Vec3<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateScaleMatrix(TValue _value)
        {
            return CreateScaleMatrix(_value, _value, _value);
        }

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4 CreateAffineTansformMatrix(
            Vec2<TValue> const& _pos,
            Rotation2D& _rotation,
            Vec2<TValue> const& _scale);

        //////////////////////////////////////////
        static inline Mat4 CreateAffineTransformMatrix(
            Vec2<TValue> const& _pos,
            Rotation2D& _rotation,
            Vec2<TValue> const& _scale,
            Vec2<TValue> const& _pivot,
            Vec2<TValue> const& _size);

        //////////////////////////////////////////
        static inline Mat4<TValue> CreateProjectionPerspectiveRHMatrix(
            F32 _fieldOfViewY,
            F32 _aspectRatio,
            F32 _nearZ,
            F32 _farZ);

        //////////////////////////////////////////
        static inline Mat4<TValue> CreateProjectionPerspectiveLHMatrix(
            F32 _fieldOfViewY,
            F32 _aspectRatio,
            F32 _nearZ,
            F32 _farZ);

        //////////////////////////////////////////
        static inline Mat4<TValue> CreateProjection2DMatrix(
            F32 _x,
            F32 _y,
            F32 _w,
            F32 _h);

        //////////////////////////////////////////
        static inline Mat4 CreateLookAtMatrix(
            Vec3<TValue> const& _from,
            Vec3<TValue> const& _to,
            Vec3<TValue> const& _up = Vec3<TValue>(0, 1, 0));

        //////////////////////////////////////////
        inline TValue const* getPlaneMatrix() const { return planeMatrix; }

        //////////////////////////////////////////
        inline void swap(Mat4<TValue>& _other);

        //////////////////////////////////////////
        inline TValue* operator[](Size _row) const;

        //////////////////////////////////////////
        inline Vec4<TValue> getColumn(Size _column) const;

        //////////////////////////////////////////
        inline void setColumn(Size _column, Vec4<TValue> const& _vec);

        //////////////////////////////////////////
        inline Mat4& operator=(Mat4 const& _matrix);

        //////////////////////////////////////////
        inline bool operator==(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline bool operator!=(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline void concatenate(Mat4 const& _m2);

        //////////////////////////////////////////
        inline Mat4 concatenatedCopy(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline Mat4 operator*(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline Vec3<TValue> operator*(Vec3<TValue> const& _v) const;

        //////////////////////////////////////////
        inline Vec4<TValue> operator*(Vec4<TValue> const& _v) const;

        //////////////////////////////////////////
        inline Mat4 operator+(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline Mat4 operator-(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline void operator=(Mat3<TValue> const& _mat3);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Mat4 transpose() const;

        //////////////////////////////////////////
        inline void setTranslation(Vec2<TValue> const& _v);

        //////////////////////////////////////////
        inline void setTranslation(Vec3<TValue> const& _v);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3<TValue> getAffineTranslation() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2<TValue> getAffineTranslation2D() const;

        //////////////////////////////////////////
        // inline MAZE_CONSTEXPR14 Vec3<TValue> getAffineRotationEulerAngles() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Mat4 operator*(F32 _scalar) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isAffine() const;

        //////////////////////////////////////////
        Mat4 inversedCopy() const;

        //////////////////////////////////////////
        Mat4 inversedAffineCopy() const;

        //////////////////////////////////////////
        inline Mat4 concatenatedAffineCopy(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline void concatenateAffine(Mat4 const& _m2, Mat4& _out) const;

        //////////////////////////////////////////
        inline Vec2<TValue> transformAffine(Vec2<TValue> const& _v) const;

        //////////////////////////////////////////
        inline void transformAffine(Vec2<TValue> const& _v, TValue& _outX, TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffine(Vec2<TValue> const& _v, Vec2<TValue>& _out) const;

        //////////////////////////////////////////
        inline Vec3<TValue> transformAffine(Vec3<TValue> const& _v) const;

        //////////////////////////////////////////
        inline Vec4<TValue> transformAffine(Vec4<TValue> const& _v) const;

        //////////////////////////////////////////
        inline bool compareAffine(Mat3<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline bool compareAffine(Mat4 const& _m2) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleXSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleYSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleZSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3<TValue> getAffineScaleSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2<TValue> getAffineScale2DSignless() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rotation2D getAffineRotation2D() const;


        //////////////////////////////////////////
        inline void getMat3(Mat3<TValue>& _mat) const
        {
            _mat.m[0][0] = m[0][0];
            _mat.m[0][1] = m[0][1];
            _mat.m[0][2] = m[0][2];
            _mat.m[1][0] = m[1][0];
            _mat.m[1][1] = m[1][1];
            _mat.m[1][2] = m[1][2];
            _mat.m[2][0] = m[2][0];
            _mat.m[2][1] = m[2][1];
            _mat.m[2][2] = m[2][2];
        }



        //////////////////////////////////////////
        inline String toString(Char _separator = ';') const;

        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, Mat4& _result, Char _separator = ';');

        //////////////////////////////////////////
        static inline CString ParseStringPretty(CString _string, Size _size, Mat4& _result, Char _separator = ',');

        //////////////////////////////////////////
        static inline Mat4 FromString(CString _string, Size _size, Char _separator = ';');

        //////////////////////////////////////////
        static Mat4 FromString(String const& _string, Char _separator = ';');

    protected:
        union 
        {
            TValue m[4][4];
            TValue planeMatrix[16];
        };
    }; 


    //////////////////////////////////////////
    template <typename>
    struct IsMat4 : std::false_type {};
    template <class TValue>
    struct IsMat4<Maze::Mat4<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Mat4S32 = Mat4<S32>;
    using Mat4U32 = Mat4<U32>;
    using Mat4F32 = Mat4<F32>;


    //////////////////////////////////////////
    // Aliases
    using Mat4S = Mat4S32;
    using Mat4U = Mat4U32;
    using Mat4F = Mat4F32;


    //////////////////////////////////////////
    template <class TValue = F32>
    inline std::ostream& operator<<(
        std::ostream& _o,
        Mat4<TValue> const& _mat)
    {
        _o << "Mat4(" << _mat[0][0] << ", " << _mat[0][1] << ", " << _mat[0][2] << ", " << _mat[0][3] << ", "
                       << _mat[1][0] << ", " << _mat[1][1] << ", " << _mat[1][2] << ", " << _mat[1][3] << ", "
                       << _mat[2][0] << ", " << _mat[2][1] << ", " << _mat[2][2] << ", " << _mat[2][3] << ", "
                       << _mat[3][0] << ", " << _mat[3][1] << ", " << _mat[3][2] << ", " << _mat[3][3] << ")";
        return _o;
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeMat4.inl"


#endif // _MazeMat4_hpp_
//////////////////////////////////////////