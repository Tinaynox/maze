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
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
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
    RenderControlSystemModule3DPtr RenderControlSystemModule3D::Create(ECSWorldPtr const& _world)
    {
        RenderControlSystemModule3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderControlSystemModule3D, object, init(_world));
        return object;
    }

    //////////////////////////////////////////
    bool RenderControlSystemModule3D::init(ECSWorldPtr const& _world)
    {
        m_world = _world.get();

        m_meshRenderers = _world->requestInclusiveSample<MeshRenderer, Transform3D>();
        m_trailRenderers3DSample = _world->requestInclusiveSample<TrailRenderer3D, Transform3D>();
        m_cameras3DSample = _world->requestInclusiveSample<Camera3D>();
        m_lights3DSample = _world->requestInclusiveSample<Light3D>();
        

        return true;
    }

    //////////////////////////////////////////
    void RenderControlSystemModule3D::draw(RenderTarget* _renderTarget)
    {
        m_cameras3DSample->process(
            [&](Entity* _entity, Camera3D* _camera3D)
            {
                RenderTargetPtr const& renderTarget = _camera3D->getRenderTarget();

                if (_renderTarget != renderTarget.get())
                    return;

                Vector<Light3D*> lights3D;
                Light3D* mainLight = nullptr;
                Vec4DF mainLightColor = Vec4DF::c_zero;
                Vec3DF mainLightDirection = Vec3DF::c_unitZ;
                m_lights3DSample->process(
                    [&](Entity* _entity, Light3D* _light3D)
                {
                    if (_camera3D->getRenderMask() & _light3D->getRenderMask()->getMask())
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

                

                Transform3DPtr const& _cameraTransform3D = _camera3D->getTransform();
                Vec3DF cameraPosition = _cameraTransform3D->getWorldPosition();
                Mat4DF const& cameraWorldTransform = _cameraTransform3D->getWorldTransform();
                S32 renderMask = _camera3D->getRenderMask();

                Rect2DF const& viewport = _camera3D->getViewport();

                

                RenderQueuePtr const& renderQueue = renderTarget->getRenderQueue();

                if (renderTarget->beginDraw())
                {
                    renderQueue->clear();
                    renderQueue->pushPushScissorRectCommand(viewport);

                    bool clearColorFlag = _camera3D->getClearColorFlag();
                    bool clearDepthFlag = _camera3D->getClearDepthFlag();

                    if (clearColorFlag)
                        renderTarget->setClearColor(_camera3D->getClearColor());

                    if (clearColorFlag || clearDepthFlag)
                    {
                        renderQueue->pushClearCurrentRenderTargetCommand(
                            clearColorFlag,
                            clearDepthFlag);
                    }

                    renderTarget->setViewport(viewport);

                    // Projection matrix
                    renderTarget->setProjectionMatrixPerspective(
                        _camera3D->getFOV(),
                        _camera3D->getNearZ(),
                        _camera3D->getFarZ());

                    // View matrix
                    Mat4DF viewMatrix = cameraWorldTransform.inversedAffineCopy();
                    renderTarget->setViewMatrix(viewMatrix);

                    // View position
                    renderTarget->setViewPosition(cameraPosition);

                    Vector<RenderUnit> renderData;

                    m_meshRenderers->process(
                        [&](Entity* _entity, MeshRenderer* _meshRenderer, Transform3D* _transform3D)
                        {
                            if (!_meshRenderer->getEnabled())
                                return;

                            if (_meshRenderer->getRenderMask()->getMask() & renderMask)
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

                                        MaterialPtr const* material = nullptr;
                                        if (!materials.empty())
                                            material = &materials[i % materials.size()];

                                        if (!material || !*material)
                                            material = &_camera3D->getRenderTarget()->getRenderSystem()->getMaterialManager()->getErrorMaterial();

                                        renderData.emplace_back(
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

                    m_trailRenderers3DSample->process(
                        [&](Entity* _entity, TrailRenderer3D* _trailRenderer, Transform3D* _transform3D)
                        {
                            if (_trailRenderer->getRenderMask()->getMask() & renderMask)
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

                                        MaterialPtr const* material = nullptr;
                                        if (materials.empty())
                                            material = &_camera3D->getRenderTarget()->getRenderSystem()->getMaterialManager()->getErrorMaterial();
                                        else
                                            material = &materials[i % materials.size()];

                                        renderData.emplace_back(
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

                    eventGatherRenderUnits(_camera3D, renderData);


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

                    S32 prevRenderQueueIndex = -1;

                    for (RenderUnit const& data : renderData)
                    {
                        RenderPassPtr const& renderPass = data.renderPass;
                        ShaderPtr const& shader = renderPass->getShader();

                        S32 currentRenderQueueIndex = renderPass->getRenderQueueIndex();

                        VertexArrayObjectPtr const& vao = data.vao;

                        if (shader->getMainLightColorUniform())
                            shader->getMainLightColorUniform()->set(mainLightColor);

                        if (shader->getMainLightDirectionUniform())
                            shader->getMainLightDirectionUniform()->set(mainLightDirection);


                        renderQueue->pushSelectRenderPassCommand(renderPass);

                        renderQueue->pushDrawVAOInstancedCommand(
                            vao,
                            data.count,
                            data.modelMatricies,
                            data.colorStream,
                            data.uvStream);

                        prevRenderQueueIndex = currentRenderQueueIndex;
                    }

                    renderQueue->pushPopScissorRectCommand();

                    renderQueue->draw();

                    renderTarget->endDraw();
                }
            });
    }

    //////////////////////////////////////////
    void RenderControlSystemModule3D::processUpdate(F32 _dt)
    {
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
    
    
} // namespace Maze
//////////////////////////////////////////
