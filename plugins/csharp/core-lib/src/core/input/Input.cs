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

    }
}
