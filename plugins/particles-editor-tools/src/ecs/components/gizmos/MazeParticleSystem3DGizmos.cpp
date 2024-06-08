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
#include "MazeParticlesEditorToolsHeader.hpp"
#include "maze-plugin-particles-editor-tools/ecs/components/gizmos/MazeParticleSystem3DGizmos.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DMainModule.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DRendererModule.hpp"
#include "maze-particles/particle-modules/MazeParticleSystem3DShapeModule.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ParticleSystem3DGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ParticleSystem3DGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ParticleSystem3DGizmos);


    //////////////////////////////////////////
    ParticleSystem3DGizmos::ParticleSystem3DGizmos()
    {

    }

    //////////////////////////////////////////
    ParticleSystem3DGizmos::~ParticleSystem3DGizmos()
    {

    }

    //////////////////////////////////////////
    ParticleSystem3DGizmosPtr ParticleSystem3DGizmos::Create()
    {
        ParticleSystem3DGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticleSystem3DGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ParticleSystem3DGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ParticleSystem3DGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        
    }

    //////////////////////////////////////////
    void ParticleSystem3DGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        ParticleSystem3D* particleSystem = _component->castRaw<ParticleSystem3D>();

        Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
        if (transform3D)
        {
            ColorF128 const color(0.79f, 1.0f, 1.0f);

            switch (particleSystem->getMainModule().getTransformPolicy())
            {
                case ParticleSystemSimulationSpace::Local:
                {
                    _drawer->pushTransform(transform3D->getWorldTransform());
                    break;
                }
                case ParticleSystemSimulationSpace::World:
                {
                    _drawer->pushTransform(
                        Mat4F::CreateAffineTranslation(transform3D->getWorldPosition()));
                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                    _drawer->pushTransform(Mat4F::c_identity);
                    break;
                }
            }

            ParticleSystem3DZoneData const& zoneData = particleSystem->getShapeModule().getZoneData();
            switch (particleSystem->getShapeModule().getType())
            {
                case ParticleSystem3DZoneType::None:
                {
                    break;
                }
                case ParticleSystem3DZoneType::Sphere:
                {
                    _drawer->pushTransform(
                        Mat4F::CreateAffineTranslation(zoneData.sphere.position).transformAffine(
                            Mat4F::CreateAffineScale(zoneData.sphere.scale)));
                    _drawer->drawWireSphere(
                        Vec3F::c_zero,
                        zoneData.sphere.radius,
                        color);
                    _drawer->popTransform();
                    break;
                }
                case ParticleSystem3DZoneType::Hemisphere:
                {
                    _drawer->pushTransform(
                        Mat4F::CreateAffineTranslation(zoneData.hemisphere.position).transformAffine(
                            Mat4F::CreateAffineScale(zoneData.hemisphere.scale)));
                    _drawer->drawWireHemisphere(
                        Vec3F::c_zero,
                        Vec3F::c_unitZ,
                        zoneData.hemisphere.radius,
                        color);
                    _drawer->popTransform();
                    break;
                }
                case ParticleSystem3DZoneType::Cone:
                {
                    _drawer->pushTransform(
                        Mat4F::CreateAffineTranslation(zoneData.cone.position).transformAffine(
                            Mat4F::CreateAffineScale(zoneData.cone.scale)));
                    _drawer->drawWireTruncatedCone(
                        Vec3F::c_zero,
                        Vec3F::c_unitZ * zoneData.cone.length,
                        zoneData.cone.radius,
                        zoneData.cone.angle,
                        color);
                    _drawer->popTransform();
                    break;
                }
                case ParticleSystem3DZoneType::Torus:
                {
                    _drawer->pushTransform(
                        Mat4F::CreateAffineTranslation(zoneData.torus.position).transformAffine(
                            Mat4F::CreateAffineScale(zoneData.torus.scale)));
                    _drawer->drawWireTorus(
                        Vec3F::c_zero,
                        Vec3F::c_unitZ,
                        zoneData.torus.radius,
                        zoneData.torus.torusRadius,
                        color,
                        GizmosDrawer::MeshRenderMode::Opaque);

                    if (zoneData.torus.radiusThickness > 0.0f && zoneData.torus.radiusThickness < 1.0f)
                    {
                        Vec3F forward = Vec3F::c_unitZ;
                        Vec3F up = forward.perpendicular();
                        Vec3F right = up.crossProduct(forward).normalizedCopy();

                        F32 radiusThickness = zoneData.torus.torusRadius * (1.0f - zoneData.torus.radiusThickness);

                        ColorF128 radiusThicknessColor(0.49f, 0.62f, 0.62f);
                        _drawer->setColor(radiusThicknessColor);
                        _drawer->drawWireCircle(
                            up * zoneData.torus.radius, right,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                        _drawer->drawWireCircle(
                            -up * zoneData.torus.radius, right,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                        _drawer->drawWireCircle(
                            right * zoneData.torus.radius, up,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                        _drawer->drawWireCircle(
                            -right * zoneData.torus.radius, up,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                    }
                    _drawer->popTransform();
                    break;
                }
                case ParticleSystem3DZoneType::Box:
                {
                    _drawer->drawWireCube(
                        zoneData.box.position,
                        Vec3F::c_unitZ,
                        Vec3F::c_unitY,
                        zoneData.box.scale,
                        color);

                    break;
                }
                case ParticleSystem3DZoneType::Circle:
                {
                    _drawer->pushTransform(
                        Mat4F::CreateAffineTranslation(zoneData.circle.position).transformAffine(
                            Mat4F::CreateAffineScale(zoneData.circle.scale)));
                    _drawer->setColor(color);
                    _drawer->drawWireCircle(
                        Vec3F::c_zero,
                        Vec3F::c_unitZ,
                        zoneData.circle.radius,
                        GizmosDrawer::MeshRenderMode::Opaque);
                    _drawer->popTransform();
                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                    break;
                }
            }

            _drawer->popTransform();
        }

    }

} // namespace Maze
//////////////////////////////////////////
