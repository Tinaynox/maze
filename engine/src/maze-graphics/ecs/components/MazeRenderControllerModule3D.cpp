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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeRenderControllerModule3D.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3DHider.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer3D.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderControllerModule3D
    //
    //////////////////////////////////////////
    RenderControllerModule3D::RenderControllerModule3D()
        : m_world(nullptr)
    {
    }

    //////////////////////////////////////////
    RenderControllerModule3D::~RenderControllerModule3D()
    {
    }

    //////////////////////////////////////////
    RenderControllerModule3DPtr RenderControllerModule3D::Create(
        EcsWorldPtr const& _world,
        RenderSystemPtr const& _renderSystem)
    {
        RenderControllerModule3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderControllerModule3D, object, init(_world, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool RenderControllerModule3D::init(
        EcsWorldPtr const& _world,
        RenderSystemPtr const& _renderSystem)
    {
        m_world = _world.get();
        m_renderSystem = _renderSystem;

        m_cameras3DSample = _world->requestInclusiveSample<Camera3D>();
        m_lights3DSample = _world->requestInclusiveSample<Light3D>();
        
        return true;
    }

    //////////////////////////////////////////
    void RenderControllerModule3D::drawDefaultPass(
        RenderTarget* _renderTarget,
        DefaultPassParams const& _params,
        std::function<void(RenderQueuePtr const&)> _beginRenderQueueCallback,
        std::function<void(RenderQueuePtr const&)> _beginDrawCallback,
        std::function<void(RenderQueuePtr const&)> _endDrawCallback,
        std::function<void(RenderQueuePtr const&)> _endRenderQueueCallback)
    {
        Vec3F cameraPosition = _params.cameraTransform.getTranslation();

        Vector<Light3D*> lights3D;
        Light3D* mainLight = nullptr;
        Vec4F mainLightColor = Vec4F::c_zero;
        Vec3F mainLightDirection = Vec3F::c_unitZ;
        m_lights3DSample->process(
            [&](Entity* _entity, Light3D* _light3D)
            {
                if (_params.renderMask & _light3D->getRenderMask()->getMask())
                {
                    lights3D.emplace_back(_light3D);

                    if (!mainLight && _light3D->getLightType() == Light3DType::Directional)
                    {
                        mainLight = _light3D;
                        mainLightColor = mainLight->getColor().toVec4F32();
                        mainLightDirection = mainLight->getTransform()->getWorldForwardDirection();
                    }
                }
            });

        RenderQueuePtr const& renderQueue = _renderTarget->getRenderQueue();

        if (_renderTarget->beginDraw())
        {
            renderQueue->clear();

            if (_beginRenderQueueCallback)
                _beginRenderQueueCallback(renderQueue);

            if (_params.clipViewport)
                renderQueue->addPushScissorRectCommand(_params.viewport);

            if (_params.clearColorFlag)
                _renderTarget->setClearColor(_params.clearColor);

            if (_params.clearColorFlag || _params.clearDepthFlag)
            {
                renderQueue->addClearCurrentRenderTargetCommand(
                    _params.clearColorFlag,
                    _params.clearDepthFlag);
            }

            _renderTarget->setViewport(_params.viewport);

            // Projection matrix
            _renderTarget->setProjectionMatrix(_params.projectionMatrix);
            _renderTarget->setNear(_params.nearZ);
            _renderTarget->setFar(_params.farZ);

            // View matrix
            TMat viewMatrix = _params.cameraTransform.inversed();
            _renderTarget->setViewMatrix(viewMatrix);

            // View position
            _renderTarget->setViewPosition(cameraPosition);

            Vector<RenderUnit> renderData;

            // Skybox
            if (_params.clearSkyBoxFlag)
            {
                if (_params.lightingSettings)
                {
                    MaterialPtr const& skyBoxMaterial = _params.lightingSettings->getSkyBoxMaterial();
                    if (skyBoxMaterial)
                    {
                        RenderMeshPtr const& cubeMesh = m_renderSystem->getRenderMeshManager()->getDefaultCubeMesh();
                        Vector<VertexArrayObjectPtr> const& vaos = cubeMesh->getVertexArrayObjects();

                        renderQueue->addSelectRenderPassCommand(skyBoxMaterial->getFirstRenderPass());

                        F32 skyboxScale = (2.0f * _params.farZ / Math::Sqrt(3.0f)) - 1.0f;

                        TMat skyboxTransform = TMat(
                            skyboxScale, 0, 0,
                            0, skyboxScale, 0,
                            0, 0, skyboxScale,
                            cameraPosition.x, cameraPosition.y, cameraPosition.z);
                        renderQueue->addDrawVAOInstancedCommand(
                            vaos[0],
                            1,
                            &skyboxTransform);
                    }
                }
            }           

            // Draw render units
            if (_params.drawFlag)
            {
                {
                    MAZE_PROFILE_EVENT("3D GatherRenderUnits");
                    m_world->broadcastEventImmediate<Render3DDefaultPassGatherRenderUnitsEvent>(_renderTarget, &_params, &renderData);
                }

                S32 renderDataSize = (S32)renderData.size();
                Vector<S32> indices;
                indices.resize(renderDataSize);

                for (S32 i = 0; i < renderDataSize; ++i)
                {
                    indices[i] = i;
                    RenderUnit& data = renderData[i];
                    data.sqrDistanceToCamera = (cameraPosition - data.worldPosition).squaredLength();
                }

                {
                    MAZE_PROFILE_EVENT("3D Sort Render Queue");
                    // Sort render queue
                    std::sort(
                        indices.begin(),
                        indices.end(),
                        [&renderData](S32 _idxA, S32 _idxB)
                    {
                        RenderUnit const& a = renderData[_idxA];
                        RenderUnit const& b = renderData[_idxB];

                        if (a.renderPass->getRenderQueueIndex() < b.renderPass->getRenderQueueIndex())
                            return true;

                        if (a.renderPass->getRenderQueueIndex() > b.renderPass->getRenderQueueIndex())
                            return false;

                        if (a.renderPass->getRenderQueueIndex() < (S32)RenderQueueIndex::Transparent)
                            return a.sqrDistanceToCamera < b.sqrDistanceToCamera;
                        else
                            return a.sqrDistanceToCamera > b.sqrDistanceToCamera;
                    });
                }


                if (_beginDrawCallback)
                    _beginDrawCallback(renderQueue);

                S32 prevRenderQueueIndex = -1;

                {
                    MAZE_PROFILE_EVENT("3D Preparation Render Queue");
                    for (S32 i = 0; i < renderDataSize; ++i)
                    {
                        RenderUnit const& data = renderData[indices[i]];

                        RenderPassPtr const& renderPass = data.renderPass;
                        ShaderPtr const& shader = renderPass->getShader();

                        S32 currentRenderQueueIndex = renderPass->getRenderQueueIndex();

                        VertexArrayObjectPtr const& vao = data.vao;

                        MAZE_DEBUG_ERROR_IF(vao == nullptr, "VAO is null!");

                        if (shader->getMainLightColorUniform())
                            shader->getMainLightColorUniform()->set(mainLightColor);

                        if (shader->getMainLightDirectionUniform())
                            shader->getMainLightDirectionUniform()->set(mainLightDirection);


                        renderQueue->addSelectRenderPassCommand(renderPass);


                        renderQueue->addDrawVAOInstancedCommand(
                            vao.get(),
                            data.count,
                            data.modelMatricies,
                            data.colorStream,
                            (Vec4F const**)data.uvStreams);

                        prevRenderQueueIndex = currentRenderQueueIndex;
                    }
                }

                if (_endDrawCallback)
                    _endDrawCallback(renderQueue);
            }

            if (_params.clipViewport)
                renderQueue->addPopScissorRectCommand();

            if (_endRenderQueueCallback)
                _endRenderQueueCallback(renderQueue);

            {
                MAZE_PROFILE_EVENT("3D Draw Render Queue");
                renderQueue->draw();
            }

            _renderTarget->endDraw();
        }
    }

    //////////////////////////////////////////
    void RenderControllerModule3D::draw(RenderTarget* _renderTarget)
    {
        MAZE_PROFILER_SCOPED_LOCK(3D);
        MAZE_PROFILE_EVENT("RenderControllerModule3D::draw");

        Vector<Camera3D*> cameras;
        m_cameras3DSample->process(
            [&](Entity* _entity, Camera3D* _camera3D)
            {
                cameras.push_back(_camera3D);
            });
        std::sort(
            cameras.begin(),
            cameras.end(),
            [](Camera3D* _cameraA, Camera3D* _cameraB) { return _cameraA->getSortOrder() < _cameraB->getSortOrder(); });

        for (Camera3D* camera : cameras)
        {
            RenderTargetPtr const& renderTarget = camera->getRenderTarget();

            if (_renderTarget != renderTarget.get())
                continue;

            DefaultPassParams params;

            params.renderMask = camera->getRenderMask();
            params.cameraTransform = camera->getTransform()->getWorldTransform();
            params.projectionMatrix = camera->calculateProjectionMatrix(renderTarget);
            params.viewport = camera->getViewport();
            params.nearZ = camera->getNearZ();
            params.farZ = camera->getFarZ();
            params.fieldOfViewY = camera->getFOV();
            params.clearColorFlag = camera->getClearColorFlag();
            params.clearColor = camera->getClearColor();
            params.clearDepthFlag = camera->getClearDepthFlag();
            params.clearSkyBoxFlag = camera->getClearSkyBoxFlag();
            params.drawFlag = camera->getDrawFlag();
            params.clipViewport = camera->getClipViewport();
            params.lightingSettings = camera->getLightingSettings();

            m_world->broadcastEventImmediate<Render3DDefaultPrePassEvent>(_renderTarget, &params);

            drawDefaultPass(
                _renderTarget,
                params);

            m_world->broadcastEventImmediate<Render3DDefaultPostPassEvent>(_renderTarget, &params);
                
        }
    }

    //////////////////////////////////////////
    void RenderControllerModule3D::processUpdate(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void RenderControllerModule3D::processPostUpdate(F32 _dt)
    {

    }
    
} // namespace Maze
//////////////////////////////////////////
