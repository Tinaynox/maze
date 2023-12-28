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
#if (!defined(_MazeParticleSystemParameterF32_hpp_))
#define _MazeParticleSystemParameterF32_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-particles/MazeParticleSystemParameter.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_PARTICLES_API, ParticleSystemParameterF32SamplingMode,
        Constant,
        Curve,
        RandomBetweenConstants,
        RandomBetweenCurves);


    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystemParameterF32
        : public IJSONValueSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        using SampleRefFunc = void (ParticleSystemParameterF32::*)(S32 _particleSeed, F32 _scalar, F32& _result) const;

    public:

        //////////////////////////////////////////
        ParticleSystemParameterF32()
            : m_mode(ParticleSystemParameterF32SamplingMode::None)
        {
        }

        //////////////////////////////////////////
        ParticleSystemParameterF32(F32 _value)
        {
            setConstant(_value);
        }

        //////////////////////////////////////////
        ParticleSystemParameterF32(F32 _value0, F32 _value1)
        {
            setRandomBetweenConstants(_value0, _value1);
        }

        //////////////////////////////////////////
        ParticleSystemParameterF32(AnimationCurve const& _value)
        {
            setCurve(_value);
        }

        //////////////////////////////////////////
        ParticleSystemParameterF32(AnimationCurve const& _value0, AnimationCurve const& _value1)
        {
            setRandomBetweenCurves(_value0, _value1);
        }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32& operator=(ParticleSystemParameterF32 const& _other)
        {
            setSamplingMode(_other.m_mode);

            m_const0 = _other.m_const0;
            m_const1 = _other.m_const1;
            
            m_curve0 = _other.m_curve0;
            m_curve1 = _other.m_curve1;

            return *this;
        }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32SamplingMode getMode() const { return m_mode; }


        //////////////////////////////////////////
        void setConstant(F32 const& _value)
        {
            m_const0 = _value;
            setSamplingMode(ParticleSystemParameterF32SamplingMode::Constant);
        }

        ////////////////////////////////////
        void setRandomBetweenConstants(F32 const& _value0, F32 const& _value1)
        {
            m_const0 = _value0;
            m_const1 = _value1;
            setSamplingMode(ParticleSystemParameterF32SamplingMode::RandomBetweenConstants);
        }

        //////////////////////////////////////////
        void setCurve(AnimationCurve const& _value)
        {
            m_curve0 = _value;
            setSamplingMode(ParticleSystemParameterF32SamplingMode::Curve);
        }

        //////////////////////////////////////////
        void setRandomBetweenCurves(AnimationCurve const& _value0, AnimationCurve const& _value1)
        {
            m_curve0 = _value0;
            m_curve1 = _value1;
            setSamplingMode(ParticleSystemParameterF32SamplingMode::RandomBetweenCurves);
        }

        //////////////////////////////////////////
        F32 const& getConst0() const { return m_const0; }

        //////////////////////////////////////////
        F32 const& getConst1() const { return m_const1; }

        //////////////////////////////////////////
        AnimationCurve const& getCurve0() const { return m_curve0; }

        //////////////////////////////////////////
        AnimationCurve const& getCurve1() const { return m_curve1; }


        //////////////////////////////////////////
        inline void sample(S32 _particleSeed, F32 _scalar, F32& _result) const;


        //////////////////////////////////////////
        inline String toString() const
        {
            return JSONHelper::ToString(toJSONValue());
        }

        //////////////////////////////////////////
        inline static ParticleSystemParameterF32 FromString(String const& _string)
        {
            ParticleSystemParameterF32 parameter;
            parameter.loadFromJSONValue(JSONHelper::FromString(_string));
            return parameter;
        }

        //////////////////////////////////////////
        inline bool operator==(ParticleSystemParameterF32 const& _parameter) const
        {
            if (m_mode != _parameter.m_mode)
                return false;

            switch (m_mode)
            {
                case ParticleSystemParameterF32SamplingMode::Constant:
                {
                    if (m_const0 != _parameter.m_const0)
                        return false;

                    break;
                }
                case ParticleSystemParameterF32SamplingMode::Curve:
                {
                    if (m_curve0 != _parameter.m_curve0)
                        return false;

                    break;
                }
                case ParticleSystemParameterF32SamplingMode::RandomBetweenConstants:
                {
                    if (m_const0 != _parameter.m_const0)
                        return false;

                    if (m_const1 != _parameter.m_const1)
                        return false;

                    break;
                }
                case ParticleSystemParameterF32SamplingMode::RandomBetweenCurves:
                {
                    if (m_curve0 != _parameter.m_curve0)
                        return false;

                    if (m_curve1 != _parameter.m_curve1)
                        return false;

                    break;
                }
                default:
                {
                    break;
                }
            }

            return true;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystemParameterF32 const& _parameter) const
        {
            return !this->operator==(_parameter);
        }

        //////////////////////////////////////////
        void multiplyValues(F32 _value);

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE;


    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        void setSamplingMode(ParticleSystemParameterF32SamplingMode value);


        //////////////////////////////////////////
        inline void sampleRefNone(S32 _particleSeed, F32 _scalar, F32& _result) const;

        //////////////////////////////////////////
        inline void sampleRefConstant(S32 _particleSeed, F32 _scalar, F32& _result) const;

        //////////////////////////////////////////
        inline void sampleRefCurve(S32 _particleSeed, F32 _scalar, F32& _result) const;

        //////////////////////////////////////////
        inline void sampleRefRandomBetweenConstants(S32 _particleSeed, F32 _scalar, F32& _result) const;

        //////////////////////////////////////////
        inline void sampleRefRandomBetweenCurves(S32 _particleSeed, F32 _scalar, F32& _result) const;

    protected:
        ParticleSystemParameterF32SamplingMode m_mode;
        SampleRefFunc m_samplingRefFunc = &ParticleSystemParameterF32::sampleRefNone;

        F32 m_const0 = 0.0f;
        F32 m_const1 = 0.0f;

        AnimationCurve m_curve0;
        AnimationCurve m_curve1;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(ParticleSystemParameterF32 const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(ParticleSystemParameterF32& _value, CString _data, Size _count)
    {
        _value = ParticleSystemParameterF32::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(ParticleSystemParameterF32 const& _value)
    {
        return sizeof(ParticleSystemParameterF32);
    }

    //////////////////////////////////////////
    inline void SerializeValue(ParticleSystemParameterF32 const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(ParticleSystemParameterF32));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(ParticleSystemParameterF32& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(ParticleSystemParameterF32));
    }

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeParticleSystemParameterF32.inl"


#endif // _MazeParticleSystemParameter_hpp_
//////////////////////////////////////////
