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
#if (!defined(_MazeParticleSystem3DZone_hpp_))
#define _MazeParticleSystem3DZone_hpp_


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
    MAZE_DECLARE_ENUMCLASS_6_API(MAZE_PARTICLES_API, ParticleSystem3DZoneType,
        Sphere,
        Hemisphere,
        Cone,
        Torus,
        Box,
        Circle);


    //////////////////////////////////////////
    // Class ParticleSystem3DZoneEmissionPoint
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneEmissionPoint
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneEmissionPoint);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneEmissionPoint);

        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setDirection(Vec3F const& _value) { direction = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getDirection() const { return direction; }


        //////////////////////////////////////////
        inline ParticleSystem3DZoneEmissionPoint(
            Vec3F const& _position = Vec3F::c_zero,
            Vec3F const& _direction = Vec3F::c_zero)
            : position(_position)
            , direction(_direction)
        {}

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
        Vec3F position = Vec3F::c_zero;
        Vec3F direction = Vec3F::c_unitZ;
    };


    //////////////////////////////////////////
    // Class ParticleSystem3DZoneSphere
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneSphere
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneSphere);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneSphere);

        //////////////////////////////////////////
        ParticleSystem3DZoneSphere()
        {}


        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setScale(Vec3F const& _value) { scale = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getScale() const { return scale; }

        //////////////////////////////////////////
        inline void setRadius(F32 _value) { radius = _value; }

        //////////////////////////////////////////
        inline F32 getRadius() const { return radius; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneSphere const& _value) const
        {
            return
                position == _value.position && scale == _value.scale &&
                radius == _value.radius;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneSphere const& _value) const
        {
            return !this->operator==(_value);
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

        Vec3F position = Vec3F::c_zero;
        Vec3F scale = Vec3F::c_one;

        F32 radius = 1.0f;
    };

    //////////////////////////////////////////
    // Class ParticleSystem3DZoneHemisphere
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneHemisphere
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneHemisphere);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneHemisphere);


        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setScale(Vec3F const& _value) { scale = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getScale() const { return scale; }

        //////////////////////////////////////////
        inline void setRadius(F32 _value) { radius = _value; }

        //////////////////////////////////////////
        inline F32 getRadius() const { return radius; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneHemisphere const& _value) const
        {
            return
                position == _value.position && scale == _value.scale &&
                radius == _value.radius;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneHemisphere const& _value) const
        {
            return !this->operator==(_value);
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

        Vec3F position = Vec3F::c_zero;
        Vec3F scale = Vec3F::c_one;

        F32 radius = 1.0f;
    };

    //////////////////////////////////////////
    // Class ParticleSystem3DZoneTorus
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneTorus
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneTorus);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneTorus);


        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setScale(Vec3F const& _value) { scale = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getScale() const { return scale; }

        //////////////////////////////////////////
        inline void setRadius(F32 _value) { radius = _value; }

        //////////////////////////////////////////
        inline F32 getRadius() const { return radius; }

        //////////////////////////////////////////
        inline void setTorusRadius(F32 _value) { torusRadius = _value; }

        //////////////////////////////////////////
        inline F32 getTorusRadius() const { return torusRadius; }

        //////////////////////////////////////////
        inline void setRadiusThickness(F32 _value) { radiusThickness = _value; }

        //////////////////////////////////////////
        inline F32 getRadiusThickness() const { return radiusThickness; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneTorus const& _value) const
        {
            return
                position == _value.position && scale == _value.scale &&
                radius == _value.radius && torusRadius == _value.torusRadius &&
                radiusThickness == _value.radiusThickness;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneTorus const& _value) const
        {
            return !this->operator==(_value);
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

        Vec3F position = Vec3F::c_zero;
        Vec3F scale = Vec3F::c_one;

        F32 radius = 0.5f;
        F32 torusRadius = 0.1f;
        F32 radiusThickness = 1.0f;
    };


    //////////////////////////////////////////
    // Class ParticleSystem3DZoneCone
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneCone
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneCone);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneCone);


        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setScale(Vec3F const& _value) { scale = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getScale() const { return scale; }

        //////////////////////////////////////////
        inline void setRadius(F32 _value) { radius = _value; }

        //////////////////////////////////////////
        inline F32 getRadius() const { return radius; }

        //////////////////////////////////////////
        inline void setAngle(F32 _value) { angle = _value; }

        //////////////////////////////////////////
        inline F32 getAngle() const { return angle; }

        //////////////////////////////////////////
        inline void setLength(F32 _value) { length = _value; }

        //////////////////////////////////////////
        inline F32 getLength() const { return length; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneCone const& _value) const
        {
            return
                position == _value.position && scale == _value.scale &&
                radius == _value.radius && angle == _value.angle &&
                length == _value.length;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneCone const& _value) const
        {
            return !this->operator==(_value);
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

        Vec3F position = Vec3F::c_zero;
        Vec3F scale = Vec3F::c_one;

        F32 radius = 0.5f;
        F32 angle = 0.0f;
        F32 length = 5.0f;
    };


    //////////////////////////////////////////
    // Class ParticleSystem3DZoneBox
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneBox
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneBox);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneBox);


        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setScale(Vec3F const& _value) { scale = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getScale() const { return scale; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneBox const& _value) const
        {
            return position == _value.position && scale == _value.scale;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneBox const& _value) const
        {
            return !this->operator==(_value);
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

        Vec3F position = Vec3F::c_zero;
        Vec3F scale = Vec3F::c_one;
    };


    //////////////////////////////////////////
    // Class ParticleSystem3DZoneCircle
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneCircle
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneCircle);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneCircle);

        //////////////////////////////////////////
        ParticleSystem3DZoneCircle()
        {}


        //////////////////////////////////////////
        inline void setPosition(Vec3F const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setScale(Vec3F const& _value) { scale = _value; }

        //////////////////////////////////////////
        inline Vec3F const& getScale() const { return scale; }

        //////////////////////////////////////////
        inline void setRadius(F32 _value) { radius = _value; }

        //////////////////////////////////////////
        inline F32 getRadius() const { return radius; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneCircle const& _value) const
        {
            return
                position == _value.position && scale == _value.scale &&
                radius == _value.radius;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneCircle const& _value) const
        {
            return !this->operator==(_value);
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

        Vec3F position = Vec3F::c_zero;
        Vec3F scale = Vec3F::c_one;

        F32 radius = 1.0f;
    };


    //////////////////////////////////////////
    // Class ParticleSystem3DZoneData
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DZoneData
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DZoneData);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DZoneData);


        //////////////////////////////////////////
        ParticleSystem3DZoneData()
        {}


        //////////////////////////////////////////
        inline void setSphere(ParticleSystem3DZoneSphere const& _value) { sphere = _value; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneSphere const& getSphere() const { return sphere; }

        //////////////////////////////////////////
        inline void setHemisphere(ParticleSystem3DZoneHemisphere const& _value) { hemisphere = _value; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneHemisphere const& getHemisphere() const { return hemisphere; }

        //////////////////////////////////////////
        inline void setTorus(ParticleSystem3DZoneTorus const& _value) { torus = _value; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneTorus const& getTorus() const { return torus; }

        //////////////////////////////////////////
        inline void setCone(ParticleSystem3DZoneCone const& _value) { cone = _value; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneCone const& getCone() const { return cone; }

        //////////////////////////////////////////
        inline void setBox(ParticleSystem3DZoneBox const& _value) { box = _value; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneBox const& getBox() const { return box; }

        //////////////////////////////////////////
        inline void setCircle(ParticleSystem3DZoneCircle const& _value) { circle = _value; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneCircle const& getCircle() const { return circle; }


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DZoneData const& _value) const
        {
            return
                sphere == _value.sphere && hemisphere == _value.hemisphere &&
                torus == _value.torus && cone == _value.cone &&
                box == _value.box;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DZoneData const& _value) const
        {
            return !this->operator==(_value);
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

        ParticleSystem3DZoneSphere sphere;
        ParticleSystem3DZoneHemisphere hemisphere;
        ParticleSystem3DZoneTorus torus;
        ParticleSystem3DZoneCone cone;
        ParticleSystem3DZoneBox box;
        ParticleSystem3DZoneCircle circle;
    };


    //////////////////////////////////////////
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneSphere);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneHemisphere);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneTorus);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneCone);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneBox);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneCircle);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DZoneData);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3DZone_hpp_
//////////////////////////////////////////
