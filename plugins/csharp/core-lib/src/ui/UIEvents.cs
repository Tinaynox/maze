
using Maze.Core;

namespace Maze.UI
{
    public class UIElementCursorMoveInEvent : NativeEvent
    {
        public Vec2F positionOS;
        public CursorInputEvent inputEvent;
    }

    public class UIElementSingleClickEvent : NativeEvent
    {
        public Vec2F positionOS;
        public CursorInputEvent inputEvent;
    }

    public class UIElementCursorPressInEvent : NativeEvent
    {
        public Vec2F positionOS;
        public CursorInputEvent inputEvent;
    }

    public class UIElementCursorReleaseEvent : NativeEvent
    {
        public CursorInputEvent inputEvent;
    }

    public class ButtonClickEvent : NativeEvent
    {
        public int buttonEid;
        public Vec2F positionOS;
        public CursorInputEvent inputEvent;
    }

}
