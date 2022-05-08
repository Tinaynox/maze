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
#if (!defined(_MazeColorGradientEdit2D_hpp_))
#define _MazeColorGradientEdit2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ColorGradientEdit2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class ColorGradientEdit2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ColorGradientEdit2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ColorGradientEdit2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ColorGradientEdit2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ColorGradientEdit2D();

        //////////////////////////////////////////
        static ColorGradientEdit2DPtr Create();


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
        inline void setGradientRenderer(SpriteRenderer2DPtr const& _gradientRenderer)
        {
            if (m_gradientRenderer == _gradientRenderer)
                return;

            unsubscribeGradientRenderer();

            m_gradientRenderer = _gradientRenderer;

            subscribeGradientRenderer();

            processGradient();
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getGradientRenderer() const { return m_gradientRenderer; }

        //////////////////////////////////////////
        inline void setGradientRenderer(ComponentPtr _avatar) { setGradientRenderer(_avatar ? _avatar->cast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getGradientRendererComponent() const { return m_gradientRenderer; }


        //////////////////////////////////////////
        void setGradient(ColorGradient const& _value);

        //////////////////////////////////////////
        inline ColorGradient const& getGradient() const { return m_gradient; }


    public:

        MultiDelegate<ColorGradientEdit2D*> eventClick;
        MultiDelegate<ColorGradientEdit2D*, bool> eventFocusChanged;
        MultiDelegate<ColorGradientEdit2D*, bool> eventPressedChanged;
        MultiDelegate<ColorGradientEdit2D*, bool> eventSelectedChanged;
        MultiDelegate<ColorGradientEdit2D*, ColorGradient const&> eventGradientChanged;

    protected:

        //////////////////////////////////////////
        ColorGradientEdit2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);


        //////////////////////////////////////////
        void subscribeGradientRenderer();

        //////////////////////////////////////////
        void unsubscribeGradientRenderer();

        //////////////////////////////////////////
        void processGradient();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        SpriteRenderer2DPtr m_gradientRenderer;        
        
        ColorGradient m_gradient;

        bool m_selected;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorGradientEdit2D_hpp_
//////////////////////////////////////////
