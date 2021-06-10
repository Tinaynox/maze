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
#include "MazeParticlesHeader.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ParticleSystemParameterF32SamplingMode);


    //////////////////////////////////////////
    void ParticleSystemParameterF32::setSamplingMode(ParticleSystemParameterF32SamplingMode value)
    {
        m_mode = value;

        switch (m_mode)
        {
            case ParticleSystemParameterF32SamplingMode::None:
            {
                m_samplingRefFunc = &ParticleSystemParameterF32::sampleRefNone;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::Constant:
            {
                m_samplingRefFunc = &ParticleSystemParameterF32::sampleRefConstant;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::Curve:
            {
                m_samplingRefFunc = &ParticleSystemParameterF32::sampleRefCurve;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenConstants:
            {
                m_samplingRefFunc = &ParticleSystemParameterF32::sampleRefRandomBetweenConstants;
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenCurves:
            {
                m_samplingRefFunc = &ParticleSystemParameterF32::sampleRefRandomBetweenCurves;
                break;
            }

            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void ParticleSystemParameterF32::loadFromJSONValue(Json::Value const& _value)
    {
        m_const0 = _value["c0"].asFloat();
        m_const1 = _value["c1"].asFloat();
        m_curve0.loadFromJSONValue(_value["cu0"]);
        m_curve1.loadFromJSONValue(_value["cu1"]);

        setSamplingMode(ParticleSystemParameterF32SamplingMode(_value["m"].asInt()));
    }

    //////////////////////////////////////////
    Json::Value ParticleSystemParameterF32::toJSONValue() const
    {
        Json::Value value;

        value["c0"] = m_const0;
        value["c1"] = m_const1;
        value["cu0"] = m_curve0.toJSONValue();
        value["cu1"] = m_curve1.toJSONValue();
        value["m"] = (S32)m_mode;

        return value;
    }

    //////////////////////////////////////////
    void ParticleSystemParameterF32::multiplyValues(F32 _value)
    {
        m_const0 *= _value;
        m_const1 *= _value;
        m_curve0.multiplyValues(_value);
        m_curve1.multiplyValues(_value);
    }
    
} // namespace Maze
//////////////////////////////////////////
