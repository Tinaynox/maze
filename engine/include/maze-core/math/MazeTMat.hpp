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
#if (!defined(_MazeTMat_hpp_))
#define _MazeTMat_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


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
    // [ RX RY RZ ]
    // | UX UY UZ |
    // | FX FY FZ |
    // [ TX TY TZ ]
    // R - right, U - up, F - front, T - translation
    //
    //////////////////////////////////////////
    class MAZE_CORE_API TMat
    {
    public:

        //////////////////////////////////////////
        using ValueType = F32;

        //////////////////////////////////////////
        static const ValueType c_epsilon;
        static const TMat c_zero;
        static const TMat c_identity;

    public:

        //////////////////////////////////////////
        inline TMat();

        //////////////////////////////////////////
        inline explicit TMat(ValueType const _arr[4][3]);

        //////////////////////////////////////////
        inline TMat(TMat const& _m);

        //////////////////////////////////////////
        inline TMat(Mat3<ValueType> const& _mat3);

        //////////////////////////////////////////
        inline TMat(    
            ValueType _m00, ValueType _m01, ValueType _m02,
            ValueType _m10, ValueType _m11, ValueType _m12,
            ValueType _m20, ValueType _m21, ValueType _m22,
            ValueType _m30, ValueType _m31, ValueType _m32);

        //////////////////////////////////////////
        inline void set(    
            ValueType _m00, ValueType _m01, ValueType _m02,
            ValueType _m10, ValueType _m11, ValueType _m12,
            ValueType _m20, ValueType _m21, ValueType _m22,
            ValueType _m30, ValueType _m31, ValueType _m32);

        //////////////////////////////////////////
        static inline TMat CreateBasis(
            Vec3<ValueType> const& _right,
            Vec3<ValueType> const& _up,
            Vec3<ValueType> const& _forward);

        //////////////////////////////////////////
        static inline TMat CreateTranslation(ValueType _x, ValueType _y, ValueType _z = 0);

        //////////////////////////////////////////
        static inline TMat CreateTranslation(Vec2<ValueType> const& _v);

        //////////////////////////////////////////
        static inline TMat CreateTranslation(Vec3<ValueType> const& _v);

        //////////////////////////////////////////
        static inline TMat CreateRotationZ(Rotation2D const& _rotation);

        //////////////////////////////////////////
        static inline TMat CreateRotationX(F32 _angle);

        //////////////////////////////////////////
        static inline TMat CreateRotationY(F32 _angle);

        //////////////////////////////////////////
        static inline TMat CreateRotationZ(F32 _angle);

        //////////////////////////////////////////
        static inline TMat CreateRotation(F32 _angleX, F32 _angleY, F32 _angleZ);

        //////////////////////////////////////////
        static inline TMat CreateRotation(Vec3F const& _angles)
        {
            return CreateRotation(_angles.x, _angles.y, _angles.z);
        }

        //////////////////////////////////////////
        static inline TMat CreateScale(ValueType _x, ValueType _y, ValueType _z = 1);

        //////////////////////////////////////////
        static inline TMat CreateScale(Vec2<ValueType> const& _v);

        //////////////////////////////////////////
        static inline TMat CreateScale(Vec3<ValueType> const& _v);

        //////////////////////////////////////////
        static inline TMat CreateScale(ValueType _value)
        {
            return CreateScale(_value, _value, _value);
        }

        //////////////////////////////////////////
        static inline TMat CreateTransform(
            Vec2<ValueType> const& _pos,
            Rotation2D& _rotation,
            Vec2<ValueType> const& _scale);

        //////////////////////////////////////////
        static inline TMat CreateTransform(
            Vec2<ValueType> const& _pos,
            Rotation2D& _rotation,
            Vec2<ValueType> const& _scale,
            Vec2<ValueType> const& _pivot,
            Vec2<ValueType> const& _size);

        //////////////////////////////////////////
        static inline TMat CreateLookAt(
            Vec3<ValueType> const& _from,
            Vec3<ValueType> const& _to,
            Vec3<ValueType> const& _up = Vec3<ValueType>(0, 1, 0));

        //////////////////////////////////////////
        inline ValueType const* getPlaneMatrix() const { return planeMatrix; }

        //////////////////////////////////////////
        inline void swap(TMat& _other);

        //////////////////////////////////////////
        inline Vec3<ValueType> getRow(Size _row) const;

        //////////////////////////////////////////
        inline void setRow(Size _row, Vec3<ValueType> const& _v);

        //////////////////////////////////////////
        inline ValueType* operator[](Size _row) const;

        //////////////////////////////////////////
        inline Vec4<ValueType> getColumn(Size _column) const;

        //////////////////////////////////////////
        inline void setColumn(Size _column, Vec4<ValueType> const& _v);

        //////////////////////////////////////////
        inline TMat& operator=(TMat const& _m);

        //////////////////////////////////////////
        inline bool operator==(TMat const& _m) const;

        //////////////////////////////////////////
        inline bool operator!=(TMat const& _m) const;

        //////////////////////////////////////////
        inline void multiply(TMat const& _m);

        //////////////////////////////////////////
        inline TMat multiplied(TMat const& _m) const;

        //////////////////////////////////////////
        inline TMat operator*(TMat const& _m) const;

        //////////////////////////////////////////
        inline TMat operator+(TMat const& _m) const;

        //////////////////////////////////////////
        inline TMat operator-(TMat const& _m) const;

        //////////////////////////////////////////
        inline void operator=(Mat3<ValueType> const& _mat3);


        //////////////////////////////////////////
        inline void setTranslation(Vec2<ValueType> const& _v);

        //////////////////////////////////////////
        inline void setTranslation(Vec3<ValueType> const& _v);

        //////////////////////////////////////////
        inline Vec3<ValueType> getTranslation() const;

        //////////////////////////////////////////
        inline Vec2<ValueType> getTranslation2D() const;




        //////////////////////////////////////////
        inline TMat inversed() const;

        //////////////////////////////////////////
        inline TMat transform(TMat const& _m) const;

        //////////////////////////////////////////
        inline void transform(TMat const& _m, TMat& _out) const;

        //////////////////////////////////////////
        inline Vec2<ValueType> transform(Vec2<ValueType> const& _v) const;

        //////////////////////////////////////////
        inline void transform(Vec2<ValueType> const& _v, ValueType& _outX, ValueType& _outY) const;

        //////////////////////////////////////////
        inline void transform(Vec2<ValueType> const& _v, Vec2<ValueType>& _out) const;

        //////////////////////////////////////////
        inline Vec3<ValueType> transform(Vec3<ValueType> const& _v) const;

        //////////////////////////////////////////
        inline Vec4<ValueType> transform(Vec4<ValueType> const& _v) const;

        //////////////////////////////////////////
        inline bool compare(Mat3<ValueType> const& _m) const;

        //////////////////////////////////////////
        inline bool compare(TMat const& _m) const;

        //////////////////////////////////////////
        inline ValueType getScaleXSignless() const;

        //////////////////////////////////////////
        inline ValueType getScaleYSignless() const;

        //////////////////////////////////////////
        inline ValueType getScaleZSignless() const;

        //////////////////////////////////////////
        inline Vec3<ValueType> getScaleSignless() const;

        //////////////////////////////////////////
        inline Vec2<ValueType> getScale2DSignless() const;

        //////////////////////////////////////////
        inline Rotation2D getRotation2D() const;


        //////////////////////////////////////////
        inline void getMat3(Mat3<ValueType>& _mat) const;


        //////////////////////////////////////////
        inline String toString(Char _separator = ',') const;

        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, TMat& _out, Char _separator = ',');

        //////////////////////////////////////////
        static inline CString ParseStringPretty(CString _string, Size _size, TMat& _out, Char _separator = ',');

        //////////////////////////////////////////
        static inline TMat FromString(CString _string, Size _size, Char _separator = ',');

        //////////////////////////////////////////
        static inline TMat FromString(String const& _string, Char _separator = ',');

    protected:
        union 
        {
            ValueType m[4][3];
            ValueType planeMatrix[12];
        };
    };


    //////////////////////////////////////////
    template <typename>
    struct IsTMat : std::false_type {};
    template <>
    struct IsTMat<Maze::TMat> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToString(TMat const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromString(TMat& _value, CString _data, Size _count)
    {
        _value = TMat::FromString(_data, _count);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void GetValueSerializationSize(TMat const& _value, U32& _outSize)
    {
        _outSize = sizeof(TMat);
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void SerializeValue(TMat const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TMat));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void DeserializeValue(TMat& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TMat));
    }

    //////////////////////////////////////////
    inline std::ostream& operator<<(
        std::ostream& _o,
        TMat const& _mat)
    {
        _o << "TMat(" << _mat[0][0] << ", " << _mat[0][1] << ", " << _mat[0][2] << ", "
                      << _mat[1][0] << ", " << _mat[1][1] << ", " << _mat[1][2] << ", "
                      << _mat[2][0] << ", " << _mat[2][1] << ", " << _mat[2][2] << ", "
                      << _mat[3][0] << ", " << _mat[3][1] << ", " << _mat[3][2] << ")";
        return _o;
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeTMat.inl"


#endif // _MazeTMat_hpp_
//////////////////////////////////////////
