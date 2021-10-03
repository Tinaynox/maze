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
#include "MazeColorGradient.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorGradient
    //
    //////////////////////////////////////////
    inline ColorGradient::ColorGradient()
    {
    }

    //////////////////////////////////////////
    inline ColorGradient::ColorGradient(
        F32 _time0,
        Vec4DF const& _color0)
    {
        addKey(_time0, _color0);
    }

    //////////////////////////////////////////
    inline ColorGradient::ColorGradient(Vec4DF const& _color0)
    {
        addKey(0.0f, _color0);
    }

    //////////////////////////////////////////
    inline ColorGradient::ColorGradient(
        F32 _time0,
        Vec4DF const& _color0,
        F32 _time1,
        Vec4DF const& _color1)
    {
        addKey(_time0, _color0);
        addKey(_time1, _color1);
    }

    //////////////////////////////////////////
    inline Vec3DF ColorGradient::evaluateRGB(F32 _time) const
    {
        if (m_keyframesRGB.empty())
            return Vec3DF::c_one;

        if (_time < m_keyframesRGB.front().time)
            return m_keyframesRGB.front().value;

        if (_time >= m_keyframesRGB.back().time)
            return m_keyframesRGB.back().value;

        for (S32 i = (S32)m_keyframesRGB.size() - 2; i >= 0; --i)
        {
            KeyframeRGB const& keyframe0 = m_keyframesRGB[i];
            if (_time >= keyframe0.time)
            {
                KeyframeRGB const& keyframe1 = m_keyframesRGB[i + 1];

                return (this->*m_evaluateRGBCallback)(keyframe0, keyframe1, _time);
            }
        }

        return m_keyframesRGB.front().value;
    }

    //////////////////////////////////////////
    inline F32 ColorGradient::evaluateAlpha(F32 _time) const
    {
        if (m_keyframesAlpha.empty())
            return 1.0f;

        if (_time < m_keyframesAlpha.front().time)
            return m_keyframesAlpha.front().value;

        if (_time >= m_keyframesAlpha.back().time)
            return m_keyframesAlpha.back().value;

        for (S32 i = (S32)m_keyframesAlpha.size() - 2; i >= 0; --i)
        {
            KeyframeAlpha const& keyframe0 = m_keyframesAlpha[i];
            if (_time >= keyframe0.time)
            {
                KeyframeAlpha const& keyframe1 = m_keyframesAlpha[i + 1];

                return (this->*m_evaluateAlphaCallback)(keyframe0, keyframe1, _time);
            }
        }

        return m_keyframesAlpha.front().value;
    }

    //////////////////////////////////////////
    inline Vec4DF ColorGradient::evaluate(F32 _time) const
    {
        return 
            {
                evaluateRGB(_time),
                evaluateAlpha(_time)
            };
    }

    //////////////////////////////////////////
    inline void ColorGradient::setMode(EvaluateMode _mode)
    {
        if (m_mode == _mode)
            return;

        m_mode = _mode;

        switch (m_mode)
        {
            case EvaluateMode::Fixed:
            {
                m_evaluateRGBCallback = &ColorGradient::evaluateRGBFixed;
                m_evaluateAlphaCallback = &ColorGradient::evaluateAlphaFixed;
                break;
            }
            case EvaluateMode::Linear:
            {
                m_evaluateRGBCallback = &ColorGradient::evaluateRGBLinear;
                m_evaluateAlphaCallback = &ColorGradient::evaluateAlphaLinear;
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }
    }

    //////////////////////////////////////////
    inline Vec3DF ColorGradient::evaluateRGBFixed(
        ColorGradient::KeyframeRGB const& _keyframe0,
        ColorGradient::KeyframeRGB const& _keyframe1,
        F32 _time) const
    {
        return _keyframe1.value;
    }

    //////////////////////////////////////////
    inline Vec3DF ColorGradient::evaluateRGBLinear(
        ColorGradient::KeyframeRGB const& _keyframe0,
        ColorGradient::KeyframeRGB const& _keyframe1,
        F32 _time) const
    {
        F32 dt = _keyframe1.time - _keyframe0.time;
        F32 t = (_time - _keyframe0.time) / dt;

        return Math::Lerp(_keyframe0.value, _keyframe1.value, t);
    }

    //////////////////////////////////////////
    inline F32 ColorGradient::evaluateAlphaFixed(
        ColorGradient::KeyframeAlpha const& _keyframe0,
        ColorGradient::KeyframeAlpha const& _keyframe1,
        F32 _time) const
    {
        return _keyframe1.value;
    }

    //////////////////////////////////////////
    inline F32 ColorGradient::evaluateAlphaLinear(
        ColorGradient::KeyframeAlpha const& _keyframe0,
        ColorGradient::KeyframeAlpha const& _keyframe1,
        F32 _time) const
    {
        F32 dt = _keyframe1.time - _keyframe0.time;
        F32 t = (_time - _keyframe0.time) / dt;

        return Math::Lerp(_keyframe0.value, _keyframe1.value, t);
    }

} // namespace Maze
//////////////////////////////////////////
