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
            ColorF128 color(0.79f, 1.0f, 1.0f);

            // #TODO: Scale

            ParticleSystem3DZoneData const& zoneData = particleSystem->getShapeModule().getZoneData();
            switch (particleSystem->getShapeModule().getType())
            {
                case ParticleSystem3DZoneType::Sphere:
                {
                    _drawer->drawWireSphere(
                        transform3D->getWorldPosition(),
                        zoneData.sphere.radius,
                        color);
                    break;
                }
                case ParticleSystem3DZoneType::Hemisphere:
                {
                    _drawer->drawWireHemisphere(
                        transform3D->getWorldPosition(),
                        transform3D->getWorldForwardDirection(),
                        zoneData.hemisphere.radius,
                        color);
                    break;
                }
                case ParticleSystem3DZoneType::Cone:
                {
                    _drawer->drawWireTruncatedCone(
                        transform3D->getWorldPosition(),
                        transform3D->getWorldForwardDirection() * zoneData.cone.length,
                        zoneData.cone.radius,
                        zoneData.cone.angle,
                        color);
                    break;
                }
                case ParticleSystem3DZoneType::Torus:
                {
                    _drawer->drawWireTorus(
                        transform3D->getWorldPosition(),
                        transform3D->getWorldForwardDirection(),
                        zoneData.torus.radius,
                        zoneData.torus.torusRadius,
                        color,
                        GizmosDrawer::MeshRenderMode::Opaque);

                    if (zoneData.torus.radiusThickness > 0.0f && zoneData.torus.radiusThickness < 1.0f)
                    {
                        Vec3DF forward = transform3D->getWorldForwardDirection();
                        Vec3DF up = forward.perpendicular();
                        Vec3DF right = up.crossProduct(forward).normalizedCopy();

                        F32 radiusThickness = zoneData.torus.torusRadius * (1.0f - zoneData.torus.radiusThickness);

                        ColorF128 radiusThicknessColor(0.49f, 0.62f, 0.62f);
                        _drawer->setColor(radiusThicknessColor);
                        _drawer->drawWireCircle(
                            transform3D->getWorldPosition() + up * zoneData.torus.radius, right,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                        _drawer->drawWireCircle(
                            transform3D->getWorldPosition() - up * zoneData.torus.radius, right,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                        _drawer->drawWireCircle(
                            transform3D->getWorldPosition() + right * zoneData.torus.radius, up,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                        _drawer->drawWireCircle(
                            transform3D->getWorldPosition() - right * zoneData.torus.radius, up,
                            radiusThickness, GizmosDrawer::MeshRenderMode::Opaque);
                    }
                    break;
                }
                case ParticleSystem3DZoneType::Box:
                {
                    _drawer->drawWireCube(
                        transform3D->getWorldPosition(),
                        zoneData.box.scale,
                        transform3D->getWorldForwardDirection(),
                        transform3D->getWorldUpDirection(),
                        color);

                    break;
                }
                case ParticleSystem3DZoneType::Circle:
                {
                    _drawer->setColor(color);
                    _drawer->drawWireCircle(
                        transform3D->getWorldPosition(),
                        transform3D->getWorldForwardDirection(),
                        zoneData.circle.radius,
                        GizmosDrawer::MeshRenderMode::Opaque);

                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                    break;
                }
            }
        }

    }

} // namespace Maze
//////////////////////////////////////////
