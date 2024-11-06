using System;

namespace Maze.Core
{
    public class Transform3D : NativeComponent
    {
        public Vec3F position
        {
            get { InternalCalls.Transform3DGetPosition(NativeComponentPtr, out Vec3F pos); return pos; }
            set { InternalCalls.Transform3DSetPosition(NativeComponentPtr, value); }
        }

        public Transform3D(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

        public void Translate(Vec3F _delta)
        {
            InternalCalls.Transform3DTranslate(NativeComponentPtr, _delta);
        }

        public void Rotate(Vec3F _axis, float _angle)
        {
            InternalCalls.Transform3DRotate(NativeComponentPtr, _axis, _angle);
        }
    }
}
