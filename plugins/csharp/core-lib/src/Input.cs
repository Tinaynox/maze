using System;

namespace Maze
{
    public class Input
    {
        public static bool GetKeyState(KeyCode _keyCode)
        {
            return InternalCalls.GetKeyState((int)_keyCode);
        }

    }
}
