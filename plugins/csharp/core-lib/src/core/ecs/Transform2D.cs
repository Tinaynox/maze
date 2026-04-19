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

        public Vec2F Anchor
        {
            get { InternalCalls.Transform2DGetAnchor(NativeComponentPtr, out Vec2F anchor); return anchor; }
            set { InternalCalls.Transform2DSetAnchor(NativeComponentPtr, ref value); }
        }

        public Vec2F Pivot
        {
            get { InternalCalls.Transform2DGetPivot(NativeComponentPtr, out Vec2F pivot); return pivot; }
            set { InternalCalls.Transform2DSetPivot(NativeComponentPtr, ref value); }
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

        public Transform2D Parent
        {
            get => GetParent();
            set => SetParent(value);
        }

        public int ChildrenCount => InternalCalls.Transform2DGetChildrenCount(NativeComponentPtr);

        public Transform2D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {
        }

        public void SetParent(Transform2D parent)
        {
            InternalCalls.Transform2DSetParent(NativeComponentPtr, parent.NativeComponentPtr);
        }

        public Transform2D GetParent()
        {
            NativePtr nativePtr = InternalCalls.Transform2DGetParent(NativeComponentPtr);
            if (nativePtr == NativePtr.Zero)
                return null;

            return new Transform2D(nativePtr);
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

        public Vec2F[] CalculateLocalCorners()
        {
            Vec2F[] corners = new Vec2F[4];

            Vec2F size = Size;
            TMat tm = Transform;
            corners[0] = tm.Transform(Vec2F.Zero);
            corners[1] = tm.Transform(new Vec2F(size.X, 0.0f));
            corners[2] = tm.Transform(size);
            corners[3] = tm.Transform(new Vec2F(0.0f, size.Y));

            return corners;
        }

        public Vec2F[] CalculateWorldCorners()
        {
            Vec2F[] corners = new Vec2F[4];

            Vec2F size = Size;
            TMat tm = WorldTransform;
            corners[0] = tm.Transform(Vec2F.Zero);
            corners[1] = tm.Transform(new Vec2F(size.X, 0.0f));
            corners[2] = tm.Transform(size);
            corners[3] = tm.Transform(new Vec2F(0.0f, size.Y));

            return corners;
        }

        public AABB2D CalculateWorldAABB()
        {
            Vec2F[] corners = CalculateWorldCorners();
            AABB2D aabb = new AABB2D(corners[0]);
            for (int i = 1; i < 4; i++)
                aabb.ApplyUnion(corners[i]);
            return aabb;
        }

        public TComponent GetFirstTrunkComponent<TComponent>()
            where TComponent : class
        {
            Transform2D node = this;
            while (node != null)
            {
                TComponent component = node.Entity.GetComponent<TComponent>();
                if (component != null)
                    return component;
                node = node.Parent;
            }
            return null;
        }

        public TComponent GetLastTrunkComponent<TComponent>()
            where TComponent : class
        {
            TComponent result = null;
            Transform2D node = this;
            while (node != null)
            {
                TComponent component = node.Entity.GetComponent<TComponent>();
                if (component != null)
                    result = component;
                node = node.Parent;
            }
            return result;
        }
    }
}
