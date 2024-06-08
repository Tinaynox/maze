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
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/math/MazeRandom.hpp"
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
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"


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
        Vec3F const& _emitterPosition,
        Particles3D& _particles,
        S32 _first,
        S32 _last,
        F32 _emitterTimePercent,
        ParticleSystemSimulationSpace _transformPolicy,
        ParticleSystemScalingMode _scalingMode,
        Mat4F const& _particleSystemLocalTransform,
        Mat4F const& _particleSystemWorldTransform)
    {
        ParticleSystem3DZoneEmissionPoint point;

        Mat4F directionTransformMat;

        if (_transformPolicy == ParticleSystemSimulationSpace::World)
        {
            if (_scalingMode == ParticleSystemScalingMode::Hierarchy)
            {
                directionTransformMat = _particleSystemWorldTransform;
                directionTransformMat.setTranslation(Vec3F::c_zero);
            }
            else
            {
                //Vec3F rotation = _particleSystemWorldTransform.getAffineRotationEulerAngles();
                Vec3F rotation = Quaternion::GetEuler(_particleSystemWorldTransform);
                directionTransformMat = Mat4F::CreateAffineRotation(rotation);
            }
            
            
        }

        switch (_transformPolicy)
        {
            case ParticleSystemSimulationSpace::Local:
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
            case ParticleSystemSimulationSpace::World:
            {
                for (S32 i = _first; i < _last; i++)
                {
                    generateRandomPoint(point, _emitterPosition);
                    _particles.accessPosition(i) = point.position;
                    _particles.accessDirection(i) = directionTransformMat.transformAffine(point.direction).normalizedCopy();
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
        Vec3F const& _center) const
    {
        _result.position = _center;
        //_result.direction = Vec3F::RandomDirection();
        _result.direction = Vec3F::c_unitZ;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointSphereVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneSphere const& zone = m_zoneData.sphere;

        Vec3F direction = Vec3F::RandomDirection();
        _result.direction = direction;
        F32 r = Random::UnitRandomRadius();
        _result.position = _center + zone.position + direction * r * zone.radius * zone.scale;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointSphereShell (
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneSphere const& zone = m_zoneData.sphere;

        Vec3F direction = Vec3F::RandomDirection();
        _result.direction = direction;
        _result.position = _center + zone.position + direction * zone.radius * zone.scale;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointCircleVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneCircle const& zone = m_zoneData.circle;

        Vec2F direction = Vec2F::RandomDirection();
        _result.direction = direction;
        F32 r = Random::UnitRandomRadius();
        _result.position = _center + zone.position + Vec3F(direction) * r * zone.radius * zone.scale;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointCircleShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneCircle const& zone = m_zoneData.circle;

        Vec2F direction = Vec2F::RandomDirection();
        _result.direction = direction;
        _result.position = _center + zone.position + Vec3F(direction) * zone.radius * zone.scale;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointHemisphereVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneHemisphere const& zone = m_zoneData.hemisphere;

        Vec3F direction = Vec3F::RandomHemisphereDirection(Vec3F::c_unitZ);
        _result.direction = direction;
        F32 r = Random::UnitRandomRadius();
        _result.position = _center + zone.position + direction * r * zone.radius * zone.scale;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointHemisphereShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneHemisphere const& zone = m_zoneData.hemisphere;

        Vec3F direction = Vec3F::RandomHemisphereDirection(Vec3F::c_unitZ);
        _result.direction = direction;
        _result.position = _center + zone.position + direction * zone.radius * zone.scale;
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointTorusVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneTorus const& zone = m_zoneData.torus;

        F32 angle0 = Random::RangeRandom(0.0f, Math::c_twoPi);
        F32 s0 = Math::Sin(angle0);
        F32 c0 = Math::Cos(angle0);

        F32 angle1 = Random::RangeRandom(0.0f, Math::c_twoPi);
        F32 s1 = Math::Sin(angle1);
        F32 c1 = Math::Cos(angle1);

        Vec3F l(c0, s0, 0.0f);

        Vec3F shift = l * zone.radius;

        F32 innerRadius = zone.torusRadius * Math::Lerp(Math::Clamp01(1.0f - zone.radiusThickness), 1.0f, Random::UnitRandom());
        Vec3F k = l.crossProduct(Vec3F::c_unitZ);
        Vec3F t = (l * c1) + (k.crossProduct(l) * s1) + (k * k.dotProduct(l)) * (1 - c1);
        shift += t * innerRadius;

        _result.position = _center + zone.position + shift * zone.scale;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointTorusShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneTorus const& zone = m_zoneData.torus;

        F32 angle0 = Random::RangeRandom(0.0f, Math::c_twoPi);
        F32 s0 = Math::Sin(angle0);
        F32 c0 = Math::Cos(angle0);

        F32 angle1 = Random::RangeRandom(0.0f, Math::c_twoPi);
        F32 s1 = Math::Sin(angle1);
        F32 c1 = Math::Cos(angle1);

        Vec3F l(c0, s0, 0.0f);

        Vec3F shift = l * zone.radius;

        F32 innerRadius = zone.torusRadius * (Random::RangeRandom(0, 2) == 0 ? Math::Clamp01(1.0f - zone.radiusThickness) : 1.0f);
        Vec3F k = l.crossProduct(Vec3F::c_unitZ);
        Vec3F t = (l * c1) + (k.crossProduct(l) * s1) + (k * k.dotProduct(l)) * (1 - c1);
        shift += t * innerRadius;

        _result.position = _center + zone.position + shift * zone.scale;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointConeVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneCone const& zone = m_zoneData.cone;

        F32 l = Random::UnitRandom();
        F32 length = zone.length * l;

        F32 finalRadius = zone.radius + tanf(zone.angle) * zone.length;
        F32 radius = zone.radius + tanf(zone.angle) * length;
        F32 u = Random::UnitRandomRadius();
        radius *= u;

        F32 angle = Random::RangeRandom(0.0f, Math::c_twoPi);
        F32 s = Math::Sin(angle);
        F32 c = Math::Cos(angle);

        Vec3F shift(
            c * radius,
            s * radius,
            length);
        
        _result.position = _center + zone.position + shift * zone.scale;
        _result.direction = 
            Vec3F
            (
                c * (finalRadius - zone.radius) * u,
                s * (finalRadius - zone.radius) * u,
                zone.length
            ).normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointConeShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneCone const& zone = m_zoneData.cone;

        F32 l = Random::UnitRandom();
        F32 length = zone.length * l;

        F32 finalRadius = zone.radius + tanf(zone.angle) * zone.length;
        F32 radius = zone.radius + tanf(zone.angle) * length;
        
        F32 angle = Random::RangeRandom(0.0f, Math::c_twoPi);
        F32 s = Math::Sin(angle);
        F32 c = Math::Cos(angle);

        Vec3F shift(
            c * radius,
            s * radius,
            length);

        _result.position = _center + zone.position + shift * zone.scale;
        _result.direction =
            Vec3F
            (
                c * (finalRadius - zone.radius),
                s * (finalRadius - zone.radius),
                zone.length
            ).normalizedCopy();

        
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointBoxVolume(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneBox const& zone = m_zoneData.box;

        Vec3F halfScale = zone.scale * 0.5f;

        Vec3F shift = Vec3F(
            Random::RangeRandom(-halfScale.x, +halfScale.x),
            Random::RangeRandom(-halfScale.y, +halfScale.y),
            Random::RangeRandom(-halfScale.z, +halfScale.z));

        _result.position = _center + zone.position + shift;
        _result.direction = shift.normalizedCopy();
    }

    //////////////////////////////////////////
    void ParticleSystem3DShapeModule::generateRandomPointBoxShell(
        ParticleSystem3DZoneEmissionPoint& _result,
        Vec3F const& _center) const
    {
        ParticleSystem3DZoneBox const& zone = m_zoneData.box;

        Vec3F halfScale = zone.scale * 0.5f;

        Vec3F shift;
        switch (Random::RangeRandomMT(0, 6))
        {
            case 0:
            {
                shift = Vec3F(-halfScale.x, Random::RangeRandom(-halfScale.y, +halfScale.y), Random::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 1:
            {
                shift = Vec3F(+halfScale.x, Random::RangeRandom(-halfScale.y, +halfScale.y), Random::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 2:
            {
                shift = Vec3F(Random::RangeRandom(-halfScale.x, +halfScale.x), -halfScale.y, Random::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 3:
            {
                shift = Vec3F(Random::RangeRandom(-halfScale.x, +halfScale.x), +halfScale.y, Random::RangeRandom(-halfScale.z, +halfScale.z));
                break;
            }
            case 4:
            {
                shift = Vec3F(Random::RangeRandom(-halfScale.x, +halfScale.x), Random::RangeRandom(-halfScale.y, +halfScale.y), -halfScale.z);
                break;
            }
            case 5:
            {
                shift = Vec3F(Random::RangeRandom(-halfScale.x, +halfScale.x), Random::RangeRandom(-halfScale.y, +halfScale.y), +halfScale.z);
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
            case ParticleSystem3DZoneType::Circle:
            {
                if (m_shapeVolume)
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointCircleVolume;
                else
                    m_generateRandomPointCallback = &ParticleSystem3DShapeModule::generateRandomPointCircleShell;

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
