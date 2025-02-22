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
#if (!defined(_MazeScrollbar2D_hpp_))
#define _MazeScrollbar2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-ui/ecs/components/MazeScrollbar2DDirection.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Scrollbar2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);


    //////////////////////////////////////////
    // Class Scrollbar2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API Scrollbar2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Scrollbar2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Scrollbar2D);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        using CallbackFunction = std::function<void(MenuListTree2DPtr const&)>;

    public:

        //////////////////////////////////////////
        virtual ~Scrollbar2D();

        //////////////////////////////////////////
        static Scrollbar2DPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }

        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }

        //////////////////////////////////////////
        inline bool getPressed() const { return m_UIElement2D->getPressed(); }



        //////////////////////////////////////////
        inline Scrollbar2DDirection getDirection() const { return m_direction; }

        //////////////////////////////////////////
        inline void setDirection(Scrollbar2DDirection _value) { m_direction = _value; }


        //////////////////////////////////////////
        inline void setHandleTransform(Transform2DPtr const& _value)
        {
            if (m_handleTransform == _value)
                return;

            m_handleTransform = _value;

            updateUI();
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getHandleTransform() const { return m_handleTransform; }

        //////////////////////////////////////////
        inline void setHandleTransform(ComponentPtr _value) { setHandleTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getHandleTransformComponent() const { return m_handleTransform; }


        //////////////////////////////////////////
        inline void setHandleSpriteRenderer(SpriteRenderer2DPtr const& _value)
        {
            if (m_handleSpriteRenderer == _value)
                return;

            m_handleSpriteRenderer = _value;
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getHandleSpriteRenderer() const { return m_handleSpriteRenderer; }

        //////////////////////////////////////////
        inline void setHandleSpriteRenderer(ComponentPtr _value) { setHandleSpriteRenderer(_value ? _value->safeCast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getHandleSpriteRendererComponent() const { return m_handleSpriteRenderer; }


        //////////////////////////////////////////
        inline bool getSelected() const { return m_selected; }

        //////////////////////////////////////////
        void setSelected(bool _selected);


        //////////////////////////////////////////
        inline F32 getValue() const { return m_value; }

        //////////////////////////////////////////
        void setValue(F32 _value);

        //////////////////////////////////////////
        inline F32 getHandleSize() const { return m_handleSize; }

        //////////////////////////////////////////
        void setHandleSize(F32 _value);


        //////////////////////////////////////////
        inline void setNormalColor(ColorU32 const& _color) { m_normalColor = _color; }

        //////////////////////////////////////////
        inline ColorU32 const& getNormalColor() const { return m_normalColor; }

        //////////////////////////////////////////
        inline void setFocusedColor(ColorU32 const& _color) { m_focusedColor = _color; }

        //////////////////////////////////////////
        inline ColorU32 const& getFocusedColor() const { return m_focusedColor; }

        //////////////////////////////////////////
        inline void setPressedColor(ColorU32 const& _color) { m_pressedColor = _color; }

        //////////////////////////////////////////
        inline ColorU32 const& getPressedColor() const { return m_pressedColor; }

        //////////////////////////////////////////
        inline void setSelectedColor(ColorU32 const& _color) { m_selectedColor = _color; }

        //////////////////////////////////////////
        inline ColorU32 const& getSelectedColor() const { return m_selectedColor; }

        //////////////////////////////////////////
        inline void setDisabledColor(ColorU32 const& _color) { m_disabledColor = _color; }

        //////////////////////////////////////////
        inline ColorU32 const& getDisabledColor() const { return m_disabledColor; }


        //////////////////////////////////////////
        void updateUI();

    public:

        MultiDelegate<Scrollbar2D*, CursorInputEvent const&> eventClick;
        MultiDelegate<Scrollbar2D*, bool> eventFocusChanged;
        MultiDelegate<Scrollbar2D*, bool> eventPressedChanged;
        MultiDelegate<Scrollbar2D*, bool> eventSelectedChanged;
        MultiDelegate<Scrollbar2D*, F32> eventValueChanged;

    protected:

        //////////////////////////////////////////
        Scrollbar2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorMoveOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void pickValueByCursor(Vec2F const& _positionWS);

        //////////////////////////////////////////
        F32 cursorPositionToValue(Vec2F const& _positionWS);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        Scrollbar2DDirection m_direction;
        Transform2DPtr m_handleTransform;
        SpriteRenderer2DPtr m_handleSpriteRenderer;

        ColorU32 m_normalColor;
        ColorU32 m_focusedColor;
        ColorU32 m_pressedColor;
        ColorU32 m_selectedColor;
        ColorU32 m_disabledColor;

        bool m_selected;
        F32 m_value;
        F32 m_handleSize;

        F32 m_deltaValue;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScrollbar2D_hpp_
//////////////////////////////////////////
