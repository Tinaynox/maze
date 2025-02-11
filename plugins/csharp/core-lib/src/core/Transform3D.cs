using System;

namespace Maze.Core
{
    public class Transform3D : NativeComponent
    {
        public Vec3F Position
        {
            get { InternalCalls.Transform3DGetPosition(NativeComponentPtr, out Vec3F pos); return pos; }
            set { InternalCalls.Transform3DSetPosition(NativeComponentPtr, value); }
        }

        public float X { get => Position.X; set { Vec3F v = Position; v.X = value; Position = v; } }
        public float Y { get => Position.Y; set { Vec3F v = Position; v.Y = value; Position = v; } }
        public float Z { get => Position.Z; set { Vec3F v = Position; v.Z = value; Position = v; } }

        public Quaternion Rotation
        {
            get { InternalCalls.Transform3DGetRotation(NativeComponentPtr, out Quaternion rotation); return rotation; }
            set { InternalCalls.Transform3DSetRotation(NativeComponentPtr, value); }
        }

        public Vec3F Scale
        {
            get { InternalCalls.Transform3DGetScale(NativeComponentPtr, out Vec3F scale); return scale; }
            set { InternalCalls.Transform3DSetScale(NativeComponentPtr, value); }
        }


        public Transform3D(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

        public TMat GetLocalTransform()
        {
            InternalCalls.Transform3DGetLocalTransform(NativeComponentPtr, out TMat tm);
            return tm;
        }

        public TMat GetWorldTransform()
        {
            InternalCalls.Transform3DGetWorldTransform(NativeComponentPtr, out TMat tm);
            return tm;
        }

        public void Translate(Vec3F _delta)
        {
            InternalCalls.Transform3DTranslate(NativeComponentPtr, _delta);
        }

        public void Rotate(Vec3F _axis, float _angle)
        {
            InternalCalls.Transform3DRotate(NativeComponentPtr, _axis, _angle);
        }

        public void SetParent(Transform3D _parent)
        {
            InternalCalls.Transform3DSetParent(NativeComponentPtr, _parent.NativeComponentPtr);
        }
    }
}
