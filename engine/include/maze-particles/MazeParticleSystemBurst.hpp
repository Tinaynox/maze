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
#if (!defined(_MazeParticleSystemBurst_hpp_))
#define _MazeParticleSystemBurst_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ParticleSystemBurst
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystemBurst
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystemBurst);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystemBurst);

        //////////////////////////////////////////
        inline void setTime(F32 _value) { time = _value; }

        //////////////////////////////////////////
        inline F32 getTime() const { return time; }

        //////////////////////////////////////////
        inline void setMinCount(S32 _value) { minCount = _value; }

        //////////////////////////////////////////
        inline S32 getMinCount() const { return minCount; }

        //////////////////////////////////////////
        inline void setMaxCount(S32 _value) { maxCount = _value; }

        //////////////////////////////////////////
        inline S32 getMaxCount() const { return maxCount; }


        //////////////////////////////////////////
        inline ParticleSystemBurst(
            F32 _time = 0.0f,
            S32 _minCount = 0,
            S32 _maxCount = 0)
            : time(_time)
            , minCount(_minCount)
            , maxCount(_maxCount)
        {}


        //////////////////////////////////////////
        inline bool operator==(ParticleSystemBurst const& _value) const
        {
            return
                time == _value.time &&
                minCount == _value.minCount &&
                maxCount == _value.maxCount;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystemBurst const& _value) const
        {
            return !this->operator==(_value);
        }

        //////////////////////////////////////////
        inline bool operator<(ParticleSystemBurst const& _value) const
        {
            return time < _value.time;
        }


    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
        {
            JSONHelper::DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
        }

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE
        {
            return JSONHelper::SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
        }

    public:
        F32	time = 0.0f;
        S32 minCount = 0;
        S32 maxCount = 0;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(ParticleSystemBurst const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    inline void ValueFromString(ParticleSystemBurst& _value, CString _data, Size _count)
    {
        _value = ParticleSystemBurst::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(ParticleSystemBurst const& _value)
    {
        return sizeof(ParticleSystemBurst);
    }

    //////////////////////////////////////////
    inline void SerializeValue(ParticleSystemBurst const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(ParticleSystemBurst));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(ParticleSystemBurst& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(ParticleSystemBurst));
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystemBurst_hpp_
//////////////////////////////////////////
