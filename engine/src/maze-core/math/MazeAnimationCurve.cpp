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
#include "MazeCoreHeader.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(AnimationCurveMinMaxMode);


    //////////////////////////////////////////
    // Class AnimationCurve
    //
    //////////////////////////////////////////
    AnimationCurve const AnimationCurve::c_linear01 = AnimationCurve(0.0f, 0.0f, 1.0f, 1.0f);
    AnimationCurve const AnimationCurve::c_linear10 = AnimationCurve(0.0f, 1.0f, 1.0f, 0.0f);
    AnimationCurve const AnimationCurve::c_sqr01 = AnimationCurve(
        Keyframe{ 0.0f, 0.0f, 2.5f, 2.5f },
        Keyframe{ 1.0f, 1.0f, 0.0f, 0.0f },
        AnimationCurve::EvaluateMode::Smooth);
    AnimationCurve const AnimationCurve::c_exp01 = AnimationCurve(
        Keyframe{ 0.0f, 0.0f, 0.0f, 0.0f },
        Keyframe{ 1.0f, 1.0f, 2.5f, 2.5f },
        AnimationCurve::EvaluateMode::Smooth);
    AnimationCurve const AnimationCurve::c_smooth01 = AnimationCurve(
        Keyframe{ 0.0f, 0.0f },
        Keyframe{ 1.0f, 1.0f },
        AnimationCurve::EvaluateMode::Smooth);
    AnimationCurve const AnimationCurve::c_smooth10 = AnimationCurve(
        Keyframe{ 0.0f, 1.0f },
        Keyframe{ 1.0f, 0.0f },
        AnimationCurve::EvaluateMode::Smooth);

    //////////////////////////////////////////
    void AnimationCurve::Keyframe::loadFromJSONValue(Json::Value const& _value)
    {
        time = _value["t"].asFloat();
        value = _value["v"].asFloat();
        inTangent = _value["it"].asFloat();
        outTangent = _value["ot"].asFloat();
    }

    //////////////////////////////////////////
    Json::Value AnimationCurve::Keyframe::toJSONValue() const
    {
        Json::Value json;
        json["t"] = time;
        json["v"] = value;
        json["it"] = inTangent;
        json["ot"] = outTangent;
        return json;
    }


    //////////////////////////////////////////
    void AnimationCurve::normalize()
    {
        if (m_keyframes.empty())
            return;

        F32 maxScalar = 0.0f;

        for (Keyframe const& keyframe : m_keyframes)
            maxScalar = Math::Max(maxScalar, Math::Abs(keyframe.value));

        if (maxScalar > 1.0f && maxScalar > m_scalar)
        {
            F32 invMaxScalar = 1.0f / maxScalar;
            for (Keyframe& keyframe : m_keyframes)
                keyframe.value *= invMaxScalar;

            setScalar(maxScalar);
        }
    }

    //////////////////////////////////////////
    void AnimationCurve::multiplyValues(F32 _value)
    {
        for (Keyframe& keyframe : m_keyframes)
            keyframe.value *= _value;
    }

    //////////////////////////////////////////
    F32 AnimationCurve::getStartTime() const
    {
        if (!m_keyframes.empty())
            return m_keyframes.front().time;
        
        return 0.0f;
    }

    //////////////////////////////////////////
    F32 AnimationCurve::getEndTime() const
    {
        if (!m_keyframes.empty())
            return m_keyframes.back().time;

        return 1.0f;
    }

    //////////////////////////////////////////
    F32 AnimationCurve::getTime() const
    {
        return getEndTime() - getStartTime();
    }

    //////////////////////////////////////////
    void AnimationCurve::loadFromJSONValue(Json::Value const& _value)
    {
        m_keyframes.clear();
        for (Json::Value const& keyframe : _value["k"])
        {
            m_keyframes.emplace_back(Keyframe::FromJSONValue(keyframe));
        }
        sortKeyframes();

        m_scalar = _value["s"].asFloat();

        setMode(EvaluateMode(_value["em"].asInt()));
        setMinMaxMode(AnimationCurveMinMaxMode(_value["mm"].asInt()));
    }

    //////////////////////////////////////////
    Json::Value AnimationCurve::toJSONValue() const
    {
        Json::Value value;

        Json::Value& keyframes = value["k"];
        for (Keyframe const& keyframe : m_keyframes)
            keyframes.append(keyframe.toJSONValue());
        value["s"] = m_scalar;
        value["em"] = (S32)m_mode;
        value["mm"] = (S32)m_minMaxMode;
        
        return value;
    }


} // namespace Maze
//////////////////////////////////////////
