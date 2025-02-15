using System;

namespace Maze.Core
{
    public class Debug
    {
        public static void Log(string text)
        {
            InternalCalls.MazeLog(text);
        }

        public static void LogWarning(string text)
        {
            InternalCalls.MazeLogWarning(text);
        }

        public static void LogError(string text)
        {
            InternalCalls.MazeLogError(text);
        }

    }
}
