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
#include "maze-graphics/ecs/components/MazeRenderControllerModule2D.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderControllerModule2D
    //
    //////////////////////////////////////////
    RenderControllerModule2D::RenderControllerModule2D()
        : m_sortedMeshRenderersDirty(false)
    {
    }

    //////////////////////////////////////////
    RenderControllerModule2D::~RenderControllerModule2D()
    {
        m_canvasesSample->eventEntityAdded.unsubscribe(this);
        m_canvasesSample->eventEntityRemoved.unsubscribe(this);

        m_meshRenderersSample->eventEntityAdded.unsubscribe(this);
        m_meshRenderersSample->eventEntityRemoved.unsubscribe(this);

        m_meshRenderersInstancedSample->eventEntityAdded.unsubscribe(this);
        m_meshRenderersInstancedSample->eventEntityRemoved.unsubscribe(this);
    }

    //////////////////////////////////////////
    RenderControllerModule2DPtr RenderControllerModule2D::Create(
        EcsWorldPtr const& _world,
        RenderSystemPtr const& _renderSystem)
    {
        RenderControllerModule2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderControllerModule2D, object, init(_world, _renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool RenderControllerModule2D::init(
        EcsWorldPtr const& _world,
        RenderSystemPtr const& _renderSystem)
    {
        m_world = _world.get();
        m_renderSystem = _renderSystem;

        m_transform2Ds = _world->requestInclusiveSample<Transform2D>();
        m_meshRenderersSample = _world->requestInclusiveSample<MeshRenderer, Transform2D>();
        m_meshRenderersInstancedSample = _world->requestInclusiveSample<MeshRendererInstanced, Transform2D>();
        m_canvasesSample = _world->requestInclusiveSample<Canvas>();
        m_canvasScalersSample = _world->requestInclusiveSample<CanvasScaler>();
        m_canvasGroupsSample = _world->requestInclusiveSample<CanvasGroup>();
        m_canvasRenderersSample = _world->requestInclusiveSample<CanvasRenderer>();
        m_spriteRenderer2DsSample =  _world->requestInclusiveSample<SpriteRenderer2D>();
        m_systemTextRenderer2DsSample =  _world->requestInclusiveSample<SystemTextRenderer2D>();
        m_lineRenderers2DSample = _world->requestInclusiveSample<LineRenderer2D, Transform2D>();

        m_sizePolicy2D = _world->requestInclusiveSample<SizePolicy2D, Transform2D>();

        m_canvasesSample->eventEntityAdded.subscribe(this, &RenderControllerModule2D::processCanvasEntityAdded);
        m_canvasesSample->eventEntityRemoved.subscribe(this, &RenderControllerModule2D::processCanvasEntityRemoved);

        m_meshRenderersSample->eventEntityAdded.subscribe(this, &RenderControllerModule2D::processMeshRendererEntityAdded);
        m_meshRenderersSample->eventEntityRemoved.subscribe(this, &RenderControllerModule2D::processMeshRendererEntityRemoved);

        m_meshRenderersInstancedSample->eventEntityAdded.subscribe(this, &RenderControllerModule2D::processMeshRendererInstancedEntityAdded);
        m_meshRenderersInstancedSample->eventEntityRemoved.subscribe(this, &RenderControllerModule2D::processMeshRendererInstancedEntityRemoved);

        updateSortedCanvasesList();

        return true;
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::draw(RenderTarget* _renderTarget)
    {
        MAZE_PROFILER_SCOPED_LOCK(2D);
        MAZE_PROFILE_EVENT("RenderControllerModule2D::draw");

        for (auto& canvasRenderData : m_sortedCanvasRenderData)
        {
            Canvas* canvas = canvasRenderData.canvas;
            Canvas* rootCanvas = canvasRenderData.rootCanvas;
            if (!canvas || !rootCanvas)
                continue;
                            
            Rect2F const& viewport = canvas->getViewport();
            Rect2F const& rootViewport = rootCanvas->getViewport();

            RenderTargetPtr const& renderTarget = canvas->getRenderTarget();
            if (!renderTarget)
                return;

            RenderQueuePtr const& renderQueue = renderTarget->getRenderQueue();

            if (_renderTarget != renderTarget.get())
                continue;

            if (renderTarget && renderTarget->beginDraw())
            {
                renderQueue->clear();

                if (canvas->getClipViewport())
                {
                    if (canvas == rootCanvas)
                    {
                        renderQueue->addPushScissorRectCommand(viewport);
                    }
                    else
                    {
                        Rect2F p = viewport.intersectedCopy(rootViewport);
                        renderQueue->addPushScissorRectCommand(p);
                    }
                }

                bool clearColorFlag = canvas->getClearColorFlag();
                bool clearDepthFlag = canvas->getClearDepthFlag();

                if (clearColorFlag)
                    renderTarget->setClearColor(canvas->getClearColor());

                if (clearColorFlag || clearDepthFlag)
                {
                    renderQueue->addClearCurrentRenderTargetCommand(
                        clearColorFlag,
                        clearDepthFlag);
                }

                renderTarget->setViewport(rootViewport);

                Vec2F viewportSize = (Vec2F)renderTarget->getRenderTargetSize() * rootViewport.size;
                viewportSize = Math::Round(viewportSize);

                // Projection matrix
                renderTarget->setProjectionMatrix2D(
                    0.0f,
                    0.0f,
                    viewportSize.x,
                    viewportSize.y);
                    
                Vec3F canvasCameraPosition = Vec3F(
                    (F32)renderTarget->getRenderTargetWidth() / 2.0f,
                    (F32)renderTarget->getRenderTargetHeight() / 2.0f,
                    0.0f);
                    
                // View matrix
                renderTarget->setViewMatrix(TMat::c_identity);

                // View position
                renderTarget->setViewPosition(canvasCameraPosition);


                for (auto const& commandData : canvasRenderData.commands)
                {
                    switch (commandData.type)
                    {
                        case CanvasRenderCommandType::DrawMeshRenderer:
                        {
                            MeshRenderer const* meshRenderer = commandData.meshRenderer;

                            if (!meshRenderer->getEnabled())
                                continue;

                            Transform2D* transform2D = commandData.transform;

                            if (!meshRenderer->getRenderMesh())
                                continue;

                            Vector<MaterialAssetRef> const& materials = meshRenderer->getMaterialRefs();
                            Vector<VertexArrayObjectPtr> const& vaos = meshRenderer->getRenderMesh()->getVertexArrayObjects();

                            if (vaos.empty())
                                continue;

                            Size c = Math::Max(vaos.size(), materials.size());

                            for (Size i = 0, in = c; i < in; ++i)
                            {
                                VertexArrayObjectPtr const& vao = vaos[i % vaos.size()];

                                MAZE_DEBUG_ERROR_IF(vao == nullptr, "VAO is null");

                                MaterialPtr const* material = nullptr;
                                if (!materials.empty())
                                    material = &materials[i % materials.size()].getMaterial();

                                if (!material || !*material)
                                    material = &renderTarget->getRenderSystem()->getMaterialManager()->getErrorMaterial();

                                renderQueue->addSelectRenderPassCommand((*material)->getFirstRenderPass());

                                TMat const& worldTransform = transform2D->getWorldTransform();

                                renderQueue->addDrawVAOInstancedCommand(vao, worldTransform);                                
                            }

                            break;
                        }

                        case CanvasRenderCommandType::DrawMeshRendererInstanced:
                        {
                            MeshRendererInstanced const* meshRenderer = commandData.meshRendererInstanced;

                            if (!meshRenderer->getEnabled())
                                continue;

                            if (!meshRenderer->getRenderMesh())
                                continue;

                            Vector<VertexArrayObjectPtr> const& vaos = meshRenderer->getRenderMesh()->getVertexArrayObjects();

                            if (vaos.empty())
                                continue;

                            Material const* material = meshRenderer->getMaterial().get();
                            if (!material)
                            {
                                material = renderTarget->getRenderSystem()->getMaterialManager()->getErrorMaterial().get();
                                if (!material)
                                    continue;
                            }

                            renderQueue->addSelectRenderPassCommand(material->getFirstRenderPass());

                            for (Size i = 0, in = vaos.size(); i < in; ++i)
                            {
                                VertexArrayObjectPtr const& vao = vaos[i];
                                MAZE_DEBUG_ERROR_IF(vao == nullptr, "VAO is null");

                                S32 count = (S32)meshRenderer->getModelMatrices().size();
                                if (count > 0)
                                {
                                    Vec4F const* uvStreams[MAZE_UV_CHANNELS_MAX];
                                    memset(uvStreams, 0, sizeof(uvStreams));
                                    uvStreams[0] = meshRenderer->getUV0Data();
                                    uvStreams[1] = meshRenderer->getUV1Data();

                                    renderQueue->addDrawVAOInstancedCommand(
                                        vao.get(),
                                        count,
                                        meshRenderer->getModelMatricesData(),
                                        meshRenderer->getColorsData(),
                                        uvStreams);
                                }
                            }

                            break;
                        }

                        case CanvasRenderCommandType::PushScissorMask:
                        {
                            ScissorMask2D* scissorMask = commandData.scissorMask;
                            
                            AABB2D const& aabb = scissorMask->getScissorBounds();
                            Rect2F rect = aabb.toRect();
                            Vec2F renderTargetSize = (Vec2F)renderTarget->getRenderTargetSize();
                            rect.position /= renderTargetSize;
                            rect.size /= renderTargetSize;
                            rect.position += rootViewport.position;
                            renderQueue->addPushScissorRectCommand(rect);
                            break;
                        }

                        case CanvasRenderCommandType::PopScissorMask:
                        {
                            renderQueue->addPopScissorRectCommand();
                            break;
                        }

                        default:
                            MAZE_NOT_IMPLEMENTED;
                            break;
                    }
                }
                                     
                if (canvas->getClipViewport())
                    renderQueue->addPopScissorRectCommand();

                renderQueue->draw();
                    
                renderTarget->endDraw();
            }
        }
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processUpdate(F32 _dt)
    {
        F32 dt = _dt;

        m_transform2Ds->query(
            [&](Entity* _entity, Transform2D* _transform)
            {
                if (_transform->updateChildrenOrder() || !_transform->getEntityRaw()->getActiveInHierarchy() ||
                    _transform->getEntityRaw()->getAdding() || _transform->getEntityRaw()->getRemoving())
                {
                    m_sortedMeshRenderersDirty = true;
                }
            }); 

        m_canvasScalersSample->query(
            [&](Entity* _entity, CanvasScaler* _canvasScaler)
            {
                _canvasScaler->updateCanvasScale();
            });            

        m_sizePolicy2D->query(
            [](Entity* entity, SizePolicy2D* _sizePolicy, Transform2D* _transform)
            {
                if (_transform->isWorldTransformChanged())
                    _sizePolicy->updateSize();
            });
        

        m_canvasGroupsSample->query(
            [](Entity* _entity, CanvasGroup* _canvasGroup)
            {
                std::function<void(Transform2D*)> updateFunc =
                    [&](Transform2D* _transform2D)
                    {
                        CanvasRenderer* canvasRenderer = _transform2D->getEntityRaw()->getComponentRaw<CanvasRenderer>();
                        if (canvasRenderer)
                            canvasRenderer->dirtyAlpha();

                        for (Transform2D* child : _transform2D->getChildren())
                        {
                            CanvasGroup* canvasGroup = child->getEntityRaw()->getComponentRaw<CanvasGroup>();
                            if (canvasGroup)
                                continue;

                            updateFunc(child);
                        }
                    };

                if (_canvasGroup->isWorldAlphaChanged())
                {
                    updateFunc(_canvasGroup->getTransform().get());
                }
            });

        m_canvasRenderersSample->query(
            [](Entity* _entity, CanvasRenderer* _canvasRenderer)
            {
                if (_canvasRenderer->getTransform()->isHierarchyChanged())
                    _canvasRenderer->dirtyAlpha();
            });

        m_systemTextRenderer2DsSample->query(
            [](Entity* _entity, SystemTextRenderer2D* _systemTextRenderer2D)
            {
                if (_systemTextRenderer2D->getTransform()->isSizeChanged())
                    _systemTextRenderer2D->updateMeshData();
                else
                {
                    if (_systemTextRenderer2D->getTransform()->isWorldTransformChanged())
                        _systemTextRenderer2D->updateMeshRendererModelMatrices();

                    if (_systemTextRenderer2D->getCanvasRenderer()->isAlphaDirty())
                        _systemTextRenderer2D->updateMeshRendererColors();
                }
            });

        m_world->broadcastEventImmediate<Render2DPostUpdateEvent>(dt);

        updateSortedMeshRenderersList();
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processPostUpdate(F32 _dt)
    {
        m_canvasGroupsSample->query(
            [](Entity* _entity, CanvasGroup* _canvasGroup)
            {
                _canvasGroup->clearEventFlags();
            });
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processCanvasEntityAdded(Entity* _entity)
    {
        updateSortedCanvasesList();
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processCanvasEntityRemoved(Entity* _entity)
    {
        updateSortedCanvasesList();
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processMeshRendererEntityAdded(Entity* _entity)
    {
        m_sortedMeshRenderersDirty = true;
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processMeshRendererEntityRemoved(Entity* _entity)
    {
        m_sortedMeshRenderersDirty = true;
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processMeshRendererInstancedEntityAdded(Entity* _entity)
    {
        m_sortedMeshRenderersDirty = true;
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::processMeshRendererInstancedEntityRemoved(Entity* _entity)
    {
        m_sortedMeshRenderersDirty = true;
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::updateSortedCanvasesList()
    {
        m_sortedCanvases.clear();

        m_canvasesSample->query(
            [&](Entity* _entity, Canvas* _canvas)        
            {
                m_sortedCanvases.emplace_back(_canvas);
            });
        std::sort(
            m_sortedCanvases.begin(),
            m_sortedCanvases.end(),
            [](Canvas* _canvas0, Canvas* _canvas1) -> bool
            {
                return _canvas0->getSortOrder() < _canvas1->getSortOrder();
            });

        m_sortedMeshRenderersDirty = true;
    }

    //////////////////////////////////////////
    void RenderControllerModule2D::updateSortedMeshRenderersList()
    {
        if (!m_sortedMeshRenderersDirty)
            return;

        m_sortedCanvasRenderData.clear();

        for (Canvas* _canvas : m_sortedCanvases)
        {
            CanvasRenderData canvasRenderData;
            canvasRenderData.canvas = _canvas;

            if (_canvas->getTransform()->getParent())
                canvasRenderData.rootCanvas = _canvas->getTransform()->getRootTransform()->getEntityRaw()->getComponentRaw<Canvas>();
            else
                canvasRenderData.rootCanvas = _canvas;

            std::function<void(Transform2D*)> processMeshRendererFunc = 
                [&](Transform2D* _transform)
                {
                    Entity* entity = _transform->getEntityRaw();
                    if (!entity->getActiveInHierarchy() || entity->getAdding() || entity->getRemoving())
                        return;

                    // #TODO: Rework (to execute after Render2DPostUpdateEvent call)
                    if (!entity->getActiveInHierarchyPrevFrame())
                        return;

                    if (_transform->getParent())
                    {
                        Canvas* canvas = entity->getComponentRaw<Canvas>();
                        if (canvas && canvas != _canvas)
                            return;
                    }

                    // Push scissor mask
                    ScissorMask2D* scissorMask = entity->getComponentRaw<ScissorMask2D>();
                    if (scissorMask)
                    {
                        canvasRenderData.commands.emplace_back(
                            CanvasRenderCommand
                            {
                                _transform,
                                scissorMask,
                                true
                            });
                    }

                    MeshRenderer* meshRenderer = entity->getComponentRaw<MeshRenderer>();
                    if (meshRenderer)
                    {
                        canvasRenderData.commands.emplace_back(
                            CanvasRenderCommand
                            {
                                _transform,
                                meshRenderer,
                            });
                    }

                    MeshRendererInstanced* meshRendererInstanced = entity->getComponentRaw<MeshRendererInstanced>();
                    if (meshRendererInstanced)
                    {
                        canvasRenderData.commands.emplace_back(
                            CanvasRenderCommand
                            {
                                _transform,
                                meshRendererInstanced,
                            });
                    }

                    for (Transform2D* _child : _transform->getChildren())
                        processMeshRendererFunc(_child);

                    // Pop scissor mask
                    if (scissorMask)
                    {
                        canvasRenderData.commands.emplace_back(
                            CanvasRenderCommand
                            {
                                _transform,
                                scissorMask,
                                false
                            });
                    }

                };

            processMeshRendererFunc(_canvas->getTransform().get());

            m_sortedCanvasRenderData.emplace_back(canvasRenderData);
        }

        m_sortedMeshRenderersDirty = false;
    }
    
    
} // namespace Maze
//////////////////////////////////////////
