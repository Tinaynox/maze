using System;

namespace Maze.Core
{
    public class Transform2D : NativeComponent
    {
        public Transform2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void SetParent(Transform2D parent)
        {
            // InternalCalls.Transform2DSetParent(NativeComponentPtr, parent.NativeComponentPtr);
        }
    }
}
