using System;
using Maze;

namespace Maze.Core
{
    public class NativeComponent : Component
    {

        public NativeComponent(IntPtr _nativeComponentPtr)
        {
            nativeComponentPtr = _nativeComponentPtr;
        }
    }
}
