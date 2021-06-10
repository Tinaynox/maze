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
#include "MazeAnimationCurve.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AnimationCurve
    //
    //////////////////////////////////////////
    inline AnimationCurve::AnimationCurve()
    {
    }

    //////////////////////////////////////////
    inline F32 AnimationCurve::evaluate(F32 _time) const
    {
        F32 unscaledValue = evaluateUnscaled(_time);

        switch (m_minMaxMode)
        {
            case AnimationCurveMinMaxMode::NormalizedPositive:
                return unscaledValue * m_scalar;

            case AnimationCurveMinMaxMode::Normalized:
                return unscaledValue * m_scalar;

            default:
                return unscaledValue;
        }
    }

    //////////////////////////////////////////
    inline F32 AnimationCurve::evaluateUnscaled(F32 _time) const
    {
        if (m_keyframes.empty())
            return 0.0f;

        if (_time < m_keyframes.front().time)
            return m_keyframes.front().value;

        if (_time >= m_keyframes.back().time)
            return m_keyframes.back().value;

        for (S32 i = (S32)m_keyframes.size() - 2; i >= 0; --i)
        {
            Keyframe const& keyframe0 = m_keyframes[i];
            if (_time >= keyframe0.time)
            {
                Keyframe const& keyframe1 = m_keyframes[i + 1];

                return (this->*m_evaluateCallback)(keyframe0, keyframe1, _time);
            }
        }

        return m_keyframes.front().value;
    }

    //////////////////////////////////////////
    inline void AnimationCurve::setMode(EvaluateMode _mode)
    {
        if (m_mode == _mode)
            return;

        m_mode = _mode;

        switch (m_mode)
        {
            case EvaluateMode::Fixed:
            {
                m_evaluateCallback = &AnimationCurve::evaluateFixed;
                break;
            }
            case EvaluateMode::Linear:
            {
                m_evaluateCallback = &AnimationCurve::evaluateLinear;
                break;
            }
            case EvaluateMode::Smooth:
            {
                m_evaluateCallback = &AnimationCurve::evaluateSmooth;
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
    inline F32 AnimationCurve::evaluateFixed(
        AnimationCurve::Keyframe const& _keyframe0,
        AnimationCurve::Keyframe const& _keyframe1,
        F32 _time) const
    {
        return _keyframe1.value;
    }

    //////////////////////////////////////////
    inline F32 AnimationCurve::evaluateLinear(
        AnimationCurve::Keyframe const& _keyframe0,
        AnimationCurve::Keyframe const& _keyframe1,
        F32 _time) const
    {
        F32 dt = _keyframe1.time - _keyframe0.time;
        F32 t = (_time - _keyframe0.time) / dt;

        return Math::Lerp(_keyframe0.value, _keyframe1.value, t);
    }

    //////////////////////////////////////////
    inline F32 AnimationCurve::evaluateSmooth(
        AnimationCurve::Keyframe const& _keyframe0,
        AnimationCurve::Keyframe const& _keyframe1,
        F32 _time) const
    {
        F32 dt = _keyframe1.time - _keyframe0.time;
        F32 t = (_time - _keyframe0.time) / dt;

        F32 m0 = _keyframe0.outTangent * dt;
        F32 m1 = _keyframe1.inTangent * dt;

        F32 t2 = t * t;
        F32 t3 = t2 * t;

        F32 a = 2.0f * t3 - 3 * t2 + 1;
        F32 b = t3 - 2 * t2 + t;
        F32 c = t3 - t2;
        F32 d = -2.0f * t3 + 3 * t2;

        return a * _keyframe0.value + b * m0 + c * m1 + d * _keyframe1.value;
    }

} // namespace Maze
//////////////////////////////////////////
