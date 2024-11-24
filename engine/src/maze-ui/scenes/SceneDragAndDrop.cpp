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
#include "MazeUIHeader.hpp"
#include "maze-ui/scenes/SceneDragAndDrop.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropController.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneDragAndDrop
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDragAndDrop, EcsRenderScene);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(SceneDragAndDrop);

    //////////////////////////////////////////
    SceneDragAndDrop::SceneDragAndDrop()
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneDragAndDrop::~SceneDragAndDrop()
    {

    }

    //////////////////////////////////////////
    SceneDragAndDropPtr SceneDragAndDrop::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneDragAndDropPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDragAndDrop, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneDragAndDrop::init(RenderTargetPtr const& _renderTarget)
    {
        if (!EcsRenderScene::init(_renderTarget))
            return false;


        m_dragAndDropZonesSample = m_world->requestInclusiveSample<DragAndDropZone>();
        m_inputSystemSample = m_world->requestInclusiveSample<InputSystem2D>();
        m_dragAndDropControllerSample = m_world->requestInclusiveSample<DragAndDropController>();

        
        EntityPtr canvasEntity = createEntity("DragAndDropController");
        m_dragAndDropController = canvasEntity->createComponent<DragAndDropController>(getId());
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearDepthFlag(false);
        m_canvas->setClipViewport(false);
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvas->setSortOrder(9000000);
        m_canvas->setViewportTransformPolicy(ViewportTransformPolicy::ViewportToTransform);
        m_canvas->getCanvasScaler()->setScaleMode(CanvasScalerScaleMode::ConstantPixelSize);

        Transform2DPtr elementTransform = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            Vec2F(0.0f, 0.0f),
            m_canvas->getTransform(),
            this);
        elementTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
        m_element = elementTransform->getEntityRaw()->ensureComponent<UIElement2D>();
        m_element->setCaptureCursorHits(false);

        return true;
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::update(F32 _dt)
    {
        if (m_dragAndDropView)
        {
            Vec2F renderTargetCursorPos = InputManager::GetInstancePtr()->getCursorPosition(0);

            if (InputManager::GetInstancePtr()->getCursorButtonState(0, 1))
            {
                cancelDrag();
            }
            else
            if (InputManager::GetInstancePtr()->getCursorButtonState(0, 0))
            {
                setCurrentDropZone(c_invalidEntityId);

                m_dragAndDropView->setLocalPosition(renderTargetCursorPos);

                m_inputSystemSample->query(
                    [&](Entity*, InputSystem2D* _inputSystem)
                    {
                        CursorElementTraceParams traceParams;
                        traceParams.ignoreElements.insert(m_element->getEntityId());

                        m_dragAndDropZonesSample->query(
                            [&](Entity* _zoneEntity, DragAndDropZone* _zone)
                            {
                                if (_inputSystem->traceElement(
                                    _zone->getUIElement().get(),
                                    renderTargetCursorPos,
                                    traceParams))
                                {
                                    DragAndDropValidateEvent validateEvent(m_data, m_viewEntityId);
                                    _zoneEntity->getEcsWorld()->sendEventImmediate(
                                        _zoneEntity->getId(),
                                        &validateEvent);

                                    if (validateEvent.dropAllowed)
                                        setCurrentDropZone(_zone->getEntityId());
                                }
                            });
                    });

                

            }
            else
                tryDrop();
        }
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::startDrag(Transform2DPtr const& _viewEntity, DataBlock const& _data)
    {
        cancelDrag();

        if (_viewEntity)
        {
            m_data = _data;

            m_viewEntityId = _viewEntity->getEntityId();

            EntityCopyData copyData;
            copyData.setScene(this);

            EntityPtr newView = _viewEntity->getEntityRaw()->createCopy(copyData);
            m_dragAndDropView = newView->getComponent<Transform2D>();
            m_dragAndDropView->setAnchor(Vec2F(0.0f));
            m_dragAndDropView->setPivot(Vec2F(0.5f));
            m_dragAndDropView->setParent(m_canvas->getTransform());

            m_element->setCaptureCursorHits(true);
        }
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::cancelDrag()
    {
        if (m_element)
            m_element->setCaptureCursorHits(false);

        setCurrentDropZone(c_invalidEntityId);

        m_viewEntityId = c_invalidEntityId;
        m_data.clear();

        if (!m_dragAndDropView)
            return;

        if (m_dragAndDropView->getEntityRaw())
            m_dragAndDropView->getEntityRaw()->removeFromEcsWorld();

        m_dragAndDropView.reset();
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::tryDrop()
    {
        if (m_currentDropZoneEid != c_invalidEntityId)
        {
            getWorld()->sendEvent<DragAndDropEvent>(m_currentDropZoneEid, m_data, m_viewEntityId);
        }

        cancelDrag();
    }

    //////////////////////////////////////////
    void SceneDragAndDrop::setCurrentDropZone(EntityId _eid)
    {
        if (m_currentDropZoneEid == _eid)
            return;

        m_currentDropZoneEid = _eid;

        getWorld()->broadcastEvent<DragAndDropCurrentZoneChangedEvent>(m_currentDropZoneEid);
    }

} // namespace Maze
//////////////////////////////////////////
