using System;

namespace Maze
{
    public class Transform3D : NativeComponent
    {
        public Vec3F position
        {
            get { InternalCalls.Transform3DGetPosition(nativeComponentPtr, out Vec3F pos); return pos; }
            set { InternalCalls.Transform3DSetPosition(nativeComponentPtr, value); }
        }

        public Transform3D(IntPtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {  
        }

        public void Translate(Vec3F _delta)
        {
            InternalCalls.Transform3DTranslate(nativeComponentPtr, _delta);
        }
    }
}
