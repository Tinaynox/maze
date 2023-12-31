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
#include "maze-particles/MazeParticleSystemParameterColor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ParticleSystemParameterColorSamplingMode);


    //////////////////////////////////////////
    void ParticleSystemParameterColor::setSamplingMode(ParticleSystemParameterColorSamplingMode value)
    {
        m_mode = value;

        switch (m_mode)
        {
            case ParticleSystemParameterColorSamplingMode::None:
            {
                m_samplingRefFunc = &ParticleSystemParameterColor::sampleRefNone;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::Color:
            {
                m_samplingRefFunc = &ParticleSystemParameterColor::sampleRefConstant;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::Gradient:
            {
                m_samplingRefFunc = &ParticleSystemParameterColor::sampleRefGradient;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenColors:
            {
                m_samplingRefFunc = &ParticleSystemParameterColor::sampleRefRandomBetweenColors;
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenGradients:
            {
                m_samplingRefFunc = &ParticleSystemParameterColor::sampleRefRandomBetweenGradients;
                break;
            }

            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void ParticleSystemParameterColor::loadFromJSONValue(Json::Value const& _value)
    {
        m_color0 = {
            _value["r0"].asFloat(),
            _value["g0"].asFloat(),
            _value["b0"].asFloat(),
            _value["a0"].asFloat()
        };
        m_color1 = {
            _value["r1"].asFloat(),
            _value["g1"].asFloat(),
            _value["b1"].asFloat(),
            _value["a1"].asFloat()
        };

        m_gradient0.loadFromJSONValue(_value["gr0"]);
        m_gradient1.loadFromJSONValue(_value["gr1"]);

        setSamplingMode(ParticleSystemParameterColorSamplingMode(_value["m"].asInt()));
    }

    //////////////////////////////////////////
    Json::Value ParticleSystemParameterColor::toJSONValue() const
    {
        Json::Value value;

        value["r0"] = m_color0.x;
        value["g0"] = m_color0.y;
        value["b0"] = m_color0.z;
        value["a0"] = m_color0.w;
        value["r1"] = m_color1.x;
        value["g1"] = m_color1.y;
        value["b1"] = m_color1.z;
        value["a1"] = m_color1.w;

        value["gr0"] = m_gradient0.toJSONValue();
        value["gr1"] = m_gradient1.toJSONValue();

        value["m"] = (S32)m_mode;

        return value;
    }

    //////////////////////////////////////////
    bool ParticleSystemParameterColor::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        m_color0 = _dataBlock.getVec4F(MAZE_HS("color0"));
        m_color1 = _dataBlock.getVec4F(MAZE_HS("color1"));

        DataBlock const* gradient0Block = _dataBlock.getDataBlock(MAZE_HS("gradient0"));
        if (gradient0Block)
            ValueFromDataBlock(m_gradient0, *gradient0Block);

        DataBlock const* gradient1Block = _dataBlock.getDataBlock(MAZE_HS("gradient1"));
        if (gradient1Block)
            ValueFromDataBlock(m_gradient1, *gradient1Block);

        setSamplingMode(ParticleSystemParameterColorSamplingMode(_dataBlock.getS32(MAZE_HS("mode"))));

        return true;
    }

    //////////////////////////////////////////
    void ParticleSystemParameterColor::toDataBlock(DataBlock& _dataBlock) const
    {
        switch (m_mode)
        {
            case ParticleSystemParameterColorSamplingMode::Color:
            {
                _dataBlock.setVec4F(MAZE_HS("color0"), m_color0);
                break;
            }
            case ParticleSystemParameterColorSamplingMode::Gradient:
            {
                DataBlock& gradient0Block = _dataBlock[MAZE_HS("gradient0")];
                ValueToDataBlock(m_gradient0, gradient0Block);
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenColors:
            {
                _dataBlock.setVec4F(MAZE_HS("color0"), m_color0);
                _dataBlock.setVec4F(MAZE_HS("color1"), m_color1);
                break;
            }
            case ParticleSystemParameterColorSamplingMode::RandomBetweenGradients:
            {
                DataBlock& gradient0Block = _dataBlock[MAZE_HS("gradient0")];
                ValueToDataBlock(m_gradient0, gradient0Block);

                DataBlock& gradient1Block = _dataBlock[MAZE_HS("gradient1")];
                ValueToDataBlock(m_gradient1, gradient1Block);

                break;
            }

            default:
            {
                break;
            }
        }

        _dataBlock.setS32(MAZE_HS("mode"), (S32)m_mode);
    }
    
} // namespace Maze
//////////////////////////////////////////
