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
        // #TODO: Back compatibility, remove later
        if (_value.isDouble())
        {
            setConstant(_value.asFloat());
            return;
        }

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
    bool ParticleSystemParameterF32::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        m_const0 = _dataBlock.getF32(MAZE_HS("const0"));
        m_const1 = _dataBlock.getF32(MAZE_HS("const1"));

        DataBlock const* curve0Block = _dataBlock.getDataBlock(MAZE_HS("curve0"));
        if (curve0Block)
            ValueFromDataBlock(m_curve0, *curve0Block);

        DataBlock const* curve1Block = _dataBlock.getDataBlock(MAZE_HS("curve1"));
        if (curve1Block)
            ValueFromDataBlock(m_curve1, *curve1Block);

        setSamplingMode(ParticleSystemParameterF32SamplingMode(_dataBlock.getS32(MAZE_HS("mode"))));

        return true;
    }

    //////////////////////////////////////////
    void ParticleSystemParameterF32::toDataBlock(DataBlock& _dataBlock) const
    {   
        switch (m_mode)
        {
            case ParticleSystemParameterF32SamplingMode::Constant:
            {
                _dataBlock.setF32(MAZE_HS("const0"), m_const0);
                break;
            }
            case ParticleSystemParameterF32SamplingMode::Curve:
            {
                DataBlock& curve0Block = _dataBlock[MAZE_HS("curve0")];
                ValueToDataBlock(m_curve0, curve0Block);
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenConstants:
            {
                _dataBlock.setF32(MAZE_HS("const0"), m_const0);
                _dataBlock.setF32(MAZE_HS("const1"), m_const1);
                break;
            }
            case ParticleSystemParameterF32SamplingMode::RandomBetweenCurves:
            {
                DataBlock& curve0Block = _dataBlock[MAZE_HS("curve0")];
                ValueToDataBlock(m_curve0, curve0Block);

                DataBlock& curve1Block = _dataBlock[MAZE_HS("curve1")];
                ValueToDataBlock(m_curve1, curve1Block);
                break;
            }

            default:
            {
                break;
            }
        }

        _dataBlock.setS32(MAZE_HS("mode"), (S32)m_mode);
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
