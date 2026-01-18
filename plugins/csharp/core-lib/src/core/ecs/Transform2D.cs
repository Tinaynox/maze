using System;
using System.Collections;
using System.Collections.Generic;

namespace Maze.Core
{
    public class Transform2D
        : NativeComponent
        , IEnumerable<Transform2D>
    {
        public Vec2F Size
        {
            get { InternalCalls.Transform2DGetSize(NativeComponentPtr, out Vec2F size); return size; }
            set { InternalCalls.Transform2DSetSize(NativeComponentPtr, ref value); }
        }

        public float Width { get => Size.X; set { Vec2F v = Size; v.X = value; Size = v; } }
        public float Height { get => Size.Y; set { Vec2F v = Size; v.Y = value; Size = v; } }

        public Vec2F Position
        {
            get { InternalCalls.Transform2DGetPosition(NativeComponentPtr, out Vec2F pos); return pos; }
            set { InternalCalls.Transform2DSetPosition(NativeComponentPtr, ref value); }
        }

        public float X { get => Position.X; set { Vec2F v = Position; v.X = value; Position = v; } }
        public float Y { get => Position.Y; set { Vec2F v = Position; v.Y = value; Position = v; } }

        public Vec2F Scale
        {
            get { InternalCalls.Transform2DGetScale(NativeComponentPtr, out Vec2F scale); return scale; }
            set { InternalCalls.Transform2DSetScale(NativeComponentPtr, ref value); }
        }

        public TMat Transform
        {
            get { InternalCalls.Transform2DGetLocalTransform(NativeComponentPtr, out TMat tm); return tm; }
            set { InternalCalls.Transform2DSetLocalTransform(NativeComponentPtr, value); }
        }

        public TMat WorldTransform
        {
            get { InternalCalls.Transform2DGetWorldTransform(NativeComponentPtr, out TMat tm); return tm; }
        }

        public int ChildrenCount => InternalCalls.Transform2DGetChildrenCount(NativeComponentPtr);

        public Transform2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void SetParent(Transform2D parent)
        {
            // #TODO: Implement
            Debug.LogWarning("Transform2D.SetParent not implemented");
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
            for (int i = 0, k = ChildrenCount; i < k; i++)
                yield return GetChild(i);
        }

        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    }
}
