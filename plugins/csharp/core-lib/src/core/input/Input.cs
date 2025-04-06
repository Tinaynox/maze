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
