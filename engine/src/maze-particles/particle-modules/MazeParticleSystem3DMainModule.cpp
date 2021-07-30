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
#include "MazeParticlesHeader.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DMainModule.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ParticleSystem3DMainModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, duration, 5.0f, getDuration, setDuration),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, looped, true, getLooped, setLooped),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, prewarm, true, getPrewarm, setPrewarm),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, playOnAwake, true, getPlayOnAwake, setPlayOnAwake),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemTransformPolicy, transformPolicy, ParticleSystemTransformPolicy::Local, getTransformPolicy, setTransformPolicy),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, lifetime, ParticleSystemParameterF32(), getLifetime, setLifetime),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, size, ParticleSystemParameterF32(), getSize, setSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterColor, color, ParticleSystemParameterColor(), getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, speed, ParticleSystemParameterF32(), getSpeed, setSpeed),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, gravity, ParticleSystemParameterF32(), getGravity, setGravity),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, rotation, ParticleSystemParameterF32(), getRotation, setRotation),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(EmissionModule, emission, EmissionModule(), getEmission, setEmission),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SizeOverLifetimeModule, sizeOverLifetime, SizeOverLifetimeModule(), getSizeOverLifetime, setSizeOverLifetime),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(VelocityOverLifetimeModule, velocityOverLifetime, VelocityOverLifetimeModule(), getVelocityOverLifetime, setVelocityOverLifetime),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(VelocityLimitOverLifetimeModule, velocityLimitOverLifetime, VelocityLimitOverLifetimeModule(), getVelocityLimitOverLifetime, setVelocityLimitOverLifetime),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(RotationOverLifetimeModule, rotationOverLifetime, RotationOverLifetimeModule(), getRotationOverLifetime, setRotationOverLifetime),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorOverLifetimeModule, colorOverLifetime, ColorOverLifetimeModule(), getColorOverLifetime, setColorOverLifetime));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule::EmissionModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, true, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, emissionPerSecond, ParticleSystemParameterF32(), getEmissionPerSecond, setEmissionPerSecond),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<ParticleSystemBurst>, bursts, Vector<ParticleSystemBurst>(), getBursts, setBursts));

    //////////////////////////////////////////
    void ParticleSystem3DMainModule::EmissionModule::sortBursts()
    {
        std::sort(bursts.begin(), bursts.end());
    }

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule::SizeOverLifetimeModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, false, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, parameter, ParticleSystemParameterF32(), getParameter, setParameter));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule::VelocityOverLifetimeModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, false, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, linearXParameter, ParticleSystemParameterF32(), getLinearXParameter, setLinearXParameter),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, linearYParameter, ParticleSystemParameterF32(), getLinearYParameter, setLinearYParameter),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, linearZParameter, ParticleSystemParameterF32(), getLinearZParameter, setLinearZParameter));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule::VelocityLimitOverLifetimeModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, false, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, parameter, ParticleSystemParameterF32(), getParameter, setParameter));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule::RotationOverLifetimeModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, false, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterF32, parameter, ParticleSystemParameterF32(), getParameter, setParameter));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DMainModule::ColorOverLifetimeModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, enabled, false, getEnabled, setEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystemParameterColor, parameter, ParticleSystemParameterColor(), getParameter, setParameter));


    //////////////////////////////////////////
    void ParticleSystem3DMainModule::updateInitial(
        Particles3D& _particles,
        S32 _first,
        S32 _last,
        F32 _emitterTimePercent,
        Mat4DF const& _particleSystemWorldTransform)
    {
        Mat4DF invParticleSystemWorldTransform = _particleSystemWorldTransform.inversedAffineCopy();
        invParticleSystemWorldTransform[0][3] = 0.0f;
        invParticleSystemWorldTransform[1][3] = 0.0f;
        
        // Life
        {
            F32 valueF32;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                m_lifetime.sample(seed, _emitterTimePercent, valueF32);

                Particles3D::ParticleLife& life = _particles.accessLife(i);
                life.initial = valueF32;
                life.current = valueF32;
            }
        }

        // Size
        {
            F32 valueF32;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                m_size.sample(seed, _emitterTimePercent, valueF32);

                Particles3D::ParticleSize& size = _particles.accessSize(i);
                size.initial = valueF32;
                size.current = valueF32;
            }
        }

        // Rotation
        {
            F32 valueF32;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                m_rotation.sample(seed, _emitterTimePercent, valueF32);

                Particles3D::ParticleRotation& rotation = _particles.accessRotation(i);
                rotation.initial = valueF32;
                rotation.current = valueF32;
            }
        }

        // Color
        {
            Vec4DF value;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                m_color.sample(seed, _emitterTimePercent, value);

                _particles.accessColorInitial(i) = value;
                _particles.accessColorCurrent(i) = value;
            }
        }

        // Movement
        {
            F32 speedValue;
            F32 gravityValue;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                m_speed.sample(seed, _emitterTimePercent, speedValue);
                m_gravity.sample(seed, _emitterTimePercent, gravityValue);

                Particles3D::ParticleMovement& movement = _particles.accessMovement(i);
                Vec3DF& direction = _particles.accessDirection(i);
                movement.velocity = direction * speedValue;
                movement.acceleration = invParticleSystemWorldTransform.transformAffine(Vec3DF::c_unitY) * gravityValue;
            }
        }
    }

    //////////////////////////////////////////
    void ParticleSystem3DMainModule::updateLifetime(
        Particles3D& _particles,
        S32 _first,
        S32 _last,
        F32 _dt)
    {

        // Life
        for (S32 i = _first; i < _last; ++i)
        {
            Particles3D::ParticleLife& life = _particles.accessLife(i);

            F32 currentLife = life.current;
            currentLife -= _dt;

            life.scalar = Math::Clamp01(1.0f - currentLife / life.initial);

            life.current = currentLife;
        }

        // Size over lifetime
        if (m_sizeOverLifetime.enabled)
        {
            F32 value;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                Particles3D::ParticleLife& life = _particles.accessLife(i);
                Particles3D::ParticleSize& size = _particles.accessSize(i);

                m_sizeOverLifetime.parameter.sample(seed, life.scalar, value);
                size.current = size.initial * value;
            }
        }

        // Velocity over lifetime
        if (m_velocityOverLifetime.enabled)
        {
            Vec3DF value;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                Particles3D::ParticleLife& life = _particles.accessLife(i);
                Particles3D::ParticleMovement& movement = _particles.accessMovement(i);

                m_velocityOverLifetime.linearXParameter.sample(seed, life.scalar, value.x);
                m_velocityOverLifetime.linearYParameter.sample(seed, life.scalar, value.y);
                m_velocityOverLifetime.linearZParameter.sample(seed, life.scalar, value.z);

                movement.velocity += value * _dt;
            }
        }

        // Velocity limit
        if (m_velocityLimitOverLifetime.enabled)
        {
            F32 value;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                Particles3D::ParticleLife& life = _particles.accessLife(i);
                Particles3D::ParticleMovement& movement = _particles.accessMovement(i);

                m_velocityLimitOverLifetime.parameter.sample(seed, life.scalar, value);

                F32 velocityLength = movement.velocity.length();

                if (velocityLength <= value)
                    continue;

                if (velocityLength == 0.0f)
                    continue;

                movement.velocity *= value / velocityLength;
            }
        }

        // Rotation over lifetime
        if (m_rotationOverLifetime.enabled)
        {
            F32 value;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                Particles3D::ParticleLife& life = _particles.accessLife(i);
                Particles3D::ParticleRotation& rotation = _particles.accessRotation(i);

                m_rotationOverLifetime.parameter.sample(seed, life.scalar, value);

                rotation.current += value * _dt;
            }
        }

        // Color over lifetime
        if (m_colorOverLifetime.enabled)
        {
            Vec4DF value;
            for (S32 i = _first; i < _last; ++i)
            {
                S32 seed = _particles.accessSeed(i);
                Particles3D::ParticleLife& life = _particles.accessLife(i);
                Vec4DF const& initalColor = _particles.accessColorInitial(i);
                Vec4DF& currentColor = _particles.accessColorCurrent(i);

                m_colorOverLifetime.parameter.sample(seed, life.scalar, value);

                currentColor = initalColor * value;
            }
        }

        // Position
        for (S32 i = _first; i < _last; ++i)
        {
            Particles3D::ParticleMovement& movement = _particles.accessMovement(i);
            Vec3DF& position = _particles.accessPosition(i);

            movement.velocity += movement.acceleration * _dt;

            position += movement.velocity * _dt;
        }

        
    }

    //////////////////////////////////////////
    void ParticleSystem3DMainModule::loadFromJSONValue(Json::Value const& _value)
    {
        DeserializeMetaInstanceFromJSONValue(getMetaClass(), getMetaInstance(), _value);
    }

    //////////////////////////////////////////
    Json::Value ParticleSystem3DMainModule::toJSONValue() const
    {
        return SerializeMetaInstanceToJSONValue(getMetaClass(), getMetaInstance());
    }

} // namespace Maze
//////////////////////////////////////////
