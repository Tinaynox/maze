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
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ViewportTransformPolicy);


    //////////////////////////////////////////
    // Class Canvas
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Canvas, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Rect2F, viewport, Rect2F(0.0f, 0.0f, 1.0f, 1.0f), getViewport, setViewport),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clearColorFlag, true, getClearColorFlag, setClearColorFlag),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, clearColor, ColorU32(99, 101, 140, 255), getClearColor, setClearColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clearDepthFlag, true, getClearDepthFlag, setClearDepthFlag),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, clipViewport, false, getClipViewport, setClipViewport),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, sortOrder, 0, getSortOrder, setSortOrder),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ViewportTransformPolicy, viewportTransformPolicy, ViewportTransformPolicy::ViewportToTransform, getViewportTransformPolicy, setViewportTransformPolicy));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Canvas);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_INDEXED_RESOURCE(Canvas);

    //////////////////////////////////////////
    Canvas::Canvas()
        : m_viewport(0.0f, 0.0f, 1.0f, 1.0f)
        , m_clearColorFlag(false)
        , m_clearColor(ColorU32(99, 101, 140, 255))
        , m_clearDepthFlag(false)
        , m_sortOrder(0)
        , m_originPosition(Vec2F::c_zero)
        , m_viewportTransformPolicy(ViewportTransformPolicy::ViewportToTransform)
    {
    }

    //////////////////////////////////////////
    Canvas::~Canvas()
    {
        setRenderTarget(nullptr);
    }

    //////////////////////////////////////////
    CanvasPtr Canvas::Create()
    {
        CanvasPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Canvas, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Canvas::init()
    {
        

        return true;
    }

    //////////////////////////////////////////
    void Canvas::prepareFrame(F32 _dt)
    {
        if (m_viewportTransformPolicy == ViewportTransformPolicy::TransformToViewport)
            updateCanvasTransform();
    }

    //////////////////////////////////////////
    void Canvas::setRenderTarget(RenderTargetPtr const& _renderTarget)
    {
        if (m_renderTarget == _renderTarget)
            return;

        if (m_renderTarget)
        {
            m_renderTarget->eventRenderTargetResized.unsubscribe(this);
        }

        m_renderTarget = _renderTarget;

        if (m_renderTarget)
        {
            m_renderTarget->eventRenderTargetResized.subscribe(this, &Canvas::notifyRenderTargetResized);

            if (m_viewportTransformPolicy == ViewportTransformPolicy::ViewportToTransform)
                updateCanvasTransform();
        }

        eventRenderTargetChanged(this, m_renderTarget);
    }

    //////////////////////////////////////////
    void Canvas::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        if (m_viewportTransformPolicy == ViewportTransformPolicy::ViewportToTransform)
            updateCanvasTransform();
    }

    //////////////////////////////////////////
    void Canvas::setViewport(Rect2F const& _viewport)
    {
        if (m_viewport == _viewport)
            return;

        m_viewport = _viewport;
        MAZE_DEBUG_ERROR_IF(!m_viewport.position.isFinite() || !m_viewport.size.isFinite(), "Viewport is not finite!");

        if (m_viewportTransformPolicy == ViewportTransformPolicy::ViewportToTransform)
            updateCanvasTransform();
    }

    //////////////////////////////////////////
    Rect2F Canvas::getRenderTargetRect() const
    {
        if (!getRenderTarget())
            return Rect2F::c_zero;

        Rect2F viewportRect = getViewport();
        viewportRect.position *= (Vec2F)getRenderTarget()->getRenderTargetSize();
        viewportRect.size *= (Vec2F)getRenderTarget()->getRenderTargetSize();
        viewportRect.position = Math::Round(viewportRect.position);
        viewportRect.size = Math::Round(viewportRect.size);
        return viewportRect;
    }

    //////////////////////////////////////////
    AABB2D Canvas::getRenderTargetAABB() const
    {
        return AABB2D::FromRect2(getRenderTargetRect());
    }

    //////////////////////////////////////////
    void Canvas::updateCanvasTransform()
    {
        switch (m_viewportTransformPolicy)
        {
            case ViewportTransformPolicy::None:
                break;
            case ViewportTransformPolicy::ViewportToTransform:
            {
                if (!m_renderTarget)
                    return;

                m_transform->setAnchor(Vec2F::c_zero);
                m_transform->setPivot(Vec2F(0.5f, 0.5f));
                Vec2F size = Math::Round((Vec2F)m_renderTarget->getRenderTargetSize() * m_viewport.size);
                m_transform->setLocalPosition(size * 0.5f);

                m_originPosition = Math::Round(m_viewport.position * (Vec2F)m_renderTarget->getRenderTargetSize());
                MAZE_DEBUG_ERROR_IF(!m_originPosition.isFinite(), "Origin position is not finite!");

                auto canvasScaler = m_canvasScaler.lock();
                if (canvasScaler)
                    canvasScaler->updateCanvasScale();

                break;
            }
            case ViewportTransformPolicy::TransformToViewport:
            {
                Canvas* canvas = nullptr;
                if (m_transform->getParent())
                    canvas = m_transform->getParent()->getFirstTrunkComponent<Canvas>();

                if (!canvas)
                    canvas = this;

                Vec2F const& size = m_transform->getSize();

                Vec2F lbCanvasSpace = m_transform->getWorldTransform().transform(Vec2F::c_zero);
                Vec2F rtCanvasSpace = m_transform->getWorldTransform().transform(size);

                Vec2F lbRenderTargetSpace = canvas->convertViewportCoordsToRenderTargetCoords(lbCanvasSpace);
                Vec2F rtRenderTargetSpace = canvas->convertViewportCoordsToRenderTargetCoords(rtCanvasSpace);

                if (canvas->getRenderTarget())
                {
                    F32 renderTargetWidth = (F32)canvas->getRenderTarget()->getRenderTargetWidth();
                    F32 renderTargetHeight = (F32)canvas->getRenderTarget()->getRenderTargetHeight();

                    if (renderTargetWidth > 0.0f && renderTargetHeight > 0.0f)
                    {
                        Rect2F listVieport(
                            lbRenderTargetSpace.x / renderTargetWidth,
                            lbRenderTargetSpace.y / renderTargetHeight,
                            (rtRenderTargetSpace.x - lbRenderTargetSpace.x) / renderTargetWidth,
                            (rtRenderTargetSpace.y - lbRenderTargetSpace.y) / renderTargetHeight);

                        setViewport(listVieport);
                    }

                    if (canvas != this)
                        setRenderTarget(canvas->getRenderTarget());
                }
                

                break;
            }
            default:
                MAZE_NOT_IMPLEMENTED
                break;
        }
        
    }

    //////////////////////////////////////////
    void Canvas::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_canvasScaler = getEntityRaw()->ensureComponent<CanvasScaler>();
    }
    

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(CanvasPreparationSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        Canvas* _canvas)
    {
        _canvas->prepareFrame(_event.getDt());
    }
    
} // namespace Maze
//////////////////////////////////////////
