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
namespace Maze
{
    //////////////////////////////////////////
    inline void ParticleSystemParameterF32::sample(S32 _particleSeed, F32 _scalar, F32& _result) const
    {
        MAZE_DEBUG_BP_IF(_scalar < 0.0f || _scalar > 1.0f);
        (this->*m_samplingRefFunc)(_particleSeed, _scalar, _result);
    }

    

    //////////////////////////////////////////
    inline void ParticleSystemParameterF32::sampleRefNone(S32 _particleSeed, F32 _scalar, F32& _result) const
    {

    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterF32::sampleRefConstant(S32 _particleSeed, F32 _scalar, F32& _result) const
    {
        _result = m_const0;
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterF32::sampleRefCurve(S32 _particleSeed, F32 _scalar, F32& _result) const
    {
        _result = m_curve0.evaluate(_scalar);
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterF32::sampleRefRandomBetweenConstants(S32 _particleSeed, F32 _scalar, F32& _result) const
    {
        F32 seed01 = (F32)_particleSeed * c_invParticleSystemParametersCount;

        _result = (m_const1 - m_const0) * seed01 + m_const0;
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterF32::sampleRefRandomBetweenCurves(S32 _particleSeed, F32 _scalar, F32& _result) const
    {
        F32 value0 = m_curve0.evaluate(_scalar);
        F32 value1 = m_curve1.evaluate(_scalar);

        F32 seed01 = (F32)_particleSeed * c_invParticleSystemParametersCount;

        F32 midValue = Math::Lerp(value0, value1, seed01);

        _result = midValue;
    }


} // namespace Maze
//////////////////////////////////////////