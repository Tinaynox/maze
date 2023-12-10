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
#pragma once
#if (!defined(_MazeScrollRect2D_hpp_))
#define _MazeScrollRect2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-ui/ecs/components/MazeScrollbar2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ScrollRect2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(
        MAZE_UI_API,
        ScrollRect2DMovementType,
        Unrestricted,
        Elastic,
        Clamped);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(
        MAZE_UI_API,
        ScrollRect2DScrollbarVisibility,
        Permanent,
        AutoHide,
        AutoHideAndExpandViewport);


    //////////////////////////////////////////
    // Class ScrollRect2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ScrollRect2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ScrollRect2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ScrollRect2D);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        using CallbackFunction = std::function<void(MenuListTree2DPtr const&)>;

    public:

        //////////////////////////////////////////
        virtual ~ScrollRect2D();

        //////////////////////////////////////////
        static ScrollRect2DPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        inline bool getHorizontalScroll() const { return m_horizontalScroll; }

        //////////////////////////////////////////
        inline void setHorizontalScroll(bool _value) { m_horizontalScroll = _value; }

        //////////////////////////////////////////
        inline bool getVerticalScroll() const { return m_verticalScroll; }

        //////////////////////////////////////////
        inline void setVerticalScroll(bool _value) { m_verticalScroll = _value; }

        //////////////////////////////////////////
        inline ScrollRect2DMovementType getMovementType() const { return m_movementType; }

        //////////////////////////////////////////
        inline void setMovementType(ScrollRect2DMovementType _value) { m_movementType = _value; }

        //////////////////////////////////////////
        inline F32 getElasticity() const { return m_elasticity; }

        //////////////////////////////////////////
        inline void setElasticity(F32 _value) { m_elasticity = _value; }

        //////////////////////////////////////////
        inline bool getInertiaEnabled() const { return m_inertiaEnabled; }

        //////////////////////////////////////////
        inline void setInertiaEnabled(bool _value) { m_inertiaEnabled = _value; }

        //////////////////////////////////////////
        inline F32 getDecelerationRate() const { return m_decelerationRate; }

        //////////////////////////////////////////
        inline void setDecelerationRate(F32 _value) { m_decelerationRate = _value; }

        //////////////////////////////////////////
        inline F32 getScrollSensitivity() const { return m_scrollSensitivity; }

        //////////////////////////////////////////
        inline void setScrollSensitivity(F32 _value) { m_scrollSensitivity = _value; }

        //////////////////////////////////////////
        inline ScrollRect2DScrollbarVisibility getHorizontalScrollbarVisibility() const { return m_horizontalScrollbarVisibility; }

        //////////////////////////////////////////
        inline void setHorizontalScrollbarVisibility(ScrollRect2DScrollbarVisibility _value) { m_horizontalScrollbarVisibility = _value; }

        //////////////////////////////////////////
        inline ScrollRect2DScrollbarVisibility getVerticalScrollbarVisibility() const { return m_verticalScrollbarVisibility; }

        //////////////////////////////////////////
        inline void setVerticalScrollbarVisibility(ScrollRect2DScrollbarVisibility _value) { m_verticalScrollbarVisibility = _value; }


        //////////////////////////////////////////
        void setHorizontalNormalizedPosition(F32 _value);

        //////////////////////////////////////////
        F32 getHorizontalNormalizedPosition();

        //////////////////////////////////////////
        void setVerticalNormalizedPosition(F32 _value);

        //////////////////////////////////////////
        F32 getVerticalNormalizedPosition();

        //////////////////////////////////////////
        void setNormalizedPosition(Vec2F const& _value);

        //////////////////////////////////////////
        Vec2F getNormalizedPosition();

        //////////////////////////////////////////
        F32 getNormalizedPosition(Size _axis);

        //////////////////////////////////////////
        void setNormalizedPosition(F32 _value, Size _axis);

        //////////////////////////////////////////
        inline void setContentTransform(Transform2DPtr const& _value)
        {
            if (m_contentTransform == _value)
                return;

            m_contentTransform = _value;
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getContentTransform() const { return m_contentTransform; }

        //////////////////////////////////////////
        inline void setContentTransform(ComponentPtr _value) { setContentTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getContentTransformComponent() const { return m_contentTransform; }


        //////////////////////////////////////////
        inline void setViewportTransform(Transform2DPtr const& _value)
        {
            if (m_viewportTransform == _value)
                return;

            m_viewportTransform = _value;
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getViewportTransform() const { return m_viewportTransform; }

        //////////////////////////////////////////
        inline void setViewportTransform(ComponentPtr _value) { setViewportTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getViewportTransformComponent() const { return m_viewportTransform; }


        //////////////////////////////////////////
        void setHorizontalScrollbar(Scrollbar2DPtr const& _value);

        //////////////////////////////////////////
        inline Scrollbar2DPtr const& getHorizontalScrollbar() const { return m_horizontalScrollbar; }

        //////////////////////////////////////////
        inline void setHorizontalScrollbar(ComponentPtr _value) { setHorizontalScrollbar(_value ? _value->safeCast<Scrollbar2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getHorizontalScrollbarComponent() const { return m_horizontalScrollbar; }


        //////////////////////////////////////////
        void setVerticalScrollbar(Scrollbar2DPtr const& _value);

        //////////////////////////////////////////
        inline Scrollbar2DPtr const& getVerticalScrollbar() const { return m_verticalScrollbar; }

        //////////////////////////////////////////
        inline void setVerticalScrollbar(ComponentPtr _value) { setVerticalScrollbar(_value ? _value->safeCast<Scrollbar2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getVerticalScrollbarComponent() const { return m_verticalScrollbar; }

    public:

        //////////////////////////////////////////
        inline Vec2F const& getVelocity() const { return m_velocity; }

        //////////////////////////////////////////
        inline void setVelocity(Vec2F const& _value) { m_velocity = _value; }

    public:

        //////////////////////////////////////////
        MultiDelegate<ScrollRect2D*, Vec2F> eventValueChanged;

    public:

        //////////////////////////////////////////
        static Vec2F CalculateOffset(
            AABB2D const& _boundsViewSpace,
            AABB2D const& _contentBoundsViewSpace,
            bool _horizontalScroll,
            bool _verticalScroll,
            ScrollRect2DMovementType _movementType,
            Vec2F const& _delta);

        //////////////////////////////////////////
        static bool UpdateOneScrollbarVisibility(
            bool _scrollingNeeded,
            bool _axisEnabled,
            ScrollRect2DScrollbarVisibility _scrollbarVisibility,
            Scrollbar2DPtr const& _scrollbar);

    protected:

        //////////////////////////////////////////
        ScrollRect2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityStarted() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void notifyHorizontalScrollbarValueChanged(Scrollbar2D* _scrollbar, F32 _value);

        //////////////////////////////////////////
        void notifyVerticalScrollbarValueChanged(Scrollbar2D* _scrollbar, F32 _value);

        //////////////////////////////////////////
        void updateBounds();

        //////////////////////////////////////////
        Transform2DPtr const& ensureViewTransform();

        //////////////////////////////////////////
        AABB2D calculateContentBounds();

        //////////////////////////////////////////
        Vec2F calculateOffset(Vec2F const& _delta);

        //////////////////////////////////////////
        void updatePrevData();

        //////////////////////////////////////////
        void updateScrollbars(Vec2F const& _offset);

        //////////////////////////////////////////
        bool updateScrollbarVisibility();

        //////////////////////////////////////////
        inline bool getHorizontalScrollingNeeded() const { return m_contentBoundsViewSpace.getWidth() > m_boundsViewSpace.getWidth() + 0.01f; }

        //////////////////////////////////////////
        inline bool getVerticalScrollingNeeded() const { return m_contentBoundsViewSpace.getHeight() > m_boundsViewSpace.getHeight() + 0.01f; }

        //////////////////////////////////////////
        void updateScrollbarsExpand();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        bool m_horizontalScroll;
        bool m_verticalScroll;
        ScrollRect2DMovementType m_movementType;
        F32 m_elasticity;
        bool m_inertiaEnabled;
        F32 m_decelerationRate;
        F32 m_scrollSensitivity;
        ScrollRect2DScrollbarVisibility m_horizontalScrollbarVisibility;
        ScrollRect2DScrollbarVisibility m_verticalScrollbarVisibility;

        Transform2DPtr m_contentTransform;
        Transform2DPtr m_viewportTransform;
        Scrollbar2DPtr m_horizontalScrollbar;
        Scrollbar2DPtr m_verticalScrollbar;

    protected:
        Vec2F m_velocity;

        Transform2DPtr m_viewRectTransform;
        AABB2D m_boundsViewSpace;
        AABB2D m_contentBoundsViewSpace;

        Vec2F m_prevPosition;
        AABB2D m_prevBoundsViewSpace;
        AABB2D m_prevContentBoundsViewSpace;

        bool m_horizontalScrollbarExpand;
        bool m_verticalScrollbarExpand;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScrollRect2D_hpp_
//////////////////////////////////////////
