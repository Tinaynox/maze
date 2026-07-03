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

        // Live high-resolution timer, unlike GetAppTime which is frame-locked.
        // Wraps around every ~71.6 minutes; unsigned subtraction of two samples stays correct across a single wrap.
        public static uint GetTimeMicroseconds()
        {
            return InternalCalls.GetTimeMicroseconds();
        }
    }
}
