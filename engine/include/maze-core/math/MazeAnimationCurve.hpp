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
#if (!defined(_MazeAnimationCurve_hpp_))
#define _MazeAnimationCurve_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_CORE_API, AnimationCurveMinMaxMode,
        NormalizedPositive,
        Normalized);


    //////////////////////////////////////////
    // Class AnimationCurve
    //
    //////////////////////////////////////////
    class MAZE_CORE_API AnimationCurve
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        static AnimationCurve const c_linear01;
        static AnimationCurve const c_linear10;
        static AnimationCurve const c_sqr01;
        static AnimationCurve const c_exp01;
        static AnimationCurve const c_smooth01;
        static AnimationCurve const c_smooth10;

    public:

        //////////////////////////////////////////
        // Class Keyframe
        //
        //////////////////////////////////////////
        struct MAZE_CORE_API Keyframe
            : public IJSONValueSerializable
        {
        public:

            //////////////////////////////////////////
            Keyframe()
            {}

            //////////////////////////////////////////
            Keyframe(F32 _time, F32 _value)
                : time(_time)
                , value(_value)
                , inTangent(0.0f)
                , outTangent(0.0f)
            {

            }

            //////////////////////////////////////////
            Keyframe(
                F32 _time,
                F32 _value,
                F32 _inTangent,
                F32 _outTangent)
                : time(_time)
                , value(_value)
                , inTangent(_inTangent)
                , outTangent(_outTangent)
            {

            }

            //////////////////////////////////////////
            static Keyframe FromJSONValue(Json::Value const& _value)
            {
                Keyframe keyframe;
                keyframe.loadFromJSONValue(_value);
                return keyframe;
            }

            //////////////////////////////////////////
            inline bool operator<(Keyframe const& _rhs) const
            {
                return time < _rhs.time;
            }

            //////////////////////////////////////////
            inline bool operator==(Keyframe const& _rhs) const
            {
                return 
                    time == _rhs.time && value == _rhs.value &&
                    inTangent == _rhs.inTangent && outTangent == _rhs.outTangent;
            }

            //////////////////////////////////////////
            inline bool operator!=(Keyframe const& _rhs) const
            {
                return !this->operator==(_rhs);
            }

            //////////////////////////////////////////
            virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

            //////////////////////////////////////////
            virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

            F32 time = 0.0f;
            F32 value = 0.0f;
            F32 inTangent = 0.0f;
            F32 outTangent = 0.0f;
        };

    public:

        //////////////////////////////////////////
        enum class EvaluateMode
        {
            Fixed,
            Linear,
            Smooth,
        };

        //////////////////////////////////////////
        using EvaluteCallback = F32(AnimationCurve::*)(Keyframe const& _keyframe0, Keyframe const& _keyframe1, F32 _time) const;

    public:

        //////////////////////////////////////////
        inline AnimationCurve();

        //////////////////////////////////////////
        inline AnimationCurve(AnimationCurveMinMaxMode _value)
        {
            setMinMaxMode(_value);
        }

        //////////////////////////////////////////
        inline AnimationCurve(F32 _time, F32 _value)
        {
            addKey(_time, _value);
        }

        //////////////////////////////////////////
        inline AnimationCurve(
            F32 _time0, F32 _value0,
            F32 _time1, F32 _value1)
        {
            addKey(_time0, _value0);
            addKey(_time1, _value1);
        }

        //////////////////////////////////////////
        inline AnimationCurve(
            Keyframe const& _value0,
            Keyframe const& _value1,
            EvaluateMode _mode = EvaluateMode::Linear,
            F32 _scalar = 1.0f,
            AnimationCurveMinMaxMode _minMaxMode = AnimationCurveMinMaxMode::NormalizedPositive)
        {
            addKey(_value0);
            addKey(_value1);
            setMode(_mode);
            setScalar(_scalar);
            setMinMaxMode(_minMaxMode);
        }

        //////////////////////////////////////////
        inline FastVector<Keyframe> const& getKeys() const { return m_keyframes; }

        //////////////////////////////////////////
        inline void addKey(Keyframe const& _keyframe)
        {
            m_keyframes.push_back(_keyframe);
            sortKeyframes();
        }

        //////////////////////////////////////////
        inline void addKey(F32 _time, F32 _value)
        {
            m_keyframes.emplace_back(Keyframe{_time, _value});
            sortKeyframes();
        }

        //////////////////////////////////////////
        inline void addKey(F32 _time, F32 _value, F32 _inTangent, F32 _outTangent)
        {
            m_keyframes.emplace_back(Keyframe{ _time, _value, _inTangent, _outTangent });
            sortKeyframes();
        }

        //////////////////////////////////////////
        inline void addKey(F32 _time, F32 _value, F32 _tangent)
        {
            m_keyframes.emplace_back(Keyframe{ _time, _value, _tangent, _tangent });
            sortKeyframes();
        }

        //////////////////////////////////////////
        inline void setKey(Size _index, F32 _value)
        {
            m_keyframes[_index].value = _value;
        }

        //////////////////////////////////////////
        inline void setKeyTime(Size _index, F32 _value)
        {
            m_keyframes[_index].time = _value;
            sortKeyframes();
        }

        //////////////////////////////////////////
        inline void setKeyInTangent(Size _index, F32 _value)
        {
            m_keyframes[_index].inTangent = _value;
        }

        //////////////////////////////////////////
        inline void setKeyOutTangent(Size _index, F32 _value)
        {
            m_keyframes[_index].outTangent = _value;
        }

        //////////////////////////////////////////
        inline void setKeyTangent(Size _index, F32 _value)
        {
            setKeyInTangent(_index, _value);
            setKeyOutTangent(_index, _value);
        }

        //////////////////////////////////////////
        inline void removeKey(Size _index)
        {
            m_keyframes.erase(m_keyframes.begin() + _index);
        }

        //////////////////////////////////////////
        inline void clearKeys()
        {
            m_keyframes.clear();
        }

        //////////////////////////////////////////
        inline Size getInsertionIndex(F32 _time) const
        {
            Size result = 0;

            for (Size i = 0, in = m_keyframes.size(); i < in; ++i)
                if (m_keyframes[i].time < _time)
                    ++result;
                else
                    return result;

            return result;
        }

        //////////////////////////////////////////
        inline bool hasKeyTime(F32 _time) const
        {
            for (Size i = 0, in = m_keyframes.size(); i < in; ++i)
                if (m_keyframes[i].time == _time)
                    return true;

            return false;
        }

        //////////////////////////////////////////
        void normalize();

        //////////////////////////////////////////
        inline AnimationCurve normalized() const
        {
            AnimationCurve curve = *this;
            curve.normalize();
            return curve;
        }

        //////////////////////////////////////////
        void multiplyValues(F32 _value);


        //////////////////////////////////////////
        inline bool empty() const { return m_keyframes.empty(); }


        //////////////////////////////////////////
        inline F32 evaluate(F32 _time) const;

        //////////////////////////////////////////
        inline F32 evaluateUnscaled(F32 _time) const;


        //////////////////////////////////////////
        inline F32 getScalar() const { return m_scalar; }

        //////////////////////////////////////////
        inline void setScalar(F32 _value) { m_scalar = _value; }


        //////////////////////////////////////////
        inline EvaluateMode getMode() const { return m_mode; }

        //////////////////////////////////////////
        inline void setMode(EvaluateMode _mode);


        //////////////////////////////////////////
        inline AnimationCurveMinMaxMode getMinMaxMode() const { return m_minMaxMode; }

        //////////////////////////////////////////
        inline void setMinMaxMode(AnimationCurveMinMaxMode _mode) { m_minMaxMode = _mode; }


        //////////////////////////////////////////
        inline bool operator==(AnimationCurve const& _rhs) const
        {
            if (m_mode != _rhs.m_mode)
                return false;

            if (m_minMaxMode != _rhs.m_minMaxMode)
                return false;

            if (m_scalar != _rhs.m_scalar)
                return false;

            if (m_keyframes.size() != _rhs.m_keyframes.size())
                return false;

            for (Size i = 0, in = m_keyframes.size(); i < in; ++i)
                if (m_keyframes[i] != _rhs.m_keyframes[i])
                    return false;

            return true;
        }

        //////////////////////////////////////////
        inline bool operator!=(AnimationCurve const& _rhs) const
        {
            return !this->operator==(_rhs);
        }

        //////////////////////////////////////////
        F32 getStartTime() const;

        //////////////////////////////////////////
        F32 getEndTime() const;

        //////////////////////////////////////////
        F32 getTime() const;


        //////////////////////////////////////////
        inline String toString() const
        {
            return JSONHelper::ToString(toJSONValue());
        }

        //////////////////////////////////////////
        inline static AnimationCurve FromString(String const& _string)
        {
            AnimationCurve curve;
            curve.loadFromJSONValue(JSONHelper::FromString(_string));
            return curve;
        }

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        inline void sortKeyframes()
        {
            std::sort(m_keyframes.begin(), m_keyframes.end());
        }

        //////////////////////////////////////////
        inline F32 evaluateFixed(
            Keyframe const& _keyframe0,
            Keyframe const& _keyframe1,
            F32 _time) const;

        //////////////////////////////////////////
        inline F32 evaluateLinear(
            Keyframe const& _keyframe0,
            Keyframe const& _keyframe1,
            F32 _time) const;

        //////////////////////////////////////////
        inline F32 evaluateSmooth(
            Keyframe const& _keyframe0,
            Keyframe const& _keyframe1,
            F32 _time) const;

    public:
        FastVector<Keyframe> m_keyframes;
        F32 m_scalar = 1.0f;

        EvaluateMode m_mode = EvaluateMode::Linear;
        EvaluteCallback m_evaluateCallback = &AnimationCurve::evaluateLinear;

        AnimationCurveMinMaxMode m_minMaxMode = AnimationCurveMinMaxMode::NormalizedPositive;
    }; 


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(AnimationCurve const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(AnimationCurve& _value, CString _data, Size _count)
    {
        // TODO:
        _value = AnimationCurve::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(AnimationCurve const& _value)
    {
        return sizeof(AnimationCurve);
    }

    //////////////////////////////////////////
    inline void SerializeValue(AnimationCurve const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(AnimationCurve));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(AnimationCurve& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(AnimationCurve));
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeAnimationCurve.inl"


#endif // _MazeAnimationCurve_hpp_
//////////////////////////////////////////
