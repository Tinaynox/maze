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
#if (!defined(_MazeMat4D_hpp_))
#define _MazeMat4D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Mat4D
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
    class Mat4D
    {
    public:

        //////////////////////////////////////////
        static const TValue c_epsilon;
        static const Mat4D<TValue> c_zero;
        static const Mat4D<TValue> c_identity;

    public:

        //////////////////////////////////////////
        inline Mat4D();

        //////////////////////////////////////////
        inline explicit Mat4D(TValue const _arr[4][4]);

        //////////////////////////////////////////
        inline Mat4D(Mat4D const& _matrix);

        //////////////////////////////////////////
        inline Mat4D(Mat3D<TValue> const& _mat3);

        //////////////////////////////////////////
        inline Mat4D(    
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
        static inline MAZE_CONSTEXPR Mat4D CreateTranslationMatrix(TValue _x, TValue _y, TValue _z = 0);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateTranslationMatrix(Vec2D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateTranslationMatrix(Vec3D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateRotationZMatrix(Rotation2D const& _rotation);

        //////////////////////////////////////////
        static inline Mat4D CreateRotationXMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline Mat4D CreateRotationYMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline Mat4D CreateRotationZMatrix(F32 _angle);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateScaleMatrix(TValue _x, TValue _y, TValue _z = 1);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateScaleMatrix(Vec2D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateScaleMatrix(Vec3D<TValue> const& _vector);

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateScaleMatrix(TValue _value)
        {
            return CreateScaleMatrix(_value, _value, _value);
        }

        //////////////////////////////////////////
        static inline MAZE_CONSTEXPR Mat4D CreateAffineTansformMatrix(
            Vec2D<TValue> const& _pos,
            Rotation2D& _rotation,
            Vec2D<TValue> const& _scale);

        //////////////////////////////////////////
        static inline Mat4D CreateAffineTransformMatrix(
            Vec2D<TValue> const& _pos,
            Rotation2D& _rotation,
            Vec2D<TValue> const& _scale,
            Vec2D<TValue> const& _pivot,
            Vec2D<TValue> const& _size);

        //////////////////////////////////////////
        static inline Mat4D<TValue> CreateProjectionPerspectiveRHMatrix(
            F32 _fieldOfViewY,
            F32 _aspectRatio,
            F32 _nearZ,
            F32 _farZ);

        //////////////////////////////////////////
        static inline Mat4D<TValue> CreateProjectionPerspectiveLHMatrix(
            F32 _fieldOfViewY,
            F32 _aspectRatio,
            F32 _nearZ,
            F32 _farZ);

        //////////////////////////////////////////
        static inline Mat4D<TValue> CreateProjection2DMatrix(
            F32 _x,
            F32 _y,
            F32 _w,
            F32 _h);

        //////////////////////////////////////////
        static inline Mat4D CreateLookAtMatrix(
            Vec3D<TValue> const& _from,
            Vec3D<TValue> const& _to,
            Vec3D<TValue> const& _up = Vec3D<TValue>(0, 1, 0));

        //////////////////////////////////////////
        inline TValue const* getPlaneMatrix() const { return planeMatrix; }

        //////////////////////////////////////////
        inline void swap(Mat4D<TValue>& _other);

        //////////////////////////////////////////
        inline TValue* operator[](Size _row) const;

        //////////////////////////////////////////
        inline Vec4D<TValue> getColumn(Size _column) const;

        //////////////////////////////////////////
        inline void setColumn(Size _column, Vec4D<TValue> const& _vec);

        //////////////////////////////////////////
        inline Mat4D& operator=(Mat4D const& _matrix);

        //////////////////////////////////////////
        inline bool operator==(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline bool operator!=(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline void concatenate(Mat4D const& _m2);

        //////////////////////////////////////////
        inline Mat4D concatenatedCopy(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline Mat4D operator*(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline Vec3D<TValue> operator*(Vec3D<TValue> const& _v) const;

        //////////////////////////////////////////
        inline Vec4D<TValue> operator*(Vec4D<TValue> const& _v) const;

        //////////////////////////////////////////
        inline Mat4D operator+(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline Mat4D operator-(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline void operator=(Mat3D<TValue> const& _mat3);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Mat4D transpose() const;

        //////////////////////////////////////////
        inline void setTranslation(Vec2D<TValue>& _v);

        //////////////////////////////////////////
        inline void setTranslation(Vec3D<TValue>& _v);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D<TValue> getAffineTranslation() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D<TValue> getAffineTranslation2D() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR14 Vec3D<TValue> getAffineRotationEulerAngles() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Mat4D operator*(F32 _scalar) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool isAffine() const;

        //////////////////////////////////////////
        Mat4D inversedCopy() const;

        //////////////////////////////////////////
        Mat4D inversedAffineCopy() const;

        //////////////////////////////////////////
        inline Mat4D concatenatedAffineCopy(Mat4D const& _m2);

        //////////////////////////////////////////
        inline void concatenateAffine(Mat4D const& _m2, Mat4D& _out) const;

        //////////////////////////////////////////
        inline Vec2D<TValue> transformAffine(Vec2D<TValue> const& _v) const;

        //////////////////////////////////////////
        inline void transformAffine(Vec2D<TValue> const& _v, TValue& _outX, TValue& _outY) const;

        //////////////////////////////////////////
        inline void transformAffine(Vec2D<TValue> const& _v, Vec2D<TValue>& _out) const;

        //////////////////////////////////////////
        inline Vec3D<TValue> transformAffine(Vec3D<TValue> const& _v) const;

        //////////////////////////////////////////
        inline Vec4D<TValue> transformAffine(Vec4D<TValue> const& _v) const;

        //////////////////////////////////////////
        inline bool compareAffine(Mat3D<TValue> const& _m2) const;

        //////////////////////////////////////////
        inline bool compareAffine(Mat4D const& _m2) const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleX() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleY() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR TValue getAffineScaleZ() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec3D<TValue> getAffineScale() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Vec2D<TValue> getAffineScale2D() const;

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rotation2D getAffineRotation2D() const;


        //////////////////////////////////////////
        inline String toString() const
        {
            String result;

            for (S32 i = 0; i < 16; ++i)
            {
                if (!result.empty())
                    result += ';';

                result += StringHelper::ToString(planeMatrix[i]);
            }

            return result;
        }

        //////////////////////////////////////////
        static Mat4D FromString(String const& _string)
        {
            Mat4D result;

            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 16)
            {
                for (S32 i = 0; i < 9; ++i)
                {
                    result.planeMatrix[i] = StringHelper::StringToF32(words[i]);
                }

                return result;
            }

            return Mat4D::c_zero;
        }

    protected:
        union 
        {
            TValue m[4][4];
            TValue planeMatrix[16];
        };
    }; 


    //////////////////////////////////////////
    template <typename>
    struct IsMat4D : std::false_type {};
    template <class TValue>
    struct IsMat4D<Maze::Mat4D<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4D<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4D<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4D<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4D<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsMat4D<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


    //////////////////////////////////////////
    using Mat4DS = Mat4D<S32>;
    using Mat4DU = Mat4D<U32>;
    using Mat4DF = Mat4D<F32>;


    //////////////////////////////////////////
    template <class TValue = F32>
    inline std::ostream& operator<<(
        std::ostream& _o,
        Mat4D<TValue> const& _mat)
    {
        _o << "Mat4D(" << _mat[0][0] << ", " << _mat[0][1] << ", " << _mat[0][2] << ", " << _mat[0][3] << ", "
                       << _mat[1][0] << ", " << _mat[1][1] << ", " << _mat[1][2] << ", " << _mat[1][3] << ", "
                       << _mat[2][0] << ", " << _mat[2][1] << ", " << _mat[2][2] << ", " << _mat[2][3] << ", "
                       << _mat[3][0] << ", " << _mat[3][1] << ", " << _mat[3][2] << ", " << _mat[3][3] << ")";
        return _o;
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeMat4D.inl"


#endif // _MazeMat4D_hpp_
//////////////////////////////////////////
