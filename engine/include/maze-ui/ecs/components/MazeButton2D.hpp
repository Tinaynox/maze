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
#if (!defined(_MazeButton2D_hpp_))
#define _MazeButton2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);


    //////////////////////////////////////////
    // Class Button2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API Button2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Button2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Button2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Button2D();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }

        //////////////////////////////////////////
        inline bool getPressed() const { return m_UIElement2D->getPressed(); }


        //////////////////////////////////////////
        inline bool getEnabled() const { return m_enabled; }

        //////////////////////////////////////////
        void setEnabled(bool _value);


        //////////////////////////////////////////
        inline void setTransitionSprite(SpriteRenderer2DPtr const& _transitionSprite)
        {
            if (m_transitionSprite == _transitionSprite)
                return;

            m_transitionSprite = _transitionSprite;

            updateTransitionUI();
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getTransitionSprite() const { return m_transitionSprite; }

        //////////////////////////////////////////
        inline void setTransitionSprite(ComponentPtr _value) { setTransitionSprite(_value ? _value->cast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getTransitionSpriteComponent() const { return m_transitionSprite; }


        //////////////////////////////////////////
        inline void setNormalColor(ColorU32 const& _color) { m_normalColor = _color; updateTransitionUI(); }

        //////////////////////////////////////////
        inline ColorU32 const& getNormalColor() const { return m_normalColor; }

        //////////////////////////////////////////
        inline void setFocusedColor(ColorU32 const& _color) { m_focusedColor = _color; updateTransitionUI(); }

        //////////////////////////////////////////
        inline ColorU32 const& getFocusedColor() const { return m_focusedColor; }

        //////////////////////////////////////////
        inline void setPressedColor(ColorU32 const& _color) { m_pressedColor = _color; updateTransitionUI(); }

        //////////////////////////////////////////
        inline ColorU32 const& getPressedColor() const { return m_pressedColor; }

        //////////////////////////////////////////
        inline void setSelectedColor(ColorU32 const& _color) { m_selectedColor = _color; updateTransitionUI(); }

        //////////////////////////////////////////
        inline ColorU32 const& getSelectedColor() const { return m_selectedColor; }

        //////////////////////////////////////////
        inline void setDisabledColor(ColorU32 const& _color) { m_disabledColor = _color; updateTransitionUI(); }

        //////////////////////////////////////////
        inline ColorU32 const& getDisabledColor() const { return m_disabledColor; }

    public:

        MultiDelegate<Button2D*, CursorInputEvent const&> eventClick;
        MultiDelegate<Button2D*, CursorInputEvent const&> eventSingleClick;
        MultiDelegate<Button2D*, CursorInputEvent const&> eventDoubleClick;
        MultiDelegate<Button2D*, bool> eventFocusChanged;
        MultiDelegate<Button2D*, bool> eventPressedChanged;
        MultiDelegate<Button2D*, bool> eventEnabledChanged;
        MultiDelegate<Button2D*, Vec2DF const&, CursorInputEvent const&> eventCursorPressIn;

    protected:

        //////////////////////////////////////////
        Button2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifySingleClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyDoubleClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);


        //////////////////////////////////////////
        virtual void updateTransitionUI();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        bool m_enabled = true;

        SpriteRenderer2DPtr m_transitionSprite;
        ColorU32 m_normalColor;
        ColorU32 m_focusedColor;
        ColorU32 m_pressedColor;
        ColorU32 m_selectedColor;
        ColorU32 m_disabledColor;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeButton2D_hpp_
//////////////////////////////////////////
