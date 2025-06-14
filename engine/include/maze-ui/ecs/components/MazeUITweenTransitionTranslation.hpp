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
#if (!defined(_MazeUITweenTransitionTranslation_hpp_))
#define _MazeUITweenTransitionTranslation_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionTranslation.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(UITweenTransitionTranslation);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(CanvasGroup);


    //////////////////////////////////////////
    // Class UITweenTransitionTranslation
    //
    //////////////////////////////////////////
    class MAZE_UI_API UITweenTransitionTranslation
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UITweenTransitionTranslation, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(UITweenTransitionTranslation);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~UITweenTransitionTranslation();

        //////////////////////////////////////////
        static UITweenTransitionTranslationPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        inline AnimationCurve const& getAnimationCurve() const { return m_animationCurve; }

        //////////////////////////////////////////
        inline void setAnimationCurve(AnimationCurve const& _value)
        {
            m_animationCurve = _value;
            updateTransform();
        }


        //////////////////////////////////////////
        inline void setUnscaledDeltaTime(bool _value) { m_unscaledDeltaTime = _value; }

        //////////////////////////////////////////
        inline bool getUnscaledDeltaTime() const { return m_unscaledDeltaTime; }


        //////////////////////////////////////////
        Vec2F const& getHiddenPosition() const { return m_hiddenPosition; }

        //////////////////////////////////////////
        void setHiddenPosition(Vec2F const& _hiddenPosition)
        {
            m_hiddenPosition = _hiddenPosition;
            updateTransform();
        }

        //////////////////////////////////////////
        Vec2F const& getShownPosition() const { return m_shownPosition; }

        //////////////////////////////////////////
        void setShownPosition(Vec2F const& _shownPosition)
        {
            m_shownPosition = _shownPosition;
            updateTransform();
        }


        //////////////////////////////////////////
        void setHidden(bool _isHidden, bool _resetProgress);

        //////////////////////////////////////////
        inline void setHidden(bool _isHidden) { setHidden(_isHidden, false); }

        //////////////////////////////////////////
        inline bool getHidden() const { return m_isHidden; }


        //////////////////////////////////////////
        inline void setHideKoef(F32 _value) { m_hideKoef = _value; updateHideSpeed(); }

        //////////////////////////////////////////
        inline F32 getHideKoef() const { return m_hideKoef; }


        //////////////////////////////////////////
        void setShowTime(F32 _showTime);

        //////////////////////////////////////////
        inline F32 getShowTime() const { return m_showTime; }


        //////////////////////////////////////////
        void processUpdate(F32 _dt);


        //////////////////////////////////////////
        void show(Delegate<void> const& _onComplete = Delegate<void>());

        //////////////////////////////////////////
        void hide(Delegate<void> const& _onComplete = Delegate<void>());

        //////////////////////////////////////////
        void showInstantly();

        //////////////////////////////////////////
        void hideInstantly();


        //////////////////////////////////////////
        void updateTransform();

    public:

        MultiDelegate<> eventShowComplete;
        MultiDelegate<> eventHideComplete;

    protected:

        //////////////////////////////////////////
        UITweenTransitionTranslation();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void updateShowSpeed();

        //////////////////////////////////////////
        void updateHideSpeed();

    protected:
        Transform2DPtr m_transform;

        bool m_unscaledDeltaTime = false;

        bool m_isHidden = true;
        AnimationCurve m_animationCurve = AnimationCurve::c_linear01;
        F32 m_showTime = 0.3f;
        F32 m_hideKoef = 1.0f;
        Vec2F m_hiddenPosition = Vec2F::c_zero;
        Vec2F m_shownPosition = Vec2F::c_zero;

        F32 m_hidingProgress = 1.0f;
        F32 m_showSpeed = 0.0f;
        F32 m_hideSpeed = 0.0f;

        Delegate<void> m_onShowComplete;
        Delegate<void> m_onHideComplete;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUITweenTransitionTranslation_hpp_
//////////////////////////////////////////
