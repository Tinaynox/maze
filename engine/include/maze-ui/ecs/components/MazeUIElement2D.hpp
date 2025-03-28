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
#if (!defined(_MazeUIElement2D_hpp_))
#define _MazeUIElement2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(Bounds2D);


    //////////////////////////////////////////
    // Class UIElement2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIElement2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIElement2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(UIElement2D);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        enum Flags
        {
            UnpressOnUnfocus = MAZE_BIT(0),
        };

    public:

        //////////////////////////////////////////
        virtual ~UIElement2D();

        //////////////////////////////////////////
        static UIElement2DPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        Bounds2DPtr const& getBounds() const { return m_bounds; }
            
        //////////////////////////////////////////
        void processCursorPress(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCursorClick(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCursorDoubleClick(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCursorRelease(
            CursorInputEvent& _inputEvent,
            bool _forceOut = false);

        //////////////////////////////////////////
        bool processCursorTrace(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCursorMove(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCursorDrag(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void processCursorWheel(CursorWheelInputEvent const& _inputEvent);

        //////////////////////////////////////////
        bool getCaptureCursorHits() const { return m_captureCursorHits;}

        //////////////////////////////////////////
        void setCaptureCursorHits(bool _value) { m_captureCursorHits = _value; }


        //////////////////////////////////////////
        inline S32 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        inline void setFlags(S32 _flags) { m_flags = _flags; }


        //////////////////////////////////////////
        inline void setUnpressOnUnfocus(bool _value) { setFlag(_value, Flags::UnpressOnUnfocus); }


        //////////////////////////////////////////
        inline bool getFocused() const { return m_focused; }

        //////////////////////////////////////////
        void setFocused(bool _focused);

        //////////////////////////////////////////
        inline bool getPressed() const { return m_pressed; }

        //////////////////////////////////////////
        void setPressed(bool _pressed);


        //////////////////////////////////////////
        inline S32 getCursorIndex() const { return m_cursorIndex; }

    public:

        // positionOS, CursorInputEvent
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventCursorPressIn;
        MultiDelegate<CursorInputEvent&> eventCursorPressOut;
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventCursorMoveIn;
        MultiDelegate<CursorInputEvent&> eventCursorMoveOut;
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventCursorDrag;
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventCursorReleaseIn;
        MultiDelegate<CursorInputEvent&> eventCursorReleaseOut;
        MultiDelegate<CursorInputEvent&> eventCursorRelease;

        MultiDelegate<bool> eventFocusChanged;
        MultiDelegate<bool> eventPressedChanged;
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventClick;
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventSingleClick;
        MultiDelegate<Vec2F const&, CursorInputEvent&> eventDoubleClick;

        MultiDelegate<CursorWheelInputEvent const&> eventCursorWheel;

    protected:

        //////////////////////////////////////////
        UIElement2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        inline void setFlag(bool _value, Flags _flag)
        {
            if (_value)
                m_flags |= _flag;
            else
                m_flags &= ~_flag;
        }

        //////////////////////////////////////////
        inline bool getFlag(Flags _flag) { return m_flags & _flag; }

    protected:
        Transform2DPtr m_transform;
        Bounds2DPtr m_bounds;
        bool m_captureCursorHits;
        S32 m_flags;

        S32 m_cursorIndex;
        bool m_focused;
        bool m_pressed;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIElement2D_hpp_
//////////////////////////////////////////
