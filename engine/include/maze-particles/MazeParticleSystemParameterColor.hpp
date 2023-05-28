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
#if (!defined(_MazeParticleSystemParameterColor_hpp_))
#define _MazeParticleSystemParameterColor_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-particles/MazeParticleSystemParameter.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_PARTICLES_API, ParticleSystemParameterColorSamplingMode,
        Color,
        Gradient,
        RandomBetweenColors,
        RandomBetweenGradients);


    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystemParameterColor
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        using SampleRefFunc = void (ParticleSystemParameterColor::*)(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;

    public:

        //////////////////////////////////////////
        ParticleSystemParameterColor()
            : m_mode(ParticleSystemParameterColorSamplingMode::None)
        {
        }

        //////////////////////////////////////////
        ParticleSystemParameterColor(Vec4DF const& _value)
        {
            setColor(_value);
        }

        //////////////////////////////////////////
        ParticleSystemParameterColor(Vec4DF const& _value0, Vec4DF const& _value1)
        {
            setRandomBetweenColors(_value0, _value1);
        }

        //////////////////////////////////////////
        ParticleSystemParameterColor(ColorGradient const& _value)
        {
            setGradient(_value);
        }

        //////////////////////////////////////////
        ParticleSystemParameterColor(ColorGradient const& _value0, ColorGradient const& _value1)
        {
            setRandomBetweenGradients(_value0, _value1);
        }

        //////////////////////////////////////////
        inline ParticleSystemParameterColor& operator=(ParticleSystemParameterColor const& _other)
        {
            setSamplingMode(_other.m_mode);

            m_color0 = _other.m_color0;
            m_color1 = _other.m_color1;
            
            m_gradient0 = _other.m_gradient0;
            m_gradient1 = _other.m_gradient1;

            return *this;
        }

        //////////////////////////////////////////
        inline ParticleSystemParameterColorSamplingMode getMode() const { return m_mode; }


        //////////////////////////////////////////
        void setColor(Vec4DF const& _value)
        {
            m_color0 = _value;
            setSamplingMode(ParticleSystemParameterColorSamplingMode::Color);
        }

        //////////////////////////////////////////
        void setRandomBetweenColors(Vec4DF const& _value0, Vec4DF const& _value1)
        {
            m_color0 = _value0;
            m_color1 = _value1;
            setSamplingMode(ParticleSystemParameterColorSamplingMode::RandomBetweenColors);
        }

        //////////////////////////////////////////
        void setGradient(ColorGradient const& _value)
        {
            m_gradient0 = _value;
            setSamplingMode(ParticleSystemParameterColorSamplingMode::Gradient);
        }

        //////////////////////////////////////////
        void setRandomBetweenGradients(ColorGradient const& _value0, ColorGradient const& _value1)
        {
            m_gradient0 = _value0;
            m_gradient1 = _value1;
            setSamplingMode(ParticleSystemParameterColorSamplingMode::RandomBetweenGradients);
        }

        //////////////////////////////////////////
        Vec4DF const& getColor0() const { return m_color0; }

        //////////////////////////////////////////
        Vec4DF const& getColor1() const { return m_color1; }

        //////////////////////////////////////////
        ColorGradient const& getGradient0() const { return m_gradient0; }

        //////////////////////////////////////////
        ColorGradient const& getGradient1() const { return m_gradient1; }


        //////////////////////////////////////////
        inline void sample(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;


        //////////////////////////////////////////
        inline String toString() const
        {
            return JSONHelper::ToString(toJSONValue());
        }

        //////////////////////////////////////////
        inline static ParticleSystemParameterColor FromString(String const& _string)
        {
            ParticleSystemParameterColor parameter;
            parameter.loadFromJSONValue(JSONHelper::FromString(_string));
            return parameter;
        }

        //////////////////////////////////////////
        inline bool operator==(ParticleSystemParameterColor const& _parameter) const
        {
            if (m_mode != _parameter.m_mode)
                return false;

            switch (m_mode)
            {
                case ParticleSystemParameterColorSamplingMode::Color:
                {
                    if (m_color0 != _parameter.m_color0)
                        return false;

                    break;
                }
                case ParticleSystemParameterColorSamplingMode::Gradient:
                {
                    if (m_gradient0 != _parameter.m_gradient0)
                        return false;

                    break;
                }
                case ParticleSystemParameterColorSamplingMode::RandomBetweenColors:
                {
                    if (m_color0 != _parameter.m_color0)
                        return false;

                    if (m_color1 != _parameter.m_color1)
                        return false;

                    break;
                }
                case ParticleSystemParameterColorSamplingMode::RandomBetweenGradients:
                {
                    if (m_gradient0 != _parameter.m_gradient0)
                        return false;

                    if (m_gradient1 != _parameter.m_gradient1)
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
        inline bool operator!=(ParticleSystemParameterColor const& _parameter) const
        {
            return !this->operator==(_parameter);
        }

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        void setSamplingMode(ParticleSystemParameterColorSamplingMode value);


        //////////////////////////////////////////
        inline void sampleRefNone(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;

        //////////////////////////////////////////
        inline void sampleRefConstant(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;

        //////////////////////////////////////////
        inline void sampleRefGradient(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;

        //////////////////////////////////////////
        inline void sampleRefRandomBetweenColors(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;

        //////////////////////////////////////////
        inline void sampleRefRandomBetweenGradients(S32 _particleSeed, F32 _scalar, Vec4DF& _result) const;

    protected:
        ParticleSystemParameterColorSamplingMode m_mode;
        SampleRefFunc m_samplingRefFunc = &ParticleSystemParameterColor::sampleRefNone;

        Vec4DF m_color0;
        Vec4DF m_color1;

        ColorGradient m_gradient0;
        ColorGradient m_gradient1;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(ParticleSystemParameterColor const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(ParticleSystemParameterColor& _value, CString _data, Size _count)
    {
        _value = ParticleSystemParameterColor::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(ParticleSystemParameterColor const& _value)
    {
        return sizeof(ParticleSystemParameterColor);
    }

    //////////////////////////////////////////
    inline void SerializeValue(ParticleSystemParameterColor const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(ParticleSystemParameterColor));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(ParticleSystemParameterColor& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(ParticleSystemParameterColor));
    }

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeParticleSystemParameterColor.inl"


#endif // _MazeParticleSystemParameterColor_hpp_
//////////////////////////////////////////
