using System;
using System.Collections;
using System.Collections.Generic;

namespace Maze.Core
{
    public class Transform3D
        : NativeComponent
        , IEnumerable<Transform3D>
    {
        public Vec3F Position
        {
            get { InternalCalls.Transform3DGetPosition(NativeComponentPtr, out Vec3F pos); return pos; }
            set { InternalCalls.Transform3DSetPosition(NativeComponentPtr, ref value); }
        }

        public float X { get => Position.X; set { Vec3F v = Position; v.X = value; Position = v; } }
        public float Y { get => Position.Y; set { Vec3F v = Position; v.Y = value; Position = v; } }
        public float Z { get => Position.Z; set { Vec3F v = Position; v.Z = value; Position = v; } }

        public Vec3F WorldPosition
        {
            get { InternalCalls.Transform3DGetWorldPosition(NativeComponentPtr, out Vec3F pos); return pos; }
        }

        public Quaternion Rotation
        {
            get { InternalCalls.Transform3DGetRotation(NativeComponentPtr, out Quaternion rotation); return rotation; }
            set { InternalCalls.Transform3DSetRotation(NativeComponentPtr, ref value); }
        }

        public Vec3F Scale
        {
            get { InternalCalls.Transform3DGetScale(NativeComponentPtr, out Vec3F scale); return scale; }
            set { InternalCalls.Transform3DSetScale(NativeComponentPtr, ref value); }
        }

        public TMat Transform
        {
            get { InternalCalls.Transform3DGetLocalTransform(NativeComponentPtr, out TMat tm); return tm; }
            set { InternalCalls.Transform3DSetLocalTransform(NativeComponentPtr, value); }
        }

        public TMat WorldTransform
        {
            get { InternalCalls.Transform3DGetWorldTransform(NativeComponentPtr, out TMat tm); return tm; }
            set { InternalCalls.Transform3DSetWorldTransform(NativeComponentPtr, value); }
        }

        public int ChildrenCount => InternalCalls.Transform3DGetChildrenCount(NativeComponentPtr);

        public Transform3D(NativePtr nativeComponentPtr)
            : base(nativeComponentPtr)
        {  
        }

        public Transform3D GetChild(int index)
        {
            InternalCalls.Transform3DGetChild(NativeComponentPtr, index, out NativePtr outChildComponent);
            if (outChildComponent != null)
                return new Transform3D(outChildComponent);

            return null;
        }

        public IEnumerator<Transform3D> GetEnumerator()
        {
            for (int i = 0, k = ChildrenCount; i < k; i++)
                yield return GetChild(i);
        }

        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        public void Translate(Vec3F delta)
        {
            InternalCalls.Transform3DTranslate(NativeComponentPtr, ref delta);
        }

        public void Rotate(Vec3F axis, float angle)
        {
            InternalCalls.Transform3DRotate(NativeComponentPtr, ref axis, angle);
        }

        public void SetParent(Transform3D parent)
        {
            InternalCalls.Transform3DSetParent(NativeComponentPtr, parent.NativeComponentPtr);
        }
    }
}
