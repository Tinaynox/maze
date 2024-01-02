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
#include "maze-graphics/ecs/systems/MazeRenderControlSystemModule3D.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3DHider.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer3D.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
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
    // Class RenderControlSystemModule3D
    //
    //////////////////////////////////////////
    RenderControlSystemModule3D::RenderControlSystemModule3D()
        : m_world(nullptr)
    {
    }

    //////////////////////////////////////////
    RenderControlSystemModule3D::~RenderControlSystemModule3D()
    {
    }

    //////////////////////////////////////////
    RenderControlSystemModule3DPtr RenderControlSystemModule3D::Create(
        ECSWorldPtr const& _world,
        RenderSystemPtr const& _renderSystem)
    {
        RenderControlSystemModule3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderControlSystemModule3D, object, init(_world, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool RenderControlSystemModule3D::init(
        ECSWorldPtr const& _world,
        RenderSystemPtr const& _renderSystem)
    {
        m_world = _world.get();
        m_renderSystem = _renderSystem;

        m_meshRenderers = _world->requestInclusiveSample<MeshRenderer, Transform3D>();
        m_meshRenderersInstancedSample = _world->requestInclusiveSample<MeshRendererInstanced, Transform3D>();
        m_trailRenderers3DSample = _world->requestInclusiveSample<TrailRenderer3D, Transform3D>();
        m_trailRendererHiders3DSample = _world->requestInclusiveSample<TrailRenderer3DHider, Transform3D>();
        m_lineRenderers3DSample = _world->requestInclusiveSample<LineRenderer3D, Transform3D>();
        m_cameras3DSample = _world->requestInclusiveSample<Camera3D>();
        m_lights3DSample = _world->requestInclusiveSample<Light3D>();
        m_systemTextRenderer3DsSample = _world->requestInclusiveSample<SystemTextRenderer3D>();
        
        eventGatherRenderUnits.subscribe(this, &RenderControlSystemModule3D::notifyGatherRenderUnits);

        return true;
    }

    //////////////////////////////////////////
    void RenderControlSystemModule3D::drawDefaultPass(
        RenderTarget* _renderTarget,
        DefaultPassParams const& _params,
        std::function<void(RenderQueuePtr const&)> _beginRenderQueueCallback,
        std::function<void(RenderQueuePtr const&)> _beginDrawCallback,
        std::function<void(RenderQueuePtr const&)> _endDrawCallback,
        std::function<void(RenderQueuePtr const&)> _endRenderQueueCallback)
    {
        Vec3F cameraPosition = _params.cameraTransform.getAffineTranslation();

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
            Mat4F viewMatrix = _params.cameraTransform.inversedAffineCopy();
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

                        Mat4F skyboxTransform = Mat4F(
                            skyboxScale, 0, 0, cameraPosition.x,
                            0, skyboxScale, 0, cameraPosition.y,
                            0, 0, skyboxScale, cameraPosition.z,
                            0, 0, 0, 1);
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
                    eventGatherRenderUnits(_renderTarget, _params, renderData);
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
    void RenderControlSystemModule3D::draw(RenderTarget* _renderTarget)
    {
        MAZE_PROFILER_SCOPED_LOCK(3D);
        MAZE_PROFILE_EVENT("RenderControlSystemModule3D::draw");

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
            params.lightingSettings = camera->getLightingSettings();

            eventPrePass(
                _renderTarget,
                params);

            drawDefaultPass(
                _renderTarget,
                params);

            eventPostPass(
                _renderTarget,
                params);
                
        }
    }

    //////////////////////////////////////////
    void RenderControlSystemModule3D::processUpdate(F32 _dt)
    {
        m_trailRendererHiders3DSample->process(
            [_dt](Entity* _entity, TrailRenderer3DHider* _trailRendererHider, Transform3D* _transform)
        {
            _trailRendererHider->update(_dt);
        });

        m_trailRenderers3DSample->process(
            [_dt](Entity* _entity, TrailRenderer3D* _trailRenderer, Transform3D* _transform)
            {
                _trailRenderer->update(_dt);
            });

        m_systemTextRenderer3DsSample->process(
            [](Entity* _entity, SystemTextRenderer3D* _systemTextRenderer3D)
        {
            if (_systemTextRenderer3D->getTransform()->isWorldTransformChanged())
                _systemTextRenderer3D->updateMeshRendererModelMatrices();
        });
    }

    //////////////////////////////////////////
    void RenderControlSystemModule3D::processPostUpdate(F32 _dt)
    {

    }
    
    //////////////////////////////////////////
    void RenderControlSystemModule3D::notifyGatherRenderUnits(
        RenderTarget* _renderTarget,
        DefaultPassParams const& _params,
        Vector<RenderUnit>& _renderData)
    {
        // Meshes
        {
            MAZE_PROFILE_EVENT("RenderControlSystemModule3D::notifyGatherRenderUnits (Meshes)");
            m_meshRenderers->process(
                [&](Entity* _entity, MeshRenderer* _meshRenderer, Transform3D* _transform3D)
                {
                    if (!_meshRenderer->getEnabled())
                        return;

                    if (_meshRenderer->getRenderMask() && _meshRenderer->getRenderMask()->getMask() & _params.renderMask)
                    {
                        if (_meshRenderer->getRenderMesh())
                        {
                            Vector<MaterialAssetRef> const& materials = _meshRenderer->getMaterialRefs();
                            Vector<VertexArrayObjectPtr> const& vaos = _meshRenderer->getRenderMesh()->getVertexArrayObjects();

                            if (vaos.empty())
                                return;

                            Size c = Math::Max(vaos.size(), materials.size());

                            for (Size i = 0, in = c; i < in; ++i)
                            {
                                VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                                MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                                MaterialPtr const* material = nullptr;
                                if (!materials.empty())
                                    material = &materials[i % materials.size()].getMaterial();

                                if (!material || !*material)
                                    material = &m_renderSystem->getMaterialManager()->getErrorMaterial();

                                _renderData.emplace_back(
                                    RenderUnit
                                    {
                                        (*material)->getFirstRenderPass(),
                                        vao,
                                        _transform3D->getWorldPosition(),
                                        1,
                                        &_transform3D->getWorldTransform()
                                    });

                            }
                        }
                    }
                });
        }

        // Mesh Instances
        {
            MAZE_PROFILE_EVENT("RenderControlSystemModule3D::notifyGatherRenderUnits (MeshInstanced)");
            m_meshRenderersInstancedSample->process(
                [&](Entity* _entity, MeshRendererInstanced* _meshRenderer, Transform3D* _transform3D)
            {
                if (!_meshRenderer->getEnabled())
                    return;

                if (_meshRenderer->getRenderMask() && _meshRenderer->getRenderMask()->getMask() & _params.renderMask)
                {
                    if (_meshRenderer->getRenderMesh())
                    {
                        Material const* material = _meshRenderer->getMaterial().get();
                        if (!material)
                            material = _renderTarget->getRenderSystem()->getMaterialManager()->getErrorMaterial().get();

                        Vector<VertexArrayObjectPtr> const& vaos = _meshRenderer->getRenderMesh()->getVertexArrayObjects();

                        if (vaos.empty())
                            return;

                        S32 count = (S32)_meshRenderer->getModelMatrices().size();
                        if (count > 0)
                        {
                            for (Size i = 0, in = vaos.size(); i < in; ++i)
                            {
                                VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                                MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                                Vec4F const* uvStreams[MAZE_UV_CHANNELS_MAX];
                                memset(uvStreams, 0, sizeof(uvStreams));
                                uvStreams[0] = _meshRenderer->getUV0Data();
                                uvStreams[1] = _meshRenderer->getUV1Data();

                                _renderData.emplace_back(
                                    RenderUnit
                                    (
                                        material->getFirstRenderPass(),
                                        vao,
                                        _transform3D->getWorldPosition(),
                                        count,
                                        _meshRenderer->getModelMatricesData(),
                                        _meshRenderer->getColorsData(),
                                        uvStreams
                                    ));
                            }

                        }
                    }
                }
            });
        }

        // Trails
        {
            MAZE_PROFILE_EVENT("RenderControlSystemModule3D::notifyGatherRenderUnits (Trails)");
            m_trailRenderers3DSample->process(
                [&](Entity* _entity, TrailRenderer3D* _trailRenderer, Transform3D* _transform3D)
                {
                    if (_trailRenderer->getEdgesCount() == 0)
                        return;

                    if (_trailRenderer->getRenderMask()->getMask() & _params.renderMask)
                    {
                        if (_trailRenderer->getRenderMesh())
                        {
                            Vector<MaterialAssetRef> const& materials = _trailRenderer->getMaterialRefs();
                            Vector<VertexArrayObjectPtr> const& vaos = _trailRenderer->getRenderMesh()->getVertexArrayObjects();

                            if (vaos.empty())
                                return;

                            Size c = Math::Max(vaos.size(), materials.size());

                            for (Size i = 0, in = c; i < in; ++i)
                            {
                                VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                                MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                                MaterialPtr const* material = nullptr;
                                if (materials.empty() || !materials[i % materials.size()].getMaterial())
                                    material = &m_renderSystem->getMaterialManager()->getErrorMaterial();
                                else
                                    material = &materials[i % materials.size()].getMaterial();

                                _renderData.emplace_back(
                                    RenderUnit
                                    {
                                        (*material)->getFirstRenderPass(),
                                        vao,
                                        _trailRenderer->getWorldPosition(),
                                        1,
                                        &Mat4F::c_identity
                                    });

                            }
                        }
                    }
                });
            }

        // Lines
        {
            MAZE_PROFILE_EVENT("RenderControlSystemModule3D::notifyGatherRenderUnits (Lines)");
            m_lineRenderers3DSample->process(
                [&](Entity* _entity, LineRenderer3D* _lineRenderer, Transform3D* _transform3D)
            {
                if (_lineRenderer->getRenderMask()->getMask() & _params.renderMask)
                {
                    if (_lineRenderer->getRenderMesh())
                    {
                        Vector<MaterialAssetRef> const& materials = _lineRenderer->getMaterialRefs();
                        Vector<VertexArrayObjectPtr> const& vaos = _lineRenderer->getRenderMesh()->getVertexArrayObjects();

                        if (vaos.empty())
                            return;

                        Size c = Math::Max(vaos.size(), materials.size());

                        for (Size i = 0, in = c; i < in; ++i)
                        {
                            VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                            MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                            MaterialPtr const* material = nullptr;
                            if (materials.empty() || !materials[i % materials.size()].getMaterial())
                                material = &m_renderSystem->getMaterialManager()->getErrorMaterial();
                            else
                                material = &materials[i % materials.size()].getMaterial();

                            _renderData.emplace_back(
                                RenderUnit
                                {
                                    (*material)->getFirstRenderPass(),
                                    vao,
                                    _transform3D->getWorldPosition(),
                                    1,
                                    &_transform3D->getWorldTransform()
                                });

                        }
                    }
                }
            });
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
