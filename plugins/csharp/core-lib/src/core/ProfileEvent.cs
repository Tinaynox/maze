using System;

namespace Maze.Core
{
    // Marks a profiling scope visible in Optick. Usage:
    //     using (new ProfileEvent("MyScope"))
    //     {
    //         ...
    //     }
    // Must be Disposed on the same thread it was created on.
    public struct ProfileEvent : IDisposable
    {
        public ProfileEvent(string name)
        {
            InternalCalls.MazeProfileEventBegin(name);
        }

        public void Dispose()
        {
            InternalCalls.MazeProfileEventEnd();
        }
    }
}
