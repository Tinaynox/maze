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
#if (!defined(_MazeColorGradient_hpp_))
#define _MazeColorGradient_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/math/MazeMat3D.hpp"
#include "maze-core/math/MazeMat4D.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class ColorGradient
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ColorGradient
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        // Class KeyframeRGB
        //
        //////////////////////////////////////////
        struct MAZE_GRAPHICS_API KeyframeRGB
            : public IJSONValueSerializable
        {
        public:

            //////////////////////////////////////////
            KeyframeRGB()
            { }

            //////////////////////////////////////////
            KeyframeRGB(F32 _time, Vec3DF const& _value)
                : time(_time)
                , value(_value)
            {

            }

            //////////////////////////////////////////
            static KeyframeRGB FromJSONValue(Json::Value const& _value)
            {
                KeyframeRGB keyframe;
                keyframe.loadFromJSONValue(_value);
                return keyframe;
            }

            //////////////////////////////////////////
            inline bool operator<(KeyframeRGB const& _rhs) const
            {
                return time < _rhs.time;
            }

            //////////////////////////////////////////
            inline bool operator==(KeyframeRGB const& _rhs) const
            {
                return time == _rhs.time && value == _rhs.value;
            }

            //////////////////////////////////////////
            inline bool operator!=(KeyframeRGB const& _rhs) const
            {
                return !this->operator==(_rhs);
            }

            //////////////////////////////////////////
            virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

            //////////////////////////////////////////
            virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

            F32 time = 0.0f;
            Vec3DF value = Vec3DF::c_zero;
        };

        //////////////////////////////////////////
        // Class KeyframeAlpha
        //
        //////////////////////////////////////////
        struct MAZE_GRAPHICS_API KeyframeAlpha
            : public IJSONValueSerializable
        {
        public:

            //////////////////////////////////////////
            KeyframeAlpha()
            { }

            //////////////////////////////////////////
            KeyframeAlpha(F32 _time, F32 _value)
                : time(_time)
                , value(_value)
            {

            }

            //////////////////////////////////////////
            static KeyframeAlpha FromJSONValue(Json::Value const& _value)
            {
                KeyframeAlpha keyframe;
                keyframe.loadFromJSONValue(_value);
                return keyframe;
            }

            //////////////////////////////////////////
            inline bool operator<(KeyframeAlpha const& _rhs) const
            {
                return time < _rhs.time;
            }

            //////////////////////////////////////////
            inline bool operator==(KeyframeAlpha const& _rhs) const
            {
                return time == _rhs.time && value == _rhs.value;
            }

            //////////////////////////////////////////
            inline bool operator!=(KeyframeAlpha const& _rhs) const
            {
                return !this->operator==(_rhs);
            }

            //////////////////////////////////////////
            virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

            //////////////////////////////////////////
            virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

            F32 time = 0.0f;
            F32 value = 0.0f;
        };

    public:

        //////////////////////////////////////////
        enum class EvaluateMode
        {
            Fixed,
            Linear,
        };

        //////////////////////////////////////////
        using EvaluteRGBCallback = Vec3DF(ColorGradient::*)(KeyframeRGB const& _keyframe0, KeyframeRGB const& _keyframe1, F32 _time) const;
        using EvaluteAlphaCallback = F32(ColorGradient::*)(KeyframeAlpha const& _keyframe0, KeyframeAlpha const& _keyframe1, F32 _time) const;

    public:

        //////////////////////////////////////////
        inline ColorGradient();

        //////////////////////////////////////////
        inline ColorGradient(
            F32 _time0,
            Vec4DF const& _color0);

        //////////////////////////////////////////
        inline ColorGradient(Vec4DF const& _color0);

        //////////////////////////////////////////
        inline ColorGradient(
            F32 _time0,
            Vec4DF const& _color0,
            F32 _time1,
            Vec4DF const& _color1);


        //////////////////////////////////////////
        inline FastVector<KeyframeRGB> const& getKeysRGB() const { return m_keyframesRGB; }

        //////////////////////////////////////////
        inline void addKeyRGB(KeyframeRGB const& _keyframe)
        {
            m_keyframesRGB.push_back(_keyframe);
            sortKeyframesRGB();
        }

        //////////////////////////////////////////
        inline void addKeyRGB(F32 _time, Vec3DF const& _value)
        {
            m_keyframesRGB.emplace_back(KeyframeRGB{_time, _value});
            sortKeyframesRGB();
        }

        //////////////////////////////////////////
        inline void setKeyRGB(Size _index, Vec3DF const& _value)
        {
            m_keyframesRGB[_index].value = _value;
        }

        //////////////////////////////////////////
        inline void setKeyRGBTime(Size _index, F32 _value)
        {
            m_keyframesRGB[_index].time = _value;
            sortKeyframesRGB();
        }

        //////////////////////////////////////////
        inline void removeKeyRGB(Size _index)
        {
            m_keyframesRGB.erase(m_keyframesRGB.begin() + _index);
        }

        //////////////////////////////////////////
        inline void clearKeysRGB()
        {
            m_keyframesRGB.clear();
        }

        //////////////////////////////////////////
        inline Size getInsertionIndexRGB(F32 _time) const
        {
            Size result = 0;

            for (Size i = 0, in = m_keyframesRGB.size(); i < in; ++i)
                if (m_keyframesRGB[i].time < _time)
                    ++result;
                else
                    return result;

            return result;
        }

        //////////////////////////////////////////
        inline bool hasKeyRGBTime(F32 _time) const
        {
            for (Size i = 0, in = m_keyframesRGB.size(); i < in; ++i)
                if (m_keyframesRGB[i].time == _time)
                    return true;

            return false;
        }


        //////////////////////////////////////////
        inline FastVector<KeyframeAlpha> const& getKeysAlpha() const { return m_keyframesAlpha; }

        //////////////////////////////////////////
        inline void addKeyAlpha(KeyframeAlpha const& _keyframe)
        {
            m_keyframesAlpha.push_back(_keyframe);
            sortKeyframesRGB();
        }

        //////////////////////////////////////////
        inline void addKeyAlpha(F32 _time, F32 _value)
        {
            m_keyframesAlpha.emplace_back(KeyframeAlpha{ _time, _value });
            sortKeyframesAlpha();
        }

        //////////////////////////////////////////
        inline void setKeyAlpha(Size _index, F32 _value)
        {
            m_keyframesAlpha[_index].value = _value;
        }

        //////////////////////////////////////////
        inline void setKeyAlphaTime(Size _index, F32 _value)
        {
            m_keyframesAlpha[_index].time = _value;
            sortKeyframesAlpha();
        }

        //////////////////////////////////////////
        inline void removeKeyAlpha(Size _index)
        {
            m_keyframesAlpha.erase(m_keyframesAlpha.begin() + _index);
        }

        //////////////////////////////////////////
        inline void clearKeysAlpha()
        {
            m_keyframesAlpha.clear();
        }

        //////////////////////////////////////////
        inline Size getInsertionIndexAlpha(F32 _time) const
        {
            Size result = 0;

            for (Size i = 0, in = m_keyframesAlpha.size(); i < in; ++i)
                if (m_keyframesAlpha[i].time < _time)
                    ++result;
                else
                    return result;

            return result;
        }

        //////////////////////////////////////////
        inline bool hasKeyAlphaTime(F32 _time) const
        {
            for (Size i = 0, in = m_keyframesAlpha.size(); i < in; ++i)
                if (m_keyframesAlpha[i].time == _time)
                    return true;

            return false;
        }


        //////////////////////////////////////////
        inline void addKey(F32 _time, Vec4DF const& _value)
        {
            addKeyRGB(_time, Vec3DF{ _value.x, _value.y, _value.z });
            addKeyAlpha(_time, _value.w);
        }

        //////////////////////////////////////////
        inline bool empty() const { return emptyRGB() && emptyAlpha(); }

        //////////////////////////////////////////
        inline bool emptyRGB() const { return m_keyframesRGB.empty(); }

        //////////////////////////////////////////
        inline bool emptyAlpha() const { return m_keyframesAlpha.empty(); }


        //////////////////////////////////////////
        inline Vec3DF evaluateRGB(F32 _time) const;

        //////////////////////////////////////////
        inline F32 evaluateAlpha(F32 _time) const;

        //////////////////////////////////////////
        inline Vec4DF evaluate(F32 _time) const;


        //////////////////////////////////////////
        inline EvaluateMode getMode() const { return m_mode; }

        //////////////////////////////////////////
        inline void setMode(EvaluateMode _mode);


        //////////////////////////////////////////
        inline bool operator==(ColorGradient const& _rhs) const
        {
            if (m_mode != _rhs.m_mode)
                return false;

            if (m_keyframesRGB.size() != _rhs.m_keyframesRGB.size())
                return false;

            if (m_keyframesAlpha.size() != _rhs.m_keyframesAlpha.size())
                return false;

            for (Size i = 0, in = m_keyframesRGB.size(); i < in; ++i)
                if (m_keyframesRGB[i] != _rhs.m_keyframesRGB[i])
                    return false;

            for (Size i = 0, in = m_keyframesAlpha.size(); i < in; ++i)
                if (m_keyframesAlpha[i] != _rhs.m_keyframesAlpha[i])
                    return false;

            return true;
        }

        //////////////////////////////////////////
        inline bool operator!=(ColorGradient const& _rhs) const
        {
            return !this->operator==(_rhs);
        }

        //////////////////////////////////////////
        FastVector<Pair<F32, Vec4DF>> toRawColors() const;

        //////////////////////////////////////////
        F32 getStartTime() const;

        //////////////////////////////////////////
        F32 getEndTime() const;

        //////////////////////////////////////////
        F32 getTime() const;

        //////////////////////////////////////////
        void clamp01();


        //////////////////////////////////////////
        inline String toString() const
        {
            return JSONHelper::ToString(toJSONValue());
        }

        //////////////////////////////////////////
        inline static ColorGradient FromString(String const& _string)
        {
            ColorGradient gradient;
            gradient.loadFromJSONValue(JSONHelper::FromString(_string));
            return gradient;
        }

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        inline void sortKeyframesRGB()
        {
            std::sort(m_keyframesRGB.begin(), m_keyframesRGB.end());
        }

        //////////////////////////////////////////
        inline void sortKeyframesAlpha()
        {
            std::sort(m_keyframesAlpha.begin(), m_keyframesAlpha.end());
        }

        //////////////////////////////////////////
        inline Vec3DF evaluateRGBFixed(
            KeyframeRGB const& _keyframe0,
            KeyframeRGB const& _keyframe1,
            F32 _time) const;

        //////////////////////////////////////////
        inline Vec3DF evaluateRGBLinear(
            KeyframeRGB const& _keyframe0,
            KeyframeRGB const& _keyframe1,
            F32 _time) const;


        //////////////////////////////////////////
        inline F32 evaluateAlphaFixed(
            KeyframeAlpha const& _keyframe0,
            KeyframeAlpha const& _keyframe1,
            F32 _time) const;

        //////////////////////////////////////////
        inline F32 evaluateAlphaLinear(
            KeyframeAlpha const& _keyframe0,
            KeyframeAlpha const& _keyframe1,
            F32 _time) const;

    public:
        FastVector<KeyframeRGB> m_keyframesRGB;
        FastVector<KeyframeAlpha> m_keyframesAlpha;

        EvaluateMode m_mode = EvaluateMode::Linear;
        EvaluteRGBCallback m_evaluateRGBCallback = &ColorGradient::evaluateRGBLinear;
        EvaluteAlphaCallback m_evaluateAlphaCallback = &ColorGradient::evaluateAlphaLinear;
    }; 


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(ColorGradient const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(ColorGradient& _value, CString _data, Size _count)
    {
        _value = ColorGradient::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(ColorGradient const& _value)
    {
        return sizeof(ColorGradient);
    }

    //////////////////////////////////////////
    inline void SerializeValue(ColorGradient const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(ColorGradient));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(ColorGradient& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(ColorGradient));
    }

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeColorGradient.inl"


#endif // _MazeColorGradient_hpp_
//////////////////////////////////////////
