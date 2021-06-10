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
#if (!defined(_MazeParticleSystem3DShapeModule_hpp_))
#define _MazeParticleSystem3DShapeModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-particles/MazeParticleSystemState.hpp"
#include "maze-particles/MazeParticleSystemParameter.hpp"
#include "maze-particles/MazeParticleSystem3DZone.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(ParticleSystem3DShapeModule);
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    // Class ParticleSystem3DShapeModule
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DShapeModule
        : public IJSONValueSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DShapeModule);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DShapeModule);

        //////////////////////////////////////////
        using GenerateRandomPointCallback = void (ParticleSystem3DShapeModule::*)(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

    public:

        //////////////////////////////////////////
        ParticleSystem3DShapeModule();

        //////////////////////////////////////////
        inline ParticleSystem3DZoneType getType() const { return m_type; }

        //////////////////////////////////////////
        void setType(ParticleSystem3DZoneType _value);


        //////////////////////////////////////////
        inline bool getShapeVolume() const { return m_shapeVolume; }

        //////////////////////////////////////////
        void setShapeVolume(bool _value);


        //////////////////////////////////////////
        inline ParticleSystem3DZoneData& getZoneData() { return m_zoneData; }

        //////////////////////////////////////////
        inline ParticleSystem3DZoneData const& getZoneData() const { return m_zoneData; }

        //////////////////////////////////////////
        inline void setZoneData(ParticleSystem3DZoneData const& _value) { m_zoneData = _value; }


        //////////////////////////////////////////
        inline void generateRandomPoint(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const
        {
            (this->*m_generateRandomPointCallback)(_result, _center);
        }

        //////////////////////////////////////////
        void updateInitial(
            Vec3DF const& _emitterPosition,
            Particles3D& _particles,
            S32 _first,
            S32 _last,
            F32 _emitterTimePercent,
            ParticleSystemTransformPolicy _transformPolicy,
            Mat4DF const& _particleSystemWorldTransform);


        //////////////////////////////////////////
        inline bool operator==(ParticleSystem3DShapeModule const& _value) const
        {
            return
                m_type == _value.m_type &&
                m_shapeVolume == _value.m_shapeVolume &&
                m_zoneData == _value.m_zoneData;
        }

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DShapeModule const& _value) const
        {
            return !this->operator==(_value);
        }

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE
        {
            DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
        }

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE
        {
            return SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
        }

    protected:
        
        //////////////////////////////////////////
        void generateRandomPointNone(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointSphereVolume(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointSphereShell(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointHemisphereVolume(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointHemisphereShell(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointTorusVolume(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointTorusShell(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointConeVolume(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointConeShell(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointBoxVolume(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void generateRandomPointBoxShell(
            ParticleSystem3DZoneEmissionPoint& _result,
            Vec3DF const& _center) const;

        //////////////////////////////////////////
        void updateShapeCallbacks();

    protected:
        ParticleSystem3DZoneType m_type = ParticleSystem3DZoneType::None;
        bool m_shapeVolume = false;
        ParticleSystem3DZoneData m_zoneData;

        GenerateRandomPointCallback m_generateRandomPointCallback;
    };


    //////////////////////////////////////////
    MAZE_STRING_ONLY_SERIALIZATION(ParticleSystem3DShapeModule);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3DShapeModule_hpp_
//////////////////////////////////////////
