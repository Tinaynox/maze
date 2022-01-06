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
#if (!defined(_MazeQuaternion_hpp_))
#define _MazeQuaternion_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Quaternion
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Quaternion
    {
    public:

        //////////////////////////////////////////
        static const F32 c_epsilon;
        static const Quaternion c_zero;
        static const Quaternion c_identity;


    public:

        //////////////////////////////////////////
        inline Quaternion();

        //////////////////////////////////////////
        inline Quaternion(Quaternion const& rhs);

        //////////////////////////////////////////
        inline Quaternion(F32 _w, F32 _x, F32 _y, F32 _z);
        
        //////////////////////////////////////////
        inline Quaternion(Mat3DF const& _rotationMatrix);

        //////////////////////////////////////////
        inline Quaternion(Mat4DF const& _rotationMatrix);

        //////////////////////////////////////////
        inline Quaternion(F32 const& _angle, Vec3DF const& _axis);

        //////////////////////////////////////////
        inline Quaternion(
            Vec3DF const& _xAxis,
            Vec3DF const& _yAxis,
            Vec3DF const& _zAxis);

        //////////////////////////////////////////
        inline Quaternion(Vec3DF const* _axes);

        //////////////////////////////////////////
        inline Quaternion(F32* _value);

        //////////////////////////////////////////
        inline Quaternion(F32 _xAngle, F32 _yAngle, F32 _zAngle);

        //////////////////////////////////////////
        inline Quaternion(Vec3DF const& _eulerAngles);

        //////////////////////////////////////////
        inline Quaternion(Vec3DF const& _fromVector, Vec3DF const& _toVector);


        //////////////////////////////////////////
        static inline Vec3DF GetEuler(Mat3DF const& _rotationMatrix) { return Quaternion(_rotationMatrix).getEuler(); }

        //////////////////////////////////////////
        static inline Vec3DF GetEuler(Mat4DF const& _rotationMatrix) { return Quaternion(_rotationMatrix).getEuler(); }


        //////////////////////////////////////////
        inline void swap(Quaternion& _q);


        //////////////////////////////////////////
        inline F32 operator[](Size const _i) const;

        //////////////////////////////////////////
        inline F32& operator[](Size const _i);

        //////////////////////////////////////////
        inline F32* ptr();

        //////////////////////////////////////////
        inline F32 const* ptr() const;


        //////////////////////////////////////////
        void setRotationMatrix(Mat3DF const& _rotationMatrix);

        //////////////////////////////////////////
        void setRotationMatrix(Mat3DF const& _rotationMatrix, bool _multInvScale);

        //////////////////////////////////////////
        inline void setRotationMatrix(Mat4DF const& _rotationMatrix, bool _multInvScale = true)
        {
            Mat3DF mat;
            _rotationMatrix.getMat3D(mat);
            setRotationMatrix(mat, _multInvScale);
        }

        //////////////////////////////////////////
        void toRotationMatrix(Mat3DF& _rotationMatrix) const;

        //////////////////////////////////////////
        void toRotationMatrix(Mat4DF& _rotationMatrix) const;

        //////////////////////////////////////////
        inline Mat4DF toRotationMatrix() const
        {
            Mat4DF result;
            toRotationMatrix(result);
            return result;
        }

        //////////////////////////////////////////
        void setAngleAxis(F32 const& _angle, Vec3DF const& _axis);

        //////////////////////////////////////////
        void toAngleAxis(F32& _angle, Vec3DF& _axis) const;

        //////////////////////////////////////////
        void setAxes(Vec3DF const* _axis);

        //////////////////////////////////////////
        void setAxes(
            Vec3DF const& _xAxis,
            Vec3DF const& _yAxis,
            Vec3DF const& _zAxis);

        //////////////////////////////////////////
        void setEulerAngles(
            F32 _xAngle,
            F32 _yAngle,
            F32 _zAngle);

        //////////////////////////////////////////
        inline void setEulerAngles(
            Vec3DF const& _eulerAngles)
        {
            setEulerAngles(
                _eulerAngles.x,
                _eulerAngles.y,
                _eulerAngles.z);
        }

        //////////////////////////////////////////
        void toAxes(Vec3DF* _axes) const;

        //////////////////////////////////////////
        Vec3DF getXAxis() const;

        //////////////////////////////////////////
        Vec3DF getYAxis() const;

        //////////////////////////////////////////
        Vec3DF getZAxis() const;

        //////////////////////////////////////////
        void toAxes(
            Vec3DF& _xAxis,
            Vec3DF& _yAxis,
            Vec3DF& _zAxis) const;

        //////////////////////////////////////////
        inline Quaternion& operator=(Quaternion const& _q);

        //////////////////////////////////////////
        Quaternion operator+(Quaternion const& _q) const;
        
        //////////////////////////////////////////
        Quaternion operator-(Quaternion const& _q) const;
        
        //////////////////////////////////////////
        Quaternion operator*(Quaternion const& _q) const;

        //////////////////////////////////////////
        Quaternion inversedCopy() const;

        //////////////////////////////////////////
        Quaternion inversedUnitCopy() const;

        //////////////////////////////////////////
        Quaternion exp() const;

        //////////////////////////////////////////
        Quaternion log() const;

        //////////////////////////////////////////
        Vec3DF operator*(Vec3DF const& _v) const;

        //////////////////////////////////////////
        inline Quaternion operator*(F32 _s) const;

        //////////////////////////////////////////
        friend Quaternion operator*(F32 _s, const Quaternion& q)
        {
            return q * _s;
        }

        //////////////////////////////////////////
        inline Quaternion operator-() const;

        //////////////////////////////////////////
        inline bool operator==(Quaternion const& _q) const;

        //////////////////////////////////////////
        inline bool operator!=(Quaternion const& _q) const;

        //////////////////////////////////////////
        inline F32 dot(Quaternion const& _q) const;

        //////////////////////////////////////////
        inline F32 norm() const;

        //////////////////////////////////////////
        inline F32 normalize();


        //////////////////////////////////////////
        inline bool equals(Quaternion const& _q, F32 const& _tolerance) const;

        //////////////////////////////////////////
        inline bool orientationEquals(Quaternion const& _q, F32 _tolerance = 1e-3f) const;

        //////////////////////////////////////////
        static Quaternion Slerp(
            F32 _t,
            Quaternion const& _p,
            Quaternion const& _q,
            bool _shortestPath = true);

        //////////////////////////////////////////
        Quaternion SlerpExtraSpins(
            F32 _t,
            Quaternion const& _p,
            Quaternion const& _q,
            S32 _extraSpins);

        //////////////////////////////////////////
        static void Intermediate(
            Quaternion const& _q0,
            Quaternion const& _q1,
            Quaternion const& _q2,
            Quaternion& _a,
            Quaternion& _b);

        //////////////////////////////////////////
        static Quaternion Squad(
            F32 _t,
            Quaternion const& _p,
            Quaternion const& _a,
            Quaternion const& _b,
            Quaternion const& _q,
            bool _shortestPath = true);

        //////////////////////////////////////////
        static Quaternion Nlerp(
            F32 _t,
            Quaternion const& _p,
            Quaternion const& _q,
            bool _shortestPath = true);

        //////////////////////////////////////////
        static Quaternion LookRotation(Vec3DF const& _forward, Vec3DF const& _up);

        //////////////////////////////////////////
        inline F32 getRoll() const { return getEuler().z; }

        //////////////////////////////////////////
        inline F32 getPitch() const { return getEuler().x; }

        //////////////////////////////////////////
        inline F32 getYaw() const { return getEuler().y; }

        //////////////////////////////////////////
        Vec3DF getEuler() const;

        //////////////////////////////////////////
        inline bool isNaN() const
        {
            return Math::IsNaN(x) || Math::IsNaN(y) || Math::IsNaN(z) || Math::IsNaN(w);
        }
        
        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(w) + ";" + StringHelper::ToString(x) + ";" + StringHelper::ToString(y) + ";" + StringHelper::ToString(z); }

        //////////////////////////////////////////
        inline static Quaternion FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 4)
            {
                return Quaternion(
                    StringHelper::StringToF32(words[0]),
                    StringHelper::StringToF32(words[1]),
                    StringHelper::StringToF32(words[2]),
                    StringHelper::StringToF32(words[3]));
            }

            return Quaternion::c_identity;
        }

    public:
        F32 w;
        F32 x;
        F32 y;
        F32 z;
    }; 

    //////////////////////////////////////////
    inline std::ostream& operator<<(
        std::ostream& _o,
        Quaternion const& _q)
    {
        _o << "Quaternion(" << _q.w << ", " << _q.x << ", " << _q.y << ", " << _q.z << ")";
        return _o;
    }


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Quaternion const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Quaternion& _value, CString _data, Size _count)
    {
        _value = Quaternion::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Quaternion const& _value)
    {
        return sizeof(Quaternion);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Quaternion const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Quaternion));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Quaternion& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Quaternion));
    }

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeQuaternion.inl"


#endif // _MazeQuaternion_hpp_
//////////////////////////////////////////
