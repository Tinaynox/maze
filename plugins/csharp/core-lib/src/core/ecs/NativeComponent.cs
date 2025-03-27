using System;
using Maze;

namespace Maze.Core
{
    public class NativeComponent : Component
    {
        public NativeComponent(NativePtr nativeComponentPtr)
        {
            NativeComponentPtr = nativeComponentPtr;
        }
    }
}
