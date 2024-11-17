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
#include "maze-ui/scenes/SceneDragAndDropDefault.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
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
    // Class SceneDragAndDropDefault
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneDragAndDropDefault, EcsRenderScene);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(SceneDragAndDropDefault);

    //////////////////////////////////////////
    SceneDragAndDropDefault::SceneDragAndDropDefault()
    {
        setIsSystemScene();
    }

    //////////////////////////////////////////
    SceneDragAndDropDefault::~SceneDragAndDropDefault()
    {

    }

    //////////////////////////////////////////
    SceneDragAndDropDefaultPtr SceneDragAndDropDefault::Create(RenderTargetPtr const& _renderTarget)
    {
        SceneDragAndDropDefaultPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneDragAndDropDefault, object, init(_renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool SceneDragAndDropDefault::init(RenderTargetPtr const& _renderTarget)
    {
        if (!EcsRenderScene::init(_renderTarget))
            return false;


        m_dragAndDropZonesSample = m_world->requestInclusiveSample<DragAndDropZone>();
        m_inputSystemSample = m_world->requestInclusiveSample<InputSystem2D>();


        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearDepthFlag(false);
        m_canvas->setRenderTarget(m_renderTarget);
        m_canvas->setSortOrder(1000000);
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

        /*
        SpriteRenderer2DPtr fakeView = SpriteHelper::CreateSprite(
            SpriteManager::GetCurrentInstance()->getBuiltinSprite(BuiltinSpriteType::White),
            Vec2F(32.0f),
            Vec2F(0.0f, 0.0f),
            MaterialManager::GetCurrentInstance()->ensureBuiltinMaterial(BuiltinMaterialType::Sprite),
            m_canvas->getTransform(),
            this,
            Vec2F(0.0f, 0.0f),
            Vec2F(0.5f));
        fakeView->setColor(ColorU32(255, 160, 0, 255));
        m_dragAndDropView = fakeView->getTransform();
        */

        return true;
    }

    //////////////////////////////////////////
    void SceneDragAndDropDefault::update(F32 _dt)
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
                m_currentDropZoneEid = c_invalidEntityId;

                m_dragAndDropView->setLocalPosition(renderTargetCursorPos);

                m_inputSystemSample->process(
                    [&](Entity*, InputSystem2D* _inputSystem)
                    {
                        CursorElementTraceParams traceParams;
                        traceParams.ignoreElements.insert(m_element->getEntityId());

                        m_dragAndDropZonesSample->process(
                            [&](Entity*, DragAndDropZone* _zone)
                            {
                                if (_inputSystem->traceElement(
                                    _zone->getUIElement().get(),
                                    renderTargetCursorPos,
                                    traceParams))
                                {
                                    m_currentDropZoneEid = _zone->getEntityId();
                                }
                            });
                    });

                

            }
            else
                tryDrop();
        }
    }

    //////////////////////////////////////////
    EcsWorld* SceneDragAndDropDefault::assignWorld()
    {
        return EntityManager::GetInstancePtr()->getDefaultWorldRaw();
    }

    //////////////////////////////////////////
    void SceneDragAndDropDefault::startDrag(Transform2DPtr const& _viewEntity, DataBlock const& _data)
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
    void SceneDragAndDropDefault::cancelDrag()
    {
        if (m_element)
            m_element->setCaptureCursorHits(false);

        m_viewEntityId = c_invalidEntityId;
        m_data.clear();

        if (!m_dragAndDropView)
            return;

        if (m_dragAndDropView->getEntityRaw())
            m_dragAndDropView->getEntityRaw()->removeFromEcsWorld();

        m_dragAndDropView.reset();
    }

    //////////////////////////////////////////
    void SceneDragAndDropDefault::tryDrop()
    {
        if (m_currentDropZoneEid != c_invalidEntityId)
        {
            getWorld()->sendEvent<DragAndDropEvent>(m_currentDropZoneEid, m_data, m_viewEntityId);
        }

        cancelDrag();
    }

} // namespace Maze
//////////////////////////////////////////
