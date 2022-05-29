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
#if (!defined(_MazeRotation2D_hpp_))
#define _MazeRotation2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMathAngles.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Rotation2D
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Rotation2D
    {
    public:

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rotation2D() 
            : m_s(0.0f)
            , m_c(1.0f)
        { 
        }

        //////////////////////////////////////////
        inline Rotation2D(Rotation2D const& _rotation) 
            : m_s(_rotation.m_s)
            , m_c(_rotation.m_c)
        {
            MAZE_DEBUG_BP_IF(fabs(m_s) > 1);
            MAZE_DEBUG_BP_IF(fabs(m_c) > 1);
        }

        //////////////////////////////////////////
        inline Rotation2D(Rotation2D&& _rotation) noexcept
            : m_s(std::move(_rotation.m_s))
            , m_c(std::move(_rotation.m_c))
        {
            MAZE_DEBUG_BP_IF(fabs(m_s) > 1);
            MAZE_DEBUG_BP_IF(fabs(m_c) > 1);
        }

        //////////////////////////////////////////
        inline explicit Rotation2D(F32 _angle)
            : m_s(sinf(_angle))
            , m_c(cosf(_angle))
        {
            
        }

        //////////////////////////////////////////
        inline explicit Rotation2D(F32 _s, F32 _c)
            : m_s(_s)
            , m_c(_c)
        {
            MAZE_DEBUG_BP_IF(fabs(m_s) > 1);
            MAZE_DEBUG_BP_IF(fabs(m_c) > 1);
        }

        //////////////////////////////////////////
        inline void set(F32 _angle)
        {
            m_s = sinf(_angle);
            m_c = cosf(_angle);
        }

        //////////////////////////////////////////
        void setIdentity()
        {
            m_s = 0.0f;
            m_c = 1.0f;
        }

        //////////////////////////////////////////
        inline F32 toRadians() const
        {
            return std::atan2(m_s, m_c);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 getSin() const { return m_s; }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR F32 getCos() const { return m_c; }

        //////////////////////////////////////////
        inline F32 toDegrees() const
        {
            return (std::atan2(m_s, m_c) / Math::c_pi) * 180.0f;
        }

        //////////////////////////////////////////
        inline Vec2DF toVector() const
        {
            return Vec2DF::FromAngle(toRadians());
        }

        //////////////////////////////////////////
        static inline Rotation2D FromVector(Vec2DF const& _vector)
        {
            return Rotation2D(_vector.toAngle());
        }

        //////////////////////////////////////////
        static inline Rotation2D FromDegrees(F32 _degrees)
        {
            return Rotation2D((_degrees / 180.0f) * Math::c_pi);
        }

        //////////////////////////////////////////
        static inline Rotation2D Random()
        {
            return Rotation2D(Math::RangeRandom(0.0f, Math::c_pi));
        }

        //////////////////////////////////////////
        inline Rotation2D& operator=(Rotation2D const& _rotation2D)
        {
            m_s = _rotation2D.m_s;
            m_c = _rotation2D.m_c;

            MAZE_DEBUG_BP_IF(fabs(m_s) > 1);
            MAZE_DEBUG_BP_IF(fabs(m_c) > 1);

            return *this;
        }

        //////////////////////////////////////////
        inline Rotation2D& operator=(Rotation2D&& _rotation2D) noexcept
        {
            m_s = std::move(_rotation2D.m_s);
            m_c = std::move(_rotation2D.m_c);

            MAZE_DEBUG_BP_IF(fabs(m_s) > 1);
            MAZE_DEBUG_BP_IF(fabs(m_c) > 1);

            return *this;
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator==(Rotation2D const& _rkRotation2D) const
        {
            return (m_s == _rkRotation2D.m_s && m_c == _rkRotation2D.m_c);
        }

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR bool operator!=(Rotation2D const& _rkRotation2D) const
        {
            return (m_s != _rkRotation2D.m_s || m_c != _rkRotation2D.m_c);
        }

        //////////////////////////////////////////
        inline void normalize()
        {
            set(Math::NormalizedAnglePI(toRadians()));
        }
        
        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(m_s) + ";" + StringHelper::ToString(m_c); }

        //////////////////////////////////////////
        inline static Rotation2D FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 2)
            {
                return Rotation2D(
                    StringHelper::StringToF32(words[0]),
                    StringHelper::StringToF32(words[1]));
            }

            return Rotation2D();
        }

    protected:
        F32 m_s;
        F32 m_c;
    }; 


    //////////////////////////////////////////
    template <>
    inline Rotation2D Math::Lerp<Rotation2D, F32>(
        Rotation2D const& _v0,
        Rotation2D const& _v1,
        F32 _s)
    {
        return Rotation2D(LerpAngle(_v0.toRadians(), _v1.toRadians(), _s));
    }

    
    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(Rotation2D const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(Rotation2D& _value, CString _data, Size _count)
    {
        _value = Rotation2D::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(Rotation2D const& _value)
    {
        return sizeof(Rotation2D);
    }

    //////////////////////////////////////////
    inline void SerializeValue(Rotation2D const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(Rotation2D));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(Rotation2D& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(Rotation2D));
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRotation2D_hpp_
//////////////////////////////////////////
