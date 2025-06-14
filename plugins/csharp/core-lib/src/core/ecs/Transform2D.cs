using System;
using System.Collections;
using System.Collections.Generic;

namespace Maze.Core
{
    public class Transform2D
        : NativeComponent
        , IEnumerable<Transform2D>
    {
        public int ChildCount
        {
            get => InternalCalls.Transform2DGetChildCount(NativeComponentPtr);
        }

        public Transform2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void SetParent(Transform2D parent)
        {
            // InternalCalls.Transform2DSetParent(NativeComponentPtr, parent.NativeComponentPtr);
        }

        public Transform2D GetChild(int index)
        {
            InternalCalls.Transform2DGetChild(NativeComponentPtr, index, out NativePtr outChildComponent);
            if (outChildComponent != null)
                return new Transform2D(outChildComponent);

            return null;
        }

        public IEnumerator<Transform2D> GetEnumerator()
        {
            for (int i = 0, k = ChildCount; i < k; i++)
                yield return GetChild(i);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
