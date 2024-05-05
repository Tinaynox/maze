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
#include "maze-particles/ecs/components/MazeParticlesDrawerController.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/ecs/events/MazeECSGraphicsEvents.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ParticlesDrawerController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ParticlesDrawerController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ParticlesDrawerController);

    //////////////////////////////////////////
    ParticlesDrawerController::ParticlesDrawerController()
    {
    }

    //////////////////////////////////////////
    ParticlesDrawerController::~ParticlesDrawerController()
    {

    }

    //////////////////////////////////////////
    ParticlesDrawerControllerPtr ParticlesDrawerController::Create(RenderSystemPtr const& _renderSystem)
    {
        ParticlesDrawerControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticlesDrawerController, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool ParticlesDrawerController::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    void ParticlesDrawerController::processEntityAwakened()
    {
        m_particleSystem3DSample = getEntityRaw()->getECSWorld()->requestInclusiveSample<ParticleSystem3D>();
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM_EVENT_HANDLER(ParticlesDrawerControllerSystem, {},
        Render3DDefaultPassGatherRenderUnitsEvent const& _event,
        Entity* _entity,
        ParticlesDrawerController* _particlesDrawerController)
    {
        Vec3F cameraPosition = _event.getPassParams()->cameraTransform.getAffineTranslation();
        Mat4F cameraTransform = _event.getPassParams()->cameraTransform;
        cameraTransform[0][3] = 0.0f;
        cameraTransform[1][3] = 0.0f;
        cameraTransform[2][3] = 0.0f;
        Vec3F cameraForward = (cameraTransform.transformAffine(Vec3F::c_unitZ)).normalizedCopy();
        Vec3F cameraUp = (cameraTransform.transformAffine(Vec3F::c_unitY)).normalizedCopy();

        RenderTarget* renderTarget = _event.getRenderTarget();


        _particlesDrawerController->getParticleSystem3DSample()->process(
            [renderTarget, &_event, &cameraPosition, &cameraForward, &cameraUp](Entity* _entity, ParticleSystem3D* _particleSystem)
        {
            if (!(_particleSystem->getRenderMask()->getMask() & _event.getPassParams()->renderMask))
                return;

            S32 aliveParticles = _particleSystem->getAliveParticles();

            if (aliveParticles == 0)
                return;

            MaterialPtr const* material = nullptr;
            if (!_particleSystem->getRendererModule().getMaterial())
                material = &renderTarget->getRenderSystem()->getMaterialManager()->getErrorMaterial();
            else
                material = &_particleSystem->getRendererModule().getMaterial();

            RenderMesh const* renderMesh = _particleSystem->getRendererModule().getRenderMesh().get();
            if (!renderMesh)
            {
                RenderMeshPtr const& quad = renderTarget->getRenderSystem()->getRenderMeshManager()->getDefaultQuadMesh();
                renderMesh = quad.get();
            }

            _particleSystem->prepareToRender(
                cameraPosition,
                cameraForward,
                cameraUp);

            _event.getRenderUnits()->emplace_back(
                RenderUnit
                {
                    (*material)->getFirstRenderPass(),
                    renderMesh->getVertexArrayObject(),
                    _particleSystem->getTransform()->getWorldPosition(),
                    aliveParticles,
                    _particleSystem->getRenderTransforms(),
                    _particleSystem->getRenderColors(),
                    _particleSystem->getRenderUVs()
                });
        });
    }
    
} // namespace Maze
//////////////////////////////////////////
