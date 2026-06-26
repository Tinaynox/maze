using System;
using Maze.Core;

namespace Maze.Core
{
    public static class CoreHelper
    {
        public static void SetSystemCursorVisible(bool visible)
        {
            InternalCalls.SetSystemCursorVisible(visible);
        }

        public static float GetAppTime()
        {
            return InternalCalls.GetAppTime();
        }
    }
}
