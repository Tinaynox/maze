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
#if (!defined(_MazeSystemTextEditBox2D_hpp_))
#define _MazeSystemTextEditBox2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
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
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class SystemTextEditBox2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API SystemTextEditBox2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SystemTextEditBox2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SystemTextEditBox2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SystemTextEditBox2D();

        //////////////////////////////////////////
        static SystemTextEditBox2DPtr Create();


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
        inline void setSystemTextRenderer(SystemTextRenderer2DPtr const& _systemTextRenderer)
        {
            if (m_systemTextRenderer == _systemTextRenderer)
                return;

            m_systemTextRenderer = _systemTextRenderer;

            updateSystemTextRenderer();
            updateCursorRendererPosition();
        }

        //////////////////////////////////////////
        inline SystemTextRenderer2DPtr const& getSystemTextRenderer() const { return m_systemTextRenderer; }

        //////////////////////////////////////////
        inline void setSystemTextRenderer(ComponentPtr _avatar)
        {
            if (_avatar)
            {
                MAZE_DEBUG_BP_RETURN_IF(_avatar->getClassUID() != ClassInfo<SystemTextRenderer2D>::UID());
                setSystemTextRenderer(_avatar->cast<SystemTextRenderer2D>());
            }
            else
            {
                setSystemTextRenderer(SystemTextRenderer2DPtr());
            }
        }

        //////////////////////////////////////////
        inline ComponentPtr getSystemTextRendererComponent() const { return m_systemTextRenderer; }


        //////////////////////////////////////////
        inline void setCursorRenderer(SpriteRenderer2DPtr const& _cursorRenderer)
        {
            if (m_cursorRenderer == _cursorRenderer)
                return;

            m_cursorRenderer = _cursorRenderer;

            updateCursorRendererPosition();
            updateCursorRendererEnabled();
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getCursorRenderer() const { return m_cursorRenderer; }

        //////////////////////////////////////////
        inline void setCursorRenderer(ComponentPtr _component)
        {
            if (_component)
            {
                MAZE_DEBUG_BP_RETURN_IF(_component->getClassUID() != ClassInfo<SpriteRenderer2D>::UID());
                setCursorRenderer(_component->cast<SpriteRenderer2D>());
            }
            else
            {
                setCursorRenderer(SpriteRenderer2DPtr());
            }
        }

        //////////////////////////////////////////
        inline ComponentPtr getCursorRendererComponent() const { return m_cursorRenderer; }


        //////////////////////////////////////////
        void setText(String const& _text);

        //////////////////////////////////////////
        inline void setText(CString _text) { setText(String(_text)); }

        //////////////////////////////////////////
        inline String const& getText() const { return m_text; }

        //////////////////////////////////////////
        inline void addChar(char _char)
        {
            m_text.push_back(_char);

            processTextChanged();
        }

        //////////////////////////////////////////
        inline void popChar()
        {
            if (m_text.empty())
                return;

            m_text.pop_back();

            processTextChanged();
        }

        //////////////////////////////////////////
        inline bool isTextInputAvailable()
        {
            return getSelected() && !getUIElement()->getPressed();
        }

        //////////////////////////////////////////
        void setHorizontalAlignment(HorizontalAlignment2D _alignment);

        //////////////////////////////////////////
        void setVerticalAlignment(VerticalAlignment2D _alignment);


    public:

        MultiDelegate<SystemTextEditBox2D*, CursorInputEvent const&> eventClick;
        MultiDelegate<SystemTextEditBox2D*, bool> eventFocusChanged;
        MultiDelegate<SystemTextEditBox2D*, bool> eventPressedChanged;
        MultiDelegate<SystemTextEditBox2D*, bool> eventSelectedChanged;
        MultiDelegate<SystemTextEditBox2D*> eventTextInput;
        MultiDelegate<SystemTextEditBox2D*, String const&> eventTextChanged;

    protected:

        //////////////////////////////////////////
        SystemTextEditBox2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyText(InputEventTextData const& _event);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _event);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void notifyCursorPressOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void updateSystemTextRenderer();

        //////////////////////////////////////////
        void updateCursorRendererPosition();

        //////////////////////////////////////////
        void updateCursorRendererEnabled();

        //////////////////////////////////////////
        void processTextChanged();

        //////////////////////////////////////////
        inline void resetBlinkTimer()
        {
            m_cursorBlink = true;
            m_cursorBlinkTimer = 0.0f;
        }

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;
        SystemTextRenderer2DPtr m_systemTextRenderer;
        SpriteRenderer2DPtr m_cursorRenderer;

        String m_text;

        bool m_selected;

        F32 m_cursorBlinkTimer;
        F32 m_cursorBlinkTime;
        bool m_cursorBlink;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemTextEditBox2D_hpp_
//////////////////////////////////////////
