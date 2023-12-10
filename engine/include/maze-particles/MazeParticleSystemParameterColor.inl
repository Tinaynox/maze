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
    inline void ParticleSystemParameterColor::sample(S32 _particleSeed, F32 _scalar, Vec4F& _result) const
    {
        MAZE_DEBUG_BP_IF(_scalar < 0.0f || _scalar > 1.0f);
        (this->*m_samplingRefFunc)(_particleSeed, _scalar, _result);
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterColor::sampleRefNone(S32 _particleSeed, F32 _scalar, Vec4F& _result) const
    {

    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterColor::sampleRefConstant(S32 _particleSeed, F32 _scalar, Vec4F& _result) const
    {
        _result = m_color0;
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterColor::sampleRefGradient(S32 _particleSeed, F32 _scalar, Vec4F& _result) const
    {
        _result = m_gradient0.evaluate(_scalar);
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterColor::sampleRefRandomBetweenColors(S32 _particleSeed, F32 _scalar, Vec4F& _result) const
    {
        F32 seed01 = (F32)_particleSeed * c_invParticleSystemParametersCount;

        _result = (m_color1 - m_color0) * seed01 + m_color0;
    }

    //////////////////////////////////////////
    inline void ParticleSystemParameterColor::sampleRefRandomBetweenGradients(S32 _particleSeed, F32 _scalar, Vec4F& _result) const
    {
        Vec4F value0 = m_gradient0.evaluate(_scalar);
        Vec4F value1 = m_gradient1.evaluate(_scalar);

        F32 seed01 = (F32)_particleSeed * c_invParticleSystemParametersCount;

        Vec4F midValue = Math::Lerp(value0, value1, seed01);

        _result = static_cast<Vec4F>(midValue);
    }


} // namespace Maze
//////////////////////////////////////////