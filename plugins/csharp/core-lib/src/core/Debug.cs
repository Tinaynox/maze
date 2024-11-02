using System;

namespace Maze.Core
{
    public class Debug
    {
        public static void Log(string _text)
        {
            InternalCalls.MazeLog(_text);
        }

        public static void LogWarning(string _text)
        {
            InternalCalls.MazeLogWarning(_text);
        }

        public static void LogError(string _text)
        {
            InternalCalls.MazeLogError(_text);
        }

    }
}
