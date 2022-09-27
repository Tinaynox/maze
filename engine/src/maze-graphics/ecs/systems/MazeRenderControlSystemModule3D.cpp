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
        m_meshRenderers->eventEntityAdded.unsubscribe(this);
        m_meshRenderers->eventEntityRemoved.unsubscribe(this);
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
        m_trailRenderers3DSample = _world->requestInclusiveSample<TrailRenderer3D, Transform3D>();
        m_trailRendererHiders3DSample = _world->requestInclusiveSample<TrailRenderer3DHider, Transform3D>();
        m_lineRenderers3DSample = _world->requestInclusiveSample<LineRenderer3D, Transform3D>();
        m_cameras3DSample = _world->requestInclusiveSample<Camera3D>();
        m_lights3DSample = _world->requestInclusiveSample<Light3D>();
        
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
        Vec3DF cameraPosition = _params.cameraTransform.getAffineTranslation();

        Vector<Light3D*> lights3D;
        Light3D* mainLight = nullptr;
        Vec4DF mainLightColor = Vec4DF::c_zero;
        Vec3DF mainLightDirection = Vec3DF::c_unitZ;
        m_lights3DSample->process(
            [&](Entity* _entity, Light3D* _light3D)
            {
                if (_params.renderMask & _light3D->getRenderMask()->getMask())
                {
                    lights3D.emplace_back(_light3D);

                    if (!mainLight && _light3D->getLightType() == Light3DType::Directional)
                    {
                        mainLight = _light3D;
                        mainLightColor = mainLight->getColor().toVec4DF();
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
            Mat4DF viewMatrix = _params.cameraTransform.inversedAffineCopy();
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

                        Mat4DF skyboxTransform = Mat4DF(
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
                eventGatherRenderUnits(_renderTarget, _params, renderData);

                for (RenderUnit& data : renderData)
                    data.sqrDistanceToCamera = (cameraPosition - data.worldPosition).squaredLength();

                // Sort render queue
                std::sort(
                    renderData.begin(),
                    renderData.end(),
                    [](
                        RenderUnit const& _a,
                        RenderUnit const& _b)
                {
                    if (_a.renderPass->getRenderQueueIndex() < _b.renderPass->getRenderQueueIndex())
                        return true;

                    if (_a.renderPass->getRenderQueueIndex() > _b.renderPass->getRenderQueueIndex())
                        return false;

                    if (_a.renderPass->getRenderQueueIndex() < (S32)RenderQueueIndex::Transparent)
                        return _a.sqrDistanceToCamera < _b.sqrDistanceToCamera;
                    else
                        return _a.sqrDistanceToCamera > _b.sqrDistanceToCamera;
                });


                if (_beginDrawCallback)
                    _beginDrawCallback(renderQueue);

                S32 prevRenderQueueIndex = -1;

                for (RenderUnit const& data : renderData)
                {
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

                    Vec4DF const* uvStreams[MAZE_UV_CHANNELS_MAX];
                    memset(uvStreams, 0, sizeof(uvStreams));
                    uvStreams[0] = data.uvStream;

                    renderQueue->addDrawVAOInstancedCommand(
                        vao,
                        data.count,
                        data.modelMatricies,
                        data.colorStream,
                        uvStreams);

                    prevRenderQueueIndex = currentRenderQueueIndex;
                }

                if (_endDrawCallback)
                    _endDrawCallback(renderQueue);
            }

            renderQueue->addPopScissorRectCommand();

            if (_endRenderQueueCallback)
                _endRenderQueueCallback(renderQueue);

            renderQueue->draw();

            _renderTarget->endDraw();
        }
    }

    //////////////////////////////////////////
    void RenderControlSystemModule3D::draw(RenderTarget* _renderTarget)
    {
        MAZE_PROFILER_SCOPED_LOCK(3D);

        m_cameras3DSample->process(
            [&](Entity* _entity, Camera3D* _camera3D)
            {
                RenderTargetPtr const& renderTarget = _camera3D->getRenderTarget();

                if (_renderTarget != renderTarget.get())
                    return;

                DefaultPassParams params;

                params.renderMask = _camera3D->getRenderMask();
                params.cameraTransform = _camera3D->getTransform()->getWorldTransform();
                params.projectionMatrix = _camera3D->calculateProjectionMatrix(renderTarget);
                params.viewport = _camera3D->getViewport();
                params.nearZ = _camera3D->getNearZ();
                params.farZ = _camera3D->getFarZ();
                params.fieldOfViewY = _camera3D->getFOV();
                params.clearColorFlag = _camera3D->getClearColorFlag();
                params.clearColor = _camera3D->getClearColor();
                params.clearDepthFlag = _camera3D->getClearDepthFlag();
                params.clearSkyBoxFlag = _camera3D->getClearSkyBoxFlag();
                params.drawFlag = _camera3D->getDrawFlag();
                params.lightingSettings = _camera3D->getLightingSettings();

                eventPrePass(
                    _renderTarget,
                    params);

                drawDefaultPass(
                    _renderTarget,
                    params);

                eventPostPass(
                    _renderTarget,
                    params);
                
            });
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
        m_meshRenderers->process(
            [&](Entity* _entity, MeshRenderer* _meshRenderer, Transform3D* _transform3D)
            {
                if (!_meshRenderer->getEnabled())
                    return;

                if (_meshRenderer->getRenderMask()->getMask() & _params.renderMask)
                {
                    if (_meshRenderer->getRenderMesh())
                    {
                        Vector<MaterialPtr> const& materials = _meshRenderer->getMaterials();
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
                                material = &materials[i % materials.size()];

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

        // Trails
        m_trailRenderers3DSample->process(
            [&](Entity* _entity, TrailRenderer3D* _trailRenderer, Transform3D* _transform3D)
            {
                if (_trailRenderer->getEdgesCount() == 0)
                    return;

                if (_trailRenderer->getRenderMask()->getMask() & _params.renderMask)
                {
                    if (_trailRenderer->getRenderMesh())
                    {
                        Vector<MaterialPtr> const& materials = _trailRenderer->getMaterials();
                        Vector<VertexArrayObjectPtr> const& vaos = _trailRenderer->getRenderMesh()->getVertexArrayObjects();

                        if (vaos.empty())
                            return;

                        Size c = Math::Max(vaos.size(), materials.size());

                        for (Size i = 0, in = c; i < in; ++i)
                        {
                            VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                            MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                            MaterialPtr const* material = nullptr;
                            if (materials.empty() || !materials[i % materials.size()])
                                material = &m_renderSystem->getMaterialManager()->getErrorMaterial();
                            else
                                material = &materials[i % materials.size()];

                            _renderData.emplace_back(
                                RenderUnit
                                {
                                    (*material)->getFirstRenderPass(),
                                    vao,
                                    _trailRenderer->getWorldPosition(),
                                    1,
                                    &Mat4DF::c_identity
                                });

                        }
                    }
                }
            });

        // Lines
        m_lineRenderers3DSample->process(
            [&](Entity* _entity, LineRenderer3D* _lineRenderer, Transform3D* _transform3D)
        {
            if (_lineRenderer->getRenderMask()->getMask() & _params.renderMask)
            {
                if (_lineRenderer->getRenderMesh())
                {
                    Vector<MaterialPtr> const& materials = _lineRenderer->getMaterials();
                    Vector<VertexArrayObjectPtr> const& vaos = _lineRenderer->getRenderMesh()->getVertexArrayObjects();

                    if (vaos.empty())
                        return;

                    Size c = Math::Max(vaos.size(), materials.size());

                    for (Size i = 0, in = c; i < in; ++i)
                    {
                        VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                        MAZE_DEBUG_WARNING_IF(vao == nullptr, "VAO is null!");

                        MaterialPtr const* material = nullptr;
                        if (materials.empty())
                            material = &m_renderSystem->getMaterialManager()->getErrorMaterial();
                        else
                            material = &materials[i % materials.size()];

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
    
} // namespace Maze
//////////////////////////////////////////
