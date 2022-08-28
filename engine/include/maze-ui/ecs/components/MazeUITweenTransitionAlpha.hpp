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
#if (!defined(_MazeUITweenTransitionAlpha_hpp_))
#define _MazeUITweenTransitionAlpha_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-core/utils/MazeDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(UITweenTransitionAlpha);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(CanvasGroup);


    //////////////////////////////////////////
    // Class UITweenTransitionAlpha
    //
    //////////////////////////////////////////
    class MAZE_UI_API UITweenTransitionAlpha
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UITweenTransitionAlpha, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(UITweenTransitionAlpha);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~UITweenTransitionAlpha();

        //////////////////////////////////////////
        static UITweenTransitionAlphaPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        CanvasGroupPtr const& getCanvasGroup() const { return m_canvasGroup; }


        //////////////////////////////////////////
        inline F32 getHidingProgress() const { return m_hidingProgress; }


        //////////////////////////////////////////
        inline void setUnscaledDeltaTime(bool _value) { m_unscaledDeltaTime = _value; }

        //////////////////////////////////////////
        inline bool getUnscaledDeltaTime() const { return m_unscaledDeltaTime; }


        //////////////////////////////////////////
        void setHidden(bool _isHidden, bool _resetProgress = false);


        //////////////////////////////////////////
        inline void setHideKoef(F32 _value) { m_hideKoef = _value; }

        //////////////////////////////////////////
        inline F32 getHideKoef() const { return m_hideKoef; }


        //////////////////////////////////////////
        inline void setHiddenAlpha(F32 _value) { m_hiddenAlpha = _value; }

        //////////////////////////////////////////
        inline F32 getHiddenAlpha() const { return m_hiddenAlpha; }

        //////////////////////////////////////////
        inline void setShownAlpha(F32 _value) { m_shownAlpha = _value; }

        //////////////////////////////////////////
        inline F32 getShownAlpha() const { return m_shownAlpha; }


        //////////////////////////////////////////
        void setShowTime(F32 _showTime);

        //////////////////////////////////////////
        void setAlphaPower(F32 _alphaPower) { m_alphaPower = _alphaPower; }


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
        void updateCanvasGroup();

    public:

        MultiDelegate<> eventShowComplete;
        MultiDelegate<> eventHideComplete;

    protected:

        //////////////////////////////////////////
        UITweenTransitionAlpha();

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
        CanvasGroupPtr m_canvasGroup;

        bool m_unscaledDeltaTime = false;

        bool m_isHidden;
        F32 m_showTime;
        F32 m_hideKoef;
        F32 m_hiddenAlpha;
        F32 m_shownAlpha;
        F32 m_alphaPower;

        F32 m_hidingProgress;
        F32 m_showSpeed;
        F32 m_hideSpeed;

        Delegate<void> m_onShowComplete;
        Delegate<void> m_onHideComplete;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUITweenTransitionAlpha_hpp_
//////////////////////////////////////////
