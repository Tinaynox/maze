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
#include "maze-particles/particle-modules/MazeParticleSystem3DShapeModule.hpp"
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
    // Class ParticleSystem3DShapeModule
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ParticleSystem3DShapeModule,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneType, type, ParticleSystem3DZoneType::None, getType, setType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, shapeVolume, false, getShapeVolume, setShapeVolume),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ParticleSystem3DZoneData, zoneData, ParticleSystem3DZoneData(), getZoneData, setZoneData)
        );

    //////////////////////////////////////////
    ParticleSystem3DShapeModule::ParticleSystem3DShapeModule()
        : m_generateRandomPointCallback(&ParticleSystem3DShapeModule::generateRandomPointNone)
    {
         
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::setType(ParticleSystem3DZoneType _value)
    {
        if (m_type == _value)
            return;

        m_type = _value;

        updateShapeCallbacks();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::setShapeVolume(bool _value)
    {
        if (m_shapeVolume == _value)
            return;

        m_shapeVolume = _value;

        updateShapeCallbacks();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::updateInitial(
        Vec3DF const& _emitterPosition,
        Particles3D& _particles,
        S32 _first,
        S32 _last,
        F32 _emitterTimePercent,
        ParticleSystemTransformPolicy _transformPolicy,
        Mat4DF const& _particleSystemWorldTransform)
    {
        ParticleSystem3DZoneEmissionPoint point;

        switch (_transformPolicy)
        {
            case ParticleSystemTransformPolicy::Local:
            {
                for (S32 i = _first; i < _last; i++)
                {
                    generateRandomPoint(point, _emitterPosition);
                    _particles.accessPosition(i) = point.position;
                    _particles.accessDirection(i) = point.direction;
                    _particles.accessSeed(i) = rand() % c_particleSystemParametersCount;
                }
                break;
            }
            case ParticleSystemTransformPolicy::World:
            {
                for (S32 i = _first; i < _last; i++)
                {
                    generateRandomPoint(point, _emitterPosition);
                    _particles.accessPosition(i) = point.position;
                    _particles.accessDirection(i) = point.direction;
                    _particles.accessSeed(i) = rand() % c_particleSystemParametersCount;
                }
                break;
            }
            default:
            {
                break;
            }
        }
        
        
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointNone(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        _result.position = _center;
        _result.direction = Vec3DF::RandomDirection();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointSphereVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneSphere const& zone = m_zoneData.sphere;

        Vec3DF direction = Vec3DF::RandomDirection();
        _result.direction = direction;
        _result.position = _center + direction * zone.radius * Math::UnitRandom();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointSphereShell (
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneSphere const& zone = m_zoneData.sphere;

        Vec3DF direction = Vec3DF::RandomDirection();
        _result.direction = direction;
        _result.position = _center + direction * zone.radius;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointHemisphereVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneHemisphere const& zone = m_zoneData.hemisphere;

        Vec3DF direction = Vec3DF::RandomHemisphereDirection(Vec3DF::c_unitZ);
        _result.direction = direction;
        _result.position = _center + direction * zone.radius * Math::UnitRandom();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointHemisphereShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneHemisphere const& zone = m_zoneData.hemisphere;

        Vec3DF direction = Vec3DF::RandomHemisphereDirection(Vec3DF::c_unitZ);
        _result.direction = direction;
        _result.position = _center + direction * zone.radius;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointTorusVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneTorus const& zone = m_zoneData.torus;

        F32 angle0 = Math::RangeRandom(0.0f, Math::c_twoPi);
        F32 s0 = Math::Sin(angle0);
        F32 c0 = Math::Cos(angle0);

        F32 angle1 = Math::RangeRandom(0.0f, Math::c_twoPi);
        F32 s1 = Math::Sin(angle1);
        F32 c1 = Math::Cos(angle1);

        Vec3DF l(c0, s0, 0.0f);

        Vec3DF shift = l * zone.radius;

        F32 innerRadius = zone.torusRadius * Math::Lerp(zone.radiusThickness, 1.0f, Math::UnitRandom());
        Vec3DF k = l.crossProduct(Vec3DF::c_unitZ);
        Vec3DF t = (l * c1) + (k.crossProduct(l) * s1) + (k * k.dotProduct(l)) * (1 - c1);
        shift += t * innerRadius;

        _result.position = _center + zone.position + shift;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointTorusShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneTorus const& zone = m_zoneData.torus;

        F32 angle0 = Math::RangeRandom(0.0f, Math::c_twoPi);
        F32 s0 = Math::Sin(angle0);
        F32 c0 = Math::Cos(angle0);

        F32 angle1 = Math::RangeRandom(0.0f, Math::c_twoPi);
        F32 s1 = Math::Sin(angle1);
        F32 c1 = Math::Cos(angle1);

        Vec3DF l(c0, s0, 0.0f);

        Vec3DF shift = l * zone.radius;

        F32 innerRadius = zone.torusRadius * (Math::RangeRandom(0, 2) == 0 ? zone.radiusThickness : 1.0f);
        Vec3DF k = l.crossProduct(Vec3DF::c_unitZ);
        Vec3DF t = (l * c1) + (k.crossProduct(l) * s1) + (k * k.dotProduct(l)) * (1 - c1);
        shift += t * innerRadius;

        _result.position = _center + zone.position + shift;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointConeVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneCone const& zone = m_zoneData.cone;

        F32 l = Math::UnitRandom();
        F32 length = zone.length * l;

        F32 finalRadius = zone.radius + tanf(zone.angle) * zone.length;
        F32 radius = zone.radius + tanf(zone.angle) * length;
        F32 u = Math::UnitRandom();
        radius *= u;

        F32 angle = Math::RangeRandom(0.0f, Math::c_twoPi);
        F32 s = Math::Sin(angle);
        F32 c = Math::Cos(angle);

        Vec3DF shift(
            c * radius,
            s * radius,
            length);
        
        _result.position = _center + zone.position + shift;
        _result.direction = 
            Vec3DF
            (
                c * (finalRadius - zone.radius) * u,
                s * (finalRadius - zone.radius) * u,
                zone.length
            ).normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointConeShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneCone const& zone = m_zoneData.cone;

        F32 l = Math::UnitRandom();
        F32 length = zone.length * l;

        F32 finalRadius = zone.radius + tanf(zone.angle) * zone.length;
        F32 radius = zone.radius + tanf(zone.angle) * length;
        
        F32 angle = Math::RangeRandom(0.0f, Math::c_twoPi);
        F32 s = Math::Sin(angle);
        F32 c = Math::Cos(angle);

        Vec3DF shift(
            c * radius,
            s * radius,
            length);

        _result.position = _center + zone.position + shift;
        _result.direction =
            Vec3DF
            (
                c * (finalRadius - zone.radius),
                s * (finalRadius - zone.radius),
                zone.length
            ).normalizedCopy();

        
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointBoxVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneBox const& zone = m_zoneData.box;

        Vec3DF halfScale = zone.scale * 0.5f;

        Vec3DF shift = Vec3DF(
            Math::RangeRandom(-halfScale.x, +halfScale.x),
            Math::RangeRandom(-halfScale.y, +halfScale.y),
            Math::RangeRandom(-halfScale.z, +halfScale.z));

        _result.position = _center + zone.position + shift;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointBoxShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3DF const& _center) const
    {
        ParticleSystem3DZoneBox const& zone = m_zoneData.box;

        Vec3DF halfScale = zone.scale * 0.5f;

        Vec3DF shift;
        switch (Math::RangeRandomMT(0, 6))
        {
            case 0:
            {
                shift = Vec3DF(-halfScale.x, Math::RangeRandom(-halfScale.y, +halfScale.y), Math::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 1:
            {
                shift = Vec3DF(+halfScale.x, Math::RangeRandom(-halfScale.y, +halfScale.y), Math::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 2:
            {
                shift = Vec3DF(Math::RangeRandom(-halfScale.x, +halfScale.x), -halfScale.y, Math::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 3:
            {
                shift = Vec3DF(Math::RangeRandom(-halfScale.x, +halfScale.x), +halfScale.y, Math::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 4:
            {
                shift = Vec3DF(Math::RangeRandom(-halfScale.x, +halfScale.x), Math::RangeRandom(-halfScale.y, +halfScale.y), -halfScale.z);
                break;
            }
            case 5:
            {
                shift = Vec3DF(Math::RangeRandom(-halfScale.x, +halfScale.x), Math::RangeRandom(-halfScale.y, +halfScale.y), +halfScale.z);
                break;
            }
            default:
                break;
        }

        _result.position = _center + zone.position + shift;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::updateShapeCallbacks()
    {
        switch (m_type)
        {
            case ParticleSystem3DZoneType::None:
            {
                m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointNone;
                break;
            }
            case ParticleSystem3DZoneType::Sphere:
            {
                if (m_shapeVolume)
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointSphereVolume;
                else
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointSphereShell;

                break;
            }
            case ParticleSystem3DZoneType::Hemisphere:
            {
                if (m_shapeVolume)
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointHemisphereVolume;
                else
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointHemisphereShell;

                break;
            }
            case ParticleSystem3DZoneType::Cone:
            {
                if (m_shapeVolume)
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointConeVolume;
                else
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointConeShell;

                break;
            }
            case ParticleSystem3DZoneType::Torus:
            {
                if (m_shapeVolume)
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointTorusVolume;
                else
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointTorusShell;

                break;
            }
            case ParticleSystem3DZoneType::Box:
            {
                if (m_shapeVolume)
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointBoxVolume;
                else
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointBoxShell;
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED
                    break;
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
