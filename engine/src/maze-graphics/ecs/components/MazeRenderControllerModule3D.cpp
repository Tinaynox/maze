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
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
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
    void RenderControllerModule3D::preRender()
    {

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
            _renderTarget->setViewMatrix(_params.viewMatrix);

            // View position
            _renderTarget->setViewPosition(cameraPosition);


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
                Vector<RenderUnit> renderData;

                {
                    MAZE_PROFILE_EVENT("3D Default GatherRenderUnits");
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
                    MAZE_PROFILE_EVENT("3D Default Render Queue");
                    for (S32 i = 0; i < renderDataSize; ++i)
                    {
                        RenderUnit const& renderUnit = renderData[indices[i]];

                        RenderPassPtr const& renderPass = renderUnit.renderPass;
                        ShaderPtr const& shader = renderPass->getShader();

                        S32 currentRenderQueueIndex = renderPass->getRenderQueueIndex();

                        if (shader->getMainLightColorUniform())
                            shader->getMainLightColorUniform()->set(_params.mainLightColor);

                        if (shader->getMainLightDirectionUniform())
                            shader->getMainLightDirectionUniform()->set(_params.mainLightDirection);

                        if (shader->getMainLightViewProjectionMatrixUniform())
                            shader->getMainLightViewProjectionMatrixUniform()->set(_params.mainLightViewProjectionMatrix);

                        if (shader->getMainLightShadowMapUniform())
                            shader->getMainLightShadowMapUniform()->set(_params.mainLightShadowMap);

                        renderQueue->addSelectRenderPassCommand(renderPass);

                        renderUnit.drawer->drawDefaultPass(renderQueue, _params, renderUnit);

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
    void RenderControllerModule3D::drawShadowPass(
        RenderBuffer* _shadowBuffer,
        ShadowPassParams const& _params)
    {
        Vec3F lightPosition = _params.mainLightTransform.getTranslation();

        RenderQueuePtr const& renderQueue = _shadowBuffer->getRenderQueue();

        if (_shadowBuffer->beginDraw())
        {
            renderQueue->clear();

            renderQueue->addClearCurrentRenderTargetCommand(
                false,
                true);

            _shadowBuffer->setViewport(0.0f, 0.0f, 1.0f, 1.0f);

            // Projection matrix
            _shadowBuffer->setProjectionMatrix(_params.mainLightProjectionMatrix);
            _shadowBuffer->setNear(_params.nearZ);
            _shadowBuffer->setFar(_params.farZ);

            // View matrix
            _shadowBuffer->setViewMatrix(_params.viewMatrix);

            // View position
            _shadowBuffer->setViewPosition(lightPosition);


            Vector<RenderUnit> renderData;

            {
                MAZE_PROFILE_EVENT("3D Shadow GatherRenderUnits");
                m_world->broadcastEventImmediate<Render3DShadowPassGatherRenderUnitsEvent>(_shadowBuffer, &_params, &renderData);
            }

            S32 renderDataSize = (S32)renderData.size();

            {
                MAZE_PROFILE_EVENT("3D Default Render Queue");
                for (S32 i = 0; i < renderDataSize; ++i)
                {
                    RenderUnit const& renderUnit = renderData[i];

                    RenderPassPtr const& renderPass = renderUnit.renderPass;
                    ShaderPtr const& shader = renderPass->getShader();

                    S32 currentRenderQueueIndex = renderPass->getRenderQueueIndex();

                    renderQueue->addSelectRenderPassCommand(renderPass);

                    renderUnit.drawer->drawShadowPass(renderQueue, _params, renderUnit);
                }
            }

            {
                MAZE_PROFILE_EVENT("3D Draw Render Queue");
                renderQueue->draw();
            }

            _shadowBuffer->endDraw();
        }
    }
    
    //////////////////////////////////////////
    void RenderControllerModule3D::draw(RenderTarget* _renderTarget)
    {
        MAZE_PROFILER_SCOPED_LOCK(3D);
        MAZE_PROFILE_EVENT("RenderControllerModule3D::draw");

        // Collect cameras
        Vector<Camera3D*> cameras;
        m_cameras3DSample->query(
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

            DefaultPassParams defaultParams;

            defaultParams.renderMask = camera->getRenderMask();
            defaultParams.cameraTransform = camera->getTransform()->getWorldTransform();
            defaultParams.viewMatrix = defaultParams.cameraTransform.inversed();
            defaultParams.projectionMatrix = camera->calculateProjectionMatrix(renderTarget);
            GraphicsUtilsHelper::CalculateCameraFrustum(
                defaultParams.viewMatrix,
                defaultParams.projectionMatrix,
                defaultParams.cameraFrustum);
            defaultParams.viewport = camera->getViewport();
            defaultParams.nearZ = camera->getNearZ();
            defaultParams.farZ = camera->getFarZ();
            defaultParams.fieldOfViewY = camera->getFOV();
            defaultParams.clearColorFlag = camera->getClearColorFlag();
            defaultParams.clearColor = camera->getClearColor();
            defaultParams.clearDepthFlag = camera->getClearDepthFlag();
            defaultParams.clearSkyBoxFlag = camera->getClearSkyBoxFlag();
            defaultParams.drawFlag = camera->getDrawFlag();
            defaultParams.clipViewport = camera->getClipViewport();
            defaultParams.lightingSettings = camera->getLightingSettings().get();

            // Find main light for this camera
            // Vector<Light3D*> lights3D;
            Light3D* mainLight = nullptr;
            m_lights3DSample->query(
                [&](Entity* _entity, Light3D* _light3D)
                {
                    if (camera->getRenderMask() & _light3D->getRenderMask()->getMask())
                    {
                        // lights3D.emplace_back(_light3D);

                        if (!mainLight && _light3D->getLightType() == Light3DType::Directional)
                        {
                            mainLight = _light3D;
                            defaultParams.mainLightColor = mainLight->getColor().toVec4F32();
                            defaultParams.mainLightDirection = mainLight->getTransform()->getWorldForwardDirection();
                        }
                    }
                });


            if (defaultParams.drawFlag && camera->getShadowBuffer() && mainLight && mainLight->getShadowCast())
            {
                ShadowPassParams shadowParams;
                shadowParams.renderMask = defaultParams.renderMask;
                shadowParams.nearZ = mainLight->getShadowCastNearZ();
                shadowParams.farZ = mainLight->getShadowCastFarZ();

                shadowParams.mainLightTransform = mainLight->getTransform()->getWorldTransform();
                shadowParams.viewMatrix = shadowParams.mainLightTransform.inversed();
                shadowParams.mainLightProjectionMatrix = Mat4F::CreateProjectionOrthographicLHMatrix(
                    -mainLight->getShadowCastSize(),
                    +mainLight->getShadowCastSize(),
                    -mainLight->getShadowCastSize(),
                    +mainLight->getShadowCastSize(),
                    shadowParams.nearZ,
                    shadowParams.farZ);
                GraphicsUtilsHelper::CalculateCameraFrustum(
                    shadowParams.viewMatrix,
                    shadowParams.mainLightProjectionMatrix,
                    shadowParams.mainLightFrustum);

                Mat4F mainLightTransformMat4;
                mainLightTransformMat4.setRow(0, Vec4F(shadowParams.mainLightTransform[0], 0.0f));
                mainLightTransformMat4.setRow(1, Vec4F(shadowParams.mainLightTransform[1], 0.0f));
                mainLightTransformMat4.setRow(2, Vec4F(shadowParams.mainLightTransform[2], 0.0f));
                mainLightTransformMat4.setRow(3, Vec4F(shadowParams.mainLightTransform[3], 1.0f));

                drawShadowPass(
                    camera->getShadowBuffer().get(),
                    shadowParams);

                defaultParams.mainLightViewProjectionMatrix = mainLightTransformMat4.inversedAffine() * shadowParams.mainLightProjectionMatrix;
                defaultParams.mainLightShadowMap = camera->getShadowBuffer()->getDepthTexture()->cast<Texture2D>();
            }
            else
            {
                defaultParams.mainLightShadowMap = m_renderSystem->getTextureManager()->getWhiteTexture();
            }

            m_world->broadcastEventImmediate<Render3DDefaultPrePassEvent>(_renderTarget, &defaultParams);

            drawDefaultPass(
                _renderTarget,
                defaultParams);

            m_world->broadcastEventImmediate<Render3DDefaultPostPassEvent>(_renderTarget, &defaultParams);
                
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
