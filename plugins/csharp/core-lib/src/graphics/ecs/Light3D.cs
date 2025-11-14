using Maze.Core;

namespace Maze.Graphics
{
    public class Light3D : NativeComponent
    {
        public float Radius
        {
            get { InternalCalls.Light3DGetRadius(NativeComponentPtr, out float value); return value; }
            set { InternalCalls.Light3DSetRadius(NativeComponentPtr, value); }
        }

        public Light3D(NativePtr _nativeComponentPtr)
            : base(_nativeComponentPtr)
        {
        }
    }
}
