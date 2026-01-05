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
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ScrollRect2DMovementType);


    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(ScrollRect2DScrollbarVisibility);


    //////////////////////////////////////////
    // Class ScrollRect2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScrollRect2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, horizontalScroll, true, getHorizontalScroll, setHorizontalScroll),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, verticalScroll, true, getVerticalScroll, setVerticalScroll),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ScrollRect2DMovementType, movementType, ScrollRect2DMovementType::Elastic, getMovementType, setMovementType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, elasticity, 0.1f, getElasticity, setElasticity),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, inertiaEnabled, true, getInertiaEnabled, setInertiaEnabled),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, decelerationRate, 0.135f, getDecelerationRate, setDecelerationRate),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, scrollSensitivity, 1.0f, getScrollSensitivity, setScrollSensitivity),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ScrollRect2DScrollbarVisibility, horizontalScrollbarVisibility, ScrollRect2DScrollbarVisibility::AutoHideAndExpandViewport, getHorizontalScrollbarVisibility, setHorizontalScrollbarVisibility),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ScrollRect2DScrollbarVisibility, verticalScrollbarVisibility, ScrollRect2DScrollbarVisibility::AutoHideAndExpandViewport, getVerticalScrollbarVisibility, setVerticalScrollbarVisibility),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, contentTransform, nullptr, getContentTransformComponent, setContentTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, viewportTransform, nullptr, getViewportTransformComponent, setViewportTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, horizontalScrollbar, nullptr, getHorizontalScrollbarComponent, setHorizontalScrollbar),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, verticalScrollbar, nullptr, getVerticalScrollbarComponent, setVerticalScrollbar));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ScrollRect2D);

    //////////////////////////////////////////
    ScrollRect2D::ScrollRect2D()
        : m_horizontalScroll(true)
        , m_verticalScroll(true)
        , m_movementType(ScrollRect2DMovementType::Elastic)
        , m_elasticity(0.1f)
        , m_inertiaEnabled(true)
        , m_decelerationRate(0.135f)
        , m_scrollSensitivity(1.0f)
        , m_horizontalScrollbarVisibility(ScrollRect2DScrollbarVisibility::AutoHideAndExpandViewport)
        , m_verticalScrollbarVisibility(ScrollRect2DScrollbarVisibility::AutoHideAndExpandViewport)
        , m_velocity(Vec2F::c_zero)
        , m_boundsViewSpace(AABB2D::c_zero)
        , m_contentBoundsViewSpace(AABB2D::c_zero)
        , m_prevPosition(Vec2F::c_zero)
        , m_prevBoundsViewSpace(AABB2D::c_zero)
        , m_prevContentBoundsViewSpace(AABB2D::c_zero)
        , m_horizontalScrollbarExpand(false)
        , m_verticalScrollbarExpand(false)
    {
    }

    //////////////////////////////////////////
    ScrollRect2D::~ScrollRect2D()
    {
        if (m_UIElement2D)
            m_UIElement2D->eventCursorReleaseIn.unsubscribe(this);

        setHorizontalScrollbar(Scrollbar2DPtr());
        setVerticalScrollbar(Scrollbar2DPtr());
    }

    //////////////////////////////////////////
    ScrollRect2DPtr ScrollRect2D::Create()
    {
        ScrollRect2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScrollRect2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ScrollRect2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ScrollRect2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);
    }

    //////////////////////////////////////////
    void ScrollRect2D::processEntityStarted()
    {
        Transform2DPtr const& viewTransform = ensureViewTransform();
        bool viewIsChild = (viewTransform->getParent() == m_transform);
        bool hScrollbarIsChild = (!m_horizontalScrollbar || m_horizontalScrollbar->getTransform()->getParent() == m_transform);
        bool vScrollbarIsChild = (!m_verticalScrollbar || m_verticalScrollbar->getTransform()->getParent() == m_transform);
        bool allAreChildren = (viewIsChild && hScrollbarIsChild && vScrollbarIsChild);

        m_horizontalScrollbarExpand = allAreChildren && m_horizontalScrollbar && m_horizontalScrollbarVisibility == ScrollRect2DScrollbarVisibility::AutoHideAndExpandViewport;
        m_verticalScrollbarExpand = allAreChildren && m_verticalScrollbar && m_verticalScrollbarVisibility == ScrollRect2DScrollbarVisibility::AutoHideAndExpandViewport;

        updatePrevData();
    }

    //////////////////////////////////////////
    void ScrollRect2D::update(F32 _dt)
    {
        if (!m_contentTransform)
            return;

        // #TODO: Optimize this

        updateBounds();

        Vec2F offset = calculateOffset(Vec2F::c_zero);

        if (m_boundsViewSpace != m_prevBoundsViewSpace ||
            m_contentBoundsViewSpace != m_prevContentBoundsViewSpace ||
            m_contentTransform->getLocalPosition() != m_prevPosition)
        {
            updateScrollbars(offset);
            eventValueChanged(this, getNormalizedPosition());
            updatePrevData();
        }

        if (updateScrollbarVisibility())
        {
            bool hScrollingNeeded = getHorizontalScrollingNeeded();
            bool vScrollingNeeded = getVerticalScrollingNeeded();

            if (!hScrollingNeeded)
                setHorizontalNormalizedPosition(0.0f);
            if (!vScrollingNeeded)
                setVerticalNormalizedPosition(0.0f);
        }

        updateScrollbarsExpand();
    }

    //////////////////////////////////////////
    void ScrollRect2D::setHorizontalNormalizedPosition(F32 _value)
    {
        setNormalizedPosition(_value, 0);
    }

    //////////////////////////////////////////
    F32 ScrollRect2D::getHorizontalNormalizedPosition()
    {
        updateBounds();

        if ((m_contentBoundsViewSpace.getWidth() <= m_boundsViewSpace.getWidth()) || Math::Abs(m_contentBoundsViewSpace.getWidth() - m_boundsViewSpace.getWidth()) <= 0.00001f)
            return (m_boundsViewSpace.getMinX() > m_contentBoundsViewSpace.getMinX()) ? 1.0f : 0.0f;

        return (m_boundsViewSpace.getMinX() - m_contentBoundsViewSpace.getMinX()) / (m_contentBoundsViewSpace.getWidth() - m_boundsViewSpace.getWidth());
    }

    //////////////////////////////////////////
    void ScrollRect2D::setVerticalNormalizedPosition(F32 _value)
    {
        setNormalizedPosition(_value, 1);
    }

    //////////////////////////////////////////
    F32 ScrollRect2D::getVerticalNormalizedPosition()
    {
        updateBounds();

        if ((m_contentBoundsViewSpace.getHeight() <= m_boundsViewSpace.getHeight()) || Math::Abs(m_contentBoundsViewSpace.getHeight() - m_boundsViewSpace.getHeight()) <= 0.00001f)
            return (m_boundsViewSpace.getMinY() > m_contentBoundsViewSpace.getMinY()) ? 1.0f : 0.0f;

        return 1.0f - ((m_boundsViewSpace.getMinY() - m_contentBoundsViewSpace.getMinY()) / (m_contentBoundsViewSpace.getHeight() - m_boundsViewSpace.getHeight()));
    }

    //////////////////////////////////////////
    void ScrollRect2D::setNormalizedPosition(Vec2F const& _value)
    {
        setNormalizedPosition(_value.x, 0);
        setNormalizedPosition(_value.y, 1);
    }

    //////////////////////////////////////////
    Vec2F ScrollRect2D::getNormalizedPosition()
    {
        return {
            getHorizontalNormalizedPosition(),
            getVerticalNormalizedPosition()};
    }

    //////////////////////////////////////////
    F32 ScrollRect2D::getNormalizedPosition(Size _axis)
    {
        return getNormalizedPosition()[_axis];
    }

    //////////////////////////////////////////
    void ScrollRect2D::setNormalizedPosition(F32 _value, Size _axis)
    {
        updateBounds();

        F32 hiddenLength = m_contentBoundsViewSpace.getSize()[_axis] - m_boundsViewSpace.getSize()[_axis];

        F32 contentBoundsMinPosition;
        
        if (_axis == 0)
        {
            // #TODO: Research and rework
            if (!getHorizontalScrollingNeeded())
                _value = 0.0f;

            contentBoundsMinPosition = m_boundsViewSpace.getMin()[_axis] - _value * hiddenLength;
        }
        else
        {
            // #TODO: Research and rework
            if (!getVerticalScrollingNeeded())
                _value = 0.0f;

            contentBoundsMinPosition = m_boundsViewSpace.getMin()[_axis] - (1.0f - _value) * hiddenLength;
        }

        F32 newLocalPosition = m_contentTransform->getLocalPosition()[_axis] + contentBoundsMinPosition - m_contentBoundsViewSpace.getMin()[_axis];

        Vec2F localPosition = m_contentTransform->getLocalPosition();
        if (Math::Abs(localPosition[_axis] - newLocalPosition) > 0.01f)
        {
            localPosition[_axis] = newLocalPosition;
            m_contentTransform->setLocalPosition(localPosition);
            m_velocity[_axis] = 0.0f;
            updateBounds();
        }
    }

    //////////////////////////////////////////
    void ScrollRect2D::setHorizontalScrollbar(Scrollbar2DPtr const& _value)
    {
        if (m_horizontalScrollbar == _value)
            return;

        if (m_horizontalScrollbar)
        {
            m_horizontalScrollbar->eventValueChanged.unsubscribe(this);
        }

        m_horizontalScrollbar = _value;

        if (m_horizontalScrollbar)
        {
            m_horizontalScrollbar->eventValueChanged.subscribe(this, &ScrollRect2D::notifyHorizontalScrollbarValueChanged);
        }
    }

    //////////////////////////////////////////
    void ScrollRect2D::setVerticalScrollbar(Scrollbar2DPtr const& _value)
    {
        if (m_verticalScrollbar == _value)
            return;

        if (m_verticalScrollbar)
        {
            m_verticalScrollbar->eventValueChanged.unsubscribe(this);
        }

        m_verticalScrollbar = _value;

        if (m_verticalScrollbar)
        {
            m_verticalScrollbar->eventValueChanged.subscribe(this, &ScrollRect2D::notifyVerticalScrollbarValueChanged);
        }
    }

    //////////////////////////////////////////
    void ScrollRect2D::notifyHorizontalScrollbarValueChanged(Scrollbar2D* _scrollbar, F32 _value)
    {
        setNormalizedPosition(_value, 0);
    }

    //////////////////////////////////////////
    void ScrollRect2D::notifyVerticalScrollbarValueChanged(Scrollbar2D* _scrollbar, F32 _value)
    {
        setNormalizedPosition(_value, 1);
    }

    //////////////////////////////////////////
    void ScrollRect2D::updateBounds()
    {
        Transform2DPtr const& viewTransform = ensureViewTransform();
        if (!viewTransform)
            return;

        m_boundsViewSpace = AABB2D(0.0f, 0.0f, viewTransform->getWidth(), viewTransform->getHeight());
        m_contentBoundsViewSpace = calculateContentBounds();
    }

    //////////////////////////////////////////
    AABB2D ScrollRect2D::calculateContentBounds()
    {
        if (!m_contentTransform)
            return AABB2D();

        Vec2F corners[4];
        m_contentTransform->calculateWorldCorners(corners);

        Transform2DPtr const& viewTransform = ensureViewTransform();
        if (!viewTransform)
            return AABB2D::c_zero;

        TMat worldToViewMatrix = viewTransform->getWorldTransform().inversed();

        Vec2F p = worldToViewMatrix.transform(corners[0]);
        AABB2D bounds(p);

        for (Size i = 1; i < 4; ++i)
        {
            p = worldToViewMatrix.transform(corners[i]);
            bounds.applyUnion(p);
        }

        return bounds;
    }

    //////////////////////////////////////////
    Transform2DPtr const& ScrollRect2D::ensureViewTransform()
    {
        static Transform2DPtr const nullPointer;

        if (m_viewRectTransform == nullptr)
            m_viewRectTransform = m_viewportTransform;

        if (m_viewRectTransform == nullptr)
            m_viewRectTransform = m_transform;

        return m_viewRectTransform;
    }

    //////////////////////////////////////////
    Vec2F ScrollRect2D::calculateOffset(Vec2F const& _delta)
    {
        return CalculateOffset(
            m_boundsViewSpace,
            m_contentBoundsViewSpace,
            m_horizontalScroll,
            m_verticalScroll,
            m_movementType,
            _delta);
    }

    //////////////////////////////////////////
    void ScrollRect2D::updatePrevData()
    {
        if (m_contentTransform == nullptr)
            m_prevPosition = Vec2F::c_zero;
        else
            m_prevPosition = m_contentTransform->getLocalPosition();

        m_prevBoundsViewSpace = m_boundsViewSpace;
        m_prevContentBoundsViewSpace = m_contentBoundsViewSpace;
    }

    //////////////////////////////////////////
    Vec2F ScrollRect2D::CalculateOffset(
        AABB2D const& _boundsViewSpace,
        AABB2D const& _contentBoundsViewSpace,
        bool _horizontalScroll,
        bool _verticalScroll,
        ScrollRect2DMovementType _movementType,
        Vec2F const& _delta)
    {
        Vec2F offset = Vec2F::c_zero;
        if (_movementType == ScrollRect2DMovementType::Unrestricted)
            return offset;

        Vec2F min = _contentBoundsViewSpace.getMin();
        Vec2F max = _contentBoundsViewSpace.getMax();

        if (_horizontalScroll)
        {
            min.x += _delta.x;
            max.x += _delta.x;

            F32 maxOffset = _boundsViewSpace.getMaxX() - max.x;
            F32 minOffset = _boundsViewSpace.getMinX() - min.x;

            if (minOffset < -0.001f)
                offset.x = minOffset;
            else
            if (maxOffset > 0.001f)
                offset.x = maxOffset;
        }

        if (_verticalScroll)
        {
            min.y += _delta.y;
            max.y += _delta.y;

            F32 maxOffset = _boundsViewSpace.getMaxY() - max.y;
            F32 minOffset = _boundsViewSpace.getMinY() - min.y;

            if (maxOffset > 0.001f)
                offset.y = maxOffset;
            else
            if (minOffset < -0.001f)
                offset.y = minOffset;
        }

        return offset;
    }

    //////////////////////////////////////////
    bool ScrollRect2D::UpdateOneScrollbarVisibility(
        bool _scrollingNeeded,
        bool _axisEnabled,
        ScrollRect2DScrollbarVisibility _scrollbarVisibility,
        Scrollbar2DPtr const& _scrollbar)
    {
        if (_scrollbar)
        {
            if (_scrollbarVisibility == ScrollRect2DScrollbarVisibility::Permanent)
            {
                if (_scrollbar->getEntityRaw()->getActiveSelf() != _axisEnabled)
                {
                    _scrollbar->getEntityRaw()->setActiveSelf(_axisEnabled);
                    return true;
                }
            }
            else
            {
                if (_scrollbar->getEntityRaw()->getActiveSelf() != _scrollingNeeded)
                {
                    _scrollbar->getEntityRaw()->setActiveSelf(_scrollingNeeded);
                    return true;
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////
    void ScrollRect2D::updateScrollbars(Vec2F const& _offset)
    {
        if (m_horizontalScrollbar)
        {
            if (m_contentBoundsViewSpace.getWidth() > 0.0f)
            {
                F32 handleSize = Math::Clamp01((m_boundsViewSpace.getWidth() - Math::Abs(_offset.x)) / m_contentBoundsViewSpace.getWidth());
                m_horizontalScrollbar->setHandleSize(handleSize);
            }
            else
            {
                m_horizontalScrollbar->setHandleSize(1.0f);
            }

            F32 normalizedValue = getHorizontalNormalizedPosition();
            m_horizontalScrollbar->setValue(normalizedValue);
        }

        if (m_verticalScrollbar)
        {
            if (m_contentBoundsViewSpace.getHeight() > 0.0f)
            {
                F32 handleSize = Math::Clamp01((m_boundsViewSpace.getHeight() - Math::Abs(_offset.y)) / m_contentBoundsViewSpace.getHeight());
                m_verticalScrollbar->setHandleSize(handleSize);
            }
            else
            {
                m_verticalScrollbar->setHandleSize(1.0f);
            }

            F32 normalizedValue = getVerticalNormalizedPosition();
            m_verticalScrollbar->setValue(normalizedValue);
        }
    }

    //////////////////////////////////////////
    bool ScrollRect2D::updateScrollbarVisibility()
    {
        updateBounds();

        bool hScrollingNeeded = getHorizontalScrollingNeeded();
        bool vScrollingNeeded = getVerticalScrollingNeeded();

        bool result = false;
        result |= UpdateOneScrollbarVisibility(vScrollingNeeded, m_verticalScroll, m_verticalScrollbarVisibility, m_verticalScrollbar);
        result |= UpdateOneScrollbarVisibility(hScrollingNeeded, m_horizontalScroll, m_horizontalScrollbarVisibility, m_horizontalScrollbar);
        return result;
    }

    //////////////////////////////////////////
    void ScrollRect2D::updateScrollbarsExpand()
    {
        SizePolicy2DPtr viewTransformSizePolicy;

        Transform2DPtr const& viewTransform = ensureViewTransform();

        if (m_horizontalScrollbarExpand || m_verticalScrollbarExpand)
        {

            viewTransformSizePolicy = viewTransform->getEntityRaw()->ensureComponent<SizePolicy2D>();
            viewTransformSizePolicy->setSizeDelta(0.0f, 0.0f);

        }

        if (m_verticalScrollbarExpand && getVerticalScrollingNeeded())
        {
            viewTransformSizePolicy->setSizeDelta(
                -(m_verticalScrollbar->getTransform()->getWidth()),
                viewTransformSizePolicy->getSizeDelta().y);
            viewTransformSizePolicy->updateSize();

            m_boundsViewSpace = AABB2D(0.0f, 0.0f, viewTransform->getWidth(), viewTransform->getHeight());
            m_contentBoundsViewSpace = calculateContentBounds();
        }

        if (m_horizontalScrollbarExpand && getHorizontalScrollingNeeded())
        {
            viewTransformSizePolicy->setSizeDelta(
                viewTransformSizePolicy->getSizeDelta().x,
                -(m_horizontalScrollbar->getTransform()->getHeight()));
            viewTransformSizePolicy->updateSize();

            m_boundsViewSpace = AABB2D(0.0f, 0.0f, viewTransform->getWidth(), viewTransform->getHeight());
            m_contentBoundsViewSpace = calculateContentBounds();
        }

        if (m_verticalScrollbarExpand && getVerticalScrollingNeeded() && viewTransformSizePolicy->getSizeDelta().x == 0.0f && viewTransformSizePolicy->getSizeDelta().y < 0.0f)
        {
            viewTransformSizePolicy->setSizeDelta(
                -(m_verticalScrollbar->getTransform()->getWidth()),
                viewTransformSizePolicy->getSizeDelta().y);
        }
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(ScrollRect2DAdded,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        EntityAddedEvent const& _event,
        Entity* _entity,
        ScrollRect2D* _scrollRect2D)
    {
        _scrollRect2D->processEntityStarted();
    }

} // namespace Maze
//////////////////////////////////////////
