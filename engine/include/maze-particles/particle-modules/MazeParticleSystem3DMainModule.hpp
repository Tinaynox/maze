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
#if (!defined(_MazeParticleSystem3DMainModule_hpp_))
#define _MazeParticleSystem3DMainModule_hpp_


//////////////////////////////////////////
#include "maze-particles/MazeParticlesHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-particles/MazeParticleSystemState.hpp"
#include "maze-particles/MazeParticleSystemParameterF32.hpp"
#include "maze-particles/MazeParticleSystemParameterColor.hpp"
#include "maze-particles/MazeParticleSystemBurst.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"
#include "maze-core/helpers/MazeJSONHelper.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/helpers/MazeDataBlockHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(ParticleSystem3DMainModule);
    MAZE_USING_SHARED_PTR(RenderMask);
    

    //////////////////////////////////////////
    // Class ParticleSystem3DMainModule
    //
    //////////////////////////////////////////
    class MAZE_PARTICLES_API ParticleSystem3DMainModule
        : public IJSONValueSerializable
        , public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ParticleSystem3DMainModule);

        //////////////////////////////////////////
        MAZE_JSON_STRING_SERIALIZATION(ParticleSystem3DMainModule);


    public:

        //////////////////////////////////////////
        class MAZE_PARTICLES_API EmissionModule
            : public IJSONValueSerializable
            , public IDataBlockSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(EmissionModule);

            //////////////////////////////////////////
            MAZE_JSON_STRING_SERIALIZATION(EmissionModule);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }

            //////////////////////////////////////////
            inline void setEmissionPerSecond(ParticleSystemParameterF32 const& _parameter) { emissionPerSecond = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32& getEmissionPerSecond() { return emissionPerSecond; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getEmissionPerSecond() const { return emissionPerSecond; }

            //////////////////////////////////////////
            inline void setEmissionPerDistance(ParticleSystemParameterF32 const& _parameter) { emissionPerDistance = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32& getEmissionPerDistance() { return emissionPerDistance; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getEmissionPerDistance() const { return emissionPerDistance; }


            //////////////////////////////////////////
            inline void setBursts(Vector<ParticleSystemBurst> const& _value)
            {
                bursts = _value;
                sortBursts();
            }

            //////////////////////////////////////////
            inline Vector<ParticleSystemBurst>& getBursts() { return bursts; }

            //////////////////////////////////////////
            inline Vector<ParticleSystemBurst> const& getBursts() const { return bursts; }


            //////////////////////////////////////////
            inline bool operator==(EmissionModule const& _value) const
            {
                if (bursts != _value.bursts)
                    return false;

                return enabled == _value.enabled &&
                       emissionPerSecond == _value.emissionPerSecond &&
                       emissionPerDistance == _value.emissionPerDistance;
            }

            //////////////////////////////////////////
            inline bool operator!=(EmissionModule const& _value){ return !this->operator==(_value); }

            //////////////////////////////////////////
            void sortBursts();

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
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = true;
            ParticleSystemParameterF32 emissionPerSecond = ParticleSystemParameterF32(1.0f);
            ParticleSystemParameterF32 emissionPerDistance = ParticleSystemParameterF32(0.0f);
            Vector<ParticleSystemBurst> bursts;
        };

        //////////////////////////////////////////
        class MAZE_PARTICLES_API SizeOverLifetimeModule
            : public IJSONValueSerializable
            , public IDataBlockSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(SizeOverLifetimeModule);

            //////////////////////////////////////////
            MAZE_JSON_STRING_SERIALIZATION(SizeOverLifetimeModule);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }

            //////////////////////////////////////////
            inline void setParameter(ParticleSystemParameterF32 const& _parameter) { parameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getParameter() const { return parameter; }

            //////////////////////////////////////////
            inline bool operator==(SizeOverLifetimeModule const& _value) const { return enabled == _value.enabled && parameter == _value.parameter; }

            //////////////////////////////////////////
            inline bool operator!=(SizeOverLifetimeModule const& _value) const { return !this->operator==(_value); }


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
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = false;
            ParticleSystemParameterF32 parameter = ParticleSystemParameterF32(1.0f);
        };

        //////////////////////////////////////////
        class MAZE_PARTICLES_API VelocityOverLifetimeModule
            : public IJSONValueSerializable
            , public IDataBlockSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(VelocityOverLifetimeModule);

            //////////////////////////////////////////
            MAZE_JSON_STRING_SERIALIZATION(VelocityOverLifetimeModule);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }

            //////////////////////////////////////////
            inline void setLinearXParameter(ParticleSystemParameterF32 const& _parameter) { linearXParameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getLinearXParameter() const { return linearXParameter; }

            //////////////////////////////////////////
            inline void setLinearYParameter(ParticleSystemParameterF32 const& _parameter) { linearYParameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getLinearYParameter() const { return linearYParameter; }

            //////////////////////////////////////////
            inline void setLinearZParameter(ParticleSystemParameterF32 const& _parameter) { linearZParameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getLinearZParameter() const { return linearZParameter; }

            //////////////////////////////////////////
            inline bool operator==(VelocityOverLifetimeModule const& _value) const
            {
                return enabled == _value.enabled &&
                    linearXParameter == _value.linearXParameter &&
                    linearYParameter == _value.linearYParameter &&
                    linearZParameter == _value.linearZParameter;
            }

            //////////////////////////////////////////
            inline bool operator!=(VelocityOverLifetimeModule const& _value) { return !this->operator==(_value); }

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
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = false;
            ParticleSystemParameterF32 linearXParameter = ParticleSystemParameterF32(0.0f);
            ParticleSystemParameterF32 linearYParameter = ParticleSystemParameterF32(0.0f);
            ParticleSystemParameterF32 linearZParameter = ParticleSystemParameterF32(0.0f);
        };

        //////////////////////////////////////////
        class MAZE_PARTICLES_API VelocityLimitOverLifetimeModule
            : public IJSONValueSerializable
            , public IDataBlockSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(VelocityLimitOverLifetimeModule);

            //////////////////////////////////////////
            MAZE_JSON_STRING_SERIALIZATION(VelocityLimitOverLifetimeModule);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }

            //////////////////////////////////////////
            inline void setParameter(ParticleSystemParameterF32 const& _parameter) { parameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getParameter() const { return parameter; }

            //////////////////////////////////////////
            inline bool operator==(VelocityLimitOverLifetimeModule const& _value) const { return enabled == _value.enabled && parameter == _value.parameter; }

            //////////////////////////////////////////
            inline bool operator!=(VelocityLimitOverLifetimeModule const& _value) const { return !this->operator==(_value); }


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
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = false;
            ParticleSystemParameterF32 parameter = ParticleSystemParameterF32(0.0f);
        };

        //////////////////////////////////////////
        class MAZE_PARTICLES_API RotationOverLifetimeModule
            : public IJSONValueSerializable
            , public IDataBlockSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(RotationOverLifetimeModule);

            //////////////////////////////////////////
            MAZE_JSON_STRING_SERIALIZATION(RotationOverLifetimeModule);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }

            //////////////////////////////////////////
            inline void setParameter(ParticleSystemParameterF32 const& _parameter) { parameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterF32 const& getParameter() const { return parameter; }

            //////////////////////////////////////////
            inline bool operator==(RotationOverLifetimeModule const& _value) const { return enabled == _value.enabled && parameter == _value.parameter; }

            //////////////////////////////////////////
            inline bool operator!=(RotationOverLifetimeModule const& _value) const { return !this->operator==(_value); }

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
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = false;
            ParticleSystemParameterF32 parameter = ParticleSystemParameterF32(0.0f);
        };

        //////////////////////////////////////////
        class MAZE_PARTICLES_API ColorOverLifetimeModule
            : public IJSONValueSerializable
            , public IDataBlockSerializable
        {
        public:

            //////////////////////////////////////////
            MAZE_DECLARE_METACLASS(ColorOverLifetimeModule);

            ////////////////////////////////////////// 
            MAZE_JSON_STRING_SERIALIZATION(ColorOverLifetimeModule);

            //////////////////////////////////////////
            inline void setEnabled(bool _enabled) { enabled = _enabled; }

            //////////////////////////////////////////
            inline bool getEnabled() const { return enabled; }

            //////////////////////////////////////////
            inline void setParameter(ParticleSystemParameterColor const& _parameter) { parameter = _parameter; }

            //////////////////////////////////////////
            inline ParticleSystemParameterColor const& getParameter() const { return parameter; }

            //////////////////////////////////////////
            inline bool operator==(ColorOverLifetimeModule const& _value) const { return enabled == _value.enabled && parameter == _value.parameter; }

            //////////////////////////////////////////
            inline bool operator!=(ColorOverLifetimeModule const& _value) const { return !this->operator==(_value); }

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
            //////////////////////////////////////////
            virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE
            {
                DataBlockHelper::DeserializeMetaInstanceFromDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
                return true;
            }

            //////////////////////////////////////////
            virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE
            {
                DataBlockHelper::SerializeMetaInstanceToDataBlock(getMetaClass(), getMetaInstance(), _dataBlock);
            }

        public:

            bool enabled = false;
            ParticleSystemParameterColor parameter = ParticleSystemParameterColor(Vec4F::c_one);
        };
    

    public:

        //////////////////////////////////////////
        inline F32 getCurrentDuration() const { return m_currentDuration; }


        //////////////////////////////////////////
        inline ParticleSystemParameterF32& getDuration() { return m_duration; }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32 const& getDuration() const { return m_duration; }

        //////////////////////////////////////////
        void setDuration(ParticleSystemParameterF32 const& _value) { m_duration = _value; }


        //////////////////////////////////////////
        inline bool getLooped() const { return m_looped; }

        //////////////////////////////////////////
        inline void setLooped(bool _value) { m_looped = _value; }

        //////////////////////////////////////////
        inline bool getPrewarm() const { return m_prewarm; }

        //////////////////////////////////////////
        inline void setPrewarm(bool _value) { m_prewarm = _value; }

        //////////////////////////////////////////
        inline bool getPlayOnAwake() const { return m_playOnAwake; }

        //////////////////////////////////////////
        inline void setPlayOnAwake(bool _value) { m_playOnAwake = _value; }

        //////////////////////////////////////////
        inline bool getAlignToDirection() const { return m_alignToDirection; }

        //////////////////////////////////////////
        inline void setAlignToDirection(bool _value) { m_alignToDirection = _value; }


        //////////////////////////////////////////
        inline ParticleSystemSimulationSpace getTransformPolicy() const { return m_transformPolicy; }

        //////////////////////////////////////////
        inline void setTransformPolicy(ParticleSystemSimulationSpace _value) { m_transformPolicy = _value; }

        //////////////////////////////////////////
        inline ParticleSystemScalingMode getScalingMode() const { return m_scalingMode; }

        //////////////////////////////////////////
        inline void setScalingMode(ParticleSystemScalingMode _value) { m_scalingMode = _value; }


        //////////////////////////////////////////
        inline ParticleSystemParameterF32& getLifetime() { return m_lifetime; }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32 const& getLifetime() const { return m_lifetime; }

        //////////////////////////////////////////
        void setLifetime(ParticleSystemParameterF32 const& _value) { m_lifetime = _value; }


        //////////////////////////////////////////
        inline ParticleSystemParameterF32& getSize() { return m_size; }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32 const& getSize() const { return m_size; }

        //////////////////////////////////////////
        void setSize(ParticleSystemParameterF32 const& _value) { m_size = _value; }


        //////////////////////////////////////////
        inline ParticleSystemParameterColor& getColor() { return m_color; }

        //////////////////////////////////////////
        inline ParticleSystemParameterColor const& getColor() const { return m_color; }

        //////////////////////////////////////////
        void setColor(ParticleSystemParameterColor const& _value) { m_color = _value; }


        //////////////////////////////////////////
        inline ParticleSystemParameterF32& getSpeed() { return m_speed; }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32 const& getSpeed() const { return m_speed; }

        //////////////////////////////////////////
        void setSpeed(ParticleSystemParameterF32 const& _value) { m_speed = _value; }


        //////////////////////////////////////////
        inline ParticleSystemParameterF32& getGravity() { return m_gravity; }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32 const& getGravity() const { return m_gravity; }

        //////////////////////////////////////////
        void setGravity(ParticleSystemParameterF32 const& _value) { m_gravity = _value; }


        //////////////////////////////////////////
        inline ParticleSystemParameterF32& getRotation() { return m_rotation; }

        //////////////////////////////////////////
        inline ParticleSystemParameterF32 const& getRotation() const { return m_rotation; }

        //////////////////////////////////////////
        void setRotation(ParticleSystemParameterF32 const& _value) { m_rotation = _value; }



        //////////////////////////////////////////
        inline EmissionModule& getEmission() { return m_emission; }

        //////////////////////////////////////////
        inline EmissionModule const& getEmission() const { return m_emission; }

        //////////////////////////////////////////
        void setEmission(EmissionModule const& _value) { m_emission = _value; }


        //////////////////////////////////////////
        inline SizeOverLifetimeModule& getSizeOverLifetime() { return m_sizeOverLifetime; }

        //////////////////////////////////////////
        inline SizeOverLifetimeModule const& getSizeOverLifetime() const { return m_sizeOverLifetime; }

        //////////////////////////////////////////
        void setSizeOverLifetime(SizeOverLifetimeModule const& _value) { m_sizeOverLifetime = _value; }

        
        //////////////////////////////////////////
        inline VelocityOverLifetimeModule& getVelocityOverLifetime() { return m_velocityOverLifetime; }

        //////////////////////////////////////////
        inline VelocityOverLifetimeModule const& getVelocityOverLifetime() const { return m_velocityOverLifetime; }

        //////////////////////////////////////////
        void setVelocityOverLifetime(VelocityOverLifetimeModule const& _value) { m_velocityOverLifetime = _value; }


        //////////////////////////////////////////
        inline VelocityLimitOverLifetimeModule& getVelocityLimitOverLifetime() { return m_velocityLimitOverLifetime; }

        //////////////////////////////////////////
        inline VelocityLimitOverLifetimeModule const& getVelocityLimitOverLifetime() const { return m_velocityLimitOverLifetime; }

        //////////////////////////////////////////
        void setVelocityLimitOverLifetime(VelocityLimitOverLifetimeModule const& _value) { m_velocityLimitOverLifetime = _value; }


        //////////////////////////////////////////
        inline RotationOverLifetimeModule& getRotationOverLifetime() { return m_rotationOverLifetime; }

        //////////////////////////////////////////
        inline RotationOverLifetimeModule const& getRotationOverLifetime() const { return m_rotationOverLifetime; }

        //////////////////////////////////////////
        void setRotationOverLifetime(RotationOverLifetimeModule const& _value) { m_rotationOverLifetime = _value; }


        //////////////////////////////////////////
        inline ColorOverLifetimeModule& getColorOverLifetime() { return m_colorOverLifetime; }

        //////////////////////////////////////////
        inline ColorOverLifetimeModule const& getColorOverLifetime() const { return m_colorOverLifetime; }

        //////////////////////////////////////////
        void setColorOverLifetime(ColorOverLifetimeModule const& _value) { m_colorOverLifetime = _value; }


        //////////////////////////////////////////
        void updateInitial(
            Particles3D& _particles,
            S32 _first,
            S32 _last,
            F32 _emitterTimePercent,
            Mat4F const& _particleSystemWorldTransform);

        //////////////////////////////////////////
        void updateLifetime(
            Particles3D& _particles,
            S32 _first,
            S32 _last,
            F32 _dt);


        //////////////////////////////////////////
        bool operator==(ParticleSystem3DMainModule const& _module) const;

        //////////////////////////////////////////
        inline bool operator!=(ParticleSystem3DMainModule const& _module) const { return !this->operator==(_module); }

        //////////////////////////////////////////
        void generateDuration();

    public:

        //////////////////////////////////////////
        virtual void loadFromJSONValue(Json::Value const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Json::Value toJSONValue() const MAZE_OVERRIDE;

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    protected:
        F32 m_currentDuration = 5.0f;

        ParticleSystemParameterF32 m_duration = ParticleSystemParameterF32(5.0f);
        bool m_looped = true;
        bool m_prewarm = false;
        bool m_playOnAwake = true;
        ParticleSystemSimulationSpace m_transformPolicy = ParticleSystemSimulationSpace::Local;
        ParticleSystemScalingMode m_scalingMode = ParticleSystemScalingMode::Hierarchy;

        ParticleSystemParameterF32 m_lifetime = ParticleSystemParameterF32(1.0f);
        ParticleSystemParameterF32 m_size = ParticleSystemParameterF32(1.0f);
        ParticleSystemParameterColor m_color = ParticleSystemParameterColor(Vec4F::c_one);
        ParticleSystemParameterF32 m_speed = ParticleSystemParameterF32(0.0f);
        ParticleSystemParameterF32 m_gravity = ParticleSystemParameterF32(-9.8f);
        ParticleSystemParameterF32 m_rotation = ParticleSystemParameterF32(0.0f);
        bool m_alignToDirection = false;

        EmissionModule m_emission;

        SizeOverLifetimeModule m_sizeOverLifetime;
        VelocityOverLifetimeModule m_velocityOverLifetime;
        VelocityLimitOverLifetimeModule m_velocityLimitOverLifetime;
        RotationOverLifetimeModule m_rotationOverLifetime;
        ColorOverLifetimeModule m_colorOverLifetime;
    };


    //////////////////////////////////////////
    /*MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule::EmissionModule);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule::SizeOverLifetimeModule);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule::VelocityOverLifetimeModule);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule::VelocityLimitOverLifetimeModule);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule::RotationOverLifetimeModule);
    MAZE_IMPLEMENT_STRING_SERIALIZATION(ParticleSystem3DMainModule::ColorOverLifetimeModule);*/


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeParticleSystem3DMainModule_hpp_
//////////////////////////////////////////
