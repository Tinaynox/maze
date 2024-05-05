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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeColorGradient.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorGradient
    //
    //////////////////////////////////////////
    void ColorGradient::KeyframeRGB::loadFromJSONValue(Json::Value const& _value)
    {
        time = _value["t"].asFloat();
        value = {
            _value["r"].asFloat(),
            _value["g"].asFloat(),
            _value["b"].asFloat()
        };
    }

    //////////////////////////////////////////
    Json::Value ColorGradient::KeyframeRGB::toJSONValue() const
    {
        Json::Value json;
        json["t"] = time;
        json["r"] = value.x;
        json["g"] = value.y;
        json["b"] = value.z;
        
        return json;
    }

    //////////////////////////////////////////
    bool ColorGradient::KeyframeRGB::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        time = _dataBlock.getF32(MAZE_HCS("time"));
        value = _dataBlock.getVec3F(MAZE_HCS("value"));
        return true;
    }

    //////////////////////////////////////////
    void ColorGradient::KeyframeRGB::toDataBlock(DataBlock& _dataBlock) const
    {
        _dataBlock.setF32(MAZE_HCS("time"), time);
        _dataBlock.setVec3F(MAZE_HCS("value"), value);
    }

    //////////////////////////////////////////
    void ColorGradient::KeyframeAlpha::loadFromJSONValue(Json::Value const& _value)
    {
        time = _value["t"].asFloat();
        value = _value["a"].asFloat();
    }

    //////////////////////////////////////////
    Json::Value ColorGradient::KeyframeAlpha::toJSONValue() const
    {
        Json::Value json;
        json["t"] = time;
        json["a"] = value;

        return json;
    }

    //////////////////////////////////////////
    bool ColorGradient::KeyframeAlpha::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        time = _dataBlock.getF32(MAZE_HCS("time"));
        value = _dataBlock.getF32(MAZE_HCS("value"));
        return true;
    }

    //////////////////////////////////////////
    void ColorGradient::KeyframeAlpha::toDataBlock(DataBlock& _dataBlock) const
    {
        _dataBlock.setF32(MAZE_HCS("time"), time);
        _dataBlock.setF32(MAZE_HCS("value"), value);
    }

    //////////////////////////////////////////
    FastVector<Pair<F32, Vec4F>> ColorGradient::toRawColors() const
    {
        FastVector<Pair<F32, Vec4F>> result;

        std::function<FastVector<Pair<F32, Vec4F>>::iterator(F32 _time)> findKeyframe =
            [&](F32 _time)
            {
                return std::find_if(
                    result.begin(),
                    result.end(),
                    [_time](Pair<F32, Vec4F> const& _value)
                    {
                        return Math::Abs(_value.first - _time) <= 0.00001f;
                    });
            };

        for (KeyframeRGB const& keyframe : m_keyframesRGB)
        {
            F32 time = keyframe.time;
            FastVector<Pair<F32, Vec4F>>::iterator it = findKeyframe(time);

            if (it != result.end())
                continue;
            
            Vec4F color = evaluate(time);
            result.emplace_back(Pair<F32, Vec4F>{time, color});
        }

        for (KeyframeAlpha const& keyframe : m_keyframesAlpha)
        {
            F32 time = keyframe.time;
            FastVector<Pair<F32, Vec4F>>::iterator it = findKeyframe(time);

            if (it != result.end())
                continue;

            Vec4F color = evaluate(time);
            result.emplace_back(Pair<F32, Vec4F>{time, color});
        }

        std::sort(
            result.begin(),
            result.end(),
            [](Pair<F32, Vec4F> const& _a, Pair<F32, Vec4F> const& _b)
            {
                return _a.first < _b.first;
            });

        return result;
    }

    //////////////////////////////////////////
    F32 ColorGradient::getStartTime() const
    {
        bool isRGBEmpty = m_keyframesRGB.empty();
        bool isAlphaEmpty = m_keyframesAlpha.empty();

        if (!isRGBEmpty && !isAlphaEmpty)
            return Math::Min(m_keyframesRGB.front().time, m_keyframesAlpha.front().time);
        else
        if (!isRGBEmpty)
            return m_keyframesRGB.front().time;
        else
        if (!isAlphaEmpty)
            return m_keyframesAlpha.front().time;

        return 0.0f;
    }

    //////////////////////////////////////////
    F32 ColorGradient::getEndTime() const
    {
        bool isRGBEmpty = m_keyframesRGB.empty();
        bool isAlphaEmpty = m_keyframesAlpha.empty();

        if (!isRGBEmpty && !isAlphaEmpty)
            return Math::Max(m_keyframesRGB.back().time, m_keyframesAlpha.back().time);
        else
        if (!isRGBEmpty)
            return m_keyframesRGB.back().time;
        else
        if (!isAlphaEmpty)
            return m_keyframesAlpha.back().time;

        return 1.0f;
    }

    //////////////////////////////////////////
    F32 ColorGradient::getTime() const
    {
        return getEndTime() - getStartTime();
    }

    //////////////////////////////////////////
    void ColorGradient::clamp01()
    {
        for (FastVector<KeyframeRGB>::iterator it = m_keyframesRGB.begin(),
                                               end = m_keyframesRGB.end();
                                               it != end;)
        {
            if ((*it).time < 0.0f || (*it).time > 1.0f)
            {
                it = m_keyframesRGB.erase(it);
                end = m_keyframesRGB.end();
            }
            else
            {
                ++it;
            }
        }

        for (FastVector<KeyframeAlpha>::iterator it = m_keyframesAlpha.begin(),
                                                 end = m_keyframesAlpha.end();
                                                 it != end;)
        {
            if ((*it).time < 0.0f || (*it).time > 1.0f)
            {
                it = m_keyframesAlpha.erase(it);
                end = m_keyframesAlpha.end();
            }
            else
            {
                ++it;
            }
        }
    }

    //////////////////////////////////////////
    void ColorGradient::loadFromJSONValue(Json::Value const& _value)
    {
        if (!_value.isObject())
        {
            clear();
            return;
        }

        m_keyframesRGB.clear();
        for (Json::Value const& keyframe : _value["rgb"])
        {
            m_keyframesRGB.emplace_back(KeyframeRGB::FromJSONValue(keyframe));
        }
        sortKeyframesRGB();

        m_keyframesAlpha.clear();
        for (Json::Value const& keyframe : _value["a"])
        {
            m_keyframesAlpha.emplace_back(KeyframeAlpha::FromJSONValue(keyframe));
        }
        sortKeyframesAlpha();

        setMode(EvaluateMode(_value["em"].asInt()));
    }

    //////////////////////////////////////////
    Json::Value ColorGradient::toJSONValue() const
    {
        Json::Value value;

        Json::Value& keyframesRGB = value["rgb"];
        for (KeyframeRGB keyframe : m_keyframesRGB)
            keyframesRGB.append(keyframe.toJSONValue());

        Json::Value& keyframesAlpha = value["a"];
        for (KeyframeAlpha keyframe : m_keyframesAlpha)
            keyframesAlpha.append(keyframe.toJSONValue());

        value["em"] = (S32)m_mode;

        return value;
    }

    //////////////////////////////////////////
    bool ColorGradient::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        m_keyframesRGB.clear();
        DataBlock const* keyframesRGBDataBlock = _dataBlock.getDataBlock(MAZE_HCS("keyframesRGB"));
        if (keyframesRGBDataBlock)
        {
            ValueFromDataBlock(m_keyframesRGB, *keyframesRGBDataBlock);
            sortKeyframesRGB();
        }

        m_keyframesAlpha.clear();
        DataBlock const* keyframesAlphaDataBlock = _dataBlock.getDataBlock(MAZE_HCS("keyframesAlpha"));
        if (keyframesAlphaDataBlock)
        {
            ValueFromDataBlock(m_keyframesAlpha, *keyframesAlphaDataBlock);
            sortKeyframesAlpha();
        }

        setMode(EvaluateMode(_dataBlock.getS32(MAZE_HCS("mode"))));

        return true;
    }

    //////////////////////////////////////////
    void ColorGradient::toDataBlock(DataBlock& _dataBlock) const
    {
        DataBlock& keyframesRGBDataBlock = _dataBlock[MAZE_HCS("keyframesRGB")];
        ValueToDataBlock(m_keyframesRGB, keyframesRGBDataBlock);

        DataBlock& keyframesAlphaDataBlock = _dataBlock[MAZE_HCS("keyframesAlpha")];
        ValueToDataBlock(m_keyframesAlpha, keyframesAlphaDataBlock);

        _dataBlock.setS32(MAZE_HCS("mode"), (S32)m_mode);
    }

    
    //////////////////////////////////////////
    String ColorGradient::toString() const
    {
        return JSONHelper::ToString(toJSONValue());
    }

    //////////////////////////////////////////
    void ColorGradient::setString(CString _data, Size _count)
    {
        (*this) = ColorGradient::FromString(String(_data, _count));
    }


} // namespace Maze
//////////////////////////////////////////
