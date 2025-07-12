using System;

namespace Maze.Core
{
    public class Input
    {
        private Input() { }

        public static bool GetKeyState(KeyCode _keyCode)
        {
            return InternalCalls.GetKeyState((int)_keyCode);
        }

        public static Vec2F GetCursorPosition(int cursorId = 0)
        {
            InternalCalls.GetCursorPosition(cursorId, out Vec2F outPosition);
            return outPosition;
        }

        public static bool GetCursorButtonState(int cursorId, int buttonIndex)
        {
            return InternalCalls.GetCursorButtonState(cursorId, buttonIndex);
        }

        public static bool GetCursorButtonState(int buttonIndex)
        {
            return InternalCalls.GetCursorButtonState(0, buttonIndex);
        }
    }
}
