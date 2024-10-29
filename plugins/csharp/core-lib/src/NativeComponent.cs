using System;
using Maze;

namespace Maze
{
    public class NativeComponent : Component
    {

        public NativeComponent(IntPtr _nativeComponentPtr)
        {
            nativeComponentPtr = _nativeComponentPtr;
        }
    }
}
