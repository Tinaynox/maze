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
#include "maze-particles/ecs/systems/MazeParticlesDrawerSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule2D.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule3D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ParticlesDrawerSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ParticlesDrawerSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ParticlesDrawerSystem);

    //////////////////////////////////////////
    ParticlesDrawerSystem::ParticlesDrawerSystem()
    {
    }

    //////////////////////////////////////////
    ParticlesDrawerSystem::~ParticlesDrawerSystem()
    {
        if (m_renderControlSystem)
        {
            RenderControlSystemModule3DPtr const& module3D = m_renderControlSystem->getModule3D();
            module3D->eventGatherRenderUnits.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    ParticlesDrawerSystemPtr ParticlesDrawerSystem::Create(RenderSystemPtr const& _renderSystem)
    {
        ParticlesDrawerSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ParticlesDrawerSystem, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool ParticlesDrawerSystem::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    void ParticlesDrawerSystem::processSystemAdded()
    {
        m_particleSystem3DSample = m_worldRaw->requestInclusiveSample<ParticleSystem3D>();

        m_renderControlSystem = m_worldRaw->getSystem<RenderControlSystem>();

        RenderControlSystemModule3DPtr const& module3D = m_renderControlSystem->getModule3D();
        module3D->eventGatherRenderUnits.subscribe(this, &ParticlesDrawerSystem::notifyGatherRenderUnits);
    }

    //////////////////////////////////////////
    void ParticlesDrawerSystem::processUpdate(F32 _dt)
    {
        m_particleSystem3DSample->process(
            [_dt](Entity* _entity, ParticleSystem3D* _particleSystem)
            {
                _particleSystem->update(_dt);
            });
    }

    //////////////////////////////////////////
    void ParticlesDrawerSystem::notifyGatherRenderUnits(
        RenderTarget* _renderTarget,
        DefaultPassParams const& _params,
        Vector<RenderUnit>& _renderData)
    {
        Vec3DF cameraPosition = _params.cameraTransform.getAffineTranslation();
        Mat4DF cameraTransform = _params.cameraTransform;
        cameraTransform[0][3] = 0.0f;
        cameraTransform[1][3] = 0.0f;
        cameraTransform[2][3] = 0.0f;
        Vec3DF cameraForward = (cameraTransform.transformAffine(Vec3DF::c_unitZ)).normalizedCopy();
        Vec3DF cameraUp = (cameraTransform.transformAffine(Vec3DF::c_unitY)).normalizedCopy();

        m_particleSystem3DSample->process(
            [_renderTarget, &_renderData, &cameraPosition, &cameraForward, &cameraUp, &_params](Entity* _entity, ParticleSystem3D* _particleSystem)
            {
                if (!(_particleSystem->getRenderMask()->getMask() & _params.renderMask))
                    return;

                S32 aliveParticles = _particleSystem->getAliveParticles();

                if (aliveParticles == 0)
                    return;

                MaterialPtr const* material = nullptr;
                if (!_particleSystem->getRendererModule().getMaterial())
                    material = &_renderTarget->getRenderSystem()->getMaterialManager()->getErrorMaterial();
                else
                    material = &_particleSystem->getRendererModule().getMaterial();

                RenderMesh const* renderMesh = _particleSystem->getRendererModule().getRenderMesh().get();
                if (!renderMesh)
                {
                    RenderMeshPtr const& quad = _renderTarget->getRenderSystem()->getRenderMeshManager()->getDefaultQuadMesh();
                    renderMesh = quad.get();
                }

                _particleSystem->prepareToRender(
                    cameraPosition,
                    cameraForward,
                    cameraUp);

                _renderData.emplace_back(
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
