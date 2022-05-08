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
#if (!defined(_MazeSlider2D_hpp_))
#define _MazeSlider2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Slider2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class Slider2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API Slider2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Slider2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Slider2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Slider2D();

        //////////////////////////////////////////
        static Slider2DPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }

        //////////////////////////////////////////
        inline bool getPressed() const { return m_UIElement2D->getPressed(); }


        //////////////////////////////////////////
        inline bool getSelected() const { return m_selected; }

        //////////////////////////////////////////
        void setSelected(bool _selected);


        //////////////////////////////////////////
        inline F32 getValue() const { return m_value; }

        //////////////////////////////////////////
        void setValue(F32 _value);



        //////////////////////////////////////////
        inline void setFillRectTransform(Transform2DPtr const& _value)
        {
            if (m_fillRectTransform == _value)
                return;

            m_fillRectTransform = _value;

            processValue();
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getFillRectTransform() const { return m_fillRectTransform; }

        //////////////////////////////////////////
        inline void setFillRectTransform(ComponentPtr _value) { setFillRectTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getFillRectTransformComponent() const { return m_fillRectTransform; }


        //////////////////////////////////////////
        inline void setHandleTransform(Transform2DPtr const& _value)
        {
            if (m_handleTransform == _value)
                return;

            m_handleTransform = _value;

            processValue();
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getHandleTransform() const { return m_handleTransform; }

        //////////////////////////////////////////
        inline void setHandleTransform(ComponentPtr _value) { setHandleTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getHandleTransformComponent() const { return m_handleTransform; }


    public:

        MultiDelegate<Slider2D*, CursorInputEvent const&> eventClick;
        MultiDelegate<Slider2D*, bool> eventFocusChanged;
        MultiDelegate<Slider2D*, bool> eventPressedChanged;
        MultiDelegate<Slider2D*, bool> eventSelectedChanged;
        MultiDelegate<Slider2D*, F32> eventValueChanged;
        
    protected:

        //////////////////////////////////////////
        Slider2D();

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
        void notifyCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorMoveOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);


        //////////////////////////////////////////
        void processValue();

        //////////////////////////////////////////
        void pickValueByCursor(Vec2DF const& _positionWS);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        Transform2DPtr m_fillRectTransform;
        Transform2DPtr m_handleTransform;
        
        bool m_selected;
        F32 m_value;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSlider2D_hpp_
//////////////////////////////////////////
