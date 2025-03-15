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
#if (!defined(_MazeAnimationCurveEdit2D_hpp_))
#define _MazeAnimationCurveEdit2D_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(AnimationCurveEdit2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_MANAGED_SHARED_PTR(Texture2D);


    //////////////////////////////////////////
    // Class AnimationCurveEdit2D
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API AnimationCurveEdit2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AnimationCurveEdit2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AnimationCurveEdit2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~AnimationCurveEdit2D();

        //////////////////////////////////////////
        static AnimationCurveEdit2DPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }


        //////////////////////////////////////////
        inline bool getSelected() const { return m_selected; }

        //////////////////////////////////////////
        void setSelected(bool _selected);


        //////////////////////////////////////////
        inline void setCurveRenderer(SpriteRenderer2DPtr const& _gradientRenderer)
        {
            if (m_curveRenderer == _gradientRenderer)
                return;

            m_curveRenderer = _gradientRenderer;

            subscribeCurveRenderer();

            processCurve();
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getCurveRenderer() const { return m_curveRenderer; }

        //////////////////////////////////////////
        inline void setCurveRenderer(ComponentPtr _avatar) { setCurveRenderer(_avatar ? _avatar->cast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getCurveRendererComponent() const { return m_curveRenderer; }





        //////////////////////////////////////////
        void setCurve(AnimationCurve const& _value);

        //////////////////////////////////////////
        inline AnimationCurve const& getCurve() const { return m_curve; }


        //////////////////////////////////////////
        inline AnimationCurveMinMaxMode getMinMaxMode() const { return m_curve.getMinMaxMode(); }

        //////////////////////////////////////////
        inline void setMinMaxMode(AnimationCurveMinMaxMode _value) { m_curve.setMinMaxMode(_value); }


        //////////////////////////////////////////
        void normalize();

    public:

        MultiDelegate<AnimationCurveEdit2D*> eventClick;
        MultiDelegate<AnimationCurveEdit2D*, bool> eventFocusChanged;
        MultiDelegate<AnimationCurveEdit2D*, bool> eventPressedChanged;
        MultiDelegate<AnimationCurveEdit2D*, bool> eventSelectedChanged;
        MultiDelegate<AnimationCurveEdit2D*, AnimationCurve const&> eventCurveChanged;

    protected:

        //////////////////////////////////////////
        AnimationCurveEdit2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2F const& _positionOS, CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCurve();


        //////////////////////////////////////////
        void subscribeCurveRenderer();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        SpriteRenderer2DPtr m_curveRenderer;        
        
        AnimationCurve m_curve;

        bool m_selected;

        PixelSheet2D m_curvePixelSheet;
        Texture2DPtr m_curveTexture;
        SpritePtr m_curveSprite;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAnimationCurveEdit2D_hpp_
//////////////////////////////////////////
